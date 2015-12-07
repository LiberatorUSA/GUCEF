/*
 *  gucefIMAGE: GUCEF module providing image utilities
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
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

#ifndef GUCEF_IMAGE_H
#include "gucefIMAGE.h"
#define GUCEF_IMAGE_H
#endif /* GUCEF_IMAGE_H ? */

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
LoadConfig( void )
{
    #ifdef GUCEF_GUI_DEBUG_MODE
    const CORE::CString configFile = "bootstrap_d.ini";
    #else
    const CORE::CString configFile = "bootstrap.ini";
    #endif

    CORE::CString configFilePath = CORE::CombinePath( "$MODULEDIR$", configFile );
    configFilePath = CORE::RelativePath( configFilePath );

    if ( !FileExists( configFilePath ) )
    {
        configFilePath = CORE::CombinePath( "$CURWORKDIR$", configFile );
        configFilePath = CORE::RelativePath( configFilePath );

        if ( !FileExists( configFilePath ) )
        {
            // hardcoded relative path from compiler output bin to testdata folder in the archive
            #if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN
            configFilePath = CORE::CombinePath( "$MODULEDIR$/../../../TestData/gucefIMAGE_TestApp/mswin", configFile );
            #elif GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX
            configFilePath = CORE::CombinePath( "$MODULEDIR$/../../TestData/gucefIMAGE_TestApp/linux", configFile );
            #endif
            configFilePath = CORE::RelativePath( configFilePath );
        }
    }

    CORE::CConfigStore& configStore = CORE::CCoreGlobal::Instance()->GetConfigStore();
    configStore.SetConfigFile( configFilePath );
    return configStore.LoadConfig();
}

/*-------------------------------------------------------------------------*/
          
/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    #ifdef GUCEF_IMAGE_DEBUG_MODE
    //GUCEF::CORE::GUCEF_LogStackToStdOut();
    //GUCEF::CORE::GUCEF_SetStackLogging( 1 );
    #endif /* GUCEF_IMAGE_DEBUG_MODE ? */
               
    try 
    {                               

        GUCEF::CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
        GUCEF::CORE::AppendToPath( logFilename, "gucefIMAGE_TestApp_Log.txt" );
        GUCEF::CORE::CFileAccess logFileAccess( logFilename, "w" );
        
        GUCEF::CORE::CStdLogger logger( logFileAccess );
        GUCEF::CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &logger );
        
        GUCEF::CORE::CPlatformNativeConsoleLogger consoleOut;
        GUCEF::CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( consoleOut.GetLogger() );

        LoadConfig();
        
        GUCEF::IMAGE::CImage image;
        image.Load( "C:\\CODE\\intovoid\\RES\\IV_MATB_IDX\\8000.1" );

        return 1;                                                                            
    }
    catch ( ... )
    {
        #ifdef GUCEF_IMAGE_DEBUG_MODE
        GUCEF_PrintCallstack();
        GUCEF_DumpCallstack( "gucefIMAGE_TestApp_callstack.txt" );
        #endif /* GUCEF_IMAGE_DEBUG_MODE ? */
        
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