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

#ifndef GUCEF_DRN_CDRNPEERLINKDATA_H
#include "gucefDRN_CDRNPeerLinkData.h"
#define GUCEF_DRN_CDRNPEERLINKDATA_H
#endif /* GUCEF_DRN_CDRNPEERLINKDATA_H ? */

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

const CORE::CEvent CDRNNode::LinkEstablishedEvent = "GUCEF::DRN::CDRNNode::LinkEstablishedEvent";
const CORE::CEvent CDRNNode::LinkDisconnectedEvent = "GUCEF::DRN::CDRNNode::LinkDisconnectedEvent";
const CORE::CEvent CDRNNode::LinkErrorEvent = "GUCEF::DRN::CDRNNode::LinkErrorEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
   
void
CDRNNode::RegisterEvents( void )
{GUCEF_TRACE;
    
    LinkEstablishedEvent.Initialize();
    LinkDisconnectedEvent.Initialize();
    LinkErrorEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CDRNNode::CDRNNode( void )
    : CObservingNotifier()               ,
      m_tcpServerSocket( false )         ,
      m_tcpClientSockets()               ,
      m_udpSocket( false )               ,
      m_peerValidator( NULL )            ,
      m_peerLinkCrypter( NULL )          ,
      m_peerLinkList()                   ,
      m_dataGroupList()                  ,
      m_dataStreamList()                 ,
      m_overrideConnectBack( false )     ,
      m_overrideHost( "127.0.0.1" ,
                      53456       )      ,
      m_overrideUDPPort( 53457 )
{GUCEF_TRACE;
    
    RegisterEvents();

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

UInt32
CDRNNode::GetOperationalLinkForUsCount( void ) const
{GUCEF_TRACE;

    UInt32 activeCount = 0;
    TPeerLinkList::const_iterator i = m_peerLinkList.begin();
    while ( i != m_peerLinkList.end() )
    {
        if ( (*i)->IsOperationalForUs() )
        {
            ++activeCount;
        }
        ++i;
    }
    return activeCount;
}

/*-------------------------------------------------------------------------*/

bool
CDRNNode::HasOperationalLinksForUs( void ) const
{GUCEF_TRACE;

    TPeerLinkList::const_iterator i = m_peerLinkList.begin();
    while ( i != m_peerLinkList.end() )
    {
        if ( (*i)->IsOperationalForUs() )
        {
            return true;
        }
        ++i;
    }
    return false;
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
CDRNNode::OnNotify( CORE::CNotifier* notifier                 ,
                    const CORE::CEvent& eventid               ,
                    CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    // Check if the message originated from our own TCP server
    if ( notifier == &m_tcpServerSocket )
    {
        if ( COMCORE::CTCPServerSocket::ClientConnectedEvent == eventid )
        {
            const COMCORE::CTCPServerSocket::TConnectionInfo& connectionInfo = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata )->GetData();
            
            // Check if a peer validation mechanism has been provided to this node
            if ( m_peerValidator != NULL )
            {            
                // Check if the peer is allowed to connect
                if ( !m_peerValidator->IsPeerAddressValid( connectionInfo.address  ,
                                                           connectionInfo.hostName ) )
                {
                    connectionInfo.connection->Close();
                    return;
                }
            }
            //else: assume the peer is allowed to connect

            CreateLink( *connectionInfo.connection );
        }
    }
    else
    if ( COMCORE::CTCPClientSocket::ConnectedEvent == eventid  )
    {
        COMCORE::CTCPClientSocket* clientSocket = static_cast< COMCORE::CTCPClientSocket* >( notifier );
        UnsubscribeFrom( clientSocket );
        CreateLink( *clientSocket );
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::CreateLink( COMCORE::CTCPConnection& tcpConnection )
{GUCEF_TRACE;

    // create a new link object        
    CDRNPeerLinkPtr peerLink( new CDRNPeerLink( *this         ,
                                                tcpConnection ,
                                                m_udpSocket   ) );
    m_peerLinkList.push_back( peerLink );
    
    // Publish our global publicized data groups and streams
    CDRNPeerLinkData& dataHolder = peerLink->GetLinkData();    
    TDataGroupList::iterator i = m_dataGroupList.begin();
    while ( i != m_dataGroupList.end() )
    {
        dataHolder.PublicizeDataGroup( (*i).second );
        ++i;
    }
    TDataStreamList::iterator n = m_dataStreamList.begin();
    while ( n != m_dataStreamList.end() )
    {
        dataHolder.PublicizeStream( (*n).second );
        ++n;
    }
        
    // Notify that a new link has been established
    LinkEstablishedEventData eData( peerLink );
    NotifyObservers( LinkEstablishedEvent, &eData );
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

    TPeerLinkList::iterator i = m_peerLinkList.begin();
    while ( i != m_peerLinkList.end() )
    {
        (*i)->CloseLink();        
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

    // Check if a peer validation mechanism has been provided to this node
    if ( m_peerValidator != NULL )
    {            
        // Check if the peer connection is allowed
        COMCORE::CIPAddress ip( address, port );
        if ( !m_peerValidator->IsPeerAddressValid( ip      ,
                                                   address ) )
        {   
            return false;
        }
    }
         
    // Obtain a TCP client socket
    COMCORE::CTCPClientSocket* tcpClient = NULL;
    UInt32 socketIndex = 0;
    GetAvailableTCPClient( &tcpClient  ,
                           socketIndex );
            
    // Attempt the connection
    SubscribeTo( tcpClient );
    return tcpClient->ConnectTo( address , 
                                 port    );
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::PublicizeStream( TDRNDataStreamPtr& dataStream )
{GUCEF_TRACE;

    // Add the data stream to the global list
    m_dataStreamList[ dataStream->GetName() ] = dataStream;
    
    // Publicize to the existing links
    TPeerLinkList::iterator i = m_peerLinkList.begin();
    while ( i != m_peerLinkList.end() )
    {
        (*i)->GetLinkData().PublicizeStream( dataStream );
        ++i;
    } 
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::StopStreamPublication( const CORE::CString& dataStreamName )
{GUCEF_TRACE;

    // Remove the stream from the global list
    m_dataStreamList.erase( dataStreamName );
    
    // Stop publication for the existing links
    TPeerLinkList::iterator i = m_peerLinkList.begin();
    while ( i != m_peerLinkList.end() )
    {
        (*i)->GetLinkData().StopStreamPublication( dataStreamName );
        ++i;
    }    
}

/*-------------------------------------------------------------------------*/
    
void
CDRNNode::PublicizeDataGroup( TDRNDataGroupPtr& dataGroup )
{GUCEF_TRACE;

    // Add the data group to the global list
    m_dataGroupList[ dataGroup->GetName() ] = dataGroup;
    
    // Publicize to the existing links
    TPeerLinkList::iterator i = m_peerLinkList.begin();
    while ( i != m_peerLinkList.end() )
    {
        (*i)->GetLinkData().PublicizeDataGroup( dataGroup );
        ++i;
    }    
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::StopDataGroupPublication( const CORE::CString& dataGroupName )
{GUCEF_TRACE;

    // Remove the data group from the global list
    m_dataGroupList.erase( dataGroupName );
    
    // Stop publication for the existing links
    TPeerLinkList::iterator i = m_peerLinkList.begin();
    while ( i != m_peerLinkList.end() )
    {
        (*i)->GetLinkData().StopDataGroupPublication( dataGroupName );
        ++i;
    }    
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::GetPeerLinkList( TPeerLinkList& peerLinkList )
{GUCEF_TRACE;

    peerLinkList = m_peerLinkList;
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::SetOverrideConnectBackSettings( const bool overrideSettings )
{GUCEF_TRACE;

    if ( m_overrideConnectBack != overrideSettings )
    {    
        m_overrideConnectBack = overrideSettings;
        SendConnectBackInfoToLinks();
    }
}

/*-------------------------------------------------------------------------*/
    
bool
CDRNNode::GetOverrideConnectBackSettings( void ) const
{GUCEF_TRACE;

    return m_overrideConnectBack;
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::SetConnectBackOverride( const CHostAddress& host ,
                                  const UInt16 udpPort     )
{GUCEF_TRACE;

    if ( ( m_overrideHost != host )       ||
         ( m_overrideUDPPort != udpPort )  )
    {
        m_overrideHost = host;
        m_overrideUDPPort = udpPort;
        
        if ( m_overrideConnectBack )
        {
            SendConnectBackInfoToLinks();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::GetConnectBackOverride( CHostAddress& host ,
                                  UInt16& udpPort    ) const
{GUCEF_TRACE;

    host = m_overrideHost;
    udpPort = m_overrideUDPPort;    
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::SendConnectBackInfoToLinks( void )
{GUCEF_TRACE;

    // Notify all peers that our connect back info has changed
    TPeerLinkList::iterator i = m_peerLinkList.begin();
    while ( i != m_peerLinkList.end() )
    {
        (*i)->SendConnectBackInfo();
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CDRNNode::OpenUDPPort( const UInt16 port )
{GUCEF_TRACE;

    if ( m_udpSocket.IsActive()        &&
         m_udpSocket.GetPort() == port )
    {
        // The socket is already open at the given port
        return true;
    }
    
    bool retValue = m_udpSocket.Open( port );
    SendConnectBackInfoToLinks();
    return retValue;
}

/*-------------------------------------------------------------------------*/
    
void
CDRNNode::CloseUDPPort( void )
{GUCEF_TRACE;

    if ( !m_udpSocket.IsActive() )
    {
        // No change,.. the socket is already closed
        return;
    }
    
    m_udpSocket.Close();
    SendConnectBackInfoToLinks();
}
    
/*-------------------------------------------------------------------------*/
    
bool
CDRNNode::IsUDPPortOpened( void ) const
{GUCEF_TRACE;

    return m_udpSocket.IsActive();
}

/*-------------------------------------------------------------------------*/

void
CDRNNode::GetListenAddress( CHostAddress& listenAddress ) const
{GUCEF_TRACE;

    m_tcpServerSocket.GetListenAddress( listenAddress );
}

/*-------------------------------------------------------------------------*/

UInt16
CDRNNode::GetUDPPort( void ) const
{GUCEF_TRACE;

    return m_udpSocket.GetPort();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
