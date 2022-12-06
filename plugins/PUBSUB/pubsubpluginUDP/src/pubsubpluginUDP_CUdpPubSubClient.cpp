/*
 *  pubsubpluginUDP: Generic GUCEF COMCORE plugin for providing pubsub approximation via UDP
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

#ifndef PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTCONFIG_H
#include "pubsubpluginUDP_CUdpPubSubClientConfig.h"
#define PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTCONFIG_H
#endif /* PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTCONFIG_H ? */

#include "pubsubpluginUDP_CUdpPubSubClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace UDP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CUdpPubSubClient::TypeName = "UDP";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CUdpPubSubClient::CUdpPubSubClient( const PUBSUB::CPubSubClientConfig& config )
    : PUBSUB::CPubSubClient()
    , m_config()
    , m_metricsTimer( GUCEF_NULL )
    , m_topicMap()
    , m_testUdpSocket( GUCEF_NULL )
{GUCEF_TRACE;

    if ( !LoadConfig( config ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClient: Failed to load config at construction" );
    }

    if ( m_config.pulseGenerator.IsNULL() )
        m_config.pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();

    if ( m_config.desiredFeatures.supportsMetrics )
    {
        m_metricsTimer = GUCEF_NEW CORE::CTimer( m_config.pulseGenerator, 1000 );
        m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
    }

    if ( m_config.transmitTestPackets )
    {
        m_testUdpSocket = GUCEF_NEW COMCORE::CUDPSocket( m_config.pulseGenerator, false );
        m_testPacketTransmitTimer = GUCEF_NEW CORE::CTimer( m_config.pulseGenerator, m_config.testPacketTransmissionIntervalInMs );
        m_testPacketTransmitTimer->SetEnabled( m_config.transmitTestPackets );
    }

    m_config.metricsPrefix += "udp.";

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClient::~CUdpPubSubClient()
{GUCEF_TRACE;
    
    Disconnect();

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        (*i).second->Shutdown();
        (*i).second.Unlink();
        ++i;
    }
    m_topicMap.clear();
    
    GUCEF_DELETE m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    GUCEF_DELETE m_testPacketTransmitTimer;
    m_testPacketTransmitTimer = GUCEF_NULL;

    GUCEF_DELETE m_testUdpSocket;
    m_testUdpSocket = GUCEF_NULL;

    SignalUpcomingDestruction();
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientConfig& 
CUdpPubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClient::GetSupportedFeatures( PUBSUB::CPubSubClientFeatures& features ) const
{GUCEF_TRACE;

    features.supportsBinaryPayloads = true;             // UDP supports a binary payload natively
    features.supportsPerMsgIds = false;                 // UDP has no such concept
    features.supportsPrimaryPayloadPerMsg = true;       // UDP only has the primary payload natively
    features.supportsAbsentPrimaryPayloadPerMsg = true; // UDP datagram header only
    features.supportsKeyValueSetPerMsg = false;         // UDP does not itself support structured data
    features.supportsDuplicateKeysPerMsg = false;       // UDP does not itself support structured data
    features.supportsMetaDataKeyValueSetPerMsg = false; // UDP does not itself support structured data
    features.supportsMultiHostSharding = false;         // Volatile and oft point-to-point so we will say no on this one
    features.supportsPublishing = true;                 // We support sending UDP packets
    features.supportsSubscribing = true;                // We support receiving UDP packets
    features.supportsMetrics = true;                    // This plugin has support for reporting its own set of metrics
    features.supportsAutoReconnect = true;              // UDP socket class provides this option wrt auto-reopen-on-error and such
    features.supportsAckUsingBookmark = false;          // UDP is fire-and-forget: not supported
    features.supportsSubscriberMsgReceivedAck = false;  // UDP is fire-and-forget: not supported
    features.supportsAutoMsgReceivedAck = false;        // UDP is fire-and-forget: not supported
    features.supportsAbsentMsgReceivedAck = false;      // UDP is fire-and-forget: not supported
    features.supportsAckUsingLastMsgInBatch = false;    // UDP is fire-and-forget: not supported
    features.supportsBookmarkingConcept = false;        // UDP is fire-and-forget: not supported
    features.supportsAutoBookmarking = false;           // UDP is fire-and-forget: not supported
    features.supportsMsgIdBasedBookmark = false;        // UDP is fire-and-forget: not supported
    features.supportsMsgIndexBasedBookmark = false;     // UDP is fire-and-forget: not supported
    features.supportsMsgDateTimeBasedBookmark = false;  // UDP is fire-and-forget: not supported
    features.supportsServerSideBookmarkPersistance = false; // UDP is fire-and-forget: not supported    
    features.supportsSubscribingUsingBookmark = false;  // UDP is fire-and-forget: not supported
    features.supportsTopicIndexBasedBookmark = false;   // UDP is fire-and-forget: not supported
    features.supportsDerivingBookmarkFromMsg = false;   // UDP is fire-and-forget: not supported
    features.supportsDiscoveryOfAvailableTopics = false; // UDP has no such concept 
    features.supportsGlobPatternTopicNames = false;
    features.supportsSubscriptionMsgArrivalDelayRequests = false;
    features.supportsSubscriptionEndOfDataEvent = false;

    return true;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopicPtr
CUdpPubSubClient::CreateTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CUdpPubSubClientTopicPtr topicAccess;
    {
        MT::CObjectScopeLock lock( this );

        topicAccess = ( GUCEF_NEW CUdpPubSubClientTopic( this ) )->CreateSharedPtr();
        if ( topicAccess->LoadConfig( topicConfig ) )
        {
            m_topicMap[ topicConfig.topicName ] = topicAccess;
        }
        else
        {
            topicAccess->Shutdown();
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
CUdpPubSubClient::GetTopicAccess( const CORE::CString& topicName )
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
CUdpPubSubClient::GetAllCreatedTopicAccess( PubSubClientTopicSet& topicAccess )
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
CUdpPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        CUdpPubSubClientTopicPtr topicAccess = (*i).second;
        m_topicMap.erase( i );

        TopicAccessDestroyedEventData eData( topicName );
        NotifyObservers( TopicAccessDestroyedEvent, &eData );
        
        topicAccess->Shutdown();
        topicAccess.Unlink();        
    }
}

/*-------------------------------------------------------------------------*/

const PUBSUB::CPubSubClientTopicConfig* 
CUdpPubSubClient::GetTopicConfig( const CORE::CString& topicName )
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
    return &m_config.defaultTopicConfig;
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
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
CUdpPubSubClient::GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList )
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
CUdpPubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClient::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    return m_config.SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClient::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
{GUCEF_TRACE;

    return m_config.SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClient::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    CUdpPubSubClientConfig parsedCfg;
    if ( parsedCfg.LoadConfig( cfg ) )
    {
        m_config = parsedCfg;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClient::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg  )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    CUdpPubSubClientConfig parsedCfg;
    if ( parsedCfg.LoadConfig( cfg ) )
    {
        m_config = parsedCfg;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClient::Disconnect( void )
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
CUdpPubSubClient::Connect( void )
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
CUdpPubSubClient::IsConnected( void ) const
{GUCEF_TRACE;

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
CUdpPubSubClient::IsHealthy( void ) const
{GUCEF_TRACE;

    if ( !m_topicMap.empty() )
    {
        bool allHealthy = true;
        TTopicMap::const_iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            allHealthy = (*i).second->IsHealthy() && allHealthy;
            ++i;
        }
        return allHealthy;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClient::IsInitialized( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClient::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CUdpPubSubClient::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }

    if ( GUCEF_NULL != m_testPacketTransmitTimer )
    {
        TEventCallback callback( this, &CUdpPubSubClient::OnTransmitTestPacketTimerCycle );
        SubscribeTo( m_testPacketTransmitTimer      ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }    
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClient::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
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
        CUdpPubSubClientTopicPtr topic = (*i).second;
        const CUdpPubSubClientTopic::TopicMetrics& topicMetrics = topic->GetMetrics();
        const CORE::CString& topicName = topic->GetMetricFriendlyTopicName();
        const CUdpPubSubClientTopicConfig& topicConfig = topic->GetTopicConfig();
        CORE::CString metricsPrefix = m_config.metricsPrefix + topicName;

        if ( topicConfig.needSubscribeSupport )
        {
            GUCEF_METRIC_TIMING( metricsPrefix + "udpBytesReceived", topicMetrics.udpBytesReceived, 1.0f );
            GUCEF_METRIC_TIMING( metricsPrefix + "udpPacketsReceived", topicMetrics.udpPacketsReceived, 1.0f );
        }
        if ( topicConfig.needPublishSupport )
        {
            GUCEF_METRIC_TIMING( metricsPrefix + "udpBytesSent", topicMetrics.udpBytesSent, 1.0f );
            GUCEF_METRIC_TIMING( metricsPrefix + "udpPacketsSent", topicMetrics.udpPacketsSent, 1.0f );
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClient::OnTransmitTestPacketTimerCycle( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        CUdpPubSubClientTopicPtr topic = (*i).second;
        const CUdpPubSubClientTopicConfig& topicConfig = topic->GetTopicConfig();

        if ( topicConfig.wantsTestPackage )
        {
            m_testUdpSocket->SendPacketTo( topicConfig.udpInterface.GetFirstIPv4Address(), 
                                           m_config.testPacket.GetBufferPtr(), 
                                           (CORE::UInt16) m_config.testPacket.GetDataSize() );
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CUdpPubSubClient::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::PUBSUBPLUGIN::UDP::CUdpPubSubClient";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace UDP */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
