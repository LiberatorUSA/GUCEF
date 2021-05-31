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

#ifndef GUCEF_CORE_CINOTIFIER_H
#define GUCEF_CORE_CINOTIFIER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CILOCKABLE_H
#include "gucefMT_CILockable.h"
#define GUCEF_MT_CILOCKABLE_H
#endif /* GUCEF_MT_CILOCKABLE_H ? */

#ifndef GUCEF_CORE_CIEVENTHANDLERFUNCTORBASE_H
#include "gucefCORE_CIEventHandlerFunctorBase.h"
#define GUCEF_CORE_CIEVENTHANDLERFUNCTORBASE_H
#endif /* GUCEF_CORE_CIEVENTHANDLERFUNCTORBASE_H ? */

#ifndef GUCEF_CORE_CITYPENAMED_H
#include "CITypeNamed.h"
#define GUCEF_CORE_CITYPENAMED_H
#endif /* GUCEF_CORE_CITYPENAMED_H ? */

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

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

class CObserver;

/*-------------------------------------------------------------------------*/

/**
 *  Interface class for all notifier implementations
 */
class GUCEF_CORE_PUBLIC_CPP CINotifier : public virtual MT::CILockable ,
                                         public virtual CITypeNamed
{
    public:

    static const CEvent SubscribeEvent;
    static const CEvent UnsubscribeEvent;
    static const CEvent ModifyEvent;
    static const CEvent DestructionEvent;

    static void RegisterEvents( void );

    public:

    CINotifier( bool registerStdEvents = true );

    CINotifier( const CINotifier& src );

    virtual ~CINotifier();

    CINotifier& operator=( const CINotifier& src );

    /**
     *  Subscribes the given observer to all
     *  notifier events. Every event dispatched by
     *  the notifier will be sent to the observer.
     */
    virtual void Subscribe( CObserver* observer ) = 0;

    /**
     *  Subscribes the given observer to the four standard
     *  notifier events if it is not yet subscribed plus
     *  subscribes to the given custom event.
     */
    virtual void Subscribe( CObserver* observer                        ,
                            const CEvent& eventid                      ,
                            CIEventHandlerFunctorBase* callback = NULL ) = 0;

    /**
     *  Detaches the given observer from the notifier.
     *  All the observers subscriptions will be cancelled
     *  This includes both standard notifier events as well
     *  as custom events.
     */
    virtual void Unsubscribe( CObserver* observer ) = 0;

    /**
     *  Cancels the observer's subscription to the given event.
     *
     *  Note that subscriptions to the standard notifier events
     *  cannot be cancelled, attempts to do so will be ignored.
     */
    virtual void Unsubscribe( CObserver* observer   ,
                              const CEvent& eventid ) = 0;

    /**
     *  Cancels the subscription of all observers subscribed to this notifier
     */
    virtual void UnsubscribeAllFromNotifier( void ) = 0;

    virtual UInt32 GetSubscriptionCountForObserver( CObserver* observer ) const = 0;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CINOTIFIER_H ? */
