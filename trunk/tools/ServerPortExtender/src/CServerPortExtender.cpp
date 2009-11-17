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

#ifndef SERVERPORTEXTENDERPROTOCOL_H
#include "ServerPortExtenderProtocol.h"
#define SERVERPORTEXTENDERPROTOCOL_H
#endif /* SERVERPORTEXTENDERPROTOCOL_H ? */

#include "CServerPortExtender.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CServerPortExtender::ControlSocketOpenedEvent = "CServerPortExtender::ControlSocketOpenedEvent";
const CORE::CEvent CServerPortExtender::ReversedSocketOpenedEvent = "CServerPortExtender::ReversedSocketOpenedEvent";
const CORE::CEvent CServerPortExtender::ClientSocketOpenedEvent = "CServerPortExtender::ClientSocketOpenedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CServerPortExtender::RegisterEvents( void )
{GUCEF_TRACE;
    
    ControlSocketOpenedEvent.Initialize();
    ReversedSocketOpenedEvent.Initialize();
    ClientSocketOpenedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CServerPortExtender::CServerPortExtender( void )
    : CObservingNotifier()                   ,
      m_reversedServerControlSocket( false ) ,
      m_reversedServerSocket( false )        ,
      m_serverSocket( false )                ,
      m_controlConnection( NULL )            ,
      m_cConnectionBuffers()                 ,
      m_rsConnectionBuffers()                ,
      m_unmappedClientConnections()          ,
      m_serverPort( 10234 )                  ,
      m_reversedServerPort( 10235 )          ,
      m_reversedServerControlPort( 10236 )   ,
      m_remoteToLocalConnectionMap()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CServerPortExtender::ListenForClientsOnPort( CORE::UInt16 port )
{GUCEF_TRACE;

    m_serverPort = port;
    return m_serverSocket.ListenOnPort( m_serverPort );
}

/*-------------------------------------------------------------------------*/
    
bool
CServerPortExtender::ListenForReversedControlClientOnPort( CORE::UInt16 port )
{GUCEF_TRACE;

    m_reversedServerControlPort = port;
    return m_reversedServerControlSocket.ListenOnPort( m_reversedServerControlPort );
}

/*-------------------------------------------------------------------------*/
    
bool
CServerPortExtender::ListenForReversedClientsOnPort( CORE::UInt16 port )
{GUCEF_TRACE;

    m_reversedServerPort = port;
    return m_reversedServerSocket.ListenOnPort( m_reversedServerPort );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSControlClientConnected( CORE::CNotifier* notifier    ,
                                                 const CORE::CEvent& eventid  ,
                                                 CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    COMCORE::CTCPServerConnection* connection = static_cast< COMCORE::CTCPServerConnection* >( notifier );
    
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
CServerPortExtender::OnRSControlClientDisconnected( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventid  ,
                                                    CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    COMCORE::CTCPServerConnection* connection = static_cast< COMCORE::CTCPServerConnection* >( notifier );
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
CServerPortExtender::OnRSControlClientDataSent( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventid  ,
                                                CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSControlClientSocketError( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventid  ,
                                                   CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSControlClientDataRecieved( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventid  ,
                                                    CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtender: Reversed server control client send us data,... possible error: we did not expect data" );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientConnected( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventid  ,
                                          CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // We won't do anything with this incoming rs connection until we get our initial
    // message which will tell us how to setup the routing for this connection
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtender: Reversed server client connection established" );
    COMCORE::CTCPServerConnection* rsConnection = static_cast< COMCORE::CTCPServerConnection* >( notifier );
    MapRemoteConnectionToLocalConnection( rsConnection );
}

/*-------------------------------------------------------------------------*/

COMCORE::CTCPServerConnection*
CServerPortExtender::GetLocalConnectionForRemoteConnection( CORE::UInt32 rsSocketId )
{GUCEF_TRACE;

    TConnectionMap::iterator i = m_remoteToLocalConnectionMap.find( rsSocketId );
    if ( i != m_remoteToLocalConnectionMap.end() )
    {
        return m_serverSocket.GetConnectionBySocketId( (*i).second );
    }
    return NULL; 
}

/*-------------------------------------------------------------------------*/

COMCORE::CTCPServerConnection*
CServerPortExtender::GetRemoteConnectionForLocalConnection( CORE::UInt32 localSocketId )
{GUCEF_TRACE;

    TConnectionMap::iterator i = m_remoteToLocalConnectionMap.begin();
    while ( i != m_remoteToLocalConnectionMap.end() )
    {
        if ( (*i).second == localSocketId )
        {
            return m_reversedServerSocket.GetConnectionBySocketId( (*i).first );
        }
        ++i;
    }
    return NULL; 
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientDisconnected( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventid  ,
                                             CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // Since the remote link has been severed we should sever the local link since its
    // the same virtual socket connection    
    COMCORE::CTCPServerConnection* rsConnection = static_cast< COMCORE::CTCPServerConnection* >( notifier );
    COMCORE::CTCPServerConnection* lConnection = GetLocalConnectionForRemoteConnection( rsConnection->GetSocketID() );
    if ( NULL != lConnection )
    {
        // the local connection still exists,... close it
        lConnection->Close();
    }
    
    // Remove the buffer for this connection
    m_rsConnectionBuffers.erase( rsConnection );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientDataSent( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventid  ,
                                         CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientSocketError( CORE::CNotifier* notifier    ,
                                            const CORE::CEvent& eventid  ,
                                            CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CServerPortExtender::MapRemoteConnectionToLocalConnection( COMCORE::CTCPServerConnection* rsConnection )
{GUCEF_TRACE;
    
    if ( m_unmappedClientConnections.size() > 0 )
    {
        TConnectionSet::iterator i = m_unmappedClientConnections.begin();
        CORE::UInt32 localClientSocketId = (*i)->GetSocketID();
        m_remoteToLocalConnectionMap[ rsConnection->GetSocketID() ] = localClientSocketId;
        m_unmappedClientConnections.erase( i );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnRSClientDataRecieved( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventid  ,
                                             CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    COMCORE::CTCPServerConnection* rsConnection = static_cast< COMCORE::CTCPServerConnection* >( notifier );
    COMCORE::CTCPServerConnection* lConnection = GetLocalConnectionForRemoteConnection( rsConnection->GetSocketID() );
    if ( NULL != lConnection )
    {
        // write data to the cyclic buffer and process the data transfer to the local client connection        
        COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventdata );
        const CORE::CDynamicBuffer& data = eData->GetData();
        CORE::CCyclicDynamicBuffer& cyclicBuffer = m_rsConnectionBuffers[ rsConnection ];
        
        // First we write the data to the buffer to assure propper data ordering
        cyclicBuffer.Write( data.GetConstBufferPtr(), data.GetDataSize(), 1 );
        
        // now we process all data in the buffer
        CORE::CDynamicBuffer readBuffer;
        while ( 0 != cyclicBuffer.ReadBlockTo( readBuffer ) )
        {
            // Send the block
            lConnection->Send( readBuffer.GetConstBufferPtr(), readBuffer.GetDataSize() );
            readBuffer.SetDataSize( 0 );
        }
    }
    else
    {
        // since we cannot locate the local connection we will hook this connection up to an unmapped client connection 
        if ( !MapRemoteConnectionToLocalConnection( rsConnection ) )
        {
            // Unable to map this connection, it is useless
            rsConnection->Close();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientConnected( CORE::CNotifier* notifier    ,
                                        const CORE::CEvent& eventid  ,
                                        CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // If a client connects to our extended port we have to make sure 
    // it triggers the remote SPE client to establish a connection in the 
    // opposite direction to this SPE server and establish a connection to 
    // the actual remote server port
    
    COMCORE::CTCPServerConnection* clientConnection = static_cast< COMCORE::CTCPServerConnection* >( notifier );
    
    // Initial sanity check
    if ( NULL == m_controlConnection )
    {
        // Nothing we can do with this connection
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtender: Client connection established without a control connection, unable to proceed,.. disconnecting client" );
        clientConnection->Close();
        return;
    }

    // Send the request for a new connection to be established ASAP  
    char msg[ 1 ];
    msg[ 0 ] = (char) SPEP_CONTROL_SERVER_MSG_CREATE_CONNECTION_REQUEST;
    m_controlConnection->Send( msg, 1 );
    
    // Reserve a buffer for this connection.
    // We need to buffer the different end of the connection since data might 
    // already be send before we complete the loop
    m_cConnectionBuffers[ clientConnection ].Clear( true );
    
    // Put this connection in this set of unmapped connections
    m_unmappedClientConnections.insert( clientConnection );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientDisconnected( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventid  ,
                                           CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // If a local client disconnects we have to disconnect the other half of the
    // virtual connection if it is still active, meaning the connection
    // from this server to the remote client
    
    COMCORE::CTCPServerConnection* clientConnection = static_cast< COMCORE::CTCPServerConnection* >( notifier );    
    COMCORE::CTCPServerConnection* rsConnection = GetRemoteConnectionForLocalConnection( clientConnection->GetSocketID() );
    if ( NULL != rsConnection )
    {
        rsConnection->Close();
    }
    
    // Remove the buffer for this connection
    m_cConnectionBuffers.erase( clientConnection );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientDataSent( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventid  ,
                                       CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientSocketError( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventid  ,
                                          CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnClientDataRecieved( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventid  ,
                                           CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    COMCORE::CTCPServerConnection* lConnection = static_cast< COMCORE::CTCPServerConnection* >( notifier );
    COMCORE::CTCPServerConnection* rsConnection = GetRemoteConnectionForLocalConnection( lConnection->GetSocketID() );
    
    // write data to the cyclic buffer and process the data transfer to the local client connection        
    COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventdata );
    const CORE::CDynamicBuffer& data = eData->GetData();
    CORE::CCyclicDynamicBuffer& cyclicBuffer = m_cConnectionBuffers[ lConnection ];
    
    // First we write the data to the buffer to assure propper data ordering
    cyclicBuffer.Write( data.GetConstBufferPtr(), data.GetDataSize(), 1 );
    
    // Check to see if we have the connection to the remote SPE client established
    // If so we can proceed with sending all the data
    if ( NULL != rsConnection )
    {
        // now we process all data in the buffer
        CORE::CDynamicBuffer readBuffer;
        while ( 0 != cyclicBuffer.ReadBlockTo( readBuffer ) )
        {
            // Send the block
            rsConnection->Send( readBuffer.GetConstBufferPtr(), readBuffer.GetDataSize() );
            readBuffer.SetDataSize( 0 );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnReversedServerControlSocketNotify( CORE::CNotifier* notifier    ,
                                                          const CORE::CEvent& eventid  ,
                                                          CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // this is where the remote connection comes in that will carry all the control
    // commands to operate the reversed client-server tunnel

    if ( COMCORE::CTCPServerSocket::ClientConnectedEvent == eventid )
    {
        // Now that the control connection is established we can listen for clients on our extended server port
        if ( !m_serverSocket.IsActive() )
        {
            m_serverSocket.ListenOnPort( m_serverPort );
        }
        
        // Get the connection object
        COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata );
        COMCORE::CTCPServerSocket::TConnectionInfo& connectionInfo = eData->GetData();

        // Subscribe to connection events        
        SubscribeTo( connectionInfo.connection                                                 ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                             ,
                     &TEventCallback( this, &CServerPortExtender::OnRSControlClientConnected ) );
        SubscribeTo( connectionInfo.connection                                                    ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                                ,
                     &TEventCallback( this, &CServerPortExtender::OnRSControlClientDisconnected ) );
        SubscribeTo( connectionInfo.connection                                                ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                            ,
                     &TEventCallback( this, &CServerPortExtender::OnRSControlClientDataSent ) );
        SubscribeTo( connectionInfo.connection                                                   ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                               ,
                     &TEventCallback( this, &CServerPortExtender::OnRSControlClientSocketError ) );
        SubscribeTo( connectionInfo.connection                                                    ,
                    COMCORE::CTCPServerConnection::ConnectedEvent                                ,
                    &TEventCallback( this, &CServerPortExtender::OnRSControlClientDataRecieved ) );
                   
    }
    else
    if ( COMCORE::CTCPServerSocket::ClientDisconnectedEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ClientErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketOpenedEvent == eventid )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtender: Reversed server socket opened,.. waiting for ServerPortExtenderClient" );
        
        NotifyObservers( ControlSocketOpenedEvent );
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketClosedEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketClientErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent == eventid )
    {
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnReversedServerSocketNotify( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventid  ,
                                                   CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // this is where the remote connection comes in to establish the reversed
    // client-server connnection. This is the connection we have to keep alive to
    // allow reversed client-server tunneling

    if ( COMCORE::CTCPServerSocket::ClientConnectedEvent == eventid )
    {
        if ( !m_serverSocket.IsActive() )
        {
            m_serverSocket.ListenOnPort( m_serverPort );
        }
        
        COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata );
        COMCORE::CTCPServerSocket::TConnectionInfo& connectionInfo = eData->GetData();
        
        SubscribeTo( connectionInfo.connection                                          ,
                    COMCORE::CTCPServerConnection::ConnectedEvent                      ,
                    &TEventCallback( this, &CServerPortExtender::OnRSClientConnected ) );
        SubscribeTo( connectionInfo.connection                                             ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                         ,
                     &TEventCallback( this, &CServerPortExtender::OnRSClientDisconnected ) );
        SubscribeTo( connectionInfo.connection                                         ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                     ,
                     &TEventCallback( this, &CServerPortExtender::OnRSClientDataSent ) );
        SubscribeTo( connectionInfo.connection                                            ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                        ,
                     &TEventCallback( this, &CServerPortExtender::OnRSClientSocketError ) );
        SubscribeTo( connectionInfo.connection                                             ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                         ,
                     &TEventCallback( this, &CServerPortExtender::OnRSClientDataRecieved ) );
                   
    }
    else
    if ( COMCORE::CTCPServerSocket::ClientDisconnectedEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ClientErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketOpenedEvent == eventid )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtender: Reversed server socket opened,.. waiting for ServerPortExtenderClient" );
        
        NotifyObservers( ReversedSocketOpenedEvent );
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketClosedEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketClientErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent == eventid )
    {
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnServerSocketNotify( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventid  ,
                                           CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // This is where clients connect that would normally connect to the remote
    // server port.
    
    if ( COMCORE::CTCPServerSocket::ClientConnectedEvent == eventid )
    {
        COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata );
        COMCORE::CTCPServerSocket::TConnectionInfo& connectionInfo = eData->GetData();
        
        SubscribeTo( connectionInfo.connection                                        ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                    ,
                     &TEventCallback( this, &CServerPortExtender::OnClientConnected ) );
        SubscribeTo( connectionInfo.connection                                           ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                       ,
                     &TEventCallback( this, &CServerPortExtender::OnClientDisconnected ) );
        SubscribeTo( connectionInfo.connection                                       ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                   ,
                     &TEventCallback( this, &CServerPortExtender::OnClientDataSent ) );
        SubscribeTo( connectionInfo.connection                                          ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                      ,
                     &TEventCallback( this, &CServerPortExtender::OnClientSocketError ) );
        SubscribeTo( connectionInfo.connection                                           ,
                     COMCORE::CTCPServerConnection::ConnectedEvent                       ,
                     &TEventCallback( this, &CServerPortExtender::OnClientDataRecieved ) );
    }
    else
    if ( COMCORE::CTCPServerSocket::ClientDisconnectedEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ClientErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketOpenedEvent == eventid )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtender: Server socket opened,.. listning for clients" );
        NotifyObservers( ClientSocketOpenedEvent );
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketClosedEvent == eventid )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtender: Server socket closed" );
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketClientErrorEvent == eventid )
    {
    }
    else
    if ( COMCORE::CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent == eventid )
    {
    }
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtender::OnNotify( CORE::CNotifier* notifier    ,
                               const CORE::CEvent& eventid  ,
                               CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    if ( notifier == &m_reversedServerControlSocket )
    {
        OnReversedServerControlSocketNotify( notifier  ,
                                             eventid   ,
                                             eventdata );
    }
    else
    if ( notifier == &m_reversedServerSocket )
    {
        OnReversedServerSocketNotify( notifier  ,
                                      eventid   ,
                                      eventdata );
    }
    else
    if ( notifier == &m_serverSocket )
    {
        OnServerSocketNotify( notifier  ,
                              eventid   ,
                              eventdata );
    }
}

/*-------------------------------------------------------------------------*/