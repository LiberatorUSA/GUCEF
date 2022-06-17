/*
 *  pubsubpluginWEB: Generic GUCEF COMCORE plugin for providing pubsub approximation via the WEB
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

#ifndef PUBSUBPLUGIN_WEB_CWEBPUBSUBCLIENTCONFIG_H
#include "pubsubpluginWEB_CWebPubSubClientConfig.h"
#define PUBSUBPLUGIN_WEB_CWEBPUBSUBCLIENTCONFIG_H
#endif /* PUBSUBPLUGIN_WEB_CWEBPUBSUBCLIENTCONFIG_H ? */

#include "pubsubpluginWEB_CWebPubSubClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CWebPubSubClient::TypeName = "WEB";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CWebPubSubClient::CWebPubSubClient( const PUBSUB::CPubSubClientConfig& config )
    : PUBSUB::CPubSubClient()
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

    m_config.metricsPrefix += "msmq.";

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CWebPubSubClient::~CWebPubSubClient()
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

CWebPubSubClientConfig&
CWebPubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClient::GetSupportedFeatures( PUBSUB::CPubSubClientFeatures& features ) const
{GUCEF_TRACE;

    features.supportsBinaryPayloads = true;                // We can use our own serializer for the messages and as such we should be able to support all its fields
    features.supportsPerMsgIds = true;                     // We can use our own serializer for the messages and as such we should be able to support all its fields
    features.supportsPrimaryPayloadPerMsg = true;          // We can use our own serializer for the messages and as such we should be able to support all its fields
    features.supportsAbsentPrimaryPayloadPerMsg = true;    // We can use our own serializer for the messages and as such we should be able to support all its fields
    features.supportsKeyValueSetPerMsg = true;             // We can use our own serializer for the messages and as such we should be able to support all its fields
    features.supportsDuplicateKeysPerMsg = true;           // We can use our own serializer for the messages and as such we should be able to support all its fields
    features.supportsMetaDataKeyValueSetPerMsg = true;     // We can use our own serializer for the messages and as such we should be able to support all its fields
    features.supportsMultiHostSharding = false;            // The backend directly opens the ports on the local machine, any sharding would have to be external
    features.supportsPublishing = true;                    // We support Web clients consuming messages in a variaty of Webby ways
    features.supportsSubscribing = true;                   // We support Web clients pushing messages in a variaty of Webby ways
    features.supportsMetrics = true;                       // This plugin has support for reporting its own set of metrics
    features.supportsAutoReconnect = true;                 // Server port can attempt to auto-re-open on error and such

    // todo:
    features.supportsAbsentMsgReceivedAck = false;         // Since MSMQ is a queue, by default you consume the message when you read it
    features.supportsAckUsingLastMsgInBatch = false;       // Even when using LookupID we have to operate per message. We dont track the batch ourselves
    features.supportsBookmarkingConcept = true;            // Always getting the top msg in the queue could be thought of as "remembering your last read position" so as such we will claim MSMQ supports this
    features.supportsAutoBookmarking = true;               // Always getting the top msg in the queue could be thought of as "remembering your last read position" so as such we will claim MSMQ supports this
    features.supportsMsgIdBasedBookmark = false;           // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsMsgIndexBasedBookmark = false;        // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsMsgDateTimeBasedBookmark = false;     // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsServerSideBookmarkPersistance = true; // since MSMQ is a queue it remembers simply through consumption
    features.supportsAutoMsgReceivedAck = true;            // Since MSMQ is a queue, by default you consume the message when you read it we can consider this an ack
    features.supportsSubscriberMsgReceivedAck = false;     // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsAckUsingBookmark = false;             // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsSubscribingUsingBookmark = false;     // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsTopicIndexBasedBookmark = false;      // MSMQ does not support this concept. receiving messages removes them from the O/S queue
    features.supportsDiscoveryOfAvailableTopics = false; // <- @TODO
    features.supportsGlobPatternTopicNames = false;

    return true;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopic*
CWebPubSubClient::CreateTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CWebPubSubClientTopic* topicAccess = GUCEF_NULL;
    {
        MT::CObjectScopeLock lock( this );

        topicAccess = new CWebPubSubClientTopic( this );
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
CWebPubSubClient::GetTopicAccess( const CORE::CString& topicName )
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
CWebPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        CWebPubSubClientTopic* topicAccess = (*i).second;
        m_topicMap.erase( i );

        TopicAccessDestroyedEventData eData( topicName );
        NotifyObservers( TopicAccessDestroyedEvent, &eData );

        delete topicAccess;
    }
}

/*-------------------------------------------------------------------------*/

const PUBSUB::CPubSubClientTopicConfig*
CWebPubSubClient::GetTopicConfig( const CORE::CString& topicName )
{GUCEF_TRACE;

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
CWebPubSubClient::GetAvailableTopicNameList( CORE::CString::StringSet& topicNameList            ,
                                             const CORE::CString::StringSet& globPatternFilters )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CWebPubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        topicNameList.insert( (*i).topicName );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CWebPubSubClient::GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList )
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
CWebPubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClient::SaveConfig( CORE::CDataNode& cfgNode ) const
{GUCEF_TRACE;

    return m_config.SaveConfig( cfgNode );
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClient::LoadConfig( const CORE::CDataNode& cfgRoot )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    CWebPubSubClientConfig cfg;
    if ( cfg.LoadConfig( cfgRoot ) )
    {
        m_config = cfg;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClient::Disconnect( void )
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
CWebPubSubClient::Connect( void )
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
CWebPubSubClient::IsConnected( void )
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
CWebPubSubClient::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CWebPubSubClient::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CWebPubSubClient::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
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
        CWebPubSubClientTopic* topic = (*i).second;
        const CWebPubSubClientTopic::TopicMetrics& topicMetrics = topic->GetMetrics();
        const CORE::CString& topicName = topic->GetMetricFriendlyTopicName();
        const CWebPubSubClientTopicConfig& topicConfig = topic->GetTopicConfig();
        CORE::CString metricsPrefix = m_config.metricsPrefix + topicName;

        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
