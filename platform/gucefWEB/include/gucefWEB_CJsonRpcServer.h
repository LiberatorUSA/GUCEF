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

#ifndef GUCEF_WEB_CJSONRPCSERVER_H
#define GUCEF_WEB_CJSONRPCSERVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include <set>

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_WEB_CJSONRPCENDPOINT_H
#include "gucefWEB_CJsonRpcEndpoint.h"
#define GUCEF_WEB_CJSONRPCENDPOINT_H
#endif /* GUCEF_WEB_CJSONRPCENDPOINT_H ? */

#ifndef GUCEF_WEB_CWEBSOCKETSERVER_H
#include "gucefWEB_CWebSocketServer.h"
#define GUCEF_WEB_CWEBSOCKETSERVER_H
#endif /* GUCEF_WEB_CWEBSOCKETSERVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/** Event data of the JSON-RPC server: every event says which client it is about */
struct SJsonRpcServerNotification
{
    TWebSocketConnectionPtr connection;
    SJsonRpcNotification notification;
};

typedef CORE::CTCloneableObj< TWebSocketConnectionPtr >         TJsonRpcServerSessionEventData;
typedef CORE::CTCloneableObj< SJsonRpcServerNotification >      TJsonRpcServerNotificationEventData;

/**
 *  The answering side of a JSON-RPC conversation, serving the clients of a web socket server.
 *
 *  Every client gets its own CJsonRpcEndpoint, a session. Methods are registered by name, a call to a method
 *  nobody registered is answered with a method not found error. Because JSON-RPC is symmetric the server can also
 *  send notifications and make calls to its clients through the endpoint of their session.
 *
 *  Events:
 *    SessionOpenedEvent     TJsonRpcServerSessionEventData       a client connected
 *    SessionClosedEvent     TJsonRpcServerSessionEventData       a client is gone
 *    NotificationEvent      TJsonRpcServerNotificationEventData  a client sent a notification
 *
 *  Calls which are answered later, see JSONRPC_REQUEST_DEFERRED, are answered through the endpoint of the session.
 *  In a batch call the answers of deferred calls are sent individually instead of as part of the batch response.
 *  The server and the web socket server must be used from the same thread.
 */
class GUCEF_WEB_PUBLIC_CPP CJsonRpcServer : public CORE::CNotifier         ,
                                            public CIJsonRpcRequestHandler
{
    public:

    static const CORE::CEvent SessionOpenedEvent;
    static const CORE::CEvent SessionClosedEvent;
    static const CORE::CEvent NotificationEvent;

    static void RegisterEvents( void );

    public:

    /** Serves the clients of the given web socket server which must outlive this object */
    explicit CJsonRpcServer( CWebSocketServer& webSocketServer );

    virtual ~CJsonRpcServer();

    /** The handler must outlive the registration. Registering a name again replaces the handler */
    void RegisterMethod( const CORE::CString& method, CIJsonRpcRequestHandler* handler );

    void UnregisterMethod( const CORE::CString& method );

    void SetDialect( EJsonRpcDialect dialect );

    /** The session of a client, GUCEF_NULL if the connection is not one of ours or it is closed */
    CJsonRpcEndpoint* GetEndpoint( const TWebSocketConnectionPtr& connection );

    bool Notify( const TWebSocketConnectionPtr& connection, const CORE::CString& method, const CORE::CDataNode* params );

    /** Sends a notification to every client, returns the number of clients it was sent to */
    UInt32 BroadcastNotification( const CORE::CString& method, const CORE::CDataNode* params );

    UInt32 GetSessionCount( void ) const;

    private:

    CJsonRpcServer( const CJsonRpcServer& src );
    CJsonRpcServer& operator=( const CJsonRpcServer& src );

    virtual EJsonRpcRequestResult OnJsonRpcRequest( CJsonRpcEndpoint& endpoint     ,
                                                    const SJsonRpcRequest& request ,
                                                    SJsonRpcResponse& response     ) GUCEF_VIRTUAL_OVERRIDE;

    /** The state of one client */
    class CSession : public CIJsonRpcTransport
    {
        public:
        CSession( const TWebSocketConnectionPtr& connection, EJsonRpcDialect dialect, CIJsonRpcRequestHandler* handler );
        virtual bool SendMessageText( const CORE::CString& messageText ) GUCEF_VIRTUAL_OVERRIDE;
        TWebSocketConnectionPtr m_connection;
        CJsonRpcEndpoint m_endpoint;
    };

    class CEventSink : public CORE::CObserver
    {
        public:
        explicit CEventSink( CJsonRpcServer& server ) : CORE::CObserver(), m_server( server ) {}
        virtual void OnNotify( CORE::CNotifier* notifier                 ,
                               const CORE::CEvent& eventid               ,
                               CORE::CICloneable* eventdata /* = NULL */ ) GUCEF_VIRTUAL_OVERRIDE;
        private:
        CJsonRpcServer& m_server;
    };

    // by value: the shared pointer handed out in events is const, a copy gives access to the non-const connection
    void OnClientConnected( TWebSocketConnectionPtr connection );
    void OnClientDisconnected( TWebSocketConnectionPtr connection );
    void OnTextMessage( const TWebSocketConnectionPtr& connection, const CORE::CString& text );
    void OnSessionNotification( CJsonRpcEndpoint* endpoint, const SJsonRpcNotification& notification );

    private:

    CWebSocketServer& m_webSocketServer;
    EJsonRpcDialect m_dialect;
    std::map< CORE::CString, CIJsonRpcRequestHandler* > m_methods;
    std::set< CSession* > m_sessions;
    std::map< CJsonRpcEndpoint*, CSession* > m_sessionsByEndpoint;
    CORE::CTimer m_timeoutTimer;
    CEventSink m_eventSink;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CJSONRPCSERVER_H ? */
