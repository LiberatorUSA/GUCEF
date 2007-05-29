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

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_DRN_CDRNPEERLINKDATA_H
#include "gucefDRN_CDRNPeerLinkData.h"
#define GUCEF_DRN_CDRNPEERLINKDATA_H
#endif /* GUCEF_DRN_CDRNPEERLINKDATA_H ? */

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
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDRNPeerLink::CDRNPeerLink( void )
    : m_udpSocket( NULL )        ,
      m_tcpConnection( NULL )    ,
      m_udpPossible( false )     ,
      m_isAuthenticated( false ) ,
      m_linkData( NULL )        
{GUCEF_TRACE;

    RegisterEvents();
    
    m_linkData = new CDRNPeerLinkData( *this );
}

/*-------------------------------------------------------------------------*/

CDRNPeerLink::~CDRNPeerLink()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CDRNPeerLink::RegisterEvents( void )
{GUCEF_TRACE;

    ConnectedEvent.Initialize();
    DisconnectedEvent.Initialize();
    SocketErrorEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CDRNPeerLink::TIPAddress
CDRNPeerLink::GetPeerIP( void ) const
{GUCEF_TRACE;

    return m_tcpConnection->GetRemoteIP();
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CDRNPeerLink::GetPeerHostName( void ) const
{GUCEF_TRACE;

    return m_tcpConnection->GetRemoteHostName();
}

/*-------------------------------------------------------------------------*/
    
UInt16
CDRNPeerLink::GetPeerTCPPort( void ) const
{GUCEF_TRACE;    
    
    return m_tcpConnection->GetRemoteTCPPort();
}

/*-------------------------------------------------------------------------*/

UInt16
CDRNPeerLink::GetPeerUDPPort( void ) const
{GUCEF_TRACE;

    return m_tcpConnection->GetRemoteTCPPort();    
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

void
CDRNPeerLink::SetTCPConnection( COMCORE::CTCPConnection& tcpConnection )
{GUCEF_TRACE;

    assert( &tcpConnection != NULL );
    
    if ( m_tcpConnection != NULL )
    {
        UnsubscribeFrom( m_tcpConnection );
    }
    
    m_tcpConnection = &tcpConnection;
}

/*-------------------------------------------------------------------------*/

COMCORE::CUDPSocket*
CDRNPeerLink::GetUDPSocket( void )
{GUCEF_TRACE;

    return m_udpSocket;
}

/*-------------------------------------------------------------------------*/
    
void
CDRNPeerLink::SetUDPSocket( COMCORE::CUDPSocket& socket )
{GUCEF_TRACE;

    assert( &socket != NULL );
    
    if ( m_udpSocket != NULL )
    {
        UnsubscribeFrom( m_udpSocket );
    }
    
    m_udpSocket = &socket;
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
        TIPAddress ip( m_tcpConnection->GetRemoteIP() );
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
CDRNPeerLink::OnNotify( CORE::CNotifier* notifier                 ,
                        const CORE::CEvent& eventid               ,
                        CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
