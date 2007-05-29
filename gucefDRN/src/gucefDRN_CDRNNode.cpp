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

#ifndef GUCEF_DRN_DRNPROTOCOL_H
#include "gucefDRN_DRNProtocol.h"
#define GUCEF_DRN_DRNPROTOCOL_H
#endif /* GUCEF_DRN_DRNPROTOCOL_H ? */

#ifndef GUCEF_DRN_CDRNPEERLINK_H
#include "gucefDRN_CDRNPeerLink.h"
#define GUCEF_DRN_CDRNPEERLINK_H
#endif /* GUCEF_DRN_CDRNPEERLINK_H ? */

#ifndef GUCEF_DRN_CIDRNPEERVALIDATOR_H
#include "gucefDRN_CIDRNPeerValidator.h"
#define GUCEF_DRN_CIDRNPEERVALIDATOR_H
#endif /* GUCEF_DRN_CIDRNPEERVALIDATOR_H ? */

#include "gucefDRN_CDRNNode.h"

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

const CORE::CEvent CDRNNode::PeerListReceivedFromPeerEvent = "GUCEF::CORE::CDRNNode::PeerListReceivedFromPeerEvent";
const CORE::CEvent CDRNNode::StreamListReceivedFromPeerEvent = "GUCEF::CORE::CDRNNode::StreamListReceivedFromPeerEvent";
const CORE::CEvent CDRNNode::DataGroupListReceivedFromPeerEvent = "GUCEF::CORE::CDRNNode::DataGroupListReceivedFromPeerEvent";
const CORE::CEvent CDRNNode::LinkEstablishedEvent = "GUCEF::CORE::CDRNNode::LinkEstablishedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
   
CDRNNode::CDRNNode( void )
    : CObservingNotifier()               ,
      m_tcpServerSocket( false )         ,
      m_tcpClientSockets()               ,
      m_tcpClientToServerSocket( false ) ,
      m_udpSocket( false )               ,
      m_peerValidator( NULL )            ,
      m_peerLinkCrypter( NULL )          ,
      m_serviceName()
{GUCEF_TRACE;

    SubscribeTo( &m_tcpServerSocket );
    SubscribeTo( &m_udpSocket );
}

/*-------------------------------------------------------------------------*/
    
CDRNNode::~CDRNNode()
{GUCEF_TRACE;

    Disconnect();
    
    TTCPClients::iterator i = m_tcpClientSockets.begin();
    while ( i != m_tcpClientSockets.end() )
    {
        delete (*i);
        ++i;
    }    
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::SetServiceName( const CORE::CString& serviceName )
{GUCEF_TRACE;

    m_serviceName = serviceName;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CDRNNode::GetServiceName( void ) const
{GUCEF_TRACE;
    
    return m_serviceName;
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::SetPeerLinkCrypter( CIDRNPeerLinkCrypter* peerLinkCrypter )
{GUCEF_TRACE;

    m_peerLinkCrypter = peerLinkCrypter;
}

/*-------------------------------------------------------------------------*/
    
CIDRNPeerLinkCrypter*
CDRNNode::GetPeerLinkCrypter( void ) const
{GUCEF_TRACE;

    return m_peerLinkCrypter;
}

/*-------------------------------------------------------------------------*/
    
void
CDRNNode::SetPeerValidator( CIDRNPeerValidator* peerValidator )
{GUCEF_TRACE;

    m_peerValidator = peerValidator;
}

/*-------------------------------------------------------------------------*/
    
CIDRNPeerValidator*
CDRNNode::GetPeervalidator( void ) const
{GUCEF_TRACE;

    return m_peerValidator;
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::OnTCPServerSocketEvent( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventid  ,
                                  CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    if ( eventid == COMCORE::CTCPServerSocket::ClientConnectedEvent )
    {
        const COMCORE::CTCPServerSocket::TConnectionInfo& connectionInfo = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata )->GetData();
        
        // Check if a peer validation mechanism has been provided to this node
        if ( m_peerValidator != NULL )
        {            
            // Check if the peer is allowed to connect
            if ( !m_peerValidator->IsPeerAddressValid( connectionInfo.address  ,
                                                       connectionInfo.hostName ) )
            {
                // This peer is not allowed to connect,.. terminate the connection
                char sendBuffer[ 2 ] = { DRN_PEERCOMM_NOT_ALLOWED, DRN_TRANSMISSION_SEPERATOR };
                connectionInfo.connection->Send( sendBuffer, 2 );
                connectionInfo.connection->Close();
                return;
            }
        }
        //else: assume the peer is allowed to connect
        
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
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::OnTCPClientPeerSocketEvent( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventid  ,
                                      CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    if ( eventid == COMCORE::CTCPClientSocket::ConnectedEvent )
    {
        
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::OnNotify( CORE::CNotifier* notifier                 ,
                    const CORE::CEvent& eventid               ,
                    CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( notifier == &m_tcpClientToServerSocket )
    {
    
    }
    else
    if ( notifier == &m_tcpServerSocket )
    {
        OnTCPServerSocketEvent( notifier  ,
                                eventid   ,
                                eventdata );
    }
    else
    if ( notifier == &m_udpSocket )
    {
    }
    else
    {
        OnTCPClientPeerSocketEvent( notifier  ,
                                    eventid   ,
                                    eventdata );
    }
}

/*-------------------------------------------------------------------------*/

bool
CDRNNode::ListenOnPort( const UInt16 port )
{GUCEF_TRACE;
    
    return m_tcpServerSocket.ListenOnPort( port );
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::StopListening( void )
{GUCEF_TRACE;

    m_tcpServerSocket.Close();
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::Disconnect( void )
{GUCEF_TRACE;

    m_tcpServerSocket.Close();
    m_udpSocket.Close();

    TTCPClients::iterator i = m_tcpClientSockets.begin();
    while ( i != m_tcpClientSockets.end() )
    {
        (*i)->Close();        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CDRNNode::IsListening( void ) const
{GUCEF_TRACE;

    return m_tcpServerSocket.IsActive();
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::GetAvailableTCPClient( COMCORE::CTCPClientSocket** tcpClient ,
                                 UInt32& socketIndex                   )
{GUCEF_TRACE;

    // Try to find an inactive socket object we can re-use
    UInt32 index = 0;
    TTCPClients::iterator i = m_tcpClientSockets.begin();
    while ( i != m_tcpClientSockets.end() )
    {
        if ( !(*i)->IsActive() )
        {
            // Assign output values
            *tcpClient = *i;
            socketIndex = index;
            return;
        }
        ++i; ++index;
    }
    
    // no such luck,.. we will create a new TCP client socket
    COMCORE::CTCPClientSocket* clientSocket = new COMCORE::CTCPClientSocket( false );
    SubscribeTo( clientSocket );    
    m_tcpClientSockets.push_back( clientSocket );
    
    // Assign output values
    socketIndex = (Int32)m_tcpClientSockets.size() - 1;
    *tcpClient = clientSocket;
}

/*-------------------------------------------------------------------------*/

bool
CDRNNode::ConnectToPeer( const CORE::CString& address , 
                         const UInt16 port            )
{GUCEF_TRACE;

    COMCORE::CTCPClientSocket* tcpClient = NULL;
    UInt32 socketIndex = 0;
    
    // Obtain a TCP client socket
    GetAvailableTCPClient( &tcpClient  ,
                           socketIndex );
            
    if ( tcpClient->ConnectTo( address , 
                               port    ) )
    {
        // Send the initial greeting
        char sendBuffer[ 14 ] = { DRN_PEERCOMM_GREETING, 'D', 'R', 'N', 'N', 'O', 'D', 'E', ' ', ' ', DRN_PROTOCOL_MAYOR_VERION, DRN_PROTOCOL_MINOR_VERION, DRN_PROTOCOL_PATCH_VERION, DRN_TRANSMISSION_SEPERATOR };        
        if ( tcpClient->Send( sendBuffer, 14 ) )
        {
            return true;
        }
        
        // Failed to send data, something is very wrong
        tcpClient->Close();        
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CDRNNode::RequestPeerList( CDRNPeerLink& peerLink )
{GUCEF_TRACE;

    // Send a peer-list-request to the given peer node
    char sendBuffer[ 2 ] = { DRN_PEERCOMM_PEERLIST_REQUEST, DRN_TRANSMISSION_SEPERATOR };
    return peerLink.SendData( sendBuffer, 2, false );
}

/*-------------------------------------------------------------------------*/
    
bool
CDRNNode::RequestStreamList( CDRNPeerLink& peerLink )
{GUCEF_TRACE;

    // Send a stream-list-request to the given peer node
    char sendBuffer[ 2 ] = { DRN_PEERCOMM_STREAMLIST_REQUEST, DRN_TRANSMISSION_SEPERATOR };
    return peerLink.SendData( sendBuffer, 2, false );
}

/*-------------------------------------------------------------------------*/
    
bool
CDRNNode::RequestDataGroupList( CDRNPeerLink& peerLink )
{GUCEF_TRACE;

    // Send a datagroup-list-request to the given peer node
    char sendBuffer[ 2 ] = { DRN_PEERCOMM_DATAGROUPLIST_REQUEST, DRN_TRANSMISSION_SEPERATOR };
    return peerLink.SendData( sendBuffer, 2, false );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
