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

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#define GUCEF_CORE_CTSGNOTIFIER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CTSGObserver.h"       /* observer implementation used internally by the CTSGNotifier */
#include "CNotifier.h"          /* notification base class */

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
 *  ThreadSafe Gateway Notifier class a.k.a TSGNotifier class a.k.a CTSGNotifier
 *
 *  Implements a base class for notifiers that can listnen to another notifier
 *  that is dispatching events from within a thread.
 *  Using this mechanism you can use a Notifier-Observer based design while
 *  mixing threadsafe code with non-threadsafe code.
 *  So basicly like the name says,... it is a messaging gateway between 
 *  threadsafe code and non-threadsafe code.
 *
 *  Note that if you do not override OnPumpedNotify() then the instance
 *  will simply function as a pass-trough notifier. Override to manipulate 
 *  the event data before it is sent to the observers.
 *  Keep in mind that the CTSGNotifier will become the source notifier !!!
 */
class GUCEFCORE_EXPORT_CPP CTSGNotifier : public CNotifier
{
    public:
    
    CTSGNotifier( void );
    
    CTSGNotifier( const CTSGNotifier& src );
    
    virtual ~CTSGNotifier();
    
    CTSGNotifier& operator=( const CTSGNotifier& src );
    
    /**
     *  Subscribes to the given notifier to all
     *  notification events. Every event dispatched by 
     *  the threadedNotifier will be sent to the
     *  instance of this class.
     */    
    void SubscribeTo( CNotifier* threadedNotifier );
    
    void UnsubscribeFrom( CNotifier* threadedNotifier );
    
    /**
     *  Subscribes to the given notifier's four standard
     *  notifier events, if it is not yet subscribed, plus
     *  subscribes to the given custom event.
     */    
    void SubscribeTo( CNotifier* threadedNotifier ,
                      const CEvent& eventid       );
                      
    /**     
     *  Cancels the subscription at the given threadedNotifier
     *  for the given event.
     *
     *  Note that subscriptions to the standard notifier events 
     *  cannot be cancelled, attempts to do so will be ignored.
     */    
    void UnsubscribeFrom( CNotifier* threadedNotifier ,
                          const CEvent& eventid       );                          
    
    protected:
    friend class CTSGObserver;
    
    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events. 
     *
     *  Note that this call is always made in the main
     *  application thread. As such it is well suited for linking
     *  non-treadsafe code via an observer behaviour to a notifier  
     *  that operates from within another thread.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnPumpedNotify( CNotifier* notifier           ,
                                 const CEvent& eventid         ,
                                 CICloneable* eventdata = NULL );    
    
    private:
    
    CTSGObserver m_tsgObserver;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTSGNOTIFIER_H  ? */