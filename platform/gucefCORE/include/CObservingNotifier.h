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

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#define GUCEF_CORE_COBSERVINGNOTIFIER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CNOTIFIEROBSERVINGCOMPONENT_H
#include "CNotifierObservingComponent.h"
#define GUCEF_CORE_CNOTIFIEROBSERVINGCOMPONENT_H
#endif /* GUCEF_CORE_CNOTIFIEROBSERVINGCOMPONENT_H ? */

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_CIOBSERVER_H
#include "gucefCORE_CIObserver.h"
#define GUCEF_CORE_CIOBSERVER_H
#endif /* GUCEF_CORE_CIOBSERVER_H ? */

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
 *  Class that combines a notifier with observer behavior
 *  Mainly to be used for node objects in your chain-of-events
 *  tree
 */
class GUCEF_CORE_PUBLIC_CPP CObservingNotifier : public CNotifier ,
                                                 public virtual CIObserver
{
    public:

    CObservingNotifier( void );

    CObservingNotifier( const CObservingNotifier& src );

    virtual ~CObservingNotifier();

    CObservingNotifier& operator=( const CObservingNotifier& src );

    /**
     *  Unsubscribes the observer component from all notifiers.
     */
    void UnsubscribeAllFromObserver( void );

    UInt32 GetObserverSubscriptionCount( void );

    UInt32 GetObserverNotifierCount( void );

    /**
     *  Subscribes the given observer to all
     *  notifier events. Every event dispatched by
     *  the notifier will be sent to the observer.
     */
    void SubscribeTo( CNotifier* notifier );

    /**
     *  Subscribes the observer component to the four standard
     *  notifier events if it is not yet subscribed plus
     *  subscribes to the given custom event.
     */
    void SubscribeTo( CNotifier* notifier   ,
                      const CEvent& eventid );

    /**
     *  Subscribes the observer component to the four standard
     *  notifier events if it is not yet subscribed plus
     *  subscribes to the given custom event.
     */
    void SubscribeTo( CNotifier* notifier                 ,
                      const CEvent& eventid               ,
                      CIEventHandlerFunctorBase& callback );

    /**
     *  Detaches the observer component from the notifier.
     *  All the observers subscriptions will be cancelled
     *  This includes both standard notifier events as well
     *  as custom events.
     */
    void UnsubscribeFrom( CNotifier* notifier );

    /**
     *  Cancels the observer component's subscription to the given event.
     *
     *  Note that subscriptions to the standard notifier events
     *  cannot be cancelled, attempts to do so will be ignored.
     */
    void UnsubscribeFrom( CNotifier* notifier   ,
                          const CEvent& eventid );

    /**
     *  Returns a reference to the mutable observer component.
     *
     *  Note that mutations to the observer component also lock
     *  the notification component in a multi-threading scenario
     *  assuming Lock() and Unlock() are implemented.
     *  As such this operation can be made in a threadsafe fashion.
     */
    CObserver& AsObserver( void );

    /**
     *  Returns a reference to the observer component.
     *
     *  Note that retrieval actions on the observer component also lock
     *  the notification component in a multi-threading scenario
     *  assuming Lock() and Unlock() are implemented.
     *  As such this operation can be made in a threadsafe fashion.
     */
    const CObserver& AsObserver( void ) const;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:
    friend class CNotifierObservingComponent;

    virtual bool Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    void SignalUpcomingObserverDestruction( void );

    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier                 ,
                           const CEvent& eventid               ,
                           CICloneable* eventdata = GUCEF_NULL ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    void SubscribeToImp( CNotifier* notifier                 ,
                         const CEvent& eventid               ,
                         CIEventHandlerFunctorBase* callback );

    private:

    CNotifierObservingComponent m_observer;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */
