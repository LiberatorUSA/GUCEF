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

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN
#include <windows.h>
#endif

#ifndef GUCEF_CORE_TESTAPP_TESTSHAREDPTR_H
#include "TestSharedPtr.h"
#define GUCEF_CORE_TESTAPP_TESTSHAREDPTR_H
#endif /* GUCEF_CORE_TESTAPP_TESTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_TESTAPP_TESTSTRING_H
#include "TestString.h"
#define GUCEF_CORE_TESTAPP_TESTSTRING_H
#endif /* GUCEF_CORE_TESTAPP_TESTSTRING_H ? */

#ifndef GUCEF_CORE_TESTAPP_TESTNOTIFIEROBSERVER_H
#include "TestNotifierObserver.h"
#define GUCEF_CORE_TESTAPP_TESTNOTIFIEROBSERVER_H
#endif /* GUCEF_CORE_TESTAPP_TESTNOTIFIEROBSERVER_H ? */

#ifndef GUCEF_CORE_TESTAPP_TESTCYCLICDYNAMICBUFFER_H
#include "TestCyclicDynamicBuffer.h"
#define GUCEF_CORE_TESTAPP_TESTCYCLICDYNAMICBUFFER_H
#endif /* GUCEF_CORE_TESTAPP_TESTCYCLICDYNAMICBUFFER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
int
main( int argc, char** argv )
{GUCEF_TRACE;

    #ifdef GUCEF_CORE_DEBUG_MODE
    //GUCEF::CORE::GUCEF_LogStackToStdOut();
    //GUCEF::CORE::GUCEF_SetStackLogging( 1 );
    #endif /* GUCE_CORE_DEBUG_MODE ? */

    try
    {
        GUCEF::CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
        GUCEF::CORE::AppendToPath( logFilename, "gucefCORE_TestApp_Log.txt" );
        GUCEF::CORE::CFileAccess logFileAccess( logFilename, "w" );

        GUCEF::CORE::CStdLogger logger( logFileAccess );
        GUCEF::CORE::CLogManager::Instance()->AddLogger( &logger );

        #ifdef GUCEF_MSWIN_BUILD
        GUCEF::CORE::CMSWinConsoleLogger consoleOut;
        GUCEF::CORE::CLogManager::Instance()->AddLogger( &consoleOut );
        #endif /* GUCEF_MSWIN_BUILD ? */

        //GUCEF::CORE::CGUCEFApplication::Instance()->main( argc, argv, true );

        PerformStringTests();
        
        //PerformSharedPtrTests();

        //PerformCyclicDynamicBufferTests();

        //PerformNotifierObserverTests();

        return 1;
    }
    catch ( ... )
    {
        #ifdef GUCEF_CORE_DEBUG_MODE
        GUCEF::CORE::GUCEF_PrintCallstack();
        GUCEF::CORE::GUCEF_DumpCallstack( "gucefCORE_TestApp_callstack.txt" );
        #endif /* GUCEF_CORE_DEBUG_MODE ? */

        GUCEF::CORE::ShowErrorMessage( "Unknown exception"                                                                 ,
                                       "Unhandled exception during program execution, the application will now terminate"  );
    }
    return 1;
}

/*---------------------------------------------------------------------------*/

#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

int __stdcall
WinMain( HINSTANCE hinstance     ,
         HINSTANCE hprevinstance ,
         LPSTR lpcmdline         ,
         int ncmdshow            )
{GUCEF_TRACE;

    int argc = 0;
    char** argv = &lpcmdline;
    if ( lpcmdline != NULL )
    {
        if ( *lpcmdline != '\0' )
        {
            argc = 1;
        }
    }

    return main( argc, argv );
}

#endif

/*-------------------------------------------------------------------------*/
