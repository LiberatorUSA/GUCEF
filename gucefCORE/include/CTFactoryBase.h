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
 
#ifndef GUCEF_CORE_CTFACTORYBASE_H
#define GUCEF_CORE_CTFACTORYBASE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_ETypes.h"
#include "CICloneable.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTFACTORYBASE_CPP
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
template< class BaseClassType >
class CTFactoryBase : public CICloneable
{
    public:
    
    /**
     *  Default contructor which acts as a no-op since factories are
     *  meta-data classes
     */
    CTFactoryBase( void );

    /**
     *  Copy contructor which acts as a no-op since factories are
     *  meta-data classes
     */
    CTFactoryBase( const CTFactoryBase& src );
    
    /**
     *  Destuctor with no special behaviour, implemented to
     *  ensure that a virtual destructor is provided.
     */
    virtual ~CTFactoryBase();

    /**
     *  Assignmemt operator which acts as a no-op since factories are
     *  meta-data classes
     */
    CTFactoryBase& operator=( const CTFactoryBase& src );

    /**
     *  Constructs the concrete factory product
     *
     *  @return pointer to the base class of the constructed factory product
     */
    virtual BaseClassType* Create( void ) = 0;
    
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

template< class BaseClassType >
CTFactoryBase< BaseClassType >::CTFactoryBase( void )
    : CICloneable()
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType >
CTFactoryBase< BaseClassType >::CTFactoryBase( const CTFactoryBase& src )
    : CICloneable( src )
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType >
CTFactoryBase< BaseClassType >::~CTFactoryBase()
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType >
CTFactoryBase< BaseClassType >&
CTFactoryBase< BaseClassType >::operator=( const CTFactoryBase& src )
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

#ifndef GUCEF_CORE_CTFACTORYBASE_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTFACTORYBASE_H ? */
