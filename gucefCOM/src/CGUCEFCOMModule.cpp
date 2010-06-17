/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

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

#include "CGUCEFCOMModule.h"  /* definition of the class implemented here */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUCEFCOMModule::CGUCEFCOMModule( void )
{GUCEF_TRACE;
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMModule::CGUCEFCOMModule( const CGUCEFCOMModule& src )
{GUCEF_TRACE;
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMModule::~CGUCEFCOMModule()
{GUCEF_TRACE;
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMModule&
CGUCEFCOMModule::operator=( const CGUCEFCOMModule& src )
{GUCEF_TRACE;
        /* dummy, should never be used */
        return *this;
}
        
/*-------------------------------------------------------------------------*/        

bool 
CGUCEFCOMModule::Load( void )
{GUCEF_TRACE;
        
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOM Module loaded" );
        
        /* simply instantiate our com manager when the module is loaded */
        //CCom::Instance();
        
        CHTTPClient::RegisterEvents();
        CHTTPURLHandler::Register();
        CPHUDPSocket::RegisterEvents();
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFCOMModule::Unload( void )
{GUCEF_TRACE;
        
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefCOMCORE Module unloading" );
        
        //CCom::Deinstance();
        return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
