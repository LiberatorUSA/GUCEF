/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#include "CTCPConnection.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CTCPConnection::ConnectedEvent = "GUCEF::COMCORE::CTCPConnection::ConnectedEvent";
const CORE::CEvent CTCPConnection::DisconnectedEvent = "GUCEF::COMCORE::CTCPConnection::DisconnectedEvent";
const CORE::CEvent CTCPConnection::DataRecievedEvent = "GUCEF::COMCORE::CTCPConnection::DataRecievedEvent";
const CORE::CEvent CTCPConnection::DataSentEvent = "GUCEF::COMCORE::CTCPConnection::DataSentEvent";
const CORE::CEvent CTCPConnection::SocketErrorEvent = "GUCEF::COMCORE::CTCPConnection::SocketErrorEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTCPConnection::CTCPConnection( void )
    : CSocket()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CTCPConnection::~CTCPConnection()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CTCPConnection::RegisterEvents( void )
{GUCEF_TRACE;
    
    ConnectedEvent.Initialize();
    DisconnectedEvent.Initialize();
    DataRecievedEvent.Initialize();
    DataSentEvent.Initialize();
    SocketErrorEvent.Initialize();    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/