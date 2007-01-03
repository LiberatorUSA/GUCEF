
#ifndef CMFCPATCHERGUIAPP_H
#define CMFCPATCHERGUIAPP_H

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#include "resource.h"       // main symbols

// CMFCPatcherGUIApp:
// See MFCPatcherGUI.cpp for the implementation of this class
class CMFCPatcherGUIApp : public CWinApp
{
    public:
	
	CMFCPatcherGUIApp();

    const GUCEF::CORE::CValueList& GetCommandLineParams( void ) const;
    
    CFrameWnd* GetMainFrame( void );
    
    public:
	
	virtual BOOL InitInstance();

    public:
	
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
    private:
    
    GUCEF::CORE::CValueList m_cmdLineParamList;
};

extern CMFCPatcherGUIApp theApp;

#endif /* CMFCPATCHERGUIAPP_H ? */