/*
 *  GucefLogService: Independent logging service for GUCEF
 *
 *  Copyright (C) 2002 - 2009.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifdef GUCEF_MSWIN_BUILD
#include <windows.h>
#endif /* GUCEF_MSWIN_BUILD ? */

#ifndef GUCEF_LOGSERVICELIB_H
#include "GucefLogServiceLib.h"
#define GUCEF_LOGSERVICELIB_H
#endif /* GUCEF_LOGSERVICELIB_H ? */ 

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

const bool defaultShowConsoleState = true;
CORE::UInt16 defaultListnenPort = 43557;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
ParseParams( const int argc                 , 
             char* argv[]                   ,
             CORE::CValueList& keyValueList )
{GUCEF_TRACE;
    
    keyValueList.DeleteAll();
    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        // Combine the argument strings back into a single string because we don't want to use
        // a space as the seperator
        for ( int i=1; i<argc; ++i )
        {
            argString += ' ' + CORE::CString( argv[ i ] );
        }
        
        // Parse the param list based on the ' symbol
        keyValueList.SetMultiple( argString, '\'' );
    }
}

/*-------------------------------------------------------------------------*/

int
main( int argc, char* argv[] )
{GUCEF_TRACE;

    #ifdef GUCEF_CALLSTACK_TRACING
    CORE::GUCEF_LogStackTo( "GucefLogServiceApp_Callstack.cvs" );
    CORE::GUCEF_SetStackLoggingInCvsFormat( 1 );
    CORE::GUCEF_SetStackLogging( 1 );
    #endif /* GUCEF_CALLSTACK_TRACING ? */    
    
    CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( logFilename, "GucefLogServiceApp_Log.txt" );
    CORE::CFileAccess logFileAccess( logFilename, "w" );
    
    CORE::CStdLogger logger( logFileAccess );
    CORE::CLogManager::Instance()->AddLogger( &logger );
    
    // Parse the application parameters
    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );

    // Do we want to display the console window?
    CORE::CPlatformNativeConsoleLogger* consoleOut = NULL;    
    bool showConsole = defaultShowConsoleState;    
    if ( keyValueList.HasKey( "showConsole" ) )
    {
        showConsole = CORE::StringToBool( keyValueList.GetValue( "showConsole" ) );
    }
    if ( showConsole )
    {
        consoleOut = new CORE::CPlatformNativeConsoleLogger();
        CORE::CLogManager::Instance()->AddLogger( consoleOut->GetLogger() );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GucefLogServiceApp: Enabled console output" );
    }
    
    // Flush bootstrap logging now that we attached all our loggers
    CORE::CLogManager::Instance()->FlushBootstrapLogEntriesToLogs();
    
    // Get the desired listnen port for the server
    CORE::UInt16 serverListnenPort = defaultListnenPort;
    if ( keyValueList.HasKey( "port" ) )
    {
        serverListnenPort = CORE::StringToUInt16( keyValueList.GetValue( "port" ) );
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GucefLogServiceApp: Changing server port from default " + CORE::UInt16ToString( defaultListnenPort ) + " to " + CORE::UInt16ToString( serverListnenPort ) );
    }

    // Get the desired output path for the log files
    CORE::CString logFilesOutputDir = "$MODULEDIR$";
    if ( keyValueList.HasKey( "outputDir" ) )
    {
        logFilesOutputDir = keyValueList.GetValue( "outputDir" );
    }
    else
    {
        logFilesOutputDir = CORE::RelativePath( "$MODULEDIR$" );
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GucefLogServiceApp: Setting server output dir for log files to " + logFilesOutputDir );
        
    int appRetValue = 0;
    LOGSERVICELIB::CLogSvcServerFileLogger fileLogger( logFilesOutputDir );
    LOGSERVICELIB::CLogSvcServer logServiceServer;    
    logServiceServer.RegisterLogger( &fileLogger );
    
    if ( logServiceServer.ListenOnPort( serverListnenPort ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GucefLogServiceApp: Server set to listnen on port " + CORE::UInt16ToString( serverListnenPort ) );
        
        appRetValue = CORE::CGUCEFApplication::Instance()->main( argc, argv, true );
    }
    
    logServiceServer.UnregisterLogger( &fileLogger );
    
    delete consoleOut;
    consoleOut = NULL;
    
    return appRetValue;
}

/*---------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

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

/*---------------------------------------------------------------------------*/
