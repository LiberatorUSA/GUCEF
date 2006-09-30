/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_CNOTIFIER_H
#define GUCEF_CORE_CNOTIFIER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string>
#include <set>
#include <map>
#include "gucefCORE_ETypes.h"
#include "CICloneable.h"

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

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
 *  Use the CNotificationIDRegistry to obtain unique event id's for 
 *  your events. It is strongly recommended to include the various namespaces 
 *  in the event strings.
 *
 *  Note that this class automaticly registers four notification events
 *  if they are not already registered. These are:
 *      - "GUCEF::CORE::CNotifier::SubscribeEvent"
 *      - "GUCEF::CORE::CNotifier::UnsubscribeEvent"
 *      - "GUCEF::CORE::CNotifier::ModifyEvent"
 *      - "GUCEF::CORE::CNotifier::DestructionEvent"
 *
 *  Note that every observer will be automaticly subscribed to
 *  the four standard notification events.
 *
 *  Note that CNotifier is NOT threadsafe on it's own to avoid introducing
 *  unnesesarry overhead. If you want a threadsafe notifier then simply
 *  implement LockData() and UnlockData() yourself in your decending class.
 *
 *  Current known issues:
 *  - chain reactions occuring during the handling of an event that access
 *    the notifier indirectly can cause errors 
 */
class EXPORT_CPP CNotifier
{
    public:

    static const CString SubscribeEvent;
    static const CString UnsubscribeEvent;
    static const CString ModifyEvent;
    static const CString DestructionEvent;

    CNotifier( void );

    CNotifier( const CNotifier& src );
    
    virtual ~CNotifier();
    
    CNotifier& operator=( const CNotifier& src );

    /**
     *  Subscribes the given observer to all
     *  notifier events. Every event dispatched by 
     *  the notifier will be sent to the observer.
     */
    void Subscribe( CObserver* observer );
    
    /**
     *  Subscribes the given observer to the four standard
     *  notifier events if it is not yet subscribed plus
     *  subscribes to the given custom event.
     */    
    void Subscribe( CObserver* observer  ,
                    const UInt32 eventid );

    /**
     *  Detaches the given observer from the notifier.
     *  All the observers subscriptions will be cancelled
     *  This includes both standard notifier events aswell 
     *  as custom events.
     */    
    void Unsubscribe( CObserver* observer );
                 
    /**     
     *  Cancels the observer's subscription to the given event.
     *
     *  Note that subscriptions to the standard notifier events 
     *  cannot be cancelled, attempts to do so will be ignored.
     */    
    void Unsubscribe( CObserver* observer  ,
                      const UInt32 eventid );

    static void RegisterEvents( void );
    
    /**
     *  Utility member function, saves you some effort in retrieving
     *  the unique id for the notifier subscribe event.
     *
     *  @return unique event id of the notifier subscribe event
     */
    UInt32 GetSubscribeEventID( void ) const;

    /**
     *  Utility member function, saves you some effort in retrieving
     *  the unique id for the notifier unsubscribe event.
     *
     *  @return unique event id of the notifier unsubscribe event
     */
    UInt32 GetUnsubscribeEventID( void ) const;
    
    /**
     *  Utility member function, saves you some effort in retrieving
     *  the unique id for the notifier modify event.
     *
     *  @return unique event id of the notifier modify event
     */    
    UInt32 GetModifyEventID( void ) const;

    /**
     *  Utility member function, saves you some effort in retrieving
     *  the unique id for the notifier destruction event.
     *
     *  @return unique event id of the notifier destruction event
     */    
    UInt32 GetDestructionEventID( void ) const;

    /**
     *  Decending classes should override this with the classname
     *  ie: a class named CMyClass should implement this member 
     *  function as: { return "MyNamespace::CMyClass"; }
     *
     *  Note that this mechanism is meant as a replacement for
     *  RTTI. You can accomplish the same with a dynamic_cast for 
     *  example. The catch is that RTTI has an implicit system-wide  
     *  performace impact !!!. Since the scenario's where you need 
     *  functionality like this are limited or even non-existent 
     *  we chose to use this approach.
     */
    virtual std::string GetTypeName( void ) const;

    protected:
    
    /**
     *  Dispatches the standard CNotifier::ModifyEvent
     *  Usefull for a notification system where you only care if
     *  a mutation is performed on an object.
     */
    void NotifyObservers( void );
    
    /**
     *  Dispatches the given eventid and eventData to all observers
     *  that are subscribed to all events and the observers that are subscribed
     *  to this specific eventid.
     *
     *  Note that the calling thread is the one in which the observer OnNotify 
     *  event handlers will be processed. Keep this in mind when notification
     *  occures across thread boundries.
     *
     *  Note that eventData is not copied. So when passing data across threads
     *  considder allocating a copy and passing that in as the data argument.
     */
    void NotifyObservers( const UInt32 eventid          ,
                          CICloneable* eventData = NULL );

    /**
     *  Dispatches the given eventid and eventData to all observers
     *  that are subscribed to all events and the observers that are subscribed
     *  to this specific eventid.
     *
     *  Note that the calling thread is the one in which the observer OnNotify 
     *  event handlers will be processed. Keep this in mind when notification
     *  occures across thread boundries.
     *
     *  Note that eventData is not copied. So when passing data across threads
     *  considder allocating a copy and passing that in as the data argument.
     */
    void NotifyObservers( const CString& eventName      ,
                          CICloneable* eventData = NULL );

    virtual void LockData( void );
    
    virtual void UnlockData( void );
                 
    private:
    typedef std::set<CObserver*> TObserverSet;
    typedef std::map<UInt32,TObserverSet> TNotificationList;
    typedef std::map<CObserver*,bool> TObserverList;

    private:
    friend class CObserver;

    /**
     *  Handler for observers that are deleted without having been
     *  unsubscribed first. This results in an observer that is no longer
     *  able to handle notification messages due to the deallocation 
     *  chain of events (decending class is already deallocated).
     *
     *  Note that if you code neatly you will want to call UnsubscribeAll()
     *  from your class destructor that decends from CObserver and implements
     *  OnNotify()
     */
    void OnObserverDestroy( CObserver* observer );

    private:
    void ForceNotifyObserversOnce( const UInt32 eventid     , 
                                   CICloneable* data = NULL );

    void UnsubscribeFromAllEvents( CObserver* observer       ,
                                   const bool notifyObserver );
    
    UInt32 m_modifyEvent;
    UInt32 m_destructionEvent;
    UInt32 m_subscribeEvent;
    UInt32 m_unsubscribeEvent;    
    TNotificationList m_eventobservers;
    TObserverList m_observers;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CNOTIFIER_H ? */
