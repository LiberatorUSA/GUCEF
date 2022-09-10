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

    CTCloneableObj( void );
    
    explicit CTCloneableObj( const T& data );

    CTCloneableObj( const CTCloneableObj& src );
        
    virtual ~CTCloneableObj() GUCEF_VIRTUAL_OVERRIDE;
    
    CTCloneableObj& operator=( const CTCloneableObj& src );

    T& operator=( const T& src );
    
    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;    
    
    inline T GetDataCopy( void ) const;
    
    inline T& GetData( void );
    
    inline const T& GetData( void ) const;

    inline operator const T&() const;

    inline operator T&();

    protected:
    T m_data;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTCloneableObj< T >::CTCloneableObj( void )
        : m_data()
{GUCEF_TRACE;

} 

/*-------------------------------------------------------------------------*/

template< typename T >
CTCloneableObj< T >::CTCloneableObj( const T& data )
        : m_data( data )
{GUCEF_TRACE;

} 

/*-------------------------------------------------------------------------*/

template< typename T >
CTCloneableObj< T >::CTCloneableObj( const CTCloneableObj< T >& src )
        : m_data( src.m_data )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableObj< T >::~CTCloneableObj()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableObj< T >& 
CTCloneableObj< T >::operator=( const CTCloneableObj< T >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_data = src.m_data;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T >
T& 
CTCloneableObj< T >::operator=( const T& src )
{GUCEF_TRACE;

    m_data = src;
    return *this;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CICloneable* 
CTCloneableObj< T >::Clone( void ) const
{GUCEF_TRACE;

    return new CTCloneableObj< T >( *this );
}

/*-------------------------------------------------------------------------*/

template< typename T >
T&
CTCloneableObj< T >::GetData( void )
{GUCEF_TRACE;

    return m_data;
}
/*-------------------------------------------------------------------------*/

template< typename T >
const T& 
CTCloneableObj< T >::GetData( void ) const
{GUCEF_TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------*/

template< typename T >
T 
CTCloneableObj< T >::GetDataCopy( void ) const
{GUCEF_TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline
CTCloneableObj< T >::operator const T&() const
{GUCEF_TRACE;

    return m_data;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline
CTCloneableObj< T >::operator T&()
{GUCEF_TRACE;

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