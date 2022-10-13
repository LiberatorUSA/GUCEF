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

#ifndef PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENT_H
#include "pubsubpluginUDP_CUdpPubSubClient.h"
#define PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENT_H ? */

#include "pubsubpluginUDP_CUdpPubSubClientTopic.h"

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CUdpPubSubClientTopic::CUdpPubSubClientTopic( CUdpPubSubClient* client )
    : PUBSUB::CPubSubClientTopic( client->GetPulseGenerator() )
    , CORE::CTSharedObjCreator< CUdpPubSubClientTopic, MT::CMutex >( this )
    , m_client( client )
    , m_config()
    , m_lock()
    , m_currentPublishActionId( 1 )
    , m_currentReceiveActionId( 1 )
    , m_publishSuccessActionIds()
    , m_publishSuccessActionEventData()
    , m_publishFailureActionIds()
    , m_publishFailureActionEventData()
    , m_metrics()
    , m_metricFriendlyTopicName()
    , m_udpSocket( client->GetPulseGenerator(), false )
{GUCEF_TRACE;
        
    m_publishSuccessActionEventData.LinkTo( &m_publishSuccessActionIds );
    m_publishFailureActionEventData.LinkTo( &m_publishFailureActionIds );

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientTopic::~CUdpPubSubClientTopic()
{GUCEF_TRACE;

    Shutdown();
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::Shutdown( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_client = GUCEF_NULL;
    
    Disconnect();
    UnsubscribeFrom( &m_udpSocket );
    SignalUpcomingDestruction();
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClient*
CUdpPubSubClientTopic::GetClient( void )
{GUCEF_TRACE;

    return m_client;
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Register UDP socket event handlers
    TEventCallback callback( this, &CUdpPubSubClientTopic::OnUDPSocketError );
    SubscribeTo( &m_udpSocket                             ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback                                 );
    TEventCallback callback2( this, &CUdpPubSubClientTopic::OnUDPSocketClosed );
    SubscribeTo( &m_udpSocket                              ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback2                                 );
    TEventCallback callback3( this, &CUdpPubSubClientTopic::OnUDPSocketClosing );
    SubscribeTo( &m_udpSocket                               ,
                 COMCORE::CUDPSocket::UDPSocketClosingEvent ,
                 callback3                                  );
    TEventCallback callback4( this, &CUdpPubSubClientTopic::OnUDPSocketOpened );
    SubscribeTo( &m_udpSocket                              ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback4                                 );
    TEventCallback callback5( this, &CUdpPubSubClientTopic::OnUDPPacketsRecieved );
    SubscribeTo( &m_udpSocket                                 ,
                 COMCORE::CUDPSocket::UDPPacketsRecievedEvent ,
                 callback5                                    );

    TEventCallback callback6( this, &CUdpPubSubClientTopic::OnPulseCycle );
    SubscribeTo( m_client->GetConfig().pulseGenerator.GetPointerAlways() ,
                 CORE::CPulseGenerator::PulseEvent                       ,
                 callback6                                               );
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return m_config.needSubscribeSupport;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CUdpPubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool 
CUdpPubSubClientTopic::Publish( CORE::UInt64& publishActionId, const PUBSUB::CIPubSubMsg& msg, bool notify )
{GUCEF_TRACE;
    
    MT::CScopeMutex lock( m_lock );

    if ( 0 == publishActionId )
    {
        publishActionId = m_currentPublishActionId; 
        ++m_currentPublishActionId;
    }

    bool success = false;
    
    const CORE::CVariant& payload = msg.GetPrimaryPayload();

    if ( m_config.maxUdpPacketPayloadSizeInBytes >= payload.ByteSize() )
    {
        CORE::Int32 bytesSent = m_udpSocket.SendPacketTo( m_config.udpInterface, payload.AsVoidPtr(), payload.ByteSize() );
        if ( bytesSent == payload.ByteSize() )
            success = true;
    }

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
CUdpPubSubClientTopic::SetupToSubscribe( PUBSUB::CPubSubClientTopicConfig& config )
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
CUdpPubSubClientTopic::SaveConfig( PUBSUB::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CUdpPubSubClientTopic::GenerateMetricsFriendlyTopicName( const CORE::CString& topicName )
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
CUdpPubSubClientTopic::LoadConfig( const PUBSUB::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;

    m_metricFriendlyTopicName = GenerateMetricsFriendlyTopicName( m_config.topicName );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    m_udpSocket.Close();
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( m_udpSocket.Open( m_config.udpInterface ) )
    {
		GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpPubSubClientTopic:Subscribe: Successfully opened UDP socket on " + m_config.udpInterface.AddressAndPortAsString() );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic:Subscribe: Failed to open UDP socket on " + m_config.udpInterface.AddressAndPortAsString() );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CUdpPubSubClientTopic::SubscribeStartingAtBookmark( const PUBSUB::CPubSubBookmark& bookmark ) 
{GUCEF_TRACE;

    // Not possible with UDP: Not supported
    return false;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubBookmark 
CUdpPubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    // Not possible with UDP: Not supported
    return PUBSUB::CPubSubBookmark( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_NOT_APPLICABLE );    
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopic::AcknowledgeReceipt( const PUBSUB::CIPubSubMsg& msg )
{GUCEF_TRACE;

    // Not possible with UDP: Not supported
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopic::AcknowledgeReceipt( const PUBSUB::CPubSubBookmark& bookmark )
{GUCEF_TRACE;
    
    // Not possible with UDP: Not supported
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CUdpPubSubClientTopic::IsConnected( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_udpSocket.IsActive();           
}

/*-------------------------------------------------------------------------*/

bool 
CUdpPubSubClientTopic::IsHealthy( void ) const
{GUCEF_TRACE;

    // @TODO
    MT::CScopeMutex lock( m_lock );
    return true;           
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( GUCEF_NULL == m_client )
        return false;

    m_udpSocket.SetMaxUpdatesPerCycle( m_config.udpSocketUpdateCyclesPerPulse );
    m_udpSocket.SetAutoReOpenOnError( m_client->GetConfig().desiredFeatures.supportsAutoReconnect );

    if ( m_config.needSubscribeSupport )
    {
        m_udpSocket.SetOsLevelSocketReceiveBufferSize( m_config.udpSocketOsReceiveBufferSize );
        m_udpSocket.SetNrOfReceiveBuffers( m_config.nrOfUdpReceiveBuffersPerSocket );
    }
    else
    {
        // Since we dont aim to receive lets keep the memory footprint low
        m_udpSocket.SetOsLevelSocketReceiveBufferSize( 10000 );
        m_udpSocket.SetNrOfReceiveBuffers( 1 );
    }
    
    return true;
}

/*-------------------------------------------------------------------------*/

const CUdpPubSubClientTopicConfig& 
CUdpPubSubClientTopic::GetTopicConfig( void ) const
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::OnPulseCycle( CORE::CNotifier* notifier    ,
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

CUdpPubSubClientTopic::TopicMetrics::TopicMetrics( void )
    : udpBytesReceived( 0 )
    , udpPacketsReceived( 0 )
    , udpBytesSent( 0 )
    , udpPacketsSent( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CUdpPubSubClientTopic::TopicMetrics& 
CUdpPubSubClientTopic::GetMetrics( void ) const
{GUCEF_TRACE;

    return m_metrics;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CUdpPubSubClientTopic::GetMetricFriendlyTopicName( void ) const
{GUCEF_TRACE;

    return m_metricFriendlyTopicName;
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::OnUDPSocketError( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventID  ,
                                         CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::TSocketErrorEventData* eData = static_cast< COMCORE::CUDPSocket::TSocketErrorEventData* >( eventData );
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "): UDP Socket experienced error " + CORE::Int32ToString( eData->GetData() ) );
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::OnUDPSocketClosed( CORE::CNotifier* notifier   ,
                                          const CORE::CEvent& eventID ,
                                          CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "): UDP Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::OnUDPSocketClosing( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventID  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "): UDP Socket is going to close" );

    // Gracefully leave the multicast groups we joined (if any)
    CUdpPubSubClientTopicConfig::HostAddressVector::iterator m = m_config.udpMulticastToJoin.begin();
    while ( m != m_config.udpMulticastToJoin.end() )
    {
        const COMCORE::CHostAddress& multicastAddr = (*m);
        if ( m_udpSocket.Leave( multicastAddr ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "):OnUDPSocketClosing: Successfully to left multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket bound to " + m_config.udpInterface.AddressAndPortAsString() );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "):OnUDPSocketClosing: Failed to leave multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket bound to " + m_config.udpInterface.AddressAndPortAsString() );
        }
        ++m;
    }
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::OnUDPSocketOpened( CORE::CNotifier* notifier   ,
                                          const CORE::CEvent& eventID ,
                                          CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "): UDP Socket has been opened" );

    CUdpPubSubClientTopicConfig::HostAddressVector::iterator m = m_config.udpMulticastToJoin.begin();
    while ( m != m_config.udpMulticastToJoin.end() )
    {
        const COMCORE::CHostAddress& multicastAddr = (*m);
        if ( m_udpSocket.Join( multicastAddr ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "):OnUDPSocketOpened: Successfully to joined multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket bound to " + m_config.udpInterface.AddressAndPortAsString() );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "):OnUDPSocketOpened: Failed to join multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket bound to " + m_config.udpInterface.AddressAndPortAsString() );
        }
        ++m;
    }
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::PrepStorageForReadMsgs( CORE::UInt32 msgCount )
{GUCEF_TRACE;

    // reset size, note this does not dealloc the underlying memory
    m_pubsubMsgsRefs.clear();

    if ( msgCount > m_pubsubMsgs.size() )
    {
        // Grow the actual storage, this is allowed to become larger than msgCount to limit
        // the nr of dynamic allocations
        m_pubsubMsgs.resize( msgCount );        
        m_pubsubMsgsRefs.reserve( msgCount );
    }
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::OnUDPPacketsRecieved( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventID  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketsRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketsRecievedEventData* >( eventData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUdpPacketsRecievedEventData& data = udpPacketData->GetData();

        CORE::UInt32 packetsReceived = data.packetsReceived;
        if ( packetsReceived > data.packets.size() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "):OnUDPPacketsRecieved: Invalid packet count of " +
                CORE::UInt32ToString( packetsReceived ) + " which exceeds storage size of " + CORE::UInt64ToString( data.packets.size() ) +
                ". This should never happen or be possible. Will clamp to smaller nr." );
            packetsReceived = (CORE::UInt32) data.packets.size();
        }

        // Make sure we have enough storage to construct our generic representations
        PrepStorageForReadMsgs( packetsReceived );
        TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs;
        for ( CORE::UInt32 i=0; i<packetsReceived; ++i )
        {
            const COMCORE::CUDPSocket::TPacketEntry& packetEntry = data.packets[ i ];
            
            PUBSUB::CBasicPubSubMsg& msgWrapper = m_pubsubMsgs[ i ];
            msgWrapper.Clear();
            msgWrapper.SetOriginClientTopic( CreateSharedPtr() );
            msgWrapper.SetReceiveActionId( m_currentReceiveActionId );
            ++m_currentReceiveActionId;

            if ( m_config.addTimestampToReceivedPackages )
                msgWrapper.GetMsgDateTime() = CORE::CDateTime::NowLocalDateTime();

            msgWrapper.GetPrimaryPayload().LinkTo( packetEntry.dataBuffer.GetData() );

            if ( m_config.addUdpSourceAddressAsMetaData )
            {
                if ( m_config.translateUdpSourceAddressToString )
                    msgWrapper.AddMetaDataKeyValuePair( m_config.udpSourceAddressAsMetaDataKeyName, packetEntry.sourceAddress.AddressAndPortAsString() );
                else
                    msgWrapper.AddMetaDataKeyValuePair( m_config.udpSourceAddressAsMetaDataKeyName, CORE::ToString( packetEntry.sourceAddress.GetAddressInHostByteOrder() ) );
            }
            
            // Now that we have prepared our wrapped message let's link it
            // as a reference in the list of received messages we send out
            m_pubsubMsgsRefs.push_back( CPubSubClientTopic::TPubSubMsgRef() );
            m_pubsubMsgsRefs.back().LinkTo( &msgWrapper );
        }
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "): Received and mapped " + CORE::ToString( packetsReceived ) + " packets" );

        // Communicate all the messages received via an event notification
        NotifyObservers( MsgsRecievedEvent, &m_pubsubMsgsRefs );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpPubSubClientTopic(" + CORE::ToString( this ) + "): UDP Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

void
CUdpPubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                            const CORE::CEvent& eventId  ,
                                            CORE::CICloneable* eventData )
{GUCEF_TRACE;

    m_metrics.udpBytesReceived = m_udpSocket.GetBytesReceived( true );
    m_metrics.udpPacketsReceived = m_udpSocket.GetNrOfDataReceivedEvents( true );
    m_metrics.udpBytesSent = m_udpSocket.GetBytesTransmitted( true );
    m_metrics.udpPacketsSent = m_udpSocket.GetNrOfDataSentEvents( true );
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CUdpPubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CUdpPubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CUdpPubSubClientTopic::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::PUBSUBPLUGIN::UDP::CUdpPubSubClientTopic";
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
