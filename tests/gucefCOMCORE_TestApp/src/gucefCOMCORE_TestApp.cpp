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

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_COMCORE_H
#include "gucefCOMCORE.h"
#define GUCEF_COMCORE_H
#endif /* GUCEF_COMCORE_H ? */

#include "TestCode_PingTest.h"
#include "TestCode_ClientServer.h"
#include "TestCode_SimpleTCPClient.h"
#include "TestCode_DNS.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    using namespace GUCEF::CORE;
    using namespace GUCEF::COMCORE;
            
    try 
    {                               
        PerformDNSTests();
        
        GUCEF::CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
        GUCEF::CORE::AppendToPath( logFilename, "gucefCOMCORE_TestApp_Log.txt" );
        GUCEF::CORE::CFileAccess logFileAccess( logFilename, "w" );
        
        GUCEF::CORE::CStdLogger logger( logFileAccess );
        GUCEF::CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &logger );
        
        GUCEF::CORE::CPlatformNativeConsoleLogger consoleOut;
        GUCEF::CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( consoleOut.GetLogger() );
        
        GUCEF::CORE::CCoreGlobal::Instance()->GetLogManager().FlushBootstrapLogEntriesToLogs();

        
        PerformPingTest();
        SetupClientServerTest();
        SetupSimpleTCPClient("httpd.apache.org", 80, "GET /\r\n");
        
        return 1;                                                                            
    }
    catch ( ... )
    {        
        GUCEF::CORE::ShowErrorMessage( "Unknown exception"                                                                 ,
                                       "Unhandled exception during program execution, the application will now terminate"  );                                                         
    }
    return 1;                                                                                                                              
}
GUCEF_OSMAIN_END

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 10-12-2006
        - Dinand: Added this comment section

-----------------------------------------------------------------------------*/