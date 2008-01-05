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

#ifndef GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#define GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_COBSERVER_H
#include "CObserver.h"
#define GUCEF_CORE_COBSERVER_H
#endif /* GUCEF_CORE_COBSERVER_H ? */

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

template < class IObserverDerived >
class CTEventHandlerFunctor : public CIObserver
{
    public:

    // Member function pointer type definition.
    typedef void (IObserverDerived::*TMemberFunctionType)( CNotifier* notifier   , 
                                                           const CEvent& event   , 
                                                           CICloneable* evenData );
    
    CTEventHandlerFunctor( IObserverDerived* observer  ,
                           TMemberFunctionType functor );
    
    CTEventHandlerFunctor( const CTEventHandlerFunctor& src );
    
    virtual ~CTEventHandlerFunctor();
    
    CTEventHandlerFunctor& operator=( const CTEventHandlerFunctor& src );
    
    virtual void OnNotify( CNotifier* notifier   , 
                           const CEvent& eventID ,
                           CICloneable* evenData );

    private:
    
    CTEventHandlerFunctor( void );
    
    private:
    
    IObserverDerived* m_observer;
    TMemberFunctionType m_functor;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class IObserverDerived >
CTEventHandlerFunctor< IObserverDerived >::CTEventHandlerFunctor( IObserverDerived* observer  ,
                                                                  TMemberFunctionType functor )
    : m_observer( observer )   ,
      m_functor( functor )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
CTEventHandlerFunctor< IObserverDerived >::CTEventHandlerFunctor( const CTEventHandlerFunctor& src )
    :  m_observer( src.m_observer ) ,
       m_functor( src.m_functor )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename IObserverDerived >
CTEventHandlerFunctor< IObserverDerived >::~CTEventHandlerFunctor()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
CTEventHandlerFunctor< IObserverDerived >&
CTEventHandlerFunctor< IObserverDerived >::operator=( const CTEventHandlerFunctor& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_observer = src.m_observer;
        m_functor = src.m_functor;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< class IObserverDerived >
void
CTEventHandlerFunctor< IObserverDerived >::OnNotify( CNotifier* notifier   , 
                                                     const CEvent& eventID ,
                                                     CICloneable* evenData )
{GUCEF_TRACE; 
    
    (m_observer->*m_functor)( notifier ,
                              eventID  ,
                              evenData );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
