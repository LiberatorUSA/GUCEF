/*
 *  pubsubpluginMSMQ: Generic GUCEF COMCORE plugin for providing pubsub via Microsoft MSMQ
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#include <Objbase.h>
#include <string.h>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#include "pubsubpluginMSMQ_CMsmqPubSubClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace MSMQ {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CMsmqPubSubClient::TypeName = "MSMQ";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CMsmqPubSubClient::CMsmqPubSubClient( const COMCORE::CPubSubClientConfig& config )
    : COMCORE::CPubSubClient()
    , m_config( config )
    , m_metricsTimer( GUCEF_NULL )
    , m_topicMap()
{GUCEF_TRACE;

    if ( GUCEF_NULL != config.pulseGenerator )
    {
        if ( config.desiredFeatures.supportsMetrics )
        {
            m_metricsTimer = new CORE::CTimer( *config.pulseGenerator, 1000 );
            m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
        }
    }
    else
    {
        if ( config.desiredFeatures.supportsMetrics )
        {
            m_metricsTimer = new CORE::CTimer( 1000 );        
            m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
        }
    }

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClient::~CMsmqPubSubClient()
{GUCEF_TRACE;
    
    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig& 
CMsmqPubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClient::GetSupportedFeatures( COMCORE::CPubSubClientFeatures& features )
{GUCEF_TRACE;

    features.supportsBinaryPayloads = true;             // The MSMQ body property supports a binary payload
    features.supportsPerMsgIds = true;                  // MSMQ has the concept of a message ID which is unique and an additional non-unique label
    features.supportsPrimaryPayloadPerMsg = true;       // For MSMQ "BODY" is the primary payload which is also in of itself a key-value message propery
    features.supportsAbsentPrimaryPayloadPerMsg = true; // Its allowed to send tags without a BODY payload
    features.supportsKeyValueSetPerMsg = false;         // Arbitrary key-value app data is not natively supported by MSMQ
    features.supportsDuplicateKeysPerMsg = false;       // TODO: Since arbitrary key-value app data is not native and we simulate this we will do so in a manner that supports duplicate keys
    features.supportsMetaDataKeyValueSetPerMsg = true;  // This is native to MSMQ
    features.supportsMultiHostSharding = false;         // MSMQ is tied to the Windows O/S and queues are not auto shared across such O/S instances
    features.supportsPublishing = true;                 // We support being a MSQM queue publisher in this plugin
    features.supportsSubscribing = true;                // We support being a MSMQ queue subscriber in this plugin
    features.supportsMetrics = true;                    // This plugin has support for reporting its own set of metrics
    features.supportsAutoReconnect = true;              // Not applicable to local queues and for remote queues MSMQ supports the concept of "offline mode"        
    features.supportsAbsentMsgReceivedAck = false;      // Since MSMQ is a queue, by default you consume the message when you read it
    features.supportsAckUsingLastMsgInBatch = false;    // Even when using LookupID we have to operate per message. We dont track the batch ourselves    
    features.supportsBookmarkingConcept = true;         // Always getting the top msg in the queue could be thought of as "remembering your last read position" so as such we will claim MSMQ supports this    
    features.supportsAutoBookmarking = true;            // Always getting the top msg in the queue could be thought of as "remembering your last read position" so as such we will claim MSMQ supports this
    features.supportsMsgIdBasedBookmark = false;        // MSMQ does not support this concept. receiving messages removes them from the O/S queue    
    features.supportsMsgIndexBasedBookmark = false;     // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsMsgDateTimeBasedBookmark = false;  // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    
    // For MSMQ 3.0 and above:
    #if ( _WIN32_WINNT >= 0x0501 )
    
    bool supportLookup = m_config.simulateReceiveAckFeatureViaLookupId && m_config.desiredFeatures.supportsSubscriberMsgReceivedAck; 
    
    features.supportsAutoMsgReceivedAck = !supportLookup;            // When simulating receive acks we never auto ack
    features.supportsSubscriberMsgReceivedAck = supportLookup;       // The whole point is simulating the ability to ack that a message was handled
    features.supportsAckUsingBookmark = supportLookup;               // Bookmark or message, either way we use the LookupID which we count as a topic index
    features.supportsServerSideBookmarkPersistance = !supportLookup; // If we are using lookups the LookUp will need to be persisted externally from the app between runs
    
    features.supportsSubscribingUsingBookmark = true;             // we use the LookupID which we count as a topic index
    features.supportsTopicIndexBasedBookmark = true;              // we use the LookupID which we count as a topic index
    
    #else

    features.supportsServerSideBookmarkPersistance = true; // since MSMQ is a queue it remembers simply through consumption
    features.supportsAutoMsgReceivedAck = true;            // Since MSMQ is a queue, by default you consume the message when you read it we can consider this an ack
    features.supportsSubscriberMsgReceivedAck = false;     // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsAckUsingBookmark = false;             // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsSubscribingUsingBookmark = false;     // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsTopicIndexBasedBookmark = false;      // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    
    #endif

    return true;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic*
CMsmqPubSubClient::CreateTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CMsmqPubSubClientTopic* rcTopic = new CMsmqPubSubClientTopic( this );
    if ( rcTopic->LoadConfig( topicConfig ) )
    {
        m_topicMap[ topicConfig.topicName ] = rcTopic;
    }
    return rcTopic;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic* 
CMsmqPubSubClient::GetTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        return (*i).second;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        delete (*i).second;
        m_topicMap.erase( i );
    }
}

/*-------------------------------------------------------------------------*/

const COMCORE::CPubSubClientTopicConfig* 
CMsmqPubSubClient::GetTopicConfig( const CORE::CString& topicName )
{GUCEF_TRACE;

    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        if ( topicName == (*i).topicName )
        {
            return &(*i);
        }
        ++i;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        topicNameList.insert( (*i).topicName );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClient::GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        topicNameList.insert( (*i).first );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CMsmqPubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClient::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClient::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClient::Disconnect( void )
{GUCEF_TRACE;

    bool totalSuccess = true;
    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        totalSuccess = (*i).second->Disconnect() && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClient::Connect( void )
{GUCEF_TRACE;

    if ( !m_topicMap.empty() )
    {
        bool allConnected = true;
        TTopicMap::iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            allConnected = (*i).second->InitializeConnectivity() && allConnected;
            ++i;
        }
        return allConnected;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClient::IsConnected( void )
{GUCEF_TRACE;

    if ( !m_topicMap.empty() )
    {
        bool allConnected = true;
        TTopicMap::iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            allConnected = (*i).second->IsConnected() && allConnected;
            ++i;
        }
        return allConnected;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClient::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CMsmqPubSubClient::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClient::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                        const CORE::CEvent& eventId  ,
                                        CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // Quickly grab a snapshot of metric values for all topics 
    // we don't combine this with metrics publishing as it adds to metrics timeframe drift across topics
    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        (*i).second->OnMetricsTimerCycle( notifier  ,
                                          eventId   ,
                                          eventData );
        ++i;
    }

    // Now publish all the metrics to the metrics system
    i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        CMsmqPubSubClientTopic* topic = (*i).second;
        const CMsmqPubSubClientTopic::TopicMetrics& topicMetrics = topic->GetMetrics();
        const CORE::CString& topicName = topic->GetMetricFriendlyTopicName();
        const CMsmqPubSubClientTopicConfig& topicConfig = topic->GetTopicConfig();
        CORE::CString metricsPrefix = m_config.metricsPrefix + topicName;

        if ( topicMetrics.msmqMsgsInQueue >= 0 )
            GUCEF_METRIC_GAUGE( metricsPrefix + ".msmqMsgsInQueue", topicMetrics.msmqMsgsInQueue, 1.0f );

        if ( m_config.desiredFeatures.supportsPublishing )
        {
            GUCEF_METRIC_COUNT( metricsPrefix + ".msmqMessagesPublished", topicMetrics.msmqMessagesPublished, 1.0f );
            GUCEF_METRIC_COUNT( metricsPrefix + ".msmqErrorsOnPublish", topicMetrics.msmqErrorsOnPublish, 1.0f );
        }
        if ( m_config.desiredFeatures.supportsSubscribing )
        {
            GUCEF_METRIC_COUNT( metricsPrefix + ".msmqMessagesReceived", topicMetrics.msmqMessagesReceived, 1.0f );
            GUCEF_METRIC_COUNT( metricsPrefix + ".msmqErrorsOnReceive", topicMetrics.msmqErrorsOnReceive, 1.0f );

            // For MSMQ 3.0 and above:
            #if ( _WIN32_WINNT >= 0x0501 )

            GUCEF_METRIC_COUNT( metricsPrefix + ".msmqErrorsOnReceiveAck", topicMetrics.msmqErrorsOnAck, 1.0f );

            #endif
        }
        
        if ( topicConfig.gatherMsmqTransitTimeOnReceiveMetric )
        {
            CMsmqPubSubClientTopic::UInt32Vector::const_iterator n = topicMetrics.msmqMsgSentToArriveLatencies.begin();
            while ( n != topicMetrics.msmqMsgSentToArriveLatencies.end() )
            {
                GUCEF_METRIC_TIMING( metricsPrefix + ".msmqMsgSentToArriveLatencyInMs", (*n), 1.0f );
                ++n;
            }
        }

        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MSMQ */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
