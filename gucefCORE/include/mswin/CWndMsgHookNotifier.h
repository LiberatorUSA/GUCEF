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

#ifndef GUCEF_CORE_CWNDMSGHOOKNOTIFIER_H
#define GUCEF_CORE_CWNDMSGHOOKNOTIFIER_H

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

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

#ifndef GUCEF_CORE_CWINDOWMSGHOOK_H
#include "CWindowMsgHook.h"
#define GUCEF_CORE_CWINDOWMSGHOOK_H
#endif /* GUCEF_CORE_CWINDOWMSGHOOK_H ? */

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
 *  Class that translates MS Windows events obtained from instance sub-classing
 *  a window into GUCEF events for easy processing
 */
class GUCEFCORE_EXPORT_CPP CWndMsgHookNotifier : public CNotifier      ,
                                                 public CWindowMsgHook
{
    public:
    
    static const CEvent WindowActivationEvent;
    static const CEvent WindowSizeEvent;
    
    static void RegisterEvents( void );
    
    struct SWindowSizeEventData
    {
        bool windowVisible;     /**< whether the window is visible */
        UInt32 windowWidth;     /**< the new width of the window */
        UInt32 windowHeight;    /**< the new height of the window */
        void* userData;         /**< user data pointer */
    };
    typedef CTCloneableObj< struct SWindowSizeEventData > TWindowSizeEventData;

    struct SWindowActivationEventData
    {
        bool windowActiveState;  /**< boolean indicating whether the window is active */
        void* userData;          /**< user data pointer */
    };
    typedef CTCloneableObj< struct SWindowActivationEventData > TWindowActivationEventData; 
    
    public:
    
    CWndMsgHookNotifier();

    virtual ~CWndMsgHookNotifier();

    protected:
    
    virtual LRESULT WindowProc( const HWND hWnd     ,
                                const UINT nMsg     ,
                                const WPARAM wParam ,
                                const LPARAM lParam ,
                                void* userData      ,
                                bool& consumeMsg    );

    private:

    CWndMsgHookNotifier( const CWndMsgHookNotifier& src );       /**< cannot be copied */
    CWndMsgHookNotifier& operator=( const CWndMsgHookNotifier ); /**< cannot be copied */
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

#endif /* GUCEF_CORE_CWNDMSGHOOKNOTIFIER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-04-2003 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
