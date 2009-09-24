/*
 *  ServerPortExtender: Generic networking tool for connecting clients
 *  to a server that is behind a NAT firewall.
 *
 *  Copyright (C) 2002 - 2009.  Dinand Vanvelzen
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

#include "CServerPortExtenderClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CServerPortExtenderClient::CServerPortExtenderClient( void )
    : CORE::CObserver()                     ,
      m_controlClient( false )              ,
      m_rsClientConnections()               ,
      m_localClientConnections()            ,
      m_remoteToLocalConnectionMap()        ,
      m_localToRemoteConnectionMap()        ,
      m_localServer()                       ,
      m_remoteSPEServer()                   ,
      m_remoteSPEServerPort( 10235 )
{GUCEF_TRACE;

    m_remoteSPEServer.SetPortInHostByteOrder( 10236 );
    m_localServer.SetPortInHostByteOrder( 10235 );
    
    SubscribeTo( &m_controlClient );
}

/*-------------------------------------------------------------------------*/

bool
CServerPortExtenderClient::ConnectToSPEControlSocket( const COMCORE::CHostAddress& host )
{GUCEF_TRACE;

    m_remoteSPEServer = host;
    return m_controlClient.ConnectTo( host );
}

/*-------------------------------------------------------------------------*/

bool
CServerPortExtenderClient::ConnectToSPEControlSocket( const CORE::CString& hostname , 
                                                      CORE::UInt16 controlPort      )
{GUCEF_TRACE;

    COMCORE::CHostAddress host( hostname, controlPort );
    return ConnectToSPEControlSocket( host );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::SetLocalServer( const COMCORE::CHostAddress& host )
{GUCEF_TRACE;

    m_localServer = host;
}

/*-------------------------------------------------------------------------*/
    
void
CServerPortExtenderClient::SetLocalServer( const CORE::CString& hostname , 
                                           CORE::UInt16 port             )
{GUCEF_TRACE;
    
    COMCORE::CHostAddress host( hostname, port );
    SetLocalServer( host );
}

/*-------------------------------------------------------------------------*/
    
void
CServerPortExtenderClient::SetRemoteServerSocket( CORE::UInt16 port )
{GUCEF_TRACE;
    
    m_remoteSPEServerPort = port;
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::OnControlMsg( TServerPortExtenderProtocolEnum msgType, )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::OnControlClientNotify( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventid  ,
                                                  CORE::CICloneable* eventdata )
{GUCEF_TRACE;
    
    if ( COMCORE::CTCPClientSocket::ConnectedEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPClientSocket::DisconnectedEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPClientSocket::DataRecievedEvent == eventid )
    {
        // Get the data buffer
        COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventdata );
        const CORE::CDynamicBuffer& data = eData->GetData();
        
        UInt32 dataSize = data.GetDataSize();
        
        
        
        
    }
    else
    if ( COMCORE::CTCPClientSocket::DataSentEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPClientSocket::SocketErrorEvent == eventid )
    {
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::OnNotify( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventid  ,
                                     CORE::CICloneable* eventdata )
{GUCEF_TRACE;
    
    if ( &m_controlClient == notifier )
    {
        OnControlClientNotify( notifier  , 
                               eventid   ,
                               eventdata );
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::MapLocalToRemoteConnection( COMCORE::CTCPClientSocket* localSocket  ,
                                                       COMCORE::CTCPClientSocket* remoteSocket )
{GUCEF_TRACE;

    m_remoteToLocalConnectionMap[ remoteSocket->GetSocketID() ] = localSocket;
    m_localToRemoteConnectionMap[ localSocket->GetSocketID() ] = remoteSocket;
}

/*-------------------------------------------------------------------------*/

COMCORE::CTCPClientSocket*
CServerPortExtenderClient::GetRemoteConnectionForLocalConnection( const CORE::UInt32 socketId )
{GUCEF_TRACE;

    TSocketIdMap::iterator i = m_remoteToLocalConnectionMap.find( socketId );
    if ( i != m_remoteToLocalConnectionMap.end() )
    {
        return (*i).second;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/
    
COMCORE::CTCPClientSocket*
CServerPortExtenderClient::GetLocalConnectionForRemoteConnection( const CORE::UInt32 socketId )
{GUCEF_TRACE;

    TSocketIdMap::iterator i = m_localToRemoteConnectionMap.find( socketId );
    if ( i != m_localToRemoteConnectionMap.end() )
    {
        return (*i).second;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/