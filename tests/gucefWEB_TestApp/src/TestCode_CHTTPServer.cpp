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

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_COMCORE_H
#include "gucefCOMCORE.h"
#define GUCEF_COMCORE_H
#endif /* GUCEF_COMCORE_H ? */

#ifndef GUCEF_WEB_H
#include "gucefWEB.h"
#define GUCEF_WEB_H
#endif /* GUCEF_WEB_H ? */

#include "TestCode_CHTTPServer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MSWIN_BUILD
  #define DebugBreak() assert( 0 )
#endif

#define ERRORHERE { GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Test failed @ " + CORE::CString( __FILE__ ) + CORE::UInt32ToString( __LINE__ ) ); DebugBreak(); }

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/ 

using namespace GUCEF;

/*-------------------------------------------------------------------------*/

void
RunHTTPServerTest( int argc    , 
                   char** argv )
{GUCEF_TRACE;

    #if defined( GUCEF_MSWIN_BUILD ) && defined( GUCEF_CORE_DEBUG_MODE )
    CORE::CMSWinConsoleLogger consoleLogger;
    consoleLogger.SetMinimalLogLevel( CORE::LOGLEVEL_BELOW_NORMAL );
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &consoleLogger );
    #endif

    WEB::CHttpServerSettings httpServerSettings;
    httpServerSettings.allowWebSocketUpgrades = true;

    WEB::CHTTPServer httpServer( GUCEF_NULL, &httpServerSettings );    
    if ( httpServer.ListenOnPort( 45678 ) )
    {
        CORE::CCoreGlobal::Instance()->GetApplication().main( argc, argv, true );
    }
    else
    {
        // We have to be able to listen on that port in order to run our tests
        ERRORHERE;
    }
    
    #if defined( GUCEF_MSWIN_BUILD ) && defined( GUCEF_CORE_DEBUG_MODE )
    CORE::CCoreGlobal::Instance()->GetLogManager().RemoveLogger( &consoleLogger );    
    #endif
}

/*-------------------------------------------------------------------------*/
