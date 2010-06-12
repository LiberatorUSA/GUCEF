/*
 *  gucefMATH: GUCEF module providing basic math classes
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

#ifndef GUCEF_MATH_CTPOINT3D_H
#define GUCEF_MATH_CTPOINT3D_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MATH_CTVECTOR3D_H
#include "gucefMATH_CTVector3D.h"
#define GUCEF_MATH_CTVECTOR3D_H
#endif /* GUCEF_MATH_CTVECTOR3D_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MATH {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename T >
class CTPoint3D
{
    public:
    
    typedef CTVector3D< T > TRelatedVector3D;
    
    CTPoint3D( void );
    
    CTPoint3D( const CTPoint3D& other );
    
    CTPoint3D( const Int32 x ,
               const Int32 y ,
               const Int32 z );

    CTPoint3D( const UInt32 x ,
               const UInt32 y ,
               const UInt32 z );

    CTPoint3D( const Float32 x ,
               const Float32 y ,
               const Float32 z );

    CTPoint3D( const Float64 x ,
               const Float64 y ,
               const Float64 z );

    ~CTPoint3D();
    
    CTPoint3D& operator=( const CTPoint3D& other );

    bool operator==( const CTPoint3D& other ) const;
    
    bool operator!=( const CTPoint3D& other ) const;

    // Vector difference
    TRelatedVector3D operator-( const CTPoint3D& other ) const; 
    
    // +translate
    CTPoint3D operator+( const TRelatedVector3D& translateVector ) const;
    
    // -translate
    CTPoint3D operator-( const TRelatedVector3D& translateVector ) const;
    
    // incremental +translate
    CTPoint3D& operator+=( const TRelatedVector3D& translateVector );
    
    // decremental -translate
    CTPoint3D& operator-=( const TRelatedVector3D& translateVector );
    
    private:
    
    T m_x;
    T m_y;
    T m_z;
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( void )
    : m_x( 0 )  ,
      m_y( 0 )  ,
      m_z( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const CTPoint3D< T >& src )
    : m_x( src.m_x )  ,
      m_y( src.m_y )  ,
      m_z( src.m_z )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::~CTPoint3D( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const Int32 x ,
                           const Int32 y ,
                           const Int32 z )
    : m_x( x ) ,
      m_y( y ) ,
      m_z( z )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const UInt32 x ,
                           const UInt32 y ,
                           const UInt32 z )
    : m_x( x ) ,
      m_y( y ) ,
      m_z( z )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const Float32 x ,
                           const Float32 y ,
                           const Float32 z )
    : m_x( x ) ,
      m_y( y ) ,
      m_z( z )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const Float64 x ,
                           const Float64 y ,
                           const Float64 z )
    : m_x( x ) ,
      m_y( y ) ,
      m_z( z )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const Float32* xyz )
    : m_x( xyz[ 0 ] ) ,
      m_y( xyz[ 1 ] ) ,
      m_z( xyz[ 2 ] )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const Float64* xyz )
    : m_x( xyz[ 0 ] ) ,
      m_y( xyz[ 1 ] ) ,
      m_z( xyz[ 2 ] )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const UInt32* xyz )
    : m_x( xyz[ 0 ] ) ,
      m_y( xyz[ 1 ] ) ,
      m_z( xyz[ 2 ] )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( const Int32* xyz )
    : m_x( xyz[ 0 ] ) ,
      m_y( xyz[ 1 ] ) ,
      m_z( xyz[ 2 ] )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >
CTPoint3D& operator=( const CTPoint3D< T >& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_x = src.m_x;
        m_y = src.m_y;
        m_z = src.m_z;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template < typename T >
bool
CTPoint3D< T >::operator==( const CTPoint3D& other ) const
{GUCEF_TRACE;
    
    return m_x == other.m_x &&
           m_y == other.m_y &&
           m_z == other.m_z;
}

/*-------------------------------------------------------------------------*/

template < typename T >
bool
CTPoint3D< T >::operator!=( const CTPoint3D& other ) const
{GUCEF_TRACE;
    
    return m_x != other.m_x ||
           m_y != other.m_y ||
           m_z != other.m_z;
}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::TRelatedVector3D
CTPoint3D< T >::operator-( const CTPoint3D& other ) const
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >
CTPoint3D< T >::operator+( const TRelatedVector3D& translateVector ) const
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >
CTPoint3D< T >::operator-( const TRelatedVector3D& translateVector ) const
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >&
CTPoint3D< T >::operator+=( const TRelatedVector3D& translateVector )
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >&
CTPoint3D< T >::operator-=( const TRelatedVector3D& translateVector )
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MATH */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MATH_CTVECTOR3D_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

---------------------------------------------------------------------------*/
