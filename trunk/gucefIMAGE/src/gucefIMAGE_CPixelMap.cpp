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

#include <string.h> /* <- for memset() */
#include <assert.h>

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CMFILEACCESS_H
#include "CMFileAccess.h"
#define GUCEF_CORE_CMFILEACCESS_H
#endif /* GUCEF_CORE_CMFILEACCESS_H ? */

#include "gucefIMAGE_CPixelMap.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPixelMap::CPixelMap( const TImageMipMapLevel& mipmapLevel )
    : m_pixelComponentDataType( (TBuildinDataType) mipmapLevel.mipLevelInfo.pixelComponentDataType ) ,
      m_pixelStorageFormat( (TPixelStorageFormat) mipmapLevel.mipLevelInfo.pixelStorageFormat )      ,
      m_heightInPixels( mipmapLevel.mipLevelInfo.frameHeight )                                       ,
      m_widthInPixels( mipmapLevel.mipLevelInfo.frameWidth )                                         ,
      m_pixelMapData( NULL )
{GUCEF_TRACE;

    m_pixelMapData = new UInt8[ mipmapLevel.pixelDataSizeInBytes ];
    memcpy( m_pixelMapData, mipmapLevel.pixelData, mipmapLevel.pixelDataSizeInBytes );
}

/*------------------------------------------------------------------------*/

CPixelMap::CPixelMap( const void* pixelMapData                      , 
                      const UInt32 widthInPixels                    ,
                      const UInt32 heightInPixels                   ,
                      const TPixelStorageFormat pixelStorageFormat  ,
                      const TBuildinDataType pixelComponentDataType )
    : m_pixelComponentDataType( pixelComponentDataType ) ,
      m_pixelStorageFormat( pixelStorageFormat )         ,
      m_heightInPixels( 0 )                              ,
      m_widthInPixels( 0 )                               ,
      m_pixelMapData( NULL )
{GUCEF_TRACE;

    Assign( pixelMapData           ,
            widthInPixels          ,
            heightInPixels         ,
            pixelStorageFormat     ,
            pixelComponentDataType );
}

/*--------------------------------------------------------------------------*/

CPixelMap::CPixelMap( const CPixelMap& src )
    : m_pixelComponentDataType( src.m_pixelComponentDataType ) ,
      m_pixelStorageFormat( src.m_pixelStorageFormat )         ,
      m_heightInPixels( 0 )                                    ,
      m_widthInPixels( 0 )                                     ,
      m_pixelMapData( NULL )
{GUCEF_TRACE;

    Assign( src.m_pixelMapData           ,
            src.m_widthInPixels          ,
            src.m_heightInPixels         ,
            src.m_pixelStorageFormat     ,
            src.m_pixelComponentDataType );    
}

/*--------------------------------------------------------------------------*/

CPixelMap::~CPixelMap()
{GUCEF_TRACE;

    Clear();
}

/*--------------------------------------------------------------------------*/

CPixelMap&
CPixelMap::operator=( const CPixelMap& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        Assign( src.m_pixelMapData           ,
                src.m_widthInPixels          ,
                src.m_heightInPixels         ,
                src.m_pixelStorageFormat     ,
                src.m_pixelComponentDataType );
    }
    
    return *this;
}

/*--------------------------------------------------------------------------*/

void
CPixelMap::Assign( const void* pixelMapData                      , 
                   const UInt32 widthInPixels                    ,
                   const UInt32 heightInPixels                   ,
                   const TPixelStorageFormat pixelStorageFormat  ,
                   const TBuildinDataType pixelComponentDataType )
{GUCEF_TRACE;

    // First we clean up whatever may already be allocated
    Clear();
    
    // Assign the pixel map properties
    m_pixelComponentDataType = pixelComponentDataType;
    m_pixelStorageFormat = pixelStorageFormat;
    m_widthInPixels = widthInPixels;
    m_heightInPixels = heightInPixels;

    UInt32 totalSize = GetTotalSizeInBytes();
    m_pixelMapData = new UInt8[ totalSize ];
    
    // If no source data is given we will generate our own pixel map
    // The user can fill in the actual data at a later time
    if ( NULL == pixelMapData )
    {
        memset( m_pixelMapData, 0, totalSize );
    }
    else
    {
        memcpy( m_pixelMapData, pixelMapData, totalSize );
    }
}

/*--------------------------------------------------------------------------*/
    
UInt32
CPixelMap::GetWidthInBytes( void ) const
{GUCEF_TRACE;
    
    return GetSizeOfPixelInBytes() * GetWidthInPixels();
}

/*--------------------------------------------------------------------------*/
    
UInt32
CPixelMap::GetHeightInBytes( void ) const
{GUCEF_TRACE;

    return GetSizeOfPixelInBytes() * GetHeightInPixels();
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetWidthInPixels( void ) const
{GUCEF_TRACE;

    return m_widthInPixels;
}
    
/*--------------------------------------------------------------------------*/
    
UInt32
CPixelMap::GetHeightInPixels( void ) const
{GUCEF_TRACE;

    return m_heightInPixels;
}

/*--------------------------------------------------------------------------*/
    
TPixelStorageFormat
CPixelMap::GetPixelStorageFormat( void ) const
{GUCEF_TRACE;

    return m_pixelStorageFormat;
}

/*--------------------------------------------------------------------------*/
    
void*
CPixelMap::GetDataPtr( const UInt32 pixelOffset /* = 0 */ )
{GUCEF_TRACE;

    return m_pixelMapData + ( GetSizeOfPixelInBytes() * pixelOffset );
}

/*--------------------------------------------------------------------------*/
    
const void*
CPixelMap::GetDataPtr( const UInt32 pixelOffset /* = 0 */ ) const
{GUCEF_TRACE;

    return m_pixelMapData + ( GetSizeOfPixelInBytes() * pixelOffset );
}

/*--------------------------------------------------------------------------*/

TBuildinDataType
CPixelMap::GetPixelComponentDataType( void ) const
{GUCEF_TRACE;

    return m_pixelComponentDataType;
}

/*--------------------------------------------------------------------------*/    

void*
CPixelMap::GetDataAtScanLine( const UInt32 scanLineIndex )
{GUCEF_TRACE;
    
    return m_pixelMapData + ( scanLineIndex * GetWidthInBytes() );
}

/*--------------------------------------------------------------------------*/

const void*
CPixelMap::GetDataAtScanLine( const UInt32 scanLineIndex ) const
{GUCEF_TRACE;

    return m_pixelMapData + ( scanLineIndex * GetWidthInBytes() );
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetPixelChannelSize( void ) const
{GUCEF_TRACE;

    return GetPixelChannelSize( m_pixelComponentDataType );  
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetNumberOfChannelsPerPixel( void ) const
{GUCEF_TRACE;
    
    return GetChannelCountForFormat( m_pixelStorageFormat );
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetSizeOfPixelInBytes( void ) const
{GUCEF_TRACE;

    return GetNumberOfChannelsPerPixel() * GetPixelChannelSize();
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetPixelCount( void ) const
{GUCEF_TRACE;

    return m_widthInPixels * m_heightInPixels;
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetTotalSizeInBytes( void ) const
{GUCEF_TRACE;
    
    return GetPixelCount() * GetSizeOfPixelInBytes();
}

/*--------------------------------------------------------------------------*/

bool
CPixelMap::FlipVertical( void )
{GUCEF_TRACE;

    assert( 0 ); // @TODO makeme
    return false;
}

/*--------------------------------------------------------------------------*/

bool
CPixelMap::FlipHorizontal( void )
{GUCEF_TRACE;

    UInt32 scanLineSize = GetWidthInBytes(); 
    void* scanLineBuffer = new UInt8[ scanLineSize ]; 

    UInt32 centerLine = m_heightInPixels / 2;
    for ( UInt32 i=0; i<centerLine; ++i )
    {
        void* frontScanLine = GetDataAtScanLine( i );
        void* backScanLine =  GetDataAtScanLine( (m_heightInPixels-1)-i );
        
        // swap the rows
        memcpy( scanLineBuffer, frontScanLine, scanLineSize );
        memcpy( frontScanLine, backScanLine, scanLineSize );
        memcpy( backScanLine, scanLineBuffer, scanLineSize );
    }
    
    delete []scanLineBuffer;

    return true;
}
    
/*--------------------------------------------------------------------------*/

bool
CPixelMap::ApplyMaskColor( Float32 r ,
                           Float32 g ,
                           Float32 b )
{GUCEF_TRACE;

    ConvertPixelStorageFormatTo( PSF_RGBA );
    
    assert( 0 ); // @TODO makeme
    return false;
}

/*--------------------------------------------------------------------------*/

bool
CPixelMap::GetHasAlpha( void ) const
{GUCEF_TRACE;

    return ( m_pixelStorageFormat == PSF_RGBA ) || ( m_pixelStorageFormat == PSF_SINGLE_CHANNEL_ALPHA );
}
   
/*--------------------------------------------------------------------------*/

void
CPixelMap::Clear( void )
{GUCEF_TRACE;

    m_widthInPixels = 0;
    m_heightInPixels = 0;

    delete []m_pixelMapData;    
    m_pixelMapData = NULL;
}

/*--------------------------------------------------------------------------*/

void*
CPixelMap::RelinquishPixelDataOwnership( void )
{GUCEF_TRACE;

    m_widthInPixels = 0;
    m_heightInPixels = 0;

    void* pixelData = m_pixelMapData;    
    m_pixelMapData = NULL;
    return pixelData;
}

/*--------------------------------------------------------------------------*/

void
CPixelMap::ConvertPixelComponentDataTypeTo( const TBuildinDataType pixelComponentDataType )
{GUCEF_TRACE;

    if ( m_pixelComponentDataType != pixelComponentDataType )
    {
        assert( 0 ); // @TODO makeme    
    }
}

/*--------------------------------------------------------------------------*/

void
CPixelMap::ConvertPixelStorageFormatTo( const TPixelStorageFormat pixelStorageFormat )
{GUCEF_TRACE;

    if ( m_pixelStorageFormat != pixelStorageFormat )
    {
        assert( 0 ); // @TODO makeme    
    }
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetChannelCountForFormat( const TPixelStorageFormat pixelStorageFormat )
{GUCEF_TRACE;

    switch ( pixelStorageFormat )
    {
        case PSF_BGR :
        case PSF_RGB :
        {
            return 3;
        }
        case PSF_BGRA :
        case PSF_RGBA :
        {
            return 4;
        }
        case PSF_SINGLE_CHANNEL_RED :
        case PSF_SINGLE_CHANNEL_GREEN :
        case PSF_SINGLE_CHANNEL_BLUE :
        case PSF_SINGLE_CHANNEL_LUMINANCE :
        case PSF_SINGLE_CHANNEL_ALPHA :
        {
            return 1;
        }
        default :
        {
            return 0;
        }
    }
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetPixelChannelSize( const TBuildinDataType pixelComponentDataType )
{GUCEF_TRACE;

    switch ( pixelComponentDataType )
    {
        case GUCEF::MT::DATATYPE_INT8 :
        case GUCEF::MT::DATATYPE_UINT8 :
        {
            return 1;
        }
        case GUCEF::MT::DATATYPE_INT16 :
        case GUCEF::MT::DATATYPE_UINT16 :
        {
            return 2;
        }        
        case GUCEF::MT::DATATYPE_INT32 :
        case GUCEF::MT::DATATYPE_UINT32 :
        case GUCEF::MT::DATATYPE_FLOAT32 :
        {
            return 4;
        }
        default :
        {
            return 0;
        }        
    }            
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetPixelSize( const TPixelStorageFormat pixelStorageFormat  ,
                         const TBuildinDataType pixelComponentDataType )
{GUCEF_TRACE;
    
    return GetChannelCountForFormat( pixelStorageFormat ) * GetPixelChannelSize( pixelComponentDataType );
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetExpectedPixelMapSize( const UInt32 widthInPixels                    ,
                                    const UInt32 heightInPixels                   ,
                                    const TPixelStorageFormat pixelStorageFormat  ,
                                    const TBuildinDataType pixelComponentDataType )
{GUCEF_TRACE;

    return widthInPixels * heightInPixels * GetPixelSize( pixelStorageFormat, pixelComponentDataType );
}

/*--------------------------------------------------------------------------*/

bool
CPixelMap::CopyTo( CORE::CIOAccess& resource )
{GUCEF_TRACE;

    UInt32 dataSize = GetTotalSizeInBytes();
    CORE::CMFileAccess memoryFile( m_pixelMapData, dataSize );
    return resource.Write( memoryFile ) == dataSize;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
