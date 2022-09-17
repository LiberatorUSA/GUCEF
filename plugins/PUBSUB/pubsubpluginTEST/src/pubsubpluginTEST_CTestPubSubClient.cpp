/*
 *  pubsubpluginTEST: Generic GUCEF PUBSUB plugin for adding integration/system tests
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

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H
#include "pubsubpluginTEST_CTestPubSubClientConfig.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H ? */

#include "pubsubpluginTEST_CTestPubSubClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace TEST {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CTestPubSubClient::TypeName = "TEST";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CTestPubSubClient::CTestPubSubClient( const PUBSUB::CPubSubClientConfig& config )
    : PUBSUB::CPubSubClient()
    , m_config()
    , m_metricsTimer( GUCEF_NULL )
    , m_topicMap()
    , m_threadPool()
    , m_isHealthy( true )
    , m_lock()
{GUCEF_TRACE;

    if ( !LoadConfig( config ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "TestPubSubClient: Failed to load config at construction" );
    }

    m_isHealthy = m_config.defaultIsHealthyStatus;
    
    if ( config.desiredFeatures.supportsMetrics )
    {
        m_metricsTimer = new CORE::CTimer( config.pulseGenerator, 1000 );
        m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
    }

    m_threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetOrCreateThreadPool( "TestPubSubClient(" + CORE::ToString( this ) + ")", config.pulseGenerator, true );

    m_config.metricsPrefix += "test.";

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CTestPubSubClient::~CTestPubSubClient()
{GUCEF_TRACE;
    
    Disconnect();

    MT::CScopeMutex lock( m_lock );

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        delete (*i).second;
        (*i).second = GUCEF_NULL;
        ++i;
    }
    m_topicMap.clear();
    
    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CTestPubSubClientConfig& 
CTestPubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

CORE::ThreadPoolPtr 
CTestPubSubClient::GetThreadPool( void )
{GUCEF_TRACE;

    return m_threadPool;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::GetSupportedFeatures( PUBSUB::CPubSubClientFeatures& features ) const
{GUCEF_TRACE;

    features.supportsBinaryPayloads = true;             // The storage backend with the natively compatible pub-sub serializers can always support everything
    features.supportsPerMsgIds = true;                  // The storage backend with the natively compatible pub-sub serializers can always support everything
    features.supportsPrimaryPayloadPerMsg = true;       // The storage backend with the natively compatible pub-sub serializers can always support everything
    features.supportsAbsentPrimaryPayloadPerMsg = true; // The storage backend with the natively compatible pub-sub serializers can always support everything
    features.supportsKeyValueSetPerMsg = true;          // The storage backend with the natively compatible pub-sub serializers can always support everything
    features.supportsDuplicateKeysPerMsg = true;        // The storage backend with the natively compatible pub-sub serializers can always support everything
    features.supportsMetaDataKeyValueSetPerMsg = true;  // The storage backend with the natively compatible pub-sub serializers can always support everything
    features.supportsMultiHostSharding = false;         // Its possible the VFS storage is multi-host but we'd have no way of knowing. Afaik its 1 file system
    features.supportsPublishing = true;                 // We support writing to VFS storage files
    features.supportsSubscribing = true;                // We support reading from VFS storage files
    features.supportsMetrics = true;                    // This plugin has support for reporting its own set of metrics
    features.supportsAutoReconnect = true;              // To the extent it even applies in this case, sure we 'reconnect' to storage
    features.supportsDiscoveryOfAvailableTopics = false; // <- @TODO
    features.supportsGlobPatternTopicNames = false;
    features.supportsSubscriptionMsgArrivalDelayRequests = false;
    features.supportsSubscriptionEndOfDataEvent = false;
    
    // Ack functionality doesnt currently make sense for this backend
    // However in theory we could implement a hard or logical delete of read messages and such functionality could go hand-in-hand with 'server-side' (read backend controlled) bookmark persistance
    // Right now we dont have that though so lets get the basics working first
    features.supportsAbsentMsgReceivedAck = false;      
    features.supportsAckUsingLastMsgInBatch = false;    
    features.supportsAutoMsgReceivedAck = false;         
    features.supportsSubscriberMsgReceivedAck = false;     
    features.supportsAckUsingBookmark = false;                 

    features.supportsBookmarkingConcept = true;         // We can create a reference to the storage location plus offset        
    features.supportsAutoBookmarking = false;           // Currently we do 'forget' where we are if the app crashes    
    features.supportsMsgIdBasedBookmark = false;        // In this context we have no idea what the message ID is, as such we cannot use it as a bookmark since we cannot garantee anything    
    features.supportsMsgIndexBasedBookmark = false;     // In this context we have no idea what the message Index is, as such we cannot use it as a bookmark since we cannot garantee anything    
    features.supportsTopicIndexBasedBookmark = true;    // We can create a reference to the storage location plus offset
    features.supportsMsgDateTimeBasedBookmark = false;      // In this context we have no idea what the message datetime is, as such we cannot use it as a bookmark since we cannot garantee anything    
    features.supportsServerSideBookmarkPersistance = false; // Currently we do 'forget' where we are if the app crashes. @TODO? Maybe we can add storage backend opinionated bookmark storage    
    features.supportsSubscribingUsingBookmark = true;       // We can create a reference to the storage location plus offset and then use that to resume the reading from that location
    features.supportsDerivingBookmarkFromMsg = false;   // since the storage backend is more of a transcribing passthrough it doesnt know what on the message means what. As such it cannot support this
    
    
    return true;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopic*
CTestPubSubClient::CreateTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CTestPubSubClientTopic* topicAccess = GUCEF_NULL;
    {
        MT::CScopeMutex lock( m_lock );

        topicAccess = new CTestPubSubClientTopic( this );
        if ( topicAccess->LoadConfig( topicConfig ) )
        {
            m_topicMap[ topicConfig.topicName ] = topicAccess;
            RegisterTopicEventHandlers( topicAccess );
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
CTestPubSubClient::GetTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        return (*i).second;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void 
CTestPubSubClient::GetAllCreatedTopicAccess( PubSubClientTopicSet& topicAccess )
{GUCEF_TRACE;
        
    MT::CScopeMutex lock( m_lock );

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        topicAccess.insert( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        CTestPubSubClientTopic* topicAccess = (*i).second;
        m_topicMap.erase( i );

        lock.EarlyUnlock();
        TopicAccessDestroyedEventData eData( topicName );
        NotifyObservers( TopicAccessDestroyedEvent, &eData );
        
        delete topicAccess;        
    }
}

/*-------------------------------------------------------------------------*/

const PUBSUB::CPubSubClientTopicConfig* 
CTestPubSubClient::GetTopicConfig( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

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
CTestPubSubClient::BeginTopicDiscovery( const CORE::CString::StringSet& globPatternFilters )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        topicNameList.insert( (*i).topicName );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClient::GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        topicNameList.insert( (*i).first );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CTestPubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}


/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    CTestPubSubClientConfig parsedCfg;
    if ( parsedCfg.LoadConfig( cfg ) )
    {
        MT::CScopeMutex lock( m_lock );
        m_config = parsedCfg;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg  )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    CTestPubSubClientConfig parsedCfg;
    if ( parsedCfg.LoadConfig( cfg ) )
    {
        MT::CScopeMutex lock( m_lock );
        m_config = parsedCfg;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

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
CTestPubSubClient::Connect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

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
CTestPubSubClient::IsConnected( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( !m_topicMap.empty() )
    {
        bool allConnected = true;
        TTopicMap::const_iterator i = m_topicMap.begin();
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

bool
CTestPubSubClient::IsHealthy( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    if ( !m_topicMap.empty() )
    {
        // Aggregate the health status of all topics
        bool allHealthy = true;
        TTopicMap::const_iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            allHealthy = (*i).second->IsHealthy() && allHealthy;
            ++i;
        }

        // Notify if there was a change in status
        if ( allHealthy != m_isHealthy )
        {
            m_isHealthy = allHealthy;        

            if ( m_isHealthy )
            {
                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClient:IsHealthy: overall health is now Ok" );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClient:IsHealthy: overall health status is now unhealthy" );         
            }

            lock.EarlyUnlock();
            THealthStatusChangeEventData eData( allHealthy ); 
            NotifyObservers( HealthStatusChangeEvent, &eData );         
        }

        return allHealthy;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::IsInitialized( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClient::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CTestPubSubClient::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void 
CTestPubSubClient::RegisterTopicEventHandlers( PUBSUB::CPubSubClientTopic* topic )
{GUCEF_TRACE;

    if ( GUCEF_NULL != topic )
    {
        TEventCallback callback( this, &CTestPubSubClient::OnTopicHealthStatusChange );
        SubscribeTo( topic                                           ,
                     CTestPubSubClientTopic::HealthStatusChangeEvent ,
                     callback                                        );
    }
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClient::OnTopicHealthStatusChange( CORE::CNotifier* notifier    ,
                                                 const CORE::CEvent& eventId  ,
                                                 CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // (Re)determine the aggregate health status
    IsHealthy();
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClient::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
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
        CTestPubSubClientTopic* topic = (*i).second;
        const CTestPubSubClientTopic::TopicMetrics& topicMetrics = topic->GetMetrics();
        const CORE::CString& topicName = topic->GetMetricFriendlyTopicName();
        const CTestPubSubClientTopicConfig& topicConfig = topic->GetTopicConfig();
        CORE::CString metricsPrefix = m_config.metricsPrefix + topicName;
        
        GUCEF_METRIC_GAUGE( metricsPrefix + ".queuedReadyToReadBuffers", topicMetrics.queuedReadyToReadBuffers, 1.0f );
        GUCEF_METRIC_GAUGE( metricsPrefix + ".smallestBufferSizeInBytes", topicMetrics.smallestBufferSizeInBytes, 1.0f );        
        GUCEF_METRIC_GAUGE( metricsPrefix + ".largestBufferSizeInBytes", topicMetrics.largestBufferSizeInBytes, 1.0f );
        
        if ( CTestPubSubClientTopicConfig::CHANNELMODE_TEST_TO_PUBSUB == topicConfig.mode )
        {
            GUCEF_METRIC_COUNT( metricsPrefix + ".storageCorruptionDetections", topicMetrics.storageCorruptionDetections, 1.0f );
            GUCEF_METRIC_COUNT( metricsPrefix + ".msgsLoadedFromStorage", topicMetrics.msgsLoadedFromStorage, 1.0f );
            GUCEF_METRIC_COUNT( metricsPrefix + ".storageDeserializationFailures", topicMetrics.storageDeserializationFailures, 1.0f );        
        }
        else
        if ( CTestPubSubClientTopicConfig::CHANNELMODE_PUBSUB_TO_TEST == topicConfig.mode )
        {
            GUCEF_METRIC_COUNT( metricsPrefix + ".msgsWrittenToStorage", topicMetrics.msgsWrittenToStorage, 1.0f );
        }        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::Unlock( void ) const 
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace TEST */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
