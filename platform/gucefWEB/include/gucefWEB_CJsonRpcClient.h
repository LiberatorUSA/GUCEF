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

#ifndef GUCEF_WEB_CJSONRPCCLIENT_H
#define GUCEF_WEB_CJSONRPCCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_WEB_CJSONRPCENDPOINT_H
#include "gucefWEB_CJsonRpcEndpoint.h"
#define GUCEF_WEB_CJSONRPCENDPOINT_H
#endif /* GUCEF_WEB_CJSONRPCENDPOINT_H ? */

#ifndef GUCEF_WEB_CWEBSOCKETCLIENT_H
#include "gucefWEB_CWebSocketClient.h"
#define GUCEF_WEB_CWEBSOCKETCLIENT_H
#endif /* GUCEF_WEB_CWEBSOCKETCLIENT_H ? */

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

/**
 *  The calling side of a JSON-RPC conversation: an endpoint which times out calls on its own and can
 *  be attached to a web socket client as its transport. Any other transport can be used through SetTransport()
 *  in which case the owner of that transport gives incoming messages to ProcessIncoming().
 */
class GUCEF_WEB_PUBLIC_CPP CJsonRpcClient : public CJsonRpcEndpoint
{
    public:

    CJsonRpcClient( void );

    virtual ~CJsonRpcClient();

    /**
     *  Uses the given web socket client, which must outlive this object or be detached, as the transport:
     *  messages are sent as text messages and the text messages received are processed. When the web socket
     *  closes the calls still waiting for an answer are failed.
     */
    void AttachWebSocketClient( CWebSocketClient& webSocketClient );

    void DetachWebSocketClient( void );

    private:

    CJsonRpcClient( const CJsonRpcClient& src );
    CJsonRpcClient& operator=( const CJsonRpcClient& src );

    class CWebSocketTransport : public CIJsonRpcTransport
    {
        public:
        CWebSocketTransport( void ) : m_webSocketClient( GUCEF_NULL ) {}
        virtual bool SendMessageText( const CORE::CString& messageText ) GUCEF_VIRTUAL_OVERRIDE
            { return GUCEF_NULL != m_webSocketClient && m_webSocketClient->SendText( messageText ); }
        CWebSocketClient* m_webSocketClient;
    };

    class CEventSink : public CORE::CObserver
    {
        public:
        explicit CEventSink( CJsonRpcClient& client ) : CORE::CObserver(), m_client( client ) {}
        virtual void OnNotify( CORE::CNotifier* notifier                 ,
                               const CORE::CEvent& eventid               ,
                               CORE::CICloneable* eventdata /* = NULL */ ) GUCEF_VIRTUAL_OVERRIDE;
        private:
        CJsonRpcClient& m_client;
    };

    friend class CEventSink;

    private:

    CORE::CTimer m_timeoutTimer;
    CWebSocketTransport m_webSocketTransport;
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

#endif /* GUCEF_WEB_CJSONRPCCLIENT_H ? */
