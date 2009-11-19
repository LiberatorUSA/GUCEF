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
    : CORE::CObserver()                      ,
      m_controlClient( false )               ,
      m_rsClientConnections()                ,
      m_localClientConnections()             ,
      m_remoteToLocalConnectionMap()         ,
      m_localToRemoteConnectionMap()         ,
      m_localServer()                        ,
      m_remoteSPEServer()                    ,
      m_remoteSPEServerPort( 10235 )         ,
      m_controlConnectionInitialized( false )
{GUCEF_TRACE;

    m_remoteSPEServer.SetPortInHostByteOrder( 10236 );
    m_localServer.SetPortInHostByteOrder( 10235 );
    
    SubscribeTo( &m_controlClient );
}

/*-------------------------------------------------------------------------*/

bool
CServerPortExtenderClient::ConnectToSPEControlSocket( const COMCORE::CHostAddress& host )
{GUCEF_TRACE;

    m_controlConnectionInitialized = false;
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
CServerPortExtenderClient::RemoveConnectionUsingLocalClient( COMCORE::CTCPClientSocket* localSocket )
{GUCEF_TRACE;

    COMCORE::CTCPClientSocket* speClientSocket = m_localToRemoteConnectionMap[ localSocket->GetSocketID() ];
    m_localToRemoteConnectionMap.erase( localSocket->GetSocketID() );
    m_remoteToLocalConnectionMap.erase( speClientSocket->GetSocketID() );
    
    speClientSocket->Close();
    localSocket->Close();
    
    m_localClientConnections.erase( localSocket );
    m_rsClientConnections.erase( speClientSocket );
}

/*-------------------------------------------------------------------------*/
    
void
CServerPortExtenderClient::RemoveConnectionUsingSPEClient( COMCORE::CTCPClientSocket* speClientSocket )
{GUCEF_TRACE;

    COMCORE::CTCPClientSocket* localSocket = m_remoteToLocalConnectionMap[ speClientSocket->GetSocketID() ];
    m_remoteToLocalConnectionMap.erase( speClientSocket->GetSocketID() );
    m_localToRemoteConnectionMap.erase( localSocket->GetSocketID() );
    
    localSocket->Close();    
    speClientSocket->Close();
    
    m_localClientConnections.erase( localSocket );
    m_rsClientConnections.erase( speClientSocket );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::OnClientToActualServerConnected( CORE::CNotifier* notifier    ,
                                                            const CORE::CEvent& eventid  ,
                                                            CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "A connection to the actual server has been established" );
}

/*-------------------------------------------------------------------------*/
                                      
void
CServerPortExtenderClient::OnClientToActualServerDisconnected( CORE::CNotifier* notifier    ,
                                                               const CORE::CEvent& eventid  ,
                                                               CORE::CICloneable* eventdata )
{GUCEF_TRACE;
    
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "A connection to the actual server has been disconnected" );
    
    // make sure we dismantle our connection links
    RemoveConnectionUsingLocalClient( static_cast< COMCORE::CTCPClientSocket* >( notifier ) );
}

/*-------------------------------------------------------------------------*/
                                               
void
CServerPortExtenderClient::OnClientToActualServerDataRecieved( CORE::CNotifier* notifier    ,
                                                               const CORE::CEvent& eventid  ,
                                                               CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Received data from the actual server, forwarding it" );
    
    // received data from the actual server on this client connection.
    // we have to forward it to remote SPE server
    COMCORE::CTCPClientSocket* localClientConnection = static_cast< COMCORE::CTCPClientSocket* >( notifier );
    COMCORE::CTCPClientSocket* remoteClientConnection = GetRemoteConnectionForLocalConnection( localClientConnection->GetSocketID() );
    
    if ( NULL != remoteClientConnection )
    {
        // get the received data
        COMCORE::CTCPClientSocket::TDataRecievedEventData* receivedDataObj = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventdata );
        const CORE::CDynamicBuffer& receivedData = receivedDataObj->GetData();
        
        // write the received data into our data buffer for this socket
        CORE::CCyclicDynamicBuffer& buffer = m_rsClientConnections[ remoteClientConnection ];
        buffer.Write( receivedData.GetConstBufferPtr(), receivedData.GetDataSize(), 1 );
        
        // send all data from the data buffer
        SendAllDataInBuffer( buffer, *remoteClientConnection );
    }
}

/*-------------------------------------------------------------------------*/
                                               
void
CServerPortExtenderClient::OnClientToActualServerDataSent( CORE::CNotifier* notifier    ,
                                                           const CORE::CEvent& eventid  ,
                                                           CORE::CICloneable* eventdata )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
                                           
void
CServerPortExtenderClient::OnClientToActualServerSocketError( CORE::CNotifier* notifier    ,
                                                              const CORE::CEvent& eventid  ,
                                                              CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::OnClientToRemoteSPEConnected( CORE::CNotifier* notifier    ,
                                                         const CORE::CEvent& eventid  ,
                                                         CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // We just managed to connect to the remote SPE server
    // Now we should connect to the real server to complete the connection
    
    // initiate a new connection to the local server
    // we create the client socket and add it to our list of connections
    COMCORE::CTCPClientSocket* clientSocket = new COMCORE::CTCPClientSocket( false );
    clientSocket->ConnectTo( m_localServer );
    
    // Subscribe to client events
    SubscribeTo( clientSocket                                                                         ,
                 COMCORE::CTCPClientSocket::ConnectedEvent                                            ,
                 &TEventCallback( this, &CServerPortExtenderClient::OnClientToActualServerConnected ) );
    SubscribeTo( clientSocket                                                                            ,
                 COMCORE::CTCPClientSocket::DisconnectedEvent                                            ,
                 &TEventCallback( this, &CServerPortExtenderClient::OnClientToActualServerDisconnected ) );
    SubscribeTo( clientSocket                                                                        ,
                 COMCORE::CTCPClientSocket::DataSentEvent                                            ,
                 &TEventCallback( this, &CServerPortExtenderClient::OnClientToActualServerDataSent ) );
    SubscribeTo( clientSocket                                                                           ,
                 COMCORE::CTCPClientSocket::SocketErrorEvent                                            ,
                 &TEventCallback( this, &CServerPortExtenderClient::OnClientToActualServerSocketError ) );
    SubscribeTo( clientSocket                                                                            ,
                 COMCORE::CTCPClientSocket::DataRecievedEvent                                            ,
                 &TEventCallback( this, &CServerPortExtenderClient::OnClientToActualServerDataRecieved ) );
    
    // add to list of connections and create buffer
    m_localClientConnections[ clientSocket ];    
    
    // map the local to remote client connections
    MapLocalToRemoteConnection( clientSocket, static_cast< COMCORE::CTCPClientSocket* >( notifier ) ); 
    
  //  GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Received request for a new connection to remote SPE server, created client and attempting to connect" );
}

/*-------------------------------------------------------------------------*/
                                      
void
CServerPortExtenderClient::OnClientToRemoteSPEDisconnected( CORE::CNotifier* notifier    ,
                                                            const CORE::CEvent& eventid  ,
                                                            CORE::CICloneable* eventdata )
{GUCEF_TRACE;
    
    // make sure we dismantle our connection links
    RemoveConnectionUsingSPEClient( static_cast< COMCORE::CTCPClientSocket* >( notifier ) );    
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::SendAllDataInBuffer( CORE::CCyclicDynamicBuffer& buffer ,
                                                COMCORE::CTCPClientSocket& socket  )
{GUCEF_TRACE;

    CORE::CDynamicBuffer dataBlock;
    while ( buffer.ReadBlockTo( dataBlock ) > 0 )
    {
        socket.Send( dataBlock.GetConstBufferPtr(), dataBlock.GetDataSize() );
    }
}

/*-------------------------------------------------------------------------*/
                                               
void
CServerPortExtenderClient::OnClientToRemoteSPEDataRecieved( CORE::CNotifier* notifier    ,
                                                            const CORE::CEvent& eventid  ,
                                                            CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // received data from the SPE server on this client connection.
    // we have to forward it to our local server
    COMCORE::CTCPClientSocket* remoteConnection = static_cast< COMCORE::CTCPClientSocket* >( notifier );
    COMCORE::CTCPClientSocket* localClientConnection = GetLocalConnectionForRemoteConnection( remoteConnection->GetSocketID() );
    
    if ( NULL != localClientConnection )
    {
        // get the received data
        COMCORE::CTCPClientSocket::TDataRecievedEventData* receivedDataObj = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventdata );
        const CORE::CDynamicBuffer& receivedData = receivedDataObj->GetData();
        
        // write the received data into our data buffer for this socket
        CORE::CCyclicDynamicBuffer& buffer = m_localClientConnections[ localClientConnection ];
        buffer.Write( receivedData.GetConstBufferPtr(), receivedData.GetDataSize(), 1 );
        
        // send all data from the data buffer
        SendAllDataInBuffer( buffer, *localClientConnection );
    }
}

/*-------------------------------------------------------------------------*/
                                               
void
CServerPortExtenderClient::OnClientToRemoteSPEDataSent( CORE::CNotifier* notifier    ,
                                                        const CORE::CEvent& eventid  ,
                                                        CORE::CICloneable* eventdata )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
                                           
void
CServerPortExtenderClient::OnClientToRemoteSPESocketError( CORE::CNotifier* notifier    ,
                                                           const CORE::CEvent& eventid  ,
                                                           CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::OnControlMsg( TServerPortExtenderProtocolEnum msgType ,
                                         const CORE::UInt8* data                 )
{GUCEF_TRACE;

    switch ( msgType )
    {
        case SPEP_INITIAL_CONTROL_SERVER_MSG :
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtenderClient: Received initial handshake on control connection" );
            m_controlConnectionInitialized = true;
            break;
        }
        case SPEP_CONTROL_SERVER_MSG_CREATE_CONNECTION_REQUEST : 
        {
            if ( m_controlConnectionInitialized )
            {
                // initiate a new connection to the SPE server
                // we create the client socket and add it to our list of connections
                COMCORE::CTCPClientSocket* clientSocket = new COMCORE::CTCPClientSocket( false );
                clientSocket->ConnectTo( m_remoteSPEServer );
                
                // Subscribe to client events
                SubscribeTo( clientSocket                                                                      ,
                             COMCORE::CTCPClientSocket::ConnectedEvent                                         ,
                             &TEventCallback( this, &CServerPortExtenderClient::OnClientToRemoteSPEConnected ) );
                SubscribeTo( clientSocket                                                                         ,
                             COMCORE::CTCPClientSocket::DisconnectedEvent                                         ,
                             &TEventCallback( this, &CServerPortExtenderClient::OnClientToRemoteSPEDisconnected ) );
                SubscribeTo( clientSocket                                                                     ,
                             COMCORE::CTCPClientSocket::DataSentEvent                                         ,
                             &TEventCallback( this, &CServerPortExtenderClient::OnClientToRemoteSPEDataSent ) );
                SubscribeTo( clientSocket                                                                        ,
                             COMCORE::CTCPClientSocket::SocketErrorEvent                                         ,
                             &TEventCallback( this, &CServerPortExtenderClient::OnClientToRemoteSPESocketError ) );
                SubscribeTo( clientSocket                                                                         ,
                             COMCORE::CTCPClientSocket::DataRecievedEvent                                         ,
                             &TEventCallback( this, &CServerPortExtenderClient::OnClientToRemoteSPEDataRecieved ) );
                
                m_rsClientConnections[ clientSocket ]; 
                
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Received request for a new connection to remote SPE server, created client and attempting to connect" );\
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtenderClient: Received request for new connection on an uninitialized control connection" );
            }
            break;
        }
        default:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtenderClient: Received control message of unknown type" );
        }
    }

}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::OnControlClientConnected( CORE::CNotifier* notifier    ,
                                                     const CORE::CEvent& eventid  ,
                                                     CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtenderClient: Control connection established, sending initial handshake,.." );

    COMCORE::CTCPClientSocket* clientSocket = static_cast< COMCORE::CTCPClientSocket* >( notifier );
    
    // Send welcome handshake with protocol version so the server can determine
    // whether it is compatible with this client
    
    char msg[ 3 ];
    msg[ 0 ] = (char) SPEP_INITIAL_CONTROL_CLIENT_MSG;
    msg[ 1 ] = 45; // magic number, additional sanity check
    msg[ 2 ]= SPEP_PROTOCOL_VERSION;
    clientSocket->Send( msg, 3 );
}

/*-------------------------------------------------------------------------*/

void
CServerPortExtenderClient::OnControlClientNotify( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventid  ,
                                                  CORE::CICloneable* eventdata )
{GUCEF_TRACE;
    
    if ( COMCORE::CTCPClientSocket::ConnectingEvent == eventid )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtenderClient: Connecting control connection,..." );
    }
    else
    if ( COMCORE::CTCPClientSocket::ConnectedEvent == eventid )
    {                
        OnControlClientConnected( notifier, eventid, eventdata );
    }
    else
    if ( COMCORE::CTCPClientSocket::DisconnectedEvent == eventid )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtenderClient: Control connection disconnected" );
    }
    else
    if ( COMCORE::CTCPClientSocket::DataRecievedEvent == eventid )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "ServerPortExtenderClient: Data received on the control connection" );
        
        // Get the data buffer
        COMCORE::CTCPClientSocket::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventdata );
        const CORE::CDynamicBuffer& data = eData->GetData();
        
        CORE::UInt32 dataSize = data.GetDataSize();
        if ( dataSize > 0 )
        {
            TServerPortExtenderProtocolEnum msgType = (TServerPortExtenderProtocolEnum) data.AsConstType< CORE::Int8 >( 0 );
            const CORE::UInt8* dataPtr = dataSize > 1 ? (const CORE::UInt8*) data.GetConstBufferPtr( 1 ) : NULL;
            OnControlMsg( msgType, dataPtr );
        }
    }
    else
    if ( COMCORE::CTCPClientSocket::DataSentEvent == eventid )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_EVERYTHING, "ServerPortExtenderClient: Data send on the control connection" );
    }
    else
    if ( COMCORE::CTCPClientSocket::SocketErrorEvent == eventid )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "ServerPortExtenderClient: Socket error on the control connection" );
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