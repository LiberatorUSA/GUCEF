/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#include "gucefPUBSUB_CPubSubGlobal.h"
#define GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#endif /* GUCEF_PUBSUB_CPUBSUBGLOBAL_H ? */

#ifndef GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#include "gucefPUBSUB_CBasicPubSubMsg.h"
#define GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#endif /* GUCEF_PUBSUB_CBASICPUBSUBMSG_H ? */

#include "gucefPUBSUB_CPubSubClientChannel.h"

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE                  10000

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubClientChannel::CPubSubClientChannel( void )
    : CORE::CTaskConsumer()
    , m_sides()
    , m_flowRouter()
    , m_isInitialized( false )
    , m_globalConfigLoadCompleted( false )
    , m_metricsTimer( CORE::PulseGeneratorPtr(), 1000 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientChannel::~CPubSubClientChannel()
{GUCEF_TRACE;

    Clear();
    UnsubscribeAllFromObserver( true );
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::RegisterEventHandlers( void )
{GUCEF_TRACE;
    
    TEventCallback callback( this, &CPubSubClientChannel::OnGlobalConfigLoadStarted );
    SubscribeTo( &CORE::CCoreGlobal::Instance()->GetConfigStore()  ,
                 CORE::CConfigStore::GlobalConfigLoadStartingEvent ,
                 callback                                          );
    TEventCallback callback2( this, &CPubSubClientChannel::OnGlobalConfigLoadCompleted );
    SubscribeTo( &CORE::CCoreGlobal::Instance()->GetConfigStore()   ,
                 CORE::CConfigStore::GlobalConfigLoadCompletedEvent ,
                 callback2                                          );
    m_globalConfigLoadCompleted = CORE::CCoreGlobal::Instance()->GetConfigStore().IsGlobalConfigLoaded(); 

    m_metricsTimer.SetPulseGenerator( GetPulseGenerator() );
    TEventCallback callback3( this, &CPubSubClientChannel::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback3                      );    
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::Clear( void )
{GUCEF_TRACE;

    // First stop all the threads for sides that have their own
    TPubSubClientSidePtrVector::iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        CPubSubClientSidePtr side = (*i);
        CPubSubSideChannelSettings& sideSettings = side->GetSideSettings();
        if ( sideSettings.performPubSubInDedicatedThread )
        {
            side->RequestTaskToStop( true );
        }
        else
        {
            // The side is linked to our task delegator
            // lets make sure we decouple it
            side->SetTaskDelegator( TTaskDelegatorBasicPtr() );
        }
        ++i;
    }

    // the flow router has a map with shared pointers to sides so we should clear that first
    m_flowRouter.ClearRoutes();

    // Now get rid of the actual side providers
    m_sides.clear();
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::IsHealthy( void ) const
{GUCEF_TRACE;

    TPubSubClientSidePtrVector::const_iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        if ( !(*i)->IsHealthy() )
            return false;
        ++i;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientChannel::InitializeChannel( bool force )
{GUCEF_TRACE;

    if ( m_isInitialized && !force )
        return true;
    
    if ( !m_globalConfigLoadCompleted )
    {
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubClientChannel:InitializeChannel: Deferring initialization because global config loading has not yet been completed" );
        return false;
    }
    
    // In case we are retrying we might have state from a previous run
    // clear it first
    Clear();

    m_metricsTimer.SetInterval( m_channelSettings.metricsIntervalInMs );
    m_metricsTimer.SetEnabled( m_channelSettings.collectMetrics );

    CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetOrCreateThreadPool( "PubSubClientChannel(" + CORE::ToString( this ) + ")", GetPulseGenerator(), true );

    // Create a side for every side config entry
    CPubSubChannelSettings::TStringToPubSubSideChannelSettingsMap::const_iterator c = m_channelSettings.pubSubSideChannelSettingsMap.begin();
    while ( c != m_channelSettings.pubSubSideChannelSettingsMap.end() )
    {
        const CORE::CString& sideId = (*c).first;
        CPubSubSideChannelSettings sideSettings = (*c).second;
        if ( sideSettings.pubsubIdPrefix.IsNULLOrEmpty() )
            sideSettings.pubsubIdPrefix = CORE::ToString( m_channelSettings.channelId );

        CPubSubClientSidePtr side( new CPubSubClientSide( sideId, &m_flowRouter ) );        
        if ( !side->LoadConfig( sideSettings ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:InitializeChannel: Aborting because side with id " + sideId + " failed LoadConfig" );
            return false;
        }
        
        // Ensure pulse generator access ahead of client setup        
        if ( side->IsRunningInDedicatedThread() )
        {            
            // Perform task setup
            // This causes a thread delegator to be associated and as such a pulse generator made accessible
            if ( !threadPool->SetupTask( side ) )
            {
                // As a fallback we support trying run as part of the channel thread instead
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:InitializeChannel: Failed to setup dedicated thread for side with id " + side->GetSideId() + ". Falling back to using main channel thread" );
                CPubSubSideChannelSettings& sideSettings = side->GetSideSettings();
                sideSettings.performPubSubInDedicatedThread = false;
                side->SetTaskDelegator( GetTaskDelegator() );
            }
        }
        else
        {
            // run as part of the channel thread
            // we need to make the channel's delegator available to correctly link up 
            side->SetTaskDelegator( GetTaskDelegator() );
        }

        if ( !side->PerformPubSubClientSetup() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:InitializeChannel: Aborting because side with id " + sideId + " failed to setup pubsub client" );
            return false;
        }

        m_sides.push_back( side );
        ++c;
    }

    // Hook up the flow router to the channel's pulse generator thus sharing the thread
    m_flowRouter.SetPulseGenerator( GetPulseGenerator() );
    
    // Build the flow router's network based on available sides
    if ( !m_flowRouter.BuildRoutes( m_channelSettings.flowRouterConfig ,
                                    m_sides                            ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:InitializeChannel: Aborting because we failed to build the flow router's routes" );
        return false;
    }

    // Now get the sides themselves started to kick things off
    TPubSubClientSidePtrVector::iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        CPubSubClientSidePtr& side = (*i);
        if ( !side->IsRunningInDedicatedThread() )
        {
            if ( !side->OnTaskStart( GUCEF_NULL ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:InitializeChannel: Aborting because non-dedicated-thread side with id " + side->GetSideId() + " failed its own OnTaskStart" );
                return false;
            }
        }
        else
        {
            if ( !threadPool->StartTask( side ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:InitializeChannel: Failed to start dedicated thread for other side. Falling back to a single thread" );
                
                // As a fallback we support trying run as part of the channel thread instead

                CPubSubSideChannelSettings& sideSettings = side->GetSideSettings();
                sideSettings.performPubSubInDedicatedThread = false;

                if ( !side->OnTaskStart( GUCEF_NULL ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:InitializeChannel: Aborting because non-dedicated-thread (fallback mode) side with id " + side->GetSideId() + " failed its own OnTaskStart" );
                    return false;
                }
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel:InitializeChannel: Successfully requested the launch of a dedicated thread for side with id " + side->GetSideId() );
            }
        }

        ++i;
    }

    m_isInitialized = true;
    return true;

}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    RegisterEventHandlers();

    InitializeChannel( true );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    bool allDone = false;
    TPubSubClientSidePtrVector::iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        CPubSubClientSidePtr& side = (*i);
        if ( !side->IsRunningInDedicatedThread() )
        {
            // This side does not have its own thread
            // we will let it use the channel's thread
            allDone = side->OnTaskCycle( taskData ) || allDone;
        }

        ++i;
    }

    return allDone;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnding( CORE::CICloneable* taskdata ,
                                    bool willBeForced           )
{GUCEF_TRACE;

    TPubSubClientSidePtrVector::iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        CPubSubClientSidePtr& side = (*i);
        if ( !side->IsRunningInDedicatedThread() )
        {
            // This side does not have its own thread
            // we will have let it use the channel's thread so its fate was tied wrt OnTaskEnded timing
            side->OnTaskEnding( taskdata, willBeForced );
        }
        else
        {
            // Since we are the ones that launched the dedicated thread for this sides we should also ask
            // to have it cleaned up when we are shutting down the main channel thread
            CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
            if ( !threadPool->RequestTaskToStop( side.StaticCast< CORE::CTaskConsumer >(), false ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskEnding: Failed to request the dedicated thread for side " + side->GetSideId() + " to stop" );
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel:OnTaskEnding: Successfully requested the dedicated thread for the side " + side->GetSideId() + " to stop" );
            }
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnded( CORE::CICloneable* taskData ,
                                   bool wasForced              )
{GUCEF_TRACE;  

    TPubSubClientSidePtrVector::iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        CPubSubClientSidePtr& side = (*i);
        if ( !side->IsRunningInDedicatedThread() )
        {
            // This side does not have its own thread
            // we will have let it use the channel's thread so its fate was tied wrt OnTaskEnded timing
            side->OnTaskEnded( taskData, wasForced );
        }
        ++i;
    }

    Clear();
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnGlobalConfigLoadStarted( CORE::CNotifier* notifier    ,
                                                 const CORE::CEvent& eventId  ,
                                                 CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // We should wait for all platform systems to initialize since this is middleware and 
    // we dont know what platform systems the various backends depend on so we cannot get a head start
    // without potentially causing issues
    m_globalConfigLoadCompleted = false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnGlobalConfigLoadCompleted( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventId  ,
                                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // We should wait for all platform systems to initialize since this is middleware and 
    // we dont know what platform systems the various backends depend on so we cannot get a head start
    // without potentially causing issues
    m_globalConfigLoadCompleted = true;
    InitializeChannel( false );
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventId  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TPubSubClientSidePtrVector::iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        const CPubSubClientSide::StringToPubSubClientSideMetricsMap& sideMetrics = (*i)->GetSideMetrics();
        CPubSubClientSide::StringToPubSubClientSideMetricsMap::const_iterator n = sideMetrics.begin();
        while ( n != sideMetrics.end() )
        {
            const CORE::CString& metricFriendlyTopicName = (*n).first;
            const CPubSubClientSide::CPubSubClientSideMetrics& metrics = (*n).second;

            if ( metrics.hasSupportForPublishing )
            {
                GUCEF_METRIC_GAUGE( metricFriendlyTopicName + "publishedMsgsInFlight", metrics.publishedMsgsInFlight, 1.0f );
                GUCEF_METRIC_GAUGE( metricFriendlyTopicName + "publishOrAckFailedMsgs", metrics.publishOrAckFailedMsgs, 1.0f );
                GUCEF_METRIC_GAUGE( metricFriendlyTopicName + "lastPublishBatchSize", metrics.lastPublishBatchSize, 1.0f );
            }
            if ( metrics.hasSupportForSubscribing )
            {
                GUCEF_METRIC_GAUGE( metricFriendlyTopicName + "queuedReceiveSuccessAcks", metrics.queuedReceiveSuccessAcks, 1.0f );
            }
            ++n;
        }

        ++i;
    }
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientChannel::GetType( void ) const
{GUCEF_TRACE;

    return "PubSubClientChannel";
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::LoadConfig( const CPubSubChannelSettings& channelSettings )
{GUCEF_TRACE;

    m_channelSettings = channelSettings;
    return m_flowRouter.LoadConfig( m_channelSettings.flowRouterConfig );
}

/*-------------------------------------------------------------------------*/

const CPubSubChannelSettings&
CPubSubClientChannel::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


