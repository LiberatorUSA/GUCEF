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

#ifndef GUCEF_PUBSUB_CPUBSUBFLOWROUTER_H
#include "gucefPUBSUB_CPubSubFlowRouter.h"
#define GUCEF_PUBSUB_CPUBSUBFLOWROUTER_H
#endif /* GUCEF_PUBSUB_CPUBSUBFLOWROUTER_H ? */

#include "gucefPUBSUB_CPubSubClientSide.h"

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

const CORE::CEvent CPubSubClientSide::HealthStatusChangeEvent = "GUCEF::PUBSUB::CPubSubClientSide::HealthStatusChangeEvent";
const CORE::CEvent CPubSubClientSide::PubSubClientInstantiationEvent = "GUCEF::PUBSUB::CPubSubClientSide::PubSubClientInstantiationEvent";

#define GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE                  10000
#define GUCEF_DEFAULT_PUBSUB_RECONNECT_DELAY_IN_MS                  100
#define GUCEF_DEFAULT_PUBSUB_MAX_PUBLISHED_MSG_INFLIGHT_TIME_IN_MS  ( 30 * 1000 )
#define GUCEF_DEFAULT_PUBSUB_SIDE_MAX_IN_FLIGHT                     1000

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void 
CPubSubClientSide::RegisterEvents( void )
{GUCEF_TRACE;

    HealthStatusChangeEvent.Initialize();
    PubSubClientInstantiationEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::CPubSubClientSide( const CORE::CString& sideId   ,
                                      CPubSubFlowRouter* flowRouter )
    : CORE::CTaskConsumer()
    , m_pubsubClient()
    , m_clientFeatures()
    , m_pubsubBookmarkPersistence()
    , m_bookmarkNamespace()
    , m_topics()
    , m_metricsMap()
    , m_sideSettings()
    , m_mailbox()
    , m_metricsTimer( GUCEF_NULL )
    , m_pubsubClientReconnectTimer( GUCEF_NULL )
    , m_timedOutInFlightMessagesCheckTimer( GUCEF_NULL )
    , m_sideId( sideId )
    , m_awaitingFailureReport( false )
    , m_totalMsgsInFlight( 0 )
    , m_flowRouter( flowRouter )
    , m_isHealthy( true )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::~CPubSubClientSide()
{GUCEF_TRACE;

    DisconnectPubSubClient( true );

    delete m_timedOutInFlightMessagesCheckTimer;
    m_timedOutInFlightMessagesCheckTimer = GUCEF_NULL;

    delete m_pubsubClientReconnectTimer;
    m_pubsubClientReconnectTimer = GUCEF_NULL;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CPubSubClientPtr
CPubSubClientSide::GetCurrentUnderlyingPubSubClient( void )
{GUCEF_TRACE;

    return m_pubsubClient;
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::TopicLink( void )
    : topic( GUCEF_NULL )
    , currentPublishActionIds()
    , inFlightMsgs()
    , publishFailedMsgs()
    , publishAckdMsgsMailbox()
    , metricFriendlyTopicName()
    , metrics( GUCEF_NULL )
    , lastBookmarkPersistSuccess( CORE::CDateTime::PastMax )
    , msgsSinceLastBookmarkPersist( 0 )
    , bookmarksOnMsgReceived()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::TopicLink( CPubSubClientTopic* t )
    : topic( t )
    , currentPublishActionIds()
    , inFlightMsgs()
    , publishFailedMsgs()
    , publishAckdMsgsMailbox()
    , metricFriendlyTopicName()
    , metrics( GUCEF_NULL )
    , lastBookmarkPersistSuccess( CORE::CDateTime::PastMax )
    , msgsSinceLastBookmarkPersist( 0 )
    , bookmarksOnMsgReceived()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::TopicLink::AddInFlightMsg( CORE::UInt64 publishActionId       ,
                                              CIPubSubMsg::TNoLockSharedPtr& msg )
{GUCEF_TRACE;

    inFlightMsgs[ publishActionId ] = MsgTrackingEntry( publishActionId, msg );
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::TopicLink::AddInFlightMsgs( const CPubSubClientTopic::TPublishActionIdVector& publishActionIds ,
                                               const CPubSubClientTopic::TIPubSubMsgSPtrVector& msgs              ,
                                               bool inFlightDefaultState                                          )
{GUCEF_TRACE;

    // this variation gets called during async flow
    size_t max = SMALLEST( publishActionIds.size(), msgs.size() );
    if ( publishActionIds.size() != msgs.size() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):TopicLink:AddInFlightMsgs: Nr of publishActionIds (" + CORE::ToString( publishActionIds.size() ) +
            ") does not match Nr of msgs (" + CORE::ToString( msgs.size() ) + "). Will proceed best effort but this will likely cause issues" );
    }

    for ( size_t i=0; i<max; ++i )
    {
        CIPubSubMsg::TNoLockSharedPtr msgPtr( msgs[ i ] );
        inFlightMsgs[ publishActionIds[ i ] ] = MsgTrackingEntry( publishActionIds[ i ], msgPtr, inFlightDefaultState );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::TopicLink::AddInFlightMsgs( const CPubSubClientTopic::TPublishActionIdVector& publishActionIds ,
                                               const CPubSubClientTopic::TPubSubMsgsRefVector& msgs               ,
                                               bool inFlightDefaultState                                          )
{GUCEF_TRACE;

    // this variation gets called during sync flow
    size_t max = SMALLEST( publishActionIds.size(), msgs.size() );
    if ( publishActionIds.size() != msgs.size() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):TopicLink:AddInFlightMsgs: Nr of publishActionIds (" + CORE::ToString( publishActionIds.size() ) +
            ") does not match Nr of msgs (" + CORE::ToString( msgs.size() ) + "). Will proceed best effort but this will likely cause issues" );
    }

    for ( size_t i=0; i<max; ++i )
    {
        // in the sync/blocking flow we have not yet made a copy, lifecycle is for the call chain which doesnt work
        // for keeping longer term references. As such we create a copy of the message here
        CIPubSubMsg::TNoLockSharedPtr msgPtr( static_cast< CIPubSubMsg* >( msgs[ i ]->Clone() ) );
        inFlightMsgs[ publishActionIds[ i ] ] = MsgTrackingEntry( publishActionIds[ i ], msgPtr, inFlightDefaultState );
    }
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::MsgTrackingEntry::MsgTrackingEntry( void )
    : retryCount( 0 )
    , firstPublishAttempt( CORE::CDateTime::Empty )
    , lastPublishAttempt( CORE::CDateTime::Empty )
    , publishActionId( 0 )
    , msg()
    , isInFlight( false )
    , waitingForInFlightConfirmation( false )
    , readyToAckPublishSuccessButAckFailed( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::MsgTrackingEntry::MsgTrackingEntry( CORE::UInt64 publishActionID      ,
                                                                 CIPubSubMsg::TNoLockSharedPtr& msg ,
                                                                 bool isInFlightState               )
    : retryCount( 0 )
    , firstPublishAttempt( CORE::CDateTime::NowUTCDateTime() )
    , lastPublishAttempt( CORE::CDateTime::Empty )
    , publishActionId( publishActionID )
    , msg( msg )
    , isInFlight( isInFlightState )
    , waitingForInFlightConfirmation( true )
    , readyToAckPublishSuccessButAckFailed( false )
{GUCEF_TRACE;

    lastPublishAttempt = firstPublishAttempt;
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::MsgTrackingEntry::MsgTrackingEntry( const MsgTrackingEntry& src )
    : retryCount( src.retryCount )
    , firstPublishAttempt( src.firstPublishAttempt )
    , lastPublishAttempt( src.lastPublishAttempt )
    , publishActionId( src.publishActionId )
    , msg( src.msg )
    , isInFlight( src.isInFlight )
    , waitingForInFlightConfirmation( src.waitingForInFlightConfirmation )
    , readyToAckPublishSuccessButAckFailed( src.readyToAckPublishSuccessButAckFailed )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::MsgTrackingEntry&
CPubSubClientSide::TopicLink::MsgTrackingEntry::operator=( const MsgTrackingEntry& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        retryCount = src.retryCount;
        firstPublishAttempt = src.firstPublishAttempt;
        lastPublishAttempt = src.lastPublishAttempt;
        publishActionId = src.publishActionId;
        msg = src.msg;
        isInFlight = src.isInFlight;
        waitingForInFlightConfirmation = src.waitingForInFlightConfirmation;
        readyToAckPublishSuccessButAckFailed = src.readyToAckPublishSuccessButAckFailed;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CPubSubClientSide::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );

    TEventCallback callback2( this, &CPubSubClientSide::OnCheckForTimedOutInFlightMessagesTimerCycle );
    SubscribeTo( m_timedOutInFlightMessagesCheckTimer ,
                 CORE::CTimer::TimerUpdateEvent       ,
                 callback2                            );

    TEventCallback callback3( this, &CPubSubClientSide::OnTopicsAccessAutoCreated );
    SubscribeTo( m_pubsubClient.GetPointerAlways()           ,
                 CPubSubClient::TopicsAccessAutoCreatedEvent ,
                 callback3                                   );

    TEventCallback callback4( this, &CPubSubClientSide::OnTopicsAccessAutoDestroyed );
    SubscribeTo( m_pubsubClient.GetPointerAlways()             ,
                 CPubSubClient::TopicsAccessAutoDestroyedEvent ,
                 callback4                                     );

    TEventCallback callback5( this, &CPubSubClientSide::OnClientHealthStatusChanged );
    SubscribeTo( m_pubsubClient.GetPointerAlways()      ,
                 CPubSubClient::HealthStatusChangeEvent ,
                 callback5                              );
    
    if ( GUCEF_NULL != m_pubsubClientReconnectTimer )
    {
        TEventCallback callback( this, &CPubSubClientSide::OnPubSubClientReconnectTimerCycle );
        SubscribeTo( m_pubsubClientReconnectTimer   ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::RegisterTopicEventHandlers( CPubSubClientTopic& topic )
{GUCEF_TRACE;

    TEventCallback callback( this, &CPubSubClientSide::OnPubSubTopicMsgsReceived );
    SubscribeTo( &topic                                ,
                 CPubSubClientTopic::MsgsRecievedEvent ,
                 callback                              );

    TEventCallback callback2( this, &CPubSubClientSide::OnPubSubTopicMsgsPublished );
    SubscribeTo( &topic                                 ,
                 CPubSubClientTopic::MsgsPublishedEvent ,
                 callback2                              );

    TEventCallback callback3( this, &CPubSubClientSide::OnPubSubTopicMsgsPublishFailure );
    SubscribeTo( &topic                                      ,
                 CPubSubClientTopic::MsgsPublishFailureEvent ,
                 callback3                                   );

    TEventCallback callback4( this, &CPubSubClientSide::OnPubSubTopicLocalPublishQueueFull );
    SubscribeTo( &topic                                         ,
                 CPubSubClientTopic::LocalPublishQueueFullEvent ,
                 callback4                                      );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientSide::GetType( void ) const
{GUCEF_TRACE;

    return "PubSubClientSide";
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::CPubSubClientSideMetrics::CPubSubClientSideMetrics( void )
    : publishedMsgsInFlight( 0 )
    , publishOrAckFailedMsgs( 0 )
    , lastPublishBatchSize( 0 )
    , queuedReceiveSuccessAcks( 0 )
    , hasSupportForPublishing( false )
    , hasSupportForSubscribing( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::CPubSubClientSideMetrics::CPubSubClientSideMetrics( const CPubSubClientSideMetrics& src )
    : publishedMsgsInFlight( src.publishedMsgsInFlight )
    , publishOrAckFailedMsgs( src.publishOrAckFailedMsgs )
    , lastPublishBatchSize( src.lastPublishBatchSize )
    , queuedReceiveSuccessAcks( src.queuedReceiveSuccessAcks )
    , hasSupportForPublishing( src.hasSupportForPublishing )
    , hasSupportForSubscribing( src.hasSupportForSubscribing )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::CPubSubClientSideMetrics::~CPubSubClientSideMetrics()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const
CPubSubClientSide::StringToPubSubClientSideMetricsMap&
CPubSubClientSide::GetSideMetrics( void ) const
{GUCEF_TRACE;

    return m_metricsMap;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientSide::GetPubSubClientSupportedFeatures( CPubSubClientFeatures& features ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    try
    {
        if ( !m_pubsubClient.IsNULL() && m_pubsubClient->GetSupportedFeatures( features ) )
            return true;
    }
    catch ( const std::exception& )
    {
        
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientSide::IsHealthy( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    // Aggregate the health status across all concerns
    bool fullyHealthy = true;

    try
    {
        if ( !m_pubsubClient.IsNULL() && !m_pubsubClient->IsHealthy() )
            fullyHealthy = false;
    }
    catch ( const std::exception& )
    {
        fullyHealthy = false;
    }

    // Notify if there was a change in status
    if ( fullyHealthy != m_isHealthy )
    {
        m_isHealthy = fullyHealthy;        

        if ( m_isHealthy )
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide:IsHealthy: overall health is now Ok for side with id " + m_sideId );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide:IsHealthy: overall health status is now unhealthy for side with id " + m_sideId );         
        }

        lock.EarlyUnlock();
        THealthStatusChangeEventData eData( fullyHealthy ); 
        NotifyObservers( HealthStatusChangeEvent, &eData );         
    }
    
    return fullyHealthy;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnClientHealthStatusChanged( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // (Re)determine the overall health status
    IsHealthy();
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::UpdateTopicMetrics( TopicLink& topicLink )
{GUCEF_TRACE;

    if ( GUCEF_NULL != topicLink.metrics )
    {
        if ( topicLink.metrics->hasSupportForPublishing )
        {
            topicLink.metrics->publishOrAckFailedMsgs = topicLink.publishFailedMsgs.size();
            topicLink.metrics->lastPublishBatchSize = topicLink.currentPublishActionIds.size();
            topicLink.metrics->publishedMsgsInFlight = topicLink.inFlightMsgs.size();
        }
        if ( topicLink.metrics->hasSupportForSubscribing )
        {
            topicLink.metrics->queuedReceiveSuccessAcks = topicLink.publishAckdMsgsMailbox.AmountOfMail();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                        const CORE::CEvent& eventId  ,
                                        CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        // We update topic metric numbers both on a timer cycle AND at various points
        // in the processing pipeline. The reasoning is that processing under certain load profiles
        // can cause the totality of thread cycle operations to exceed the timer cycle time
        // this would unintentionally reduce the number of metric updates obtained
        TopicLink& topicLink = (*i).second;
        UpdateTopicMetrics( topicLink );

        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnTopicsAccessAutoCreated( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CPubSubClient::TopicsAccessAutoCreatedEventData* eData = static_cast< CPubSubClient::TopicsAccessAutoCreatedEventData* >( eventData );
    if ( GUCEF_NULL != eData )
    {
        CPubSubClient::PubSubClientTopicSet& topicsAccess = *eData;
        CPubSubClient::PubSubClientTopicSet::iterator i = topicsAccess.begin();
        while ( i != topicsAccess.end() )
        {
            CPubSubClientTopic* tAccess = (*i);
            if ( GUCEF_NULL != tAccess )
            {
                if ( ConfigureTopicLink( m_sideSettings, *tAccess ) )
                {
                    ConnectPubSubClientTopic( *tAccess, m_clientFeatures, m_sideSettings );
                }
            }
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnTopicsAccessAutoDestroyed( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CPubSubClient::TopicsAccessAutoDestroyedEventData* eData = static_cast< CPubSubClient::TopicsAccessAutoDestroyedEventData* >( eventData );
    if ( GUCEF_NULL != eData )
    {
        CPubSubClient::PubSubClientTopicSet& topicsAccess = *eData;
        CPubSubClient::PubSubClientTopicSet::iterator i = topicsAccess.begin();
        while ( i != topicsAccess.end() )
        {
            CPubSubClientTopic* tAccess = (*i);
            if ( GUCEF_NULL != tAccess )
            {
                // @TODO: What to do about in-flight messages etc? Any special action?
                //      send them to dead letter ?
                m_topics.erase( tAccess );
            }
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubClientReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                                      const CORE::CEvent& eventId  ,
                                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // stop the timer, reconnect time itself should not count towards the reconnect interval
    m_pubsubClientReconnectTimer->SetEnabled( false );

    // Since the client does not support reconnects we will destructively reconnnect
    // Meaning we wipe out any previous client as we cannot rely on the client implementation
    // properly handling the state transition
    if ( DisconnectPubSubClient( true ) )
    {
        if ( ConnectPubSubClient() )
            return; // no need to resume the timer
    }

    // no joy, start the timer again
    m_pubsubClientReconnectTimer->SetEnabled( false );
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientSide::HasSubscribersNeedingAcks( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    // better safe then sorry...
    if ( m_pubsubClient.IsNULL() )
        return true;

    const CPubSubClientConfig& pubSubConfig = m_sideSettings.pubsubClientConfig;

    // Whether we need to track successfull message handoff (garanteed handling) depends both on whether we want that extra reliability per the config
    // (optional since nothing is free and this likely degrades performance a bit) but also whether the backend even supports it.
    // If the backend doesnt support it all we will be able to do between the sides is fire-and-forget
    
    bool doWeWantIt = ( pubSubConfig.desiredFeatures.supportsSubscribing &&                         // <- does it apply in this context ?
                        ( pubSubConfig.desiredFeatures.supportsSubscriberMsgReceivedAck ||          // <- do we want it?
                          pubSubConfig.desiredFeatures.supportsSubscribingUsingBookmark  )
                      );

    bool isItSupported = m_clientFeatures.supportsSubscriberMsgReceivedAck ||
                         ( m_clientFeatures.supportsBookmarkingConcept && m_clientFeatures.supportsSubscribingUsingBookmark );

    bool canWeNotWantIt = m_clientFeatures.supportsAbsentMsgReceivedAck &&          // <- Is it even an option to not do it regardless of desired features
                          ( !m_clientFeatures.supportsBookmarkingConcept ||         // <- if we need to perform client-side bookmarking then its not really an option to forgo acks if you want a reliable handoff and thus bookmark progression
                             m_clientFeatures.supportsBookmarkingConcept && m_clientFeatures.supportsSubscribingUsingBookmark && m_clientFeatures.supportsServerSideBookmarkPersistance );
                              
    return ( doWeWantIt && isItSupported ) || 
           ( !doWeWantIt && canWeNotWantIt && isItSupported );
}

/*-------------------------------------------------------------------------*/

template < typename TMsgCollection >
bool
CPubSubClientSide::PublishMsgsSync( const TMsgCollection& msgs )
{GUCEF_TRACE;

    CORE::UInt64 totalMsgsInFlight = 0;
    CORE::UInt32 topicsToPublishOn = 0;
    CORE::UInt32 topicsPublishedOn = 0;
    bool publishSuccess = true;
    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        CPubSubClientTopic* topic = topicLink.topic;

        if ( GUCEF_NULL != topic )
        {
            if ( topic->IsPublishingSupported() )
            {
                ++topicsToPublishOn;

                topicLink.currentPublishActionIds.clear();
                if ( topic->Publish( topicLink.currentPublishActionIds, msgs, m_sideSettings.needToTrackInFlightPublishedMsgsForAck ) )
                {
                    if ( m_sideSettings.needToTrackInFlightPublishedMsgsForAck )
                        topicLink.AddInFlightMsgs( topicLink.currentPublishActionIds, msgs, true );

                    ++topicsPublishedOn;
                }
                else
                {
                    publishSuccess = false;

                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):PublishMsgsSync: Failed to publish messages to topic" );

                    if ( m_sideSettings.retryFailedPublishAttempts )
                    {
                        topicLink.AddInFlightMsgs( topicLink.currentPublishActionIds, msgs, false );
                        m_awaitingFailureReport = true;
                    }
                }
            }

            totalMsgsInFlight += topicLink.inFlightMsgs.size();
        }

        UpdateTopicMetrics( topicLink );
        ++i;
    }

    if ( publishSuccess )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):PublishMsgsSync: Successfully published messages to " + CORE::ToString( topicsPublishedOn ) + " topics, " +
            CORE::ToString( topicsToPublishOn ) + " topics available for publishing" );
    }

    m_totalMsgsInFlight = totalMsgsInFlight;
    return publishSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::PublishMsgsASync( const CPubSubClientTopic::TPubSubMsgsRefVector& msgs )
{GUCEF_TRACE;

    // Add the messages in bulk to the mailbox. Since we use pointer semantics we are actually
    // Adding the IPubSubMsg* elements since the ref will be dereferenced
    return m_mailbox.AddPtrBulkMail< const CPubSubClientTopic::TPubSubMsgsRefVector >( msgs );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::PublishMsgs( const CPubSubClientTopic::TPubSubMsgsRefVector& msgs )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    // If we are running async from other sides we need to check the mailbox
    if ( m_sideSettings.performPubSubInDedicatedThread || m_awaitingFailureReport )
    {
        return PublishMsgsASync( msgs );
    }
    else
    {
        return PublishMsgsSync< const CPubSubClientTopic::TPubSubMsgsRefVector >( msgs );
    }
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientSide::GetMsgAttributesForLog( const CIPubSubMsg& msg )
{GUCEF_TRACE;

    return "MsgId=\"" + msg.GetMsgId().AsString() + "\", MsgIndex=\"" + msg.GetMsgIndex().AsString() + "\", MsgDateTime=\"" + CORE::ToString( msg.GetMsgDateTime() ) +
            "\", MsgPrimaryPayloadSize=\"" + CORE::ToString( msg.GetPrimaryPayload().ByteSize( false ) ) + "\", MsgNrOfKeyValuePairs=\"" + CORE::ToString( msg.GetKeyValuePairs().size() ) + "\", MsgNrOfMetaDataKeyValuePairs=\"" + CORE::ToString( msg.GetMetaDataKeyValuePairs().size() ) +
            "\", ReceiveActionId=\"" + CORE::ToString( msg.GetReceiveActionId() ) + "\", OriginClientTopic=\"" + ( msg.GetOriginClientTopic() != GUCEF_NULL ?  msg.GetOriginClientTopic()->GetTopicName() : CORE::CString::Empty ) + "\"";
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnCheckForTimedOutInFlightMessagesTimerCycle( CORE::CNotifier* notifier    ,
                                                                 const CORE::CEvent& eventId  ,
                                                                 CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CORE::UInt64 totalMsgsInFlight = 0;
    CORE::CDateTime cycleNowDt = CORE::CDateTime::NowUTCDateTime();
    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        CPubSubClientTopic* topic = topicLink.topic;

        totalMsgsInFlight += topicLink.inFlightMsgs.size();

        if ( GUCEF_NULL != topic && topic->IsPublishingSupported() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):OnCheckForTimedOutInFlightMessagesTimerCycle: Topic " + topic->GetTopicName() + " has " + CORE::ToString( topicLink.inFlightMsgs.size() ) + " messages in flight" );

            TopicLink::TUInt64Set inFlightDiscardList;
            TopicLink::TUInt64ToMsgTrackingEntryMap::iterator m = topicLink.inFlightMsgs.begin();
            while ( m != topicLink.inFlightMsgs.end() )
            {
                TopicLink::MsgTrackingEntry& inFlightEntry = (*m).second;
                if ( !inFlightEntry.msg.IsNULL() )
                {
                    if ( inFlightEntry.isInFlight && inFlightEntry.waitingForInFlightConfirmation )
                    {
                        CORE::Int64 timeInFlightInMs = inFlightEntry.lastPublishAttempt.GetTimeDifferenceInMillisecondsTowards( cycleNowDt );
                        if ( timeInFlightInMs > m_sideSettings.maxPublishedMsgInFlightTimeInMs )
                        {
                            // Since the max time elapsed we no longer consider the message in-flight
                            inFlightEntry.isInFlight = false;

                            if ( m_sideSettings.allowTimedOutPublishedInFlightMsgsRetryOutOfOrder )
                            {
                                // We will act as if we received a notice that publishing failed and will perform the regular
                                // retry routine

                                GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                    "):OnCheckForTimedOutInFlightMessagesTimerCycle: A message on topic " + topic->GetTopicName() + " has exceeded max in-flight time in ms (" + CORE::ToString( timeInFlightInMs ) +
                                    "). Will consider publishing as failed and will retry. First publish attempt was at " + CORE::ToString( inFlightEntry.firstPublishAttempt ) +
                                    " and the last attempt occured at " + CORE::ToString( inFlightEntry.lastPublishAttempt ) +
                                    ". publishActionId=" + CORE::ToString( inFlightEntry.publishActionId ) + ". " +
                                    GetMsgAttributesForLog( *inFlightEntry.msg ) );

                                // If the message has not been retried yet we will bump up the last published time
                                // this prevents the message from being discarded due to exceeding max retry time
                                if ( 0 == inFlightEntry.retryCount )
                                    inFlightEntry.firstPublishAttempt = cycleNowDt;

                                topicLink.publishFailedMsgs.insert( inFlightEntry.publishActionId );
                            }
                            else
                            {
                                inFlightDiscardList.insert( inFlightEntry.publishActionId );

                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                    "):OnCheckForTimedOutInFlightMessagesTimerCycle: A message on topic " + topic->GetTopicName() + " has exceeded max in-flight time in ms (" + CORE::ToString( timeInFlightInMs ) +
                                    ") Since we do not allow out-of-order retry for these the mesage will be lost! First publish attempt was at " + CORE::ToString( inFlightEntry.firstPublishAttempt ) +
                                    " and the last attempt occured at " + CORE::ToString( inFlightEntry.lastPublishAttempt ) +
                                    ". publishActionId=" + CORE::ToString( inFlightEntry.publishActionId ) + ". " +
                                    GetMsgAttributesForLog( *inFlightEntry.msg ) );
                            }
                        }
                    }
                }
                else
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                        "):OnCheckForTimedOutInFlightMessagesTimerCycle: A message tracking entry on topic " + topic->GetTopicName() + " has no message associated. We will discard the entry. publishActionId=" + CORE::ToString( inFlightEntry.publishActionId ) );

                    inFlightDiscardList.insert( inFlightEntry.publishActionId );
                }

                ++m;
            }

            size_t nrOfDiscardedMsgs = inFlightDiscardList.size();
            if ( nrOfDiscardedMsgs > 0 )
            {
                // Before we delete the message in-memory lets send it to the dead letter queue if one is configured for this flow
                if ( GUCEF_NULL != m_flowRouter )
                {
                    // We create a list with all messages instead of trying to send to the dead letter destination on a per message basis
                    // reason being that this is a localized cost whereas publishing across has a cross-locking blocking cost when running multi-threaded
                    // hence a batch operation reduces the calling /locking overhead
                    CPubSubClientTopic::TPubSubMsgsRefVector discardedMsgs;
                    discardedMsgs.reserve( nrOfDiscardedMsgs );

                    TopicLink::TUInt64Set::iterator n = inFlightDiscardList.begin();
                    while ( n != inFlightDiscardList.end() )
                    {               
                        TopicLink::MsgTrackingEntry& trackingEntry = topicLink.inFlightMsgs[ (*n) ];
                        CPubSubClientTopic::TPubSubMsgRef msgRef;
                        msgRef.LinkTo( trackingEntry.msg.GetPointerAlways() );
                        discardedMsgs.push_back( msgRef );
                        ++n;
                    }

                    if ( m_flowRouter->PublishMsgs( this, discardedMsgs, RouteType::DeadLetter ) )
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                            "):OnCheckForTimedOutInFlightMessagesTimerCycle: For topic " + topic->GetTopicName() + " we successfully sent a total of " + CORE::ToString( nrOfDiscardedMsgs ) +
                            " messages to a dead letter destination" );
                    }
                    else
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                            "):OnCheckForTimedOutInFlightMessagesTimerCycle: For topic " + topic->GetTopicName() + " we were unable to send a total of " + CORE::ToString( nrOfDiscardedMsgs ) +
                            " messages to any dead letter destination" );
                    }
                }
                
                // end of the road as far as we are concerned here
                // just delete the relevant tracking entries
                TopicLink::TUInt64Set::iterator n = inFlightDiscardList.begin();
                while ( n != inFlightDiscardList.end() )
                {               
                    topicLink.inFlightMsgs.erase( (*n) );
                    ++n;
                }

                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnCheckForTimedOutInFlightMessagesTimerCycle: For topic " + topic->GetTopicName() + " we discarded a total of " + CORE::ToString( nrOfDiscardedMsgs ) +
                    " messages due to exceeding the max in-flight time allowed which is configured as " + CORE::ToString( m_sideSettings.maxPublishedMsgInFlightTimeInMs ) );
            }
        }

        UpdateTopicMetrics( topicLink );
        ++i;
    }

    m_totalMsgsInFlight = totalMsgsInFlight;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::RetryPublishFailedMsgs( void )
{GUCEF_TRACE;

    CORE::UInt64 totalMsgsInFlight = 0;
    bool publishRetrySuccess = true;
    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        CPubSubClientTopic* topic = topicLink.topic;

        totalMsgsInFlight += topicLink.inFlightMsgs.size();

        if ( GUCEF_NULL != topic )
        {
            if ( !topicLink.publishFailedMsgs.empty() )
            {
                if ( !topic->IsPublishingSupported() )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has " + CORE::ToString( topicLink.publishFailedMsgs.size() ) +
                        " publish failed messages and yet the topic is set as being incapable of publishing. This should never happen! Clearing the failed message as resolving the situation is impossible" );

                    topicLink.publishFailedMsgs.clear();
                    ++i; continue;
                }

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has " + CORE::ToString( topicLink.publishFailedMsgs.size() ) + " messages queued for retry" );

                TopicLink::TUInt64Set inFlightDiscardList;
                TopicLink::TUInt64Set::iterator n = topicLink.publishFailedMsgs.begin();
                while ( n != topicLink.publishFailedMsgs.end() )
                {
                    CORE::UInt64 publishActionId = (*n);
                    TopicLink::TUInt64ToMsgTrackingEntryMap::iterator m = topicLink.inFlightMsgs.find( publishActionId );
                    if ( m != topicLink.inFlightMsgs.end() )
                    {
                        TopicLink::MsgTrackingEntry& retryEntry = (*m).second;
                        if ( !retryEntry.msg.IsNULL() )
                        {
                            if ( !retryEntry.isInFlight )
                            {
                                // Is this a publish retry or a ack retry?
                                if ( retryEntry.readyToAckPublishSuccessButAckFailed )
                                {
                                    // We are retrying a publish success ack for this message

                                    // Check if we are applying a max nr of retries and if we are in violation if so
                                    if ( m_sideSettings.maxMsgPublishAckRetryAttempts >= 0                                    &&
                                            retryEntry.retryCount >= (CORE::UInt32) m_sideSettings.maxMsgPublishAckRetryAttempts )
                                    {
                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has reached max nr of ack retries (" + CORE::ToString( retryEntry.retryCount ) +
                                            ") for acknowledging successfull publishing of a message. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                            " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                            ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                            GetMsgAttributesForLog( *retryEntry.msg ) );

                                        inFlightDiscardList.insert( retryEntry.publishActionId );
                                        n = topicLink.publishFailedMsgs.erase( n );
                                        continue;
                                    }
                                    else
                                    if ( m_sideSettings.maxMsgPublishRetryTotalTimeInMs >= 0 ) // Are we checking for max retry time?
                                    {
                                        CORE::Int64 timeDiffInMs = retryEntry.firstPublishAttempt.GetTimeDifferenceInMillisecondsTowards( retryEntry.lastPublishAttempt );
                                        if ( timeDiffInMs >= m_sideSettings.maxMsgPublishRetryTotalTimeInMs )
                                        {
                                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                                "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has reached amount of ack retry time in ms (" + CORE::ToString( timeDiffInMs ) +
                                                ") for acknowledging successfull publishing of a message. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                                " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                                ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                                GetMsgAttributesForLog( *retryEntry.msg ) );

                                            inFlightDiscardList.insert( retryEntry.publishActionId );
                                            n = topicLink.publishFailedMsgs.erase( n );
                                            continue;
                                        }
                                    }

                                    // If we make it here we are allowed to try again

                                    ++retryEntry.retryCount;
                                    retryEntry.lastPublishAttempt = CORE::CDateTime::NowUTCDateTime();

                                    if ( m_flowRouter != GUCEF_NULL )
                                    {
                                        if ( m_flowRouter->AcknowledgeReceiptForSide( this, retryEntry.msg ) )
                                        {
                                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                                "):RetryPublishFailedMsgs: Signaled receipt of message acknowledgement. publishActionId=" + CORE::ToString( retryEntry.publishActionId ) +
                                                ". receiveActionId=" + CORE::ToString( retryEntry.msg->GetReceiveActionId() ) + ". Success after " + CORE::ToString( retryEntry.retryCount ) + " ack retry attempts" );

                                            topicLink.inFlightMsgs.erase( m );
                                        }
                                        else
                                        {
                                            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                                "):RetryPublishFailedMsgs: Still failed to signal receipt of message acknowledgement. publishActionId=" + CORE::ToString( retryEntry.publishActionId ) +
                                                ". receiveActionId=" + CORE::ToString( retryEntry.msg->GetReceiveActionId() ) + ". Currently at " + CORE::ToString( retryEntry.retryCount ) + " ack retry attempts" );
                                        }
                                    }
                                }
                                else
                                {
                                    // We are retrying a backend publish of the message

                                    // Check if we are applying a max nr of retries and if we are in violation if so
                                    if ( m_sideSettings.maxMsgPublishRetryAttempts >= 0                                    &&
                                         retryEntry.retryCount >= (CORE::UInt32) m_sideSettings.maxMsgPublishRetryAttempts )
                                    {
                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has reached max nr of publish retries (" + CORE::ToString( retryEntry.retryCount ) +
                                            ") for publishing a message. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                            " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                            ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                            GetMsgAttributesForLog( *retryEntry.msg ) );

                                        inFlightDiscardList.insert( retryEntry.publishActionId );
                                        n = topicLink.publishFailedMsgs.erase( n );
                                        continue;
                                    }
                                    else
                                    if ( m_sideSettings.maxMsgPublishRetryTotalTimeInMs >= 0 ) // Are we checking for max retry time?
                                    {
                                        CORE::Int64 timeDiffInMs = retryEntry.firstPublishAttempt.GetTimeDifferenceInMillisecondsTowards( retryEntry.lastPublishAttempt );
                                        if ( timeDiffInMs >= m_sideSettings.maxMsgPublishRetryTotalTimeInMs )
                                        {
                                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                                "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has reached amount of publish retry time in ms (" + CORE::ToString( timeDiffInMs ) +
                                                ") for publishing a message. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                                " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                                ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                                GetMsgAttributesForLog( *retryEntry.msg ) );

                                            inFlightDiscardList.insert( retryEntry.publishActionId );
                                            n = topicLink.publishFailedMsgs.erase( n );
                                            continue;
                                        }
                                    }

                                    // If we make it here we are allowed to try again

                                    ++retryEntry.retryCount;
                                    retryEntry.lastPublishAttempt = CORE::CDateTime::NowUTCDateTime();

                                    CORE::UInt64 publishActionId = retryEntry.publishActionId;
                                    retryEntry.isInFlight = true;
                                    retryEntry.waitingForInFlightConfirmation = true;
                                    if ( topic->Publish( publishActionId, *retryEntry.msg.GetPointerAlways(), m_sideSettings.needToTrackInFlightPublishedMsgsForAck ) )
                                    {
                                        // It worked this time!

                                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Successfully retried publish of message on topic " + topic->GetTopicName() + " after " + CORE::ToString( retryEntry.retryCount ) +
                                            " retries. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                            " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                            ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                            GetMsgAttributesForLog( *retryEntry.msg ) );

                                        n = topicLink.publishFailedMsgs.erase( n );
                                    }
                                    else
                                    {
                                        // Still didnt work :(
                                        publishRetrySuccess = false;

                                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Retry failed of publish of message on topic " + topic->GetTopicName() + ". Retry attempt " + CORE::ToString( retryEntry.retryCount ) +
                                            ". First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                            " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                            ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input PublishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                            GetMsgAttributesForLog( *retryEntry.msg ) );

                                        if ( !m_sideSettings.allowOutOfOrderPublishRetry )
                                        {
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                "):RetryPublishFailedMsgs: A message tracking entry on topic " + topic->GetTopicName() + " has no message associated. We will discard the entry. publishActionId=" + CORE::ToString( retryEntry.publishActionId ) );

                            inFlightDiscardList.insert( retryEntry.publishActionId );
                        }
                    }

                    if ( n != topicLink.publishFailedMsgs.end() )
                        ++n;
                }

                size_t discardedMsgs = inFlightDiscardList.size();
                TopicLink::TUInt64Set::iterator d = inFlightDiscardList.begin();
                while ( d != inFlightDiscardList.end() )
                {
                    topicLink.inFlightMsgs.erase( (*d) );
                    ++d;
                }
                if ( discardedMsgs > 0 )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):RetryPublishFailedMsgs: For topic " + topic->GetTopicName() + " we discarded a total of " + CORE::ToString( discardedMsgs ) +
                        " messages due to exceeding the max retry attempts and/or sanity checks" );
                }
            }
        }
        UpdateTopicMetrics( topicLink );
        ++i;
    }

     m_totalMsgsInFlight = totalMsgsInFlight;
    return publishRetrySuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::PublishMailboxMsgs( void )
{GUCEF_TRACE;

    CORE::Int32 maxMailItemsToGrab = -1;
    if ( m_sideSettings.maxTotalMsgsInFlight > 0 )
    {
        CORE::Int64 remainingForFlight = m_sideSettings.maxTotalMsgsInFlight - m_totalMsgsInFlight;
        if ( remainingForFlight > 0 && remainingForFlight < GUCEF_MT_INT32MAX )
            maxMailItemsToGrab = (CORE::Int32) remainingForFlight;
    }

    CPubSubClientTopic::TIPubSubMsgSPtrVector msgs;
    if ( m_mailbox.GetSPtrBulkMail( msgs, maxMailItemsToGrab ) )
    {
        bool publishResult = PublishMsgsSync< CPubSubClientTopic::TIPubSubMsgSPtrVector >( msgs );
        return publishResult;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubTopicLocalPublishQueueFull( CORE::CNotifier* notifier    ,
                                                       const CORE::CEvent& eventId  ,
                                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CPubSubClientTopic* topic = static_cast< CPubSubClientTopic* >( notifier );

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
        "):OnPubSubTopicLocalPublishQueueFull: Topic=" + topic->GetTopicName() );

    TopicMap::iterator i = m_topics.find( topic );
    if ( i != m_topics.end() )
        UpdateTopicMetrics( (*i).second );
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubTopicMsgsReceived( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL == eventData )
        return;

    try
    {
        const CPubSubClientTopic::TPubSubMsgsRefVector& msgs = *static_cast< CPubSubClientTopic::TPubSubMsgsRefVector* >( eventData );
        if ( !msgs.empty() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                "):OnPubSubTopicMsgsReceived: Received " + CORE::ToString( msgs.size() ) + " message(s)" );
            
            // We now broadcast the received messages to all other sides which is the purpose of this class
            if ( GUCEF_NULL != m_flowRouter )
            {
                bool totalSuccess = m_flowRouter->PublishMsgs( this, msgs, RouteType::Active );
                if ( totalSuccess )
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                        "):OnPubSubTopicMsgsReceived: Successfully published message(s) via flow router" );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                        "):OnPubSubTopicMsgsReceived: Failed to publish (some?) message(s) via flow router" );
                }

                if ( totalSuccess && m_clientFeatures.supportsBookmarkingConcept )
                {
                    const CPubSubClientTopic::TPubSubMsgRef& lastMsgRef = msgs[ msgs.size()-1 ];
                    TopicMap::iterator i = m_topics.find( lastMsgRef->GetOriginClientTopic() );
                    if ( i != m_topics.end() )
                    {
                        TopicLink& topicLink = (*i).second;
                        CPubSubBookmark currentBookmark;
                        if ( m_clientFeatures.supportsDerivingBookmarkFromMsg )
                        {
                            if ( !lastMsgRef->GetOriginClientTopic()->DeriveBookmarkFromMsg( *lastMsgRef, currentBookmark ) )
                            {
                                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                    "):OnPubSubTopicMsgsReceived: Unable to derive bookmark from message even through its supported. Falling back to getting bookmark for the current message batch" );

                                currentBookmark = lastMsgRef->GetOriginClientTopic()->GetCurrentBookmark();
                            }
                        }
                        else
                        {
                            currentBookmark = lastMsgRef->GetOriginClientTopic()->GetCurrentBookmark();
                        }
                        CPubSubBookmark::TBookmarkType currentBookmarkType = currentBookmark.GetBookmarkType();

                        switch ( currentBookmarkType )
                        {
                            case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_NOT_AVAILABLE:
                            {
                                // Better luck next batch, we will potentially not be able to use bookmarking to cover the current batch in isolation
                                // if derivation of a bookmark from a message is not supported
                                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                    "):OnPubSubTopicMsgsReceived: Current bookmark is temp not available for the current message batch" );
                                break;
                            }                            
                            case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_NOT_APPLICABLE:
                            {
                                // We should not get here, if we do there is some logic error in feature checking here or in 
                                // bookmark management in the backend code
                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                    "):OnPubSubTopicMsgsReceived: Current bookmark obtained from backend came back as not applicable. This should not happen based on the feature set!" );
                                break;
                            }
                            case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_NOT_INITIALIZED:
                            {
                                // We should not get here, if we do there is some logic error in feature checking here or in 
                                // bookmark management in the backend code
                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                    "):OnPubSubTopicMsgsReceived: Current bookmark obtained from backend came back as not initialized. This should not happen!" );
                                break;
                            }
                            default:
                            {
                                // Per the spec the 'current' bookmark requested during this event and its callback handler will be 
                                // bookmark position of the beginning of this message batch 'msgs'
                                // As such even if the backend does not support deriving a bookmark from a message we can still use bookmarks
                                // we just need perform some extra administration to keep track of them. Its the extra admin needs that make
                                // the derivation of a bookmark directly from a message the preferred route as its likely to be more efficient
                                if ( m_sideSettings.needToTrackInFlightPublishedMsgsForAck )
                                {
                                    if ( !m_clientFeatures.supportsDerivingBookmarkFromMsg )
                                    {
                                        // note this this operation is in opposition to the one below where we dont need to keep track because we dont care
                                        // about delayed acks. In that case you have the luxory of skipping this overhead
                                        //
                                        // Note that entries added here are removed at a later time via calls to CleanupMsgBatchBookmarksUpTo()
                                        topicLink.bookmarksOnMsgReceived[ lastMsgRef->GetReceiveActionId() ] = currentBookmark;

                                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                            "):OnPubSubTopicMsgsReceived: Retaining bookmark at recieveActionId " + CORE::ToString( lastMsgRef->GetReceiveActionId() ) );
                                    }
                                }
                                else
                                {
                                    // If we successfully published but we dont need to wait on any acks then we can update bookmarks right now if applicable
                                    // instead of waiting for the ack to come back before doing the same thing
                                    const CPubSubClientTopic::TPubSubMsgRef& lastMsgRef = msgs[ msgs.size()-1 ];
                                    TopicMap::iterator i = m_topics.find( lastMsgRef->GetOriginClientTopic() );
                                    if ( i != m_topics.end() )
                                    {
                                        TopicLink& topicLink = (*i).second;
                                        topicLink.msgsSinceLastBookmarkPersist += (Int32) msgs.size();

                                        UpdateReceivedMessagesBookmarkAsNeeded( *lastMsgRef     , 
                                                                                currentBookmark , 
                                                                                topicLink       );
                                    }
                                }
                                break;
                            }
                        }
                    }   
                }  
            }                
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) + "):OnPubSubTopicMsgsReceived: exception: " + CORE::CString( e.what() ) );
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::UpdateReceivedMessagesBookmarkAsNeeded( const CIPubSubMsg& msg                  , 
                                                           const CPubSubBookmark& msgBatchBookmark ,
                                                           TopicLink& topicLink                    )
{GUCEF_TRACE;

    if ( !m_clientFeatures.supportsBookmarkingConcept )
        return true; // not supported, treat as fyi no-op
    
    // Check criterea for our generic bookmark persistance     
    if ( !m_pubsubBookmarkPersistence.IsNULL() && 
            (
                ( m_sideSettings.pubsubBookmarkPersistenceConfig.autoPersistMsgInterval > 0 && topicLink.msgsSinceLastBookmarkPersist >= m_sideSettings.pubsubBookmarkPersistenceConfig.autoPersistMsgInterval ) || 
                ( m_sideSettings.pubsubBookmarkPersistenceConfig.autoPersistIntervalInMs > 0 && topicLink.lastBookmarkPersistSuccess.GetTimeDifferenceInMillisecondsToNow() >= m_sideSettings.pubsubBookmarkPersistenceConfig.autoPersistIntervalInMs ) 
            ) )
    {
        if ( m_pubsubBookmarkPersistence->StoreBookmark( m_bookmarkNamespace                , 
                                                         *m_pubsubClient.GetPointerAlways() , 
                                                         *topicLink.topic                   ,  
                                                         msgBatchBookmark                   ) )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                "):UpdateReceivedMessagesBookmarkAsNeeded: Stored bookmark. receiveActionId=" + CORE::ToString( msg.GetReceiveActionId() ) );

            topicLink.lastBookmarkPersistSuccess = CORE::CDateTime::NowUTCDateTime();
            topicLink.msgsSinceLastBookmarkPersist = 0;
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                "):UpdateReceivedMessagesBookmarkAsNeeded: Failed to store bookmark. msgsSinceLastBookmarkPersist=" + CORE::ToString( topicLink.msgsSinceLastBookmarkPersist ) +
                ". lastBookmarkPersistSuccess=" + CORE::ToString( topicLink.lastBookmarkPersistSuccess ) + ". " +
                GetMsgAttributesForLog( msg ) );
            return false;
        }
    }

    // not supported due to config or does not apply due to storage contraints
    // either way... treat as fyi no-op
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::UpdateReceivedMessagesBookmarkAsNeeded( const CIPubSubMsg& msg , 
                                                           TopicLink& topicLink   )
{GUCEF_TRACE;

    if ( m_clientFeatures.supportsBookmarkingConcept )
    {    
        // Deriving a bookmark from a message is preferred if supported due to the reduced administrative overhead
        // versus the assumed-to-be-implemented-more-efficiently 'DeriveBookmarkFromMsg' operation
        CPubSubBookmark bookmark;
        if ( m_clientFeatures.supportsDerivingBookmarkFromMsg )
        {
            if ( topicLink.topic->DeriveBookmarkFromMsg( msg, bookmark ) )
            {
                // this message will represent the 'bookmarked' batch which may differ from the batch size in which it was originally received
                // functionality this does not matter as the effect achieved is the same
                return UpdateReceivedMessagesBookmarkAsNeeded( msg, bookmark, topicLink );
            }
            else
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):UpdateReceivedMessagesBookmarkAsNeeded: Failed to derive bookmark from message. msgsSinceLastBookmarkPersist=" + CORE::ToString( topicLink.msgsSinceLastBookmarkPersist ) +
                    ". lastBookmarkPersistSuccess=" + CORE::ToString( topicLink.lastBookmarkPersistSuccess ) + ". " +
                    GetMsgAttributesForLog( msg ) );
                return false;
            }
        }
        else
        {
            // find the bookmark associated with the message batch
            // this gets a tad tricky since we do not store bookmarks for every message in a batch plus the ones we do store at the batch beginning
            // may not line up since this message is likely not the exact beginning of a batch
            // As such we rely on the receiveActionId to help us out since we know it is required to always increment
            // We can use this in a sorted map to find the closest bookmark <= the position of this message
            UInt64 msgBatchBookmarkReceiveId = 0;
            if ( FindClosestMsgBatchBookmarkToMsg( msg, topicLink, msgBatchBookmarkReceiveId, bookmark ) )
            {
                // the message passed will represent the closest 'bookmarked' batch which may differ from the batch size in which it was originally received
                // functionality this does not matter as the effect achieved is the same
                if ( UpdateReceivedMessagesBookmarkAsNeeded( msg, bookmark, topicLink ) )
                {
                    CleanupMsgBatchBookmarksUpTo( topicLink, msgBatchBookmarkReceiveId );
                    return true;
                }
                return false;
            }
            else
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):UpdateReceivedMessagesBookmarkAsNeeded: Failed to locate nearest msg batch bookmark for message. msgsSinceLastBookmarkPersist=" + CORE::ToString( topicLink.msgsSinceLastBookmarkPersist ) +
                    ". lastBookmarkPersistSuccess=" + CORE::ToString( topicLink.lastBookmarkPersistSuccess ) + ". " +
                    GetMsgAttributesForLog( msg ) );
                return false;
            }
        }
    }

    // not supported, treat as fyi no-op
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::FindClosestMsgBatchBookmarkToMsg( const CIPubSubMsg& msg            ,                                                            
                                                     const TopicLink& topicLink        ,
                                                     UInt64& msgBatchBookmarkReceiveId ,
                                                     CPubSubBookmark& msgBatchBookmark )
{GUCEF_TRACE;

    CORE::UInt64 msgReceiveActionId = msg.GetReceiveActionId();
    CORE::UInt64 lastBestBmReceiveActionId = 0;
    const CPubSubBookmark* lastBestBm = GUCEF_NULL;

    // the map will store the IDs in ascending order, we can count on that
    TopicLink::TUInt64ToBookmarkMap::const_iterator i = topicLink.bookmarksOnMsgReceived.begin();
    while ( i != topicLink.bookmarksOnMsgReceived.end() )
    {
        CORE::UInt64 bmReceiveActionId = (*i).first;
        if ( bmReceiveActionId <= msgReceiveActionId )
        {
            lastBestBmReceiveActionId = bmReceiveActionId;
            lastBestBm = &(*i).second;
        }
        else
            break;
        ++i;
    }

    if ( 0 != lastBestBmReceiveActionId && GUCEF_NULL != lastBestBm )
    {
        // We found closest bookmark we can use which matches or pre-dates the bookmark
        // that would have existed for the message given
        msgBatchBookmarkReceiveId = lastBestBmReceiveActionId;
        msgBatchBookmark.LinkTo( *lastBestBm );
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::CleanupMsgBatchBookmarksUpTo( TopicLink& topicLink             , 
                                                 UInt64 msgBatchBookmarkReceiveId )
{GUCEF_TRACE;

    // the map will store the IDs in ascending order, we can count on that, hence we can delete the entire range
    TopicLink::TUInt64ToBookmarkMap::iterator i = topicLink.bookmarksOnMsgReceived.find( msgBatchBookmarkReceiveId );
    if ( i != topicLink.bookmarksOnMsgReceived.end() )
    {
        topicLink.bookmarksOnMsgReceived.erase( topicLink.bookmarksOnMsgReceived.begin(), i );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::ProcessAcknowledgeReceiptsMailbox( void )
{GUCEF_TRACE;

    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;

        CIPubSubMsg::TNoLockSharedPtr msg;
        while ( topicLink.publishAckdMsgsMailbox.GetMail( msg ) )
        {
            if ( !AcknowledgeReceiptSync( msg, topicLink ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ProcessAcknowledgeReceiptsMailbox: Failed to sync ack receipt of message. " + GetMsgAttributesForLog( *msg ) );
            }
        }

        UpdateTopicMetrics( topicLink );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::AcknowledgeReceiptSync( CIPubSubMsg::TNoLockSharedPtr& msg )
{GUCEF_TRACE;

    TopicMap::iterator i = m_topics.find( msg->GetOriginClientTopic() );
    if ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        return AcknowledgeReceiptSync( msg, topicLink );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::AcknowledgeReceiptSync( CIPubSubMsg::TNoLockSharedPtr& msg ,
                                           TopicLink& topicLink               )
{GUCEF_TRACE;

    bool success = true;
    if ( m_clientFeatures.supportsSubscriberMsgReceivedAck )
        success = msg->GetOriginClientTopic()->AcknowledgeReceipt( *msg );

    if ( success )
    {
        // the ack success determines the result, the bookmark persistance is best-effort
        ++topicLink.msgsSinceLastBookmarkPersist;
        UpdateReceivedMessagesBookmarkAsNeeded( *msg.GetPointerAlways(), topicLink );
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::AcknowledgeReceiptASync( CIPubSubMsg::TNoLockSharedPtr& msg )
{GUCEF_TRACE;

    TopicMap::iterator i = m_topics.find( msg->GetOriginClientTopic() );
    if ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        topicLink.publishAckdMsgsMailbox.AddMail( msg );
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubTopicMsgsPublished( CORE::CNotifier* notifier    ,
                                               const CORE::CEvent& eventId  ,
                                               CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL == eventData || GUCEF_NULL == notifier || !m_sideSettings.needToTrackInFlightPublishedMsgsForAck )
        return;

    const CPubSubClientTopic::TMsgsPublishedEventData& eData = *static_cast< CPubSubClientTopic::TMsgsPublishedEventData* >( eventData );
    const CPubSubClientTopic::TPublishActionIdVector* publishActionIds = eData;
    CPubSubClientTopic* topic = static_cast< CPubSubClientTopic* >( notifier );

    // Here we translate the publish action IDs back into the original messages
    // Subsequently we use said original messages to ack that to the message origin that we received the message
    // This is what allows us to provide a garanteed handling garantee since its an explicit handoff all the way through

    TopicMap::iterator i = m_topics.find( topic );
    if ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;

        CPubSubClientTopic::TPublishActionIdVector::const_iterator n = publishActionIds->begin();
        while ( n != publishActionIds->end() )
        {
            TopicLink::TUInt64ToMsgTrackingEntryMap::iterator m = topicLink.inFlightMsgs.find( (*n) );
            if ( m != topicLink.inFlightMsgs.end() )
            {
                TopicLink::MsgTrackingEntry& msgTrackingEntry = (*m).second;
                msgTrackingEntry.isInFlight = false;
                msgTrackingEntry.waitingForInFlightConfirmation = false;
                CIPubSubMsg::TNoLockSharedPtr msg = msgTrackingEntry.msg;

                if ( GUCEF_NULL != m_flowRouter )
                {
                    if ( m_flowRouter->AcknowledgeReceiptForSide( this, msg ) )
                    {
                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                            "):OnPubSubTopicMsgsPublished: Signaled receipt of message acknowledgement. publishActionId=" + CORE::ToString( msgTrackingEntry.publishActionId ) +
                            ". receiveActionId=" + CORE::ToString( msg->GetReceiveActionId() ) );

                        topicLink.inFlightMsgs.erase( m );
                        if ( m_totalMsgsInFlight > 0 )
                            --m_totalMsgsInFlight;
                    }
                    else
                    {
                        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                            "):OnPubSubTopicMsgsPublished: Failed to signal receipt of message acknowledgement of message on topic " + topic->GetTopicName() +
                            ". Will reset entry for PublishAck retries. Publish details: retryCount=" + CORE::ToString( msgTrackingEntry.retryCount ) +
                            ". First publish attempt was at " + CORE::ToString( msgTrackingEntry.firstPublishAttempt ) +
                            " and this last attempt was successfull at " + CORE::ToString( msgTrackingEntry.lastPublishAttempt ) +
                            ". publishActionId=" + CORE::ToString( msgTrackingEntry.publishActionId ) + ". " +
                            GetMsgAttributesForLog( *msgTrackingEntry.msg ) );

                        // Configure the entry for a different type of retry: Ack retry
                        // This is a different retry and as such we reset the retry count and time, the log message above will provide the original values for tracing
                        msgTrackingEntry.readyToAckPublishSuccessButAckFailed = true;
                        msgTrackingEntry.retryCount = 0;
                        msgTrackingEntry.firstPublishAttempt = CORE::CDateTime::NowUTCDateTime();
                        msgTrackingEntry.lastPublishAttempt = msgTrackingEntry.firstPublishAttempt;

                        topicLink.publishFailedMsgs.insert( msgTrackingEntry.publishActionId );
                    }
                }
                else
                {
                    // @TODO
                }
            }
            else
            {
                // This should not happen
                // Only understandable reasons are bad eventing from the backend or if the in-flight time-out is configured too aggressively
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnPubSubTopicMsgsPublished: Failed to locate original in-flight message related to publishActionId " + CORE::ToString( (*n) ) );
            }
            ++n;
        }

        UpdateTopicMetrics( topicLink );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubTopicMsgsPublishFailure( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventId  ,
                                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL == eventData || GUCEF_NULL == notifier || !m_sideSettings.retryFailedPublishAttempts )
        return;

    const CPubSubClientTopic::TMsgsPublishedEventData& eData = *static_cast< CPubSubClientTopic::TMsgsPublishedEventData* >( eventData );
    const CPubSubClientTopic::TPublishActionIdVector* publishActionIds = eData;
    CPubSubClientTopic* topic = static_cast< CPubSubClientTopic* >( notifier );

    // Here we translate the publish action IDs back into the original messages
    // Subsequently we use said original messages to set up the retry mechanism

    TopicMap::iterator i = m_topics.find( topic );
    if ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;

        CPubSubClientTopic::TPublishActionIdVector::const_iterator n = publishActionIds->begin();
        while ( n != publishActionIds->end() )
        {
            CORE::UInt64 publishActionId = (*n);
            TopicLink::TUInt64ToMsgTrackingEntryMap::iterator m = topicLink.inFlightMsgs.find( publishActionId );
            if ( m != topicLink.inFlightMsgs.end() )
            {
                TopicLink::MsgTrackingEntry& msgTrackingEntry = (*m).second;
                msgTrackingEntry.isInFlight = false;
                msgTrackingEntry.waitingForInFlightConfirmation = false;
                msgTrackingEntry.readyToAckPublishSuccessButAckFailed = false;
                topicLink.publishFailedMsgs.insert( publishActionId );

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnPubSubTopicMsgsPublishFailure: Message with publishActionId of " + CORE::ToString( publishActionId ) + " queued for retry, publish failed" +
                    ". receiveActionId=" + CORE::ToString( msgTrackingEntry.msg->GetReceiveActionId() ) );
            }
            else
            {
                // This should not happen
                // Only understandable reasons are bad eventing from the backend or if the in-flight time-out is configured too aggressively
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnPubSubTopicMsgsPublishFailure: Failed to locate original in-flight message related to publishActionId " + CORE::ToString( (*n) ) );
            }
            ++n;
        }

        UpdateTopicMetrics( topicLink );
    }

    m_awaitingFailureReport = false;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientSide::GenerateMetricsFriendlyTopicName( const CORE::CString& topicName )
{GUCEF_TRACE;

    // Let's avoid non-ASCII stumbling blocks and force the down to ASCII
    CORE::CAsciiString asciiMetricsFriendlyTopicName = topicName.ForceToAscii( '_' );

    // Replace special chars
    static const char disallowedChars[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '=', '+', ',', '.', '<', '>', '/', '?', '`', '~', '\\', '|', '{', '}', '[', ']', ';', ':', '\'', '\"' };
    asciiMetricsFriendlyTopicName = asciiMetricsFriendlyTopicName.ReplaceChars( disallowedChars, sizeof(disallowedChars)/sizeof(char), '_' );

    // Back to the platform wide string convention format
    CORE::CString metricsFriendlyTopicName = CORE::ToString( asciiMetricsFriendlyTopicName );
    return metricsFriendlyTopicName;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::DisconnectPubSubClient( bool destroyClient )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( m_pubsubClient.IsNULL() )
        return true;

    if ( !m_pubsubClient->Disconnect() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClient: Failed to disconnect the pub-sub client" );
        return false;
    }

    if ( destroyClient || !m_clientFeatures.supportsAutoReconnect )
    {
        m_pubsubClient.Unlink();
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::ConfigureTopicLink( const CPubSubSideChannelSettings& pubSubSideSettings ,
                                       CPubSubClientTopic& topic                            )
{GUCEF_TRACE;                                                              

    RegisterTopicEventHandlers( topic );

    TopicLink& topicLink = m_topics[ &topic ];
    topicLink.topic = &topic;
    topicLink.metricFriendlyTopicName = pubSubSideSettings.metricsPrefix + "topic." + GenerateMetricsFriendlyTopicName( topic.GetTopicName() ) + ".";
    topicLink.metrics = &m_metricsMap[ topicLink.metricFriendlyTopicName ];
    topicLink.metrics->hasSupportForPublishing = topic.IsPublishingSupported();
    topicLink.metrics->hasSupportForSubscribing = topic.IsSubscribingSupported();

    UpdateTopicMetrics( topicLink );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::GetLatestBookmark( const CPubSubClientTopic& topic ,
                                      CPubSubBookmark& bookmark       )
{GUCEF_TRACE;

    if ( m_pubsubBookmarkPersistence.IsNULL() || m_pubsubClient.IsNULL() )
        return false;

    return m_pubsubBookmarkPersistence->GetLatestBookmark( m_bookmarkNamespace, *m_pubsubClient.GetPointerAlways(), topic, bookmark );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::ConnectPubSubClientTopic( CPubSubClientTopic& topic                             ,
                                             const CPubSubClientFeatures& clientFeatures           ,
                                             const CPubSubSideChannelSettings& pubSubSideSettings  )
{GUCEF_TRACE;
    
    if ( topic.InitializeConnectivity() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClientTopic: Successfully requested connectivity initialization for topic \"" + topic.GetTopicName() + "\". Proceeding" );

        // We use the 'desired' feature to also drive whether we actually subscribe at this point
        // saves us an extra setting
        const CPubSubClientTopicConfig* topicConfig = m_pubsubClient->GetTopicConfig( topic.GetTopicName() );
        if ( GUCEF_NULL != topicConfig && topicConfig->needSubscribeSupport )
        {
            // The method of subscription depends on the supported feature set
            bool subscribeSuccess = false;
            if ( !clientFeatures.supportsBookmarkingConcept ) // We have no control bookmark wise with this backend, just subscribe and hope for the best
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ConnectPubSubClientTopic: Bookmarking concept is not supported by the backend, we will attempt to subscribe as-is" );

                subscribeSuccess = topic.Subscribe();
            }
            else
            if ( clientFeatures.supportsServerSideBookmarkPersistance ) // first preference is always backend managed bookmarking if available
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ConnectPubSubClientTopic: Bookmarking concept is natively supported and managed by the backend independently and we will attempt to subscribe as such" );

                subscribeSuccess = topic.Subscribe();
            }
            else
            {
                // bookmarks are supported but they rely on client-side persistance
                // we will need to obtain said bookmark
                CPubSubBookmark bookmark;
                if ( GetLatestBookmark( topic, bookmark ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                        "):ConnectPubSubClientTopic: Bookmarking concept is supported by the backend via a client-side bookmark. Bookmark type=" + CORE::ToString( bookmark.GetBookmarkType() ) + ". Bookmark=" + bookmark.GetBookmarkData().AsString() );

                    subscribeSuccess = topic.SubscribeStartingAtBookmark( bookmark );
                }
                else
                {
                    // GUCEF_ERROR_LOG
                    // This is not fully supported yet, make it a non-error log statement for now
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                        "):ConnectPubSubClientTopic: Bookmarking concept is supported by the backend via a client-side message index marker but we failed at obtaining the last used message index" );

                    if ( pubSubSideSettings.subscribeWithoutBookmarkIfNoneIsPersisted )
                    {
                        subscribeSuccess = topic.Subscribe();
                        if ( !subscribeSuccess )
                        {
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                "):ConnectPubSubClientTopic: Also unable to subscribe using the default bookmark as a fallback" );
                            return false;
                        }
                    }
                    else
                        return false;
                }
            }

            if ( !subscribeSuccess )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ConnectPubSubClient: Failed to subscribe to topic: " + topic.GetTopicName() );
                return false;
            }
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::PerformPubSubClientSetup( bool hardReset )
{GUCEF_TRACE;

    if ( !DisconnectPubSubClient( hardReset ) )
        return false;
    
    MT::CObjectScopeLock lock( this );

    CPubSubClientConfig& pubSubConfig = m_sideSettings.pubsubClientConfig;
    CPubSubBookmarkPersistenceConfig& pubsubBookmarkPersistenceConfig = m_sideSettings.pubsubBookmarkPersistenceConfig;

    // Set the bookmark namespace:
    // Depending on the context in which pubsub is used you'd want a different namespace
    // In this case we need to take care to account for the channel and side relationship to provide a unique storage area or rely on config
    if ( !m_sideSettings.pubsubBookmarkPersistenceConfig.bookmarkNamespace.IsNULLOrEmpty() )
        m_bookmarkNamespace = m_sideSettings.pubsubBookmarkPersistenceConfig.bookmarkNamespace;
    else
        m_bookmarkNamespace = m_sideSettings.pubsubIdPrefix + '.' + m_sideId; 

    // Set the pubsub ID prefix automatically if so desired:
    // Depending on the context in which pubsub is used you'd want a different namespace
    // In this case we need to take care to account for the channel and side relationship to provide a unique storage area / namespace / prefix or rely on config
    if ( pubSubConfig.pubsubIdPrefix == "{auto}"  )
        pubSubConfig.pubsubIdPrefix = m_sideSettings.pubsubIdPrefix + '.' + m_sideId; 

    bool clientSetupWasNeeded = false;
    if ( m_pubsubClient.IsNULL() )
    {
        // Create and configure the pub-sub client
        pubSubConfig.pulseGenerator = GetPulseGenerator();
        pubSubConfig.metricsPrefix = m_sideSettings.metricsPrefix;
        m_pubsubClient = CPubSubGlobal::Instance()->GetPubSubClientFactory().Create( pubSubConfig.pubsubClientType, pubSubConfig );

        if ( m_pubsubClient.IsNULL() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                "):PerformPubSubClientSetup: Failed to create a pub-sub client of type \"" + pubSubConfig.pubsubClientType + "\". Cannot proceed" );
            return false;
        }

        // Link the client back to this object
        // This allows getting all the way from:
        //      a message -> a topic -> a client -> a pubsub side
        m_pubsubClient->SetOpaqueUserData( this );
        
        clientSetupWasNeeded = true;
    }

    // Refresh our client features cache
    if ( !m_pubsubClient->GetSupportedFeatures( m_clientFeatures ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):PerformPubSubClientSetup: Failed to obtain supported a pub-sub client supported features" );
        return false;
    }

    if ( !m_clientFeatures.supportsAutoReconnect )
    {
        if ( GUCEF_NULL != m_pubsubClientReconnectTimer )
            m_pubsubClientReconnectTimer = new CORE::CTimer( *GetPulseGenerator(), pubSubConfig.reconnectDelayInMs );
        else
            m_pubsubClientReconnectTimer->SetInterval( pubSubConfig.reconnectDelayInMs );
    }

    if ( clientSetupWasNeeded )
    {
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):PerformPubSubClientSetup: Setup completed for pub-sub client of type \"" + pubSubConfig.pubsubClientType + "\" for side with id " +
            GetSideId() );

        NotifyObservers( PubSubClientInstantiationEvent, &m_pubsubClient );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::ConnectPubSubClient( void )
{GUCEF_TRACE;

    // Make sure setup was completed before connecting
    if ( !PerformPubSubClientSetup( false ) )
        return false;

    MT::CObjectScopeLock lock( this );

    CPubSubClientConfig& pubSubConfig = m_sideSettings.pubsubClientConfig;
    CPubSubBookmarkPersistenceConfig& pubsubBookmarkPersistenceConfig = m_sideSettings.pubsubBookmarkPersistenceConfig;
    
    // Whether we need to track successfull message handoff (garanteed handling) depends on various factors outside the scope of any one side
    // as such we need to ask the overarching infra to come up with a conclusion on this need
    // We will cache the outcome as a side local setting to negate locking needs
    m_sideSettings.needToTrackInFlightPublishedMsgsForAck = false;
    if ( GUCEF_NULL != m_flowRouter )
        m_sideSettings.needToTrackInFlightPublishedMsgsForAck = m_flowRouter->IsTrackingInFlightPublishedMsgsForAcksNeeded( this );

    if ( m_pubsubBookmarkPersistence.IsNULL() )
    {
        // Create and configure the pub-sub bookmark persistence
        m_pubsubBookmarkPersistence = CPubSubGlobal::Instance()->GetPubSubBookmarkPersistenceFactory().Create( pubsubBookmarkPersistenceConfig.bookmarkPersistenceType, pubsubBookmarkPersistenceConfig );

        if ( m_pubsubBookmarkPersistence.IsNULL() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                "):ConnectPubSubClient: Failed to create bookmark persistance access of type \"" + pubsubBookmarkPersistenceConfig.bookmarkPersistenceType + "\". Cannot proceed" );
            return false;
        }
    }

    if ( !m_pubsubClient->Connect() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClient: Failed to connect the pub-sub client" );
        return false;
    }

    // Create and configure the pub-sub client's topics
    m_topics.clear();
    CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = pubSubConfig.topics.begin();
    while ( i != pubSubConfig.topics.end() )
    {
        CPubSubClient::PubSubClientTopicSet topicAccess;
        if ( m_pubsubClient->CreateMultiTopicAccess( (*i), topicAccess ) )
        {
            CPubSubClient::PubSubClientTopicSet::iterator a = topicAccess.begin();
            while ( a != topicAccess.end() )
            {            
                CPubSubClientTopic* topic = (*a);
                if ( GUCEF_NULL == topic )
                {
                    if ( !(*i).isOptional )
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                            "):ConnectPubSubClient: Failed to create a pub-sub client topic access for topic \"" + (*i).topicName + "\". Cannot proceed" );
                        return false;
                    }
                    else
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                            "):ConnectPubSubClient: Unable to create a pub-sub client topic access for optional topic \"" + (*i).topicName + "\". Proceeding" );
                    }
                }
                else
                {
                    ConfigureTopicLink( m_sideSettings, *topic );
                }
                ++a;
            }
        }
        ++i;
    }

    bool totalTopicConnectSuccess = true;
    TopicMap::iterator t = m_topics.begin();
    while ( t != m_topics.end() )
    {
        TopicLink& topicLink = (*t).second;
        CPubSubClientTopic* topic = topicLink.topic;
        
        totalTopicConnectSuccess = ConnectPubSubClientTopic( *topicLink.topic ,
                                                             m_clientFeatures ,
                                                             m_sideSettings   ) && totalTopicConnectSuccess;

        ++t;
    }
    return totalTopicConnectSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    CPubSubClientConfig& pubSubConfig = m_sideSettings.pubsubClientConfig;

    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), m_sideSettings.metricsIntervalInMs );
    m_metricsTimer->SetEnabled( m_sideSettings.collectMetrics );

    m_timedOutInFlightMessagesCheckTimer = new CORE::CTimer( *GetPulseGenerator(), (CORE::UInt32) m_sideSettings.maxPublishedMsgInFlightTimeInMs );
    m_timedOutInFlightMessagesCheckTimer->SetEnabled( m_sideSettings.maxPublishedMsgInFlightTimeInMs > 0 );

    if ( m_sideSettings.performPubSubInDedicatedThread )
    {
        // Set the minimum number of cycles we will go full speed if a single cycle was not enough to handle
        // all the processing. This will cause a bypass of CPU yielding if/when the situation arises.
        // In such a case the thread will run at max speed for a least the below set nr of cycles.
        GetPulseGenerator()->RequestPulsesPerImmediatePulseRequest( m_sideSettings.ticketRefillOnBusyCycle );

        // Default smallest pulse delta at 25ms
        GetPulseGenerator()->RequestPeriodicPulses( this, 25 );

        if ( m_sideSettings.applyThreadCpuAffinity )
        {
            if ( SetCpuAffinityByCpuId( m_sideSettings.cpuAffinityForPubSubThread ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Successfully set a CPU affinity for logical CPU " + CORE::UInt32ToString( m_sideSettings.cpuAffinityForPubSubThread ) );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Failed to set a CPU affinity for logical CPU " + CORE::UInt32ToString( m_sideSettings.cpuAffinityForPubSubThread ) +
                    ". Proceeding without affinity");
            }
        }
    }

    if ( !ConnectPubSubClient() )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):OnTaskStart: Failed initial connection attempt on task start, will rely on auto-reconnect" );
    }

    RegisterEventHandlers();

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::IsRunningInDedicatedThread( void ) const
{GUCEF_TRACE;

    return m_sideSettings.performPubSubInDedicatedThread;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    // If we are running async from other sides we need to check the mailbox
    if ( m_sideSettings.performPubSubInDedicatedThread )
    {
        ProcessAcknowledgeReceiptsMailbox();

        // Check if we have a max for messages in-flight, if so dont try to process any more messages right now
        if ( m_sideSettings.maxTotalMsgsInFlight <= 0 || m_totalMsgsInFlight < (CORE::UInt64) m_sideSettings.maxTotalMsgsInFlight )
        {
            if ( !m_awaitingFailureReport )
            {
                if ( RetryPublishFailedMsgs() || m_sideSettings.allowOutOfOrderPublishRetry )
                    PublishMailboxMsgs();
            }
        }
    }

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnTaskEnding( CORE::CICloneable* taskdata ,
                                 bool willBeForced           )
{GUCEF_TRACE;

    CORE::CTaskConsumer::OnTaskEnding( taskdata, willBeForced );

    if ( willBeForced )
    {
        // reduce memory leaks
        m_mailbox.Clear();
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnTaskEnded( CORE::CICloneable* taskData ,
                                bool wasForced              )
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    delete m_pubsubClientReconnectTimer;
    m_pubsubClientReconnectTimer = GUCEF_NULL;

    delete m_timedOutInFlightMessagesCheckTimer;
    m_timedOutInFlightMessagesCheckTimer = GUCEF_NULL;

    CORE::CTaskConsumer::OnTaskEnded( taskData, wasForced );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::LoadConfig( const CPubSubSideChannelSettings& sideSettings )
{GUCEF_TRACE;

    m_sideSettings = sideSettings;
    return true;
}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelSettings&
CPubSubClientSide::GetSideSettings( void )
{GUCEF_TRACE;

    return m_sideSettings;
}

/*-------------------------------------------------------------------------*/

const CPubSubSideChannelSettings&
CPubSubClientSide::GetSideSettings( void ) const
{GUCEF_TRACE;

    return m_sideSettings;
}

/*-------------------------------------------------------------------------*/

CORE::CString 
CPubSubClientSide::GetSideId( void ) const
{GUCEF_TRACE;

    return m_sideId;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


