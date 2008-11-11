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

#ifndef GUCEFCORE_H
#include "gucefCORE.h"
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefCOMCORE.h"
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */

#ifndef GUCEFCOM_H
#include "gucefCOM.h"
#define GUCEFCOM_H
#endif /* GUCEFCOM_H ? */

#include "TestCode_CHTTPServer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MSWIN_BUILD
  #define DebugBreak() assert( 0 )
#endif

#define ERRORHERE { GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "Test failed @ " + CORE::CString( __FILE__ ) + CORE::UInt32ToString( __LINE__ ) ); DebugBreak(); }

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/ 

using namespace GUCEF;
using namespace GUCEF::CORE;
using namespace GUCEF::COMCORE;
using namespace GUCEF::COM;

/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/

void
RunHTTPServerTest( int argc    , 
                   char** argv )
{GUCEF_TRACE;

    CHTTPServer httpServer;
    if ( httpServer.ListenOnPort( 45678 ) )
    {
        CGUCEFApplication::Instance()->main( argc, argv, true );
    }
    else
    {
        // We have to be able to listen on that port in order to run our tests
        ERRORHERE;
    }
}

/*-------------------------------------------------------------------------*/
