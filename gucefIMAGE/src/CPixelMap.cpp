/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h> /* <- for memset() */
#include <assert.h>

#include "CTracer.h"

#include "CPixelMap.h"

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
{TRACE;

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
{TRACE;

    Assign( src.m_pixelMapData           ,
            src.m_widthInPixels          ,
            src.m_heightInPixels         ,
            src.m_pixelStorageFormat     ,
            src.m_pixelComponentDataType );    
}

/*--------------------------------------------------------------------------*/

CPixelMap::~CPixelMap()
{TRACE;

    Clear();
}

/*--------------------------------------------------------------------------*/

CPixelMap&
CPixelMap::operator=( const CPixelMap& src )
{TRACE;

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
{TRACE;

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
{TRACE;
    
    return GetSizeOfPixelComponentInBytes() * GetWidthInPixels();
}

/*--------------------------------------------------------------------------*/
    
UInt32
CPixelMap::GetHeightInBytes( void ) const
{TRACE;

    return GetSizeOfPixelComponentInBytes() * GetHeightInPixels();
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetWidthInPixels( void ) const
{TRACE;

    return m_widthInPixels;
}
    
/*--------------------------------------------------------------------------*/
    
UInt32
CPixelMap::GetHeightInPixels( void ) const
{TRACE;

    return m_heightInPixels;
}

/*--------------------------------------------------------------------------*/
    
TPixelStorageFormat
CPixelMap::GetPixelStorageFormat( void ) const
{TRACE;

    return m_pixelStorageFormat;
}

/*--------------------------------------------------------------------------*/
    
void*
CPixelMap::GetDataPtr( const UInt32 pixelOffset /* = 0 */ )
{TRACE;

    return m_pixelMapData + ( GetSizeOfPixelInBytes() * pixelOffset );
}

/*--------------------------------------------------------------------------*/
    
const void*
CPixelMap::GetDataPtr( const UInt32 pixelOffset /* = 0 */ ) const
{TRACE;

    return m_pixelMapData + ( GetSizeOfPixelInBytes() * pixelOffset );
}

/*--------------------------------------------------------------------------*/

TBuildinDataType
CPixelMap::GetPixelComponentDataType( void ) const
{TRACE;

    return m_pixelComponentDataType;
}

/*--------------------------------------------------------------------------*/    

void*
CPixelMap::GetDataAtScanLine( const UInt32 scanLineIndex )
{TRACE;
    
    return m_pixelMapData + ( scanLineIndex * GetWidthInBytes() );
}

/*--------------------------------------------------------------------------*/

const void*
CPixelMap::GetDataAtScanLine( const UInt32 scanLineIndex ) const
{TRACE;

    return m_pixelMapData + ( scanLineIndex * GetWidthInBytes() );
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetSizeOfPixelComponentInBytes( void ) const
{TRACE;

    switch ( m_pixelComponentDataType )
    {
        case MT::DT_UINT8 :
        case MT::DT_INT8  :
        {
            return 1;
        }
        case MT::DT_UINT16 :
        case MT::DT_INT16  :
        {
            return 2;
        }
        case MT::DT_FLOAT32 :
        case MT::DT_UINT32  :
        case MT::DT_INT32   :
        {
            return 4;
        }
        case MT::DT_FLOAT64 :
        case MT::DT_UINT64  :
        case MT::DT_INT64   :
        {
            return 8;
        }
        default :
        {
            // We should never get here
            GUCEF_UNREACHABLE;
            return 0;
        }
    }    
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetNumberOfChannelsPerPixel( void ) const
{TRACE;
    
    switch ( m_pixelStorageFormat )
    {
        case PSF_RGB :
        {
            return 3;
        }
        case PSF_RGBA :
        {
            return 4;
        }        
        case PSF_SINGLE_CHANNEL_RED :
        case PSF_SINGLE_CHANNEL_GREEN :
        case PSF_SINGLE_CHANNEL_BLUE :
        case PSF_SINGLE_CHANNEL_ALPHA :
        case PSF_SINGLE_CHANNEL_LUMINANCE :
        {
            return 1;
        }
        default :
        {
            // We should never get here
            GUCEF_UNREACHABLE;
            return 0;
        }                
    }    
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetSizeOfPixelInBytes( void ) const
{TRACE;

    return GetNumberOfChannelsPerPixel() * GetSizeOfPixelComponentInBytes();
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetPixelCount( void ) const
{TRACE;

    return m_widthInPixels * m_heightInPixels;
}

/*--------------------------------------------------------------------------*/

UInt32
CPixelMap::GetTotalSizeInBytes( void ) const
{TRACE;
    
    return GetPixelCount() * GetSizeOfPixelInBytes();
}

/*--------------------------------------------------------------------------*/

bool
CPixelMap::FlipVertical( void )
{TRACE;

    assert( 0 ); // @TODO makeme
    return false;
}

/*--------------------------------------------------------------------------*/

bool
CPixelMap::FlipHorizontal( void )
{TRACE;

    assert( 0 ); // @TODO makeme
    return false;
}
    
/*--------------------------------------------------------------------------*/

bool
CPixelMap::ApplyMaskColor( Float32 r ,
                           Float32 g ,
                           Float32 b )
{TRACE;

    ConvertPixelStorageFormatTo( PSF_RGBA );
    
    assert( 0 ); // @TODO makeme
    return false;
}

/*--------------------------------------------------------------------------*/

bool
CPixelMap::GetHasAlpha( void ) const
{TRACE;

    return ( m_pixelStorageFormat == PSF_RGBA ) || ( m_pixelStorageFormat == PSF_SINGLE_CHANNEL_ALPHA );
}
   
/*--------------------------------------------------------------------------*/

void
CPixelMap::Clear( void )
{TRACE;

    m_widthInPixels = 0;
    m_heightInPixels = 0;

    delete []m_pixelMapData;    
    m_pixelMapData = NULL;
}

/*--------------------------------------------------------------------------*/

void
CPixelMap::ConvertPixelComponentDataTypeTo( const TBuildinDataType pixelComponentDataType )
{TRACE;

    if ( m_pixelComponentDataType != pixelComponentDataType )
    {
        assert( 0 ); // @TODO makeme    
    }
}

/*--------------------------------------------------------------------------*/

void
CPixelMap::ConvertPixelStorageFormatTo( const TPixelStorageFormat pixelStorageFormat )
{TRACE;

    if ( m_pixelStorageFormat != pixelStorageFormat )
    {
        assert( 0 ); // @TODO makeme    
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
