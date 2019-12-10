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
 
#ifndef GUCEF_CORE_CTFACTORY_H
#define GUCEF_CORE_CTFACTORY_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CTFactoryBase.h"

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

#endif /* GUCEF_CORE_CTFACTORY_H ? */
