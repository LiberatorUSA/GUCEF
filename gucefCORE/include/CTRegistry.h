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

#ifndef GUCEF_CORE_CTREGISTRY_H
#define GUCEF_CORE_CTREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>                  /* standard variable mapping template */
#include <string>               /* standard string template implementation */
#include <vector>               /* standard dynamic array implementation */
#include "CTracer.h"            /* stack tracing */
#include "gucefCORE_ETypes.h"   /* some basic libary types */
#include "CDVString.h"          /* GUCEF platform string implementation */
#include "ExceptionMacros.h"    /* macros for dealing with scope typed exceptions */
#include "CINamedInstance.h"    /* basic name retrieval interface */
#include "CTSharedPtr.h"        /* pointer to the shared object */

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
 *  The Registry provides a mechanism for storing data centrally in a well
 *  managed fashion, helping to prevent object management chaos.
 *  The usage of shared pointers will ensure safe fire-and-forget usage
 *  where object access is concerned
 */
template < class T >
class CTRegistry
{
    public:
    typedef CTSharedPtr< T > TRegisteredObjPtr;
    typedef std::vector< CString > TStringList;

    CTRegistry( void );

    CTRegistry( const CTRegistry& src );

    virtual ~CTRegistry();

    CTRegistry& operator=( const CTRegistry& src );

    virtual bool IsRegistered( const CString& name ) const;

    virtual TRegisteredObjPtr Lookup( const CString& name ) const;

    virtual bool TryLookup( const CString& name           ,
                            TRegisteredObjPtr& locatedObj ,
                            bool caseSensitive = true     ) const;

    virtual void Register( const CString& name                ,
                           const TRegisteredObjPtr& sharedPtr );

    virtual void Unregister( const CString& name );

    virtual void UnregisterAll( void );

    virtual UInt32 GetCount( void ) const;

    virtual void GetList( TStringList& destList ) const;

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( EAlreadyRegistered );
    GUCEF_DEFINE_INLINED_MSGEXCEPTION( EUnregisteredName );

    protected:

    virtual void LockData( void );

    virtual void UnlockData( void );

    private:

    typedef std::map< CString, TRegisteredObjPtr* > TRegisteredObjList;

    TRegisteredObjList m_list;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class T >
CTRegistry< T >::CTRegistry( void )
    : m_list()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T >
CTRegistry< T >::CTRegistry( const CTRegistry& src )
    : m_list()
{GUCEF_TRACE;

    typename TRegisteredObjList::const_iterator i = src.m_list.begin();
    while ( i != src.m_list.end() )
    {
        m_list[ (*i).first ] = new TRegisteredObjPtr( *( (*i).second ) );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

template< class T >
CTRegistry< T >::~CTRegistry()
{GUCEF_TRACE;

    UnregisterAll();
}

/*-------------------------------------------------------------------------*/

template< class T >
CTRegistry< T >&
CTRegistry< T >::operator=( const CTRegistry& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        UnregisterAll();

        typename TRegisteredObjList::const_iterator i = src.m_list.begin();
        while ( i != src.m_list.end() )
        {
            m_list[ (*i).first ] = new TRegisteredObjPtr( *( (*i).second ) );
            ++i;
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< class T >
bool
CTRegistry< T >::IsRegistered( const CString& name ) const
{GUCEF_TRACE;

    return m_list.find( name ) != m_list.end();
}

/*-------------------------------------------------------------------------*/

template< class T >
CTSharedPtr< T >
CTRegistry< T >::Lookup( const CString& name ) const
{GUCEF_TRACE;

    typename TRegisteredObjList::const_iterator i = m_list.find( name );
    if ( i != m_list.end() )
    {
        // create a stack copy which is then returned.
        return *( (*i).second );
    }

    GUCEF_EMSGTHROW( EUnregisteredName, "gucefCORE::CTRegistry::Lookup(): unregistered name given" );
}

/*-------------------------------------------------------------------------*/

template< class T >
bool
CTRegistry< T >::TryLookup( const CString& name           ,
                            TRegisteredObjPtr& locatedObj ,
                            bool caseSensitive            ) const
{GUCEF_TRACE;

    if ( caseSensitive )
    {
        typename TRegisteredObjList::const_iterator i = m_list.find( name );
        if ( i != m_list.end() )
        {
            locatedObj = *( (*i).second );
            return true;
        }
        locatedObj = NULL;
        return false;
    }
    else
    {
        typename TRegisteredObjList::const_iterator i = m_list.begin();
        while ( i != m_list.end() )
        {
            if ( (*i).first.Equals( name, caseSensitive ) )
            {
                locatedObj = *( (*i).second );
                return true;
            }
            ++i;
        }
        locatedObj = NULL;
        return false;
    }
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTRegistry< T >::Register( const CString& name                ,
                           const TRegisteredObjPtr& sharedPtr )
{GUCEF_TRACE;

    typename TRegisteredObjList::iterator i = m_list.find( name );
    if ( i == m_list.end() )
    {
        m_list[ name ] = new TRegisteredObjPtr( sharedPtr );
        return;
    }

    GUCEF_EMSGTHROW( EAlreadyRegistered, "gucefCORE::CTRegistry::Register(): the given name is already registered" );
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTRegistry< T >::Unregister( const CString& name )
{GUCEF_TRACE;

    typename TRegisteredObjList::iterator i = m_list.find( name );
    if ( i != m_list.end() )
    {
        delete (*i).second;
        m_list.erase( i );
        return;
    }

    GUCEF_EMSGTHROW( EUnregisteredName, "gucefCORE::CTRegistry::Unregister(): unregistered name given" );
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTRegistry< T >::UnregisterAll( void )
{GUCEF_TRACE;

    typename TRegisteredObjList::iterator i = m_list.begin();
    while ( i != m_list.end() )
    {
        delete (*i).second;
        ++i;
    }
    m_list.clear();
}

/*-------------------------------------------------------------------------*/

template< class T >
UInt32
CTRegistry< T >::GetCount( void ) const
{GUCEF_TRACE;
    return static_cast< UInt32 >( m_list.size() );
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTRegistry< T >::GetList( TStringList& destList ) const
{GUCEF_TRACE;

    typename TRegisteredObjList::const_iterator i = m_list.begin();
    while ( i != m_list.end() )
    {
        destList.push_back( (*i).first );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTRegistry< T >::LockData( void )
{GUCEF_TRACE;
        /* implemented to avoid manditory implementation by decending classes */
}

/*-------------------------------------------------------------------------*/

template< class T >
void
CTRegistry< T >::UnlockData( void )
{GUCEF_TRACE;
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

#endif /* GUCEF_CORE_CTREGISTRY_H  ? */
