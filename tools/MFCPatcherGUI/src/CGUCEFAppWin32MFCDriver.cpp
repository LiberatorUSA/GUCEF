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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"                  /* Main application object for GUCEF applications */
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#include "CGUCEFAppWin32MFCDriver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEFWIN32DRIVERMSG_STARTTIMER (WM_USER+164)
#define GUCEFWIN32DRIVERMSG_STOPTIMER  (WM_USER+165)
#define GUCEFWIN32DRIVERMSG_UPDATE     (WM_USER+163)

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

BEGIN_MESSAGE_MAP( CGUCEFAppWin32MFCDriver, CWnd )
	//{{AFX_MSG_MAP(CXBAppWin32Driver)
    ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_MESSAGE( GUCEFWIN32DRIVERMSG_STARTTIMER, OnStartUpdateTimer )
    ON_MESSAGE( GUCEFWIN32DRIVERMSG_STOPTIMER, OnStopUpdateTimer )
    ON_MESSAGE( GUCEFWIN32DRIVERMSG_UPDATE, OnGUCEFUpdateRequestMsg )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUCEFAppWin32MFCDriver::CGUCEFAppWin32MFCDriver( void )
    : m_appPtr( GUCEF::CORE::CGUCEFApplication::Instance() ) ,
      m_nTimer( 0 )                                          ,
      m_frequency( 10 )                                      ,
      m_initialized( false )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CGUCEFAppWin32MFCDriver::~CGUCEFAppWin32MFCDriver()
{TRACE;

    if ( GetSafeHwnd() )
    {
        OnStopUpdateTimer( 0, 0 );
        DestroyWindow();
    }
    m_appPtr->SetApplicationDriver( NULL );    
}

/*-------------------------------------------------------------------------*/

bool
CGUCEFAppWin32MFCDriver::Init( CWnd* pParentWnd )
{TRACE;

    if ( !m_initialized )
    {
        if ( Create( NULL                            ,
                     "CGUCEFAppWin32MFCDriverWindow" ,
                     WS_CHILD|WS_VISIBLE             ,
                     CRect(0, 0, 0, 0)               ,
                     pParentWnd                      ,
                     1234                            ) )
        {    
            m_initialized = true;
            m_appPtr->SetApplicationDriver( this );
            return true;
        }
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void 
CGUCEFAppWin32MFCDriver::OnRequestNewMinimalUpdateFreq( const GUCEF::CORE::UInt32 frequency )
{TRACE;

    m_frequency = frequency;
    if ( m_appPtr->GetRequiresPeriodicUpdate() )
    {
        /*
         *  Reset the timer with the new timer interval
         */
        if ( GetSafeHwnd() )
        {
            PostMessage( GUCEFWIN32DRIVERMSG_STARTTIMER );
        }
    }
}

/*-------------------------------------------------------------------------*/

void 
CGUCEFAppWin32MFCDriver::OnRequestNewUpdateCycle( void )
{TRACE;

    if ( GetSafeHwnd() )
    {
        PostMessage( GUCEFWIN32DRIVERMSG_UPDATE );
    }
}

/*-------------------------------------------------------------------------*/

afx_msg LRESULT
CGUCEFAppWin32MFCDriver::OnGUCEFUpdateRequestMsg( WPARAM wParam , 
                                                  LPARAM lParam )
{TRACE;

    GUCEF::CORE::CGUCEFApplication::Instance()->Update();
    return 0;
}

/*-------------------------------------------------------------------------*/

afx_msg LRESULT 
CGUCEFAppWin32MFCDriver::OnStartUpdateTimer( WPARAM wParam , 
                                             LPARAM lParam )
{TRACE;

    if ( GetSafeHwnd() )
    {
        if ( m_nTimer > 0 )
        {
            KillTimer( m_nTimer );
        }
        m_nTimer = SetTimer( GUCEFWIN32DRIVERMSG_UPDATE, m_frequency, 0 );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

afx_msg LRESULT 
CGUCEFAppWin32MFCDriver::OnStopUpdateTimer( WPARAM wParam , 
                                            LPARAM lParam )
{TRACE;

    if ( GetSafeHwnd() )
    {    
        if ( m_nTimer )
        {
            KillTimer( m_nTimer );
            m_nTimer = 0;
        }
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CGUCEFAppWin32MFCDriver::OnClose()
{TRACE;

    if ( GetSafeHwnd() )
    {
        PostMessage( GUCEFWIN32DRIVERMSG_STOPTIMER );
    }
    m_appPtr->SetApplicationDriver( NULL );
    CWnd::OnClose();
}

/*-------------------------------------------------------------------------*/

afx_msg void
CGUCEFAppWin32MFCDriver::OnTimer( UINT nIDEvent ) 
{TRACE;

    if ( nIDEvent == GUCEFWIN32DRIVERMSG_UPDATE )
    {
        if ( GetSafeHwnd() )
        {
            m_appPtr->Update();
        }
    }
    CWnd::OnTimer( nIDEvent );
}

/*-------------------------------------------------------------------------*/

void 
CGUCEFAppWin32MFCDriver::OnSwitchUpdateMethod( const bool periodic )
{TRACE;

    if ( periodic )
    {
        m_frequency = m_appPtr->GetMinimalReqUpdateResolution();
        if ( GetSafeHwnd() )
        {
            PostMessage( GUCEFWIN32DRIVERMSG_STARTTIMER );
        }
    }
    else
    {
        if ( GetSafeHwnd() )
        {
            PostMessage( GUCEFWIN32DRIVERMSG_STOPTIMER );
        }
    }
}

/*-------------------------------------------------------------------------*/