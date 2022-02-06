/*
 *  pubsubpluginSTORAGE: Generic GUCEF COMCORE plugin for providing pubsub storage
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

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTCONFIG_H
#include "pubsubpluginSTORAGE_CStoragePubSubClientConfig.h"
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTCONFIG_H
#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTCONFIG_H ? */

#include "pubsubpluginSTORAGE_CStoragePubSubClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace STORAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CStoragePubSubClient::TypeName = "STORAGE";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CStoragePubSubClient::CStoragePubSubClient( const COMCORE::CPubSubClientConfig& config )
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

    m_config.metricsPrefix += "storage.";

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClient::~CStoragePubSubClient()
{GUCEF_TRACE;
    
    Disconnect();

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

CStoragePubSubClientConfig& 
CStoragePubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClient::GetSupportedFeatures( COMCORE::CPubSubClientFeatures& features )
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
    
    
    return true;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic*
CStoragePubSubClient::CreateTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CStoragePubSubClientTopic* rcTopic = new CStoragePubSubClientTopic( this );
    if ( rcTopic->LoadConfig( topicConfig ) )
    {
        m_topicMap[ topicConfig.topicName ] = rcTopic;
    }
    return rcTopic;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic* 
CStoragePubSubClient::GetTopicAccess( const CORE::CString& topicName )
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
CStoragePubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
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
CStoragePubSubClient::GetTopicConfig( const CORE::CString& topicName )
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
CStoragePubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
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
CStoragePubSubClient::GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList )
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
CStoragePubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClient::SaveConfig( CORE::CDataNode& cfgNode ) const
{GUCEF_TRACE;

    return m_config.SaveConfig( cfgNode );
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClient::LoadConfig( const CORE::CDataNode& cfgRoot )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    CStoragePubSubClientConfig cfg;
    if ( cfg.LoadConfig( cfgRoot ) )
    {
        m_config = cfg;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClient::Disconnect( void )
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
CStoragePubSubClient::Connect( void )
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
CStoragePubSubClient::IsConnected( void )
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
CStoragePubSubClient::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CStoragePubSubClient::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClient::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
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
        CStoragePubSubClientTopic* topic = (*i).second;
        const CStoragePubSubClientTopic::TopicMetrics& topicMetrics = topic->GetMetrics();
        const CORE::CString& topicName = topic->GetMetricFriendlyTopicName();
        const CStoragePubSubClientTopicConfig& topicConfig = topic->GetTopicConfig();
        CORE::CString metricsPrefix = m_config.metricsPrefix + topicName;
        
        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace STORAGE */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
