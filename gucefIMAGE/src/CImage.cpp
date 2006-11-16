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

#include <malloc.h>       /* Memory Management */
#include <string.h>       /* needed for memcpy() */

#ifndef CIMGCODECMANAGER_H
#include "CIMGCodecManager.h"
#define CIMGCODECMANAGER_H
#endif /* CIMGCODECMANAGER_H ? */

#ifndef CIMAGE_H
#include "CImage.h"       /* Header for this class */
#define CIMAGE_H
#endif /* CIMAGE_H ? */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

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

/**
 *      Creates an empty image object
 */
CImage::CImage( void )
{
}

/*--------------------------------------------------------------------------*/

/**
 *      Cleans up any data that may have been allocated/stored
 */
CImage::~CImage()
{
        Clear();
}

/*-------------------------------------------------------------------------*/

const TImageInfo&
CImage::GetImageInfo( void ) const
{TRACE;
    
    return m_imageInfo;
} 

/*--------------------------------------------------------------------------*/

/**
 *      Get the frame size in bytes of a single frame at the highest
 *      mipmap level.
 *
 *      @return size of the frame data in bytes
 */
UInt32
CImage::GetFrameSize( void ) const
{
        return _imgdata.dsize;
}   

/*--------------------------------------------------------------------------*/

/**
 *      Get Bits Per Pixel
 *      Typical values are 8, 24, 32
 *
 *      @return the depth a.k.a bbp of the image data.
 */
UInt8
CImage::GetDepth( void ) const
{
        return _imgdata.bbp;
}

/*--------------------------------------------------------------------------*/

UInt32
CImage::GetWidth( void ) const
{
        return _imgdata.width;
}

/*--------------------------------------------------------------------------*/

UInt32
CImage::GetStride( void ) const
{
        return _imgdata.stride;
}

/*--------------------------------------------------------------------------*/

UInt32
CImage::GetHeight( void ) const
{
        return _imgdata.height;
}

/*--------------------------------------------------------------------------*/

UInt8
CImage::GetChannels( void ) const
{
        return _imgdata.channels;
}

/*--------------------------------------------------------------------------*/

Int8
CImage::GetFormat( void ) const
{
        return _imgdata.format;
}

/*--------------------------------------------------------------------------*/

UInt32
CImage::GetFrames( void ) const
{
        return _imgdata.frames;
}

/*--------------------------------------------------------------------------*/

/**
 *      Get the number of mipmap levels per frame
 *
 *      @return mipmap levels per frame.
 */
UInt8
CImage::GetMipmapLevels( void ) const
{
        return _imgdata.mipmaps;        
}

/*--------------------------------------------------------------------------*/

/**
 *      Get a pointer to the pixel data of the first frame.
 *      The pixel data is that of the highest mipmap level.
 *
 *      @return         pixel data of the requested frame.
 */
const void*
CImage::GetData( void ) const
{
        return _imgdata.idata;
}

/*--------------------------------------------------------------------------*/

/**
 *      Get a pointer to the pixel data of the frame given.
 *      The pixel data is that of the highest mipmap level.
 *
 *      @param frame    frame index for which you want the pixel data
 *      @return         pixel data of the requested frame.
 */
const void*
CImage::GetData( UInt32 frame ) const
{
        return _imgdata.idata;
}

/*--------------------------------------------------------------------------*/

/**
 *      Get a pointer to the pixel data of the frame given.
 *      The pixel data is that of the highest mipmap level.
 *
 *      @param frame      frame index for which you want the pixel data
 *      @param mipmapidx  index of the mipmap level.
 *      @return           pixel data of the requested frame.
 */
const void*
CImage::GetData( UInt32 frame    ,
                 UInt8 mipmapidx ) const
{
        return _imgdata.idata;                
}

/*--------------------------------------------------------------------------*/

/**
 *      Get the type of value that is used for each pixel component.
 *
 *      @return The type of each pixel component value.
 */
TBuildinDataType
CImage::GetPixelComponentDataType( void ) const
{
        return _imgdata.dtype;
}

/*--------------------------------------------------------------------------*/

/**
 *      Check if the image data has an alpha channel
 *
 *      @return Wheter or not the image data has an alpha channel.
 */
bool
CImage::GetHasAlpha( void ) const
{
        return _imgdata.has_alpha > 0;
}

/*--------------------------------------------------------------------------*/

/**
 *      Returns a refrence to the entire image data structure.
 *      Note that it is a better idea to use the member functions of
 *      this class to obtain the field values since they are less
 *      likely to change.
 *
 *      @return the image storage structure
 */
const TImageData&
CImage::GetImageDataStructure( void ) const
{
        return _imgdata;
}

/*--------------------------------------------------------------------------*/

/**
 *      Adds an alpha channel to the stored data.
 *      The alpha channel values are not initialized if an alpha channel
 *      is added. Alpha channel data can be set with ApplyMaskColor()
 */
void
CImage::ForceAlphaChannel( void )
{
        
}

/*--------------------------------------------------------------------------*/

/**
 *      Use mask color given to set the aplha value of each pixel to 0
 *      when the color matches the given values.
 *
 *      @param r        value of the red channel to match
 *      @param g        value of the green channel to match
 *      @param b        value of the blue channel to match
 */
void
CImage::ApplyMaskColor( Float32 r ,
                        Float32 g ,
                        Float32 b )
{

}

/*--------------------------------------------------------------------------*/

/**
 *      Attempts to load image data using the plugin identified with
 *      the provided handle. The plugin will attempt to load the image
 *      data from the recource provided by 'access' and store the
 *      result in 'imgdata'.
 */
bool
CImage::Load( UInt32 codecidx         ,
              UInt32 hidx             ,
              CORE::CIOAccess& access )
{
        Clear();

        if ( CIMGCodecManager::Instance()->LoadImageData( codecidx    ,
                                                          hidx        ,
                                                          access      ,
                                                          &_imgdata   ,
                                                          &_codecdata ) )
        {
                _codecisowner = true;
                _codecused = codecidx;
                _hidxused = hidx;
                return true;
        }
        return false;
}

/*--------------------------------------------------------------------------*/

/**
 *      This function should save the image data provided in the format
 *      provided to a file with the given filename. If format is one of the
 *      explicit types then the save must be in that format or the save
 *      operation should fail. if format is IMGFORMAT_DONT_CARE then the
 *      format of the output file can be decided uppon at the user's discression.
 *      A return value of 0 indicates failure. A return value of > 0 indicates
 *      success (I recommend using 1). compression is a percentage between 0
 *      and 100 indicating the desired amount of compression. 0 indicates the
 *      lowest level of compression the plugin can provide and 100 the highest.
 */
bool
CImage::Save( UInt32 codecidx               ,
              UInt32 hidx                   ,
              const CORE::CString& filename ,
              UInt32 format                 ,
              UInt32 compression            )
{
        return CIMGCodecManager::Instance()->SaveImageData( codecidx    ,
                                                            hidx        ,
                                                            filename    ,
                                                            format      ,
                                                            compression ,
                                                            &_imgdata   );
}

/*--------------------------------------------------------------------------*/

/**
 *      Unloads image data if needed and resets values to there defaults
 */
void
CImage::Clear( void )
{
    m_frameList.clear();
}

/*--------------------------------------------------------------------------*/

bool
CImage::FlipVertical( void )
{TRACE;
        return false;
}

/*--------------------------------------------------------------------------*/

bool
CImage::FlipHorizontal( void )
{TRACE;
        return false;
}

/*--------------------------------------------------------------------------*/

UInt32
CImage::GetPixelComponentSize( void ) const
{TRACE;

    return _imgdata.vsize;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*--------------------------------------------------------------------------*/




