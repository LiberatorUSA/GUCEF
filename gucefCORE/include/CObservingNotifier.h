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

#include "CNotifierObservingComponent.h"
#include "CNotifier.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_CPP
    #pragma warning( push )
#endif

#pragma warning( disable: 4018 ) // signed/unsigned mismatch
#pragma warning( disable: 4146 ) // unary minus operator applied to unsigned type, result still unsigned
#pragma warning( disable: 4251 ) // 'classname' needs to have dll-interface to be used by clients of class 'classname'
#pragma warning( disable: 4284 ) // return type for operator -> is 'const *' (ie; not a UDT or reference to a UDT).
#pragma warning( disable: 4786 ) // identifier was truncated to 'number' characters

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
 *  Class that combines a notifier with observer behaviour
 *  Mainly to be used for node objects in your chain-of-events
 *  tree
 */
class GUCEFCORE_EXPORT_CPP CObservingNotifier : public CNotifier
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
    void SubscribeTo( CNotifier* notifier  ,
                      const UInt32 eventid );

    /**
     *  Detaches the observer component from the notifier.
     *  All the observers subscriptions will be cancelled
     *  This includes both standard notifier events aswell 
     *  as custom events.
     */    
    void UnsubscribeFrom( CNotifier* notifier );
                 
    /**     
     *  Cancels the observer component's subscription to the given event.
     *
     *  Note that subscriptions to the standard notifier events 
     *  cannot be cancelled, attempts to do so will be ignored.
     */    
    void UnsubscribeFrom( CNotifier* notifier  ,
                          const UInt32 eventid );

    /**
     *  Returns a refrence to the mutable observer component.
     *
     *  Note that mutations to the observer component also lock
     *  the notification component in a multi-threading scenario
     *  assuming LockData() and UnlockData() are implemented.
     *  As such this operation can be made in a threadsafe fashion.     
     */
    CObserver& AsObserver( void );

    /**
     *  Returns a refrence to the observer component. 
     *
     *  Note that retrieval actions on the observer component also lock
     *  the notification component in a multi-threading scenario
     *  assuming LockData() and UnlockData() are implemented.
     *  As such this operation can be made in a threadsafe fashion.            
     */
    const CObserver& AsObserver( void ) const;

    protected:
    friend class CNotifierObservingComponent;
    
    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const UInt32 eventid          ,
                           CICloneable* eventdata = NULL ) = 0;
                           
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

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */
