/*
 *  gucefIMAGE: GUCEF module providing image utilities
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"          /* abstract base for IO Access */
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_IMAGE_CPIXELMAP_H
#include "gucefIMAGE_CPixelMap.h"
#define GUCEF_IMAGE_CPIXELMAP_H
#endif /* GUCEF_IMAGE_CPIXELMAP_H ? */

#include "gucefIMAGE_CPixel.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPixel::CPixel( void )
    : m_pixelStorageFormat( PSF_UNKNOWN )              ,
      m_pixelComponentDataType( MT::DATATYPE_UNKNOWN ) ,  
      m_pixelData( NULL )                              ,
      m_ownsData( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPixel::CPixel( const void* pixelData                         , 
                const TPixelStorageFormat pixelStorageFormat  ,
                const TBuildinDataType pixelComponentDataType ,
                bool referenceData /* = false */              )
    : m_pixelStorageFormat( pixelStorageFormat )         ,
      m_pixelComponentDataType( pixelComponentDataType ) ,  
      m_pixelData( NULL )                                ,
      m_ownsData( false )
{GUCEF_TRACE;

    if ( referenceData )
    {
        m_pixelData = (UInt8*) pixelData;
        m_ownsData = false;
    }
    else
    {
        UInt32 pixelSize = GetSizeOfPixelInBytes();
        m_pixelData = new UInt8[ pixelSize ];
        memcpy( m_pixelData, pixelData, pixelSize );
        m_ownsData = true;
    }
}

/*-------------------------------------------------------------------------*/

CPixel::CPixel( const CPixel& src )
    : m_pixelStorageFormat( src.m_pixelStorageFormat )         ,
      m_pixelComponentDataType( src.m_pixelComponentDataType ) ,  
      m_pixelData( NULL )                                      ,
      m_ownsData( false )
{GUCEF_TRACE;

    if ( !src.m_ownsData )
    {
        m_pixelData = src.m_pixelData;
        m_ownsData = false;
    }
    else
    {
        UInt32 pixelSize = GetSizeOfPixelInBytes();
        m_pixelData = new UInt8[ pixelSize ];
        memcpy( m_pixelData, src.m_pixelData, pixelSize );
        m_ownsData = true;
    }
}

/*-------------------------------------------------------------------------*/
    
CPixel::~CPixel()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/

UInt32
CPixel::GetSizeOfPixelInBytes( void ) const
{GUCEF_TRACE;

    return CPixelMap::GetExpectedPixelMapSize( 1, 1, m_pixelStorageFormat, m_pixelComponentDataType );
}

/*-------------------------------------------------------------------------*/

UInt32 
CPixel::GetNumberOfChannels( void ) const
{GUCEF_TRACE;

    return CPixelMap::GetChannelCountForFormat(  m_pixelStorageFormat );
}

/*-------------------------------------------------------------------------*/
    
TPixelStorageFormat 
CPixel::GetPixelStorageFormat( void ) const
{GUCEF_TRACE;

    return m_pixelStorageFormat;
}

/*-------------------------------------------------------------------------*/
    
void* 
CPixel::GetDataPtr( void )
{GUCEF_TRACE;

    return m_pixelData;
}

/*-------------------------------------------------------------------------*/
    
const void* 
CPixel::GetDataPtr( void ) const
{GUCEF_TRACE;

    return m_pixelData;
}

/*-------------------------------------------------------------------------*/

void* 
CPixel::RelinquishPixelDataOwnership( void )
{GUCEF_TRACE;

    void* pixelData = m_pixelData;
    m_pixelData = NULL;
    return pixelData;
}

/*-------------------------------------------------------------------------*/

TBuildinDataType 
CPixel::GetPixelComponentDataType( void ) const
{GUCEF_TRACE;

    return m_pixelComponentDataType;
}

/*-------------------------------------------------------------------------*/

bool 
CPixel::HasAlpha( void ) const
{GUCEF_TRACE;

    return CPixelMap::GetHasAlpha( m_pixelStorageFormat );
}

/*-------------------------------------------------------------------------*/
    
void 
CPixel::Clear( void )
{GUCEF_TRACE;

    if ( m_ownsData )
    {
        delete []m_pixelData;
    }
    m_pixelData = NULL;
    m_ownsData = false;
}

/*-------------------------------------------------------------------------*/                 

bool 
CPixel::CopyTo( CORE::CIOAccess& resource )
{GUCEF_TRACE;

    if ( NULL != m_pixelData )
    {
        return 1 == resource.Write( m_pixelData, GetSizeOfPixelInBytes(), 1 );
    }
    return true;
}

/*-------------------------------------------------------------------------*/
    
CPixel& 
CPixel::operator=( const CPixel& other )
{GUCEF_TRACE;

    if ( &other != this )
    {
        Clear();

        UInt32 pixelSize = other.GetSizeOfPixelInBytes();
        m_pixelData = new UInt8[ pixelSize ];
        memcpy( m_pixelData, other.m_pixelData, pixelSize );
        m_ownsData = true;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CPixel::operator==( const CPixel& other ) const
{GUCEF_TRACE;

    if ( m_pixelStorageFormat != other.m_pixelStorageFormat ) return false;
    if ( m_pixelComponentDataType != other.m_pixelComponentDataType ) return false;

    UInt32 ourPixelSize = GetSizeOfPixelInBytes();
    UInt32 otherPixelSize = other.GetSizeOfPixelInBytes();
    if ( ourPixelSize != otherPixelSize ) return false;

    return 0 == memcmp( m_pixelData, other.m_pixelData, ourPixelSize );
}

/*-------------------------------------------------------------------------*/

bool 
CPixel::operator<( const CPixel& other ) const
{GUCEF_TRACE;

    if ( m_pixelStorageFormat < other.m_pixelStorageFormat ) return true;
    if ( m_pixelComponentDataType < other.m_pixelComponentDataType ) return true;

    UInt32 ourPixelSize = GetSizeOfPixelInBytes();
    UInt32 otherPixelSize = other.GetSizeOfPixelInBytes();
    if ( ourPixelSize < otherPixelSize ) return true;

    return 0 < memcmp( m_pixelData, other.m_pixelData, ourPixelSize );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

