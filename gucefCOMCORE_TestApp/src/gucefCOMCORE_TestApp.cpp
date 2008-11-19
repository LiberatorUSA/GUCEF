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

#include <windows.h>

#ifndef GUCEFCORE_H
#include "gucefCORE.h"
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefCOMCORE.h"
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */

#include "TestCode_PingTest.h"
#include "TestCode_ClientServer.h"
#include "TestCode_SimpleTCPClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/


          
/*
 *      Application entry point
 */
int __stdcall
WinMain( HINSTANCE hinstance     ,
         HINSTANCE hprevinstance ,
         LPSTR lpcmdline         ,
         int ncmdshow            )
{
        using namespace GUCEF::CORE;
        using namespace GUCEF::COMCORE;
                
        try 
        {                               
                PerformPingTest();
                SetupClientServerTest();
                SetupSimpleTCPClient("httpd.apache.org", 80, "GET /\r\n");
                
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 10-12-2006
        - Dinand: Added this comment section

-----------------------------------------------------------------------------*/