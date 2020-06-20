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
class GUCEF_CORE_PUBLIC_CPP CTONRegistryEvents
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
    typedef CTObservingNotifierExpansion< CTRegistry< T > > TExpansionBase;

    CTONRegistry( void );

    CTONRegistry( const CTONRegistry& src );

    virtual ~CTONRegistry();

    CTONRegistry& operator=( const CTONRegistry& src );

    virtual void Register( const CString& name                ,
                           const TRegisteredObjPtr& sharedPtr ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool TryRegister( const CString& name                ,
                              const TRegisteredObjPtr& sharedPtr ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void Unregister( const CString& name ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void UnregisterAll( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class T >
CTONRegistry< T >::CTONRegistry( void )
    : CTObservingNotifierExpansion< CTRegistry< T > >()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T >
CTONRegistry< T >::CTONRegistry( const CTONRegistry& src )
    : CTObservingNotifierExpansion< CTRegistry< T > >( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T >
CTONRegistry< T >::~CTONRegistry()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T >
CTONRegistry< T >&
CTONRegistry< T >::operator=( const CTONRegistry& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CTObservingNotifierExpansion< CTRegistry< T > >::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< class T >
const CString&
CTONRegistry< T >::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CTONRegistry< T >";
    return typeName;
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTONRegistry< T >::Register( const CString& name                ,
                             const TRegisteredObjPtr& sharedPtr )
{GUCEF_TRACE;

    TExpansionBase::Lock();

    try
    {
        CTObservingNotifierExpansion< CTRegistry< T > >::Register( name, sharedPtr );
        ItemRegisteredEventData eData( name );
        TExpansionBase::NotifyObservers( ItemRegisteredEvent, &eData );
    }
    catch ( typename TExpansionBase::EAlreadyRegistered& e )
    {
        TExpansionBase::Unlock();
        throw e;
    }
    TExpansionBase::Unlock();
}

/*-------------------------------------------------------------------------*/

template< class T >
bool
CTONRegistry< T >::TryRegister( const CString& name                ,
                                const TRegisteredObjPtr& sharedPtr )
{GUCEF_TRACE;

    TExpansionBase::Lock();

    bool wasRegistered = CTObservingNotifierExpansion< CTRegistry< T > >::TryRegister( name, sharedPtr );
    if ( wasRegistered )
    {
        ItemRegisteredEventData eData( name );
        TExpansionBase::NotifyObservers( ItemRegisteredEvent, &eData );
    }

    TExpansionBase::Unlock();

    return wasRegistered;
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTONRegistry< T >::Unregister( const CString& name )
{GUCEF_TRACE;

    TExpansionBase::Lock();

    CTObservingNotifierExpansion< CTRegistry< T > >::Unregister( name );
    ItemRegisteredEventData eData( name );
    TExpansionBase::NotifyObservers( ItemRegisteredEvent, &eData );

    TExpansionBase::Unlock();
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTONRegistry< T >::UnregisterAll( void )
{GUCEF_TRACE;

    TExpansionBase::Lock();

    TStringList list;
    this->GetList( list );

    for ( UInt32 i=0; i<list.size(); ++i )
    {
        Unregister( list[ i ] );
    }

    TExpansionBase::Unlock();
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
