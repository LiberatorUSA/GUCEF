/*
 *  pubsubpluginAWSSQS: Generic GUCEF COMCORE plugin for providing pubsub via AWS's SQS
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

#ifndef GUCEF_PLUGINGLUE_AWSSDK_CAWSSDKGLOBAL_H
#include "pluginglueAWSSDK_CAwsSdkGlobal.h"
#define GUCEF_PLUGINGLUE_AWSSDK_CAWSSDKGLOBAL_H
#endif /* GUCEF_PLUGINGLUE_AWSSDK_CAWSSDKGLOBAL_H ? */

#include "pubsubpluginAWSSQS_CAwsSqsPubSubClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace AWSSQS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CAwsSqsPubSubClient::TypeName = "AwsSqs";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CAwsSqsPubSubClient::CAwsSqsPubSubClient( const PUBSUB::CPubSubClientConfig& config )
    : PUBSUB::CPubSubClient()
    , m_config( config )
    , m_metricsTimer( GUCEF_NULL )
    , m_topicMap()
    , m_sqsClient( PLUGINGLUE::AWSSDK::CAwsSdkGlobal::Instance()->GetDefaultAwsClientConfig() )
{GUCEF_TRACE;

    if ( !LoadConfig( config ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "AwsSqsPubSubClient: Failed to load config at construction" );
    }

    if ( GUCEF_NULL != config.pulseGenerator )
    {
        if ( config.desiredFeatures.supportsMetrics )
        {
            m_metricsTimer = GUCEF_NEW CORE::CTimer( config.pulseGenerator, 1000 );
            m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
        }
    }
    else
    {
        if ( config.desiredFeatures.supportsMetrics )
        {
            m_metricsTimer = GUCEF_NEW CORE::CTimer( 1000 );        
            m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
        }
    }

    m_config.metricsPrefix += "sqs.";

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClient::~CAwsSqsPubSubClient()
{GUCEF_TRACE;
    
    GUCEF_DELETE m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        (*i).second->UnlinkFromParentClient();
        (*i).second.Unlink();
        ++i;
    }
    m_topicMap.clear();
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientConfig& 
CAwsSqsPubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::GetSupportedFeatures( PUBSUB::CPubSubClientFeatures& features ) const
{GUCEF_TRACE;

    features.supportsBinaryPayloads = false;             // Per SendMessage doc: "A message can include only XML, JSON, and unformatted text. The following Unicode characters are allowed", so only text and a tiny subset of Unicode
    features.supportsPerMsgIds = true;                   // Supported but system generated only
    features.supportsPrimaryPayloadPerMsg = true;        // This is the primary way to send the payload in SQS, key-value attribute support is merely supplemental
    features.supportsAbsentPrimaryPayloadPerMsg = false; // With SQS the primary payload is not optional
    features.supportsKeyValueSetPerMsg = true;           // Supported in SQS using message attributes. "Each message can have up to 10 attributes. Message attributes are optional and separate from the message body (however, they are sent alongside it)"
    features.supportsMetaDataKeyValueSetPerMsg = true;   // "Whereas you can use message attributes to attach custom metadata to Amazon SQS messages for your applications, you can use message system attributes to store metadata for other AWS services, such as AWS X-Ray"
    features.supportsDuplicateKeysPerMsg = false;        // Since attributes are a map of keys to a value it mandates that every key is unique
    features.supportsMultiHostSharding = true;           // SQS is a managed service which under the coveres is shareded across hardware/nodes
    features.supportsPublishing = true;                  // SQS supports sending messages to the queue
    features.supportsSubscribing = true;                 // SQS supports reading messages from the queue
    features.supportsMetrics = true;                     // We add our own metrics support in this plugin for SQS specific stats
    features.supportsAutoReconnect = true;               // Our plugin adds auto reconnect out of the box
    features.supportsSubscriberMsgReceivedAck = false;   // Since SQS is a queue where you consume the messages: this does not apply
    features.supportsAutoMsgReceivedAck = false;         // Since SQS is a queue where you consume the messages: grabbing the message is in a way the ack but this does not really apply
    features.supportsAbsentMsgReceivedAck = true;        // Since SQS is a queue where you consume the messages: this does not apply and hence can be absent
    features.supportsAckUsingLastMsgInBatch = false;     // Since SQS is a queue where you consume the messages: this does not apply
    features.supportsAckUsingBookmark = false;           // Since SQS is a queue where you consume the messages: this does not apply
    features.supportsBookmarkingConcept = true;          // Since SQS is a queue where you consume the messages: Your offset is remembered simply due to the nature of a queue
    features.supportsSubscribingUsingBookmark = false;   // Since SQS is a queue where you consume the messages: This is not supported
    features.supportsServerSideBookmarkPersistance = true; // Since SQS is a queue where you consume the messages: Your offset is remembered simply due to the nature of a queue
    features.supportsAutoBookmarking = true;             // Since SQS is a queue where you consume the messages: Your offset is remembered simply due to the nature of a queue
    features.supportsMsgIdBasedBookmark = false;         // Since SQS is a queue where you consume the messages: You cannot provide a msg ID to resume from a given point
    features.supportsMsgIndexBasedBookmark = false;      // Since SQS is a queue where you consume the messages: You cannot provide a msg index to resume from a given point
    features.supportsTopicIndexBasedBookmark = false;    // Since SQS is a queue where you consume the messages: You cannot provide a msg index to resume from a given point
    features.supportsMsgDateTimeBasedBookmark = false;   // Since SQS is a queue where you consume the messages: You cannot provide a datetime to resume from a given point in time
    features.supportsDiscoveryOfAvailableTopics = false; // @TODO: need to look into this
    features.supportsGlobPatternTopicNames = false;
    features.supportsSubscriptionMsgArrivalDelayRequests = false;
    features.supportsSubscriptionEndOfDataEvent = false;
    return true;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopicPtr
CAwsSqsPubSubClient::CreateTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CAwsSqsPubSubClientTopicPtr topicAccess;
    {
        MT::CObjectScopeLock lock( this );

        topicAccess = ( GUCEF_NEW CAwsSqsPubSubClientTopic( this ) )->CreateSharedPtr();
        if ( topicAccess->LoadConfig( topicConfig ) )
        {
            m_topicMap[ topicConfig.topicName ] = topicAccess;
            RegisterTopicEventHandlers( topicAccess );
        }
        else
        {
            topicAccess->UnlinkFromParentClient();
            topicAccess.Unlink();
        }
    }

    if ( !topicAccess.IsNULL() )
    {
        TopicAccessCreatedEventData eData( topicConfig.topicName );
        NotifyObservers( TopicAccessCreatedEvent, &eData );
    }

    return topicAccess;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopicPtr 
CAwsSqsPubSubClient::GetTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;
        
    MT::CObjectScopeLock lock( this );

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        return (*i).second;
    }
    return PUBSUB::CPubSubClientTopicPtr();
}

/*-------------------------------------------------------------------------*/

void 
CAwsSqsPubSubClient::GetAllCreatedTopicAccess( PubSubClientTopicSet& topicAccess )
{GUCEF_TRACE;
        
    MT::CObjectScopeLock lock( this );

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        topicAccess.insert( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        CAwsSqsPubSubClientTopicPtr topicAccess = (*i).second;
        m_topicMap.erase( i );

        TopicAccessDestroyedEventData eData( topicName );
        NotifyObservers( TopicAccessDestroyedEvent, &eData );
        
        topicAccess->UnlinkFromParentClient();
        topicAccess.Unlink();        
    }
}

/*-------------------------------------------------------------------------*/

const PUBSUB::CPubSubClientTopicConfig* 
CAwsSqsPubSubClient::GetTopicConfig( const CORE::CString& topicName )
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
CAwsSqsPubSubClient::BeginTopicDiscovery( const CORE::CString::StringSet& globPatternFilters )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
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
CAwsSqsPubSubClient::GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        topicNameList.insert( (*i).first );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

Aws::SQS::SQSClient&
CAwsSqsPubSubClient::GetSqsClient( void )
{GUCEF_TRACE;

    return m_sqsClient;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CAwsSqsPubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    //MT::CScopeMutex lock( m_lock );
    return m_config.SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
{GUCEF_TRACE;

    //MT::CScopeMutex lock( m_lock );
    cfg = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    PUBSUB::CPubSubClientConfig parsedCfg;
    if ( parsedCfg.LoadConfig( cfg ) )
    {
        //MT::CScopeMutex lock( m_lock );
        m_config = parsedCfg;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg  )
{GUCEF_TRACE;

    //MT::CScopeMutex lock( m_lock );
    m_config = cfg;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::Disconnect( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::Connect( void )
{GUCEF_TRACE;

    Disconnect();

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::IsConnected( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::IsHealthy( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClient::IsInitialized( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClient::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CAwsSqsPubSubClient::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void 
CAwsSqsPubSubClient::RegisterTopicEventHandlers( CAwsSqsPubSubClientTopicPtr& topic )
{GUCEF_TRACE;

    if ( !topic.IsNULL() )
    {
        TEventCallback callback( this, &CAwsSqsPubSubClient::OnTopicHealthStatusChange );
        SubscribeTo( topic.GetPointerAlways()                          ,
                     CAwsSqsPubSubClientTopic::HealthStatusChangeEvent ,
                     callback                                          );
    }
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClient::OnTopicHealthStatusChange( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // (Re)determine the aggregate health status
    IsHealthy();
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClient::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                        const CORE::CEvent& eventId  ,
                                        CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        (*i).second->OnMetricsTimerCycle( notifier  ,
                                          eventId   ,
                                          eventData );
        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSQS */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
