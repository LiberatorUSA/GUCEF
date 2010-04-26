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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* module config macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifdef GUCEF_MSWIN_BUILD

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "gucefCORE_CMsWin32ConsoleWindow.h"

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

//const CEvent CWndMsgHookNotifier::WindowActivationEvent = "GUCEF::CORE::CWndMsgHookNotifier::WindowActivationEvent";
//const CEvent CWndMsgHookNotifier::WindowSizeEvent = "GUCEF::CORE::CWndMsgHookNotifier::WindowSizeEvent";
//const CEvent CWndMsgHookNotifier::WindowDestroyEvent = "GUCEF::CORE::CWndMsgHookNotifier::WindowDestroyEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

//void
//CWndMsgHookNotifier::RegisterEvents( void )
//{GUCEF_TRACE;
//
//    WindowActivationEvent.Initialize();
//    WindowSizeEvent.Initialize();
//    WindowDestroyEvent.Initialize();
//}

/*-------------------------------------------------------------------------*/

CMsWin32ConsoleWindow::CMsWin32ConsoleWindow( void )
    : CMsWin32Window()
{GUCEF_TRACE;

  //  RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CMsWin32ConsoleWindow::~CMsWin32ConsoleWindow()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CMsWin32ConsoleWindow::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::CORE::CMsWin32ConsoleWindow";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

LRESULT
CMsWin32ConsoleWindow::WindowProc( const HWND hWnd     ,
                                   const UINT nMsg     ,
                                   const WPARAM wParam ,
                                   const LPARAM lParam ,
                                   void* userData      ,
                                   bool& consumeMsg    )
{GUCEF_TRACE;

    return 0;
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
