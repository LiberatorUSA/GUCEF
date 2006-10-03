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

#include "stdafx.h"
#include "PatcherGUI.h"
#include "PatcherGUIDlg.h"
#include "gucefPATCHER_CStandardPSPEventHandler.h"
#include "CPatcherApplication.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*-------------------------------------------------------------------------*/

// CPatcherGUIApp

BEGIN_MESSAGE_MAP(CPatcherGUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

/*-------------------------------------------------------------------------*/


// CPatcherGUIApp construction

CPatcherGUIApp::CPatcherGUIApp()
        : m_patchApplication( CPatcherApplication::Instance() )
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

// The one and only CPatcherGUIApp object

CPatcherGUIApp theApp;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

// CPatcherGUIApp initialization

BOOL CPatcherGUIApp::InitInstance()
{
	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T( "VanvelzenSoftware" ));

	CPatcherGUIDlg dlg;
	m_pMainWnd = &dlg;
	
	// Create a list of event handlers which includes out dialog
	GUCEF::PATCHER::CStandardPSPEventHandler standardHandler;
	CPatcherApplication::TEventHandlerList eventHandlerList;
	eventHandlerList.push_back( reinterpret_cast< CPatcherApplication::TEventHandler* >( &dlg ) );
	eventHandlerList.push_back( &standardHandler );
	m_patchApplication->SetEventHandlers( eventHandlerList );	
	
	// Start the dialog
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/*-------------------------------------------------------------------------*/
