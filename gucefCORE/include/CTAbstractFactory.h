/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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
 
#ifndef GUCEF_CORE_CTABSTRACTFACTORY_H
#define GUCEF_CORE_CTABSTRACTFACTORY_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include "CTFactory.h"
#include "CTFactoryBase.h"
#include "CMsgException.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTABSTRACTFACTORY_CPP
    #pragma warning( push )
#endif

#pragma warning( disable: 4100 ) // unreferenced formal parameter
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
//      TEMPLATES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Abstract factory template for the selection and utilization of concrete 
 *  factories based on a criteria.
 *
 *  Note that when using this template it is manditory for BaseClassType 
 *  classes to define the following member function:
 *      SelectionCriteriaType GetType( void ) const;
 *
 */
template< typename SelectionCriteriaType, class BaseClassType >
class GUCEFCORE_EXPORT_CPP CTAbstractFactory
{
    public:
    typedef CTFactoryBase< BaseClassType > TConcreteFactory;
    
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
    if ( &src != this )
    {
    }
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

#ifndef GUCEF_CORE_CTABSTRACTFACTORY_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTABSTRACTFACTORY_H ? */