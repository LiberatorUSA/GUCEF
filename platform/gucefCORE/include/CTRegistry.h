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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"          /* GUCEF platform string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_EXCEPTIONMACROS_H
#include "ExceptionMacros.h"    /* macros for dealing with scope typed exceptions */
#define GUCEF_CORE_EXCEPTIONMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONMACROS_H ? */

#ifndef GUCEF_CORE_CINAMEDINSTANCE_H
#include "CINamedInstance.h"    /* basic name retrieval interface */
#define GUCEF_CORE_CINAMEDINSTANCE_H
#endif /* GUCEF_CORE_CINAMEDINSTANCE_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"        /* pointer to the shared object */
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

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
template < class T, class LockType >
class CTRegistry : public MT::CILockable
{
    public:
    typedef CTSharedPtr< T, LockType > TRegisteredObjPtr;
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

    virtual bool TryRegister( const CString& name                ,
                              const TRegisteredObjPtr& sharedPtr );

    virtual void Unregister( const CString& name );

    virtual void UnregisterAll( void );

    virtual UInt32 GetCount( void ) const;

    virtual void GetList( TStringList& destList ) const;

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( EAlreadyRegistered );
    GUCEF_DEFINE_INLINED_MSGEXCEPTION( EUnregisteredName );

    protected:

    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    typedef std::map< CString, TRegisteredObjPtr* > TRegisteredObjList;

    TRegisteredObjList m_list;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class T, class LockType >
CTRegistry< T, LockType >::CTRegistry( void )
    : m_list()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
CTRegistry< T, LockType >::CTRegistry( const CTRegistry& src )
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

template< class T, class LockType >
CTRegistry< T, LockType >::~CTRegistry()
{GUCEF_TRACE;

    UnregisterAll();
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
CTRegistry< T, LockType >&
CTRegistry< T, LockType >::operator=( const CTRegistry& src )
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

template< class T, class LockType >
bool
CTRegistry< T, LockType >::IsRegistered( const CString& name ) const
{GUCEF_TRACE;

    return m_list.find( name ) != m_list.end();
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
CTSharedPtr< T, LockType >
CTRegistry< T, LockType >::Lookup( const CString& name ) const
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

template< class T, class LockType >
bool
CTRegistry< T, LockType >::TryLookup( const CString& name           ,
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

template< class T, class LockType >
void
CTRegistry< T, LockType >::Register( const CString& name                ,
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

template< class T, class LockType >
bool
CTRegistry< T, LockType >::TryRegister( const CString& name                ,
                                        const TRegisteredObjPtr& sharedPtr )
{GUCEF_TRACE;

    typename TRegisteredObjList::iterator i = m_list.find( name );
    if ( i == m_list.end() )
    {
        m_list[ name ] = new TRegisteredObjPtr( sharedPtr );
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
void
CTRegistry< T, LockType >::Unregister( const CString& name )
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

template< class T, class LockType >
void
CTRegistry< T, LockType >::UnregisterAll( void )
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

template< class T, class LockType >
UInt32
CTRegistry< T, LockType >::GetCount( void ) const
{GUCEF_TRACE;
    return static_cast< UInt32 >( m_list.size() );
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
void
CTRegistry< T, LockType >::GetList( TStringList& destList ) const
{GUCEF_TRACE;

    typename TRegisteredObjList::const_iterator i = m_list.begin();
    while ( i != m_list.end() )
    {
        destList.push_back( (*i).first );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
bool
CTRegistry< T, LockType >::Lock( void ) const
{GUCEF_TRACE;
        /* implemented to avoid manditory implementation by decending classes */
    return false;
}

/*-------------------------------------------------------------------------*/

template< class T, class LockType >
bool
CTRegistry< T, LockType >::Unlock( void ) const
{GUCEF_TRACE;
        /* implemented to avoid manditory implementation by decending classes */
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

#endif /* GUCEF_CORE_CTREGISTRY_H  ? */
