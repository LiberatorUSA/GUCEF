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

#ifndef MFCPATCHERGUI_CCHILDVIEW_H
#define MFCPATCHERGUI_CCHILDVIEW_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdafx.h>

#ifndef GUCEF_CORE_COBSERVER_H
#include "CObserver.h"
#define GUCEF_CORE_COBSERVER_H
#endif /* GUCEF_CORE_COBSERVER_H ? */

#ifndef GUCEF_PATCHER_CPATCHENGINE_H
#include "gucefPATCHER_CPatchEngine.h"
#define GUCEF_PATCHER_CPATCHENGINE_H
#endif /* GUCEF_PATCHER_CPATCHENGINE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { namespace PATCHER { class CStandardPSPEventHandler; } }
class CGUCEFAppWin32MFCDriver;

class CChildView : public CWnd                    ,
                   private GUCEF::CORE::CObserver 
{
    // Construction
    public:
    CChildView();

    // Attributes
    public:

    // Operations
    public:

    // Overrides
    protected:
    virtual int OnCreate(LPCREATESTRUCT lpcs);
    virtual void OnSize(UINT nType, int cx, int cy);    	
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    // Implementation
    public:
    virtual ~CChildView();

    // Generated message map functions
    protected:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()

    private:
    /*
    virtual void OnPatchSetStart( const GUCEF::CORE::CString& patchSetName );

    virtual void OnEnterLocalDir( const GUCEF::CORE::CString& localPath );

    virtual void OnLocalFileOK( const GUCEF::CORE::CString& localPath ,
                                const GUCEF::CORE::CString& localFile );

    virtual void OnLocalFileNotFound( const GUCEF::CORE::CString& localPath ,
                                      const GUCEF::CORE::CString& localFile );

    virtual void OnLocalFileDifference( const GUCEF::CORE::CString& localPath ,
                                        const GUCEF::CORE::CString& localFile );

    virtual void OnNewSourceRequired( const TSourceInfo& sourceInfo );

    virtual void OnLeaveLocalDir( const GUCEF::CORE::CString& localPath );

    virtual void OnPatchSetEnd( const GUCEF::CORE::CString& patchSetName );

    virtual void OnParserError( void );   */

    protected:

    virtual void OnNotify( GUCEF::CORE::CNotifier* notifier           ,
                           const GUCEF::CORE::CEvent& eventid         ,
                           GUCEF::CORE::CICloneable* eventdata = NULL );

    private:
    
    bool LoadPatchEngineConfig( void );
    
    private:

    CListBox* m_listBox;
    CProgressCtrl* m_transferProgress;    
    CProgressCtrl* m_totalProgress;
    bool m_closeAppWhenDone;
    
    GUCEF::PATCHER::CPatchEngine m_patchEngine;
    CGUCEFAppWin32MFCDriver* m_gucefDriver;   
    
};

#endif /* MFCPATCHERGUI_CCHILDVIEW_H ? */

