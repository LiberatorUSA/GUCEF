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

#ifndef GUCEF_IMAGE_IMAGEDATA_H
#define GUCEF_IMAGE_IMAGEDATA_H

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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  The following are a number of values that you should always assign to the
 *  matching field of the struct mentioned. This will allow checking of the
 *  linked type definition versus the runtime passed object
 */
#define GUCEF_IMAGE_TIMAGE_VERSION                  1
#define GUCEF_IMAGE_TIMAGEINFO_VERSION              1
#define GUCEF_IMAGE_TIMAGEFRAME_VERSION             1
#define GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION         1
#define GUCEF_IMAGE_TIMAGEMIPMAPLEVEL_VERSION       1
#define GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION   1

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Image Storage formats
 */
typedef enum TPixelStorageFormat
{
    PSF_UNKNOWN = 0              ,
    
    PSF_RGB = 1                  ,
    PSF_RGBA                     ,
    PSF_BGR                      ,
    PSF_BGRA                     ,
    PSF_SINGLE_CHANNEL_RED       ,
    PSF_SINGLE_CHANNEL_GREEN     ,
    PSF_SINGLE_CHANNEL_BLUE      ,
    PSF_SINGLE_CHANNEL_ALPHA     ,
    PSF_SINGLE_CHANNEL_LUMINANCE ,
    
    PSF_COUNT
};

/*-------------------------------------------------------------------------*/

/**
 *      structure for image information
 */
struct SImageInfo
{
    UInt32 version;                 /**< version of the TImageInfo definition */
    UInt32 nrOfFramesInImage;       /**< total number of frames in the image */
};

typedef struct SImageInfo TImageInfo;

/*-------------------------------------------------------------------------*/

/**
 *      structure for image frame information
 */
struct SImageFrameInfo
{
    UInt32 version;                 /**< version of the TImageInfo definition */
    UInt32 nrOfMipmapLevels;        /**< total number of mimmap levels in the image */
};

typedef struct SImageFrameInfo TImageFrameInfo;

/*-------------------------------------------------------------------------*/

/**
 *      structure for image frame mipmap level information
 */
struct SImageMipMapLevelInfo
{
    UInt32 version;                    /**< version of the TImageInfo definition */
    Int32 pixelComponentDataType;      /**< data type of each value of idata */
    UInt32 channelComponentSize;       /**< size of the data value of the pixel's color channel in bits */
    UInt32 channelCountPerPixel;       /**< number of color component channels per pixel */
    UInt32 frameWidth;                 /**< Width of an image frame in pixels, mipmap level 0 */
    UInt32 frameHeight;                /**< Height of an image frame in pixels, mipmap level 0 */
    Int32 pixelStorageFormat;          /**< format in which the pixel components are stored */
};

typedef struct SImageMipMapLevelInfo TImageMipMapLevelInfo;

/*-------------------------------------------------------------------------*/

/**
 *      Storage structure for an image frame's mipmap level
 */
struct SImageMipMapLevel
{
    UInt32 version;                     /**< version of the TImageInfo definition */
    TImageMipMapLevelInfo mipLevelInfo; /**< information about the mipmap level and it's data */
    void* pixelData;                    /**< array with the actual pixel data */
};

typedef struct SImageMipMapLevel TImageMipMapLevel;

/*-------------------------------------------------------------------------*/

/**
 *      Storage structure for an image frame
 */
struct SImageFrame
{
    UInt32 version;                  /**< version of the TImageFrame definition */
    TImageFrameInfo frameInfo;       /**< information about the frame */
    TImageMipMapLevel* mipmapLevel;  /**< array with mipmap level info + data */
};

typedef struct SImageFrame TImageFrame;

/*-------------------------------------------------------------------------*/

/**
 *      Storage structure for an image
 */
struct SImage
{
    UInt32 version;             /**< version of the TImage definition */
    TImageInfo imageInfo;       /**< basic image information */ 
    TImageFrame* frames;        /**< array of image frames */
};

typedef struct SImage TImage;

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

#endif /* GUCEF_IMAGE_IMAGEDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-11-2006 :
        - Dinand: refactored the type definitions to allow for a more flexible
          linking of image information
        - Dinand: removed support in the definition for compression, it simply does not
          belong here at this technical level.       
- 29-08-2004 :
        - Dinand: Added the mipmaps field.
- 25-08-2004 :
        - Dinand: Added support for image compression: 
          An OpenGL compressed pixmap and an S3TC compressed pixmap.
        - Dinand: ISF_SINGLE_CHANNEL has been split into several formats so that the
          channel type can be preserved.
        - Dinand: Got rid of the typedefs on the enums. typedefed enum fields turned
          into byte.
- 01-03-2004 :
        - Dinand: Seperated the storage attributes of the CImage class into a seperate
          storage structure so that it may be used elsewhere aswell.

-----------------------------------------------------------------------------*/
