/*
 *  gucefCOM: GUCEF module providing communication implementations 
 *  for standardized protocols
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

#include <string.h>
#include <time.h> 

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_COM_CHTTPSERVER_H
#include "gucefCOM_CHTTPServer.h"
#define GUCEF_COM_CHTTPSERVER_H
#endif /* GUCEF_COM_CHTTPSERVER_H */

#include "gucefCOM_CAsyncHttpServerRequestHandler.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CAsyncHttpServerRequestHandler::TaskType = "AsyncHttpServerRequestHandler";
const CORE::CEvent CAsyncHttpServerRequestHandler::AsyncHttpServerRequestHandlingCompletedEvent = "GUCEF::COM::CAsyncHttpServerRequestHandler::AsyncHttpServerRequestHandlingCompletedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void 
CAsyncHttpServerRequestHandler::RegisterEvents( void )
{GUCEF_TRACE;

    AsyncHttpServerRequestHandlingCompletedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CAsyncHttpServerRequestHandler::CAsyncHttpServerRequestHandler()
    : CORE::CTaskConsumer()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CAsyncHttpServerRequestHandler::CAsyncHttpServerRequestHandler( const CAsyncHttpServerRequestHandler& src )
    : CORE::CTaskConsumer()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncHttpServerRequestHandler::~CAsyncHttpServerRequestHandler()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CString
CAsyncHttpServerRequestHandler::GetType( void ) const
{GUCEF_TRACE;

    return TaskType;
}

/*-------------------------------------------------------------------------*/

bool
CAsyncHttpServerRequestHandler::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    // This is a simplistic task, no bootstrap is required.
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler(" + CORE::PointerToString( this ) + "):OnTaskStart" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAsyncHttpServerRequestHandler::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    time_t startTime = time( NULL );
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler(" + CORE::PointerToString( this ) + "):OnTaskCycle" );

    CHttpRequestData* httpRequestData = static_cast< CHttpRequestData* >( taskData );
    if ( GUCEF_NULL == httpRequestData )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler:OnTaskCycle: No task data (http request) to operate upon" );
        return true;
    }


    return true;
}

/*-------------------------------------------------------------------------*/

void
CAsyncHttpServerRequestHandler::OnTaskEnd( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    // This is a simplistic task, no shutdown or cleanup is required.
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler(" + CORE::PointerToString( this ) + "):OnTaskEnd" );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
