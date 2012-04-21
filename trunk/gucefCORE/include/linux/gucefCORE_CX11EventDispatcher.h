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

#ifndef GUCEF_CORE_CX11EVENTDISPATCHER_H
#define GUCEF_CORE_CX11EVENTDISPATCHER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* module config macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifdef GUCEF_LINUX_BUILD

#include <X11/Xlib.h>

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

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
 *  Class which wraps the X11 event polling and turns it into a notification
 *  system
 */
class GUCEF_CORE_PUBLIC_CPP CX11EventDispatcher : public CObservingNotifier
{
    public:

    static const CEvent X11Event;

    static void RegisterEvents( void );

    typedef CTCloneableObj< ::XEvent >  TX11EventData;

    public:

    static CX11EventDispatcher* Instance( void );

    /**
     *  In addition to the standard subscription options this gives you the option of
     *  subscribing to events to be sent only if the event is destined for the given window
     */
    void SubscribeOnBehalfOfWindow( CObserver& observer ,
                                    ::Window window     );

    void UnsubscribeOnBehalfOfWindow( CObserver& observer ,
                                      ::Window window     );

    ::Display* GetDisplay( void ) const;

    protected:

    virtual void OnObserverDestruction( CObserver* observer );

    virtual void LockData( void ) const;

    virtual void UnlockData( void ) const;

    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );

    private:
    friend class CModule;

    static void Deinstance( void );

    private:

    CX11EventDispatcher();

    virtual ~CX11EventDispatcher();

    void ProcessEvent( ::XEvent& event );

    void DispatchEventToWindow( ::XEvent& event, ::Window window );

    private:

    typedef std::set< CObserver* > TObserverSet;
    typedef std::map< ::Window, TObserverSet >  TWindowObserverMap;

    static CX11EventDispatcher* g_instance;
    static MT::CMutex g_mutex;
    TWindowObserverMap m_windowObserverMap;
    ::Display* m_display;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_LINUX_BUILD ? */

#endif /* GUCEF_CORE_CX11EVENTDISPATCHER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-04-2003 :
        - Initial implementation

---------------------------------------------------------------------------*/
