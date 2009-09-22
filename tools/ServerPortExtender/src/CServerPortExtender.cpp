/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef SERVERPORTEXTENDERPROTOCOL_H
#include "ServerPortExtenderProtocol.h"
#define SERVERPORTEXTENDERPROTOCOL_H
#endif /* SERVERPORTEXTENDERPROTOCOL_H ? */

#include "CServerPortExtender.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CServerPortExtender::CServerPortExtender( void )
    : CObserver()                            ,
      m_reversedServerControlSocket( false ) ,
      m_reversedServerSocket( false )        ,
      m_serverSocket( false )                ,
      m_controlConnection( NULL )            ,
      m_serverPort( 76234 )                  ,
      m_reversedServerPort( 67234 )          ,
      m_reversedServerControlPort( 67235 )   ,
      m_remoteToLocalConnectionMap()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CServerPortExtender::ListenForClientsOnPort( UInt16 port )
{GUCEF_TRACE;

    m_serverPort = port;
    return m_serverSocket.ListenOnPort( m_serverPort );
}

/*-------------------------------------------------------------------------*/
    
bool
CServerPortExtender::ListenForReversedClientControlOnPort( UInt16 port )
{GUCEF_TRACE;

    m_reversedServerControlPort = port;
    return m_reversedServerControlSocket.ListenOnPort( m_reversedServerControlPort );
}

/*-------------------------------------------------------------------------*/
    
bool
CServerPortExtender::ListenForReversedClientsOnPort( UInt16 port )
{GUCEF_TRACE;

    m_reversedServerPort = port;
    return m_reversedServerSocket.ListenOnPort( m_reversedServerPort );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSControlClientConnected( CNotifier* notifier    ,
                                                 const CEvent& eventid  ,
                                                 CICloneable* eventdata )
{GUCEF_TRACE;

    CTCPServerConnection* connection = static_cast< CTCPServerConnection* >( notifier );
    
    // Send welcome handshake with protocol version so the client can determine
    // whether it is compatible with this server
    
    char msg[ 4 ];
    msg[ 0 ] = (char) SPEP_INITIAL_CONTROL_SERVER_MSG;
    msg[ 1 ] = 45; // magic number, additional sanity check
    msg[ 2 ]= SPEP_PROTOCOL_VERSION;
    connection->Send( msg, 3 );
    
    // If we already have an existing control connection we should close it
    // we only support 1 control connection at a time.
    if ( NULL != m_controlConnection )
    {
        m_controlConnection->Close();
        m_controlConnection = NULL;
    }
    m_controlConnection = connection;
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSControlClientDisconnected( CNotifier* notifier    ,
                                                    const CEvent& eventid  ,
                                                    CICloneable* eventdata )
{GUCEF_TRACE;

    CTCPServerConnection* connection = static_cast< CTCPServerConnection* >( notifier );
    if ( NULL != m_controlConnection )
    {
        // Make sure the control connection object is no longer used since we where
        // disconnected
        if ( m_controlConnection == connection )
        {
            m_controlConnection = NULL;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSControlClientDataSent( CNotifier* notifier    ,
                                                const CEvent& eventid  ,
                                                CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSControlClientSocketError( CNotifier* notifier    ,
                                                   const CEvent& eventid  ,
                                                   CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSControlClientDataRecieved( CNotifier* notifier    ,
                                                    const CEvent& eventid  ,
                                                    CICloneable* eventdata )
{GUCEF_TRACE;

    CTCPConnection* connection = static_cast< CTCPConnection* >( notifier );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientConnected( CNotifier* notifier    ,
                                          const CEvent& eventid  ,
                                          CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTCPServerConnection*
CServerPortExtender::GetLocalConnectionForRemoteConnection( UInt32 rsSocketId )
{GUCEF_TRACE;

    TConnectionMap::iterator i = m_remoteToLocalConnectionMap.find( rsSocket );
    if ( i != m_remoteToLocalConnectionMap.end() )
    {
        return m_serverSocket.GetConnectionBySocketId( (*i) );
    }
    return NULL; 
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientDisconnected( CNotifier* notifier    ,
                                             const CEvent& eventid  ,
                                             CICloneable* eventdata )
{GUCEF_TRACE;

    // Since the remote link has been severed we should sever the local link since its
    // the same virtual socket connection    
    CTCPServerConnection* rsConnection = static_cast< CTCPServerConnection* >( notifier );
    CTCPServerConnection* lConnection = GetLocalConnectionForRemoteConnection( connection->GetSocketID() );
    if ( NULL != lConnection )
    {
        lConnection->Close();
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientDataSent( CNotifier* notifier    ,
                                              const CEvent& eventid  ,
                                              CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientSocketError( CNotifier* notifier    ,
                                            const CEvent& eventid  ,
                                            CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientDataRecieved( CNotifier* notifier    ,
                                             const CEvent& eventid  ,
                                             CICloneable* eventdata )
{GUCEF_TRACE;

    CTCPServerConnection* rsConnection = static_cast< CTCPServerConnection* >( notifier );
    CTCPServerConnection* lConnection = GetLocalConnectionForRemoteConnection( connection->GetSocketID() );
    if ( NULL != lConnection )
    {
        lConnection->Close();
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientConnected( CNotifier* notifier    ,
                                        const CEvent& eventid  ,
                                        CICloneable* eventdata )
{GUCEF_TRACE;

    // If a client connects to our extended port we have to make sure 
    // it triggers the remote SPE client to establish a connection in the 
    // opposite direction to this SPE server and establish a connection to 
    // the actual remote server port
    
    CTCPServerConnection* clientConnection = static_cast< CTCPServerConnection* >( notifier );

    // Send the request for a new connection to be established    
    char msg[ 5 ];
    msg[ 0 ] = (char) SPEP_CONTROL_SERVER_MSG_CREATE_CONNECTION_REQUEST;
    memcpy( msg+1, &clientConnection->GetSocketID(), 4 );
    m_controlConnection->Send( msg, 5 );  
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientDisconnected( CNotifier* notifier    ,
                                           const CEvent& eventid  ,
                                           CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientDataSent( CNotifier* notifier    ,
                                       const CEvent& eventid  ,
                                       CICloneable* eventdata )
{GUCEF_TRACE;

    CTCPServerConnection* clientConnection = static_cast< CTCPServerConnection* >( notifier );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientSocketError( CNotifier* notifier    ,
                                          const CEvent& eventid  ,
                                          CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientDataRecieved( CNotifier* notifier    ,
                                           const CEvent& eventid  ,
                                           CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnReversedServerControlSocketNotify( CNotifier* notifier    ,
                                                          const CEvent& eventid  ,
                                                          CICloneable* eventdata )
{GUCEF_TRACE;

    // this is where the remote connection comes in that will carry all the control
    // commands to operate the reversed client-server tunnel

    if ( CTCPServerSocket::ClientConnectedEvent == eventid )
    {
        // Now that the control connection is established we can listen for clients on our extended server port
        if ( !m_serverSocket.IsActive() )
        {
            m_serverSocket.ListenOnPort( m_serverPort );
        }
        
        // Get the connection object
        CTCPServerSocket::TClientConnectedEventData* eData = static_cast< CTCPServerSocket::TClientConnectedEventData* >( eventdata );
        CTCPServerSocket::TConnectionInfo& connectionInfo = eData->GetData();

        // Subscribe to connection events        
        Subscribe( connectionInfo.connection                                                      ,
                   CTCPServerConnection::ConnectedEvent                                           ,
                   &TEventCallback( this, &CServerPortExtender::OnRSControlClientConnectedEvent ) );
        Subscribe( connectionInfo.connection                                                         ,
                   CTCPServerConnection::ConnectedEvent                                              ,
                   &TEventCallback( this, &CServerPortExtender::OnRSControlClientDisconnectedEvent ) );
        Subscribe( connectionInfo.connection                                                ,
                   CTCPServerConnection::ConnectedEvent                                     ,
                   &TEventCallback( this, &CServerPortExtender::OnRSControlClientDataSent ) );
        Subscribe( connectionInfo.connection                                                   ,
                   CTCPServerConnection::ConnectedEvent                                        ,
                   &TEventCallback( this, &CServerPortExtender::OnRSControlClientSocketError ) );
        Subscribe( connectionInfo.connection                                                    ,
                   CTCPServerConnection::ConnectedEvent                                         ,
                   &TEventCallback( this, &CServerPortExtender::OnRSControlClientDataRecieved ) );
                   
    }
    else
    if ( CTCPServerSocket::ClientDisconnectedEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ClientErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketOpenedEvent == eventid )
    {
        GUCEF_LOG( 0, "ServerPortExtender: Reversed server socket opened,.. waiting for ServerPortExtenderClient" );
    }
    else
    if ( CTCPServerSocket::ServerSocketClosedEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketClientErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent == eventid )
    {
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnReversedServerSocketNotify( CNotifier* notifier    ,
                                                   const CEvent& eventid  ,
                                                   CICloneable* eventdata )
{GUCEF_TRACE;

    // this is where the remote connection comes in to establish the reversed
    // client-server connnection. This is the connection we have to keep alive to
    // allow reversed client-server tunneling

    if ( CTCPServerSocket::ClientConnectedEvent == eventid )
    {
        if ( !m_serverSocket.IsActive() )
        {
            m_serverSocket.ListenOnPort( m_serverPort );
        }
        
        CTCPServerSocket::TClientConnectedEventData* eData = static_cast< CTCPServerSocket::TClientConnectedEventData* >( eventdata );
        CTCPServerSocket::TConnectionInfo& connectionInfo = eData->GetData();
        
        Subscribe( connectionInfo.connection                                             ,
                   CTCPServerConnection::ConnectedEvent                                  ,
                   &TEventCallback( this, &CServerPortExtender::OnRSClientConnectedEvent ) );
        Subscribe( connectionInfo.connection                                                  ,
                   CTCPServerConnection::ConnectedEvent                                       ,
                   &TEventCallback( this, &CServerPortExtender::OnRSClientDisconnectedEvent ) );
        Subscribe( connectionInfo.connection                                         ,
                   CTCPServerConnection::ConnectedEvent                              ,
                   &TEventCallback( this, &CServerPortExtender::OnRSClientDataSent ) );
        Subscribe( connectionInfo.connection                                            ,
                   CTCPServerConnection::ConnectedEvent                                 ,
                   &TEventCallback( this, &CServerPortExtender::OnRSClientSocketError ) );
        Subscribe( connectionInfo.connection                                             ,
                   CTCPServerConnection::ConnectedEvent                                  ,
                   &TEventCallback( this, &CServerPortExtender::OnRSClientDataRecieved ) );
                   
    }
    else
    if ( CTCPServerSocket::ClientDisconnectedEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ClientErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketOpenedEvent == eventid )
    {
        GUCEF_LOG( 0, "ServerPortExtender: Reversed server socket opened,.. waiting for ServerPortExtenderClient" );
    }
    else
    if ( CTCPServerSocket::ServerSocketClosedEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketClientErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent == eventid )
    {
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnServerSocketNotify( CNotifier* notifier    ,
                                           const CEvent& eventid  ,
                                           CICloneable* eventdata )
{GUCEF_TRACE;

    // This is where clients connect that would normally connect to the remote
    // server port.
    
    if ( CTCPServerSocket::ClientConnectedEvent == eventid )
    {
        CTCPServerSocket::TClientConnectedEventData* eData = static_cast< CTCPServerSocket::TClientConnectedEventData* >( eventdata );
        CTCPServerSocket::TConnectionInfo& connectionInfo = eData->GetData();
        
        Subscribe( connectionInfo.connection                                             ,
                   CTCPServerConnection::ConnectedEvent                                  ,
                   &TEventCallback( this, &CServerPortExtender::OnClientConnectedEvent ) );
        Subscribe( connectionInfo.connection                                                ,
                   CTCPServerConnection::ConnectedEvent                                     ,
                   &TEventCallback( this, &CServerPortExtender::OnClientDisconnectedEvent ) );
        Subscribe( connectionInfo.connection                                       ,
                   CTCPServerConnection::ConnectedEvent                            ,
                   &TEventCallback( this, &CServerPortExtender::OnClientDataSent ) );
        Subscribe( connectionInfo.connection                                          ,
                   CTCPServerConnection::ConnectedEvent                               ,
                   &TEventCallback( this, &CServerPortExtender::OnClientSocketError ) );
        Subscribe( connectionInfo.connection                                           ,
                   CTCPServerConnection::ConnectedEvent                                ,
                   &TEventCallback( this, &CServerPortExtender::OnClientDataRecieved ) );
    }
    else
    if ( CTCPServerSocket::ClientDisconnectedEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ClientErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketOpenedEvent == eventid )
    {
        GUCEF_LOG( 0, "ServerPortExtender: Server socket opened,.. listning for clients" );
    }
    else
    if ( CTCPServerSocket::ServerSocketClosedEvent == eventid )
    {
        GUCEF_LOG( 0, "ServerPortExtender: Server socket closed" );
    }
    else
    if ( CTCPServerSocket::ServerSocketErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketClientErrorEvent == eventid )
    {
    }
    else
    if ( CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent == eventid )
    {
    }
}

/*-------------------------------------------------------------------------*/