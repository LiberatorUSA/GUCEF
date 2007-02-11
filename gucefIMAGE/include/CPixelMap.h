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

#ifndef GUCEF_IMAGE_CPIXELMAP_H
#define GUCEF_IMAGE_CPIXELMAP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"       /* module macro's */
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

#ifndef IMAGEDATA_H
#include "imagedata.h"               /* C-style shared header for image related types */
#define IMAGEDATA_H
#endif /* IMAGEDATA_H ? */

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
    
    UInt32 GetSizeOfPixelComponentInBytes( void ) const;
    
    UInt32 GetSizeOfPixelInBytes( void ) const;
    
    UInt32 GetTotalSizeInBytes( void ) const; 
    
    UInt32 GetNumberOfChannelsPerPixel( void ) const;   
    
    TPixelStorageFormat GetPixelStorageFormat( void ) const;
    
    void* GetDataPtr( const UInt32 pixelOffset = 0 );
    
    const void* GetDataPtr( const UInt32 pixelOffset = 0 ) const;

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

    void ConvertPixelStorageFormatTo( const TPixelStorageFormat pixelStorageFormat );

    void ConvertPixelComponentDataTypeTo( const TBuildinDataType pixelComponentDataType );

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
    
    private:
    
    CPixelMap( void );
    
    private:
    
    UInt32 m_widthInPixels;
    UInt32 m_heightInPixels;
    TPixelStorageFormat m_pixelStorageFormat;
    TBuildinDataType m_pixelComponentDataType;
    
    UInt8* m_pixelMapData; 
};

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

-----------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CPIXELMAP_H ? */
