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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CMsWin32Window::CMsWin32Window( void )
    : m_hwnd( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMsWin32Window::~CMsWin32Window()
{GUCEF_TRACE;

    if ( 0 != m_hwnd )
    {
        DestroyWindow( m_hwnd );
    }
    m_hwnd = 0;
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
        case WM_CLOSE:
        {
            DestroyWindow( m_hwnd );
            m_hwnd = 0;
            break;
        }
        default:
        {
            return DefWindowProc( hWnd   ,
                                  nMsg   , 
                                  wParam ,
                                  lParam );
        }
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CMsWin32Window::SetHwnd( HWND hwnd )
{GUCEF_TRACE;

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
        SetWindowLong( hwnd, GWL_USERDATA, (long) windowObj );
	
	    //If you process any messages that are sent before CreateWindowEx returns
	    //the HWND, you need something in the place of your HWND member.
	    windowObj->SetHwnd( hwnd );
    }
    else
    {
        windowObj = (CMsWin32Window*) GetWindowLong( hwnd, GWL_USERDATA );
    }
    
    if ( 0 != windowObj )
    {
        return windowObj->WindowProc( hwnd, msg, wParam, lParam );
    }
    return DefWindowProc( hwnd, msg, wParam, lParam );
}

/*-------------------------------------------------------------------------*/

bool
CMsWin32Window::WindowCreate( const CString& windowClassName ,
                              const CString& windowTitle     )
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
                             CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
                             NULL, NULL, GetCurrentModuleHandle(), (LPVOID)this );
    return m_hwnd != 0;
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

    return 0 == RegisterClassEx( &wc );
}

/*-------------------------------------------------------------------------*/

const CString&
CMsWin32Window::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::CORE::CMsWin32Window";
    return classTypeName;
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
