/*
 *  Udp2Redis: service which pushes UDP packets into Redis
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefCOM_CDummyHTTPServerResource.h"
#define GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H ? */

#include "UdpViaTcp.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <winsock2.h>
#endif

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

RestApiUdpViaTcpInfoResource::RestApiUdpViaTcpInfoResource( UdpViaTcp* app )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiUdpViaTcpInfoResource::~RestApiUdpViaTcpInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiUdpViaTcpInfoResource::Serialize( CORE::CDataNode& output             ,
                                         const CORE::CString& representation )
{GUCEF_TRACE;

    output.SetName( "info" );
    output.SetAttribute( "application", "UdpViaTcp" );
    output.SetAttribute( "buildDateTime", __TIMESTAMP__ );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiUdpViaTcpConfigResource::RestApiUdpViaTcpConfigResource( UdpViaTcp* app, bool appConfig )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiUdpViaTcpConfigResource::~RestApiUdpViaTcpConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiUdpViaTcpConfigResource::Serialize( CORE::CDataNode& output             ,
                                           const CORE::CString& representation )
{GUCEF_TRACE;

    if ( m_appConfig )
    {
        const CORE::CValueList& loadedConfig = m_app->GetAppConfig();
        return loadedConfig.SaveConfig( output );
    }

    const CORE::CDataNode& globalConfig = m_app->GetGlobalConfig();
    output.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

UdpViaTcp::UdpViaTcp( void )
    : m_tcpServerSocket( false )
    , m_tcpClientSocket( false )
    , m_udpTransmitSocket( false )
    , m_udpReceiveSocket( false )
    , m_tcpClientSendPacketBuffers()
    , m_tcpServerReceivePacketBuffers()
    , m_udpReceiveUnicast( true )
    , m_udpReceiveMulticast( false )
    , m_mode( UDPVIATCPMODE_UDP_RECEIVER_ONLY )
    , m_tcpDestination()
    , m_tcpReceiver()
    , m_udpDestinations()
    , m_udpReceiver()
    , m_udpTransmitter()
    , m_udpReceiverMulticastSources()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_transmitMetrics( true )
{GUCEF_TRACE;

    RegisterEventHandlers();    
}

/*-------------------------------------------------------------------------*/

UdpViaTcp::~UdpViaTcp()
{GUCEF_TRACE;

    m_httpServer.Close();
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Register UDP socket event handlers
    TEventCallback callback( this, &UdpViaTcp::OnUDPReceiveSocketError );
    SubscribeTo( &m_udpReceiveSocket                      ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback                                 );
    TEventCallback callback2( this, &UdpViaTcp::OnUDPReceiveSocketClosed );
    SubscribeTo( &m_udpReceiveSocket                       ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback2                                 );
    TEventCallback callback3( this, &UdpViaTcp::OnUDPReceiveSocketOpened );
    SubscribeTo( &m_udpReceiveSocket                       ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback3                                 );
    TEventCallback callback4( this, &UdpViaTcp::OnUDPReceiveSocketPacketRecieved );
    SubscribeTo( &m_udpReceiveSocket                         ,
                 COMCORE::CUDPSocket::UDPPacketRecievedEvent ,
                 callback4                                   );


    TEventCallback callback5( this, &UdpViaTcp::OnUDPTransmitSocketError );
    SubscribeTo( &m_udpTransmitSocket                     ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback5                                );
    TEventCallback callback6( this, &UdpViaTcp::OnUDPTransmitSocketClosed );
    SubscribeTo( &m_udpTransmitSocket                      ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback6                                 );
    TEventCallback callback7( this, &UdpViaTcp::OnUDPTransmitSocketOpened );
    SubscribeTo( &m_udpTransmitSocket                      ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback7                                 );
    TEventCallback callback8( this, &UdpViaTcp::OnUDPTransmitSocketPacketRecieved );
    SubscribeTo( &m_udpTransmitSocket                        ,
                 COMCORE::CUDPSocket::UDPPacketRecievedEvent ,
                 callback8                                   );


    TEventCallback callback9( this, &UdpViaTcp::OnTCPServerClientConnected );
    SubscribeTo( &m_tcpServerSocket                              ,
                 COMCORE::CTCPServerSocket::ClientConnectedEvent ,
                 callback9                                       );
    TEventCallback callback10( this, &UdpViaTcp::OnTCPServerClientDisconnected );
    SubscribeTo( &m_tcpServerSocket                                 ,
                 COMCORE::CTCPServerSocket::ClientDisconnectedEvent ,
                 callback10                                         );
    TEventCallback callback11( this, &UdpViaTcp::OnTCPServerClientError );
    SubscribeTo( &m_tcpServerSocket                          ,
                 COMCORE::CTCPServerSocket::ClientErrorEvent ,
                 callback11                                  );
    TEventCallback callback12( this, &UdpViaTcp::OnTCPServerSocketOpened );
    SubscribeTo( &m_tcpServerSocket                                 ,
                 COMCORE::CTCPServerSocket::ServerSocketOpenedEvent ,
                 callback12                                         );
    TEventCallback callback13( this, &UdpViaTcp::OnTCPServerSocketClosed );
    SubscribeTo( &m_tcpServerSocket                                 ,
                 COMCORE::CTCPServerSocket::ServerSocketClosedEvent ,
                 callback13                                         );
    TEventCallback callback14( this, &UdpViaTcp::OnTCPServerSocketError );
    SubscribeTo( &m_tcpServerSocket                                ,
                 COMCORE::CTCPServerSocket::ServerSocketErrorEvent ,
                 callback14                                        );
    TEventCallback callback15( this, &UdpViaTcp::OnTCPServerSocketClientError );
    SubscribeTo( &m_tcpServerSocket                                      ,
                 COMCORE::CTCPServerSocket::ServerSocketClientErrorEvent ,
                 callback15                                              );
    TEventCallback callback16( this, &UdpViaTcp::OnTCPServerSocketMaxConnectionsChanged );
    SubscribeTo( &m_tcpServerSocket                                                ,
                 COMCORE::CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent ,
                 callback16                                                        );

    TEventCallback callback17( this, &UdpViaTcp::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback17                     );

    TEventCallback callback18( this, &UdpViaTcp::OnTCPClientConnected );
    SubscribeTo( &m_tcpClientSocket                        ,
                 COMCORE::CTCPClientSocket::ConnectedEvent ,
                 callback18                                );

}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_transmitMetrics )
    {
        if ( UDPVIATCPMODE_BIDIRECTIONAL_UDP == m_mode || UDPVIATCPMODE_UDP_TRANSMITTER_ONLY == m_mode )
        {
            GUCEF_METRIC_GAUGE( "UdpViaTcp.TcpServer.ConnectedClients", m_tcpServerSocket.GetActiveCount(), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.TcpServer.BytesReceived", m_tcpServerSocket.GetBytesReceived( true ), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.UdpTransmitter.BytesSent", m_udpTransmitSocket.GetBytesTransmitted( true ), 1.0f );
        }
        if ( UDPVIATCPMODE_BIDIRECTIONAL_UDP == m_mode || UDPVIATCPMODE_UDP_RECEIVER_ONLY == m_mode )
        {
            GUCEF_METRIC_COUNT( "UdpViaTcp.TcpClient.BufferedSendDataInBytes", m_tcpClientSocket.GetBufferedDataToSendInBytes(), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.TcpClient.BytesSent", m_tcpClientSocket.GetBytesTransmitted( true ), 1.0f );
            GUCEF_METRIC_GAUGE( "UdpViaTcp.TcpClient.PackagesQueued", m_tcpClientSendPacketBuffers.size(), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.UdpReceiver.BytesReceived", m_udpReceiveSocket.GetBytesReceived( true ), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.UdpReceiver.MsgsReceived", m_udpReceiveSocket.GetNrOfDataReceivedEvents( true ), 1.0f );
        }
        if ( UDPVIATCPMODE_BIDIRECTIONAL_UDP == m_mode )
        {
            GUCEF_METRIC_COUNT( "UdpViaTcp.TcpServer.BytesSent", m_tcpServerSocket.GetBytesTransmitted( true ), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.TcpClient.BytesReceived", m_tcpClientSocket.GetBytesReceived( true ), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.UdpTransmitter.BytesReceived", m_udpTransmitSocket.GetBytesReceived( true ), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.UdpTransmitter.MsgsReceived", m_udpTransmitSocket.GetNrOfDataReceivedEvents( true ), 1.0f );
            GUCEF_METRIC_COUNT( "UdpViaTcp.UdpReceiver.BytesSent", m_udpReceiveSocket.GetBytesTransmitted( true ), 1.0f );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnUDPReceiveSocketError( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventID  ,
                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::TSocketErrorEventData* eData = static_cast< COMCORE::CUDPSocket::TSocketErrorEventData* >( eventData );    
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Receive Socket experienced error " + CORE::Int32ToString( eData->GetData() ) );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnUDPReceiveSocketClosed( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventID  ,
                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Receive Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnUDPReceiveSocketOpened( CORE::CNotifier* notifier   ,
                                     const CORE::CEvent& eventID ,
                                     CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Receive Socket has been opened" );

    if ( m_udpReceiveMulticast )
    {
        CORE::CValueList::TStringVector::iterator i = m_udpReceiverMulticastSources.begin();
        while ( i != m_udpReceiverMulticastSources.end() )
        {
            COMCORE::CHostAddress multicastGroup;
            multicastGroup.SetHostname( (*i) );
            m_udpReceiveSocket.Join( multicastGroup );
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnUDPReceiveSocketPacketRecieved( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventID  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketRecievedEventData* >( eventData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUDPPacketRecievedEventData& data = udpPacketData->GetData();
        const CORE::CDynamicBuffer& udpPacketBuffer = data.dataBuffer.GetData();

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp: UDP Receive Socket received a packet from " + data.sourceAddress.AddressAndPortAsString() );

        char packetHeader[ 7 ]; 
        memcpy( packetHeader, "UDP", 3 );
        
        bool successfullSend = false;
        bool failedToSendQueued = false;
        if ( m_tcpClientSocket.IsActive() )
        {            
            while ( !m_tcpClientSendPacketBuffers.empty() )
            {
                CORE::CDynamicBuffer& packet = m_tcpClientSendPacketBuffers.front();
                CORE::UInt32 packetSize = packet.GetDataSize();
                memcpy( packetHeader+3, &packetSize, 4 );
                if ( m_tcpClientSocket.Send( packetHeader, 7 ) )
                {
                    if ( m_tcpClientSocket.Send( packet.GetConstBufferPtr(), packetSize ) )
                    {
                        m_tcpClientSendPacketBuffers.pop();
                    }
                    else
                    {
                        failedToSendQueued = true;
                        break;
                    }
                }
                else
                {
                    failedToSendQueued = true;
                    break;
                }
            }

            if ( !failedToSendQueued )
            {
                CORE::UInt32 packetSize = udpPacketBuffer.GetDataSize();
                memcpy( packetHeader+3, &packetSize, 4 );
                if ( m_tcpClientSocket.Send( packetHeader, 7 ) )
                    if ( m_tcpClientSocket.Send( udpPacketBuffer.GetConstBufferPtr(), packetSize ) )
                        successfullSend = true;
            }
        }
        if ( !successfullSend )
        {
            m_tcpClientSendPacketBuffers.push( udpPacketBuffer );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Receive Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnUDPTransmitSocketError( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventID  ,
                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::TSocketErrorEventData* eData = static_cast< COMCORE::CUDPSocket::TSocketErrorEventData* >( eventData );    
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Transmit Socket experienced error " + CORE::Int32ToString( eData->GetData() ) );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnUDPTransmitSocketClosed( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventID  ,
                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Transmit Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnUDPTransmitSocketOpened( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventID  ,
                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Transmit Socket has been opened" );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnUDPTransmitSocketPacketRecieved( CORE::CNotifier* notifier   ,
                                              const CORE::CEvent& eventID ,
                                              CORE::CICloneable* evenData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketRecievedEventData* >( evenData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUDPPacketRecievedEventData& data = udpPacketData->GetData();
        const CORE::CDynamicBuffer& udpPacketBuffer = data.dataBuffer.GetData();

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Transmit Socket received a packet from " + data.sourceAddress.AddressAndPortAsString() );

        char packetHeader[ 7 ]; 
        CORE::UInt32 packetSize = udpPacketBuffer.GetDataSize();
        memcpy( packetHeader, "UDP", 3 );
        memcpy( packetHeader+3, &packetSize, 4 );        
        m_tcpServerSocket.SendToAllClients( packetHeader, 7 );
        m_tcpServerSocket.SendToAllClients( udpPacketBuffer.GetConstBufferPtr(), packetSize );

    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: UDP Receive Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnTCPServerConnectionDataRecieved( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    const COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventData );
    const CORE::CDynamicBuffer& receivedData = eData->GetData();
    COMCORE::CTCPServerConnection* connection = static_cast< COMCORE::CTCPServerConnection* >( notifier );

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp(" + CORE::PointerToString( this ) + "): " + CORE::UInt32ToString( receivedData.GetDataSize() ) + " Bytes received from TCP client " + connection->GetRemoteHostName() );

    // Since TCP is streaming we may or may not have received a full packet
    // As such we concat bytes into a packet buffer and split according to the protocol this app uses
    CORE::CDynamicBuffer& packetBuffer = m_tcpServerReceivePacketBuffers[ connection->GetConnectionIndex() ];
    packetBuffer.Append( receivedData.GetConstBufferPtr(), receivedData.GetDataSize(), true );
    
    // Check to see if we received full UDP packets and if so transmit them
    CORE::UInt32 offset = 0;
    CORE::Int32 index = 0;
    while ( ( index = packetBuffer.Find( "UDP", 3, offset ) ) >= 0 )
    {
        if ( offset+7 > packetBuffer.GetDataSize() )
            break;

        CORE::UInt32 packetSize = packetBuffer.AsConstType< CORE::UInt32 >( offset+3 );                
        if ( offset+7+packetSize > packetBuffer.GetDataSize() )
            break;

        // We have received a complete UDP packet, transmit it
        bool sendError = false;
        THostAddressVector::iterator i = m_udpDestinations.begin();
        while ( i != m_udpDestinations.end() )
        {
            CORE::Int32 bytesTransmitted = m_udpTransmitSocket.SendPacketTo( (*i)                                       , 
                                                                             packetBuffer.GetConstBufferPtr( offset+7 ) , 
                                                                             packetSize                                 );
    
            // If we could not transmit all the bytes just try again later and keep the bytes in the packet buffer
            if ( bytesTransmitted != packetSize )
            {
                sendError = true;
                break;
            }
            ++i;
        }
        
        if ( sendError )
            break;

        offset = ( (CORE::UInt32) index ) + 7 + packetSize;
    }

    // Get rid of stuff we successfully transmitted in the packet buffer
    packetBuffer.Downshift( offset );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnTCPClientConnected( CORE::CNotifier* notifier    ,
                                 const CORE::CEvent& eventId  ,
                                 CORE::CICloneable* eventData )
{GUCEF_TRACE;
    
    if ( !m_tcpClientSendPacketBuffers.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: TCP client connected. Will attempt to send " + 
            CORE::UInt32ToString( (CORE::UInt32) m_tcpClientSendPacketBuffers.size() ) + " queued packages" );
        
        char packetHeader[ 7 ]; 
        memcpy( packetHeader, "UDP", 3 );

        while ( !m_tcpClientSendPacketBuffers.empty() )
        {
            CORE::CDynamicBuffer& packet = m_tcpClientSendPacketBuffers.front();
            CORE::UInt32 packetSize = packet.GetDataSize();
            memcpy( packetHeader+3, &packetSize, 4 );
            if ( m_tcpClientSocket.Send( packetHeader, 7 ) )
            {
                if ( m_tcpClientSocket.Send( packet.GetConstBufferPtr(), packetSize ) )
                {
                    m_tcpClientSendPacketBuffers.pop();
                }
                else
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }
    }
    else
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: TCP client connected, no packages were queued for transmission" );
    }
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnTCPServerClientConnected( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    const COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventData );
    const COMCORE::CTCPServerSocket::TConnectionInfo& info = eData->GetData();

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp: TCP Client connected" );

    // Subscribe to the connection
    TEventCallback callback( this, &UdpViaTcp::OnTCPServerConnectionDataRecieved );
    SubscribeTo( info.connection                                  ,
                 COMCORE::CTCPServerConnection::DataRecievedEvent ,
                 callback                                         );
}

/*-------------------------------------------------------------------------*/
    
void
UdpViaTcp::OnTCPServerClientDisconnected( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventId  ,
                                          CORE::CICloneable* eventData )
{GUCEF_TRACE;

    const COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventData );
    if ( GUCEF_NULL == eData ) return;
    const COMCORE::CTCPServerSocket::TConnectionInfo& info = eData->GetData();

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp: TCP Client disconnected" );

    // Wipe data stored for this connection
    m_tcpServerReceivePacketBuffers[ info.connection->GetConnectionIndex() ].Clear( true );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnTCPServerClientError( CORE::CNotifier* notifier    ,
                                   const CORE::CEvent& eventId  ,
                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp: TCP Client Error" );
}
    
/*-------------------------------------------------------------------------*/

void 
UdpViaTcp::OnTCPServerSocketOpened( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventId  ,
                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp: TCP Server socket has been opened" );
}
    
/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnTCPServerSocketClosed( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventId  ,
                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp: TCP Server socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnTCPServerSocketError( CORE::CNotifier* notifier    ,
                                   const CORE::CEvent& eventId  ,
                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp: TCP Server socket error occured" );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnTCPServerSocketClientError( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventId  ,
                                         CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "UdpViaTcp: TCP Server socket client error occured" );
}

/*-------------------------------------------------------------------------*/

void
UdpViaTcp::OnTCPServerSocketMaxConnectionsChanged( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventId  ,
                                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    const COMCORE::CTCPServerSocket::TServerSocketMaxConnectionsChangedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TServerSocketMaxConnectionsChangedEventData* >( eventData );
    CORE::Int32 maxConnections = eData->GetData();
    
    m_tcpServerReceivePacketBuffers.resize( (size_t) maxConnections );

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: Max TCP Client connections set to " + CORE::Int32ToString( maxConnections ) );
}

/*-------------------------------------------------------------------------*/

bool
UdpViaTcp::Start( void )
{GUCEF_TRACE;

    if ( UDPVIATCPMODE_BIDIRECTIONAL_UDP == m_mode || UDPVIATCPMODE_UDP_TRANSMITTER_ONLY == m_mode )
    {
        // We will be receiving wrapped UDP packets over the TCP tunnel and transmitting them as regular UDP again
        m_tcpServerSocket.SetMaxUpdatesPerCycle( 10 );
        m_tcpServerSocket.SetAutoReOpenOnError( true );
        m_tcpServerSocket.ListenOnPort( m_tcpReceiver.GetPortInHostByteOrder() );
        
        m_udpTransmitSocket.SetMaxUpdatesPerCycle( 10 );
        m_udpTransmitSocket.SetAutoReOpenOnError( true ); 
        m_udpTransmitSocket.Open( m_udpTransmitter );
    }
    if ( UDPVIATCPMODE_BIDIRECTIONAL_UDP == m_mode || UDPVIATCPMODE_UDP_RECEIVER_ONLY == m_mode )
    {
        // We will be receiving regular UDP packets and will be pushing them into the TCP tunnel
        m_tcpClientSocket.SetMaxRead( 102400 );
        m_tcpClientSocket.SetAutoReconnectOnError( true );
        m_tcpClientSocket.SetMaxUpdatesPerCycle( 10 );
        m_tcpClientSocket.ConnectTo( m_tcpDestination, false );

        m_udpReceiveSocket.SetMaxUpdatesPerCycle( 10 );
        m_udpReceiveSocket.SetAutoReOpenOnError( true ); 
        m_udpReceiveSocket.Open( m_udpReceiver );
    }

    if ( m_transmitMetrics )
    {
        m_metricsTimer.SetInterval( 1000 );
        m_metricsTimer.SetEnabled( true );
    }
   
    
    if ( m_httpServer.Listen() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: Opened REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        return true;
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: Failed to open REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
    return false;

}

/*-------------------------------------------------------------------------*/

bool 
UdpViaTcp::LoadConfig( const CORE::CValueList& appConfig   ,
                       const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    CORE::CString modeAsString = appConfig.GetValueAlways( "Mode", "Receiver" ).Lowercase();
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpViaTcp: Application mode: " + modeAsString );

    if ( "receiver" == modeAsString )
        m_mode = UDPVIATCPMODE_UDP_RECEIVER_ONLY;
    else
    if ( "transmitter" == modeAsString )
        m_mode = UDPVIATCPMODE_UDP_TRANSMITTER_ONLY;
    else
    if ( "bidirectional" == modeAsString )
        m_mode = UDPVIATCPMODE_BIDIRECTIONAL_UDP;
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "UdpViaTcp:LoadConfig: Unknown application mode specified: " + modeAsString );
        return false;
    }

    m_transmitMetrics = CORE::StringToBool( appConfig.GetValueAlways( "TransmitMetrics", "true" ) );
    
    m_udpReceiveUnicast = CORE::StringToBool( appConfig.GetValueAlways( "UdpReceiverAcceptsUnicast", "true" ) );
    m_udpReceiveMulticast = CORE::StringToBool( appConfig.GetValueAlways( "UdpReceiverAcceptsMulticast", "false" ) );
    m_udpReceiver.SetPortInHostByteOrder( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "UdpReceiverPort", "20000" ) ) ) );
    m_udpReceiver.SetHostname( CORE::ResolveVars( appConfig.GetValueAlways( "UdpReceiverInterface", "0.0.0.0" ) ) );
    m_udpReceiverMulticastSources = appConfig.GetValueVectorAlways( "UdpReceiverMulticastSource" );
    CORE::CValueList::TStringVector::iterator i = m_udpReceiverMulticastSources.begin();
    while ( i != m_udpReceiverMulticastSources.end() )
    {
        (*i) = CORE::ResolveVars( (*i) );
        ++i;
    }
    m_udpReceiveSocket.SetAllowMulticastLoopback( CORE::StringToBool( appConfig.GetValueAlways( "UdpReceiverAllowMulticastLoopback", "false" ) ) );
    m_udpReceiveSocket.SetMulticastTTL( CORE::StringToInt32( appConfig.GetValueAlways( "UdpReceiverMulticastTTL", "8" ) ) );
    
    m_tcpDestination.SetPortInHostByteOrder( CORE::StringToUInt16( CORE::ResolveVars( ( appConfig.GetValueAlways( "TcpTunnelDestinationPort", "30000" ) ) ) ) );
    m_tcpDestination.SetHostname( CORE::ResolveVars( appConfig.GetValueAlways( "TcpTunnelDestinationAddr", "127.0.0.1" ) ) );
    m_tcpReceiver.SetPortInHostByteOrder( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "TcpTunnelReceivePort", "30000" ) ) ) );
    m_tcpReceiver.SetHostname( CORE::ResolveVars( appConfig.GetValueAlways( "TcpTunnelReceiveInterface", "0.0.0.0" ) ) );
        
    m_udpTransmitter.SetPortInHostByteOrder( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "UdpTransmitterPort", "20001" ) ) ) );
    m_udpTransmitter.SetHostname( CORE::ResolveVars( appConfig.GetValueAlways( "UdpTransmitterInterface", "0.0.0.0" ) ) ); 
    
    // UDP Destinations are allowed to be configured via multiple config lines or via a , separated list or some combination thereof
    // Do note that in order to garantee correct association between IP and Port we have to specify IP and Port together as <IP>:<Port>
    CORE::CValueList::TStringVector udpDestinationsCombo = appConfig.GetValueVectorAlways( "UdpDestinations" );
    i = udpDestinationsCombo.begin();
    while ( i != udpDestinationsCombo.end() )
    {
        CORE::CValueList::TStringVector elements = (*i).ParseElements( ',', false );
        CORE::CValueList::TStringVector::iterator n = elements.begin();
        while ( n != elements.end() )
        {
            CORE::CString destinationStr = CORE::ResolveVars( (*n) ); 
            COMCORE::CHostAddress destination( destinationStr );
            m_udpDestinations.push_back( destination );
            ++n;
        }
        ++i;
    }

    // Also still allow the simpler port and ip definition
    // Initially we did not support multiple UDP destinations for the transmitter
    CORE::CString udpDstPort = appConfig.GetValueAlways( "UdpDestinationPort" );
    CORE::CString udpDstAddr = appConfig.GetValueAlways( "UdpDestinationAddr" );
    if ( !udpDstPort.IsNULLOrEmpty() && !udpDstAddr.IsNULLOrEmpty() )
    {
        COMCORE::CHostAddress destination;
        destination.SetPortInHostByteOrder( CORE::StringToUInt16( CORE::ResolveVars( udpDstPort ) ) );
        destination.SetHostname( CORE::ResolveVars( udpDstAddr ) );
        m_udpDestinations.push_back( destination );
    }
    if ( m_udpDestinations.empty() )
    {
        COMCORE::CHostAddress destination( "127.0.0.1", 40000 );
        m_udpDestinations.push_back( destination );
    }

    m_udpReceiverMulticastSources = appConfig.GetValueVectorAlways( "UdpReceiverMulticastSource" );
    CORE::CValueList::TStringVector::iterator n = m_udpReceiverMulticastSources.begin();
    while ( n != m_udpReceiverMulticastSources.end() )
    {
        (*n) = CORE::ResolveVars( (*n) );
        ++n;
    }
        
    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort", "10000" ) ) ) );

    m_httpRouter.SetResourceMapping( "/info", RestApiUdpViaTcpInfoResource::THTTPServerResourcePtr( new RestApiUdpViaTcpInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiUdpViaTcpInfoResource::THTTPServerResourcePtr( new RestApiUdpViaTcpConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiUdpViaTcpInfoResource::THTTPServerResourcePtr( new RestApiUdpViaTcpConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping(  appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ), RestApiUdpViaTcpInfoResource::THTTPServerResourcePtr( new COM::CDummyHTTPServerResource() )  );
    
    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList& 
UdpViaTcp::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode& 
UdpViaTcp::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/