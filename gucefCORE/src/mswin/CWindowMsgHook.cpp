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