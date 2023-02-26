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

#ifndef GUCEF_CORE_CTFACTORYBASE_H
#include "CTFactoryBase.h"
#define GUCEF_CORE_CTFACTORYBASE_H
#endif /* GUCEF_CORE_CTFACTORYBASE_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

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
    virtual BaseClassType* Create( void ) GUCEF_VIRTUAL_OVERRIDE;
    
    /**
     *  Destroys the concrete factory product
     *
     *  @param factoryProduct pointer to the base class of the constructed factory product
     */
    virtual void Destroy( BaseClassType* factoryProduct ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Allows creation of factories without knowing the decending concrete
     *  classes.
     */
    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Returns a string representing the name of concrete class that can be created
     */
    virtual CString GetConcreteClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType >
CTFactory< BaseClassType, ConcreteClassType >::CTFactory( void )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType >
CTFactory< BaseClassType, ConcreteClassType >::CTFactory( const CTFactory< BaseClassType, ConcreteClassType >& src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
   
template< class BaseClassType, class ConcreteClassType >
CTFactory< BaseClassType, ConcreteClassType >::~CTFactory()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType >
CTFactory< BaseClassType, ConcreteClassType >& 
CTFactory< BaseClassType, ConcreteClassType >::operator=( const CTFactory< BaseClassType, ConcreteClassType >& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType >
BaseClassType* 
CTFactory< BaseClassType, ConcreteClassType >::Create( void )
{GUCEF_TRACE;

    return GUCEF_NEW ConcreteClassType();
}

/*-------------------------------------------------------------------------*/
    
template< class BaseClassType, class ConcreteClassType >
void 
CTFactory< BaseClassType, ConcreteClassType >::Destroy( BaseClassType* factoryProduct )
{GUCEF_TRACE;

    /*
     *  We cast to the decendant class or concrete factory product if you will, 
     *  to ensure it's destructor is called even if the base class does not have a
     *  virtual destructor
     */
    GUCEF_DELETE static_cast< ConcreteClassType* >( factoryProduct );
}

/*-------------------------------------------------------------------------*/
    
template< class BaseClassType, class ConcreteClassType >
CICloneable* 
CTFactory< BaseClassType, ConcreteClassType >::Clone( void ) const
{GUCEF_TRACE;

    /*
     *  In contrast to normal cloning we do not need to use the copy constructor
     *  here because a factory is meta data and has no attributes.
     *
     *  Decending classes can offcourse override this if additional criteria are
     *  to be considdered.
     */
    return GUCEF_NEW CTFactory< BaseClassType, ConcreteClassType >;
}

/*-------------------------------------------------------------------------*/

template< class BaseClassType, class ConcreteClassType >
CString 
CTFactory< BaseClassType, ConcreteClassType >::GetConcreteClassTypeName( void ) const
{GUCEF_TRACE;

    return ToString< ConcreteClassType >();
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
