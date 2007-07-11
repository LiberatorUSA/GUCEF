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
      m_initialized( false )                                 ,
      m_useTimer( false )
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
        if ( Create( NULL                                     ,
                     (LPCTSTR)"CGUCEFAppWin32MFCDriverWindow" ,
                     WS_CHILD|WS_VISIBLE                      ,
                     CRect(0, 0, 0, 0)                        ,
                     pParentWnd                               ,
                     1234                                     ) )
        {    
            m_initialized = true;
            m_appPtr->SetApplicationDriver( this );
            
            // Give the first update pulse
            OnRequestNewUpdateCycle();
            return true;
        }
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void 
CGUCEFAppWin32MFCDriver::OnRequestNewMinimalUpdateFreq( const GUCEF::CORE::Float64 updateDeltaInMilliSecs )
{TRACE;

    m_frequency = updateDeltaInMilliSecs;
    if ( m_appPtr->GetRequiresPeriodicUpdate() )
    {
        /*
         *  Reset the timer with the new timer interval
         */
        m_useTimer = true;
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
        m_nTimer = (UINT) SetTimer( GUCEFWIN32DRIVERMSG_UPDATE, (DWORD)m_frequency, 0 );
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
    m_useTimer = false;
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
        // don't start the timer twice
        if ( m_useTimer )
        {
            return;
        }
        
        // Get the update frequency desired and post the timer start message
        m_frequency = m_appPtr->GetMinimalReqUpdateResolution();
        if ( GetSafeHwnd() )
        {
            PostMessage( GUCEFWIN32DRIVERMSG_STARTTIMER );
        }
        m_useTimer = true;
    }
    else
    {
        if ( GetSafeHwnd() )
        {
            PostMessage( GUCEFWIN32DRIVERMSG_STOPTIMER );
        }
        m_useTimer = false;
    }
}

/*-------------------------------------------------------------------------*/