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

    virtual void OnNotify( GUCEF::CORE::CNotifier* notifier           ,
                           const GUCEF::CORE::CEvent& eventid         ,
                           GUCEF::CORE::CICloneable* eventdata = NULL );

    private:

    CListBox* m_listBox;
    CProgressCtrl* m_transferProgress;    
    CProgressCtrl* m_totalProgress;
    
    GUCEF::PATCHER::CPatchEngine m_patchEngine;
    CGUCEFAppWin32MFCDriver* m_gucefDriver;   
    
};

#endif /* MFCPATCHERGUI_CCHILDVIEW_H ? */

