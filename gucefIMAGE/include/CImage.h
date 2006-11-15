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

#ifndef CIMAGE_H
#define CIMAGE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCORE_H
#include "gucefCORE.h"               /* gucef libary CORE module */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"       /* build switches */
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

#ifndef IMAGEDATA_H
#include "imagedata.h"               /* image data storage */
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

/**
 *      Capable of loading different formats of images and storing there data
 *      in a protected manner. Designed to have a minimal memory footprint when
 *      loading and when storing image data. Also contains a few save to file
 *      functions.
 *      This class is not thread-safe.
 */
class EXPORT_CPP CImage
{
        public :

        /**
         *      Creates an empty image object
         */
        CImage( void );     

        /**
         *      Cleans up any data that may have been allocated/stored
         */
        ~CImage();

        /**
         *      Returns a refrence to the entire image data structure.
         *      Note that it is a better idea to use the member functions of
         *      this class to obtain the field values since they are less
         *      likely to change.
         *
         *      @return the image storage structure
         */
        const TImageData& GetImageDataStructure( void ) const;

        /**
         *      Get Bits Per Pixel
         *      Typical values are 8, 24, 32
         *
         *      @return the depth a.k.a bbp of the image data.
         */
        UInt8   GetDepth( void ) const;

        UInt32  GetWidth( void ) const;
        UInt32  GetStride( void ) const;
        UInt32  GetHeight( void ) const;
        UInt8   GetChannels( void ) const;
        Int8    GetFormat( void ) const;
        UInt32  GetFrames( void ) const;
        
        UInt32 GetTotalPixelStorageSize( void ) const;
        TBuildinDataType GetPixelComponentDataType( void ) const;

        /**
         *      Get the number of mipmap levels per frame
         *
         *      @return mipmap levels per frame.
         */
        UInt8 GetMipmapLevels( void ) const;

        /**
         *      Get the frame size in bytes of a single frame at the highest
         *      mipmap level.
         *
         *      @return size of the frame data in bytes 
         */
        UInt32 GetFrameSize( void ) const;

        /**
         *      Get a pointer to the pixel data of the first frame.
         *      The pixel data is that of the highest mipmap level.
         *
         *      @return         pixel data of the requested frame.
         */
        const void* GetData( void ) const;

        /**
         *      Get a pointer to the pixel data of the frame given.
         *      The pixel data is that of the highest mipmap level.
         *
         *      @param frame    frame index for which you want the pixel data
         *      @return         pixel data of the requested frame.
         */
        const void* GetData( UInt32 frame ) const;

        /**
         *      Get a pointer to the pixel data of the frame given.
         *      The pixel data is that of the highest mipmap level.
         *
         *      @param frame      frame index for which you want the pixel data
         *      @param mipmapidx  index of the mipmap level.
         *      @return           pixel data of the requested frame.
         */
        const void* GetData( UInt32 frame    ,
                             UInt8 mipmapidx ) const;

        /**
         *      Get the type of value that is used for each pixel component.
         *
         *      @return The type of each pixel component value.
         */
        Int8 GetPixelComponentType( void ) const;

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
         *      Use mask color given to set the aplha value of each pixel to 0
         *      when the color matches the given values.
         *
         *      @param r        value of the red channel to match
         *      @param g        value of the green channel to match
         *      @param b        value of the blue channel to match
         */
        void ApplyMaskColor( Float32 r ,
                             Float32 g ,
                             Float32 b );

        /**
         *      Attempts to load image data using the plugin identified with
         *      the provided handle. The plugin will attempt to load the image
         *      data from the recource provided by 'access' and store the
         *      result in 'imgdata'.
         */
        bool Load( UInt32 codecidx         ,
                   UInt32 hidx             ,
                   CORE::CIOAccess& access );

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
        bool Save( UInt32 codecidx               ,
                   UInt32 hidx                   ,
                   const CORE::CString& filename ,
                   UInt32 format                 ,
                   UInt32 compression            );

        /**
         *      Unloads image data if needed and resets values to there defaults
         */
        void Clear( void );

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

        private :
        CImage( const CImage& src ); /* @TODO: not implemented yet */
        CImage& operator=( const CImage& src ); /* @TODO: not implemented yet */

        TImageData _imgdata;
        UInt32 _codecused;
        UInt32 _hidxused;
        bool _codecisowner;
        void *_codecdata;        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* CIMAGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Updated class coding style to the latest standerd:
            - Got rid of inline implementations.
            - Got rid of underscore in member function identifiers.
            - Added Get/Set prefixes where appropriote.
        - Now uses the new codec system instead of the CImageLoaderPlugin class.
- 19-07-2004 :
        - Disabled the copy member functions. They need to be updated. 
- 03-02-2004 :
        - Reconfigured the class and changed the member functions to
          accomodate the new plugin implementation. All actual loading and/or
          saving and checking for the ability's to do so will no longer be
          implemented in this class. The CImage class will now simply provide
          an interface to the plugin utility's plus provide you with an instance
          of the data stored in a CImage object. This new setup will allow a lot
          more flexible image loading and/or saving capability.
        - Implemented support for implementaion independant I/O access in the
          form of the ioaccess functions.
- 14-12-2003 :
        - Implemented the DIB BMP loader.
          Only uncompressed dib bmp files are supported
        - Added support for frames. A frame is image data just like before
          but more then one image can be stored by a CImage instance. The
          images are stored sequentially. 
- 13-12-2003 :
        - Implemented the GIF loader, now using libungif to avoid legal issues.
- 21-08-2003 :
        - Removed circular friend refrence which is not allowed by mingw.
- 26-07-2003 :
        - Added a loader for WIN32 cursor files. This is mainly intended to make
          reuse of resources more convenient. With this loader you can load a
          cursor and then make a texture with it for use with OpenGL.
          Colored cursors are not supported and the cursor's dimensions must be
          a power of 2. Multiple cursor images in a single cursor files are also
          not supported in which case only the first image gets loaded 
- 23-07-2003 :
        - Implemented the TGA save member function. Saving will not work if the
          the image contained is a single channel. Otherwise either BGR or BGRA
          will be saved depending on the precense of an alpha channel. Bit depth
          is 24 or 32. No compression is applyed.
- 08-07-2003 :
        - Modifyed TGA loaders:
        - Added a TGA id field skip so that the precense of an id field does not
          pose a problem.
        - Added support for 16 bit TGA images. This means we now support 16, 24
          and 32 bit TGA images. Yay :)
        - TGA support is limited to the following types: 2, 3, 10
          This means colormaps are not supported.
- 02-06-2003 :
        - Added rounding of width and height in Read_From_glScreen so that
          we have nice byte allignment.
        - Made the different specific loaders private, the user can use any of
          the generic loader functions.
        - Added Clear() function to unload an image and reset some vars. It is
          automaticly called when you load an image thus unloading any previous
          image that may have been loaded.  
- 31-05-2003 :
        - Fixed a bug in the PCX loader where it would get stuck in a loop
          if the data was RLE packed.
- 30-05-2003 :
        - Changed interface a bit by removing the Get_ prefix from member
          function names.
        - Added the glData_Type() member function so it's easyer to use the
          pixmaps with OpenGL when you have the converted OpenGL element size.
        - Added 3 Copy member functions allowing you to copy pixmap sections. 
- 24-03-2003 :
        - Added the ability to read the screen data from the current OpenGL
          context with Read_From_glScreen().
- 23-03-2003 :
        - Added support for a custom RAW format, this format is meant for fast
          loading, so in the final product you might want to use this format
          for run-time image loading.
        - Implemented the TGA reading support
        - Added the ability to change the storage format using Force_Value_Type()
          ie changing the size per value. This does not take into account any
          overflow problems that may arize. This ability is needed to comply
          with image format specifications when saving.
- 21-03-2003 :
        - Added #ifdef IMAGE_SUPPORT_xxx build swithes where applicable. This
          allows the enduser to remove support for image formats that arent used
          the modified image load libs are also edited with these swiches to
          assure the removal of all code even if they are still included in the
          project. 
- 08-02-2003 :
        - Added the ability to use a mask color to set alpha value's.
          If the mask color matches a pixel color the pixel will be invisible.
          if no alpha channel is present it will be added.
        - Added generic loaders that use the filename extension to determine
          what loader to use.
- 03-02-2003 :
        - Added some Alpha channel capabilities. not all image formats have
          alpha channels so it's usefull to have the capability to add one.
          Force_Alpha() is used uppon loading and setting force alpha.
          Therefore Has_Alpha() always returns the presense of an alpha channel
          depending on the current image data.
- 24-01-2003 :
        - Added Vertical_Flip() to flip images if so desired.
          Comes in handy because OpenGL uses the image data upside down.
- 21-01-2003 :
        - Implemented Data dump of image data functionality.
          This gives you the option to view each value of the image in a
          textfile that has been written in binary mode.
        - Changed PCX loader. now uses file version to check for 256 palette
          if version is 5 then it's assumed a 256 color palette exists.
        - Removed a lame ass division that mucked up the color table in PCX
        - PCX loader now stores data in RGB with unsigned byte elements
- 20-01-2003 :
        - Implemented the PPM loader.
          Only version P6 is supported. P6 is a lot faster then P3 but only
          supports a maximum of 256 color values.
          It is assumed there is only 1 image per PPM file.
          The RGB data is converted to RGBA, all alpha values will be set to 1.0
          Comments in the PPM file are not supported.
- 19-01-2003 :
        - Designed CImage class :
          Both loading from disk and loading from a file in memory is
          supported.
        - Implemented the BMP loader :
          Reads Win32 bitmap's. There is support for 8, 24 or 32 bit
          images.      
        - Implemented the PCX loader :
          there is PCX support for 16 and 256 color images.
          The pixel data is stored in RGBA format,
          all alpha values will be set to 1.0
          only single plane images are supported.


-----------------------------------------------------------------------------*/



