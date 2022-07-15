/*
 *  pubsubpluginKAFKA: Generic GUCEF COMCORE plugin for providing pubsub via Kafka
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

#include "pubsubpluginKAFKA_CKafkaPubSubClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace KAFKA {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CKafkaPubSubClient::TypeName = "Kafka";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CKafkaPubSubClient::CKafkaPubSubClient( const PUBSUB::CPubSubClientConfig& config )
    : PUBSUB::CPubSubClient()
    , m_config( config )
    , m_metricsTimer( GUCEF_NULL )
    , m_topicMap()
{GUCEF_TRACE;

    SetupBasedOnConfig();
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClient::~CKafkaPubSubClient()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClient::Clear( void )
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        delete (*i).second;
        (*i).second = GUCEF_NULL;
        ++i;
    }
    m_topicMap.clear();
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientConfig& 
CKafkaPubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

CORE::ThreadPoolPtr 
CKafkaPubSubClient::GetThreadPool( void )
{GUCEF_TRACE;

    return m_threadPool;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::GetSupportedFeatures( PUBSUB::CPubSubClientFeatures& features ) const
{GUCEF_TRACE;
                     
    features.supportsBinaryPayloads = true;               // Kafka supports this natively
    features.supportsPerMsgIds = true;                    // Kafka supports this natively
    features.supportsPrimaryPayloadPerMsg = true;         // This is the primary method for conveying data
    features.supportsAbsentPrimaryPayloadPerMsg = false;  // With Kafka the primary payload is not optional
    features.supportsKeyValueSetPerMsg = true;            // Kafka supports adding attibutes beyond the primary payload
    features.supportsDuplicateKeysPerMsg = true;          // Kafka not care about duplicate header keys, they are just "fields". Some client libraries might have issues with lookups though
    features.supportsMultiHostSharding = true;            // Kafka supports a cluster of brokers with multiple replicated partitions
    features.supportsPublishing = true;                   // We support being a Kafka producer in this plugin
    features.supportsSubscribing = true;                  // We support being a Kafka consumer in this plugin
    features.supportsMetrics = true;                      // We support metrics in this plugin
    features.supportsAutoReconnect = true;                // RdKafka adds auto reconnect out of the box
    features.supportsSubscriberMsgReceivedAck = true;     // Normal Kafka behaviour
    features.supportsAutoMsgReceivedAck = true;           // This is configurable on the Kafka client library
    features.supportsAbsentMsgReceivedAck = true;         // supported if you wish to 'peek' only without removing messages but can become problematic in various scenarios
    features.supportsAckUsingLastMsgInBatch = true;       // This is supported via how you trigger offset commits and is standard practice 
    features.supportsAckUsingBookmark = true;             // This is supported via how you trigger offset commits
    features.supportsBookmarkingConcept = true;           // Bookmarks are a native Kafka concept
    features.supportsSubscribingUsingBookmark = true;     // We provide limited support for client-side provided bookmarks
    features.supportsServerSideBookmarkPersistance = true;// The natural way to interact with Kafka is to let it manage bookmarks server-side
    features.supportsAutoBookmarking = true;              // Kafka keeps bookmarks for a given client id Kafka-side and supports auto commit in RdKafka
    features.supportsMsgIdBasedBookmark = false;          // Kafka does not support finding a conclusive offset based on a message key, there could be many across many partitions making it useless
    features.supportsMsgIndexBasedBookmark = true;        // Offsets (index) is the native Kafka "bookmark"method and thus preferred
    features.supportsTopicIndexBasedBookmark = true;      // Offsets (index) is the native Kafka "bookmark"method and thus preferred
    features.supportsMsgDateTimeBasedBookmark = true;     // We support this via code that converts the DateTime to offsets
    features.supportsDerivingBookmarkFromMsg = true;      // Supported via a BSOD on the message's index field currently
    features.supportsDiscoveryOfAvailableTopics = false;  // @TODO: not implemented yet
    features.supportsGlobPatternTopicNames = false;
    features.supportsSubscriptionMsgArrivalDelayRequests = true;    // We support a backoff of the consume() event processing
    return true;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopic*
CKafkaPubSubClient::CreateTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CKafkaPubSubClientTopic* topicAccess = GUCEF_NULL;
    {
        MT::CObjectScopeLock lock( this );

        topicAccess = new CKafkaPubSubClientTopic( this );
        if ( topicAccess->LoadConfig( topicConfig ) )
        {
            m_topicMap[ topicConfig.topicName ] = topicAccess;
        }
        else
        {
            delete topicAccess;
            topicAccess = GUCEF_NULL;
        }
    }

    if ( GUCEF_NULL != topicAccess )
    {
        TopicAccessCreatedEventData eData( topicConfig.topicName );
        NotifyObservers( TopicAccessCreatedEvent, &eData );
    }

    return topicAccess;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopic* 
CKafkaPubSubClient::GetTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        return (*i).second;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        CKafkaPubSubClientTopic* topicAccess = (*i).second;
        m_topicMap.erase( i );

        TopicAccessDestroyedEventData eData( topicName );
        NotifyObservers( TopicAccessDestroyedEvent, &eData );
        
        delete topicAccess;        
    }
}

/*-------------------------------------------------------------------------*/

const PUBSUB::CPubSubClientTopicConfig* 
CKafkaPubSubClient::GetTopicConfig( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
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

bool 
CKafkaPubSubClient::BeginTopicDiscovery( const CORE::CString::StringSet& globPatternFilters )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        topicNameList.insert( (*i).topicName );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClient::GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        topicNameList.insert( (*i).first );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CKafkaPubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}

/*-------------------------------------------------------------------------*/

bool 
CKafkaPubSubClient::SaveConfig( CORE::CDataNode& cfgNode ) const
{GUCEF_TRACE;

    return m_config.SaveConfig( cfgNode );
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::SetupBasedOnConfig( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_config.pulseGenerator )
    {
        // Use the global pulse generator.
        // NOT what you want if you want thread isolation or basically any time you are not
        // writing a single threaded app
        m_config.pulseGenerator = &CORE::CCoreGlobal::Instance()->GetPulseGenerator();
    }
        
    if ( m_config.desiredFeatures.supportsMetrics )
    {
        m_metricsTimer = new CORE::CTimer( m_config.pulseGenerator, 1000 );
        m_metricsTimer->SetEnabled( m_config.desiredFeatures.supportsMetrics );
    }

    RegisterEventHandlers();

    if ( m_config.remoteAddresses.empty() )
    {
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClient:SetupBasedOnConfig: No remote addresses have been provided" );
        return false;
    }

    m_config.metricsPrefix += "kafka.";

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::LoadConfig( const PUBSUB::CPubSubClientConfig& config )
{GUCEF_TRACE;

    Clear();

    m_config = config;
    return SetupBasedOnConfig();
}

/*-------------------------------------------------------------------------*/

bool 
CKafkaPubSubClient::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    // Make sure we can actually load the config before overwriting anything
    CKafkaPubSubClientConfig config;
    if ( config.LoadConfig( cfg  ) )
    {
        // make the new config the active config
        m_config = config;
        return true;
    }
    else        
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::Disconnect( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    if ( IsConnected() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClient(" + CORE::PointerToString( this ) + "):Disconnect: Beginning topic disconnect" );
        
        bool totalSuccess = true;
        TTopicMap::iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            totalSuccess = (*i).second->Disconnect() && totalSuccess;
            ++i;
        }

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClient(" + CORE::PointerToString( this ) + "):Disconnect: Finished topic disconnect" );

        return totalSuccess;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::Connect( void )
{GUCEF_TRACE;

    Disconnect();
    
    MT::CObjectScopeLock lock( this );
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClient(" + CORE::PointerToString( this ) + "):Disconnect: Beginning topic connect" );
        
    bool totalSuccess = true;
    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        if ( !(*i).second->IsConnected() )
        {
            if ( (*i).second->IsSubscribingSupported() )
            {
                totalSuccess = (*i).second->Subscribe() && totalSuccess;
            }
        }
        ++i;
    }

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClient(" + CORE::PointerToString( this ) + "):Disconnect: Finished topic connect" );

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::IsConnected( void ) const 
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    if ( m_topicMap.empty() )
        return false;
    
    bool fullyConnected = true;
    TTopicMap::const_iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        fullyConnected = (*i).second->IsConnected() && fullyConnected;
        ++i;
    }
    return fullyConnected;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::IsHealthy( void ) const 
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    if ( m_topicMap.empty() )
        return true;
    
    bool fullyHealthy = true;
    TTopicMap::const_iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        fullyHealthy = (*i).second->IsHealthy() && fullyHealthy;
        ++i;
    }
    return fullyHealthy;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClient::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CKafkaPubSubClient::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClient::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventId  ,
                                         CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        const CKafkaPubSubClientTopic::TopicMetrics& metrics = (*i).second->GetMetrics();
        const CORE::CString& topicName = (*i).second->GetTopicName();

        GUCEF_METRIC_COUNT( m_config.metricsPrefix + topicName + ".kafkaErrorReplies", metrics.kafkaErrorReplies, 1.0f );

        if ( m_config.desiredFeatures.supportsPublishing )
        {
            GUCEF_METRIC_COUNT( m_config.metricsPrefix + topicName + ".kafkaMessagesTransmitted", metrics.kafkaMessagesTransmitted, 1.0f );
            GUCEF_METRIC_GAUGE( m_config.metricsPrefix + topicName + ".kafkaTransmitQueueSize", metrics.kafkaTransmitQueueSize, 1.0f );
            GUCEF_METRIC_GAUGE( m_config.metricsPrefix + topicName + ".kafkaTransmitOverflowQueueSize", metrics.kafkaTransmitOverflowQueueSize, 1.0f );
        }
        if ( m_config.desiredFeatures.supportsSubscribing )
        {
            GUCEF_METRIC_COUNT( m_config.metricsPrefix + topicName + ".kafkaMessagesReceived", metrics.kafkaMessagesReceived, 1.0f );
            GUCEF_METRIC_COUNT( m_config.metricsPrefix + topicName + ".kafkaMessagesFiltered", metrics.kafkaMessagesFiltered, 1.0f );
        }

        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAFKA */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
