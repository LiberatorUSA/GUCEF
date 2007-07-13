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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "stdafx.h"
#include <assert.h>
#include "MFCPatcherGUI.h"

#ifndef MFCWIN32_CGUCEFAPPWIN32MFCDRIVER_H
#include "CGUCEFAppWin32MFCDriver.h"
#define MFCWIN32_CGUCEFAPPWIN32MFCDRIVER_H
#endif /* MFCWIN32_CGUCEFAPPWIN32MFCDRIVER_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#include "DVOSWRAP.h"

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
      m_patchEngine()            ,
      m_gucefDriver( NULL )      ,
      m_closeAppWhenDone( false )
{GUCEF_TRACE;

    // Subscribe to all patch engine events
    m_patchEngine.Subscribe( this );
    
    // Subscribe the patch engine to the GUCEF application object
    CORE::CGUCEFApplication* gucefApp = CORE::CGUCEFApplication::Instance();
    m_patchEngine.SubscribeTo( gucefApp, CORE::CGUCEFApplication::AppInitEvent );
    m_patchEngine.SubscribeTo( gucefApp, CORE::CGUCEFApplication::AppShutdownEvent );
    
    // Set the application startup and shutdown events as triggers
    m_patchEngine.AddEngineStartTriggerEvent( CORE::CGUCEFApplication::AppInitEvent );
    m_patchEngine.AddEngineStopTriggerEvent( CORE::CGUCEFApplication::AppShutdownEvent );
}

/*-------------------------------------------------------------------------*/

CChildView::~CChildView()
{GUCEF_TRACE;

    UnsubscribeFromAll();
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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

	CWnd::DoDataExchange( pDX );

	//{{AFX_DATA_MAP(CPatcherGUIDlg)
//	DDX_Control( pDX, IDC_LISTBOX, *m_listBox );
//	DDX_Control( pDX, IDC_TRANSFERPROGRESS, *m_transferProgress );
//	DDX_Control( pDX, IDC_TOTALPROGRESS, *m_totalProgress );
	//}}AFX_DATA_MAP
	
}

/*-------------------------------------------------------------------------*/

void CChildView::OnPaint() 
{GUCEF_TRACE;

	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}

/*-------------------------------------------------------------------------*/

bool
CChildView::LoadPatchEngineConfig( void )
{GUCEF_TRACE;

    // Check for mandatory command line params
    if ( !theApp.GetCommandLineParams().HasKey( "ConfigFile" ) )
    {
        // we need command line params
        return false;
    }
    if ( !theApp.GetCommandLineParams().HasKey( "ConfigFileCodec" ) )
    {
        // we need command line params
        return false;
    }    
    
    m_closeAppWhenDone = GUCEF::CORE::StringToBool( theApp.GetCommandLineParams().GetValue( "CloseWhenDone" ) );
    
    GUCEF::CORE::CString configFile = theApp.GetCommandLineParams().GetValue( "ConfigFile" );    
    configFile = GUCEF::CORE::RelativePath( configFile );
    GUCEF::CORE::CString configFileCodec = theApp.GetCommandLineParams().GetValue( "ConfigFileCodec" );
    if ( ( configFile.Length() == 0 )     ||
         ( configFileCodec.Length() == 0 ) )
    {
        // We need actual values
        return false;
    }
    
    // Load the config for the patch engine
    GUCEF::CORE::CDStoreCodecRegistry* codecRegistry = GUCEF::CORE::CDStoreCodecRegistry::Instance();
    if ( codecRegistry->IsRegistered( configFileCodec ) )
    {
        GUCEF::CORE::CDataNode configData;
        GUCEF::CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec = codecRegistry->Lookup( configFileCodec );
        if ( codec->BuildDataTree( &configData ,
                                   configFile  ) )
        {
            return m_patchEngine.LoadConfig( configData );
        }
        else
        {
            // The configuration data was not as required by the patch engine
            return false;
        }
    }
    else
    {   
        // The requested codec is not available, this is a fatal error
        return false;
    }
}

/*-------------------------------------------------------------------------*/

int CChildView::OnCreate(LPCREATESTRUCT lpcs)
{GUCEF_TRACE;

    if ( !LoadPatchEngineConfig() )
    {
        GUCEF::CORE::ShowErrorMessage( "Init error", "Failed to load patch engine config" );
        PostQuitMessage( 0 );
        return 1;
    }

    // Hook the GUCEF application driver into the window message loop
    m_gucefDriver = new CGUCEFAppWin32MFCDriver();
    assert( m_gucefDriver );
    if ( m_gucefDriver->Init( this ) )
    {
        // Create our listbox
        m_listBox = new CListBox();
        assert( m_listBox );
        m_listBox->Create( WS_CHILD|WS_VISIBLE|LBS_STANDARD|WS_HSCROLL , 
                           CRect( 10, 40, 460, 200 )                   ,
                           this                                        , 
                           1                                           );
   
      /*  CFont listFont;
        if ( TRUE == listFont.CreatePointFont( 60, "Arial" ) )
        {
            m_listBox->SetFont( &listFont, TRUE );
        } */
        
        m_listBox->SetFont( GetFont() );
        
        m_transferProgress = new CProgressCtrl();
        assert( m_transferProgress );
        m_transferProgress->Create( WS_CHILD|WS_VISIBLE      , 
                                    CRect( 10, 10, 460, 20 ) ,
                                    this                     , 
                                    1                        );
        m_totalProgress = new CProgressCtrl();
        assert( m_totalProgress );
        m_totalProgress->Create( WS_CHILD|WS_VISIBLE      , 
                                 CRect( 10, 30, 460, 40 ) ,
                                 this                     , 
                                 1                        );        
        
        // Now connect the application driver with GUCEF, from this point on things can start
        // happening behind the screen
        CORE::CGUCEFApplication::Instance()->SetApplicationDriver( m_gucefDriver );
        
        // Start the application from the GUCEF point of view
        CORE::CGUCEFApplication::Instance()->Main( AfxGetInstanceHandle() ,
                                                   NULL                   ,
                                                   0                      ,
                                                   false                  );
        return 0;                                                  
    }
    
    GUCEF::CORE::ShowErrorMessage( "Init error", "Failed to initialize the GUCEF application driver" );
    PostQuitMessage( 0 );
    return 1;
}

/*-------------------------------------------------------------------------*/

void CChildView::OnSize(UINT nType, int cx, int cy)
{GUCEF_TRACE;

  //  MoveWindow( 0, 0, 480, 160 );
  //  m_listBox->MoveWindow(0, 0, cx, 100);
}

/*-------------------------------------------------------------------------*/
//
//void
//CChildView::OnPatchSetStart( const CORE::CString& patchSetName )
//{GUCEF_TRACE;
//    
//    CORE::CString infoStr( ">>> start patchset: " );
//    infoStr += patchSetName;
//    m_listBox->AddString( infoStr.C_String() );
//}
//
///*-------------------------------------------------------------------------*/
//    
//void
//CChildView::OnEnterLocalDir( const CORE::CString& localPath )
//{GUCEF_TRACE;
//
//    CORE::CString infoStr( "entering local directory: " );
//    infoStr += localPath;
//    m_listBox->AddString( infoStr.C_String() );
//}
//
///*-------------------------------------------------------------------------*/
//    
//void
//CChildView::OnLocalFileOK( const CORE::CString& localPath ,
//                           const CORE::CString& localFile )
//{GUCEF_TRACE;
//
//    CORE::CString infoStr( "local file \"" );
//    infoStr += localFile;
//    infoStr += "\" is up-to-date. Path: ";
//    infoStr += localPath;
//    m_listBox->AddString( infoStr.C_String() );
//}
//
///*-------------------------------------------------------------------------*/
//
//void
//CChildView::OnLocalFileNotFound( const CORE::CString& localPath ,
//                                 const CORE::CString& localFile )
//{GUCEF_TRACE;
//
//    CORE::CString infoStr( "local file \"" );
//    infoStr += localFile;
//    infoStr += "\" is not found. Path: ";
//    infoStr += localPath;
//    m_listBox->AddString( infoStr.C_String() );
//}
//
///*-------------------------------------------------------------------------*/
//
//void
//CChildView::OnLocalFileDifference( const CORE::CString& localPath ,
//                                   const CORE::CString& localFile )
//{GUCEF_TRACE;
//
//    CORE::CString infoStr( "local file \"" );
//    infoStr += localFile;
//    infoStr += "\" differs from the master file. Path: ";
//    infoStr += localPath;
//    m_listBox->AddString( infoStr.C_String() );
//}
//
///*-------------------------------------------------------------------------*/
//
//void
//CChildView::OnNewSourceRequired( const TSourceInfo& sourceInfo )
//{GUCEF_TRACE;
//
//    CORE::CString infoStr( "new source required" );
//    m_listBox->AddString( infoStr.C_String() );
//}
//
///*-------------------------------------------------------------------------*/
//    
//void
//CChildView::OnLeaveLocalDir( const CORE::CString& localPath )
//{GUCEF_TRACE;
//
//    CORE::CString infoStr( "leaving local directory: " );
//    infoStr += localPath;
//    m_listBox->AddString( infoStr.C_String() );
//}
//
///*-------------------------------------------------------------------------*/
//    
//void
//CChildView::OnPatchSetEnd( const CORE::CString& patchSetName )
//{GUCEF_TRACE;
//
//    CORE::CString infoStr( ">>> end patchset: " );
//    infoStr += patchSetName;
//    m_listBox->AddString( infoStr.C_String() );
//}
//
///*-------------------------------------------------------------------------*/
//
//void
//CChildView::OnParserError( void )
//{GUCEF_TRACE;
//
//    CORE::CString infoStr( ">>> parser error" );
//    m_listBox->AddString( infoStr.C_String() );
//}

/*-------------------------------------------------------------------------*/

void
CChildView::OnNotify( CORE::CNotifier* notifier                 ,
                      const CORE::CEvent& eventid               ,
                      CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( notifier == &m_patchEngine )
    {
        if ( eventid == PATCHER::CPatchEngine::PatchProcessStartedEvent )
        {
            m_listBox->AddString( "Patch process starting..." );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchProcessFailedEvent )
        {
            m_listBox->AddString( "ERROR: Patch process failed" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchProcessCompletedEvent )
        {
            m_listBox->AddString( "Completed the patch process" );
            
            if ( m_closeAppWhenDone )
            {
                PostQuitMessage( 0 );
            }
        }        
        else
        if ( eventid == PATCHER::CPatchEngine::PatchListRetrievalStartedEvent )
        {
            m_listBox->AddString( "Retrieving patch list..." );
        }
        else        
        if ( eventid == PATCHER::CPatchEngine::PatchListRetrievalFailedEvent )
        {
            m_listBox->AddString( "ERROR: Failed to retrieve the patch list" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchListRetrievalCompletedEvent )
        {
            m_listBox->AddString( "Completed patch list retrieval" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchListProcessingStartedEvent )
        {
            m_listBox->AddString( "Patch list processing started..." );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchListProcessingCompletedEvent )
        {
            m_listBox->AddString( "Completed processing the patch list" );
        }        
        else        
        if ( eventid == PATCHER::CPatchEngine::PatchListDataReceivedEvent )
        {
            m_listBox->AddString( "Received patch list data" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchListRetrievalAbortedEvent )
        {
            m_listBox->AddString( "Aborted patch list retrieval" );
        } 
        else
        if ( eventid == PATCHER::CPatchEngine::PatchListDecodingFailedEvent )
        {
            m_listBox->AddString( "ERROR: Failed to decode the patch list" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchSetProcessingStartedEvent )
        {
            m_listBox->AddString( "Started processing patch set..." );
        }        
        else
        if ( eventid == PATCHER::CPatchEngine::PatchSetRetrievalFailedEvent )
        {
            m_listBox->AddString( "ERROR: Failed to retrieve the patch set" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchSetDataRecievedEvent )
        {
            m_listBox->AddString( "Received patch set data" );
        }
        else        
        if ( eventid == PATCHER::CPatchEngine::PatchSetRetrievalAbortedEvent )
        {
            m_listBox->AddString( "Aborted patch set retrieval" );
        }  
        else        
        if ( eventid == PATCHER::CPatchEngine::PatchSetRetrievalStartedEvent )
        {
            m_listBox->AddString( "Retrieving patch set..." );
        }  
        else        
        if ( eventid == PATCHER::CPatchEngine::PatchSetRetrievalCompletedEvent )
        {
            m_listBox->AddString( "Completed patch set retrieval" );
        }  
        else        
        if ( eventid == PATCHER::CPatchEngine::PatchSetProcessingCompletedEvent )
        {
            m_listBox->AddString( "Completed processing the patch set" );
        }        
        else
        if ( eventid == PATCHER::CPatchEngine::PatchSetDecodingFailedEvent )
        {
            m_listBox->AddString( "ERROR: Failed to decode a patch set" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::PatchProcessAbortedEvent )
        {
            m_listBox->AddString( "The patching process has been aborted" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::DirProcessingStartedEvent )
        {
            m_listBox->AddString( "Started processing directory..." );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::SubDirProcessingCompletedEvent )
        {
            m_listBox->AddString( "Completed processing sub directories" );
        }        
        else        
        if ( eventid == PATCHER::CPatchEngine::FileRetrievalStartedEvent )
        {
            m_listBox->AddString( "Retrieving file..." );
        }                 
        else        
        if ( eventid == PATCHER::CPatchEngine::FileListProcessingStartedEvent )
        {
            m_listBox->AddString( "Started processing file list..." );
        }
        else        
        if ( eventid == PATCHER::CPatchEngine::FileListProcessingCompleteEvent )
        {
            m_listBox->AddString( "Completed file list processing" );
        }                 
        else
        if ( eventid == PATCHER::CPatchEngine::FileListProcessingAbortedEvent )
        {
            m_listBox->AddString( "Aborted file list processing" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::LocalFileIsOKEvent )
        {
            m_listBox->AddString( "Local file is Ok" );
        }         
        else
        if ( eventid == PATCHER::CPatchEngine::LocalFileSizeMismatchEvent )
        {
            m_listBox->AddString( "Local file size mismatch" );
        }         
        else
        if ( eventid == PATCHER::CPatchEngine::LocalFileHashMismatchEvent )
        {
            m_listBox->AddString( "Local file hash mismatch" );
        }        
        else
        if ( eventid == PATCHER::CPatchEngine::LocalFileNotFoundEvent )
        {
            m_listBox->AddString( "Local file not found" );
        }
        else
        if ( eventid == PATCHER::CPatchEngine::LocalFileReplacedEvent )
        {
            m_listBox->AddString( "Local file replaced" );
        }
        else
        {
            if ( ( NULL != m_listBox )                            &&
                 ( eventid != CORE::CNotifier::DestructionEvent ) &&
                 ( eventid != CORE::CNotifier::UnsubscribeEvent )  )
            {
                CORE::CString eventStr( "Event: " );
                eventStr += eventid.GetName();
                m_listBox->AddString( eventStr.C_String() );
            }
        }
    }
}

/*-------------------------------------------------------------------------*/
