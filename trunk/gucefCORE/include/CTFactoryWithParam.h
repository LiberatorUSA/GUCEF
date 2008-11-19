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
 
#ifndef GUCEF_CORE_CTFACTORYWITHPARAM_H
#define GUCEF_CORE_CTFACTORYWITHPARAM_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CTFactoryBaseWithParam.h"

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
 *  Template for creating a concrete factory as described in the
 *  factory design pattern.
 */
template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
class CTFactoryWithParam : public CTFactoryBaseWithParam< BaseClassType, ConstructionParamType >            
{
    public:
    
    /**
     *  Default constructor which acts as a no-op since factories are
     *  meta-data classes
     */
    CTFactoryWithParam( void );
    
    /**
     *  Copy constructor which acts as a no-op since factories are
     *  meta-data classes
     */
    CTFactoryWithParam( const CTFactoryWithParam& src );
    
    /**
     *  Default destructor which acts as a no-op since factories are
     *  meta-data classes
     */
    virtual ~CTFactoryWithParam();
    
    /**
     *  Default assignment operator which acts as a no-op since factories are
     *  meta-data classes
     */    
    CTFactoryWithParam& operator=( const CTFactoryWithParam& src );
    
    /**
     *  Constructs the concrete factory product
     *
     *  @return pointer to the base class of the constructed factory product
     */
    virtual BaseClassType* Create( const ConstructionParamType& param );
    
    /**
     *  Destroys the concrete factory product
     *
     *  @param factoryProduct pointer to the base class of the constructed factory product
     */
    virtual void Destroy( BaseClassType* factoryProduct );

    /**
     *  Allows creation of factories without knowing the decending concrete
     *  classes.
     */
    virtual CICloneable* Clone( void ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >::CTFactoryWithParam( void )
{
    
}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >::CTFactoryWithParam( const CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >& src )
{

}

/*-------------------------------------------------------------------------*/
   
template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >::~CTFactoryWithParam()
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >& 
CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >::operator=( const CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >& src )
{
    if ( &src != this )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
BaseClassType* 
CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >::Create( const ConstructionParamType& param )
{
    return new ConcreteClassType( param );
}

/*-------------------------------------------------------------------------*/
    
template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
void 
CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >::Destroy( BaseClassType* factoryProduct )
{
    /*
     *  We cast to the decendant class or concrete factory product if you will, 
     *  to ensure it's destructor is called even if the base class does not have a
     *  virtual destructor
     */
    delete static_cast< ConcreteClassType* >( factoryProduct );
}

/*-------------------------------------------------------------------------*/
    
template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
CICloneable* 
CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >::Clone( void ) const
{
    /*
     *  In contrast to normal cloning we do not need to use the copy constructor
     *  here because a factory is meta data and has no attributes.
     *
     *  Decending classes can offcourse override this if additional criteria are
     *  to be considdered.
     */
    return new CTFactoryWithParam< BaseClassType, ConcreteClassType, ConstructionParamType >;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTFACTORYWITHPARAM_H ? */
