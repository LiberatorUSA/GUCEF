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

class EXPORT_CPP CPixelMap
{
    public:
    
    CPixelMap( const CPixelMap& src );
    
    virtual ~CPixelMap();
    
    CPixelMap& operator=( const CPixelMap& src );
    
    UInt32 GetWidthInBits( void ) const;
    
    UInt32 GetHeightInBits( void ) const;
    
    TPixelStorageFormat GetPixelStorageFormat( void ) const;
    
    UInt32 GetStrideInBits( void ) const;
    
    virtual void* GetDataPtr( void );
    
    virtual const void* GetDataPtr( void ) const;

    virtual TBuildinDataType GetPixelComponentDataType( void ) const = 0;

    void* GetDataAtPixelOffset( const UInt32 pixelIndex );
    
    const void* GetDataAtPixelOffset( const UInt32 pixelIndex ) const;
    
    void* GetDataAtScanLine( const UInt32 scanLineIndex );
    
    const void* GetDataAtScanLine( const UInt32 scanLineIndex ) const;
    
    UInt32 GetTotalSizeInBytes( void ) const;

    /**
     *      flips image vertically
     *      Note that flipping is not possible when useing compressed data
     *
     *      @return wheter or the the flip was successfull.
     */
    bool FlipVertical( void );

    /**
     *      flips image horizontally
     *      Note that flipping is not possible when useing compressed data
     *
     *      @return wheter or the the flip was successfull.
     */
    bool FlipHorizontal( void );
    
    /**
     *      Use mask color given to set the aplha value of each pixel to 0
     *      when the color matches the given values.
     *
     *      @param r        value of the red channel to match
     *      @param g        value of the green channel to match
     *      @param b        value of the blue channel to match
     */
    bool ApplyMaskColor( Float32 r ,
                         Float32 g ,
                         Float32 b );    

    /**
     *      Check if the image data has an alpha channel
     *
     *      @return Wheter or not the image data has an alpha channel.
     */
    bool GetHasAlpha( void ) const;

    /**
     *      Adds an alpha channel to the stored data.
     *      The alpha channel values are not initialized if an alpha channel
     *      is added. Alpha channel data can be set with ApplyMaskColor()
     */
    void ForceAlphaChannel( void );
    
    /**
     *      Unloads pixel data if needed and resets values to there defaults
     */
    void Clear( void );    
    
    protected:
    
    CPixelMap( void );
    
    private:
    
    UInt32 m_widthInPixels;
    UInt32 m_heightInPixels;
    TPixelStorageFormat m_pixelStorageFormat;
    TBuildinDataType m_pixelComponentDataType;
    
    void* m_pixelMapData; 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CPIXELMAP_H ? */
