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
#include <vector>
#include <map>
#include "gucefCORE_ETypes.h"
#include "CICloneable.h"
#include "CITypeNamed.h"

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

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
 *  in the event strings. Following this simply rule will ensure that your strings
 *  are unique simply because your namespace::class::event combination is unique in 
 *  C++ as well.
 *
 *  Note that this class automatically registers four notification events
 *  if they are not already registered. These are:
 *      - "GUCEF::CORE::CNotifier::SubscribeEvent"
 *      - "GUCEF::CORE::CNotifier::UnsubscribeEvent"
 *      - "GUCEF::CORE::CNotifier::ModifyEvent"
 *      - "GUCEF::CORE::CNotifier::DestructionEvent"
 *
 *  Note that every observer will be automatically subscribed to
 *  the four standard notification events.
 *
 *  Note that CNotifier is NOT threadsafe on it's own to avoid introducing
 *  unnecessary overhead. If you want a threadsafe notifier then simply
 *  implement LockData() and UnlockData() yourself in your descending class.
 *
 *  Current known issues:
 *  - chain reactions occurring during the handling of an event that access
 *    the notifier indirectly can cause errors 
 */
class GUCEFCORE_EXPORT_CPP CNotifier : public CITypeNamed
{
    public:

    static const CEvent SubscribeEvent;
    static const CEvent UnsubscribeEvent;
    static const CEvent ModifyEvent;
    static const CEvent DestructionEvent;

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
    void Subscribe( CObserver* observer   ,
                    const CEvent& eventid );

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
    void Unsubscribe( CObserver* observer   ,
                      const CEvent& eventid );

    static void RegisterEvents( void );

    /**
     *  descending classes should override this with the class name
     *  ie: a class named CMyClass should implement this member 
     *  function as: { return "MyNamespace::CMyClass"; }
     *
     *  Note that this mechanism is meant as a replacement for
     *  RTTI. You can accomplish the same with a dynamic_cast for 
     *  example. The catch is that RTTI has an implicit system-wide  
     *  performance impact !!!. Since the scenario's where you need 
     *  functionality like this are limited or even non-existent 
     *  we chose to use this approach.
     */
    virtual CString GetType( void ) const;

    protected:
    
    /**
     *  Dispatches the standard CNotifier::ModifyEvent
     *  Useful for a notification system where you only care if
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
     *  occurs across thread boundaries.
     *
     *  Note that eventData is not copied. So when passing data across threads
     *  consider allocating a copy and passing that in as the data argument.
     */
    void NotifyObservers( const CEvent& eventid         ,
                          CICloneable* eventData = NULL );

    /**
     *  The same as NotifyObservers( CEvent, CICloneable )
     *  except that this allows you to specify the sender yourself.
     *  You will basically be faking an event emitted at the given sender.
     *  
     *  Use with great care !!!
     *  Use of this version should be an exception and not standard practice
     */
    void NotifyObservers( CNotifier& sender             ,
                          const CEvent& eventid         ,
                          CICloneable* eventData = NULL );

    virtual void LockData( void ) const;
    
    virtual void UnlockData( void ) const;
                 
    private:
    typedef std::set<CObserver*> TObserverSet;
    typedef std::map<CEvent,TObserverSet> TNotificationList;
    typedef std::map<CObserver*,bool> TObserverList;

    private:
    friend class CObserver;

    /**
     *  Handler for observers that are deleted without having been
     *  unsubscribed first. This results in an observer that is no longer
     *  able to handle notification messages due to the deallocation 
     *  chain of events (descending class is already deallocated).
     *
     *  Note that if you code neatly you will want to call UnsubscribeAll()
     *  from your class destructor that descends from CObserver and implements
     *  OnNotify()
     */
    void OnObserverDestroy( CObserver* observer );

    private:
   
    void ForceNotifyObserversOnce( const CEvent& eventid    ,
                                   CICloneable* data = NULL );

    void UnsubscribeFromAllEvents( CObserver* observer       ,
                                   const bool notifyObserver );

    void ProcessMailbox( void );
    void ProcessCmdMailbox( void );
    void ProcessEventMailbox( void );

    private:
    
    typedef std::pair< CEvent, CICloneable* > TEventMailElement;
    typedef enum TCmdType
    {
        REQUEST_SUBSCRIBE   ,
        REQUEST_UNSUBSCRIBE
    };
    
    struct SCmdMailElement
    {
        TCmdType cmdType;
        CObserver* observer;
        CEvent eventID;
        bool notify;
    };
    typedef struct SCmdMailElement TCmdMailElement;    
        
    TNotificationList m_eventobservers;
    TObserverList m_observers;
    bool m_isBusy;
    std::vector< TEventMailElement > m_eventMailStack;
    std::vector< TCmdMailElement > m_cmdMailStack;
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
