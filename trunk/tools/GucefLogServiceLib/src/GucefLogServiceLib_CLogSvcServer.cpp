/*
 *  GucefLogServiceLib: Library containing the main logic for the Logging service
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_LOGSERVICELIB_CILOGSVCSERVERLOGGER_H
#include "GucefLogServiceLib_CILogSvcServerLogger.h"
#define GUCEF_LOGSERVICELIB_CILOGSVCSERVERLOGGER_H
#endif /* GUCEF_LOGSERVICELIB_CILOGSVCSERVERLOGGER_H ? */

#ifndef GUCEF_LOGSERVICELIB_PROTOCOL_H
#include "GucefLogServiceLib_protocol.h"
#define GUCEF_LOGSERVICELIB_PROTOCOL_H
#endif /* GUCEF_LOGSERVICELIB_PROTOCOL_H ? */

#include "GucefLogServiceLib_CLogSvcServer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace LOGSERVICELIB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

//                              1024 *  1KB = 1MB * 10 = 10MB
#define MAX_ACCEPTED_MSGLENGTH  10 * 1024 * 1024 

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CLogSvcServer::CLogSvcServer( void )
    : CORE::CObservingNotifier()                                                     ,
      m_tcpServer( CORE::CGUCEFApplication::Instance()->GetPulseGenerator(), false ) ,
      m_clientInfoMap()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CLogSvcServer::CLogSvcServer( CORE::CPulseGenerator& pulseGenerator )
    : CORE::CObservingNotifier()           ,
      m_tcpServer( pulseGenerator, false ) ,
      m_clientInfoMap()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CLogSvcServer::RegisterServerSocketEventHandlers( void )
{
    SubscribeTo( &m_tcpServer                                                           ,
                 COMCORE::CTCPServerSocket::ClientConnectedEvent                        ,
                 &TEventCallback( this, &CLogSvcServer::OnServerSocketClientConnected ) );
    SubscribeTo( &m_tcpServer                                                              ,
                 COMCORE::CTCPServerSocket::ClientDisconnectedEvent                        ,
                 &TEventCallback( this, &CLogSvcServer::OnServerSocketClientDisconnected ) );                 
    SubscribeTo( &m_tcpServer                                                       ,
                 COMCORE::CTCPServerSocket::ClientErrorEvent                        ,
                 &TEventCallback( this, &CLogSvcServer::OnServerSocketClientError ) );
    SubscribeTo( &m_tcpServer                                                              ,
                 COMCORE::CTCPServerSocket::ServerSocketClosedEvent                        ,
                 &TEventCallback( this, &CLogSvcServer::OnServerSocketServerSocketClosed ) );
    SubscribeTo( &m_tcpServer                                                             ,
                 COMCORE::CTCPServerSocket::ServerSocketErrorEvent                        ,
                 &TEventCallback( this, &CLogSvcServer::OnServerSocketServerSocketError ) );
    SubscribeTo( &m_tcpServer                                                                   ,
                 COMCORE::CTCPServerSocket::ServerSocketClientErrorEvent                        ,
                 &TEventCallback( this, &CLogSvcServer::OnServerSocketServerSocketClientError ) );
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServer::RegisterClientConnectionEventHandlers( COMCORE::CTCPServerConnection* connection )
{GUCEF_TRACE;

    SubscribeTo( connection                                                              ,
                 COMCORE::CTCPServerConnection::DataRecievedEvent                        ,
                 &TEventCallback( this, &CLogSvcServer::OnClientConnectionDataReceived ) );

    //static const CORE::CEvent ConnectedEvent;
    //static const CORE::CEvent DisconnectedEvent;
    //static const CORE::CEvent DataSentEvent;
    //static const CORE::CEvent SocketErrorEvent;
}

/*-------------------------------------------------------------------------*/

CLogSvcServer::~CLogSvcServer()
{GUCEF_TRACE;

    m_tcpServer.Close();
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CLogSvcServer::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::LOGSERVICELIB::CLogSvcServer";
    return classTypeName; 
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServer::ProcessReceivedMessage( TClientInfo& clientInfo                   ,
                                       COMCORE::CTCPServerConnection* connection ,
                                       const CORE::CDynamicBuffer& messageBuffer )
{GUCEF_TRACE;

    try
    {
        // First read the message type and handle accordingly
        CORE::UInt8 msgType = messageBuffer.AsConstType< CORE::UInt8 >( 0 );
        switch ( msgType )
        {
            case LOGSVCMSGTYPE_CLIENTINFO: 
            {
                // Parse the client info from the message
                CORE::UInt32 offset = 1;
     
                // Set log client version number
                CORE::UInt32 strLength = messageBuffer.AsConstType< CORE::UInt32 >( offset );
                offset += 4;
                clientInfo.logClientVersion.Set( messageBuffer.AsConstTypePtr< char >( offset, strLength ), strLength );
                offset += strLength;
                
                // Set the application name
                strLength = messageBuffer.AsConstType< CORE::UInt32 >( offset );
                offset += 4;
                clientInfo.appName.Set( messageBuffer.AsConstTypePtr< char >( offset, strLength ), strLength );
                offset += strLength;
                
                // Set the process name
                strLength = messageBuffer.AsConstType< CORE::UInt32 >( offset );
                offset += 4;
                clientInfo.processName.Set( messageBuffer.AsConstTypePtr< char >( offset, strLength ), strLength );
                offset += strLength;
                
                // Set the process Id
                strLength = messageBuffer.AsConstType< CORE::UInt32 >( offset );
                offset += 4;
                clientInfo.processId.Set( messageBuffer.AsConstTypePtr< char >( offset, strLength ), strLength );

                // If we got here without an exception then the we successfully received all info required to 
                // consider the connection as initialized.
                clientInfo.initialized = true;
                
                // Send notification to the client that the initialization was successfull
                CORE::UInt8 statusCode = LOGSVCMSGTYPE_INITIALIZED;
                connection->Send( &statusCode, 1 ); 
                
                break;
            }
            case LOGSVCMSGTYPE_FLUSHLOG:
            {
                // We are being asked to flush the log
                
                break;
            }
            case LOGSVCMSGTYPE_LOGMSG:
            {
            
                // This is what its all about, we have been given a log message.
                
                break;
            }
            default:
            {
                // We should not get here if the protocol is correctly adhered to and there is no
                // corruption in the stream. This is a fatal error for the connection.
                connection->Close();
            }
        }
    }
    catch ( CORE::CDynamicBuffer::EIllegalCast& )
    {
        // Something is wrong with this msg buffer, kill the connection
        connection->Close();
    }
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServer::ProcessReceivedData( TClientInfo& clientInfo                   ,
                                    COMCORE::CTCPServerConnection* connection )
{GUCEF_TRACE;
    
    // Cycle as long as we have buffered data and no errors occur
    bool noError = true;    
    while ( clientInfo.receiveBuffer.HasBufferedData() && noError )
    {    
        noError = false;
        
        // First read the message delimiter
        char msgDelimterHeader[ 5 ];
        if ( 1 == clientInfo.receiveBuffer.Read( msgDelimterHeader, 5, 1 ) )
        {
            if ( msgDelimterHeader[ 0 ] == LOGSVCMSGTYPE_DELIMITER )
            {
                // Get the total message length from the delimiter header
                CORE::UInt32 msgLength = *((CORE::UInt32*) msgDelimterHeader+1);
                
                // subtract what we already read
                msgLength -= 5; 
                
                // Perform a sanity check
                // Server side we limit the length of the message we will accept from the client
                if ( msgLength <= MAX_ACCEPTED_MSGLENGTH )
                {
                    // Make a buffer for this message and read it from the receive buffer
                    CORE::CDynamicBuffer messageBuffer( msgLength, false );
                    if ( 1 == clientInfo.receiveBuffer.Read( messageBuffer.GetBufferPtr(), msgLength, 1 ) )
                    {
                        // Set the size of usefull data in the buffer
                        messageBuffer.SetDataSize( msgLength );
                        
                        // Process this single message
                        ProcessReceivedMessage( clientInfo    , 
                                                connection    ,
                                                messageBuffer );
                        noError = true;
                    }
                    
                }            
            }
        }
    }
    
    if ( !noError )
    {
        // Something is wrong with this connection, kill it
        connection->Close();
    }
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServer::OnClientConnectionDataReceived( CORE::CNotifier* notifier    ,
                                               const CORE::CEvent& eventid  ,
                                               CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // Get access to the event data
    COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventdata );
    const CORE::CDynamicBuffer& buffer = eData->GetData();
    
    // Get access to the connection object & client info
    COMCORE::CTCPServerConnection* connection = static_cast< COMCORE::CTCPServerConnection* >( notifier );    
    TClientInfo& clientInfo = m_clientInfoMap[ connection->GetConnectionIndex() ];
    
    // Handle the tcp stream, take into account message concatonation
    // First we copy the data recieved into our recieve buffer where we 
    // can properly segment data because we retain whatever data we need
    if ( !( buffer.GetDataSize() == clientInfo.receiveBuffer.Write( buffer, 0 ) ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to write all data recieved into a receive buffer for processsing" );
    } 

    // Use the received data as a trigger to process whatever we can on this connection
    ProcessReceivedData( clientInfo ,
                         connection );
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServer::OnServerSocketClientConnected( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventid  ,
                                              CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // Get access to the event data
    COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata );
    COMCORE::CTCPServerSocket::TConnectionInfo& eventData = eData->GetData();            
    
    // Build an initialization structure
    TClientInfo initialClientInfo;
    initialClientInfo.appName = "Unknown";
    initialClientInfo.processId = "Unknown";
    initialClientInfo.processName = "Unknown";
    initialClientInfo.logClientVersion = "Unknown";
    initialClientInfo.initialized = false;
    initialClientInfo.connected = true;
    
    m_clientInfoMap[ eventData.connectionIndex ] = initialClientInfo;
    
    RegisterClientConnectionEventHandlers( eventData.connection );
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServer::OnServerSocketClientDisconnected( CORE::CNotifier* notifier    ,
                                                 const CORE::CEvent& eventid  ,
                                                 CORE::CICloneable* eventdata )
{GUCEF_TRACE;

        // Get access to the event data
        COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata );
        COMCORE::CTCPServerSocket::TConnectionInfo& eventData = eData->GetData();  
        
        TClientInfo& clientInfo = m_clientInfoMap[ eventData.connectionIndex ];
        clientInfo.connected = false;
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServer::OnServerSocketClientError( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventid  ,
                                          CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                                           
void
CLogSvcServer::OnServerSocketServerSocketClosed( CORE::CNotifier* notifier    ,
                                                 const CORE::CEvent& eventid  ,
                                                 CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                                           
void
CLogSvcServer::OnServerSocketServerSocketError( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventid  ,
                                                CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                                           
void
CLogSvcServer::OnServerSocketServerSocketClientError( CORE::CNotifier* notifier    ,
                                                      const CORE::CEvent& eventid  ,
                                                      CORE::CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace LOGSERVICELIB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
