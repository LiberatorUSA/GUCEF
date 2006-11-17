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

#include <vector>

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_IMAGE_CPIXELMAP_H
#include "CPixelMap.h"
#define GUCEF_IMAGE_CPIXELMAP_H
#endif /* GUCEF_IMAGE_CPIXELMAP_H ? */

#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"       /* module macro's */
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

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
 *  Image class that defines an image as a series of frames with each frame beeing
 *  capable of having multiple mipmapping levels. Pixel maps can be shared between
 *  image objects if desired.
 */
class EXPORT_CPP CImage
{
    public :

    typedef CORE::CTSharedPtr< CImage >    TImagePtr;
    typedef CORE::CTSharedPtr< CPixelMap > TPixelMapPtr;
    typedef std::vector< TPixelMapPtr >    TMipMapList;
    typedef std::vector< TMipMapList >     TFrameList;
        
    public:
    
    CImage( const TFrameList& frameList );
    
    CImage( const TMipMapList& mipmapList );
    
    explicit CImage( const TPixelMapPtr& pixelMapPtr );

    /**
     *      Creates an empty image object
     */
    CImage( void );     

    ~CImage();

    void Assign( const TFrameList& frameList );
    
    void Assign( const TMipMapList& mipmapList );
    
    void Assign( const TPixelMapPtr& pixelMapPtr );
    
    UInt32 GetFrameCount( void ) const;

    UInt32 GetTotalPixelStorageSize( void ) const;

    /**
     *  Get the number of mipmap levels for the given frame
     *
     *  @return mipmap levels per frame.
     *  @throw EInvalidIndex is thrown if an invalid index is given
     */
    UInt32 GetMipmapLevels( const UInt32 frameIndex = 0 ) const;
        
    /**
     *  @throw EInvalidIndex is thrown if an invalid index is given
     */
    TPixelMapPtr GetFrame( const UInt32 frameIndex = 0  ,
                           const UInt32 mipMapLevel = 0 );

    /**
     *      Unloads image data if needed and resets values to there defaults
     */
    void Clear( void );
    
    GUCEF_DEFINE_MSGEXCEPTION( EXPORT_CPP, EInvalidIndex );

    private :
    CImage( const CImage& src ); /* @TODO: not implemented yet */
    CImage& operator=( const CImage& src ); /* @TODO: not implemented yet */

    private:        

    TFrameList m_frameList;
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

- 16-11-2006 :
        - Dinand: refactored this class:
            - no longer operates as an interface to a plugin with plugin 'features'
            - no longer handles all the data itself but the class is now more of a collection manager,
              managing a collection of PixelMap objects. 
- 27-11-2004 :
        - Dinand: Updated class coding style to the latest standerd:
            - Got rid of inline implementations.
            - Got rid of underscore in member function identifiers.
            - Added Get/Set prefixes where appropriote.
        - Dinand: Now uses the new codec system instead of the CImageLoaderPlugin class.
- 19-07-2004 :
        - Dinand: Disabled the copy member functions. They need to be updated. 
- 03-02-2004 :
        - Dinand: Reconfigured the class and changed the member functions to
          accomodate the new plugin implementation. All actual loading and/or
          saving and checking for the ability's to do so will no longer be
          implemented in this class. The CImage class will now simply provide
          an interface to the plugin utility's plus provide you with an instance
          of the data stored in a CImage object. This new setup will allow a lot
          more flexible image loading and/or saving capability.
        - Dinand: Implemented support for implementaion independant I/O access in the
          form of the ioaccess functions.
- 14-12-2003 :
        - Dinand: Implemented the DIB BMP loader.
          Only uncompressed dib bmp files are supported
        - Dinand: Added support for frames. A frame is image data just like before
          but more then one image can be stored by a CImage instance. The
          images are stored sequentially. 
- 13-12-2003 :
        - Dinand: Implemented the GIF loader, now using libungif to avoid legal issues.
- 21-08-2003 :
        - Dinand: Removed circular friend refrence which is not allowed by mingw.
- 26-07-2003 :
        - Dinand: Added a loader for WIN32 cursor files. This is mainly intended to make
          reuse of resources more convenient. With this loader you can load a
          cursor and then make a texture with it for use with OpenGL.
          Colored cursors are not supported and the cursor's dimensions must be
          a power of 2. Multiple cursor images in a single cursor files are also
          not supported in which case only the first image gets loaded 
- 23-07-2003 :
        - Dinand: Implemented the TGA save member function. Saving will not work if the
          the image contained is a single channel. Otherwise either BGR or BGRA
          will be saved depending on the precense of an alpha channel. Bit depth
          is 24 or 32. No compression is applyed.
- 08-07-2003 :
        - Dinand: Modifyed TGA loaders:
        - Dinand: Added a TGA id field skip so that the precense of an id field does not
          pose a problem.
        - Dinand: Added support for 16 bit TGA images. This means we now support 16, 24
          and 32 bit TGA images. Yay :)
        - Dinand: TGA support is limited to the following types: 2, 3, 10
          This means colormaps are not supported.
- 02-06-2003 :
        - Dinand: Added rounding of width and height in Read_From_glScreen so that
          we have nice byte allignment.
        - Dinand: Made the different specific loaders private, the user can use any of
          the generic loader functions.
        - Dinand: Added Clear() function to unload an image and reset some vars. It is
          automaticly called when you load an image thus unloading any previous
          image that may have been loaded.  
- 31-05-2003 :
        - Dinand: Fixed a bug in the PCX loader where it would get stuck in a loop
          if the data was RLE packed.
- 30-05-2003 :
        - Dinand: Changed interface a bit by removing the Get_ prefix from member
          function names.
        - Dinand: Added the glData_Type() member function so it's easyer to use the
          pixmaps with OpenGL when you have the converted OpenGL element size.
        - Dinand: Added 3 Copy member functions allowing you to copy pixmap sections. 
- 24-03-2003 :
        - Dinand: Added the ability to read the screen data from the current OpenGL
          context with Read_From_glScreen().
- 23-03-2003 :
        - Dinand: Added support for a custom RAW format, this format is meant for fast
          loading, so in the final product you might want to use this format
          for run-time image loading.
        - Dinand: Implemented the TGA reading support
        - Dinand: Added the ability to change the storage format using Force_Value_Type()
          ie changing the size per value. This does not take into account any
          overflow problems that may arize. This ability is needed to comply
          with image format specifications when saving.
- 21-03-2003 :
        - Dinand: Added #ifdef IMAGE_SUPPORT_xxx build swithes where applicable. This
          allows the enduser to remove support for image formats that arent used
          the modified image load libs are also edited with these swiches to
          assure the removal of all code even if they are still included in the
          project. 
- 08-02-2003 :
        - Dinand: Added the ability to use a mask color to set alpha value's.
          If the mask color matches a pixel color the pixel will be invisible.
          if no alpha channel is present it will be added.
        - Dinand: Added generic loaders that use the filename extension to determine
          what loader to use.
- 03-02-2003 :
        - Dinand: Added some Alpha channel capabilities. not all image formats have
          alpha channels so it's usefull to have the capability to add one.
          Force_Alpha() is used uppon loading and setting force alpha.
          Therefore Has_Alpha() always returns the presense of an alpha channel
          depending on the current image data.
- 24-01-2003 :
        - Dinand: Added Vertical_Flip() to flip images if so desired.
          Comes in handy because OpenGL uses the image data upside down.
- 21-01-2003 :
        - Dinand: Implemented Data dump of image data functionality.
          This gives you the option to view each value of the image in a
          textfile that has been written in binary mode.
        - Dinand: Changed PCX loader. now uses file version to check for 256 palette
          if version is 5 then it's assumed a 256 color palette exists.
        - Dinand: Removed a lame ass division that mucked up the color table in PCX
        - Dinand: PCX loader now stores data in RGB with unsigned byte elements
- 20-01-2003 :
        - Dinand: Implemented the PPM loader.
          Only version P6 is supported. P6 is a lot faster then P3 but only
          supports a maximum of 256 color values.
          It is assumed there is only 1 image per PPM file.
          The RGB data is converted to RGBA, all alpha values will be set to 1.0
          Comments in the PPM file are not supported.
- 19-01-2003 :
        - Dinand: Designed CImage class :
          Both loading from disk and loading from a file in memory is
          supported.
        - Dinand: Implemented the BMP loader :
          Reads Win32 bitmap's. There is support for 8, 24 or 32 bit
          images.      
        - Dinand: Implemented the PCX loader :
          there is PCX support for 16 and 256 color images.
          The pixel data is stored in RGBA format,
          all alpha values will be set to 1.0
          only single plane images are supported.


-----------------------------------------------------------------------------*/



