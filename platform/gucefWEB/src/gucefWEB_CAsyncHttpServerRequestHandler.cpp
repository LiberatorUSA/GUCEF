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

#ifndef GUCEF_COM_CASYNCHTTPSERVERRESPONSEHANDLER_H
#include "gucefCOM_CAsyncHttpServerResponseHandler.h"
#define GUCEF_COM_CASYNCHTTPSERVERRESPONSEHANDLER_H
#endif /* GUCEF_COM_CASYNCHTTPSERVERRESPONSEHANDLER_H */

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

CAsyncHttpRequestData::CAsyncHttpRequestData( void )
    : CHttpRequestData()
    , httpServer( GUCEF_NULL )
    , clientConnectionId( 0 )
    , remoteClientAddress()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncHttpRequestData::CAsyncHttpRequestData( const CAsyncHttpRequestData& src )
    : CHttpRequestData( src )
    , httpServer( src.httpServer )
    , clientConnectionId( src.clientConnectionId )
    , remoteClientAddress( src.remoteClientAddress )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncHttpRequestData::~CAsyncHttpRequestData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CAsyncHttpRequestData::Clone( void ) const 
{GUCEF_TRACE;

    return new CAsyncHttpRequestData( *this );
}

/*-------------------------------------------------------------------------*/

void 
CAsyncHttpServerRequestHandler::RegisterEvents( void )
{GUCEF_TRACE;

    AsyncHttpServerRequestHandlingCompletedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CAsyncHttpServerRequestHandler::CAsyncHttpServerRequestHandler( void )
    : CORE::CTaskConsumer() 
    , m_requestHandler( GUCEF_NULL )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CAsyncHttpServerRequestHandler::CAsyncHttpServerRequestHandler( const CAsyncHttpServerRequestHandler& src )
    : CORE::CTaskConsumer()
    , m_requestHandler( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncHttpServerRequestHandler::~CAsyncHttpServerRequestHandler()
{GUCEF_TRACE;

    delete m_requestHandler;
    m_requestHandler = GUCEF_NULL;
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

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler(" + CORE::PointerToString( this ) + "):OnTaskStart" );

    CAsyncHttpRequestData* httpRequestData = static_cast< CAsyncHttpRequestData* >( taskData );
    if ( GUCEF_NULL != httpRequestData )
    {
        m_requestHandler = httpRequestData->httpServer->GetRequestHandlerFactory()->Create();
        if ( GUCEF_NULL == m_requestHandler )
            return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAsyncHttpServerRequestHandler::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    time_t startTime = time( NULL );
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler(" + CORE::PointerToString( this ) + "):OnTaskCycle" );

    CAsyncHttpRequestData* httpRequestData = static_cast< CAsyncHttpRequestData* >( taskData );
    if ( GUCEF_NULL == httpRequestData )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler(" + CORE::PointerToString( this ) + "):OnTaskCycle: No task data (http request) to operate upon" );
        return true;
    }

    if ( GUCEF_NULL == m_requestHandler )
    {
        m_requestHandler = httpRequestData->httpServer->GetRequestHandlerFactory()->Create();
        if ( GUCEF_NULL == m_requestHandler )
            return true;
    }

    CAsyncHttpResponseData* response = new CAsyncHttpResponseData( httpRequestData );
    m_requestHandler->OnRequest( *httpRequestData, *response );
    
    if ( !CORE::CCoreGlobal::Instance()->GetTaskManager().QueueTask( CAsyncHttpServerResponseHandler::TaskType, 
                                                                     response, 
                                                                     GUCEF_NULL, 
                                                                     GUCEF_NULL, 
                                                                     true ) )
    {
        // Failed to queue task to send the response to the client
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler(" + CORE::PointerToString( this ) + "):OnTaskCycle: Failed to QueueTask to disptach result to client after request processing" );

        // Use this thread instead as a fallback
        CORE::CDynamicBuffer fullResponse;
        if ( response->Serialize( fullResponse ) )
        {
            httpRequestData->httpServer->SendResponseASync( response->clientConnectionId  ,
                                                            fullResponse                  ,
                                                            response->remoteClientAddress );
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
CAsyncHttpServerRequestHandler::OnTaskEnding( CORE::CICloneable* taskData ,
                                              bool willBeForced           )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CAsyncHttpServerRequestHandler::OnTaskEnded( CORE::CICloneable* taskData ,
                                             bool wasForced              )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncHttpServerRequestHandler(" + CORE::PointerToString( this ) + "):OnTaskEnd" );

    CAsyncHttpRequestData* httpRequestData = static_cast< CAsyncHttpRequestData* >( taskData );
    if ( GUCEF_NULL != httpRequestData )
    {
         httpRequestData->httpServer->GetRequestHandlerFactory()->Destroy( m_requestHandler );
         m_requestHandler = GUCEF_NULL;
    }

    CORE::CTaskConsumer::OnTaskEnded( taskData, wasForced );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
