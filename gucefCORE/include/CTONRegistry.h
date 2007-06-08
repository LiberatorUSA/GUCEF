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

#ifndef GUCEF_CORE_CTONREGISTRY_H
#define GUCEF_CORE_CTONREGISTRY_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTREGISTRY_H
#include "CTRegistry.h"
#define GUCEF_CORE_CTREGISTRY_H
#endif /* GUCEF_CORE_CTREGISTRY_H ? */

#ifndef GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H
#include "CTObservingNotifierExpansion.h"
#define GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H
#endif /* GUCEF_CORE_CTOBSERVINGNOTIFIEREXPANSION_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

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
 *  Interface class for the events emitted by CTONRegistry template
 *  instantiations.
 */
class GUCEFCORE_EXPORT_CPP CTONRegistryEvents
{
    public:
    
    static const CEvent ItemRegisteredEvent;
    static const CEvent ItemUnregisteredEvent;
    
    typedef TCloneableString ItemRegisteredEventData;
    typedef TCloneableString ItemUnregisteredEventData;
    
    static void RegisterEvents( void );
    
    public:
    
    CTONRegistryEvents( void );
    CTONRegistryEvents( const CTONRegistryEvents& src );
    virtual ~CTONRegistryEvents();
    CTONRegistryEvents& operator=( const CTONRegistryEvents& src );
};

/*-------------------------------------------------------------------------*/

/**
 *  Template that expands the normal CTRegistry functionality with
 *  notification.
 */
template < class T >
class CTONRegistry : public CTObservingNotifierExpansion< CTRegistry< T > > ,
                     public CTONRegistryEvents /* <- interface */
{
    public:
    
    typedef typename CTObservingNotifierExpansion< CTRegistry< T > >::TRegisteredObjPtr TRegisteredObjPtr;
    typedef typename CTObservingNotifierExpansion< CTRegistry< T > >::TStringList TStringList;
    
    CTONRegistry( void );
    
    CTONRegistry( const CTONRegistry& src );
    
    virtual ~CTONRegistry();
    
    CTONRegistry& operator=( const CTONRegistry& src );
    
    virtual void Register( const CString& name                ,
                           const TRegisteredObjPtr& sharedPtr );
                   
    virtual void Unregister( const CString& name );                   
    
    virtual void UnregisterAll( void );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class T >
CTONRegistry< T >::CTONRegistry( void )
    : CTObservingNotifierExpansion< CTRegistry< T > >()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< class T >
CTONRegistry< T >::CTONRegistry( const CTONRegistry& src )
    : CTObservingNotifierExpansion< CTRegistry< T > >( src )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< class T >
CTONRegistry< T >::~CTONRegistry()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< class T >
CTONRegistry< T >& 
CTONRegistry< T >::operator=( const CTONRegistry& src )
{TRACE;
    if ( &src != this )
    {
        CTObservingNotifierExpansion< CTRegistry< T > >::operator=( src );
    }
    return *this;        
}

/*-------------------------------------------------------------------------*/
    
template< class T >
void 
CTONRegistry< T >::Register( const CString& name                ,
                             const TRegisteredObjPtr& sharedPtr )
{TRACE;

    LockData();
    
    CTObservingNotifierExpansion< CTRegistry< T > >::Register( name, sharedPtr );
    ItemRegisteredEventData eData( name );
    NotifyObservers( ItemRegisteredEvent, &eData );
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/
                   
template< class T >
void 
CTONRegistry< T >::Unregister( const CString& name )
{TRACE;

    LockData();
    
    CTObservingNotifierExpansion< CTRegistry< T > >::Unregister( name );
    ItemRegisteredEventData eData( name );
    NotifyObservers( ItemRegisteredEvent, &eData );
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTONRegistry< T >::UnregisterAll( void )
{TRACE;

    LockData();
    
    TStringList list;
    GetList( list );
    
    for ( UInt32 i=0; i<list.size(); ++i )
    {
        Unregister( list[ i ] );
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTONREGISTRY_H  ? */
