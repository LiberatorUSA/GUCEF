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

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>
#include <map>

#include "CNotifierObservingComponent.h"
#include "CNotifier.h"

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
class GUCEFCORE_EXPORT_CPP CObservingNotifier : public CNotifier
{
    public:

    typedef enum TEventOriginFilter
    {
        EVENTORIGINFILTER_UNMODIFIED , /**< do not change the event emission origin */
        EVENTORIGINFILTER_NO_SENDER  , /**< void's the event emitter, NULL will be given as sender */
        EVENTORIGINFILTER_TRANSFER     /**< turns the re-sender into the emitter of the event */
    };
    
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
     *  assuming LockData() and UnlockData() are implemented.
     *  As such this operation can be made in a threadsafe fashion.     
     */
    CObserver& AsObserver( void );

    /**
     *  Returns a reference to the observer component. 
     *
     *  Note that retrieval actions on the observer component also lock
     *  the notification component in a multi-threading scenario
     *  assuming LockData() and UnlockData() are implemented.
     *  As such this operation can be made in a threadsafe fashion.            
     */
    const CObserver& AsObserver( void ) const;
    
    /**
     *  Adds the given event ID to the forwarding list.
     *
     *  This allows you to forward events from one notifier 
     *  trough this notifier. This is useful if you have a
     *  notifier who's derived class delegates tasks to 
     *  notifying sub-systems. Using forwarding you can create 
     *  the composite class and make it seem like a single system 
     *  to the observer.
     *
     *  Note that the instance of this class will become the sending
     *  notifier instead of the original notifier. As such it is recommended
     *  to create a event declaration interface from which both your exposed
     *  class and potential sub-system classes derive. This allows you
     *  to use forwarding without breaking event scoping rules.
     *
     *  @param eventid the ID specifying the event you wish to forward
     *  @param notifier if non-NULL only event's triggered from the 
     *  @param notifier given notifier are forwarded, otherwise the 
     *  @param notifier event is always forwarded
     */
    void AddEventForwarding( const CEvent& eventid                 ,
                             const TEventOriginFilter originFilter ,
                             CNotifier* notifier = NULL            );

    /**
     *  Removes the given event ID to the forwarding list.
     *
     *  See AddEventForwarding() for more details
     *
     *  @param eventid the ID specifying the event you wish to remove from forwarding
     *  @param notifier if non-NULL only event's triggered from the 
     *  @param notifier given notifier are removed from the forwarding list,  
     *  @param notifier otherwise the event is always forwarded
     */
    void RemoveEventForwarding( const CEvent& eventid      , 
                                CNotifier* notifier = NULL );
                             
    protected:
    friend class CNotifierObservingComponent;
    
    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  Note: Do NOT  forget to call this class's implementation
     *  from your descending class or you will break the event 
     *  forwarding mechanism.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );
                           
    private:
    
    /**
     *  Removes all references to the given notifier from our forwarding
     *  system
     */
    void RemoveEventForwarding( CNotifier& notifier );
                           
    private:    
    typedef std::set< CEvent > TEventList;
    typedef std::set< CNotifier* > TNotifierList;
    typedef std::map< CEvent, TNotifierList > TEventNotifierMap;
    
    CNotifierObservingComponent m_observer;        
    TEventList m_eventList;
    TEventNotifierMap m_eventNotifierMap;                              
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
