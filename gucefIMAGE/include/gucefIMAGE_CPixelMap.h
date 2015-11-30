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

#ifndef GUCEF_IMAGE_CPIXELMAP_H
#define GUCEF_IMAGE_CPIXELMAP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_IMAGE_MACROS_H
#include "gucefIMAGE_macros.h"       /* module macro's */
#define GUCEF_IMAGE_MACROS_H
#endif /* GUCEF_IMAGE_MACROS_H ? */

#ifndef GUCEF_IMAGE_IMAGEDATA_H
#include "gucefIMAGE_imagedata.h"               /* C-style shared header for image related types */
#define GUCEF_IMAGE_IMAGEDATA_H
#endif /* GUCEF_IMAGE_IMAGEDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

// Forward declarations of classes used
namespace GUCEF { namespace CORE  { class CIOAccess; } }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_IMAGE_EXPORT_CPP CPixelMap
{
    public:

    typedef CORE::CTSharedPtr< CPixelMap > TPixelMapPtr;
    
    CPixelMap( const TImageMipMapLevel& mipmapLevel );
    
    CPixelMap( const void* pixelMapData                      , 
               const UInt32 widthInPixels                    ,
               const UInt32 heightInPixels                   ,
               const TPixelStorageFormat pixelStorageFormat  ,
               const TBuildinDataType pixelComponentDataType );

    CPixelMap( const CPixelMap& src );
    
    virtual ~CPixelMap();
    
    CPixelMap& operator=( const CPixelMap& src );
    
    UInt32 GetWidthInBytes( void ) const;
    
    UInt32 GetHeightInBytes( void ) const;

    UInt32 GetWidthInPixels( void ) const;
    
    UInt32 GetHeightInPixels( void ) const;
    
    UInt32 GetPixelCount( void ) const;
    
    UInt32 GetPixelChannelSize( void ) const;
    
    UInt32 GetSizeOfPixelInBytes( void ) const;
    
    UInt32 GetTotalSizeInBytes( void ) const; 
    
    UInt32 GetNumberOfChannelsPerPixel( void ) const;   
    
    TPixelStorageFormat GetPixelStorageFormat( void ) const;
    
    void* GetDataPtr( const UInt32 pixelOffset = 0 );
    
    const void* GetDataPtr( const UInt32 pixelOffset = 0 ) const;

    void* RelinquishPixelDataOwnership( void );

    TBuildinDataType GetPixelComponentDataType( void ) const;
   
    void* GetDataAtScanLine( const UInt32 scanLineIndex );
    
    const void* GetDataAtScanLine( const UInt32 scanLineIndex ) const;

    /**
     *  flips image vertically
     *  This operation will be applied to all frames and mipmap levels    
     *
     *  @return wheter or the the flip was successfull.
     */
    bool FlipVertical( void );

    /**
     *      flips image horizontally
     *
     *      @return wheter or the the flip was successfull.
     */
    bool FlipHorizontal( void );
    
    /**
     *  Use mask color given to set the aplha value of each pixel to 0
     *  when the color matches the given values.
     *
     *  Note that if the TPixelStorageFormat is not RGBA the data
     *  will be converted to RGBA implicitly before the mask is applied
     *
     *      @param r        value of the red channel to match
     *      @param g        value of the green channel to match
     *      @param b        value of the blue channel to match
     */
    bool ApplyMaskColor( Float32 r ,
                         Float32 g ,
                         Float32 b );    

    /**
     *  Attempts to convert the pixel format into the given format and gives back the new pixelmap
     */
    bool ConvertFormatTo( const TPixelStorageFormat pixelStorageFormat  , 
                          const TBuildinDataType pixelComponentDataType ,
                          TPixelMapPtr& newMap                          );


    bool ConvertFormatTo( const TPixelStorageFormat pixelStorageFormat  , 
                          TPixelMapPtr& newMap                          );

    bool ConvertFormatTo( const TBuildinDataType pixelComponentDataType , 
                          TPixelMapPtr& newMap                          );

    /**
     *      Check if the image data has an alpha channel
     *
     *      @return Wheter or not the image data has an alpha channel.
     */
    bool GetHasAlpha( void ) const;
    
    /**
     *      Unloads pixel data if needed and resets values to there defaults
     */
    void Clear( void );
    
    /**
     *  Assigns the data to the object.
     *  If no source pixel data is given then a 0'ed pixel grid
     *  will be generated instead which can be altered at a later time
     *
     *  @param pixelMapData pointer to the source pixel data, can be NULL in which case a pixel map is generated
     *  @param widthInPixels width of the pixel map in pixels
     *  @param heightInPixels height of the pixel map in pixels
     *  @param pixelStorageFormat format used to define a pixel, ie. how the pixel component(s) make up a pixel
     *  @param pixelComponentDataType data type used to store a single pixel component of a single pixel
     */
    void Assign( const void* pixelMapData                      , 
                 const UInt32 widthInPixels                    ,
                 const UInt32 heightInPixels                   ,
                 const TPixelStorageFormat pixelStorageFormat  ,
                 const TBuildinDataType pixelComponentDataType );
                 
    /**
     *  Copy the pixel map data to the resource given.
     *  No meta data is added, just the pixel map itself
     *
     *  @param resource the resource to which the pixel map will be written
     */
    bool CopyTo( CORE::CIOAccess& resource );
    
    static UInt32 GetChannelCountForFormat( const TPixelStorageFormat pixelStorageFormat );
    
    static UInt32 GetPixelChannelSize( const TBuildinDataType pixelComponentDataType );
    
    static UInt32 GetPixelSize( const TPixelStorageFormat pixelStorageFormat  ,
                                const TBuildinDataType pixelComponentDataType );

    /**
     *  Returns the expected size of a pixel map given the width and height and pixel
     *  properties. If the size cannot be determined 0 will be returned.
     */
    static UInt32 GetExpectedPixelMapSize( const UInt32 widthInPixels                    ,
                                           const UInt32 heightInPixels                   ,
                                           const TPixelStorageFormat pixelStorageFormat  ,
                                           const TBuildinDataType pixelComponentDataType );
    
    /**
     *  Utility member function allowing assignment to the channels of a pixel without
     *  having to directly deal with different storage sizes of each channel
     */
    template< typename T >
    bool AssignToPixel( const UInt32 pixelIndex ,
                        const T* channelValues  );

    private:
    
    CPixelMap( void );
    
    template< typename T >
    bool ConvertFormatToImp( T* pixelMapData                               ,
                             const TPixelStorageFormat pixelStorageFormat  , 
                             const TBuildinDataType pixelComponentDataType ,
                             TPixelMapPtr& newMap                          );

    private:
    
    UInt32 m_widthInPixels;
    UInt32 m_heightInPixels;
    TPixelStorageFormat m_pixelStorageFormat;
    TBuildinDataType m_pixelComponentDataType;
    
    UInt8* m_pixelMapData; 
};

/*-------------------------------------------------------------------------*/

typedef CPixelMap::TPixelMapPtr TPixelMapPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
bool
CPixelMap::AssignToPixel( const UInt32 pixelIndex ,
                          const T* channelValues  )
{GUCEF_TRACE;

    UInt32 channelCount = GetNumberOfChannelsPerPixel();
    void* pixelAddr = GetDataPtr( pixelIndex );
    
    // Pixel color values as an industry standard dont use negative values
    // 0 is already the absence of light, negative light makes no sense, thus we use:
    // NewValue = (OldValue * NewRange) / OldRange

    switch ( m_pixelComponentDataType )
    {
        case MT::DATATYPE_FLOAT32:
        {
            CORE::Float32* pixel = (CORE::Float32*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::Float32 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Float32 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Float32 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::Float32 >::max() );
                }
            }
            return true;
        }
        case MT::DATATYPE_FLOAT64:
        {
            CORE::Float64* pixel = (CORE::Float64*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::Float64 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Float64 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Float64 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::Float64 >::max() );
                }
            }
            return true;
        }
        case MT::DATATYPE_UINT8:
        {
            CORE::UInt8* pixel = (CORE::UInt8*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::UInt8 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::UInt8 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::UInt8 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::UInt8 >::max() );
                }
            }
            return true;
        }
        case MT::DATATYPE_INT8:
        {
            CORE::Int8* pixel = (CORE::Int8*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::Int8 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Int8 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Int8 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::Int8 >::max() );
                }
            }
            return true;
        }             
        case MT::DATATYPE_UINT16:
        {
            CORE::UInt16* pixel = (CORE::UInt16*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::UInt16 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::UInt16 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::UInt16 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::UInt16 >::max() );
                }
            }
            return true;
        }
        case MT::DATATYPE_INT16:
        {
            CORE::Int16* pixel = (CORE::Int16*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::Int16 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Int16 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Int16 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::Int16 >::max() );
                }
            }
            return true;
        }
        case MT::DATATYPE_UINT32:
        {
            CORE::UInt32* pixel = (CORE::UInt32*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::UInt32 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::UInt32 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::UInt32 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::UInt32 >::max() );
                }
            }
            return true;
        }
        case MT::DATATYPE_INT32:
        {
            CORE::Int32* pixel = (CORE::Int32*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::Int32 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Int32 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Int32 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::Int32 >::max() );
                }
            }            
            return true;
        }
        case MT::DATATYPE_UINT64:
        {
            CORE::UInt64* pixel = (CORE::UInt64*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::UInt64 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::UInt64 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::UInt64 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::UInt64 >::max() );
                }
            }
            return true;
        }
        case MT::DATATYPE_INT64:
        {
            CORE::Int64* pixel = (CORE::Int64*) pixelAddr;
            if ( std::numeric_limits< T >::max() == std::numeric_limits< CORE::Int64 >::max() )
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Int64 >( channelValues[ i ] );
                }
            }
            else
            {
                for ( UInt32 i=0; i<channelCount; ++i )
                {
                    pixel[ i ] = static_cast< CORE::Int64 >( ( channelValues[ i ] * std::numeric_limits< T >::max() ) / std::numeric_limits< CORE::Int64 >::max() );
                }
            }
            return true;
        }
    }

    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-11-2006 :
        - Dinand: designed and implemented CPixelMap class

---------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CPIXELMAP_H ? */
