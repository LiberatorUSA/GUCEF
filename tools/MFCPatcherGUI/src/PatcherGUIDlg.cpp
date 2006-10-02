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


CPatcherGUIDlg::CPatcherGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatcherGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPatcherGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPatcherGUIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CPatcherGUIDlg message handlers

BOOL CPatcherGUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPatcherGUIDlg::OnPaint()
{
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPatcherGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void
CPatcherGUIDlg::OnPatchSetStart( const CORE::CString& patchSetName )
{
}

/*-------------------------------------------------------------------------*/
    
void
CPatcherGUIDlg::OnEnterLocalDir( const CORE::CString& localPath )
{
}

/*-------------------------------------------------------------------------*/
    
void
CPatcherGUIDlg::OnLocalFileOK( const CORE::CString& localPath ,
                               const CORE::CString& localFile )
{
}

/*-------------------------------------------------------------------------*/

void
CPatcherGUIDlg::OnLocalFileNotFound( const CORE::CString& localPath ,
                                     const CORE::CString& localFile )
{
}

/*-------------------------------------------------------------------------*/

void
CPatcherGUIDlg::OnLocalFileDifference( const CORE::CString& localPath ,
                                       const CORE::CString& localFile )
{
}

/*-------------------------------------------------------------------------*/

void
CPatcherGUIDlg::OnNewSourceRequired( const TSourceInfo& sourceInfo )
{
}

/*-------------------------------------------------------------------------*/
    
void
CPatcherGUIDlg::OnLeaveLocalDir( const CORE::CString& localPath )
{
}

/*-------------------------------------------------------------------------*/
    
void
CPatcherGUIDlg::OnPatchSetEnd( const CORE::CString& patchSetName )
{
}

/*-------------------------------------------------------------------------*/