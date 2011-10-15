/*
 *  gucefGUI_TestApp: Application for testing gucefGUI
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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_CPLUGINMETADATA_H
#include "gucefCORE_CPluginMetaData.h"
#define GUCEF_CORE_CPLUGINMETADATA_H
#endif /* GUCEF_CORE_CPLUGINMETADATA_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H
#include "gucefCORE_CPlatformNativeConsoleWindow.h"
#define GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H
#endif /* GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H ? */

#ifndef GUCEF_GUI_CWINDOWMANAGER_H
#include "gucefGUI_CWindowManager.h"
#define GUCEF_GUI_CWINDOWMANAGER_H
#endif /* GUCEF_GUI_CWINDOWMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

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

    #ifdef GUCEF_GUI_DEBUG_MODE
    //CORE::GUCEF_LogStackToStdOut();
    //CORE::GUCEF_SetStackLogging( 1 );
    #endif /* GUCEF_GUI_DEBUG_MODE ? */
               
    try 
    {                               
        // setup file logger
        CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
        CORE::AppendToPath( logFilename, "gucefGUI_TestApp_Log.txt" );
        CORE::CFileAccess logFileAccess( logFilename, "w" );
        
        CORE::CStdLogger logger( logFileAccess );
        CORE::CLogManager::Instance()->AddLogger( &logger );
        
        // setup console logger
        CORE::CPlatformNativeConsoleLogger consoleOut;
        CORE::CLogManager::Instance()->AddLogger( consoleOut.GetLogger() );

        // flush startup log entries
        CORE::CLogManager::Instance()->FlushBootstrapLogEntriesToLogs();
        
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Attempting to load Ms Win drivers" );
        
        // Define our backend plugin
        CORE::CPluginMetaData pluginMetaData;
        pluginMetaData.SetPluginType( "GucefGenericPlugin" );
        #ifdef GUCEF_GUI_DEBUG_MODE
        pluginMetaData.SetModuleFilename( "guidriverWin32GL_d" );
        #else
        pluginMetaData.SetModuleFilename( "guidriverWin32GL" );
        #endif
        pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$" ) );
        
        // Add plugin metadata and load the plugin
        if ( CORE::CPluginControl::Instance()->AddPluginMetaData( pluginMetaData ,
                                                                  "GUI"          ,
                                                                  true           ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverWin32GL" );

            GUI::TWindowManagerBackendPtr windowMngrBackend = GUI::CWindowManager::Instance()->GetBackend( "Win32GL" );
            if ( NULL != windowMngrBackend )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully obtained window manager backend Win32GL" );

                GUI::TWindowContextPtr windowContext = windowMngrBackend->CreateWindowContext( "gucefGUI_TestAPP WindowsContext Win32GL" ,
                                                                                               800                                       ,
                                                                                               600                                       ,
                                                                                               false                                     );

                if ( NULL != windowContext )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully created window context using backend Win32GL" );

                    CORE::CPlatformNativeConsoleWindow consoleWindow;
                    consoleWindow.CreateConsole();
                    
                    CORE::CGUCEFApplication::Instance()->main( argc, argv, true );
                }
                else
                {
                }
            }
            else
            {
            }
        }
        else
        {
        }

        #endif

        return 1;                                                                            
    }
    catch ( ... )
    {
        #ifdef GUCEF_GUI_DEBUG_MODE
        CORE::GUCEF_PrintCallstack();
        CORE::GUCEF_DumpCallstack( "gucefGUI_TestApp_callstack.txt" );
        #endif /* GUCEF_GUI_DEBUG_MODE ? */
        
        CORE::ShowErrorMessage( "Unknown exception"                                                                 ,
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

---------------------------------------------------------------------------*/