/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
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

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEFCORE_TESTAPP_TESTSHAREDPTR_H
#include "TestSharedPtr.h"
#define GUCEFCORE_TESTAPP_TESTSHAREDPTR_H
#endif /* GUCEFCORE_TESTAPP_TESTSHAREDPTR_H ? */

#ifndef GUCEFCORE_TESTAPP_TESTNOTIFIEROBSERVER_H
#include "TestNotifierObserver.h"
#define GUCEFCORE_TESTAPP_TESTNOTIFIEROBSERVER_H
#endif /* GUCEFCORE_TESTAPP_TESTNOTIFIEROBSERVER_H ? */

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
                CString* x = new CString( "x" );
                CString w("w");
                w = NULL;
                w = "aap";
                
                std::string* stlStr = new std::string( "stlStr" );
                w = *stlStr;
                *stlStr = "1234567";
                delete stlStr;
                stlStr = new std::string( *x );
                delete x;
                delete stlStr;
                
                CString a("a"), b("b"), c("c"), d, e, f;
        
                d = a;
                d = NULL;
                e = "e";
                f = e;
                e = NULL;
                f = NULL;
        
               // CPluginControl::Instance()->SetPluginDir( "$MODULEDIR$\\plugins" );
               // CPluginControl::Instance()->LoadAll();
        
              //  CConfigStore* cs = CConfigStore::Instance();
                
                //cs->SetConfigFile( "GUC_cfg.xml" );
                //cs->SaveConfig( "GUCCONFIG" );
                
                
                //cs->LoadConfig();

                //PerformSharedPtrTests();
                
                PerformNotifierObserverTests();
        
               // cs->SaveConfig( "GUCCONFIG" ); 
                
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
