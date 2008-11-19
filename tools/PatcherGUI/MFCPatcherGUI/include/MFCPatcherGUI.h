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

#ifndef GUCEF_CORE_CSTDLOGGER_H
#include "CStdLogger.h"
#define GUCEF_CORE_CSTDLOGGER_H
#endif /* GUCEF_CORE_CSTDLOGGER_H ? */

#ifndef GUCEF_PATCHER_MACROS_H
#include "gucefPATCHER_macros.h"
#define GUCEF_PATCHER_MACROS_H
#endif /* GUCEF_PATCHER_MACROS_H ? */

#if defined( GUCEF_MSWIN_BUILD ) && defined( GUCEF_PATCHER_DEBUG_MODE )
#include "CMSWinConsoleLogger.h"
#endif /* GUCEF_MSWIN_BUILD && GUCEF_PATCHER_DEBUG_MODE ? */

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
    GUCEF::CORE::CStdLogger m_logger;
    #if defined( GUCEF_MSWIN_BUILD ) && defined( GUCEF_PATCHER_DEBUG_MODE )
    GUCEF::CORE::CMSWinConsoleLogger m_consoleOut;
    #endif /* GUCEF_MSWIN_BUILD && GUCEF_PATCHER_DEBUG_MODE ? */
};

extern CMFCPatcherGUIApp theApp;

#endif /* CMFCPATCHERGUIAPP_H ? */