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
#include "MFCPatcherGUI.h"
#include "CPatcherApplication.h"
#include "gucefPATCHER_CStandardPSPEventHandler.h"
#include "CGUCEFAppWin32MFCDriver.h"
#include "CGUCEFApplication.h"
#include "ChildView.h"

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CChildView::CChildView()
    : m_listBox( NULL )          ,
      m_transferProgress( NULL ) ,
      m_totalProgress( NULL )    ,
      m_standardHandler( NULL )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CChildView::~CChildView()
{TRACE;

}

/*-------------------------------------------------------------------------*/


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*-------------------------------------------------------------------------*/

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{TRACE;

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

/*-------------------------------------------------------------------------*/

void CChildView::DoDataExchange( CDataExchange* pDX )
{TRACE;

	CWnd::DoDataExchange( pDX );

	//{{AFX_DATA_MAP(CPatcherGUIDlg)
//	DDX_Control( pDX, IDC_LISTBOX, *m_listBox );
//	DDX_Control( pDX, IDC_TRANSFERPROGRESS, *m_transferProgress );
//	DDX_Control( pDX, IDC_TOTALPROGRESS, *m_totalProgress );
	//}}AFX_DATA_MAP
	
}

/*-------------------------------------------------------------------------*/

void CChildView::OnPaint() 
{TRACE;

	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}

/*-------------------------------------------------------------------------*/

int CChildView::OnCreate(LPCREATESTRUCT lpcs)
{TRACE;

    CWnd::OnCreate( lpcs );

    m_gucefDriver = new CGUCEFAppWin32MFCDriver();
    if ( m_gucefDriver->Init( this ) )
    {
        m_listBox = new CListBox();
        m_listBox->Create( WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_HSCROLL , 
                           CRect( 10, 10, 200, 200 )                   ,
                           this                                        , 
                           1                                           );

        m_standardHandler = new CStandardPSPEventHandler();

        TEventHandlerList handlerList;
        handlerList.push_back( this );
        handlerList.push_back( m_standardHandler );
        CPatcherApplication::Instance()->SetEventHandlers( handlerList );
        
        CGUCEFApplication::Instance()->SetApplicationDriver( m_gucefDriver );
        
        return 0;
    }
    
    return 1;
}

/*-------------------------------------------------------------------------*/

void CChildView::OnSize(UINT nType, int cx, int cy)
{TRACE;

    m_listBox->MoveWindow(0, 0, cx, 100);
}

/*-------------------------------------------------------------------------*/

void
CChildView::OnPatchSetStart( const CORE::CString& patchSetName )
{TRACE;
    
    CORE::CString infoStr( ">>> start patchset: " );
    infoStr += patchSetName;
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CChildView::OnEnterLocalDir( const CORE::CString& localPath )
{TRACE;

    CORE::CString infoStr( "entering local directory: " );
    infoStr += localPath;
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CChildView::OnLocalFileOK( const CORE::CString& localPath ,
                           const CORE::CString& localFile )
{TRACE;

    CORE::CString infoStr( "local file \"" );
    infoStr += localFile;
    infoStr += "\" is up-to-date. Path: ";
    infoStr += localPath;
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/

void
CChildView::OnLocalFileNotFound( const CORE::CString& localPath ,
                                 const CORE::CString& localFile )
{TRACE;

    CORE::CString infoStr( "local file \"" );
    infoStr += localFile;
    infoStr += "\" is not found. Path: ";
    infoStr += localPath;
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/

void
CChildView::OnLocalFileDifference( const CORE::CString& localPath ,
                                   const CORE::CString& localFile )
{TRACE;

    CORE::CString infoStr( "local file \"" );
    infoStr += localFile;
    infoStr += "\" differs from the master file. Path: ";
    infoStr += localPath;
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/

void
CChildView::OnNewSourceRequired( const TSourceInfo& sourceInfo )
{TRACE;

    CORE::CString infoStr( "new source required" );
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CChildView::OnLeaveLocalDir( const CORE::CString& localPath )
{TRACE;

    CORE::CString infoStr( "leaving local directory: " );
    infoStr += localPath;
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CChildView::OnPatchSetEnd( const CORE::CString& patchSetName )
{TRACE;

    CORE::CString infoStr( ">>> end patchset: " );
    infoStr += patchSetName;
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/

void
CChildView::OnParserError( void )
{TRACE;

    CORE::CString infoStr( ">>> parser error" );
    m_listBox->AddString( (LPCTSTR) infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
