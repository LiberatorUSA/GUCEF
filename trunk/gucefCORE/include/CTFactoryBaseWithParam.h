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
 
#ifndef GUCEF_CORE_CTFACTORYBASEWITHPARAM_H
#define GUCEF_CORE_CTFACTORYBASEWITHPARAM_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_ETypes.h"
#include "CICloneable.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTFACTORYBASEWITHPARAM_CPP
    #pragma warning( push )
#endif

#pragma warning( disable: 4786 ) // identifier was truncated to 'number' characters
#pragma warning( disable: 4251 ) // 'classname' needs to have dll-interface to be used by clients of class 'classname'
#pragma warning( disable: 4284 )  
#pragma warning( disable: 4146 ) // unary minus operator applied to unsigned type, result still unsigned

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
 *  Template for creating an abstract interface for creation and destruction of 
 *  objects without specifieing the concrete type
 */
template< class BaseClassType, typename ConstructionParamType >
class CTFactoryBaseWithParam : public CICloneable
{
    public:
    
    CTFactoryBaseWithParam( void );

    CTFactoryBaseWithParam( const CTFactoryBaseWithParam& src );
    
    /**
     *  Destuctor with no special behaviour, implemented to
     *  ensure that a virtual destructor is provided.
     */
    virtual ~CTFactoryBaseWithParam();

    CTFactoryBaseWithParam& operator=( const CTFactoryBaseWithParam& src );

    /**
     *  Constructs the concrete factory product
     *
     *  @return pointer to the base class of the constructed factory product
     */
    virtual BaseClassType* Create( const ConstructionParamType& param  ) = 0;
    
    /**
     *  Destroys the concrete factory product
     *
     *  @param factoryProduct pointer to the base class of the constructed factory product
     */
    virtual void Destroy( BaseClassType* factoryProduct ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class BaseClassType, typename ConstructionParamType >
CTFactoryBaseWithParam< BaseClassType, ConstructionParamType >::CTFactoryBaseWithParam( void )
    : CICloneable()
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, typename ConstructionParamType >
CTFactoryBaseWithParam< BaseClassType, ConstructionParamType >::CTFactoryBaseWithParam( const CTFactoryBaseWithParam& src )
    : CICloneable( src )
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, typename ConstructionParamType >
CTFactoryBaseWithParam< BaseClassType, ConstructionParamType >::~CTFactoryBaseWithParam()
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, typename ConstructionParamType >
CTFactoryBaseWithParam< BaseClassType, ConstructionParamType >&
CTFactoryBaseWithParam< BaseClassType, ConstructionParamType >::operator=( const CTFactoryBaseWithParam& src )
{
    if ( &src != this )
    {
        // nothing to do,.. meta-data class
    }
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTFACTORYBASEWITHPARAM_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTFACTORYBASEWITHPARAM_H ? */
