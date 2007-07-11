/*
 *  MFCPatcherGUI: GUI shell for the gucefPATCHER module
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