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

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H
#define GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIOBSERVER_H
#include "gucefCORE_CIObserver.h"
#define GUCEF_CORE_CIOBSERVER_H
#endif /* GUCEF_CORE_CIOBSERVER_H ? */

#include "CTObservingNotifierComponent.h"

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
 *  Class that allows you to add notification to any class with ease.
 *  Creates a compound class relationship for you that gives you the notification system
 *  combined with the chosen base class without having to resort to multiple inheritance
 *  The template argument is a base-class that will be expanded with
 *  notification capabilities.
 */
template < class BaseClass >
class CTObservingNotifierExpansion : public BaseClass  ,
                                     public CIObserver
{
    public:
    
    CTObservingNotifierExpansion( void );

    CTObservingNotifierExpansion( const CTObservingNotifierExpansion& src );
    
    virtual ~CTObservingNotifierExpansion();

    CTObservingNotifierExpansion& operator=( const CTObservingNotifierExpansion& src );
    
    /**
     *  Provides mutable access to the notifier capabilities of your class
     */
    CNotifier& AsNotifier( void );
    
    /**
     *  Provides read-only access to the notifier capabilities of your class
     */
    const CNotifier& AsNotifier( void ) const;

    /**
     *  Provides mutable access to the observer capabilities of your class
     */
    CObserver& AsObserver( void );
    
    /**
     *  Provides read-only access to the observer capabilities of your class
     */
    const CObserver& AsObserver( void ) const;
    
    protected:
    
    typedef CTObservingNotifierExpansion< BaseClass >    TONExpansion;
    
    friend class CTObservingNotifierComponent< TONExpansion >;
    
    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );

    /**
     *  Dispatches the standard CNotifier::ModifyEvent
     *  Useful for a notification system where you only care if
     *  a mutation is performed on an object.
     *
     *  @return Alive state of the notifier, if false then the notifier died itself as a result of the notification.
     */
    bool NotifyObservers( void );
    
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
    bool NotifyObservers( const CEvent& eventid         ,
                          CICloneable* eventData = NULL );

    /**
     *  The same as NotifyObservers( CEvent, CICloneable )
     *  except that this allows you to specify the sender yourself.
     *  You will basically be faking an event emitted at the given sender.
     *  
     *  Use with great care !!!
     *  Use of this version should be an exception and not standard practice
     *
     *  @return Alive state of the notifier, if false then the notifier died itself as a result of the notification.
     */
    bool NotifyObservers( CNotifier& sender             ,
                          const CEvent& eventid         ,
                          CICloneable* eventData = NULL );
                              
    private:
    
    typedef CTObservingNotifierComponent< TONExpansion > TObservingNotifierComponent;
    
    TObservingNotifierComponent m_notificationComponent; /**< the notification component that houses the actual mechanisms */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < class BaseClass >
CTObservingNotifierExpansion< BaseClass >::CTObservingNotifierExpansion( void )
    : BaseClass()               ,
      m_notificationComponent()
{GUCEF_TRACE;

    m_notificationComponent.SetOwner( this );
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CTObservingNotifierExpansion< BaseClass >::CTObservingNotifierExpansion( const CTObservingNotifierExpansion& src )
    : BaseClass( src )          ,
      m_notificationComponent()
{GUCEF_TRACE;

    m_notificationComponent.SetOwner( this );
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CTObservingNotifierExpansion< BaseClass >::~CTObservingNotifierExpansion()
{GUCEF_TRACE;

    m_notificationComponent.UnsubscribeAllFromObserver();
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CTObservingNotifierExpansion< BaseClass >&
CTObservingNotifierExpansion< BaseClass >::operator=( const CTObservingNotifierExpansion< BaseClass >& src )
{GUCEF_TRACE;

    BaseClass::operator=( src );

    if ( &src != this )
    {
        m_notificationComponent = src.m_notificationComponent;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CObserver& 
CTObservingNotifierExpansion< BaseClass >::AsObserver( void )
{GUCEF_TRACE;

    return m_notificationComponent.AsObserver();
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
const CObserver& 
CTObservingNotifierExpansion< BaseClass >::AsObserver( void ) const
{GUCEF_TRACE;

    return m_notificationComponent.AsObserver();
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CNotifier& 
CTObservingNotifierExpansion< BaseClass >::AsNotifier( void )
{GUCEF_TRACE;

    return m_notificationComponent;
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
const CNotifier& 
CTObservingNotifierExpansion< BaseClass >::AsNotifier( void ) const
{GUCEF_TRACE;

    return m_notificationComponent;
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
bool
CTObservingNotifierExpansion< BaseClass >::NotifyObservers( void )
{GUCEF_TRACE;

    return m_notificationComponent.DoNotifyObservers();
}

/*-------------------------------------------------------------------------*/
    
template < class BaseClass >
bool
CTObservingNotifierExpansion< BaseClass >::NotifyObservers( const CEvent& eventid               ,
                                                            CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;

    return m_notificationComponent.DoNotifyObservers( eventid   ,
                                                      eventData );
}

/*-------------------------------------------------------------------------*/
    
template < class BaseClass >
bool
CTObservingNotifierExpansion< BaseClass >::NotifyObservers( CNotifier& sender                   ,
                                                            const CEvent& eventid               ,
                                                            CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;

    return m_notificationComponent.DoNotifyObservers( sender    ,
                                                      eventid   ,
                                                      eventData );
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
void
CTObservingNotifierExpansion< BaseClass >::OnNotify( CNotifier* /* notifier */                ,
                                                     const CEvent& /* eventid */              ,
                                                     CICloneable* /* eventdata *//* = NULL */ )
{GUCEF_TRACE;

    /* implemented to avoid mandatory implementation by descending classes */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H  ? */
