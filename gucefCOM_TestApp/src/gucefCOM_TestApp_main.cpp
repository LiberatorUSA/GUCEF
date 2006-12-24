/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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

#ifndef GUCEFCOM_H
#include "gucefCOM.h"
#define GUCEFCOM_H
#endif /* GUCEFCOM_H ? */

#include "TestCode_CHTTPClient.h"

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
        using namespace GUCEF::COM;
                
        try 
        {                               
                SetupHTTPClientTest();

                CGUCEFApplication::Instance()->Main( hinstance     ,
                                                     lpcmdline     ,
                                                     ncmdshow      ,
                                                     true          );
                
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

/*-------------------------------------------------------------------------*/
