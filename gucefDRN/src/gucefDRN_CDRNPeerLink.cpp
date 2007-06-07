/*
 *  gucefDRN: GUCEF module providing RAD networking trough data replication
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CSTREAMEREVENTS_H
#include "CStreamerEvents.h"
#define GUCEF_CORE_CSTREAMEREVENTS_H
#endif /* GUCEF_CORE_CSTREAMEREVENTS_H ? */

#ifndef GUCEF_COMCORE_CTCPCONNECTION_H
#include "CTCPConnection.h"
#define GUCEF_COMCORE_CTCPCONNECTION_H
#endif /* GUCEF_COMCORE_CTCPCONNECTION_H ? */

#ifndef GUCEF_COMCORE_CUDPCHANNEL_H
#include "CUDPChannel.h"
#define GUCEF_COMCORE_CUDPCHANNEL_H
#endif /* GUCEF_COMCORE_CUDPCHANNEL_H ? */

#ifndef GUCEF_DRN_DRNPROTOCOL_H
#include "gucefDRN_DRNProtocol.h"
#define GUCEF_DRN_DRNPROTOCOL_H
#endif /* GUCEF_DRN_DRNPROTOCOL_H ? */

#ifndef GUCEF_DRN_CDRNPEERLINKDATA_H
#include "gucefDRN_CDRNPeerLinkData.h"
#define GUCEF_DRN_CDRNPEERLINKDATA_H
#endif /* GUCEF_DRN_CDRNPEERLINKDATA_H ? */

#ifndef GUCEF_COMCORE_CUDPMASTERSOCKET_H
#include "CUDPMasterSocket.h"
#define GUCEF_COMCORE_CUDPMASTERSOCKET_H
#endif /* GUCEF_COMCORE_CUDPMASTERSOCKET_H ? */

#ifndef GUCEF_DRN_CDRNNODE_H
#include "gucefDRN_CDRNNode.h"
#define GUCEF_DRN_CDRNNODE_H
#endif /* GUCEF_DRN_CDRNNODE_H ? */

#ifndef GUCEF_DRN_CIDRNPEERVALIDATOR_H
#include "gucefDRN_CIDRNPeerValidator.h"
#define GUCEF_DRN_CIDRNPEERVALIDATOR_H
#endif /* GUCEF_DRN_CIDRNPEERVALIDATOR_H ? */

#include "gucefDRN_CDRNPeerLink.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace DRN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CDRNPeerLink::ConnectedEvent = "GUCEF::DRN::CDRNPeerLink::ConnectedEvent";
const CORE::CEvent CDRNPeerLink::DisconnectedEvent = "GUCEF::DRN::CDRNPeerLink::DisconnectedEvent";
const CORE::CEvent CDRNPeerLink::SocketErrorEvent = "GUCEF::DRN::CDRNPeerLink::SocketErrorEvent";
const CORE::CEvent CDRNPeerLink::LinkCorruptionEvent = "GUCEF::DRN::CDRNPeerLink::LinkCorruptionEvent"; 
const CORE::CEvent CDRNPeerLink::LinkProtocolMismatchEvent = "GUCEF::DRN::CDRNPeerLink::LinkProtocolMismatchEvent"; 
const CORE::CEvent CDRNPeerLink::LinkIncompatibleEvent = "GUCEF::DRN::CDRNPeerLink::LinkIncompatibleEvent"; 
const CORE::CEvent CDRNPeerLink::PeerListReceivedFromPeerEvent = "GUCEF::DRN::CDRNPeerLink::PeerListReceivedFromPeerEvent";
const CORE::CEvent CDRNPeerLink::StreamListReceivedFromPeerEvent = "GUCEF::DRN::CDRNPeerLink::StreamListReceivedFromPeerEvent";
const CORE::CEvent CDRNPeerLink::DataGroupListReceivedFromPeerEvent = "GUCEF::DRN::CDRNPeerLink::DataGroupListReceivedFromPeerEvent";
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CDRNPeerLink::RegisterEvents( void )
{GUCEF_TRACE;
    
    ConnectedEvent.Initialize();
    DisconnectedEvent.Initialize();
    SocketErrorEvent.Initialize();
    LinkCorruptionEvent.Initialize();
    LinkProtocolMismatchEvent.Initialize();
    LinkIncompatibleEvent.Initialize();
    PeerListReceivedFromPeerEvent.Initialize();
    StreamListReceivedFromPeerEvent.Initialize();
    DataGroupListReceivedFromPeerEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CDRNPeerLink::CDRNPeerLink( CDRNNode& parentNode                   ,
                            COMCORE::CTCPConnection& tcpConnection ,
                            COMCORE::CUDPMasterSocket& udpSocket   )
    : m_udpSocket( &udpSocket )         ,
      m_tcpConnection( &tcpConnection ) ,
      m_udpPossible( false )            ,
      m_isAuthenticated( false )        ,
      m_linkData( NULL )                ,
      m_parentNode( &parentNode )       ,
      m_tcpStreamBuffer()               ,
      m_tcpStreamKeepBytes( 0 )         ,
      m_linkOperational( false )
{GUCEF_TRACE;

    assert( m_udpSocket != NULL );
    assert( m_parentNode != NULL );
    assert( m_tcpConnection != NULL );    
    RegisterEvents();
    
    // Verify that we are connected
    if ( tcpConnection.IsActive() )
    {
        m_linkData = new CDRNPeerLinkData( *this );        
        SubscribeTo( &tcpConnection );        
        NotifyObservers( ConnectedEvent );        
        SendGreetingMessage();
    }
}

/*-------------------------------------------------------------------------*/

CDRNPeerLink::~CDRNPeerLink()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::SendGreetingMessage( void )
{GUCEF_TRACE;

    // Compose the greeting message
    char sendBuffer[ 8 ] = { DRN_TRANSMISSION_START, 0, 0, DRN_PEERCOMM_GREETING, DRN_PROTOCOL_MAYOR_VERION, DRN_PROTOCOL_MINOR_VERION, DRN_PROTOCOL_PATCH_VERION, DRN_TRANSMISSION_END };
    UInt16 payloadSize = 4;
    memcpy( sendBuffer+1, &payloadSize, 2 );
    
    if ( !SendData( sendBuffer, 8, false ) )
    {
        // Failed to send data, something is very wrong
        CloseLink();
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::CloseLink( void )
{GUCEF_TRACE;

    m_tcpConnection->Close();
    UnsubscribeFrom( m_tcpConnection );
    m_udpPossible = false;
    m_isAuthenticated = false;
    m_tcpConnection = NULL;
    m_udpSocket = NULL;
}
/*-------------------------------------------------------------------------*/

CDRNPeerLink::CIPAddress
CDRNPeerLink::GetPeerIP( void ) const
{GUCEF_TRACE;

    if ( m_tcpConnection != NULL )
    {
        return m_tcpConnection->GetRemoteIP();
    }
    return CIPAddress();
}

/*-------------------------------------------------------------------------*/

CORE::CString
CDRNPeerLink::GetPeerHostName( void ) const
{GUCEF_TRACE;

    if ( NULL != m_tcpConnection )
    {
        return m_tcpConnection->GetRemoteHostName();
    }
    return CORE::CString();
}

/*-------------------------------------------------------------------------*/
    
UInt16
CDRNPeerLink::GetPeerTCPPort( void ) const
{GUCEF_TRACE;    
    
    if ( NULL != m_tcpConnection )
    {
        return m_tcpConnection->GetRemoteTCPPort();
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt16
CDRNPeerLink::GetPeerUDPPort( void ) const
{GUCEF_TRACE;

    if ( NULL != m_tcpConnection )
    {
        return m_tcpConnection->GetRemoteTCPPort();
    }
    return 0;
}

/*-------------------------------------------------------------------------*/
    
bool
CDRNPeerLink::IsUDPPossible( void ) const
{GUCEF_TRACE;

    return m_udpPossible;
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLink::IsAuthenticated( void ) const
{GUCEF_TRACE;

    return m_isAuthenticated;
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::SetAuthenticatedFlag( const bool authenticated )
{GUCEF_TRACE;

    m_isAuthenticated = authenticated;
}

/*-------------------------------------------------------------------------*/

COMCORE::CTCPConnection*
CDRNPeerLink::GetTCPConnection( void )
{GUCEF_TRACE;

    return m_tcpConnection;
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLink::IsOperational( void ) const
{GUCEF_TRACE;

    return m_linkOperational;
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLink::SendData( const void* dataSource                   ,
                        const UInt16 dataSize                    ,
                        const bool allowUnreliable /* = false */ )
{GUCEF_TRACE;

    if ( allowUnreliable           && 
         m_udpPossible             && 
        ( m_udpSocket != NULL )    && 
        ( m_tcpConnection!= NULL  ) )
    {
        // Send the data using UDP
        CIPAddress ip( m_tcpConnection->GetRemoteIP() );
        return m_udpSocket->SendPacketTo( ip         ,
                                          dataSource ,
                                          dataSize   ) >= 0;
                                   
    }
    else
    if ( m_tcpConnection != NULL )
    {        
        // Send the data using TCP
        return m_tcpConnection->Send( dataSource ,
                                      dataSize   );
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::SendServiceTypeMessage( void )
{GUCEF_TRACE;

    // Obtain the service name from our parent node
    const CORE::CString& serviceName = m_parentNode->GetServiceName();
    
    // Compose the service type message
    CORE::CDynamicBuffer sendBuffer( serviceName.Length() + 5 , true );
    sendBuffer[ 0 ] = DRN_TRANSMISSION_START;
    sendBuffer[ 3 ] = DRN_PEERCOMM_SERVICE;
    sendBuffer[ serviceName.Length() + 3 ] = DRN_TRANSMISSION_END;
    
    UInt16 payloadSize = (UInt16)serviceName.Length()+1;
    sendBuffer.CopyFrom( 2, 2, &payloadSize );
    sendBuffer.CopyFrom( 4, serviceName.Length(), serviceName.C_String() );    
    
    // Send the service name message
    if ( !SendData( sendBuffer.GetConstBufferPtr()   , 
                    (UInt16)sendBuffer.GetDataSize() , 
                    false                            ) )
    {
        // Failed to send data, something is very wrong
        CloseLink();
    }    
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerLinkIncompatible( void )
{GUCEF_TRACE;

    NotifyObservers( LinkIncompatibleEvent );
    CloseLink();
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::SendIncompatibleLinkMessage( void )
{GUCEF_TRACE;
    
    // Compose the greeting message
    char sendBuffer[ 5 ] = { DRN_TRANSMISSION_START, 0, 0, DRN_PEERCOMM_INCOMPATIBLE_LINK, DRN_TRANSMISSION_END };
    UInt16 payloadSize = 1;
    memcpy( sendBuffer+1, &payloadSize, 2 );
    
    if ( !SendData( sendBuffer, 5, false ) )
    {
        // Failed to send data, something is very wrong
        CloseLink();
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::SendLinkOperationalMessage( void )
{GUCEF_TRACE;

    // Compose the link operational message
    char sendBuffer[ 5 ] = { DRN_TRANSMISSION_START, 0, 0, DRN_PEERCOMM_LINK_OPERATIONAL, DRN_TRANSMISSION_END };
    UInt16 payloadSize = 1;
    memcpy( sendBuffer+1, &payloadSize, 2 );
    
    if ( !SendData( sendBuffer, 5, false ) )
    {
        // Failed to send data, something is very wrong
        CloseLink();
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::SendAuthenticationRequiredMessage( void )
{GUCEF_TRACE;

    // Compose the link operational message
    char sendBuffer[ 5 ] = { DRN_TRANSMISSION_START, 0, 0, DRN_PEERCOMM_AUTHENTICATION_REQUIRED, DRN_TRANSMISSION_END };
    UInt16 payloadSize = 1;
    memcpy( sendBuffer+1, &payloadSize, 2 );
    
    if ( !SendData( sendBuffer, 5, false ) )
    {
        // Failed to send data, something is very wrong
        CloseLink();
    }    
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerServicesCompatible( void )
{GUCEF_TRACE;

    // Check if a validator mechanism has been provided to the node
    CIDRNPeerValidator* validator = m_parentNode->GetPeervalidator();
    if ( validator != NULL )
    {
        // A validator is available for use
        if ( validator->IsPeerLoginRequired() )
        {
            // Peer authentication is mandatory
            SendAuthenticationRequiredMessage();
            return;
        }
    }
    
    // Peer authentication is optional, we can proceed without it
    SendLinkOperationalMessage();
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerServiceType( const char* data      ,
                                 const UInt32 dataSize )
{GUCEF_TRACE;

    // Check if the payload contains a service name
    if ( dataSize > 1 )
    {
        // Build the remote service string
        UInt32 serviceStringSize = dataSize - 1;
        CORE::CString remoteService( data+1, serviceStringSize );
        
        // Compare the service names
        if ( remoteService == m_parentNode->GetServiceName() )
        {
            // The service names match, the link is compatible
            OnPeerServicesCompatible();
            return;
        }
        else
        {
            // Incompatible: the local service does not match the remote service name.
            // The names have to match for the link to be considered compatible
            NotifyObservers( LinkIncompatibleEvent );
            SendIncompatibleLinkMessage();
            CloseLink();
            return;
        }
    }
    else
    if ( m_parentNode->GetServiceName().Length() > 0 )
    {
        // Incompatible: the local service is not unnamed
        // Both have to be unnamed to be considered as compatible
        NotifyObservers( LinkIncompatibleEvent );
        SendIncompatibleLinkMessage();
        CloseLink();
        return;        
    }
    
    //Both services are without a name: always considered compatible
    OnPeerServicesCompatible();
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerLinkOperational( void )
{GUCEF_TRACE;
    
    m_linkOperational = true;
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerDataGroupRequest( void )
{GUCEF_TRACE;

    if ( m_linkOperational )
    {
        // Obtain a list of streams publicized on this link
        CDRNPeerLinkData::TDRNDataGroupList dataGroupList;
        m_linkData->GetPublicizedDataGroups( dataGroupList );
        
        // Compose the list message 
        CORE::CDynamicBuffer streamBuffer( 4, true );
        streamBuffer[ 0 ] = DRN_TRANSMISSION_START;
        streamBuffer[ 3 ] = DRN_PEERCOMM_DATAGROUPLIST;
        
        // Copy the list into the buffer
        CDRNPeerLinkData::TDRNDataGroupList::iterator i = dataGroupList.begin();
        while ( i != dataGroupList.end() )
        {
            // Copy the string into the buffer including the null terminator
            streamBuffer.Append( (*i)->GetName().C_String(), (*i)->GetName().Length()+1 );
            ++i;
        }
        
        // Fill in the payload size segment
        UInt16 payloadSize = (UInt16) streamBuffer.GetDataSize() - 4;
        streamBuffer.CopyFrom( 1, 2, &payloadSize );
        
        // Add the transmission delimiter
        UInt8 delimiter = DRN_TRANSMISSION_END;
        streamBuffer.Append( &delimiter, 1 );
        
        if ( !SendData( streamBuffer.GetBufferPtr()        ,
                        (UInt16)streamBuffer.GetDataSize() ,
                        false                              ) )
        {
            // Failed to send data, something is very wrong
            CloseLink();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerStreamListRequest( void )
{GUCEF_TRACE;

    if ( m_linkOperational )
    {
        // Obtain a list of streams publicized on this link
        CDRNPeerLinkData::TDRNDataStreamList dataStreamList;
        m_linkData->GetPublicizedDataStreams( dataStreamList );
        
        // Compose the list message 
        CORE::CDynamicBuffer streamBuffer( 4, true );
        streamBuffer[ 0 ] = DRN_TRANSMISSION_START;
        streamBuffer[ 3 ] = DRN_PEERCOMM_STREAMLIST;
        
        // Copy the list into the buffer
        CDRNPeerLinkData::TDRNDataStreamList::iterator i = dataStreamList.begin();
        while ( i != dataStreamList.end() )
        {
            // Copy the string into the buffer including the null terminator
            streamBuffer.Append( (*i)->GetName().C_String(), (*i)->GetName().Length()+1 );
            ++i;
        }
        
        // Fill in the payload size segment
        UInt16 payloadSize = (UInt16) streamBuffer.GetDataSize() - 4;
        streamBuffer.CopyFrom( 1, 2, &payloadSize );
        
        // Add the transmission delimiter
        UInt8 delimiter = DRN_TRANSMISSION_END;
        streamBuffer.Append( &delimiter, 1 );
        
        if ( !SendData( streamBuffer.GetBufferPtr()        ,
                        (UInt16)streamBuffer.GetDataSize() ,
                        false                              ) )
        {
            // Failed to send data, something is very wrong
            CloseLink();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerDataGroupListReceived( const char* data      ,
                                           const UInt32 dataSize )
{GUCEF_TRACE;

    if ( m_linkOperational )
    {
        
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerDataGroupItemUpdate( const char* data      ,
                                         const UInt32 dataSize )
{GUCEF_TRACE;

    if ( m_linkOperational )
    {
        //m_linkData->
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerGreeting( const char* data      ,
                              const UInt32 dataSize )
{GUCEF_TRACE;

    // First we perform a sanity check on the data size since the
    // greeting has a fixed transmission length
    if ( dataSize == 4 )
    {
        // data size checks out,.. we now handle the payload
        char drnProtocolMayorVersion = data[ 1 ];
        char drnProtocolMinorVersion = data[ 2 ];
        char drnProtocolPatchVersion = data[ 3 ];
        
        if ( ( drnProtocolMayorVersion == DRN_PROTOCOL_MAYOR_VERION ) &&
             ( drnProtocolMinorVersion == DRN_PROTOCOL_MINOR_VERION ) &&
             ( drnProtocolPatchVersion == DRN_PROTOCOL_PATCH_VERION )  )
        {
            // We now know we have a peer that is using a compatible 
            // DRN protocol version.
            SendServiceTypeMessage();
            return;
        }
        else
        {
            // The peer is using a different version of the DRN protocol.
            NotifyObservers( LinkProtocolMismatchEvent );
            NotifyObservers( LinkIncompatibleEvent );
            
            // Notify the peer that we consider the link to be incompatible
            SendIncompatibleLinkMessage();
            
            // Terminate the link
            CloseLink();
            return;            
        }
    }
    else
    {
        // We should not get here
        NotifyObservers( LinkCorruptionEvent );
        
        // Terminate the link
        CloseLink();
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnPeerDataReceived( const char* data      ,
                                  const UInt32 dataSize )
{GUCEF_TRACE;

    switch ( data[ 0 ] )
    {
        case DRN_PEERCOMM_GREETING :
        {
            OnPeerGreeting( data, dataSize );
            return;
        }
        case DRN_PEERCOMM_SERVICE :
        {
            OnPeerServiceType( data, dataSize );
            return;
        }
        case DRN_PEERCOMM_INCOMPATIBLE_LINK :
        {
            OnPeerLinkIncompatible();
            return;
        }
        case DRN_PEERCOMM_AUTHENTICATION_REQUIRED :
        {
            return;
        }
        case DRN_PEERCOMM_AUTHENTICATION_FAILED :
        {
            return;
        }
        case DRN_PEERCOMM_AUTHENTICATION_SUCCESS :
        {
            return;
        }
        case DRN_PEERCOMM_LINK_OPERATIONAL :
        {
            OnPeerLinkOperational();
            return;
        }
        case DRN_PEERCOMM_DATAGROUP_ITEM_UPDATE :
        {
            OnPeerDataGroupItemUpdate( data, dataSize );
            return;
        }
        case DRN_PEERCOMM_STREAM_DATA :
        {
            return;
        }
        case DRN_PEERCOMM_NOT_ALLOWED :
        {
            return;
        }
        case DRN_PEERCOMM_PEERLIST_REQUEST :
        {
            return;
        }
        case DRN_PEERCOMM_PEERLIST :
        {
            return;
        }
        case DRN_PEERCOMM_STREAMLIST_REQUEST :
        {
            OnPeerStreamListRequest();
            return;
        }
        case DRN_PEERCOMM_STREAMLIST :        
        {
            return;
        }
        case DRN_PEERCOMM_DATAGROUPLIST_REQUEST :
        {
            OnPeerDataGroupRequest();
            return;
        }
        case DRN_PEERCOMM_DATAGROUPLIST :
        {
            OnPeerDataGroupListReceived( data, dataSize );
            return;
        }
        case DRN_PEERCOMM_SUBSCRIBE_TO_DATAGROUP_REQUEST :
        {
            return;
        }
        case DRN_PEERCOMM_SUBSCRIBE_TO_STREAM_REQUEST :
        {
            return;
        }        
        default:        
        {
            // If we get here then an unexpected value was found as the command
            // This is not something that should happen or something that is allowed
            NotifyObservers( LinkCorruptionEvent );
            
            // Terminate the link
            CloseLink();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnUDPChannelEvent( CORE::CNotifier* notifier    ,
                                 const CORE::CEvent& eventid  ,
                                 CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    if ( COMCORE::CUDPChannel::UDPPacketReceivedEvent == eventid )
    {
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnTCPDataReceived( const GUCEF::CORE::CDynamicBuffer& buffer )
{GUCEF_TRACE;

    // When using TCP transmissions can get concatenated. We have to separate these
    // transmissions ourselves using the delimiters specified in our DRN protocol.
    UInt32 dataSize = buffer.GetDataSize();
    UInt32 i = m_tcpStreamKeepBytes;
    if ( i < dataSize )
    {
        if ( buffer[ i ] == DRN_TRANSMISSION_START )
        {
            // Check if the data we received is a segmented transmission
            if ( i != 0 )
            {
                // perform an end delimiter sanity check on the previous transmission
                if ( buffer[ i-1 ] == DRN_TRANSMISSION_END )
                {
                    // Append the received prefix to what we have in our buffer
                    // the data segment belongs to an earlier transmission.
                    m_tcpStreamKeepBytes = 0;
                    m_tcpStreamBuffer.Append( buffer.GetConstBufferPtr() ,
                                              i                          ,
                                              true                       );

                    // We now have a complete transmission in our stream buffer
                    // We will now process the buffer
                    OnPeerDataReceived( static_cast< const char* >( m_tcpStreamBuffer.GetConstBufferPtr() ) ,
                                        m_tcpStreamBuffer.GetDataSize()                                     );
                                    
                    // We must now empty the buffer to make room for a new segmented message
                    // We only perform a logical clear to avoid unnecessary reallocations
                    m_tcpStreamBuffer.Clear( true );
                }
                else
                {
                    // We should not get here, the transmission has been corrupted somehow
                    // Even though we have all the data needed we did not find the end
                    // delimiter as expected. 
                    NotifyObservers( LinkCorruptionEvent );
                    
                    // Terminate the link
                    CloseLink();
                    return;
                }
            }
            
            // Sanity check on the buffer size versus payload info offset
            if ( i+2 < dataSize )
            {
                // Get the transmission size
                UInt16 transmissionSize = buffer.AsConstType< UInt16 >( i );
                
                // Sanity check on the buffer size versus reported payload size
                if ( i+2+transmissionSize < dataSize )
                {
                    // Additional sanity check on the reported transmission size
                    if ( buffer[ i+3+transmissionSize ] == DRN_TRANSMISSION_END )
                    {
                        // The payload segment seems to be all here and the size checks
                        // out. We can now proceed with processing the transmission.
                        OnPeerDataReceived( static_cast< const char* >( buffer.GetConstBufferPtr() )+i+2 ,
                                            transmissionSize                                             );                            
                    }
                    else
                    {
                        // We should not get here, the transmission has been corrupted somehow
                        // Even though we have all the data needed we did not find the end
                        // delimiter as expected. 
                        NotifyObservers( LinkCorruptionEvent );
                        
                        // Terminate the link
                        CloseLink();
                        return;                        
                    }
                }
                else
                {
                    // We do not have all the data needed to process the transmission
                    // We will place the data we have in the stream buffer. The data can
                    // then be processed when we receive the remainder of the transmission
                    m_tcpStreamKeepBytes = transmissionSize - ( dataSize-i );
                    m_tcpStreamBuffer.Append( buffer.GetConstBufferPtr( i ) ,
                                              dataSize-i                    ,
                                              true                          );                        
                }
            }
            else
            {
                // if the stream split happened here we are screwed
                // @todo come up with a solution for this
                assert( false );
            }
        }
    }
    else
    {
        // The entire transmission we received is part of a bigger transmission
        // We will append the received data to our stream buffer
        m_tcpStreamKeepBytes -= dataSize;
        m_tcpStreamBuffer.Append( buffer.GetConstBufferPtr() ,
                                  dataSize                   ,
                                  true                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnTCPConnectionEvent( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventid  ,
                                    CORE::CICloneable* eventdata )
{GUCEF_TRACE;


    if ( GUCEF::COMCORE::CTCPConnection::DataRecievedEvent == eventid )
    {
        // Prepare access to the data buffer
        GUCEF::CORE::TLinkedCloneableBuffer* bufferLink = static_cast< GUCEF::CORE::TLinkedCloneableBuffer* >( eventdata );
        const GUCEF::CORE::CDynamicBuffer& buffer = bufferLink->GetData();
        
        // Call the event handler
        OnTCPDataReceived( buffer );
        return;
    }
    else
    if ( GUCEF::COMCORE::CTCPConnection::DataSentEvent == eventid )
    {
        // No need to do anything
        return;
    }
    else
    if ( GUCEF::COMCORE::CTCPConnection::SocketErrorEvent == eventid )
    {
        NotifyObservers( SocketErrorEvent );
        return;    
    }
    else
    if ( GUCEF::COMCORE::CTCPConnection::DisconnectedEvent == eventid )
    {
        NotifyObservers( DisconnectedEvent );
        return;
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::OnNotify( CORE::CNotifier* notifier                 ,
                        const CORE::CEvent& eventid               ,
                        CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( notifier == m_tcpConnection )
    {
        OnTCPConnectionEvent( notifier  ,
                              eventid   ,
                              eventdata );
    }
    else
    if ( notifier == m_udpChannel )
    {
        OnUDPChannelEvent( notifier  ,
                           eventid   ,
                           eventdata );
    }
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLink::RequestPeerList( void )
{GUCEF_TRACE;

    // Send a peer-list-request to the given peer node
    char sendBuffer[ 5 ] = { DRN_TRANSMISSION_START, 0, 0, DRN_PEERCOMM_PEERLIST_REQUEST, DRN_TRANSMISSION_END };
    UInt16 payloadSize = 1;
    memcpy( sendBuffer+1, &payloadSize, 2 );
    
    return SendData( sendBuffer, 2, false );
}

/*-------------------------------------------------------------------------*/
    
bool
CDRNPeerLink::RequestStreamList( void )
{GUCEF_TRACE;

    // Send a stream-list-request to the given peer node
    char sendBuffer[ 5 ] = { DRN_TRANSMISSION_START, 0, 0, DRN_PEERCOMM_STREAMLIST_REQUEST, DRN_TRANSMISSION_END };
    UInt16 payloadSize = 1;
    memcpy( sendBuffer+1, &payloadSize, 2 );
        
    return SendData( sendBuffer, 2, false );
}

/*-------------------------------------------------------------------------*/
    
bool
CDRNPeerLink::RequestDataGroupList( void )
{GUCEF_TRACE;

    // Send a datagroup-list-request to the given peer node
    char sendBuffer[ 5 ] = { DRN_TRANSMISSION_START, 0, 0, DRN_PEERCOMM_DATAGROUPLIST_REQUEST, DRN_TRANSMISSION_END };
    UInt16 payloadSize = 1;
    memcpy( sendBuffer+1, &payloadSize, 2 );
        
    return SendData( sendBuffer, 2, false );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
