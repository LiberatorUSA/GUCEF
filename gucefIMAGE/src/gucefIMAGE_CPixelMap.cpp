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
#include <math.h>

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

//    ConvertPixelStorageFormatTo( PSF_RGBA );
    
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

template< typename T >
bool
CPixelMap::ConvertFormatToImp( T* pixelMapData                               ,
                               const TPixelStorageFormat pixelStorageFormat  , 
                               const TBuildinDataType pixelComponentDataType ,
                               TPixelMapPtr& newMap                          )
{
    // create new pixelmap with enough space
    newMap = new CPixelMap( NULL, m_widthInPixels, m_heightInPixels, pixelStorageFormat, pixelComponentDataType );
    
    UInt32 pixelCount = GetPixelCount();
    UInt32 channelCount = GetNumberOfChannelsPerPixel();

    switch ( m_pixelStorageFormat )
    {   
        case PSF_BGR:
        {
            switch ( pixelStorageFormat )
            {
                case PSF_RGB:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 3 ] = { orgPixelData[ 2 ], orgPixelData[ 1 ], orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_RGBA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 4 ] = { orgPixelData[ 2 ], orgPixelData[ 1 ], orgPixelData[ 0 ], 1 }; // fully opaque

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_BGRA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 4 ] = { orgPixelData[ 0 ], orgPixelData[ 1 ], orgPixelData[ 2 ], 1 };  // fully opaque

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_RED:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 2 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_GREEN:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 1 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_BLUE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }         
                case PSF_SINGLE_CHANNEL_STD_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (standard for certain colour spaces): (0.2126*R + 0.7152*G + 0.0722*B) 
                        CORE::Float64 luminance = ( 0.0722f * orgPixelData[ 0 ] ) + ( 0.7152f * orgPixelData[ 1 ] ) + ( 0.2126f * orgPixelData[ 2 ] );                      
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_P1_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (perceived option 1): (0.299*R + 0.587*G + 0.114*B)                       
                        CORE::Float64 luminance = ( 0.114f * orgPixelData[ 0 ] ) + ( 0.587f * orgPixelData[ 1 ] ) + ( 0.299f * orgPixelData[ 2 ] );
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_P2_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (perceived option 1): sqrt( 0.299*R^2 + 0.587*G^2 + 0.114*B^2 )                      
                        CORE::Float64 luminance = sqrt( ( 0.114f * pow( orgPixelData[ 0 ], 2 ) ) + ( 0.587f * pow( orgPixelData[ 1 ], 2 ) ) + ( 0.299f * pow( orgPixelData[ 2 ], 2 ) ) );
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
            }
            break;
        }
        case PSF_BGRA:
        {
            switch ( pixelStorageFormat )
            {
                case PSF_RGB:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 3 ] = { orgPixelData[ 2 ], orgPixelData[ 1 ], orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_RGBA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 4 ] = { orgPixelData[ 2 ], orgPixelData[ 1 ], orgPixelData[ 0 ], orgPixelData[ 3 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_BGR:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 3 ] = { orgPixelData[ 0 ], orgPixelData[ 1 ], orgPixelData[ 2 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_RED:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 2 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_GREEN:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 1 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_BLUE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }         
                case PSF_SINGLE_CHANNEL_ALPHA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 3 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_STD_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (standard for certain colour spaces): (0.2126*R + 0.7152*G + 0.0722*B)                        
                        CORE::Float64 luminance = ( 0.0722f * orgPixelData[ 0 ] ) + ( 0.7152f * orgPixelData[ 1 ] ) + ( 0.2126f * orgPixelData[ 2 ] );
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_P1_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (perceived option 1): (0.299*R + 0.587*G + 0.114*B)                       
                        CORE::Float64 luminance = ( 0.114f * orgPixelData[ 0 ] ) + ( 0.587f * orgPixelData[ 1 ] ) + ( 0.299f * orgPixelData[ 2 ] );
                        T pixelData[ 1 ] { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_P2_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (perceived option 1): sqrt( 0.299*R^2 + 0.587*G^2 + 0.114*B^2 )                      
                        CORE::Float64 luminance = sqrt( ( 0.114f * pow( orgPixelData[ 0 ], 2 ) ) + ( 0.587f * pow( orgPixelData[ 1 ], 2 ) ) + ( 0.299f * pow( orgPixelData[ 2 ], 2 ) ) );
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
            }
            break;
        }
        case PSF_RGB:
        {
            switch ( pixelStorageFormat )
            {
                case PSF_BGR:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 3 ] = { orgPixelData[ 2 ], orgPixelData[ 1 ], orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_BGRA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 4 ] = { orgPixelData[ 2 ], orgPixelData[ 1 ], orgPixelData[ 0 ], 0 }; // fully opaque

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_RGBA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 4 ] = { orgPixelData[ 0 ], orgPixelData[ 1 ], orgPixelData[ 2 ], 0 };  // fully opaque

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_RED:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_GREEN:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 1 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_BLUE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 2 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }         
                case PSF_SINGLE_CHANNEL_STD_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (standard for certain colour spaces): (0.2126*R + 0.7152*G + 0.0722*B)                        
                        CORE::Float64 luminance = ( 0.0722f * orgPixelData[ 2 ] ) + ( 0.7152f * orgPixelData[ 1 ] ) + ( 0.2126f * orgPixelData[ 0 ] );
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_P1_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (perceived option 1): (0.299*R + 0.587*G + 0.114*B)                       
                        CORE::Float64 luminance = ( 0.114f * orgPixelData[ 2 ] ) + ( 0.587f * orgPixelData[ 1 ] ) + ( 0.299f * orgPixelData[ 0 ] );
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_P2_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (perceived option 1): sqrt( 0.299*R^2 + 0.587*G^2 + 0.114*B^2 )                      
                        CORE::Float64 luminance = sqrt( ( 0.114f * pow( orgPixelData[ 2 ], 2 ) ) + ( 0.587f * pow( orgPixelData[ 1 ], 2 ) ) + ( 0.299f * pow( orgPixelData[ 0 ], 2 ) ) );
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
            }
            break;
        }
        case PSF_RGBA:
        {
            switch ( pixelStorageFormat )
            {
                case PSF_BGR:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 3 ] = { orgPixelData[ 2 ], orgPixelData[ 1 ], orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_BGRA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 4 ] = { orgPixelData[ 2 ], orgPixelData[ 1 ], orgPixelData[ 0 ], orgPixelData[ 3 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_RGB:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 3 ] = { orgPixelData[ 0 ], orgPixelData[ 1 ], orgPixelData[ 2 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_RED:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_GREEN:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 1 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_BLUE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 2 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }         
                case PSF_SINGLE_CHANNEL_ALPHA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 3 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_STD_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (standard for certain colour spaces): (0.2126*R + 0.7152*G + 0.0722*B)
                        CORE::Float64 luminance = ( 0.0722f * orgPixelData[ 2 ] ) + ( 0.7152f * orgPixelData[ 1 ] ) + ( 0.2126f * orgPixelData[ 0 ] );                        
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_P1_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (perceived option 1): (0.299*R + 0.587*G + 0.114*B)                       
                        CORE::Float64 luminance = ( 0.114f * orgPixelData[ 2 ] ) + ( 0.587f * orgPixelData[ 1 ] ) + ( 0.299f * orgPixelData[ 0 ] );
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_P2_LUMINANCE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);
                        
                        // Luminance (perceived option 1): sqrt( 0.299*R^2 + 0.587*G^2 + 0.114*B^2 )                      
                        CORE::Float64 luminance = sqrt( ( 0.114f * pow( orgPixelData[ 2 ], 2 ) ) + ( 0.587f * pow( orgPixelData[ 1 ], 2 ) ) + ( 0.299f * pow( orgPixelData[ 0 ], 2 ) ) ); 
                        T pixelData[ 1 ] = { (T) round( luminance ) };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
            }
            break;
        }
        case PSF_SINGLE_CHANNEL_GRAYSCALE:
        {
            switch ( pixelStorageFormat )
            {
                case PSF_BGR:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 3 ] = { orgPixelData[ 0 ], orgPixelData[ 0 ], orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_BGRA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 4 ] = { orgPixelData[ 0 ], orgPixelData[ 0 ], orgPixelData[ 0 ], 0 };  // fully opaque

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_RGB:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 3 ] = { orgPixelData[ 0 ], orgPixelData[ 0 ], orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_RED:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_GREEN:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
                case PSF_SINGLE_CHANNEL_BLUE:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }         
                case PSF_SINGLE_CHANNEL_ALPHA:
                {
                    // copy segments and alter/supplement
                    for ( UInt32 i=0; i<pixelCount; ++i )
                    {                            
                        T* orgPixelData = pixelMapData+(i*channelCount);                        
                        T pixelData[ 1 ] = { orgPixelData[ 0 ] };

                        // Assign new pixel value and convert data type from T to its type
                        newMap->AssignToPixel< T >( i, pixelData );
                    }
                    return true;
                }
            }
            break;
        }
    }   

    newMap = NULL;
    return false;
}

/*--------------------------------------------------------------------------*/

bool
CPixelMap::ConvertFormatTo( const TPixelStorageFormat pixelStorageFormat  , 
                            const TBuildinDataType pixelComponentDataType ,
                            TPixelMapPtr& newMap                          )
{GUCEF_TRACE;

    // Only do conversions if needed
    if ( m_pixelStorageFormat != pixelStorageFormat || m_pixelComponentDataType != pixelComponentDataType )
    {
        switch ( m_pixelComponentDataType )
        {
            case MT::DATATYPE_FLOAT32:
            {
                CORE::Float32* pixelMapData = (CORE::Float32*) m_pixelMapData;
                return ConvertFormatToImp< CORE::Float32 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_FLOAT64:
            {
                CORE::Float64* pixelMapData = (CORE::Float64*) m_pixelMapData;
                return ConvertFormatToImp< CORE::Float64 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_UINT8:
            {
                CORE::UInt8* pixelMapData = (CORE::UInt8*) m_pixelMapData;
                return ConvertFormatToImp< CORE::UInt8 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_INT8:
            {
                CORE::Int8* pixelMapData = (CORE::Int8*) m_pixelMapData;
                return ConvertFormatToImp< CORE::Int8 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_UINT16:
            {
                CORE::UInt16* pixelMapData = (CORE::UInt16*) m_pixelMapData;
                return ConvertFormatToImp< CORE::UInt16 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_INT16:
            {
                CORE::Int16* pixelMapData = (CORE::Int16*) m_pixelMapData;
                return ConvertFormatToImp< CORE::Int16 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_UINT32:
            {
                CORE::UInt32* pixelMapData = (CORE::UInt32*) m_pixelMapData;
                return ConvertFormatToImp< CORE::UInt32 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_INT32:
            {
                CORE::Int32* pixelMapData = (CORE::Int32*) m_pixelMapData;
                return ConvertFormatToImp< CORE::Int32 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_UINT64:
            {
                CORE::UInt64* pixelMapData = (CORE::UInt64*) m_pixelMapData;
                return ConvertFormatToImp< CORE::UInt64 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            case MT::DATATYPE_INT64:
            {
                CORE::Int64* pixelMapData = (CORE::Int64*) m_pixelMapData;
                return ConvertFormatToImp< CORE::Int64 >( pixelMapData, pixelStorageFormat, pixelComponentDataType, newMap );
            }
            default:
            {
                // this should not happen
                return false;
            }
        }
    }
    else
    {
        // no change needed
        newMap = NULL;
        return true;
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
        case PSF_SINGLE_CHANNEL_STD_LUMINANCE :
        case PSF_SINGLE_CHANNEL_P1_LUMINANCE :
        case PSF_SINGLE_CHANNEL_P2_LUMINANCE :
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
