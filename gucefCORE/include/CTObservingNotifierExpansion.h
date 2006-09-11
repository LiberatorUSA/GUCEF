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

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H
#define GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CTObservingNotifierComponent.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_CPP
    #pragma warning( push )
#endif

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
 *  Class that allows you to add notification to any class with ease.
 *  The template argument is a base-class that will be expanded with
 *  notification capabilities.
 */
template < class BaseClass >
class CTObservingNotifierExpansion : public BaseClass
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
     *  Implement this in your decending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const UInt32 eventid          ,
                           CICloneable* eventdata = NULL );
    
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
    : BaseClass()
{
    m_notificationComponent.SetOwner( this );
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CTObservingNotifierExpansion< BaseClass >::CTObservingNotifierExpansion( const CTObservingNotifierExpansion& src )
    : BaseClass( src )
{
    m_notificationComponent.SetOwner( this );
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CTObservingNotifierExpansion< BaseClass >::~CTObservingNotifierExpansion()
{
    m_notificationComponent.UnsubscribeAllFromObserver();
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CTObservingNotifierExpansion< BaseClass >&
CTObservingNotifierExpansion< BaseClass >::operator=( const CTObservingNotifierExpansion< BaseClass >& src )
{
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
{
    return m_notificationComponent.AsObserver();  //TICS !GOOP-002: special case, this is a compound object and should be seen as 1 conceptual oject
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
const CObserver& 
CTObservingNotifierExpansion< BaseClass >::AsObserver( void ) const
{
    return m_notificationComponent.AsObserver();
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
CNotifier& 
CTObservingNotifierExpansion< BaseClass >::AsNotifier( void )
{
    return m_notificationComponent;  //TICS !GOOP-002: special case, this is a compound object and should be seen as 1 conceptual oject
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
const CNotifier& 
CTObservingNotifierExpansion< BaseClass >::AsNotifier( void ) const
{
    return m_notificationComponent;
}

/*-------------------------------------------------------------------------*/

template < class BaseClass >
void
CTObservingNotifierExpansion< BaseClass >::OnNotify( CNotifier* /* notifier */                ,
                                                     const UInt32 /* eventid */               ,
                                                     CICloneable* /* eventdata *//* = NULL */ )
{
    /* implemented to avoid manditory implementation by decending classes */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H  ? */
