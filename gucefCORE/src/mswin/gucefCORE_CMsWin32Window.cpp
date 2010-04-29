/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_CMsWin32Window.h"

#ifdef GUCEF_MSWIN_BUILD

#ifndef GUCEF_CORE_MSWINUTILS_H
#include "gucefCORE_mswinutils.h"
#define GUCEF_CORE_MSWINUTILS_H
#endif /* GUCEF_CORE_MSWINUTILS_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "CGUCEFApplication.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CEvent CMsWin32Window::WindowCloseEvent = "GUCEF::CORE::CMsWin32Window::WindowCloseEvent";
const CEvent CMsWin32Window::WindowDestroyEvent = "GUCEF::CORE::CMsWin32Window::WindowDestroyEvent";
const CEvent CMsWin32Window::WindowActivationEvent = "GUCEF::CORE::CMsWin32Window::WindowActivationEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CMsWin32Window::RegisterEvents( void )
{GUCEF_TRACE;

    WindowCloseEvent.Initialize();    
    WindowDestroyEvent.Initialize();
    WindowActivationEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CMsWin32Window::CMsWin32Window( void )
    : m_hwnd( 0 ) ,
      m_orgWinProc( 0 )
{GUCEF_TRACE;

    SubscribeTo( &CGUCEFApplication::Instance()->GetPulseGenerator(), CPulseGenerator::PulseEvent );

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CMsWin32Window::~CMsWin32Window()
{GUCEF_TRACE;

    SetHwnd( 0 );
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Window::WindowDestroy( void )
{
    SetHwnd( 0 );
}

/*-------------------------------------------------------------------------*/

LRESULT
CMsWin32Window::WindowProc( const HWND hWnd     ,
                            const UINT nMsg     ,
                            const WPARAM wParam ,
                            const LPARAM lParam )
{GUCEF_TRACE;

    switch( nMsg )
    {
        //case WM_COMMAND:
        //{
        //    CMsWin32Window* child = (CMsWin32Window*) LOWORD( wParam );
        //    
        //    // Check if this is a text changed notification
        //    if ( HIWORD( wParam ) == EN_CHANGE )
        //    {
        //        //NotifyObservers( TextChangedEvent );
        //        break;
        //    }
        //}
        case WM_CLOSE:
        {
            NotifyObservers( WindowCloseEvent );
            
            DestroyWindow( m_hwnd );
            m_hwnd = 0;
            break;
        }
        case WM_DESTROY:
        {
            NotifyObservers( WindowDestroyEvent );
            break;
        }
        default:
        {
            return GetOriginalWinProc()( hWnd   ,
                                         nMsg   , 
                                         wParam ,
                                         lParam );
        }
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

bool
CMsWin32Window::Show( void )
{
    return ::ShowWindow( GetHwnd(), SW_SHOW ) == TRUE;
}
    
/*-------------------------------------------------------------------------*/
    
bool
CMsWin32Window::Hide( void )
{
    return ::ShowWindow( GetHwnd(), SW_HIDE ) == TRUE;
}
    
/*-------------------------------------------------------------------------*/
    
bool
CMsWin32Window::Maximize( void )
{
    return ::ShowWindow( GetHwnd(), SW_SHOWMAXIMIZED ) == TRUE;
}
    
/*-------------------------------------------------------------------------*/
    
bool
CMsWin32Window::Minimize( void )
{
    return ::ShowWindow( GetHwnd(), SW_MINIMIZE ) == TRUE;
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Window::SetHwnd( HWND hwnd )
{GUCEF_TRACE;

    if ( 0 != m_hwnd )
    {
        ::DestroyWindow( m_hwnd );
    }
    m_hwnd = hwnd;
}

/*-------------------------------------------------------------------------*/

HWND
CMsWin32Window::GetHwnd( void ) const
{GUCEF_TRACE;

    return m_hwnd;
}

/*-------------------------------------------------------------------------*/

LRESULT
CMsWin32Window::WndProc( HWND hwnd     , 
                         UINT msg      ,
                         WPARAM wParam ,
                         LPARAM lParam )
{GUCEF_TRACE;

    CMsWin32Window* windowObj = 0 ;

    if ( msg == WM_NCCREATE )
    {
        windowObj = (CMsWin32Window*) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        ::SetWindowLongPtr( hwnd, GWL_USERDATA, (LONG_PTR) windowObj );
	
	    //If you process any messages that are sent before CreateWindowEx returns
	    //the HWND, you need something in the place of your HWND member.
	    windowObj->SetHwnd( hwnd );
    }
    else
    {
        windowObj = (CMsWin32Window*) ::GetWindowLong( hwnd, GWL_USERDATA );
    }
    
    if ( 0 != windowObj )
    {
        return windowObj->WindowProc( hwnd, msg, wParam, lParam );
    }
    return ::DefWindowProc( hwnd, msg, wParam, lParam );
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Window::Close( void )
{GUCEF_TRACE;
    
    if ( 0 != m_hwnd )
    {
        ::CloseWindow( m_hwnd );
    }
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Window::Resize( int x, int y, int width, int height )
{GUCEF_TRACE;

    if ( 0 != m_hwnd )
    {
        ::MoveWindow( m_hwnd, x, y, width, height, TRUE );
    }
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Window::SetText( const CString& text )
{GUCEF_TRACE;

    SetWindowText( m_hwnd, text.C_String() );
}

/*-------------------------------------------------------------------------*/
    
CString
CMsWin32Window::GetText( void ) const
{GUCEF_TRACE;

    int len = GetWindowTextLength( m_hwnd );
    if( len > 0 )
    {
        char* buf = (char*) GlobalAlloc( GPTR, len + 1 );
        GetWindowText( m_hwnd, buf, len + 1);

        CString returnValue( buf, len );

        GlobalFree( (HANDLE)buf );
        
        return returnValue;
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Window::SetOriginalWinProc( WNDPROC originalWinProc )
{GUCEF_TRACE;

    m_orgWinProc = originalWinProc;
}

/*-------------------------------------------------------------------------*/

WNDPROC
CMsWin32Window::GetOriginalWinProc( void )
{GUCEF_TRACE;

    if ( 0 != m_orgWinProc )
    {
        return m_orgWinProc;
    }
    return DefWindowProc;
}

/*-------------------------------------------------------------------------*/

bool
CMsWin32Window::WindowCreate( const CString& windowClassName ,
                              const CString& windowTitle     ,
                              const Int32 xPosition          ,
                              const Int32 yPosition          ,
                              const UInt32 width             ,
                              const UInt32 height            ,
                              const HWND hWndParent          )
{GUCEF_TRACE;

    if ( 0 != m_hwnd )
    {
        DestroyWindow( m_hwnd );
        m_hwnd = 0;
    }
    
    m_hwnd = CreateWindowEx( WS_EX_CLIENTEDGE,
                             windowClassName.C_String(),
                             windowTitle.C_String(),
                             WS_OVERLAPPEDWINDOW,
                             xPosition, yPosition, (int)width, (int)height,
                             hWndParent, NULL, GetCurrentModuleHandle(), (LPVOID)this );
    if ( 0 == m_hwnd )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_IMPORTANT, "CMsWin32Window::WindowCreate(): Failed to create a window" );
        return false;
    }

    WNDPROC orgWinProc = (WNDPROC) ::GetWindowLongPtr( m_hwnd, GWLP_WNDPROC );
    if ( orgWinProc != (WNDPROC) WndProc )
    {
        ::SetWindowLongPtr( m_hwnd, GWLP_WNDPROC, (LONG_PTR) WndProc );
        m_orgWinProc = orgWinProc;
    }
    ::SetWindowLongPtr( m_hwnd, GWLP_USERDATA, (LONG_PTR) this );
                                 
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CMsWin32Window::RegisterWindowClass( const CString& windowClassName )
{GUCEF_TRACE;

    WNDCLASSEX wc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetCurrentModuleHandle();
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = windowClassName.C_String();
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if ( 0 == RegisterClassEx( &wc ) )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_IMPORTANT, "CMsWin32Window::RegisterWindowClass(): Failed to register a window class with name \"" + windowClassName + "\"" );
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

const CString&
CMsWin32Window::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::CORE::CMsWin32Window";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Window::OnNotify( CNotifier* notifier    ,
                          const CEvent& eventid  ,
                          CICloneable* eventdata )
{
    if ( eventid == CPulseGenerator::PulseEvent )
    {
        ::UpdateWindow( m_hwnd );
        
        MSG msg;
	    while( ::PeekMessage( &msg, m_hwnd, 0U, 0U, PM_REMOVE ) )
	    {
		    ::TranslateMessage( &msg );
		    ::DispatchMessage( &msg );
	    }
    }
    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */
