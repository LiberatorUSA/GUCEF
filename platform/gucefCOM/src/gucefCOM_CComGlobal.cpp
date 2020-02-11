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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CMETRICSCLIENTMANAGER_H
#include "gucefCORE_CMetricsClientManager.h"
#define GUCEF_CORE_CMETRICSCLIENTMANAGER_H
#endif /* GUCEF_CORE_CMETRICSCLIENTMANAGER_H ? */

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

CComGlobal* CComGlobal::g_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CComGlobal::CComGlobal( void )
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

    CORE::CMetricsClientManager::CIMetricsSystemClientPtr statsDClient( new CStatsDClient() );
    CORE::CCoreGlobal::Instance()->GetMetricsClientManager().AddMetricsClient( statsDClient );
}

/*-------------------------------------------------------------------------*/

CComGlobal::~CComGlobal()
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOM Global systems shutting down" );
}

/*-------------------------------------------------------------------------*/

CComGlobal*
CComGlobal::Instance( void )
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        g_instance = new CComGlobal();
        g_instance->Initialize();
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
