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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#include "gucefPUBSUB_CPubSubGlobal.h"
#define GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#endif /* GUCEF_PUBSUB_CPUBSUBGLOBAL_H ? */

#ifndef GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#include "gucefPUBSUB_CBasicPubSubMsg.h"
#define GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#endif /* GUCEF_PUBSUB_CBASICPUBSUBMSG_H ? */

#ifndef GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefWEB_CDummyHTTPServerResource.h"
#define GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

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
#define GUCEF_DEFAULT_PUBSUB_RECONNECT_DELAY_IN_MS                  100
#define GUCEF_DEFAULT_PUBSUB_MAX_PUBLISHED_MSG_INFLIGHT_TIME_IN_MS  ( 30 * 1000 )
#define GUCEF_DEFAULT_PUBSUB_SIDE_MAX_IN_FLIGHT                     1000

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubClientChannel::CPubSubClientChannel( void )
    : CPubSubClientSide( 'A' )
    , m_sideBPubSub()
    , m_sides()
{GUCEF_TRACE;

    // for now just work with a and b, we can do more sides later
    m_sideBPubSub = CPubSubClientOtherSidePtr( new CPubSubClientOtherSide( this, 'B' ) );

    m_sides.push_back( this );
    m_sides.push_back( m_sideBPubSub.GetPointerAlways() );
}

/*-------------------------------------------------------------------------*/

CPubSubClientChannel::~CPubSubClientChannel()
{GUCEF_TRACE;

    m_sideBPubSub.Unlink();
    m_sides.clear();
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::GetAllSides( TPubSubClientSideVector*& sides )
{GUCEF_TRACE;

    sides = &m_sides;
    return true;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::PublishChannelMetrics( void ) const
{GUCEF_TRACE;

    TPubSubClientSideVector::const_iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        const StringToPubSubClientSideMetricsMap& sideMetrics = (*i)->GetSideMetrics();
        StringToPubSubClientSideMetricsMap::const_iterator n = sideMetrics.begin();
        while ( n != sideMetrics.end() )
        {
            const CORE::CString& metricFriendlyTopicName = (*n).first;
            const CPubSubClientSideMetrics& metrics = (*n).second;

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
CPubSubClientChannel::IsTrackingInFlightPublishedMsgsForAcksNeeded( void )
{GUCEF_TRACE;

    // Right now we dont have config driven data flows yet
    // every side flows to every side.
    // As such we need tracking as soon as 1 side needs a subscriber ack
   
    TPubSubClientSideVector::const_iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        const CPubSubSideChannelSettings* sideSettings = (*i)->GetSideSettings();
        if ( GUCEF_NULL != sideSettings )
        {
            if ( (*i)->HasSubscribersNeedingAcks() )
                return true;
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::AcknowledgeReceiptForSide( CIPubSubMsg::TNoLockSharedPtr& msg ,
                                                 CPubSubClientSide* msgReceiverSide )
{GUCEF_TRACE;

    return AcknowledgeReceiptForSideImpl( GetDelegatorThreadId(), msg, msgReceiverSide );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::AcknowledgeReceiptForSideImpl( CORE::UInt32 invokerThreadId       ,
                                                     CIPubSubMsg::TNoLockSharedPtr& msg ,
                                                     CPubSubClientSide* msgReceiverSide )
{GUCEF_TRACE;

    // if we only have 2 sides, no need for anything more complicated
    if ( m_sides.size() <= 2 )
    {
        CPubSubClientTopic* originTopic = msg->GetOriginClientTopic();
        if ( GUCEF_NULL != originTopic )
        {
            CPubSubClient* originClient = originTopic->GetClient();
            if ( GUCEF_NULL != originClient )
            {
                CPubSubClientSide* originSide = static_cast< CPubSubClientSide* >( originClient->GetOpaqueUserData() );
                if ( GUCEF_NULL != originSide )
                {
                    if ( invokerThreadId != originSide->GetDelegatorThreadId() )
                        return originSide->AcknowledgeReceiptASync( msg );
                    else
                        return originSide->AcknowledgeReceiptSync( msg );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since parent client owning the origin topic provided on the message does not have a link back to the owner pubsub side. This should never happen." );
                    return false;
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since no parent client owns the origin topic provided on the message. This should never happen." );
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since no origin topic was provided on the message. Check your config and backend feature compatibility" );
            return false;
        }
    }
    else
    {
        // @TODO: support for more than 2 sides is not implemented yet
            // The trickyness in this case is that you have to wait to ack until all sides have
            // acked (or perhaps just 1 based on config?)
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    if ( CPubSubClientSide::OnTaskStart( taskData ) )
    {
        if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
        {
            return m_sideBPubSub->OnTaskStart( taskData );
        }
        else
        {
            CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
            if ( !threadPool->StartTask( m_sideBPubSub ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskStart: Failed to start dedicated thread for other side. Falling back to a single thread" );
                CPubSubSideChannelSettings* sideSettings = m_sideBPubSub->GetSideSettings();
                if ( GUCEF_NULL != sideSettings )
                    sideSettings->performPubSubInDedicatedThread = false;
                else
                    return false;

                return m_sideBPubSub->OnTaskStart( taskData );
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel:OnTaskStart: Successfully requested the launch of a dedicated thread for other side" );
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskCycle( taskData );

    if ( !m_sideBPubSub.IsNULL() )
    {
        if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
        {
            m_sideBPubSub->OnTaskCycle( taskData );
        }
    }

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnding( CORE::CICloneable* taskdata ,
                                    bool willBeForced           )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskEnding( taskdata, willBeForced );

    if ( !m_sideBPubSub.IsNULL() )
    {
        if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
        {
            m_sideBPubSub->OnTaskEnding( taskdata, willBeForced );
        }
        else
        {
            // Since we are the ones that launched the dedicated thread for the other sides we should also ask
            // to have it cleaned up when we are shutting down this thread
            CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
            if ( !threadPool->RequestTaskToStop( m_sideBPubSub.StaticCast< CORE::CTaskConsumer >(), false ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskEnding: Failed to request the dedicated thread for other side to stop" );
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel:OnTaskEnding: Successfully requested the dedicated thread for the other side to stop" );
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnded( CORE::CICloneable* taskData ,
                                   bool wasForced              )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskEnded( taskData, wasForced );

    if ( !m_sideBPubSub.IsNULL() )
    {
        if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
        {
            m_sideBPubSub->OnTaskEnded( taskData, wasForced );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::LoadConfig( const CPubSubChannelSettings& channelSettings )
{GUCEF_TRACE;

    return CPubSubClientSide::LoadConfig( channelSettings ) && m_sideBPubSub->LoadConfig( channelSettings );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


