/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIERCOMPONENT_H
#define GUCEF_CORE_CTOBSERVINGNOTIFIERCOMPONENT_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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
 *  Template for encapsulating a CObservingNotifier instance in another class
 *  It will proxy the mandatory overloads to the component owner, allowing instances
 *  to be created without having to implement the pure virtuals.
 *
 *  Note that as a user you will most likely only want to use the
 *  CTObservingNotifierExpansion template. Check it out if all you want to do
 *  is add notification to your class.
 *
 *  Note: When using this template you must implement OnNotify() in your class with the
 *  same semantics as it is implemented here and provide the template instantiation
 *  with access to your OnNotify() (if so desired with a friend relation).
 *  You must also place 'varName.SetOwner( this )' in your class constructor where
 *  'varName' is the name the instance of this class template.
 */
template < class TOwnerClass >
class CTObservingNotifierComponent : public CObservingNotifier
{
    public:
    
    CTObservingNotifierComponent( void );

    CTObservingNotifierComponent( const CTObservingNotifierComponent& src );
    
    virtual ~CTObservingNotifierComponent();

    CTObservingNotifierComponent& operator=( const CTObservingNotifierComponent& src );
    
    /**
     *  Sets the callback owner class pointer.
     *  must be set in your class contructor using
     *      'SetOwner( this );'
     *
     *  @param ownerInstance pointer to the owner of the compound instance.
     */
    void SetOwner( TOwnerClass* ownerInstance );
    
    /**
     *  Dispatches the standard CNotifier::ModifyEvent
     *  Useful for a notification system where you only care if
     *  a mutation is performed on an object.
     *
     *  @return Alive state of the notifier, if false then the notifier died itself as a result of the notification.
     */
    bool DoNotifyObservers( void );
    
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
     *
     *  @return Alive state of the notifier, if false then the notifier died itself as a result of the notification.
     */
    bool DoNotifyObservers( const CEvent& eventid         ,
                            CICloneable* eventData = NULL );

    /**
     *  The same as DoNotifyObservers( CEvent, CICloneable )
     *  except that this allows you to specify the sender yourself.
     *  You will basically be faking an event emitted at the given sender.
     *  
     *  Use with great care !!!
     *  Use of this version should be an exception and not standard practice
     *
     *  @return Alive state of the notifier, if false then the notifier died itself as a result of the notification.
     */
    bool DoNotifyObservers( CNotifier& sender             ,
                            const CEvent& eventid         ,
                            CICloneable* eventData = NULL );
    
    protected:
    
    /**
     *  Event callback member function.
     *  Implement this in your owner class to handle
     *  notification events. This template will proxy the 
     *  callback to the owner class.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const UInt32 eventid          ,
                           CICloneable* eventdata = NULL );
    
    private:
    
    TOwnerClass* m_componentOwner;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < class TOwnerClass >
CTObservingNotifierComponent< TOwnerClass >::CTObservingNotifierComponent( void )
    : CObservingNotifier()     ,
      m_componentOwner( NULL )
{TRACE;

}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
CTObservingNotifierComponent< TOwnerClass >::CTObservingNotifierComponent( const CTObservingNotifierComponent& src )
    : CObservingNotifier( src ) ,
      m_componentOwner( NULL )
{TRACE;
    
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
CTObservingNotifierComponent< TOwnerClass >::~CTObservingNotifierComponent( void )
{TRACE;

    UnsubscribeAllFromObserver();
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
CTObservingNotifierComponent< TOwnerClass >&
CTObservingNotifierComponent< TOwnerClass >::operator=( const CTObservingNotifierComponent& src )
{TRACE;

    CObservingNotifier::operator=( src );
    
    if ( &src != this )
    {
        /* don't do anything, leave the owner pointer as is */
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
void 
CTObservingNotifierComponent< TOwnerClass >::SetOwner( TOwnerClass* ownerInstance )
{TRACE;

    m_componentOwner = ownerInstance;
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
bool
CTObservingNotifierComponent< TOwnerClass >::DoNotifyObservers( void )
{TRACE;

    return NotifyObservers();
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
bool
CTObservingNotifierComponent< TOwnerClass >::DoNotifyObservers( const CEvent& eventid               ,
                                                                CICloneable* eventData /* = NULL */ )
{TRACE;

    return NotifyObservers( eventid   ,
                            eventData );
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
bool
CTObservingNotifierComponent< TOwnerClass >::DoNotifyObservers( CNotifier& sender                   ,
                                                                const CEvent& eventid               ,
                                                                CICloneable* eventData /* = NULL */ )
{TRACE;

    return NotifyObservers( sender    ,
                            eventid   ,
                            eventData );
}

/*-------------------------------------------------------------------------*/

template < class TOwnerClass >
void
CTObservingNotifierComponent< TOwnerClass >::OnNotify( CNotifier* notifier                 ,
                                                       const UInt32 eventid                ,
                                                       CICloneable* eventdata /* = NULL */ )
{TRACE;

    /*
     *  If you crash here then you forgot to call SetOwner() in your
     *  class constructor.
     */
    assert( m_componentOwner );
    
    /*
     *  Simply proxy the call to the owner class.
     *  This is the purpose of this template 
     */
    m_componentOwner->OnNotify( notifier  ,
                                eventid   ,
                                eventdata );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTOBSERVINGNOTIFIERCOMPONENT_H  ? */
