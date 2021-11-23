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

#include <string>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H
#include "pubsubpluginSTORAGE_CStoragePubSubClient.h"
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H ? */

#include "pubsubpluginSTORAGE_CStoragePubSubClientTopic.h"

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::CStoragePubSubClientTopic( CStoragePubSubClient* client )
    : COMCORE::CPubSubClientTopic()
    , m_client( client )
    , m_config()
    , m_syncReadTimer( GUCEF_NULL )
    , m_reconnectTimer( GUCEF_NULL )
    , m_lock()
    , m_currentPublishActionId( 1 )
    , m_currentReceiveActionId( 1 )
    , m_publishSuccessActionIds()
    , m_publishSuccessActionEventData()
    , m_publishFailureActionIds()
    , m_publishFailureActionEventData()
    , m_metrics()
    , m_metricFriendlyTopicName()
{GUCEF_TRACE;
        
    m_publishSuccessActionEventData.LinkTo( &m_publishSuccessActionIds );
    m_publishFailureActionEventData.LinkTo( &m_publishFailureActionIds );

    m_syncReadTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, 25 );

    if ( m_client->GetConfig().desiredFeatures.supportsAutoReconnect )
    {
        m_reconnectTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, m_client->GetConfig().reconnectDelayInMs );
    }

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::~CStoragePubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();

    delete m_syncReadTimer;
    m_syncReadTimer = GUCEF_NULL;

    delete m_reconnectTimer;
    m_reconnectTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClient*
CStoragePubSubClientTopic::GetClient( void )
{GUCEF_TRACE;

    return m_client;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_reconnectTimer )
    {
        TEventCallback callback( this, &CStoragePubSubClientTopic::OnReconnectTimerCycle );
        SubscribeTo( m_reconnectTimer               ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }

    TEventCallback callback( this, &CStoragePubSubClientTopic::OnPulseCycle );
    SubscribeTo( m_client->GetConfig().pulseGenerator ,
                 CORE::CPulseGenerator::PulseEvent    ,
                 callback                             );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return m_config.needSubscribeSupport;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CStoragePubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::Publish( CORE::UInt64& publishActionId, const COMCORE::CIPubSubMsg& msg, bool notify )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    if ( 0 == publishActionId )
    {
        publishActionId = m_currentPublishActionId; 
        ++m_currentPublishActionId;
    }

    
    bool success = false;
    // @TODO: Code the actual publish to MSMQ, not implemented yet

    if ( notify )
    {
        if ( success )
            m_publishSuccessActionIds.push_back( publishActionId );
        else
            m_publishFailureActionIds.push_back( publishActionId );
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::SetupToSubscribe( COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( LoadConfig( config ) )
    {

        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CStoragePubSubClientTopic::GenerateMetricsFriendlyTopicName( const CORE::CString& topicName )
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
CStoragePubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;

    m_metricFriendlyTopicName = GenerateMetricsFriendlyTopicName( m_config.topicName );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    m_syncReadTimer->SetEnabled( false );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:Subscribe: Successfully 'subscribed to topic' for MSMQ queue. Topic Name: " + m_config.topicName );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    // Currently not supported.
    // Could theoretically be implemented using a cursor combined with PEEK only reads
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    // Currently not supported.
    // Could theoretically be implemented using a cursor combined with PEEK only reads
    // For regular queue consuming reads you could perhaps ignore messages with a send time older then msgDtBookmark
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::SubscribeStartingAtBookmark( const COMCORE::CPubSubBookmark& bookmark ) 
{GUCEF_TRACE;

    // Currently not supported.
    return false;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubBookmark 
CStoragePubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    // Not supported
    return COMCORE::CPubSubBookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_NOT_APPLICABLE );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceipt( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    COMCORE::CPubSubBookmark bookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_TOPIC_INDEX, msg.GetMsgIndex() );
    return AcknowledgeReceiptImpl( bookmark, msg.GetReceiveActionId() );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceipt( const COMCORE::CPubSubBookmark& bookmark )
{GUCEF_TRACE;
    
    return AcknowledgeReceiptImpl( bookmark, 0 );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceiptImpl( const COMCORE::CPubSubBookmark& bookmark , 
                                                CORE::UInt64 receiveActionId             )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    const CStoragePubSubClientConfig& clientConfig = m_client->GetConfig();

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
           
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

const CStoragePubSubClientTopicConfig& 
CStoragePubSubClientTopic::GetTopicConfig( void ) const
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    bool totalSuccess = true;
    if ( m_config.needPublishSupport )
        totalSuccess = InitializeConnectivity() && totalSuccess;
    if ( m_config.needSubscribeSupport )
        totalSuccess = Subscribe() && totalSuccess;

    m_reconnectTimer->SetEnabled( !totalSuccess );
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnPulseCycle( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventId  ,
                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;
                      
    if ( !m_publishSuccessActionIds.empty() )
    {
        if ( !NotifyObservers( MsgsPublishedEvent, &m_publishSuccessActionEventData ) ) return;
        m_publishSuccessActionIds.clear();
    }
    if ( !m_publishFailureActionIds.empty() )
    {
        if ( !NotifyObservers( MsgsPublishFailureEvent, &m_publishFailureActionEventData ) ) return;
        m_publishFailureActionIds.clear();
    }
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::TopicMetrics::TopicMetrics( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CStoragePubSubClientTopic::TopicMetrics& 
CStoragePubSubClientTopic::GetMetrics( void ) const
{GUCEF_TRACE;

    return m_metrics;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CStoragePubSubClientTopic::GetMetricFriendlyTopicName( void ) const
{GUCEF_TRACE;

    return m_metricFriendlyTopicName;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    const COMCORE::CPubSubClientConfig& clientConfig = m_client->GetConfig();
    
    if ( clientConfig.desiredFeatures.supportsPublishing )
    {
    }
    if ( clientConfig.desiredFeatures.supportsSubscribing )
    {
    }
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CStoragePubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CStoragePubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
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
