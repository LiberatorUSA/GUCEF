// MFCPatcherGUI.h : main header file for the MFCPatcherGUI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCPatcherGUIApp:
// See MFCPatcherGUI.cpp for the implementation of this class
//

class CMFCPatcherGUIApp : public CWinApp
{
public:
	CMFCPatcherGUIApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCPatcherGUIApp theApp;