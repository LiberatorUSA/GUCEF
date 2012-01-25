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

#ifndef GUCEF_GUI_CGUIGLOBAL_H
#include "gucefGUI_CGuiGlobal.h"
#define GUCEF_GUI_CGUIGLOBAL_H
#endif /* GUCEF_GUI_CGUIGLOBAL_H ? */

#ifndef GUCEF_GUI_CGUIMANAGER_H
#include "gucefGUI_CGUIManager.h"
#define GUCEF_GUI_CGUIMANAGER_H
#endif /* GUCEF_GUI_CGUIMANAGER_H ? */

#ifndef GUCEF_GUI_CWINDOWMANAGER_H
#include "gucefGUI_CWindowManager.h"
#define GUCEF_GUI_CWINDOWMANAGER_H
#endif /* GUCEF_GUI_CWINDOWMANAGER_H ? */

#ifndef GUCEF_GUI_CFORMEX_H
#include "gucefGUI_CFormEx.h"
#define GUCEF_GUI_CFORMEX_H
#endif /* GUCEF_GUI_CFORMEX_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

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

bool
LoadPlugins( void )
{GUCEF_TRACE;

    CORE::CPluginMetaData pluginMetaData;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Attempting to load Ms Win drivers" );

    // Define our windowing backend plugin
    pluginMetaData.SetPluginType( "GucefGenericPlugin" );
    pluginMetaData.SetModuleFilename( "guidriverWin32GL" );
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$" ) );

    // Add plugin metadata and load the plugin
    if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                              "GUI"          ,
                                                                              true           ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverWin32GL" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load guidriverWin32GL" );

        pluginMetaData.SetModuleFilename( "guidriverWin32GL_d" );

        // Add plugin metadata and load the plugin
        if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                                  "GUI"          ,
                                                                                  true           ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverWin32GL_d" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load guidriverWin32GL_d" );
            return false;
        }
    }

    #endif


    /*-------------------------------------------------------------*/

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Attempting to load X11 drivers" );

    // Define our windowing backend plugin
    pluginMetaData.SetPluginType( "GucefGenericPlugin" );
    pluginMetaData.SetModuleFilename( "guidriverXWinGL" );
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$/../lib" ) );

    // Add plugin metadata and load the plugin
    if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                              "GUI"          ,
                                                                              true           ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverXWinGL" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load guidriverXWinGL" );

        pluginMetaData.SetModuleFilename( "libguidriverXWinGL_d" );

        // Add plugin metadata and load the plugin
        if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                                  "GUI"          ,
                                                                                  true           ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverXWinGL" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load guidriverXWinGL" );
            return false;
        }
    }

    #endif


    /*-------------------------------------------------------------*/

    // Define our GUI backend plugin
    pluginMetaData.Clear();
    pluginMetaData.SetPluginType( "GucefGenericPlugin" );
    pluginMetaData.SetModuleFilename( "guidriverRocket" );
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$/../lib" ) );
    #else
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$" ) );
    #endif

    // Add plugin metadata and load the plugin
    if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                              "GUI"          ,
                                                                              true           ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverRocket" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load guidriverRocket" );

        pluginMetaData.SetModuleFilename( "guidriverRocket_d" );

        // Add plugin metadata and load the plugin
        if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                                  "GUI"          ,
                                                                                  true           ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverRocket_d" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load guidriverRocket_d" );
            return false;
        }
    }

    /*-------------------------------------------------------------*/

    // Define our GUI backend OpenGL plugin
    pluginMetaData.Clear();
    pluginMetaData.SetPluginType( "GucefGenericPlugin" );
    pluginMetaData.SetModuleFilename( "guidriverRocketOpenGL" );

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$/../lib" ) );
    #else
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$" ) );
    #endif

    // Add plugin metadata and load the plugin
    if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                              "GUI"          ,
                                                                              true           ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverRocketOpenGL" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load guidriverRocketOpenGL" );

        pluginMetaData.SetModuleFilename( "guidriverRocketOpenGL_d" );

        // Add plugin metadata and load the plugin
        if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                                  "GUI"          ,
                                                                                  true           ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded guidriverRocketOpenGL_d" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load guidriverRocketOpenGL_d" );
            return false;
        }
    }

    /*-------------------------------------------------------------*/

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    // Define our input plugin for Win32
    pluginMetaData.Clear();
    pluginMetaData.SetPluginType( "GucefInputDriverPlugin" );
    #ifdef GUCEF_GUI_DEBUG_MODE
    pluginMetaData.SetModuleFilename( "inputdriverMSWINMSG_d" );
    #else
    pluginMetaData.SetModuleFilename( "inputdriverMSWINMSG" );
    #endif
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$" ) );

    // Add plugin metadata and load the plugin
    if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                              "INPUT"        ,
                                                                              true           ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded inputdriverMSWINMSG" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load inputdriverMSWINMSG" );
        return false;
    }

    #endif

    /*-------------------------------------------------------------*/

    #if 0

    // Define our optional logging service client plugin
    pluginMetaData.Clear();
    pluginMetaData.SetPluginType( "GucefGenericPlugin" );
    #ifdef GUCEF_GUI_DEBUG_MODE
    pluginMetaData.SetModuleFilename( "GucefLogServiceClientPlugin_d" );
    #else
    pluginMetaData.SetModuleFilename( "GucefLogServiceClientPlugin" );
    #endif
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$/../lib" ) );
    #else
    pluginMetaData.SetFullModulePath( CORE::RelativePath( "$MODULEDIR$" ) );
    #endif

    // Add plugin metadata and load the plugin
    if ( CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginMetaData( pluginMetaData ,
                                                                              "LOGGING"      ,
                                                                              true           ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded GucefLogServiceClientPlugin" );
    }
    else
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load GucefLogServiceClientPlugin" );
    }

    #endif

    /*-------------------------------------------------------------*/

    return true;
}

/*-------------------------------------------------------------------------*/

void
LoadFonts( GUI::TGuiContextPtr guiContext )
{GUCEF_TRACE;

	guiContext->GetDriver()->LoadFontFromAsset( "Delicious-Roman.otf" );
    guiContext->GetDriver()->LoadFontFromAsset( "Delicious-Italic.otf" );
    guiContext->GetDriver()->LoadFontFromAsset( "Delicious-Bold.otf" );
    guiContext->GetDriver()->LoadFontFromAsset( "Delicious-BoldItalic.otf" );
}

/*-------------------------------------------------------------------------*/

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
        CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &logger );

        // setup console logger
        CORE::CPlatformNativeConsoleLogger consoleOut;
        CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( consoleOut.GetLogger() );

        // flush startup log entries
        CORE::CCoreGlobal::Instance()->GetLogManager().FlushBootstrapLogEntriesToLogs();

        // Load all the plugins we need for this test
        if ( LoadPlugins() )
        {
            GUI::TWindowManagerBackendPtr windowMngrBackend = GUI::CGuiGlobal::Instance()->GetWindowManager().GetBackend( "Win32GL" );
            if ( NULL != windowMngrBackend )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully obtained window manager backend Win32GL" );

                GUI::TWindowContextPtr windowContext = windowMngrBackend->CreateWindowContext( "gucefGUI_TestAPP Win32GL WindowContext" ,
                                                                                                800                                     ,
                                                                                                600                                     ,
                                                                                                false                                   );

                if ( NULL != windowContext )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully created window context using backend Win32GL" );

                    // Create console window for easy test interaction
                    CORE::CPlatformNativeConsoleWindow consoleWindow;
                    consoleWindow.CreateConsole();

                    // create GUI context for our window
                    GUI::TGuiContextPtr guiContext = GUI::CGuiGlobal::Instance()->GetGuiManager().CreateGUIContext( "RocketOpenGL", windowContext );

                    // The following determines the path to our test data. Note that this makes assumptions about the archive paths
                    CORE::CString assetDir = CORE::RelativePath( "$MODULEDIR$" );
                    assetDir = assetDir.SubstrToSubstr( "trunk" );
                    CORE::AppendToPath( assetDir, "trunk\\dependencies\\libRocket\\Samples\\assets" );
                    VFS::CVfsGlobal::Instance()->GetVfs().AddRoot( assetDir, "RocketGUISampleAssets", false, false );
                    assetDir = CORE::RelativePath( "$MODULEDIR$" );
                    assetDir = assetDir.SubstrToSubstr( "trunk" );
                    CORE::AppendToPath( assetDir, "trunk\\dependencies\\libRocket\\Samples\\invaders\\data" );
                    VFS::CVfsGlobal::Instance()->GetVfs().AddRoot( assetDir, "RocketGUISampleAssets2", false, false );
                    assetDir = CORE::RelativePath( "$MODULEDIR$" );
                    assetDir = assetDir.SubstrToSubstr( "trunk" );
                    CORE::AppendToPath( assetDir, "trunk\\dependencies\\libRocket\\Samples\\basic\\drag\\data" );
                    VFS::CVfsGlobal::Instance()->GetVfs().AddRoot( assetDir, "RocketGUISampleAssets3", false, false );

                    // Load some fonts
                    LoadFonts( guiContext );

                    // Create a form to load the layout into
                    GUI::CFormEx* form = static_cast< GUI::CFormEx* >( guiContext->CreateForm( "FormEx" ) );

                    // load the test layout resource
                    if ( form->LoadLayoutUsingVfs( "main_menu.rml" ) )
                    {
                        CORE::CCoreGlobal::Instance()->GetApplication().main( argc, argv, true );
                    }
                    else
                    {
                        CORE::ShowErrorMessage( "Initialization error"                                                     ,
                                                GUI::CString( "Failed to load layout assets from " + assetDir ).C_String() );
                    }
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to obtain window management backend" );
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to obtain a window context" );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load one or more plugins" );
        }

        CORE::CCoreGlobal::Instance()->GetLogManager().ClearLoggers();
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
