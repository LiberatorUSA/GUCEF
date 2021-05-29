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

#ifndef GUCEF_CORE_CTABSTRACTFACTORY_H
#define GUCEF_CORE_CTABSTRACTFACTORY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include <set>

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#include "CTFactory.h"
#include "CTFactoryBase.h"
#include "CMsgException.h"

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CABSTRACTFACTORYBASE_H
#include "gucefCORE_CAbstractFactoryBase.h"
#define GUCEF_CORE_CABSTRACTFACTORYBASE_H
#endif /* GUCEF_CORE_CABSTRACTFACTORYBASE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TEMPLATES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Abstract factory template for the selection and utilization of concrete
 *  factories based on a criteria.
 *
 *  Note that when using this template it is mandatory for BaseClassType
 *  classes to define the following member function:
 *      SelectionCriteriaType GetType( void ) const;
 *
 */
template< typename SelectionCriteriaType, class BaseClassType, class LockType >
class CTAbstractFactory : public CAbstractFactoryBase ,
                          public CTDynamicDestructorBase< BaseClassType >
{
    public:

    typedef SelectionCriteriaType TSelectionCriteriaType;
    typedef CTCloneableObj< SelectionCriteriaType > TKeyContainer;
    typedef CTFactoryBase< BaseClassType > TConcreteFactory;
    typedef std::set< SelectionCriteriaType > TKeySet;
    typedef CTBasicSharedPtr< BaseClassType, LockType > TProductPtr;

    explicit CTAbstractFactory( const bool assumeFactoryOwnership = false ,
                                const bool useEventing = true             );

    CTAbstractFactory( const CTAbstractFactory& src );

    virtual ~CTAbstractFactory();

    CTAbstractFactory& operator=( const CTAbstractFactory& src );

    /**
     *  Constructs the concrete factory product
     *
     *  @return pointer to the base class of the constructed factory product
     */
    TProductPtr Create( const SelectionCriteriaType& selectedType );

    void RegisterConcreteFactory( const SelectionCriteriaType& selectedType ,
                                  TConcreteFactory* concreteFactory         );

    void UnregisterConcreteFactory( const SelectionCriteriaType& selectedType );

    void UnregisterAllConcreteFactories( void );

    bool IsConstructible( const SelectionCriteriaType& selectedType ) const;

    void ObtainKeySet( TKeySet& keySet ) const;

    /**
     *  Destroys the concrete factory product
     *  Intended to be invoked automatically by the shared pointer implementation
     *
     *  @param factoryProduct pointer to the base class of the constructed factory product
     */
    virtual void DestroyObject( BaseClassType* factoryProduct ) GUCEF_VIRTUAL_OVERRIDE;
    
    protected:

    virtual bool Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    private:
    typedef std::map< SelectionCriteriaType, TConcreteFactory* >  TFactoryList;

    TFactoryList m_concreteFactoryList;
    bool m_assumeFactoryOwnership;
    LockType m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::CTAbstractFactory( const bool assumeFactoryOwnership /* = false */ ,
                                                                                        const bool useEventing /* = true */             )
    : CAbstractFactoryBase( useEventing )
    , m_concreteFactoryList()
    , m_assumeFactoryOwnership( assumeFactoryOwnership )
    , m_lock()
{

}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::CTAbstractFactory( const CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >& src )
    : CAbstractFactoryBase( src )
    , m_concreteFactoryList( src )
    , m_assumeFactoryOwnership( src.m_assumeFactoryOwnership )
    , m_lock()
{

}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::~CTAbstractFactory()
{

}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >&
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::operator=( const CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >& src )
{
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
CTBasicSharedPtr< BaseClassType, LockType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::Create( const SelectionCriteriaType& selectedType )
{
    MT::CObjectScopeLock lock( this );
    typename TFactoryList::iterator i( m_concreteFactoryList.find( selectedType ) );
    if ( i != m_concreteFactoryList.end() )
    {
        TProductPtr product( (*i).second->Create(), this );
        return product;
    }
    return TProductPtr();
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
void
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::DestroyObject( BaseClassType* factoryProduct )
{
    MT::CObjectScopeLock lock( this );
    typename TFactoryList::iterator i( m_concreteFactoryList.find( factoryProduct->GetType() ) );
    if ( i != m_concreteFactoryList.end() )
    {
        (*i).second->Destroy( factoryProduct );
        return;
    }
    throw CMsgException( __FILE__, __LINE__, "CTAbstractFactory: Cannot find concrete factory capable of destroying the given product" );
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
void
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::ObtainKeySet( TKeySet& keySet ) const
{
    MT::CObjectScopeLock lock( this );
    typename TFactoryList::const_iterator i( m_concreteFactoryList.begin() );
    while ( i != m_concreteFactoryList.end() )
    {
        keySet.insert( (*i).first );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
bool
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::IsConstructible( const SelectionCriteriaType& selectedType ) const
{
    MT::CObjectScopeLock lock( this );
    return m_concreteFactoryList.find( selectedType ) != m_concreteFactoryList.end();
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
void
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::RegisterConcreteFactory( const SelectionCriteriaType& selectedType ,
                                                                                              TConcreteFactory* concreteFactory         )
{
    MT::CObjectScopeLock lock( this );
    m_concreteFactoryList[ selectedType ] = concreteFactory;

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "TAbstractFactory<>: Registered concrete factory for type \"" + ToString( selectedType ) + "\"" );

    if ( m_useEventing )
    {
        TKeyContainer keyContainer( selectedType );
        NotifyObservers( ConcreteFactoryRegisteredEvent, &keyContainer );
    }
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
void
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::UnregisterConcreteFactory( const SelectionCriteriaType& selectedType )
{
    MT::CObjectScopeLock lock( this );
    typename TFactoryList::iterator i = m_concreteFactoryList.find( selectedType );
    if ( i != m_concreteFactoryList.end() )
    {
        if ( m_assumeFactoryOwnership )
        {
            delete (*i).second;
        }
        m_concreteFactoryList.erase( i );

        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "TAbstractFactory<>: Unregistered concrete factory for type \"" + ToString( selectedType ) + "\"" );
        
        if ( m_useEventing )
        {
            TKeyContainer keyContainer( selectedType );
            NotifyObservers( ConcreteFactoryUnregisteredEvent, &keyContainer );
        }
    }
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
void
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::UnregisterAllConcreteFactories( void )
{
    MT::CObjectScopeLock lock( this );
    while ( !m_concreteFactoryList.empty() )
    {
        typename TFactoryList::iterator i = m_concreteFactoryList.begin();
        if ( m_assumeFactoryOwnership )
        {
            delete (*i).second;
        }

        SelectionCriteriaType selectedType = (*i).first;
        m_concreteFactoryList.erase( i );

        if ( m_useEventing )
        {
            TKeyContainer keyContainer( selectedType );
            NotifyObservers( ConcreteFactoryUnregisteredEvent, &keyContainer );
        }
    }
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
bool
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, class LockType >
bool
CTAbstractFactory< SelectionCriteriaType, BaseClassType, LockType >::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTABSTRACTFACTORY_H ? */
