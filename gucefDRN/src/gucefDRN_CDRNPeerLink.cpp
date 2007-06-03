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
const CORE::CEvent CDRNPeerLink::PeerListReceivedFromPeerEvent = "GUCEF::CORE::CDRNPeerLink::PeerListReceivedFromPeerEvent";
const CORE::CEvent CDRNPeerLink::StreamListReceivedFromPeerEvent = "GUCEF::CORE::CDRNPeerLink::StreamListReceivedFromPeerEvent";
const CORE::CEvent CDRNPeerLink::DataGroupListReceivedFromPeerEvent = "GUCEF::CORE::CDRNPeerLink::DataGroupListReceivedFromPeerEvent";
    
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
      m_parentNode( &parentNode )
{GUCEF_TRACE;

    assert( m_udpSocket != NULL );
    assert( m_parentNode != NULL );
    assert( m_tcpConnection != NULL );    
    RegisterEvents();
    
    m_linkData = new CDRNPeerLinkData( *this );
    
    // Send the initial greeting
    char sendBuffer[ 14 ] = { DRN_PEERCOMM_GREETING, 'D', 'R', 'N', 'N', 'O', 'D', 'E', ' ', ' ', DRN_PROTOCOL_MAYOR_VERION, DRN_PROTOCOL_MINOR_VERION, DRN_PROTOCOL_PATCH_VERION, DRN_TRANSMISSION_SEPERATOR };        
    if ( !SendData( sendBuffer, 14, false ) )
    {
        // Failed to send data, something is very wrong
        CloseLink();
    }    
}

/*-------------------------------------------------------------------------*/

CDRNPeerLink::~CDRNPeerLink()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::CloseLink( void )
{GUCEF_TRACE;

    m_tcpConnection->Close();
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
CDRNPeerLink::IsActive( void ) const
{GUCEF_TRACE;

    if ( NULL != m_tcpConnection )
    {
        return m_tcpConnection->IsActive();
    }
    return false;
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
CDRNPeerLink::OnTCPConnectionEvent( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventid  ,
                                    CORE::CICloneable* eventdata )
{GUCEF_TRACE;

        if ( GUCEF::COMCORE::CTCPConnection::ConnectedEvent == eventid )
        {
        }
        else
        if ( GUCEF::COMCORE::CTCPConnection::DisconnectedEvent == eventid )
        {
        }
        else
        if ( GUCEF::COMCORE::CTCPConnection::DataRecievedEvent == eventid )
        {
        
        }
        else
        if ( GUCEF::COMCORE::CTCPConnection::DataSentEvent == eventid )
        {
        
        }
        else
        if ( GUCEF::COMCORE::CTCPConnection::SocketErrorEvent == eventid )
        {
        }

    /*        
        // Send the initial greeting
        char sendBuffer[ 14 ] = { DRN_PEERCOMM_GREETING, 'D', 'R', 'N', 'N', 'O', 'D', 'E', ' ', ' ', DRN_PROTOCOL_MAYOR_VERION, DRN_PROTOCOL_MINOR_VERION, DRN_PROTOCOL_PATCH_VERION, DRN_TRANSMISSION_SEPERATOR };        
        if ( !connectionInfo.connection->Send( sendBuffer, 14 ) )
        {
            // Failed to send data, something is very wrong
            connectionInfo.connection->Close();
        }
        return;
    }
    if ( eventid == COMCORE::CTCPServerSocket::ClientDataRecievedEvent )
    {
        // Any data we receive in this phase should be a greeting message
        // nothing else is accepted. If we do get something else then the connection
        // is considered broken and terminated.
    }          */

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
    if ( notifier == m_udpSocket )
    {
    }
}

/*-------------------------------------------------------------------------*/

bool
CDRNPeerLink::RequestPeerList( void )
{GUCEF_TRACE;

    // Send a peer-list-request to the given peer node
    char sendBuffer[ 2 ] = { DRN_PEERCOMM_PEERLIST_REQUEST, DRN_TRANSMISSION_SEPERATOR };
    return SendData( sendBuffer, 2, false );
}

/*-------------------------------------------------------------------------*/
    
bool
CDRNPeerLink::RequestStreamList( void )
{GUCEF_TRACE;

    // Send a stream-list-request to the given peer node
    char sendBuffer[ 2 ] = { DRN_PEERCOMM_STREAMLIST_REQUEST, DRN_TRANSMISSION_SEPERATOR };
    return SendData( sendBuffer, 2, false );
}

/*-------------------------------------------------------------------------*/
    
bool
CDRNPeerLink::RequestDataGroupList( void )
{GUCEF_TRACE;

    // Send a datagroup-list-request to the given peer node
    char sendBuffer[ 2 ] = { DRN_PEERCOMM_DATAGROUPLIST_REQUEST, DRN_TRANSMISSION_SEPERATOR };
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
