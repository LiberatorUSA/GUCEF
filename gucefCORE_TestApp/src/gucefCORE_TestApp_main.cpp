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

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
          
/*
 *      Application entry point
 */
int APIENTRY 
WinMain( HINSTANCE hinstance     ,
           HINSTANCE hprevinstance ,
           LPSTR lpcmdline         ,
           int ncmdshow            )
{TRACE;
        using namespace GUCEF::CORE;
                
        try 
        {
                CString w("w");
                w = NULL;
                w = "aap";
        
                CString a("a"), b("b"), c("c"), d, e, f;
        
                d = a;
                d = NULL;
                e = "e";
                f = e;
                e = NULL;
                f = NULL;
        
                CPluginControl::Instance()->SetPluginDir( "$MODULEDIR$\\plugins" );
                CPluginControl::Instance()->LoadAll();
        
                CConfigStore* cs = CConfigStore::Instance();
                
                //cs->SetConfigFile( "GUC_cfg.xml" );
                //cs->SaveConfig( "GUCCONFIG" );
                
                
                //cs->LoadConfig();
        
                cs->SaveConfig( "GUCCONFIG" ); 
                
                return 1;                                                                            
        }
        catch ( ... )
        {
                GUCEF_PrintCallstack();
                GUCEF_DumpCallstack( "GUCEFCallstack.txt" );
                
                ShowErrorMessage( "Unknown exception"                                                                 ,
                                  "Unhandled exception during program execution, the application will now terminate"  );                                                         
        }
        return 1;                                                                                                                              
}

/*-------------------------------------------------------------------------*/
