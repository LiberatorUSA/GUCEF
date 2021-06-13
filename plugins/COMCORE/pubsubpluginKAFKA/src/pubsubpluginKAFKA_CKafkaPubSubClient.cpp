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

CKafkaPubSubClient::CKafkaPubSubClient( const COMCORE::CPubSubClientConfig& config )
    : COMCORE::CPubSubClient()
    , m_config( config )
    , m_metricsTimer( GUCEF_NULL )
    , m_topicMap()
    , m_threadPool()
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
    
    if ( config.desiredFeatures.supportsSubscribing )
        m_threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetOrCreateThreadPool( "KafkaPubSubClient(" + CORE::ToString( this ) + ")", true );

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClient::~CKafkaPubSubClient()
{GUCEF_TRACE;

    if ( !m_threadPool.IsNULL() )
        m_threadPool->RequestAllThreadsToStop( true, false );
    
    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientConfig& 
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
CKafkaPubSubClient::GetSupportedFeatures( COMCORE::CPubSubClientFeatures& features )
{GUCEF_TRACE;

    features.supportsBinaryPayloads = true;             // Redis strings are binary safe so yes redis natively supports binary data
    features.supportsPerMsgIds = true;
    features.supportsPrimaryPayloadPerMsg = false;      // We can fake this best effort but not natively supported
    features.supportsKeyValueSetPerMsg = true;          // This is the native Redis way of communicating message data
    features.supportsDuplicateKeysPerMsg = true;        // Redis does not care about duplicate keys, they are just "fields"
    features.supportsMultiHostSharding = true;          // Redis doesnt support this but clustered Redis does which is what this plugin supports
    features.supportsPublishing = true;                 // We support being a Redis producer in this plugin
    features.supportsSubscribing = true;                // We support being a Redis consumer in this plugin
    features.supportsMetrics = true;
    features.supportsAutoReconnect = true;              // Our plugin adds auto reconnect out of the box
    features.supportsBookmarkingConcept = true;         // Redis does not support this server-side but does support it via passing your "bookmark" back to Redis as an offset
    features.supportsAutoBookmarking = false;           // Redis does not support this concept. The client needs to take care of remembering the offset
    features.supportsMsgIdBasedBookmark = true;         // This is the native Redis "bookmark" method and thus preferered
    features.supportsMsgDateTimeBasedBookmark = true;   // The auto-generated msgId is a timestamp so its essentially the same thing for Redis
    return true;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic*
CKafkaPubSubClient::CreateTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CKafkaPubSubClientTopic* rcTopic = new CKafkaPubSubClientTopic( this );
    if ( rcTopic->LoadConfig( topicConfig ) )
    {
        m_topicMap[ topicConfig.topicName ] = rcTopic;
    }
    return rcTopic;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic* 
CKafkaPubSubClient::GetTopicAccess( const CORE::CString& topicName )
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
CKafkaPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        delete (*i).second;
        m_topicMap.erase( i );
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
CKafkaPubSubClient::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CKafkaPubSubClient::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::Disconnect( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::Connect( void )
{GUCEF_TRACE;

    Disconnect();

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClient::IsConnected( void )
{GUCEF_TRACE;

    return false;
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

}; /* namespace KAFKA */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
