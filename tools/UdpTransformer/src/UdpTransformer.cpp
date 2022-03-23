/*
 *  UdpTransformer: service to perform basic transformations to in-flight UDP
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

#ifndef GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefWEB_CDummyHTTPServerResource.h"
#define GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H ? */

#include "UdpTransformer.h"

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

RestApiUdpTransformerInfoResource::RestApiUdpTransformerInfoResource( UdpTransformer* app )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiUdpTransformerInfoResource::~RestApiUdpTransformerInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiUdpTransformerInfoResource::Serialize( const CORE::CString& resourcePath   ,
                                              CORE::CDataNode& output             ,
                                              const CORE::CString& representation )
{GUCEF_TRACE;

    output.SetName( "info" );
    output.SetAttribute( "application", "UdpTransformer" );
    output.SetAttribute( "buildDateTime", CORE::CDateTime::CompileDateTime().ToIso8601DateTimeString( true, true ) );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", CORE::CVariant( false ) );
    #else
    output.SetAttribute( "isReleaseBuild", CORE::CVariant( true ) );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiUdpTransformerConfigResource::RestApiUdpTransformerConfigResource( UdpTransformer* app, bool appConfig )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiUdpTransformerConfigResource::~RestApiUdpTransformerConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiUdpTransformerConfigResource::Serialize( const CORE::CString& resourcePath   ,
                                                CORE::CDataNode& output             ,
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

UdpTransformer::UdpTransformer( void )
    : CORE::CObservingNotifier()
    , m_udpTransmitSocket( false )
    , m_udpReceiveSocket( false )
    , m_udpReceiveUnicast( true )
    , m_udpReceiveMulticast( false )
    , m_udpDestinations()
    , m_udpReceiverAddress()
    , m_udpTransmitterAddress()
    , m_udpReceiverMulticastSources()
    , m_udpPayloadTransformDescription()
    , m_transformDescriptionSegments()
    , m_udpSourceAddrTransformAsString( true )
    , m_udpSourceAddrTransformAsStringIncludePort( false )
    , m_udpSourceAddrTransformAsStringDotReplacementChar( '_' )
    , m_udpSourceAddrTransformAsStringColonReplacementChar( '_' )
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

UdpTransformer::~UdpTransformer()
{GUCEF_TRACE;

    m_httpServer.Close();
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Register UDP socket event handlers
    TEventCallback callback( this, &UdpTransformer::OnUDPReceiveSocketError );
    SubscribeTo( &m_udpReceiveSocket                      ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback                                 );
    TEventCallback callback2( this, &UdpTransformer::OnUDPReceiveSocketClosed );
    SubscribeTo( &m_udpReceiveSocket                       ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback2                                 );
    TEventCallback callback3( this, &UdpTransformer::OnUDPReceiveSocketOpened );
    SubscribeTo( &m_udpReceiveSocket                       ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback3                                 );
    TEventCallback callback4( this, &UdpTransformer::OnUDPReceiveSocketPacketsRecieved );
    SubscribeTo( &m_udpReceiveSocket                         ,
                 COMCORE::CUDPSocket::UDPPacketsRecievedEvent ,
                 callback4                                   );
    TEventCallback callback5( this, &UdpTransformer::OnUDPTransmitSocketError );
    SubscribeTo( &m_udpTransmitSocket                     ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback5                                );
    TEventCallback callback6( this, &UdpTransformer::OnUDPTransmitSocketClosed );
    SubscribeTo( &m_udpTransmitSocket                      ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback6                                 );
    TEventCallback callback7( this, &UdpTransformer::OnUDPTransmitSocketOpened );
    SubscribeTo( &m_udpTransmitSocket                      ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback7                                 );
    TEventCallback callback8( this, &UdpTransformer::OnUDPTransmitSocketPacketsRecieved );
    SubscribeTo( &m_udpTransmitSocket                        ,
                 COMCORE::CUDPSocket::UDPPacketsRecievedEvent ,
                 callback8                                   );

    TEventCallback callback19( this, &UdpTransformer::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback19                     );

}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventId  ,
                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_transmitMetrics )
    {
        GUCEF_METRIC_COUNT( "UdpTransformer.UdpTransmitter.BytesSent", m_udpTransmitSocket.GetBytesTransmitted( true ), 1.0f );
        GUCEF_METRIC_COUNT( "UdpTransformer.UdpTransmitter.BytesReceived", m_udpTransmitSocket.GetBytesReceived( true ), 1.0f );
        GUCEF_METRIC_COUNT( "UdpTransformer.UdpTransmitter.MsgsReceived", m_udpTransmitSocket.GetNrOfDataReceivedEvents( true ), 1.0f );
        GUCEF_METRIC_COUNT( "UdpTransformer.UdpReceiver.BytesSent", m_udpReceiveSocket.GetBytesTransmitted( true ), 1.0f );
        GUCEF_METRIC_COUNT( "UdpTransformer.UdpReceiver.BytesReceived", m_udpReceiveSocket.GetBytesReceived( true ), 1.0f );
        GUCEF_METRIC_COUNT( "UdpTransformer.UdpReceiver.MsgsReceived", m_udpReceiveSocket.GetNrOfDataReceivedEvents( true ), 1.0f );
    }
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnUDPReceiveSocketError( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventID  ,
                                         CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::TSocketErrorEventData* eData = static_cast< COMCORE::CUDPSocket::TSocketErrorEventData* >( eventData );    
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Receive Socket experienced error " + CORE::Int32ToString( eData->GetData() ) );
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnUDPReceiveSocketClosed( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventID  ,
                                          CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Receive Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnUDPReceiveSocketOpened( CORE::CNotifier* notifier   ,
                                          const CORE::CEvent& eventID ,
                                          CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Receive Socket has been opened" );

    if ( m_udpReceiveMulticast )
    {
        CORE::CValueList::TVariantVector::iterator i = m_udpReceiverMulticastSources.begin();
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

bool
UdpTransformer::TransformUdpPacket( const CORE::CDynamicBuffer& udpPacketBuffer      ,
                                    const COMCORE::CIPAddress& sourceAddress         ,
                                    CORE::CDynamicBuffer& transformedUdpOutputBuffer )
{GUCEF_TRACE;

    if ( !m_transformDescriptionSegments.empty() )
    {
        CORE::UInt32 dataSize = 0;
        CORE::CValueList::TStringVector::iterator i = m_transformDescriptionSegments.begin();
        while ( i != m_transformDescriptionSegments.end() )
        {            
            // Check for known variables/keywords we support
            if ( (*i) == "$UDPSOURCEADDR$" )
            {
                if ( m_udpSourceAddrTransformAsString )
                {
                    CORE::CString srcAddr;
                    if ( m_udpSourceAddrTransformAsStringIncludePort )
                    {
                        srcAddr = sourceAddress.AddressAndPortAsString();
                        srcAddr = srcAddr.ReplaceChar( '.', m_udpSourceAddrTransformAsStringDotReplacementChar );
                        srcAddr = srcAddr.ReplaceChar( ':', m_udpSourceAddrTransformAsStringColonReplacementChar ); 
                    }
                    else
                    {
                        srcAddr = sourceAddress.AddressAsString();
                        srcAddr = srcAddr.ReplaceChar( '.', m_udpSourceAddrTransformAsStringDotReplacementChar );
                    }

                    transformedUdpOutputBuffer.CopyFrom( dataSize, srcAddr.Length(), srcAddr.C_String() );
                    dataSize += srcAddr.Length();
                }
                else
                {
                    CORE::UInt32 addrInNetworkByteOrder = sourceAddress.GetAddress();
                    transformedUdpOutputBuffer.CopyFrom( dataSize, sizeof( addrInNetworkByteOrder ), &addrInNetworkByteOrder );
                    dataSize += sizeof( addrInNetworkByteOrder );
                    CORE::UInt16 portInNetworkByteOrder = sourceAddress.GetPort();
                    transformedUdpOutputBuffer.CopyFrom( dataSize, sizeof( portInNetworkByteOrder ), &portInNetworkByteOrder );
                    dataSize += sizeof( portInNetworkByteOrder );
                }                
            }
            else
            if ( (*i) == "$UDPPAYLOAD$" )
            {
                CORE::UInt32 bytesWritten = transformedUdpOutputBuffer.CopyFrom( dataSize, udpPacketBuffer.GetDataSize(), udpPacketBuffer.GetConstBufferPtr() );
                if ( bytesWritten != udpPacketBuffer.GetDataSize() )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "UdpTransformer:TransformUdpPacket: Failed to write source UDP payload into transform buffer for a packet from " + 
                        sourceAddress.AddressAndPortAsString() + ". Expected bytes written=" + CORE::ToString( udpPacketBuffer.GetDataSize() ) +
                        ". Actual bytes written=" + CORE::ToString( bytesWritten ) );
                    return false;
                }
                dataSize += bytesWritten;
            }
            else
            {
                // Everything else we just put in place as-is after resolving potential standard variables, including environment variables
                CORE::CString segmentText = CORE::ResolveVars( (*i) );
                transformedUdpOutputBuffer.CopyFrom( dataSize, segmentText.Length(), segmentText.C_String() );
                dataSize += segmentText.Length();
            }
            ++i;
        }
    }
    else
    {
        // No transform needed. A simple weak copy will do.
        //      Are you just using this service as a router/proxy ?
        transformedUdpOutputBuffer.LinkTo( udpPacketBuffer );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnUDPReceiveSocketPacketsRecieved( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventID  ,
                                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketsRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketsRecievedEventData* >( eventData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUdpPacketsRecievedEventData& data = udpPacketData->GetData();
        for ( CORE::UInt32 i=0; i<data.packetsReceived; ++i )
        {
            const CORE::CDynamicBuffer& udpPacketBuffer = data.packets[ i ].dataBuffer.GetData();

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "UdpTransformer: UDP Receive Socket received a packet from " + data.packets[ i ].sourceAddress.AddressAndPortAsString() );

            if ( TransformUdpPacket( udpPacketBuffer                 , 
                                     data.packets[ i ].sourceAddress ,
                                     m_packetTransformBuffer         ) )
            {            
                THostAddressVector::iterator n = m_udpDestinations.begin();
                while ( n != m_udpDestinations.end() )
                {
                    CORE::Int32 bytesSent = m_udpTransmitSocket.SendPacketTo( (*n)                                                 , 
                                                                              m_packetTransformBuffer.GetBufferPtr()               , 
                                                                              (CORE::UInt16) m_packetTransformBuffer.GetDataSize() );
                
                    if ( bytesSent != m_packetTransformBuffer.GetDataSize() )
                    {
                        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "UdpTransformer: UDP Transmit Socket Failed to forward correct nr of bytes for a packet from " + 
                            data.packets[ i ].sourceAddress.AddressAndPortAsString() + ". Expected bytes=" + CORE::ToString( m_packetTransformBuffer.GetDataSize() ) +
                            ". Actual bytes=" + CORE::ToString( bytesSent ) );
                    }
                    ++n;
                }
            }
            else
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "UdpTransformer: Not retransmitting UDP packet because the transform failed for a packet from " + 
                    data.packets[ i ].sourceAddress.AddressAndPortAsString() + ". Packet byte size=" + CORE::ToString( m_packetTransformBuffer.GetDataSize() ) );
            }
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Receive Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnUDPTransmitSocketError( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventID  ,
                                          CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::TSocketErrorEventData* eData = static_cast< COMCORE::CUDPSocket::TSocketErrorEventData* >( eventData );    
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Transmit Socket experienced error " + CORE::Int32ToString( eData->GetData() ) );
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnUDPTransmitSocketClosed( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventID  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;                     

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Transmit Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnUDPTransmitSocketOpened( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventID  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Transmit Socket has been opened" );
}

/*-------------------------------------------------------------------------*/

void
UdpTransformer::OnUDPTransmitSocketPacketsRecieved( CORE::CNotifier* notifier   ,
                                                    const CORE::CEvent& eventID ,
                                                    CORE::CICloneable* evenData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketsRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketsRecievedEventData* >( evenData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUdpPacketsRecievedEventData& data = udpPacketData->GetData();
        for ( CORE::UInt32 i=0; i<data.packetsReceived; ++i )
        {
            const CORE::CDynamicBuffer& udpPacketBuffer = data.packets[ i ].dataBuffer.GetData();

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Transmit Socket received a packet from " + data.packets[ i ].sourceAddress.AddressAndPortAsString() );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: UDP Receive Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

bool
UdpTransformer::Start( void )
{GUCEF_TRACE;

    m_packetTransformBuffer.SetAutoEnlarge( true );
    if ( !m_packetTransformBuffer.SetBufferSize( 10000, false ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "UdpTransformer:Start: Failed to allocate buffer for packet transforms" );
        return false;
    }
    
    m_udpTransmitSocket.SetMaxUpdatesPerCycle( 10 );
    m_udpTransmitSocket.SetAutoReOpenOnError( true ); 
    m_udpTransmitSocket.Open( m_udpTransmitterAddress );

    m_udpReceiveSocket.SetMaxUpdatesPerCycle( 10 );
    m_udpReceiveSocket.SetAutoReOpenOnError( true ); 
    m_udpReceiveSocket.Open( m_udpReceiverAddress );

    if ( m_transmitMetrics )
    {
        m_metricsTimer.SetInterval( 1000 );
        m_metricsTimer.SetEnabled( true );
    }  

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer:Start: Completed startup" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
UdpTransformer::LoadConfig( const CORE::CValueList& appConfig   ,
                            const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    m_transmitMetrics = CORE::StringToBool( appConfig.GetValueAlways( "transmitMetrics" ), true );
    
    m_udpPayloadTransformDescription = appConfig.GetValueAlways( "udpPayloadTransformDescription", m_udpPayloadTransformDescription );
    m_transformDescriptionSegments = m_udpPayloadTransformDescription.ParseElements( '|', false );
    m_udpSourceAddrTransformAsString = CORE::StringToBool( appConfig.GetValueAlways( "udpSourceAddrTransformAsString" ), true );
    m_udpSourceAddrTransformAsStringIncludePort = CORE::StringToBool( appConfig.GetValueAlways( "udpSourceAddrTransformAsStringIncludePort" ), false );
    m_udpSourceAddrTransformAsStringDotReplacementChar = appConfig.GetValueAlways( "udpSourceAddrTransformAsStringDotReplacementChar", "_" ).AsString().CodepointAtIndex( 0 );
    m_udpSourceAddrTransformAsStringColonReplacementChar = appConfig.GetValueAlways( "udpSourceAddrTransformAsStringColonReplacementChar", "_" ).AsString().CodepointAtIndex( 0 );

    m_udpReceiveUnicast = CORE::StringToBool( appConfig.GetValueAlways( "udpReceiverAcceptsUnicast" ), true );
    m_udpReceiveMulticast = CORE::StringToBool( appConfig.GetValueAlways( "udpReceiverAcceptsMulticast" ), false );
    m_udpReceiverAddress.SetPortInHostByteOrder( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "udpReceiverPort" ) ), 20000 ) );
    m_udpReceiverAddress.SetHostname( CORE::ResolveVars( appConfig.GetValueAlways( "udpReceiverInterface", "0.0.0.0" ) ) );
    m_udpReceiverMulticastSources = appConfig.GetValueVectorAlways( "udpReceiverMulticastSource" );
    CORE::CValueList::TVariantVector::iterator i = m_udpReceiverMulticastSources.begin();
    while ( i != m_udpReceiverMulticastSources.end() )
    {
        (*i) = CORE::ResolveVars( (*i) );
        ++i;
    }
    m_udpReceiveSocket.SetAllowMulticastLoopback( CORE::StringToBool( appConfig.GetValueAlways( "udpReceiverAllowMulticastLoopback" ), false ) );
    m_udpReceiveSocket.SetMulticastTTL( CORE::StringToInt32( appConfig.GetValueAlways( "udpReceiverMulticastTTL" ), 8 ) );
        
    m_udpTransmitterAddress.SetPortInHostByteOrder( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "udpTransmitterPort" ) ), 20001 ) );
    m_udpTransmitterAddress.SetHostname( CORE::ResolveVars( appConfig.GetValueAlways( "udpTransmitterInterface", "0.0.0.0" ) ) ); 
    
    // UDP Destinations are allowed to be configured via multiple config lines or via a , separated list or some combination thereof
    // Do note that in order to garantee correct association between IP and Port we have to specify IP and Port together as <IP>:<Port>
    CORE::CValueList::TVariantVector udpDestinationsCombo = appConfig.GetValueVectorAlways( "udpDestinations" );
    i = udpDestinationsCombo.begin();
    while ( i != udpDestinationsCombo.end() )
    {
        CORE::CString::StringVector elements = (*i).AsString().ParseElements( ',', false );
        CORE::CString::StringVector::iterator n = elements.begin();
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
    CORE::CString udpDstPort = appConfig.GetValueAlways( "udpDestinationPort" );
    CORE::CString udpDstAddr = appConfig.GetValueAlways( "udpDestinationAddr" );
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

    m_udpReceiverMulticastSources = appConfig.GetValueVectorAlways( "udpReceiverMulticastSource" );
    CORE::CValueList::TVariantVector::iterator n = m_udpReceiverMulticastSources.begin();
    while ( n != m_udpReceiverMulticastSources.end() )
    {
        (*n) = CORE::ResolveVars( (*n) );
        ++n;
    }
        
    bool enableRestApi = CORE::StringToBool( appConfig.GetValueAlways( "enableRestApi" ), true );
    if ( enableRestApi )
    {
        m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "restApiPort" ) ), 10000 ) );

        m_httpRouter.SetResourceMapping( "/info", RestApiUdpTransformerInfoResource::THTTPServerResourcePtr( new RestApiUdpTransformerInfoResource( this ) )  );
        m_httpRouter.SetResourceMapping( "/config/appargs", RestApiUdpTransformerInfoResource::THTTPServerResourcePtr( new RestApiUdpTransformerConfigResource( this, true ) )  );
        m_httpRouter.SetResourceMapping( "/config", RestApiUdpTransformerInfoResource::THTTPServerResourcePtr( new RestApiUdpTransformerConfigResource( this, false ) )  );
        m_httpRouter.SetResourceMapping(  appConfig.GetValueAlways( "restBasicHealthUri", "/health/basic" ), RestApiUdpTransformerInfoResource::THTTPServerResourcePtr( new WEB::CDummyHTTPServerResource() )  );
    
        m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

        if ( m_httpServer.Listen() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: Opened REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer: Failed to open REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        }
    }

    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );
    
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "UdpTransformer:LoadConfig: Completed config loading" );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList& 
UdpTransformer::GetAppConfig( void ) const
{GUCEF_TRACE;

    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode& 
UdpTransformer::GetGlobalConfig( void ) const
{GUCEF_TRACE;

    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/