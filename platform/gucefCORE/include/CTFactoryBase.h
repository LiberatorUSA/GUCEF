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
 
#ifndef GUCEF_CORE_CTFACTORYBASE_H
#define GUCEF_CORE_CTFACTORYBASE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

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

    /**
     *  Returns a string representing the name of concrete class that can be created
     */
    virtual CString GetConcreteClassTypeName( void ) const = 0;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class BaseClassType >
CTFactoryBase< BaseClassType >::CTFactoryBase( void )
    : CICloneable()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType >
CTFactoryBase< BaseClassType >::CTFactoryBase( const CTFactoryBase& src )
    : CICloneable( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType >
CTFactoryBase< BaseClassType >::~CTFactoryBase()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType >
CTFactoryBase< BaseClassType >&
CTFactoryBase< BaseClassType >::operator=( const CTFactoryBase& src )
{GUCEF_TRACE;

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
