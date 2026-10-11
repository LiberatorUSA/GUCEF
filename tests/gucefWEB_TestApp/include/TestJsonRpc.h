/*
 *  gucefWEB_TestApp: GUCEF test application for the WEB module
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

#ifndef GUCEF_WEB_TESTAPP_TESTJSONRPC_H
#define GUCEF_WEB_TESTAPP_TESTJSONRPC_H

/**
 *  Tests the JSON-RPC endpoint, client and server:
 *  - the examples of the JSON-RPC 2.0 specification, fed to an endpoint through a capturing transport
 *  - calls between two endpoints over an in-memory transport: ids, errors, timeouts, deferred answers, batches
 *  - the lenient dialect and extra envelope fields as used by the Chrome DevTools Protocol
 *  - a client and server over real web sockets
 */
void
PerformJsonRpcTests( void );

#endif /* GUCEF_WEB_TESTAPP_TESTJSONRPC_H ? */
