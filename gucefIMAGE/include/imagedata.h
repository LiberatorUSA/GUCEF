/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef IMAGEDATA_H
#define IMAGEDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_IMAGE_ETYPES_H
#include "gucefIMAGE_ETypes.h"      /* simple types used */
#define GUCEF_IMAGE_ETYPES_H
#endif /* GUCEF_IMAGE_ETYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace IMAGE {
#endif /* __cplusplus ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      The following are a number of explicit image storage format types.
 */
enum
{
        IMGFORMAT_JPEG = 1,                    /* usually has extension jpg */
        IMGFORMAT_PORTABLE_NETWORK_GRAPHICS,   /* usually has extension png */
        IMGFORMAT_TAGGED_IMAGE_FORMAT,         /* usually has extension tif */
        IMGFORMAT_GRAPHICS_INTERCHANGE_FORMAT, /* usually has extension gif */
        IMGFORMAT_PORTABLE_PIXMAP,             /* usually has extension ppm */
        IMGFORMAT_TARGA,                       /* usually has extension tga */
        IMGFORMAT_PC_PAINTBRUSH_EXCHANGE,      /* usually has extension pcx */
        IMGFORMAT_GALAXY_UNLIMITED_IMAGE,      /* usually has extension gui */
        IMGFORMAT_GALAXY_UNLIMITED_BITMAP,     /* usually has extension bm  */        
        IMGFORMAT_MSWINDOWS_BITMAP,            /* usually has extension bmp */
        IMGFORMAT_MSWINDOWS_CURSOR,            /* usually has extension cur */
        IMGFORMAT_MSWINDOWS_ICON,              /* usually has extension ico */
        IMGFORMAT_FS2_ANI,                     /* usually has extension ani */
        IMGFORMAT_DIRECTDRAW_SURFACE,          /* usually has extension dds */
        IMGFORMAT_OPENGL_CONTEXT,              /* for loading only, not a file format */ 
        IMGFORMAT_DONT_CARE = 10000            /* who cares */
};

/*-------------------------------------------------------------------------*/

/**
 *      Possible color data component types
 */
enum
{
        DT_FLOAT32 = 1 ,
        DT_FLOAT64     ,
        DT_UINT8       ,
        DT_INT8        ,
        DT_UINT16      ,
        DT_INT16       ,
        DT_UINT32      ,
        DT_INT32       ,
        DT_UINT64      ,
        DT_INT64
};

/*-------------------------------------------------------------------------*/

/**
 *      Image Storage formats
 */
enum
{
        ISF_RGB = 1                  ,
        ISF_RGBA                     ,
        ISF_SINGLE_CHANNEL_RED       ,
        ISF_SINGLE_CHANNEL_GREEN     ,
        ISF_SINGLE_CHANNEL_BLUE      ,
        ISF_SINGLE_CHANNEL_ALPHA     ,
        ISF_SINGLE_CHANNEL_LUMINANCE
};

/*-------------------------------------------------------------------------*/

/**
 *      Image compression formats
 */
enum
{
        ICF_NO_COMPRESSION = 0   ,
        ICF_GL_COMPRESSED        ,
        ICF_S3TC_COMPRESSED_DXT1 ,
        ICF_S3TC_COMPRESSED_DXT3 ,
        ICF_S3TC_COMPRESSED_DXT5 
};

/*-------------------------------------------------------------------------*/

/**
 *      Storage structure for image data.
 *      Some data members can be derived from others but for the sake of speed
 *      the derivative is stored aswell.
 */
struct SImageData
{
        Int8 dtype;         /* data type of each value of idata */
        UInt32 vsize;       /* size of each data value */
        UInt8 channels;     /* number of channels per pixel */
        UInt8 has_alpha;    /* wheter the image has an alpha channel */
        UInt32 width;       /* Width of image. */
        UInt32 height;      /* Height of image. */
        UInt8 bbp;          /* BPP, Bits Per Pixel */
        UInt32 stride;      /* Number of bytes in each horizontal scan line. used for flipping vertically */
        UInt32 dsize;       /* Allocated pixel data space used by image. */
        Int8 format;        /* format in which the pixel components are stored */
        UInt32 frames;      /* total number of images stored */
        UInt8 mipmaps;      /* number of mipmaps per frame */
        Int8 icf;           /* image compression format */
        void *idata;        /* pointer to the pixel data. */
};

typedef struct SImageData TImageData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace IMAGE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#endif /* IMAGEDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-08-2004 :
        - Added the mipmaps field.
- 25-08-2004 :
        - Added support for image compression: 
          An OpenGL compressed pixmap and an S3TC compressed pixmap.
        - ISF_SINGLE_CHANNEL has been split into several formats so that the
          channel type can be preserved.
        - Got rid of the typedefs on the enums. typedefed enum fields turned
          into byte.
- 01-03-2004 :
        - Seperated the storage attributes of the CImage class into a seperate
          storage structure so that it may be used elsewhere aswell.

-----------------------------------------------------------------------------*/
