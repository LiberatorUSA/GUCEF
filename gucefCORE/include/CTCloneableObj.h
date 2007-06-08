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

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#define GUCEF_CORE_CTCLONEABLEOBJ_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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

template< typename T > 
class CTCloneableObj : public CICloneable
{
    public:

    explicit CTCloneableObj( const T& data );

    CTCloneableObj( const CTCloneableObj& src );
        
    virtual ~CTCloneableObj();
    
    CTCloneableObj& operator=( const CTCloneableObj& src );
    
    virtual CICloneable* Clone( void ) const;    
    
    T GetDataCopy( void ) const;
    
    T& GetData( void );
    
    const T& GetData( void ) const;

    private:

    CTCloneableObj( void );

    private:
    T m_data;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTCloneableObj< T >::CTCloneableObj( const T& data )
        : m_data( data )
{TRACE;

}


/*-------------------------------------------------------------------------*/

template< typename T >
CTCloneableObj< T >::CTCloneableObj( const CTCloneableObj< T >& src )
        : m_data( src.m_data )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableObj< T >::~CTCloneableObj()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableObj< T >& 
CTCloneableObj< T >::operator=( const CTCloneableObj< T >& src )
{TRACE;

    if ( this != &src )
    {
        m_data = src.m_data;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CICloneable* 
CTCloneableObj< T >::Clone( void ) const
{TRACE;

    return new CTCloneableObj< T >( *this );
}

/*-------------------------------------------------------------------------*/

template< typename T >
T&
CTCloneableObj< T >::GetData( void )
{TRACE;

    return m_data;
}
/*-------------------------------------------------------------------------*/

template< typename T >
const T& 
CTCloneableObj< T >::GetData( void ) const
{TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------*/

template< typename T >
T 
CTCloneableObj< T >::GetDataCopy( void ) const
{TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */