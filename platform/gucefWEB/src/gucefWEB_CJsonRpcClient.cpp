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

#ifndef GUCEF_WEB_CJSONRPCCLIENT_H
#include "gucefWEB_CJsonRpcClient.h"
#define GUCEF_WEB_CJSONRPCCLIENT_H
#endif /* GUCEF_WEB_CJSONRPCCLIENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CJsonRpcClient::CEventSink::OnNotify( CORE::CNotifier* notifier                 ,
                                      const CORE::CEvent& eventid               ,
                                      CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( eventid == CORE::CTimer::TimerUpdateEvent )
    {
        m_client.ExpireTimedOutCalls();
    }
    else
    if ( eventid == CWebSocketClient::TextMessageEvent )
    {
        TWebSocketTextMessageEventData* eData = static_cast< TWebSocketTextMessageEventData* >( eventdata );
        m_client.ProcessIncoming( eData->GetData().text );
    }
    else
    if ( eventid == CWebSocketClient::ClosedEvent )
    {
        m_client.FailAllPendingCalls( JSONRPC_ERROR_TRANSPORT_CLOSED, "The connection was closed" );
    }
}

/*-------------------------------------------------------------------------*/

CJsonRpcClient::CJsonRpcClient( void )
    : CJsonRpcEndpoint()
    , m_timeoutTimer( 250 )
    , m_webSocketTransport()
    , m_eventSink( *this )
{GUCEF_TRACE;

    m_eventSink.SubscribeTo( &m_timeoutTimer );
    m_timeoutTimer.SetEnabled( true );
}

/*-------------------------------------------------------------------------*/

CJsonRpcClient::~CJsonRpcClient()
{GUCEF_TRACE;

    DetachWebSocketClient();
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcClient::AttachWebSocketClient( CWebSocketClient& webSocketClient )
{GUCEF_TRACE;

    DetachWebSocketClient();

    m_webSocketTransport.m_webSocketClient = &webSocketClient;
    SetTransport( &m_webSocketTransport );
    m_eventSink.SubscribeTo( &webSocketClient );
}

/*-------------------------------------------------------------------------*/

void
CJsonRpcClient::DetachWebSocketClient( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_webSocketTransport.m_webSocketClient )
    {
        m_eventSink.UnsubscribeFrom( *m_webSocketTransport.m_webSocketClient );
        m_webSocketTransport.m_webSocketClient = GUCEF_NULL;
        SetTransport( GUCEF_NULL );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
