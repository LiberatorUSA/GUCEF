/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CTEVENTHANDLERFUNCTORPROXY_H
#define GUCEF_CORE_CTEVENTHANDLERFUNCTORPROXY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#include "gucefCORE_CTEventHandlerFunctor.h"
#define GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#endif /* GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASS TEMPLATES                                                    //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Same purpose as the CTEventHandlerFunctor<> but with an additional layer 
 *  of indirection. Allows you to make an indirect functor invocation where
 *  the original event handler is passed as an additonal param.
 *
 *  The main reason to use this is for cross-thread callbacks where you need
 *  to ensure that within a certain context only a given thread is active.
 *  Doing so allows one to simplify the threading management.
 *  Do note that when used as such, while it allows the thread invoking the callback
 *  do be controlled, you still need to ensure the lifespan of the observer to avoid
 *  the callback from becoming invalid
 */
template < class IObserverDerived >
class CTEventHandlerFunctorProxy : public virtual CIEventHandlerFunctorBase
{
    public:

    // Member function pointer type definition.
    typedef void (IObserverDerived::*TMemberFunctionType)( CNotifier* notifier                       ,
                                                           const CEvent& eventId                     ,
                                                           CICloneable* evenData                     ,
                                                           CIEventHandlerFunctorBase* actualCallback );

    CTEventHandlerFunctorProxy( IObserverDerived* observer                ,
                                TMemberFunctionType functor               ,
                                CIEventHandlerFunctorBase* actualCallback );

    CTEventHandlerFunctorProxy( const CTEventHandlerFunctorProxy& src );

    virtual ~CTEventHandlerFunctorProxy();

    CTEventHandlerFunctorProxy& operator=( const CTEventHandlerFunctorProxy& src );

    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual const CIObserver* GetTargetObserver( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CIObserver* GetTargetObserver( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnNotify( CNotifier* notifier                ,
                           const CEvent& eventId              ,
                           CICloneable* evenData = GUCEF_NULL ) GUCEF_VIRTUAL_OVERRIDE;

    void SetMemberFunctionPointer( TMemberFunctionType functor );
    
    private:

    CTEventHandlerFunctorProxy( void ); /**< dummy, dont use */

    private:

    IObserverDerived* m_observer;
    TMemberFunctionType m_functor;
    CIEventHandlerFunctorBase* m_actualCallback;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class IObserverDerived >
CTEventHandlerFunctorProxy< IObserverDerived >::CTEventHandlerFunctorProxy( IObserverDerived* observer                ,
                                                                            TMemberFunctionType functor               ,
                                                                            CIEventHandlerFunctorBase* actualCallback )
    : CIEventHandlerFunctorBase()
    , m_observer( observer )      
    , m_functor( functor )
    , m_actualCallback( GUCEF_NULL )
{GUCEF_TRACE;

    m_actualCallback = static_cast< CIEventHandlerFunctorBase* >( actualCallback->Clone() );
    assert( GUCEF_NULL != m_actualCallback );
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
CTEventHandlerFunctorProxy< IObserverDerived >::CTEventHandlerFunctorProxy( const CTEventHandlerFunctorProxy& src )
    : CIEventHandlerFunctorBase( src )
    , m_observer( src.m_observer )     
    , m_functor( src.m_functor )
    , m_actualCallback( GUCEF_NULL )
{GUCEF_TRACE;

    m_actualCallback = static_cast< CIEventHandlerFunctorBase* >( src.m_actualCallback->Clone() );
    assert( GUCEF_NULL != m_actualCallback );
}

/*-------------------------------------------------------------------------*/

template< typename IObserverDerived >
CTEventHandlerFunctorProxy< IObserverDerived >::~CTEventHandlerFunctorProxy()
{GUCEF_TRACE;

    GUCEF_DELETE m_actualCallback;
    m_actualCallback = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
CTEventHandlerFunctorProxy< IObserverDerived >&
CTEventHandlerFunctorProxy< IObserverDerived >::operator=( const CTEventHandlerFunctorProxy& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CIEventHandlerFunctorBase::operator=( src );

        m_observer = src.m_observer;
        m_functor = src.m_functor;
        
        GUCEF_DELETE m_actualCallback;
        m_actualCallback = static_cast< CIEventHandlerFunctorBase* >( src.m_actualCallback->Clone() );
        assert( GUCEF_NULL != m_actualCallback );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
const CString&
CTEventHandlerFunctorProxy< IObserverDerived >::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CTEventHandlerFunctorProxy< IObserverDerived >";
    return typeName;
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
void
CTEventHandlerFunctorProxy< IObserverDerived >::OnNotify( CNotifier* notifier   ,
                                                          const CEvent& eventID ,
                                                          CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG_EVERYTHING( "CTEventHandlerFunctorProxy(" + CORE::PointerToString( this ) + "): Class " + notifier->GetClassTypeName() + 
        ": Dispatching event \"" + eventID.GetName() + "\" to " + m_observer->GetClassTypeName() + "(" + CORE::PointerToString( m_observer ) + ")" );

    (m_observer->*m_functor)( notifier         ,
                              eventID          ,
                              evenData         ,
                              m_actualCallback ); // <- Adding the ability to pass in the original callback via indirection is the whole purpose of this template
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
CICloneable*
CTEventHandlerFunctorProxy< IObserverDerived >::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CTEventHandlerFunctorProxy< IObserverDerived >( *this );
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
const CIObserver* 
CTEventHandlerFunctorProxy< IObserverDerived >::GetTargetObserver( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_actualCallback )
    {
        return m_actualCallback->GetTargetObserver();
    }
    return static_cast< const CIObserver* >( m_observer );
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
CIObserver* 
CTEventHandlerFunctorProxy< IObserverDerived >::GetTargetObserver( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_actualCallback )
    {
        return m_actualCallback->GetTargetObserver();
    }
    return static_cast< CIObserver* >( m_observer );
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
void
CTEventHandlerFunctorProxy< IObserverDerived >::SetMemberFunctionPointer( TMemberFunctionType functor )
{GUCEF_TRACE;

    m_functor = functor;
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
const MT::CILockable* 
CTEventHandlerFunctorProxy< IObserverDerived >::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
bool
CTEventHandlerFunctorProxy< IObserverDerived >::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
bool
CTEventHandlerFunctorProxy< IObserverDerived >::Unlock( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTEVENTHANDLERFUNCTORPROXY_H ? */
