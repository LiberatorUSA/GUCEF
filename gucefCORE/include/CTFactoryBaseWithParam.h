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
