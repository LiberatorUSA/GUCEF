/*
 *  gucefWEB: GUCEF module providing WEB application functionality
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#ifndef GUCEF_WEB_CJSONRPCSERVER_H
#include "gucefWEB_CJsonRpcServer.h"
#define GUCEF_WEB_CJSONRPCSERVER_H
#endif /* GUCEF_WEB_CJSONRPCSERVER_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CJsonRpcServer::SessionOpenedEvent = "GUCEF::WEB::CJsonRpcServer::SessionOpenedEvent";
const CORE::CEvent CJsonRpcServer::SessionClosedEvent = "GUCEF::WEB::CJsonRpcServer::SessionClosedEvent";
const CORE::CEvent CJsonRpcServer::NotificationEvent = "GUCEF::WEB::CJsonRpcServer::NotificationEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CJsonRpcServer::CSession::CSession( const TWebSocketConnectionPtr& connection, EJsonRpcDialect dialect, CIJsonRpcRequestHandler* handler )
    : CIJsonRpcTransport()
    , m_connection( connection )
    , m_endpoint( this )
{GUCEF_TRACE;

    m_endpoint.SetDialect( dialect );
    m_endpoint.SetRequestHandler( handler );
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcServer::CSession::SendMessageText( const CORE::CString& messageText )
{GUCEF_TRACE;

    return m_connection->SendText( messageText );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::CEventSink::OnNotify( CORE::CNotifier* notifier                 ,
                                      const CORE::CEvent& eventid               ,
                                      CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( eventid == CWebSocketServer::ClientConnectedEvent )
    {
        m_server.OnClientConnected( static_cast< TWebSocketServerConnectedEventData* >( eventdata )->GetData() );
    }
    else
    if ( eventid == CWebSocketServer::ClientDisconnectedEvent )
    {
        m_server.OnClientDisconnected( static_cast< TWebSocketServerClosedEventData* >( eventdata )->GetData().connection );
    }
    else
    if ( eventid == CWebSocketServer::TextMessageEvent )
    {
        SWebSocketServerTextMessage& message = static_cast< TWebSocketServerTextMessageEventData* >( eventdata )->GetData();
        m_server.OnTextMessage( message.connection, message.text );
    }
    else
    if ( eventid == CJsonRpcEndpoint::NotificationEvent )
    {
        m_server.OnSessionNotification( static_cast< CJsonRpcEndpoint* >( notifier ), static_cast< TJsonRpcNotificationEventData* >( eventdata )->GetData() );
    }
    else
    if ( eventid == CORE::CTimer::TimerUpdateEvent )
    {
        // one timer for all sessions
        std::set< CSession* > sessions = m_server.m_sessions;
        for ( std::set< CSession* >::iterator i = sessions.begin(); i != sessions.end(); ++i )
            (*i)->m_endpoint.ExpireTimedOutCalls();
    }
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::RegisterEvents( void )
{GUCEF_TRACE;

    SessionOpenedEvent.Initialize();
    SessionClosedEvent.Initialize();
    NotificationEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CJsonRpcServer::CJsonRpcServer( CWebSocketServer& webSocketServer )
    : CORE::CNotifier()
    , CIJsonRpcRequestHandler()
    , m_webSocketServer( webSocketServer )
    , m_dialect( JSONRPC_DIALECT_STRICT_2_0 )
    , m_methods()
    , m_sessions()
    , m_sessionsByEndpoint()
    , m_timeoutTimer( 250 )
    , m_eventSink( *this )
{GUCEF_TRACE;

    RegisterEvents();

    // all events: the sink picks out the ones it needs
    m_eventSink.SubscribeTo( &m_webSocketServer );
    m_eventSink.SubscribeTo( &m_timeoutTimer );
    m_timeoutTimer.SetEnabled( true );

    // connections that already exist when we are created get a session too
    CWebSocketServer::TConnectionVector connections;
    m_webSocketServer.GetConnections( connections );
    for ( size_t i=0; i<connections.size(); ++i )
        OnClientConnected( connections[ i ] );
}

/*-------------------------------------------------------------------------*/

CJsonRpcServer::~CJsonRpcServer()
{GUCEF_TRACE;

    m_eventSink.UnsubscribeFrom( m_webSocketServer );

    for ( std::set< CSession* >::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i )
    {
        (*i)->m_connection->SetUserData( GUCEF_NULL );
        GUCEF_DELETE (*i);
    }
    m_sessions.clear();
    m_sessionsByEndpoint.clear();
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::RegisterMethod( const CORE::CString& method, CIJsonRpcRequestHandler* handler )
{GUCEF_TRACE;

    if ( GUCEF_NULL == handler )
        m_methods.erase( method );
    else
        m_methods[ method ] = handler;
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::UnregisterMethod( const CORE::CString& method )
{GUCEF_TRACE;

    m_methods.erase( method );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::SetDialect( EJsonRpcDialect dialect )
{GUCEF_TRACE;

    m_dialect = dialect;
    for ( std::set< CSession* >::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i )
        (*i)->m_endpoint.SetDialect( dialect );
}

/*-------------------------------------------------------------------------*/

EJsonRpcRequestResult
CJsonRpcServer::OnJsonRpcRequest( CJsonRpcEndpoint& endpoint     ,
                                  const SJsonRpcRequest& request ,
                                  SJsonRpcResponse& response     )
{GUCEF_TRACE;

    std::map< CORE::CString, CIJsonRpcRequestHandler* >::iterator i = m_methods.find( request.method );
    if ( i == m_methods.end() )
    {
        response.isError = true;
        response.error.code = JSONRPC_ERROR_METHOD_NOT_FOUND;
        response.error.message = "Method not found";
        return JSONRPC_REQUEST_HANDLED;
    }
    return (*i).second->OnJsonRpcRequest( endpoint, request, response );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::OnClientConnected( TWebSocketConnectionPtr connection )
{GUCEF_TRACE;

    CSession* session = GUCEF_NEW CSession( connection, m_dialect, this );
    connection->SetUserData( session );
    m_sessions.insert( session );
    m_sessionsByEndpoint[ &session->m_endpoint ] = session;
    m_eventSink.SubscribeTo( &session->m_endpoint );

    TJsonRpcServerSessionEventData eData( connection );
    NotifyObservers( SessionOpenedEvent, &eData );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::OnClientDisconnected( TWebSocketConnectionPtr connection )
{GUCEF_TRACE;

    CSession* session = static_cast< CSession* >( connection->GetUserData() );
    if ( GUCEF_NULL == session )
        return;

    connection->SetUserData( GUCEF_NULL );
    m_sessions.erase( session );
    m_sessionsByEndpoint.erase( &session->m_endpoint );
    m_eventSink.UnsubscribeFrom( session->m_endpoint );

    // calls we made to this client will never be answered
    session->m_endpoint.FailAllPendingCalls( JSONRPC_ERROR_TRANSPORT_CLOSED, "The connection was closed" );
    GUCEF_DELETE session;

    TJsonRpcServerSessionEventData eData( connection );
    NotifyObservers( SessionClosedEvent, &eData );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::OnTextMessage( const TWebSocketConnectionPtr& connection, const CORE::CString& text )
{GUCEF_TRACE;

    // the session is linked on the connection, no lookup needed
    CSession* session = static_cast< CSession* >( connection->GetUserData() );
    if ( GUCEF_NULL != session )
        session->m_endpoint.ProcessIncoming( text );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcServer::OnSessionNotification( CJsonRpcEndpoint* endpoint, const SJsonRpcNotification& notification )
{GUCEF_TRACE;

    std::map< CJsonRpcEndpoint*, CSession* >::iterator i = m_sessionsByEndpoint.find( endpoint );
    if ( i == m_sessionsByEndpoint.end() )
        return;

    TJsonRpcServerNotificationEventData eData;
    eData.GetData().connection = (*i).second->m_connection;
    eData.GetData().notification = notification;
    NotifyObservers( NotificationEvent, &eData );
}

/*-------------------------------------------------------------------------*/

CJsonRpcEndpoint*
CJsonRpcServer::GetEndpoint( const TWebSocketConnectionPtr& connection )
{GUCEF_TRACE;

    if ( connection.IsNULL() )
        return GUCEF_NULL;
    CSession* session = static_cast< CSession* >( connection->GetUserData() );
    return GUCEF_NULL != session && m_sessions.find( session ) != m_sessions.end() ? &session->m_endpoint : GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
CJsonRpcServer::Notify( const TWebSocketConnectionPtr& connection, const CORE::CString& method, const CORE::CDataNode* params )
{GUCEF_TRACE;

    CJsonRpcEndpoint* endpoint = GetEndpoint( connection );
    return GUCEF_NULL != endpoint && endpoint->Notify( method, params );
}

/*-------------------------------------------------------------------------*/

UInt32
CJsonRpcServer::BroadcastNotification( const CORE::CString& method, const CORE::CDataNode* params )
{GUCEF_TRACE;

    UInt32 sentCount = 0;
    for ( std::set< CSession* >::iterator i = m_sessions.begin(); i != m_sessions.end(); ++i )
        if ( (*i)->m_endpoint.Notify( method, params ) )
            ++sentCount;
    return sentCount;
}

/*-------------------------------------------------------------------------*/

UInt32
CJsonRpcServer::GetSessionCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_sessions.size();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
