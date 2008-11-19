// PatcherGUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPatcherGUIApp:
// See PatcherGUI.cpp for the implementation of this class
//

class CPatcherApplication;

class CPatcherGUIApp : public CWinApp
{
public:
	CPatcherGUIApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	
	private:
	
	CPatcherApplication* m_patchApplication;
};

extern CPatcherGUIApp theApp;