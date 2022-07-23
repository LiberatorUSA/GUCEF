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
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientChannel::~CPubSubClientChannel()
{GUCEF_TRACE;

    Clear();
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

void
CPubSubClientChannel::PublishChannelMetrics( void ) const
{GUCEF_TRACE;

    TPubSubClientSidePtrVector::const_iterator i = m_sides.begin();
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

bool
CPubSubClientChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    // In case we are retrying we might have state from a previous run
    // clear it first
    Clear();

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
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskStart: Aborting because side with id " + sideId + " failed LoadConfig" );
            return false;
        }

        m_sides.push_back( side );
        ++c;
    }
    
    // Build the flow router's network based on available sides
    if ( !m_flowRouter.BuildRoutes( m_channelSettings.flowRouterConfig ,
                                    m_sides                            ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskStart: Aborting because we failed to build the flow router's routes" );
        return false;
    }

    // Now get the sides themselves started to kick things off
    TPubSubClientSidePtrVector::iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        CPubSubClientSidePtr& side = (*i);
        if ( !side->IsRunningInDedicatedThread() )
        {
            if ( !side->OnTaskStart( taskData ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskStart: Aborting because non-dedicated-thread side with id " + side->GetSideId() + " failed its own OnTaskStart" );
                return false;
            }
        }
        else
        {
            CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
            if ( !threadPool->StartTask( side ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskStart: Failed to start dedicated thread for other side. Falling back to a single thread" );
                
                // As a fallback we support trying run as part of the channel thread instead

                CPubSubSideChannelSettings& sideSettings = side->GetSideSettings();
                sideSettings.performPubSubInDedicatedThread = false;

                if ( !side->OnTaskStart( taskData ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskStart: Aborting because non-dedicated-thread (fallback mode) side with id " + side->GetSideId() + " failed its own OnTaskStart" );
                    return false;
                }
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel:OnTaskStart: Successfully requested the launch of a dedicated thread for side with id " + side->GetSideId() );
            }
        }

        ++i;
    }

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


