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

#ifndef GUCEF_CORE_CWINDOWMSGHOOK_H
#define GUCEF_CORE_CWINDOWMSGHOOK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_macros.h"  /* module config macros */

#ifdef GUCEF_MSWIN_BUILD

#include <map>
#include "gucefMT_CMutex.h"    /* needed for threadsafety */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class that should be used when performing instance subclassing of 
 *  windows. Using an uniform aproach minimizes the risks.
 */
class GUCEFCORE_EXPORT_CPP CWindowMsgHook
{
    public:
    
    CWindowMsgHook();

    virtual ~CWindowMsgHook();

    /**
     *  Subscribes the hook object to messages sent to the window
     *  with the given handle
     *
     *  @param hWnd handle to the window you wish to instance subclass
     *  @param userData optional pointer that will be passed when WindowProc is called
     */
    void Hook( const HWND hWnd       ,
               void* userData = NULL );

    /**
     *  Unsubscribes the hook object from messages sent to the window
     *  with the given handle
     */
    void Unhook( const HWND hWnd );

    protected:
    
    //This is the function you need to override in your derived
    //version of CHookWnd so that you can handle the window messages
    virtual LRESULT WindowProc( const HWND hWnd     ,
                                const UINT nMsg     ,
                                const WPARAM wParam ,
                                const LPARAM lParam ,
                                void* userData      ,
                                bool& consumeMsg    );

    private:

    //The callback function through which all window messages
    //will be handled when they are hooked by CHookWnd 
    static LRESULT CALLBACK HookProc( HWND hWnd     ,
                                      UINT nMsg     ,
                                      WPARAM wParam ,
                                      LPARAM lParam );

    private:
    typedef std::map< CWindowMsgHook*, void* > TWindowMsgHookList;
    struct SWindowData
    {
        TWindowMsgHookList hookList;
        WNDPROC orgWinProc;
    };
    typedef struct SWindowData TWindowData;
    typedef std::map< HWND, TWindowData > TMappedWindowHookList;

    static MT::CMutex s_globalDataLock;
    static TMappedWindowHookList s_list;

    private:

    CWindowMsgHook( const CWindowMsgHook& src );       /**< cannot be copied */
    CWindowMsgHook& operator=( const CWindowMsgHook ); /**< cannot be copied */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MSWIN_BUILD ? */

#endif /* GUCEF_CORE_CWINDOWMSGHOOK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-04-2003 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
