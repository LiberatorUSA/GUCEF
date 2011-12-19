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

#include "gucefCORE_CX11Window.h"

#ifdef GUCEF_LINUX_BUILD

#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CX11EVENTDISPATCHER_H
#include "gucefCORE_CX11EventDispatcher.h"
#define GUCEF_CORE_CX11EVENTDISPATCHER_H
#endif /* GUCEF_CORE_CX11EVENTDISPATCHER_H ? */

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

const CEvent CX11Window::WindowCloseEvent = "GUCEF::CORE::CX11Window::WindowCloseEvent";
const CEvent CX11Window::WindowDestroyEvent = "GUCEF::CORE::CX11Window::WindowDestroyEvent";
const CEvent CX11Window::WindowActivationEvent = "GUCEF::CORE::CX11Window::WindowActivationEvent";
const CEvent CX11Window::WindowResizeEvent = "GUCEF::CORE::CX11Window::WindowResizeEvent";
const CEvent CX11Window::WindowPaintEvent = "GUCEF::CORE::CX11Window::WindowPaintEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CX11Window::RegisterEvents( void )
{GUCEF_TRACE;

    WindowCloseEvent.Initialize();
    WindowDestroyEvent.Initialize();
    WindowActivationEvent.Initialize();
    WindowResizeEvent.Initialize();
    WindowPaintEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CX11Window::CX11Window( void )
    : m_display( NULL )   ,
      m_screenNr( 0 )     ,
      m_window( 0 )       ,
      m_wmDeleteWindow()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CX11Window::~CX11Window()
{GUCEF_TRACE;

    WindowDestroy();
    SetDisplay( NULL );
}

/*-------------------------------------------------------------------------*/

void
CX11Window::SetDisplay( ::Display* display )
{GUCEF_TRACE;

    if ( NULL != m_display )
    {
        ::XCloseDisplay( m_display );
    }
    m_display = display;
}

/*-------------------------------------------------------------------------*/

::Display*
CX11Window::GetDisplay( void ) const
{GUCEF_TRACE;

    return m_display;
}

/*-------------------------------------------------------------------------*/

void
CX11Window::WindowDestroy( void )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        // Since we are getting rid of the window we don't need to listnen to events for it anymore
        CX11EventDispatcher::Instance()->UnsubscribeOnBehalfOfWindow( AsObserver(), m_window );

        ::XDestroyWindow( m_display, m_window );
        m_window = 0;
    }
}

/*-------------------------------------------------------------------------*/

void
CX11Window::SendToForegound( void )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XRaiseWindow( m_display, m_window );
    }
}

/*-------------------------------------------------------------------------*/

void
CX11Window::GrabFocus( void )
{GUCEF_TRACE;

    if ( ( NULL != m_display ) && ( 0 != m_window ) )
    {
        XSetInputFocus( m_display, m_window, RevertToNone, CurrentTime );
    }
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::Show( void )
{
    if ( NULL != m_display && 0 != m_window )
    {
        ::XMapWindow( m_display, m_window );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::Hide( void )
{
    if ( NULL != m_display && 0 != m_window )
    {
        ::XUnmapWindow( m_display, m_window );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::Maximize( void )
{
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::Minimize( void )
{
    return false;
}

/*-------------------------------------------------------------------------*/

void
CX11Window::SetWindow( ::Window window )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XDestroyWindow( m_display, m_window );
    }
    m_window = window;
}

/*-------------------------------------------------------------------------*/

::Window
CX11Window::GetWindow( void ) const
{GUCEF_TRACE;

    return m_window;
}

/*-------------------------------------------------------------------------*/

::Window
CX11Window::GetParentWindow( void ) const
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XWindowAttributes windowAttrs;
        ::XGetWindowAttributes( m_display, m_window, &windowAttrs );
        return windowAttrs.root;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CX11Window::Close( void )
{GUCEF_TRACE;

    WindowDestroy();
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::SetWindowAreaPosition( int x, int y )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XMoveWindow( m_display, m_window, x, y );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::GetWindowAreaPosition( int& x, int& y ) const
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XWindowAttributes windowAttrs;
        ::XGetWindowAttributes( m_display, m_window, &windowAttrs );
        x = windowAttrs.x - windowAttrs.border_width;
        y = windowAttrs.y - windowAttrs.border_width;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::GetClientAreaPosition( int& x, int& y ) const
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XWindowAttributes windowAttrs;
        ::XGetWindowAttributes( m_display, m_window, &windowAttrs );
        x = windowAttrs.x;
        y = windowAttrs.y;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::GetClientArea( int& x, int& y, int& width, int& height ) const
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XWindowAttributes windowAttrs;
        ::XGetWindowAttributes( m_display, m_window, &windowAttrs );
        x = windowAttrs.x;
        y = windowAttrs.y;
        width = windowAttrs.width;
        height = windowAttrs.height;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::GetWindowArea( int& x, int& y, int& width, int& height ) const
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XWindowAttributes windowAttrs;
        ::XGetWindowAttributes( m_display, m_window, &windowAttrs );
        x = windowAttrs.x - windowAttrs.border_width;
        y = windowAttrs.y - windowAttrs.border_width;
        width = windowAttrs.width + windowAttrs.border_width;
        height = windowAttrs.height + windowAttrs.border_width;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::GetWindowAreaRelativeToParentClientArea( Float32& x      ,
                                                     Float32& y      ,
                                                     Float32& width  ,
                                                     Float32& height ) const
{GUCEF_TRACE;

//    if ( 0 != m_hwnd )
//    {
//        HWND parentHwnd = ::GetParent( m_hwnd );
//        if ( 0 != parentHwnd )
//        {
//            RECT rcWindow;
//            ::GetWindowRect( m_hwnd, &rcWindow );
//            RECT rcParentClient;
//            ::GetClientRect( parentHwnd, &rcParentClient );
//
//            Float32 thisWidth = (Float32) rcWindow.right - rcWindow.left;
//            Float32 thisHeight = (Float32) rcWindow.bottom - rcWindow.top;
//            Float32 parentWidth = (Float32) rcParentClient.right - rcParentClient.left;
//            Float32 parentHeight = (Float32) rcParentClient.bottom - rcParentClient.top;
//
//            x = rcWindow.left / parentWidth;
//            y = rcWindow.top / parentHeight;
//            width = thisWidth / parentWidth;
//            height = thisHeight / parentHeight;
//            return true;
//        }
//    }
    return false;
}

/*-------------------------------------------------------------------------*/

int
CX11Window::GetWidthOfClientAreaBorder( void ) const
{GUCEF_TRACE;

    ::XWindowAttributes windowAttrs;
    ::XGetWindowAttributes( m_display, m_window, &windowAttrs );
     return windowAttrs.border_width;
}

/*-------------------------------------------------------------------------*/

void
CX11Window::WindowAreaResize( int x, int y, int width, int height, bool repaint )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XMoveResizeWindow( m_display, m_window, x, y, width, height );
        if ( repaint )
        {
            ::XClearWindow( m_display, m_window );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CX11Window::WindowAreaResize( int width, int height, bool repaint )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XResizeWindow( m_display, m_window, width, height );
        if ( repaint )
        {
            ::XClearWindow( m_display, m_window );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CX11Window::ClientAreaResize( int x, int y, int width, int height, bool repaint )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XMoveResizeWindow( m_display, m_window, x, y, width, height );
        if ( repaint )
        {
            ::XClearWindow( m_display, m_window );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CX11Window::ClientAreaResize( int width, int height, bool repaint )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XResizeWindow( m_display, m_window, width, height );
        if ( repaint )
        {
            ::XClearWindow( m_display, m_window );
        }
    }
}

/*-------------------------------------------------------------------------*/

inline Float32
ClampFloat( Float32 valueToClamp )
{
    if ( valueToClamp < 0.0 )
    {
        return 0.0;
    }
    else
    if ( valueToClamp > 1.0 )
    {
        return 1.0;
    }
    return valueToClamp;
}

/*-------------------------------------------------------------------------*/

void
CX11Window::ClientAreaResizeRelativeToParentClientArea( Float32 x      ,
                                                        Float32 y      ,
                                                        Float32 width  ,
                                                        Float32 height ,
                                                        bool repaint   )
{GUCEF_TRACE;

//    if ( 0 != m_hwnd )
//    {
//        HWND parentWindow = ::GetParent( m_hwnd );
//        if ( 0 != parentWindow )
//        {
//            x = ClampFloat( x );
//            y = ClampFloat( y );
//            width = ClampFloat( width );
//            height = ClampFloat( height );
//
//            RECT parentClientAreaSize;
//            ::GetClientRect( parentWindow, &parentClientAreaSize );
//
//            int parentWidth = parentClientAreaSize.right - parentClientAreaSize.left;
//            int parentHeight = parentClientAreaSize.bottom - parentClientAreaSize.top;
//
//            ClientAreaResize( (int) ( x * parentWidth )       ,
//                              (int) ( y * parentHeight )      ,
//                              (int) ( width * parentWidth )   ,
//                              (int) ( height * parentHeight ) ,
//                              repaint                         );
//        }
//    }
}

/*-------------------------------------------------------------------------*/

void
CX11Window::WindowAreaResizeRelativeToParentClientArea( Float32 x      ,
                                                        Float32 y      ,
                                                        Float32 width  ,
                                                        Float32 height ,
                                                        bool repaint   )
{GUCEF_TRACE;

//    if ( 0 != m_hwnd )
//    {
//        HWND parentWindow = ::GetParent( m_hwnd );
//        if ( 0 != parentWindow )
//        {
//            x = ClampFloat( x );
//            y = ClampFloat( y );
//            width = ClampFloat( width );
//            height = ClampFloat( height );
//
//            RECT parentClientAreaSize;
//            ::GetClientRect( parentWindow, &parentClientAreaSize );
//
//            int parentWidth = parentClientAreaSize.right - parentClientAreaSize.left;
//            int parentHeight = parentClientAreaSize.bottom - parentClientAreaSize.top;
//
//            WindowAreaResize( (int) ( x * parentWidth )       ,
//                              (int) ( y * parentHeight )      ,
//                              (int) ( width * parentWidth )   ,
//                              (int) ( height * parentHeight ) ,
//                              repaint                         );
//        }
//    }
}

/*-------------------------------------------------------------------------*/

void
CX11Window::Repaint( void )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        ::XClearWindow( m_display, m_window );
    }
}

/*-------------------------------------------------------------------------*/

void
CX11Window::SetText( const CString& text )
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        // We use the XTextProperty structure to store the title.
        ::XTextProperty windowName;
        windowName.value    = (unsigned char*) text.C_String();
        windowName.encoding = XA_STRING;
        windowName.format   = 8;
        windowName.nitems   = text.Length();

        // Tell X to ask the window manager to set the window title.
        // X11 itself doesn't provide window title functionality.
        ::XSetWMName( m_display, m_window, &windowName );
    }
}

/*-------------------------------------------------------------------------*/

CString
CX11Window::GetText( void ) const
{GUCEF_TRACE;

    if ( NULL != m_display && 0 != m_window )
    {
        // Tell X to ask the window manager to get the window title.
        // X11 itself doesn't provide window title functionality.
        ::XTextProperty windowName;
        ::XGetWMName( m_display, m_window, &windowName );

        return (const char*) windowName.value;
    }
}

/*-------------------------------------------------------------------------*/

bool
CX11Window::WindowCreate( const Int32 xPosition ,
                          const Int32 yPosition ,
                          const UInt32 width    ,
                          const UInt32 height   ,
                          ::Window parentWindow )
{GUCEF_TRACE;

    // If we already have a window destroy it
    WindowDestroy();

    if ( NULL == m_display )
    {
        m_display = ::XOpenDisplay( NULL );
    }
    if ( NULL == m_display )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "CX11Window::WindowCreate(): Failed to open display" );
        return false;
    }
    m_screenNr = DefaultScreen( m_display );

    if ( 0 == parentWindow )
    {
        // no parent window was provided so we will make this window a child of the default display's root
        parentWindow = RootWindow( m_display, m_screenNr );
    }

    m_window = ::XCreateSimpleWindow( m_display,
                                      parentWindow,
                                      xPosition, yPosition, width, height, 1,
                                      BlackPixel( m_display, m_screenNr ),
                                      WhitePixel( m_display, m_screenNr ) );

    // Subscribe to window events at the centralized dispatcher
    CX11EventDispatcher::Instance()->SubscribeOnBehalfOfWindow( AsObserver(), m_window );

    // register interest in the delete window message
    ::Atom wmDeleteMessage = ::XInternAtom( m_display, "WM_DELETE_WINDOW", False );
    ::XSetWMProtocols( m_display, m_window, &wmDeleteMessage, 1 );

    Show();
    Repaint();
    return true;
}

/*-------------------------------------------------------------------------*/

const CString&
CX11Window::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::CORE::CX11Window";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
CX11Window::OnNotify( CNotifier* notifier    ,
                          const CEvent& eventid  ,
                          CICloneable* eventdata )
{
    if ( eventid == CPulseGenerator::PulseEvent )
    {

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

#endif /* GUCEF_LINUX_BUILD ? */
