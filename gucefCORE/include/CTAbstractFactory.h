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

#include "CTFactory.h"
#include "CTFactoryBase.h"
#include "CMsgException.h"

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

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
template< typename SelectionCriteriaType, class BaseClassType >
class CTAbstractFactory : public CAbstractFactoryBase
{
    public:
    
    typedef SelectionCriteriaType TSelectionCriteriaType;
    typedef CTCloneableObj< SelectionCriteriaType > TKeyContainer;
    typedef CTFactoryBase< BaseClassType > TConcreteFactory;
    typedef std::set< SelectionCriteriaType > TKeySet;
    
    explicit CTAbstractFactory( const bool assumeFactoryOwnership = false );
    
    CTAbstractFactory( const CTAbstractFactory& src );
    
    virtual ~CTAbstractFactory();
    
    CTAbstractFactory& operator=( const CTAbstractFactory& src );

    /**
     *  Constructs the concrete factory product
     *
     *  @return pointer to the base class of the constructed factory product
     */
    BaseClassType* Create( const SelectionCriteriaType& selectedType );
    
    /**
     *  Destroys the concrete factory product
     *
     *  @param factoryProduct pointer to the base class of the constructed factory product
     */
    void Destroy( BaseClassType* factoryProduct );
    
    void RegisterConcreteFactory( const SelectionCriteriaType& selectedType ,
                                  TConcreteFactory* concreteFactory         );

    void UnregisterConcreteFactory( const SelectionCriteriaType& selectedType );

    bool IsConstructible( const SelectionCriteriaType& selectedType ) const;
    
    void ObtainKeySet( TKeySet& keySet ) const;
    
    private:
    typedef std::map< SelectionCriteriaType, TConcreteFactory* >  TFactoryList;
    
    TFactoryList m_concreteFactoryList;
    bool m_assumeFactoryOwnership;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::CTAbstractFactory( const bool assumeFactoryOwnership /* = false */ )
    : m_assumeFactoryOwnership( assumeFactoryOwnership )
{

}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::CTAbstractFactory( const CTAbstractFactory< SelectionCriteriaType, BaseClassType >& /* src */ )
{

}

/*-------------------------------------------------------------------------*/
   
template< typename SelectionCriteriaType, class BaseClassType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::~CTAbstractFactory()
{

}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType >
CTAbstractFactory< SelectionCriteriaType, BaseClassType >& 
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::operator=( const CTAbstractFactory< SelectionCriteriaType, BaseClassType >& src )
{
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType >
BaseClassType* 
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::Create( const SelectionCriteriaType& selectedType )
{
    TFactoryList::iterator i( m_concreteFactoryList.find( selectedType ) );
    if ( i != m_concreteFactoryList.end() )
    {
        return (*i).second->Create(); 
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/
    
template< typename SelectionCriteriaType, class BaseClassType >
void 
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::Destroy( BaseClassType* factoryProduct )
{
    TFactoryList::iterator i( m_concreteFactoryList.find( factoryProduct->GetType() ) );
    if ( i != m_concreteFactoryList.end() )
    {
        (*i).second->Destroy( factoryProduct );
        return;
    }
    throw CMsgException( __FILE__, __LINE__, "CTAbstractFactory: Cannot find concrete factory capable of destroying the given product" );
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType >
void
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::ObtainKeySet( TKeySet& keySet ) const
{
    TFactoryList::const_iterator i( m_concreteFactoryList.begin() );
    while ( i != m_concreteFactoryList.end() )
    {
        keySet.insert( (*i).first );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType >
bool
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::IsConstructible( const SelectionCriteriaType& selectedType ) const
{
    return m_concreteFactoryList.find( selectedType ) != m_concreteFactoryList.end();
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType >
void 
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::RegisterConcreteFactory( const SelectionCriteriaType& selectedType ,
                                                                                    TConcreteFactory* concreteFactory         )
{
    m_concreteFactoryList[ selectedType ] = concreteFactory;
    
    TKeyContainer keyContainer( selectedType );
    NotifyObservers( ConcreteFactoryRegisteredEvent, &keyContainer );
}

/*-------------------------------------------------------------------------*/

template< typename SelectionCriteriaType, class BaseClassType >
void 
CTAbstractFactory< SelectionCriteriaType, BaseClassType >::UnregisterConcreteFactory( const SelectionCriteriaType& selectedType )
{
    TFactoryList::iterator i = m_concreteFactoryList.find( selectedType );
    if ( i != m_concreteFactoryList.end() )
    {
        if ( m_assumeFactoryOwnership )
        {
            delete (*i).second;
        }
        m_concreteFactoryList.erase( i );
        
        TKeyContainer keyContainer( selectedType );
        NotifyObservers( ConcreteFactoryUnregisteredEvent, &keyContainer );
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

#endif /* GUCEF_CORE_CTABSTRACTFACTORY_H ? */