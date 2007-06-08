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

#include "CWindowMsgHook.h"

#ifdef GUCEF_MSWIN_BUILD

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex CWindowMsgHook::s_globalDataLock;
CWindowMsgHook::TMappedWindowHookList CWindowMsgHook::s_list;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

// The pointer warnings are always posted even when we know for certain that the code
// is correct so we disable the warning and keep an eye on this code.
#pragma warning( disable:4311 ) // pointer truncation from 'type' to 'type'
#pragma warning( disable:4312 ) // 'type cast' : conversion from 'type' to 'type' of greater size

/*-------------------------------------------------------------------------*/

CWindowMsgHook::CWindowMsgHook( void )
{TRACE;

    
}

/*-------------------------------------------------------------------------*/

CWindowMsgHook::~CWindowMsgHook()
{TRACE;

    s_globalDataLock.Lock();

    // Make sure there are no outstanding refrences to this window
    TMappedWindowHookList::iterator i = s_list.begin();
    while ( i != s_list.end() )
    {
        (*i).second.hookList.erase( this );
        if ( (*i).second.hookList.size() == 0 )
        {
            // if this is the last hook then the original
            // window procedure should be restored
            #ifdef GUCEF_32BIT
            SetWindowLongPtr( (*i).first, GWLP_WNDPROC, (LONG)( (*i).second.orgWinProc ) );
            #else
            SetWindowLongPtr( (*i).first, GWLP_WNDPROC, (LONG_PTR)( (*i).second.orgWinProc ) );
            #endif
        }

        ++i;
    }

    s_globalDataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CWindowMsgHook::Hook( const HWND hWnd              ,
                      void* userData /* = NULL  */ )
{TRACE;

    if ( hWnd && ::IsWindow( hWnd ) )
    {
        s_globalDataLock.Lock();
    
        TMappedWindowHookList::iterator i = s_list.find( hWnd );
        if ( i != s_list.end() )
        {
            (*i).second.hookList[ this ] = userData;
            s_globalDataLock.Unlock();
            return;
        }

        // Add this new window to our list
        TWindowData windowData;
        windowData.hookList[ this ] = userData;
        windowData.orgWinProc = ( WNDPROC ) GetWindowLongPtr( hWnd, GWLP_WNDPROC );
        s_list[ hWnd ] = windowData;

        // Perform instance subclassing on the window
        #ifdef GUCEF_32BIT
        SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG)( HookProc ) );
        #else
        SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG_PTR)( HookProc ) );
        #endif
            
        s_globalDataLock.Unlock();
    }
}

/*-------------------------------------------------------------------------*/

void
CWindowMsgHook::Unhook( const HWND hWnd )
{TRACE;

    if ( hWnd && ::IsWindow( hWnd ) )
    {
        s_globalDataLock.Lock();
    
        TMappedWindowHookList::iterator i = s_list.find( hWnd );
        if ( i != s_list.end() )
        {
            // remove this hook from the hook list
            (*i).second.hookList.erase( this );

            // if this is the last hook then the original
            // window procedure should be restored
            if ( (*i).second.hookList.size() == 0 )
            {
                #ifdef GUCEF_32BIT
                SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG)( (*i).second.orgWinProc ) );
                #else
                SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG_PTR)( (*i).second.orgWinProc ) );
                #endif
            }

        }
        s_globalDataLock.Unlock();
    }            
}

/*-------------------------------------------------------------------------*/

LRESULT
CWindowMsgHook::WindowProc( const HWND hWnd                  ,
                            const UINT nMsg                  ,
                            const WPARAM wParam              ,
                            const LPARAM lParam              ,
                            void* /* userData *//* = NULL */ ,
                            bool& /* consumeMsg */           )
{TRACE;

    //  We don't deal with it, let somebody else do it
    return DefWindowProc( hWnd    , 
                          nMsg    , 
                          wParam  , 
                          lParam  );
}

/*-------------------------------------------------------------------------*/

LRESULT CALLBACK
CWindowMsgHook::HookProc( HWND hWnd     ,
                          UINT nMsg     ,
                          WPARAM wParam ,
                          LPARAM lParam )
{TRACE;

    s_globalDataLock.Lock();

    // Iterate our hook list for each hook for this window handle
    TWindowData& windowData = s_list[ hWnd ];
    TWindowMsgHookList::iterator i = windowData.hookList.begin(); 
    bool consumeMsg( false );
    while ( i != windowData.hookList.end() )
    {
        // Call each hook ourselves
        ( *i ).first->WindowProc( hWnd          ,
                                  nMsg          ,
                                  wParam        ,
                                  lParam        ,
                                  ( *i ).second ,
                                  consumeMsg    );
        if ( consumeMsg )
        {
            // No need to proceed, the message has been consumed
            break;
        }  
        ++i;
    }

    if ( !consumeMsg )
    {
        // Store the window proc pointer on stack before releasing our lock
        WNDPROC orgWinProc = windowData.orgWinProc;    
        s_globalDataLock.Unlock();

        // Now we call the original window procedure
        return CallWindowProc( orgWinProc ,
                               hWnd       , 
                               nMsg       ,
                               wParam     ,
                               lParam     );
    }
    s_globalDataLock.Unlock();
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