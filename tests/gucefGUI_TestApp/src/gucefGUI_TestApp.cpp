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

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

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

#ifndef GUCEF_INPUT_CINPUTGLOBAL_H
#include "gucefINPUT_CInputGlobal.h"
#define GUCEF_INPUT_CINPUTGLOBAL_H
#endif /* GUCEF_INPUT_CINPUTGLOBAL_H ? */

#ifndef GUCEF_IMAGE_CIMAGEGLOBAL_H
#include "gucefIMAGE_CImageGlobal.h"
#define GUCEF_IMAGE_CIMAGEGLOBAL_H
#endif /* GUCEF_IMAGE_CIMAGEGLOBAL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CGucefGuiTestAppSettings : public CORE::CIConfigurable
{
    public:

    struct SFormInfo
    {
        GUI::CString formTypeName;
        GUI::CString formResourcePath;
    };
    typedef struct SFormInfo TFormInfo;

    private:
    typedef std::map< GUI::CString, TFormInfo > TFormInfoMap;


    GUI::CString m_windowManagerName;
    TFormInfoMap m_formInfoPerGuiBackend;

    static CGucefGuiTestAppSettings* g_instance;

    public:

    const GUI::CString& GetWindowManagerName( void ) const
    {
        return m_windowManagerName;
    }

    TFormInfo GetInitialFormInfoForGuiBackend( const GUI::CString& guiBackendName ) const
    {
        TFormInfoMap::const_iterator i = m_formInfoPerGuiBackend.find( guiBackendName );
        if ( i != m_formInfoPerGuiBackend.end() )
        {
            return (*i).second;
        }

        return TFormInfo();
    }

    static CGucefGuiTestAppSettings* Instance( void )
    {
        if ( NULL == g_instance )
        {
            g_instance = new CGucefGuiTestAppSettings();
        }
        return g_instance;
    }

    static void Deinstance( void )
    {
        delete g_instance;
        g_instance = NULL;
    }

    /*-------------------------------------------------------------*/

    virtual bool SaveConfig( CORE::CDataNode& tree )
    {
        return true;
    }

    /*-------------------------------------------------------------*/

    virtual bool LoadConfig( const CORE::CDataNode& treeroot )
    {
        CORE::CDataNode::TConstDataNodeSet configNodes( treeroot.FindChildrenOfType( "GucefGuiTestAppSettings", true ) );
        CORE::CDataNode::TConstDataNodeSet::iterator i = configNodes.begin();
        while ( i != configNodes.end() )
        {
            const CORE::CDataNode* configNode = (*i);

            CORE::CString windowManagerName = configNode->GetAttributeValueOrChildValueByName( "WindowManager" );
            if ( !windowManagerName.IsNULLOrEmpty() )
            {
                 m_windowManagerName = windowManagerName;
            }

            CORE::CDataNode::TConstDataNodeSet formDataNodes( configNode->FindChildrenOfType( "InitialForm", true ) );
            CORE::CDataNode::TConstDataNodeSet::iterator n = formDataNodes.begin();
            while ( n != formDataNodes.end() )
            {
                const CORE::CDataNode* formInfoNode = (*n);

                CORE::CString guiBackendName = formInfoNode->GetAttributeValueOrChildValueByName( "GuiBackend" );
                if ( !guiBackendName.IsNULLOrEmpty() )
                {
                     TFormInfo& formInfo = m_formInfoPerGuiBackend[ guiBackendName ];

                     formInfo.formTypeName = formInfoNode->GetChildValueByName( "FormTypeName" );
                     formInfo.formResourcePath = formInfoNode->GetChildValueByName( "FormResource" );

                }
                ++n;
            }

            ++i;
        }
        return true;
    }

    /*-------------------------------------------------------------*/

    private:

    CGucefGuiTestAppSettings( void )
        : CORE::CIConfigurable( true )
    {
    }

    virtual ~CGucefGuiTestAppSettings()
    {
    }
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CGucefGuiTestAppSettings* CGucefGuiTestAppSettings::g_instance = NULL;

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
            configFilePath = CORE::CombinePath( "$MODULEDIR$/../../../TestData/gucefGUI_TestApp/mswin", configFile );
            #elif GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX
            configFilePath = CORE::CombinePath( "$MODULEDIR$/../../TestData/gucefGUI_TestApp/linux", configFile );
            #endif
            configFilePath = CORE::RelativePath( configFilePath );
        }
    }

    CORE::CConfigStore& configStore = CORE::CCoreGlobal::Instance()->GetConfigStore();
    configStore.SetConfigFile( configFilePath );
    return configStore.LoadConfig();
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

void
ConfigAssets( void )
{
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
}

/*-------------------------------------------------------------------------*/

bool
SetupWindowContext( GUI::TWindowManagerBackendPtr windowMngrBackend ,
                    const GUI::CString& windowMngrBackendName       ,
                    const GUI::CString& guiDriverToUse              )
{
    GUI::CString windowTitle( "gucefGUI_TestAPP " + windowMngrBackendName + " - " + guiDriverToUse + " WindowContext" );
    GUI::TWindowContextPtr windowContext = windowMngrBackend->CreateWindowContext( windowTitle ,
                                                                                   800         ,
                                                                                   600         ,
                                                                                   false       );

    if ( NULL != windowContext )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully created window context using backend " + windowMngrBackendName );

        // create GUI context for our window
        GUI::TGuiContextPtr guiContext = GUI::CGuiGlobal::Instance()->GetGuiManager().CreateGUIContext( guiDriverToUse, windowContext );
        if ( NULL != guiContext )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully created GUI context using backend " + guiDriverToUse );

            // Load some fonts
            LoadFonts( guiContext );

            CGucefGuiTestAppSettings::TFormInfo formInfo = CGucefGuiTestAppSettings::Instance()->GetInitialFormInfoForGuiBackend( guiDriverToUse );

            // Create a form to load the layout into
            GUI::CFormEx* form = static_cast< GUI::CFormEx* >( guiContext->CreateForm( formInfo.formTypeName ) );
            if ( NULL != form )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully created form object of type: " + formInfo.formTypeName );

                // load the test layout resource
                if ( form->LoadLayout( formInfo.formResourcePath ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded form from resource: " + formInfo.formResourcePath );
                    return form->Show();
                }
            }
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to obtain a window context" );
    }

    return false;
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
        // Initialize systems
        GUCEF::CORE::CCoreGlobal::Instance();
        GUCEF::VFS::CVfsGlobal::Instance();
        GUCEF::GUI::CGuiGlobal::Instance();
        GUCEF::INPUT::CInputGlobal::Instance();
        GUCEF::IMAGE::CImageGlobal::Instance();
        CGucefGuiTestAppSettings::Instance();

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

        // Create console window for easy test interaction
        CORE::CPlatformNativeConsoleWindow consoleWindow;
        consoleWindow.CreateConsole();

        bool errorOccured = false;

        // Load the config for this test
        if ( !LoadConfig() )
        {
            CORE::ShowErrorMessage( "Initialization error"                ,
                                    "Failures occured loading the config" );

            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load config" );
            errorOccured = true;
        }


        if ( !errorOccured )
        {
            #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
            CORE::CString windowMngrBackendName( "Win32GL" );
            #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
            CORE::CString windowMngrBackendName( "XWinGL" );
            #else
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "There is no window manager defined for the current platform in the test application" );
            #endif

            GUI::TWindowManagerBackendPtr windowMngrBackend = GUI::CGuiGlobal::Instance()->GetWindowManager().GetBackend( windowMngrBackendName );
            if ( NULL != windowMngrBackend )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully obtained window manager backend " + windowMngrBackendName );

                // Now get a list of all registered GUI drivers
                bool allFailed = true;
                GUI::CGUIManager::TDriverNameSet guiDriverList = GUI::CGuiGlobal::Instance()->GetGuiManager().GetGuiDriverList();
                GUI::CGUIManager::TDriverNameSet::iterator i = guiDriverList.begin();
                while ( i != guiDriverList.end() )
                {
                    // Setup a window context for each GUI driver
                    if ( SetupWindowContext( windowMngrBackend, windowMngrBackendName, (*i) ) )
                    {
                        allFailed = false;
                    }
                    else
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to setup a window context for GUI driver " + (*i) );
                    }
                    ++i;
                }

                if ( !allFailed )
                {
                    // Run the test app with what we have
                    CORE::CCoreGlobal::Instance()->GetApplication().main( argc, argv, true );
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to obtain window management backend" );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to start test due to failed preconditions" );
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

        try
        {
            CORE::CCoreGlobal::Instance()->GetLogManager().ClearLoggers();
        }
        catch ( ... ) {}

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
