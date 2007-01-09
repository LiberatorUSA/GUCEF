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
 
#ifndef GUCEF_CORE_CTFACTORY_H
#define GUCEF_CORE_CTFACTORY_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CTFactoryBase.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTFACTORY_CPP
    #pragma warning( push )
#endif

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
template< class BaseClassType, class ConcreteClassType >
class CTFactory : public CTFactoryBase< BaseClassType >            
{
    public:
    
    CTFactory( void );
    
    CTFactory( const CTFactory& src );
    
    virtual ~CTFactory();
    
    CTFactory& operator=( const CTFactory& src );

    /**
     *  Constructs the concrete factory product
     *
     *  @return pointer to the base class of the constructed factory product
     */
    virtual BaseClassType* Create( void );
    
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

template< class BaseClassType, class ConcreteClassType >
CTFactory< BaseClassType, ConcreteClassType >::CTFactory( void )
{
    
}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType >
CTFactory< BaseClassType, ConcreteClassType >::CTFactory( const CTFactory< BaseClassType, ConcreteClassType >& src )
{

}

/*-------------------------------------------------------------------------*/
   
template< class BaseClassType, class ConcreteClassType >
CTFactory< BaseClassType, ConcreteClassType >::~CTFactory()
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType >
CTFactory< BaseClassType, ConcreteClassType >& 
CTFactory< BaseClassType, ConcreteClassType >::operator=( const CTFactory< BaseClassType, ConcreteClassType >& src )
{
    if ( &src != this )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType >
BaseClassType* 
CTFactory< BaseClassType, ConcreteClassType >::Create( void )
{
    return new ConcreteClassType();
}

/*-------------------------------------------------------------------------*/
    
template< class BaseClassType, class ConcreteClassType >
void 
CTFactory< BaseClassType, ConcreteClassType >::Destroy( BaseClassType* factoryProduct )
{
    /*
     *  We cast to the decendant class or concrete factory product if you will, 
     *  to ensure it's destructor is called even if the base class does not have a
     *  virtual destructor
     */
    delete static_cast< ConcreteClassType* >( factoryProduct );
}

/*-------------------------------------------------------------------------*/
    
template< class BaseClassType, class ConcreteClassType >
CICloneable* 
CTFactory< BaseClassType, ConcreteClassType >::Clone( void ) const
{
    /*
     *  In contrast to normal cloning we do not need to use the copy constructor
     *  here because a factory is meta data and has no attributes.
     *
     *  Decending classes can offcourse override this if additional criteria are
     *  to be considdered.
     */
    return new CTFactory< BaseClassType, ConcreteClassType >;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTFACTORY_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTFACTORY_H ? */
