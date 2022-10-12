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

#include "gucefCORE_COSWindow.h"

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    #ifndef GUCEF_CORE_CMSWIN32WINDOW_H
    #include "gucefCORE_CMsWin32Window.h"
    #define GUCEF_CORE_CMSWIN32WINDOW_H
    #endif /* GUCEF_CORE_CMSWIN32WINDOW_H ? */

#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

    #ifndef GUCEF_CORE_CX11WINDOW_H
    #include "gucefCORE_CX11Window.h"
    #define GUCEF_CORE_CX11WINDOW_H
    #endif /* GUCEF_CORE_CX11WINDOW_H ? */

#endif

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

const CEvent COSWindow::WindowCloseEvent = "GUCEF::CORE::COSWindow::WindowCloseEvent";
const CEvent COSWindow::WindowDestroyEvent = "GUCEF::CORE::COSWindow::WindowDestroyEvent";
const CEvent COSWindow::WindowActivationEvent = "GUCEF::CORE::COSWindow::WindowActivationEvent";
const CEvent COSWindow::WindowResizeEvent = "GUCEF::CORE::COSWindow::WindowResizeEvent";
const CEvent COSWindow::WindowPaintEvent = "GUCEF::CORE::COSWindow::WindowPaintEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
COSWindow::RegisterEvents( void )
{GUCEF_TRACE;

    WindowCloseEvent.Initialize();
    WindowDestroyEvent.Initialize();
    WindowActivationEvent.Initialize();
    WindowResizeEvent.Initialize();
    WindowPaintEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

COSWindow::COSWindow( void )
    : CObservingNotifier()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

COSWindow::~COSWindow()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

COSWindow*
COSWindow::Create( void )
{GUCEF_TRACE;

    // Build-in strategy pattern, there is only 1 right answer per graphical platform
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    CMsWin32Window* window = GUCEF_NEW CMsWin32Window();
    if ( !CORE::CMsWin32Window::RegisterWindowClass( window->GetClassTypeName() ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "COSWindow: Could not register window class" );
        GUCEF_DELETE window;
        return 0;
    }

    return window;
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    return GUCEF_NEW CX11Window();
    #else
    return 0;
    #endif
}

/*-------------------------------------------------------------------------*/

void
COSWindow::Destroy( COSWindow* osWindow )
{GUCEF_TRACE;

    GUCEF_DELETE osWindow;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

