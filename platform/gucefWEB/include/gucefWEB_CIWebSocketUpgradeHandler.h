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

#ifndef GUCEF_WEB_CIWEBSOCKETUPGRADEHANDLER_H
#define GUCEF_WEB_CIWEBSOCKETUPGRADEHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CTCPSERVERCONNECTION_H
#include "CTCPServerConnection.h"
#define GUCEF_COMCORE_CTCPSERVERCONNECTION_H
#endif /* GUCEF_COMCORE_CTCPSERVERCONNECTION_H ? */

#ifndef GUCEF_WEB_CHTTPREQUESTDATA_H
#include "gucefWEB_CHttpRequestData.h"
#define GUCEF_WEB_CHTTPREQUESTDATA_H
#endif /* GUCEF_WEB_CHTTPREQUESTDATA_H ? */

#ifndef GUCEF_WEB_CHTTPRESPONSEDATA_H
#include "gucefWEB_CHttpResponseData.h"
#define GUCEF_WEB_CHTTPRESPONSEDATA_H
#endif /* GUCEF_WEB_CHTTPRESPONSEDATA_H ? */

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

class CHTTPServer;

/**
 *  Receives the data and the end of a connection which was upgraded to the web socket protocol.
 *  The handler of the upgrade links an implementation to the TCP connection through its user data slot:
 *  \code
 *  connection.SetUserData( static_cast< CIWebSocketConnectionHandler* >( myHandler ) );
 *  \endcode
 *  From then on the HTTP server hands everything that happens to the connection to it. There is no lookup
 *  involved in that: it is a pointer on the connection object itself.
 *  The callbacks are made from the thread which drives the HTTP server's socket.
 */
class GUCEF_WEB_PUBLIC_CPP CIWebSocketConnectionHandler
{
    public:

    CIWebSocketConnectionHandler( void ) {}

    virtual ~CIWebSocketConnectionHandler() {}

    /** Raw bytes received on the connection, not necessarily aligned to frame boundaries */
    virtual void OnWebSocketData( const void* data, UInt32 dataSize ) = 0;

    /** The connection ended, either side may have closed it. The link on the connection is removed afterwards */
    virtual void OnWebSocketClosed( void ) = 0;
};

/*-------------------------------------------------------------------------*/

/**
 *  Interface for a party that takes over HTTP connections which upgrade to the web socket protocol.
 *  The HTTP server performs the opening handshake: it validates the request headers and sends the 101 response.
 *  From then on the connection no longer carries HTTP and data for it goes to the connection's
 *  CIWebSocketConnectionHandler instead of the HTTP request parser.
 *  CWebSocketServer implements this interface.
 *
 *  Callbacks are made from the thread which drives the HTTP server's socket.
 */
class GUCEF_WEB_PUBLIC_CPP CIWebSocketUpgradeHandler
{
    public:

    CIWebSocketUpgradeHandler( void ) {}

    virtual ~CIWebSocketUpgradeHandler() {}

    /**
     *  Called after the HTTP server validated the upgrade request but before it accepts it.
     *  The handler can adjust the 101 response (for example to select a sub protocol) or refuse the upgrade
     *  by returning false in which case the response, which should then have a status code of 400 or above, is sent
     *  and the connection is closed. Origin checks and authentication belong here.
     */
    virtual bool OnWebSocketUpgradeRequested( CHTTPServer& server                 ,
                                              COMCORE::CTCPServerConnection& connection ,
                                              const CHttpRequestData& request    ,
                                              CHttpResponseData& response         ) = 0;

    /**
     *  The 101 response was sent, the connection now speaks the web socket protocol.
     *  The handler takes over the connection and must link a CIWebSocketConnectionHandler to it, see above.
     *  Note that data the client sent in the same network read as its upgrade request is not forwarded.
     */
    virtual void OnWebSocketConnectionEstablished( CHTTPServer& server                 ,
                                                   COMCORE::CTCPServerConnection& connection ,
                                                   const CHttpRequestData& request    ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CIWEBSOCKETUPGRADEHANDLER_H ? */
