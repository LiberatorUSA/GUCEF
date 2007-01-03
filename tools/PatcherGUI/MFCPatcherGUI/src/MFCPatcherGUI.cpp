// MFCPatcherGUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#include "MFCPatcherGUI.h"
#include "MainFrm.h"
#include "CMFCCommandLineInfo.h"

// CMFCPatcherGUIApp

BEGIN_MESSAGE_MAP(CMFCPatcherGUIApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMFCPatcherGUIApp::OnAppAbout)
END_MESSAGE_MAP()


// CMFCPatcherGUIApp construction

CMFCPatcherGUIApp::CMFCPatcherGUIApp()
    : m_cmdLineParamList()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMFCPatcherGUIApp object

CMFCPatcherGUIApp theApp;

CFrameWnd*
CMFCPatcherGUIApp::GetMainFrame( void )
{TRACE;

    return static_cast< CFrameWnd* >( m_pMainWnd );
}

// CMFCPatcherGUIApp initialization

BOOL CMFCPatcherGUIApp::InitInstance()
{
    // Parse the command line params	
    CMFCCommandLineInfo oInfo;
    ParseCommandLine( oInfo );
    m_cmdLineParamList.SetMultiple( oInfo.GetParamString(), '"' );

    // Obtain the plugin dir path
    GUCEF::CORE::CString pluginDir = m_cmdLineParamList.GetValue( "PluginDir" );
    if ( pluginDir.Length() == 0 )
    {
        pluginDir = "$CURWORKDIR$\\plugins";
    }
    
    // Load all plugins
    GUCEF::CORE::CPluginControl* pluginControl = GUCEF::CORE::CPluginControl::Instance();
    pluginControl->SetPluginDir( pluginDir );
    pluginControl->LoadAll();

    // Check for mandatory command line params
    // If we do not find any params we will use defaults, this makes it easy to use the tool 
    // for rapid deployment while keeping your options open.
    if ( !m_cmdLineParamList.HasKey( "ConfigFile" ) )
    {
        m_cmdLineParamList.Set( "ConfigFile", "$CURWORKDIR$\\MFCPatcherConfig.xml" );
    }
    if ( !m_cmdLineParamList.HasKey( "ConfigFileCodec" ) )
    {
        m_cmdLineParamList.Set( "ConfigFileCodec", "xml" );
    }    

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("VanvelzenSoftware"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

const GUCEF::CORE::CValueList&
CMFCPatcherGUIApp::GetCommandLineParams( void ) const
{TRACE;
    
    return m_cmdLineParamList;
}

// CMFCPatcherGUIApp message handlers




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CMFCPatcherGUIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMFCPatcherGUIApp message handlers

