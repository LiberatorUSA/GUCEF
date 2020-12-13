/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

/*-------------------------------------------------------------------------*/

const CString&
CTCPConnection::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::COMCORE::CTCPConnection";
    return typeName;
}

/*-------------------------------------------------------------------------*/

bool
CTCPConnection::SendString( const CString& data )
{GUCEF_TRACE;

    return Send( data.C_String() ,
                 data.Length()   );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/