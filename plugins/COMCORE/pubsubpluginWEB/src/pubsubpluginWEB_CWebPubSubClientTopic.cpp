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

#ifndef PUBSUBPLUGIN_UDP_CWEBPUBSUBCLIENT_H
#include "pubsubpluginWEB_CWebPubSubClient.h"
#define PUBSUBPLUGIN_UDP_CWEBPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_UDP_CWEBPUBSUBCLIENT_H ? */

#include "pubsubpluginWEB_CWebPubSubClientTopic.h"

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CWebPubSubClientTopic::CWebPubSubClientTopic( CWebPubSubClient* client )
    : COMCORE::CPubSubClientTopic()
    , m_client( client )
    , m_config()
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
    , m_httpServer( *m_client->GetConfig().pulseGenerator )
    , m_httpRouter()
    , m_publishedMsgs()
{GUCEF_TRACE;
        
    m_publishSuccessActionEventData.LinkTo( &m_publishSuccessActionIds );
    m_publishFailureActionEventData.LinkTo( &m_publishFailureActionIds );

    if ( m_client->GetConfig().desiredFeatures.supportsAutoReconnect )
    {
        m_reconnectTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, m_client->GetConfig().reconnectDelayInMs );
    }

 //   m_httpRouter.SetResourceMapping( "/messages", RestApiPubSubClientChannelConfigResource::THTTPServerResourcePtr( new RestApiPubSubClientChannelConfigResource( this ) )  );
    
 //   if ( m_config.exposeBasicHealthEndpoint )
 //       m_httpRouter.SetResourceMapping( m_config.basicHealthEndpointPath, RestApiPubSub2PubSubConfigResource::THTTPServerResourcePtr( new WEB::CDummyHTTPServerResource() ) );
    
    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CWebPubSubClientTopic::~CWebPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();
    m_httpRouter.RemoveAllResourceMappings();

    delete m_reconnectTimer;
    m_reconnectTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClient*
CWebPubSubClientTopic::GetClient( void )
{GUCEF_TRACE;

    return m_client;
}

/*-------------------------------------------------------------------------*/

void
CWebPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_reconnectTimer )
    {
        TEventCallback callback( this, &CWebPubSubClientTopic::OnReconnectTimerCycle );
        SubscribeTo( m_reconnectTimer               ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }

    TEventCallback callback( this, &CWebPubSubClientTopic::OnPulseCycle );
    SubscribeTo( m_client->GetConfig().pulseGenerator ,
                 CORE::CPulseGenerator::PulseEvent    ,
                 callback                             );
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return m_config.needSubscribeSupport;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CWebPubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool 
CWebPubSubClientTopic::Publish( CORE::UInt64& publishActionId, const COMCORE::CIPubSubMsg& msg, bool notify )
{GUCEF_TRACE;
    
    bool success = false;
    
    const COMCORE::CPubSubClient* originClient = GUCEF_NULL;
    const COMCORE::CPubSubClientTopic* originClientTopic = msg.GetOriginClientTopic();
    if ( GUCEF_NULL != originClientTopic )
    {       
        originClient = originClientTopic->GetClient();
    }
    COMCORE::CIPubSubMsg::TNoLockSharedPtr msgClone( static_cast< COMCORE::CIPubSubMsg* >( msg.Clone() ) );
        
    MT::CScopeMutex lock( m_lock );

    if ( 0 == publishActionId )
    {
        publishActionId = m_currentPublishActionId;
        ++m_currentPublishActionId;
    }

    m_publishedMsgs[ originClient ][ originClientTopic ][ publishActionId ] = msgClone;

    success = true;

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
CWebPubSubClientTopic::SetupToSubscribe( COMCORE::CPubSubClientTopicConfig& config )
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
CWebPubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CWebPubSubClientTopic::GenerateMetricsFriendlyTopicName( const CORE::CString& topicName )
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
CWebPubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;

    m_metricFriendlyTopicName = GenerateMetricsFriendlyTopicName( m_config.topicName );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    m_httpServer.Close();
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_httpServer.ListenOnPort( m_config.httpServerPort );
}

/*-------------------------------------------------------------------------*/

bool 
CWebPubSubClientTopic::SubscribeStartingAtBookmark( const COMCORE::CPubSubBookmark& bookmark ) 
{GUCEF_TRACE;

    // Not supported. We are dependent on submissions we dont control
    return false;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubBookmark 
CWebPubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    // Not supported
    return COMCORE::CPubSubBookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_NOT_APPLICABLE );    
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::AcknowledgeReceipt( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    COMCORE::CPubSubBookmark bookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_TOPIC_INDEX, msg.GetMsgIndex() );
    return AcknowledgeReceiptImpl( bookmark, msg.GetReceiveActionId() );
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::AcknowledgeReceipt( const COMCORE::CPubSubBookmark& bookmark )
{GUCEF_TRACE;
    
    return AcknowledgeReceiptImpl( bookmark, 0 );
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::AcknowledgeReceiptImpl( const COMCORE::CPubSubBookmark& bookmark , 
                                               CORE::UInt64 receiveActionId             )
{GUCEF_TRACE;

 
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CWebPubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_httpServer.IsActive();           
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_httpServer.ListenOnPort( m_config.httpServerPort );
}

/*-------------------------------------------------------------------------*/

const CWebPubSubClientTopicConfig& 
CWebPubSubClientTopic::GetTopicConfig( void ) const
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

void
CWebPubSubClientTopic::OnReconnectTimerCycle( CORE::CNotifier* notifier    ,
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
CWebPubSubClientTopic::OnPulseCycle( CORE::CNotifier* notifier    ,
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

CWebPubSubClientTopic::TopicMetrics::TopicMetrics( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CWebPubSubClientTopic::TopicMetrics& 
CWebPubSubClientTopic::GetMetrics( void ) const
{GUCEF_TRACE;

    return m_metrics;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CWebPubSubClientTopic::GetMetricFriendlyTopicName( void ) const
{GUCEF_TRACE;

    return m_metricFriendlyTopicName;
}

/*-------------------------------------------------------------------------*/

void
CWebPubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CWebPubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CWebPubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
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
