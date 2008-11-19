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
 
#ifndef MFCPATCHERGUI_CPATCHERGUIDLG_H
#define MFCPATCHERGUI_CPATCHERGUIDLG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdafx.h>

#ifndef GUCEF_PATCHER_CPATCHSETPARSEREVENTHANDLER_H
#include "gucefPATCHER_CPatchSetParserEventHandler.h"
#define GUCEF_PATCHER_CPATCHSETPARSEREVENTHANDLER_H
#endif /* GUCEF_PATCHER_CPATCHSETPARSEREVENTHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

// CPatcherGUIDlg dialog
class CPatcherGUIDlg : public CDialog                                      ,
                       private GUCEF::PATCHER::CPatchSetParserEventHandler
{
// Construction
public:
	CPatcherGUIDlg(CWnd* pParent = NULL);	// standard constructor
   
// Dialog Data
	enum { IDD = IDD_PATCHERGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
private:
    
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
    
    virtual void OnParserError( void );	
    
private:

    CListBox m_listBox;
    CProgressCtrl m_transferProgress;    
    CProgressCtrl m_totalProgress;
};

#endif /* MFCPATCHERGUI_CPATCHERGUIDLG_H ? */
