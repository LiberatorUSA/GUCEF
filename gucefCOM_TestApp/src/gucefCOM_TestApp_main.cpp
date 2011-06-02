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

#ifndef GUCEF_COM_H
#include "gucefCOM.h"
#define GUCEF_COM_H
#endif /* GUCEF_COM_H ? */

#ifndef TESTCODE_CHTTPCLIENT_H
#include "TestCode_CHTTPClient.h"
#define TESTCODE_CHTTPCLIENT_H
#endif /* TESTCODE_CHTTPCLIENT_H ? */

#ifndef TESTCODE_CHTTPSERVER_H
#include "TestCode_CHTTPServer.h"
#define TESTCODE_CHTTPSERVER_H
#endif /* TESTCODE_CHTTPSERVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    using namespace GUCEF::CORE;
    using namespace GUCEF::COMCORE;
    using namespace GUCEF::COM;
            
    try 
    {                               
       // SetupHTTPClientTest();

        RunHTTPServerTest( argc, argv );
        
        return 1;                                                                            
    }
    catch ( ... )
    {
        #ifdef DEBUG_MODE
        GUCEF_PrintCallstack();
        GUCEF_DumpCallstack( "GUCEFCallstack.txt" );
        #endif
        
        ShowErrorMessage( "Unknown exception"                                                                 ,
                          "Unhandled exception during program execution, the application will now terminate"  );                                                         
    }
    GUCEF_END;
    return 1;                                                                                                                              
}
GUCEF_OSMAIN_END

/*-------------------------------------------------------------------------*/
