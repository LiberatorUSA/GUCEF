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

#ifndef GUCEF_CORE_CTABSTRACTFACTORYWITHPARAM_H
#define GUCEF_CORE_CTABSTRACTFACTORYWITHPARAM_H

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

#include "CTFactoryWithParam.h"
#include "CTFactoryBaseWithParam.h"
#include "CMsgException.h"

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H */

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
template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
class CTAbstractFactoryWithParam : public CAbstractFactoryBase ,
                                   public CTDynamicDestructorBase< BaseClassType >
{
    public:

    typedef SelectionCriteriaType TSelectionCriteriaType;
    typedef CTCloneableObj< SelectionCriteriaType > TKeyContainer;
    typedef CTFactoryBaseWithParam< BaseClassType, ConstructionParamType > TConcreteFactory;
    typedef std::set< SelectionCriteriaType > TKeySet;
    typedef CTBasicSharedPtr< BaseClassType, LockType > TProductPtr;

    explicit CTAbstractFactoryWithParam( const bool assumeFactoryOwnership = false ,
                                         const bool useEventing = true             );

    CTAbstractFactoryWithParam( const CTAbstractFactoryWithParam& src );

    virtual ~CTAbstractFactoryWithParam();

    CTAbstractFactoryWithParam& operator=( const CTAbstractFactoryWithParam& src );

    /**
     *  Constructs the concrete factory product
     *
     *  @return pointer to the base class of the constructed factory product
     */
    TProductPtr Create( const SelectionCriteriaType& selectedType ,
                        const ConstructionParamType& param        );

    void RegisterConcreteFactory( const SelectionCriteriaType& selectedType ,
                                  TConcreteFactory* concreteFactory         );

    void UnregisterConcreteFactory( const SelectionCriteriaType& selectedType );

    bool IsConstructible( const SelectionCriteriaType& selectedType ) const;

    void ObtainKeySet( TKeySet& keySet ) const;

    /**
     *  Destroys the concrete factory product
     *  Intended to be invoked automatically by the shared pointer implementation
     *
     *  @param factoryProduct pointer to the base class of the constructed factory product
     */
    virtual void DestroyObject( BaseClassType* factoryProduct ) GUCEF_VIRTUAL_OVERRIDE;

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

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::CTAbstractFactoryWithParam( const bool assumeFactoryOwnership /* = false */ ,
                                                                                                                                 const bool useEventing /* = true */             )
    : CAbstractFactoryBase( useEventing )
    , m_concreteFactoryList()
    , m_assumeFactoryOwnership( assumeFactoryOwnership )
    , m_lock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::CTAbstractFactoryWithParam( const CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >& src )
    : CAbstractFactoryBase( src )
    , m_concreteFactoryList( src )
    , m_assumeFactoryOwnership( src.m_assumeFactoryOwnership )
    , m_lock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::~CTAbstractFactoryWithParam()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >&
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::operator=( const CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >& /* src */ )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
CTBasicSharedPtr< BaseClassType, LockType >
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::Create( const SelectionCriteriaType& selectedType ,
																                                             const ConstructionParamType& param        )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    typename TFactoryList::iterator i( m_concreteFactoryList.find( selectedType ) );
    if ( i != m_concreteFactoryList.end() )
    {
        TProductPtr product( (*i).second->Create( param ), this );
        return product;
    }
    return TProductPtr();
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
void
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::DestroyObject( BaseClassType* factoryProduct )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    typename TFactoryList::iterator i( m_concreteFactoryList.find( factoryProduct->GetType() ) );
    if ( i != m_concreteFactoryList.end() )
    {
        (*i).second->Destroy( factoryProduct );
        return;
    }
    throw CMsgException( __FILE__, __LINE__, "CTAbstractFactoryWithParam: Cannot find concrete factory capable of destroying the given product" );
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
void
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::ObtainKeySet( TKeySet& keySet ) const
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

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
bool
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::IsConstructible( const SelectionCriteriaType& selectedType ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    return m_concreteFactoryList.find( selectedType ) != m_concreteFactoryList.end();
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
void
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::RegisterConcreteFactory( const SelectionCriteriaType& selectedType ,
                                                                                                                              TConcreteFactory* concreteFactory         )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_concreteFactoryList[ selectedType ] = concreteFactory;

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "TAbstractFactoryWithParam<>: Registered concrete factory for type \"" + ToString( selectedType ) + "\"" );

    if ( m_useEventing )
    {
        TKeyContainer keyContainer( selectedType );
        NotifyObservers( ConcreteFactoryRegisteredEvent, &keyContainer );
    }
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType, typename ConstructionParamType, class LockType >
void
CTAbstractFactoryWithParam< SelectionCriteriaType, BaseClassType, ConstructionParamType, LockType >::UnregisterConcreteFactory( const SelectionCriteriaType& selectedType )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    typename TFactoryList::iterator i = m_concreteFactoryList.find( selectedType );
    if ( i != m_concreteFactoryList.end() )
    {
        if ( m_assumeFactoryOwnership )
        {
            delete (*i).second;
        }
        m_concreteFactoryList.erase( i );

        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "TAbstractFactoryWithParam<>: Unregistered concrete factory for type \"" + ToString( selectedType ) + "\"" );
        
        if ( m_useEventing )
        {
            TKeyContainer keyContainer( selectedType );
            NotifyObservers( ConcreteFactoryUnregisteredEvent, &keyContainer );
        }
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTABSTRACTFACTORYWITHPARAM_H ? */
