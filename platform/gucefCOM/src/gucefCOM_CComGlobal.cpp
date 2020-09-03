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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CMETRICSCLIENTMANAGER_H
#include "gucefCORE_CMetricsClientManager.h"
#define GUCEF_CORE_CMETRICSCLIENTMANAGER_H
#endif /* GUCEF_CORE_CMETRICSCLIENTMANAGER_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_COMCORE_CCOMCOREGLOBAL_H
#include "gucefCOMCORE_CComCoreGlobal.h"
#define GUCEF_COMCORE_CCOMCOREGLOBAL_H
#endif /* GUCEF_COMCORE_CCOMCOREGLOBAL_H ? */

#ifndef GUCEF_COM_CHTTPCLIENT_H
#include "CHTTPClient.h"
#define GUCEF_COM_CHTTPCLIENT_H
#endif /* GUCEF_COM_CHTTPCLIENT_H ? */

#ifndef GUCEF_COM_CHTTPURLHANDLER_H
#include "CHTTPURLHandler.h"
#define GUCEF_COM_CHTTPURLHANDLER_H
#endif /* GUCEF_COM_CHTTPURLHANDLER_H ? */

#ifndef GUCEF_COM_CPHUDPSOCKET_H
#include "CPHUDPSocket.h"
#define GUCEF_COM_CPHUDPSOCKET_H
#endif /* GUCEF_COM_CPHUDPSOCKET_H ? */

#ifndef GUCEF_COM_CSTATSDCLIENT_H
#include "gucefCOM_CStatsDClient.h"
#define GUCEF_COM_CSTATSDCLIENT_H
#endif /* GUCEF_COM_CSTATSDCLIENT_H ? */

#ifndef GUCEF_COM_CASYNCHTTPSERVERREQUESTHANDLER_H
#include "gucefCOM_CAsyncHttpServerRequestHandler.h"
#define GUCEF_COM_CASYNCHTTPSERVERREQUESTHANDLER_H
#endif /* GUCEF_COM_CASYNCHTTPSERVERREQUESTHANDLER_H ? */

#ifndef GUCEF_COM_CASYNCHTTPSERVERRESPONSEHANDLER_H
#include "gucefCOM_CAsyncHttpServerResponseHandler.h"
#define GUCEF_COM_CASYNCHTTPSERVERRESPONSEHANDLER_H
#endif /* GUCEF_COM_CASYNCHTTPSERVERRESPONSEHANDLER_H ? */

#ifndef GUCEF_COM_CGLOBALHTTPCODECLINKS_H
#include "gucefCOM_CGlobalHttpCodecLinks.h"
#define GUCEF_COM_CGLOBALHTTPCODECLINKS_H
#endif /* GUCEF_COM_CGLOBALHTTPCODECLINKS_H ? */

#include "gucefCOM_CComGlobal.h"  /* definition of the class implemented here */

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

MT::CMutex CComGlobal::g_dataLock;
CComGlobal* CComGlobal::g_instance = GUCEF_NULL;
TAsyncHttpServerRequestHandlerFactory g_asyncHttpServerRequestHandlerFactory;
TAsyncHttpServerResponseHandlerFactory g_asyncHttpServerResponseHandlerFactory;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CComGlobal::CComGlobal( void )
    : m_globalHttpCodecLinks( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CComGlobal::Initialize( void )
{GUCEF_TRACE;

    // Initialize modules we depend on
    CORE::CCoreGlobal::Instance();
    COMCORE::CComCoreGlobal::Instance();

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOM Global systems initializing" );

    CHTTPClient::RegisterEvents();
    CHTTPURLHandler::Register();
    CPHUDPSocket::RegisterEvents();

    m_globalHttpCodecLinks = new CGlobalHttpCodecLinks();

    CORE::CMetricsClientManager::CIMetricsSystemClientPtr statsDClient( new CStatsDClient() );
    CORE::CCoreGlobal::Instance()->GetMetricsClientManager().AddMetricsClient( statsDClient );

    
    CORE::CCoreGlobal::Instance()->GetTaskManager().RegisterTaskConsumerFactory( CAsyncHttpServerRequestHandler::TaskType, &g_asyncHttpServerRequestHandlerFactory );
    CORE::CCoreGlobal::Instance()->GetTaskManager().RegisterTaskConsumerFactory( CAsyncHttpServerResponseHandler::TaskType, &g_asyncHttpServerResponseHandlerFactory );
}

/*-------------------------------------------------------------------------*/

CComGlobal::~CComGlobal()
{GUCEF_TRACE;
  
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOM Global systems shutting down" );

    CORE::CCoreGlobal::Instance()->GetTaskManager().UnregisterTaskConsumerFactory( CAsyncHttpServerRequestHandler::TaskType );
    CORE::CCoreGlobal::Instance()->GetTaskManager().UnregisterTaskConsumerFactory( CAsyncHttpServerResponseHandler::TaskType );

    delete m_globalHttpCodecLinks;
    m_globalHttpCodecLinks = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CComGlobal*
CComGlobal::Instance()
{GUCEF_TRACE;

    if ( GUCEF_NULL == g_instance )
    {
        g_dataLock.Lock();
        if ( NULL == g_instance )
        {
            g_instance = new CComGlobal();
            g_instance->Initialize();
        }
        g_dataLock.Unlock();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CComGlobal::Deinstance( void )
{GUCEF_TRACE;

    delete g_instance;
    g_instance = NULL;
}

/*-------------------------------------------------------------------------*/

CGlobalHttpCodecLinks& 
CComGlobal::GetGlobalHttpCodecLinks( void ) const
{GUCEF_TRACE;

    return *m_globalHttpCodecLinks;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
