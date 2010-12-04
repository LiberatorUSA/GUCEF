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
CLogSvcServer::OnClientConnectionDataReceived( CORE::CNotifier* notifier    ,
                                               const CORE::CEvent& eventid  ,
                                               CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // Get access to the event data
    COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventdata );
    const CORE::CDynamicBuffer& buffer = eData->GetData();
    
    

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
