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
 
#ifndef GUCEF_CORE_CTFACTORYWITHPARAM_H
#define GUCEF_CORE_CTFACTORYWITHPARAM_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CTFactoryBaseWithParam.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTFACTORYWITHPARAM_CPP
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
 *  Template for creating a concrete factory as described in the
 *  factory design pattern.
 */
template< class BaseClassType, class ConcreteClassType, typename ConstructionParamType >
class CTFactoryWithParam : public CTFactoryBaseWithParam< BaseClassType, ConstructionParamType >            
{
    public:
    
    /**
     *  Default contructor which acts as a no-op since factories are
     *  meta-data classes
     */
    CTFactoryWithParam( void );
    
    /**
     *  Copy contructor which acts as a no-op since factories are
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

#ifndef GUCEF_CORE_CTFACTORYWITHPARAM_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTFACTORYWITHPARAM_H ? */
