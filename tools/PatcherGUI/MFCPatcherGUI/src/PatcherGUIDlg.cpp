// PatcherGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatcherGUI.h"
#include "PatcherGUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPatcherGUIDlg dialog

using namespace GUCEF;


/*-------------------------------------------------------------------------*/

CPatcherGUIDlg::CPatcherGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatcherGUIDlg::IDD, pParent)
{TRACE;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/*-------------------------------------------------------------------------*/

void CPatcherGUIDlg::DoDataExchange( CDataExchange* pDX )
{TRACE;

	CDialog::DoDataExchange( pDX );

	//{{AFX_DATA_MAP(CPatcherGUIDlg)
	DDX_Control( pDX, IDC_LISTBOX, m_listBox );
	DDX_Control( pDX, IDC_TRANSFERPROGRESS, m_transferProgress );
	DDX_Control( pDX, IDC_TOTALPROGRESS, m_totalProgress );
	//}}AFX_DATA_MAP
	
}

/*-------------------------------------------------------------------------*/

BEGIN_MESSAGE_MAP(CPatcherGUIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*-------------------------------------------------------------------------*/

// CPatcherGUIDlg message handlers

BOOL 
CPatcherGUIDlg::OnInitDialog()
{TRACE;

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/*-------------------------------------------------------------------------*/

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void 
CPatcherGUIDlg::OnPaint()
{TRACE;
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

/*-------------------------------------------------------------------------*/

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR 
CPatcherGUIDlg::OnQueryDragIcon()
{TRACE;

	return static_cast<HCURSOR>(m_hIcon);
}

/*-------------------------------------------------------------------------*/

void
CPatcherGUIDlg::OnPatchSetStart( const CORE::CString& patchSetName )
{TRACE;
    
    CORE::CString infoStr( ">>> start patchset: " );
    infoStr += patchSetName;
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CPatcherGUIDlg::OnEnterLocalDir( const CORE::CString& localPath )
{TRACE;

    CORE::CString infoStr( "entering local directory: " );
    infoStr += localPath;
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CPatcherGUIDlg::OnLocalFileOK( const CORE::CString& localPath ,
                               const CORE::CString& localFile )
{TRACE;

    CORE::CString infoStr( "local file \"" );
    infoStr += localFile;
    infoStr += "\" is up-to-date. Path: ";
    infoStr += localPath;
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/

void
CPatcherGUIDlg::OnLocalFileNotFound( const CORE::CString& localPath ,
                                     const CORE::CString& localFile )
{TRACE;

    CORE::CString infoStr( "local file \"" );
    infoStr += localFile;
    infoStr += "\" is not found. Path: ";
    infoStr += localPath;
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/

void
CPatcherGUIDlg::OnLocalFileDifference( const CORE::CString& localPath ,
                                       const CORE::CString& localFile )
{TRACE;

    CORE::CString infoStr( "local file \"" );
    infoStr += localFile;
    infoStr += "\" differs from the master file. Path: ";
    infoStr += localPath;
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/

void
CPatcherGUIDlg::OnNewSourceRequired( const TSourceInfo& sourceInfo )
{TRACE;

    CORE::CString infoStr( "new source required" );
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CPatcherGUIDlg::OnLeaveLocalDir( const CORE::CString& localPath )
{TRACE;

    CORE::CString infoStr( "leaving local directory: " );
    infoStr += localPath;
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/
    
void
CPatcherGUIDlg::OnPatchSetEnd( const CORE::CString& patchSetName )
{TRACE;

    CORE::CString infoStr( ">>> end patchset: " );
    infoStr += patchSetName;
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/

void
CPatcherGUIDlg::OnParserError( void )
{TRACE;

    CORE::CString infoStr( ">>> parser error" );
    m_listBox.AddString( infoStr.C_String() );
}

/*-------------------------------------------------------------------------*/