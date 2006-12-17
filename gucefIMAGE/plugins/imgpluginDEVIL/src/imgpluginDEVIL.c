/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#include <stdlib.h>             /* for memory utils */

#include "imgpluginDEVIL.h"     /* function prototypes for this plugin */

#ifndef __IL_H__
#include "il.h"                 /* main API include for the DevIL library */
#define __IL_H__
#endif /* __IL_H__ ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define IMGPLUGIN_DEVIL_MAYOR_VERSION       1
#define IMGPLUGIN_DEVIL_MINOR_VERSION       0
#define IMGPLUGIN_DEVIL_PATCH_VERSION       0
#define IMGPLUGIN_DEVIL_RELEASE_VERSION     1

/*---------------------------------------------------------------------------*/

static TIOAccess* currentRecource = NULL;

static const char* supportedTypes[] = {

    #ifdef IL_TGA
    "tga", "vda", "icb", "vst",
    #endif  
    #ifdef IL_JPG
    "jpg", "jpe", "jpeg",
    #endif
    #ifdef IL_PNG
    "png",
    #endif
    #ifdef IL_BMP
    "bmp", "dib",
    #endif
    #ifdef IL_GIF
    "gif",
    #endif
    #ifdef IL_CUT
    "cut",
    #endif
    #ifdef IL_HDR
    "hdr",
    #endif
    #ifdef IL_ICO
    "ico",
    #endif
    #ifdef IL_JNG
    "jng",
    #endif
    #ifdef IL_LIF
    "lif",
    #endif
    #ifdef IL_MDL
    "mdl",
    #endif    
    #ifdef IL_MNG
    "mng", "jng",
    #endif 
    #ifdef IL_PCD
    "pcd",
    #endif
    #ifdef IL_PCX
    "pcx",
    #endif
    #ifdef IL_PIC
    "pic",
    #endif
    #ifdef IL_PIX
    "pix",
    #endif
    #ifdef IL_PNM
    "pbm", "pgm", "pnm", "ppm",
    #endif
    #ifdef IL_PSD
    "psd", "pdd",
    #endif
    #ifdef IL_PSP
    "psp",
    #endif    
    #ifdef IL_PXR
    "pxr",
    #endif
    #ifdef IL_SGI
    "sgi", "bw", "rgb", "rgba",
    #endif
    #ifdef IL_TIF
    "tif", "tiff",
    #endif
    #ifdef IL_WAL
    "wal",
    #endif 
    #ifdef IL_XPM
    "xpm",
    #endif                   
    NULL
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

ILvoid ILAPIENTRY
ilfCloseRProc( ILHANDLE handle )
{
    if ( NULL != currentRecource )
    {
        currentRecource->close( currentRecource );
    }
}

/*---------------------------------------------------------------------------*/

ILboolean ILAPIENTRY 
ilfEofProc( ILHANDLE handle )
{
    if ( NULL != currentRecource )
    {
        return currentRecource->eof( currentRecource ) != 0 ? IL_TRUE : IL_FALSE;
    }
    return IL_TRUE;
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfGetcProc( ILHANDLE handle )
{
    if ( NULL != currentRecource )
    {
        return currentRecource->getc( currentRecource );
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

ILHANDLE ILAPIENTRY 
ilfOpenRProc( const ILstring ilstring )
{
    if ( NULL != currentRecource )
    {
        currentRecource->open( currentRecource );
    }
    
    return currentRecource;
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfReadProc( void* data, 
             ILuint size     , 
             ILuint elements , 
             ILHANDLE handle )
{
    if ( NULL != currentRecource )
    {
        return currentRecource->read( currentRecource, data, size, elements );
    }
    return 0;        
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfSeekRProc( ILHANDLE handle , 
              ILint offset    , 
              ILint origin    )
{
    if ( NULL != currentRecource )
    {
        return currentRecource->seek( currentRecource, offset, origin );
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfTellRProc( ILHANDLE handle )
{
    return currentRecource->tell( currentRecource );
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Init( void** plugdata    , 
                  const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    if ( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION ) return 0;    
    
    ilInit();
    
    ilSetRead( ilfOpenRProc  , 
               ilfCloseRProc , 
               ilfEofProc    , 
               ilfGetcProc   , 
               ilfReadProc   , 
               ilfSeekRProc  , 
               ilfTellRProc  );
               
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    ilSetRead( NULL , 
               NULL , 
               NULL , 
               NULL , 
               NULL , 
               NULL , 
               NULL );
               
    ilShutDown();
    
    return 1;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Name( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "imgpluginDEVIL";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.\0";
}

/*---------------------------------------------------------------------------*/

const TVersion* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    static TVersion version;
    
    version.mayor = IMGPLUGIN_DEVIL_MAYOR_VERSION;
    version.minor = IMGPLUGIN_DEVIL_MINOR_VERSION;
    version.patch = IMGPLUGIN_DEVIL_PATCH_VERSION;
    version.release = IMGPLUGIN_DEVIL_RELEASE_VERSION;
    
    return &version;
}

/*---------------------------------------------------------------------------*/

Int32
ConvertILPixelFormatToGUCEFPixelFormat( ILint devilType )
{
    switch ( devilType )
    {
        case IL_RGB : return PSF_RGB;
        case IL_RGBA : return PSF_RGBA;
        case IL_BGR : return PSF_BGR;
        case IL_BGRA : return PSF_BGRA;
        case IL_LUMINANCE : return PSF_SINGLE_CHANNEL_LUMINANCE;
        case IL_LUMINANCE_ALPHA : return PSF_SINGLE_CHANNEL_ALPHA;
        default: return -1;
    }
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_LoadImage( void* plugdata           ,
                       void** pluginImageData   ,
                       const char* imageType    ,
                       TIOAccess* sourceData    ,  
                       TImage** outputImageData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    int i=0, n=0, mipmapCount=0, frameCount=0;
    ILint imageID = 0, imageSize = 0;
    TImage* imageData = NULL;
    TImageFrame* imageFrame = NULL;
    TImageMipMapLevel* imageMMInfo = NULL;
    
    if ( ( NULL == pluginImageData ) || ( NULL == imageType ) || ( NULL == sourceData ) || ( NULL == outputImageData ) ) return 0;
    
    /* generate an image ID and make that ID the ID of the current image */
    imageID = ilGenImage();
    ilBindImage( imageID );
    
    currentRecource = sourceData;
    
    if ( IL_TRUE == ilLoadF( ilTypeFromExt( (const wchar_t*) imageType ), sourceData ) )
    {
        /* this plugin does not need to store additional data per image */
        *pluginImageData = NULL;
        
        /* Create the image structure hierarchy */
        imageData = (TImage*) malloc( sizeof( TImage ) );
        imageData->version = GUCEF_IMAGE_TIMAGE_VERSION;
        imageData->imageInfo.version = GUCEF_IMAGE_TIMAGEINFO_VERSION;
        frameCount = ilGetInteger( IL_NUM_IMAGES );
        imageData->imageInfo.nrOfFramesInImage = frameCount;
        imageData->frames = (TImageFrame*) malloc( frameCount * sizeof(TImageFrame) );
        
        /* Only 1 layer is supported atm */
        ilActiveLayer( 0 );
        
        for ( i=0; i<frameCount; ++i )
        {
            /* activate the frame */
            ilActiveImage( i );
            
            /* Add all information for this image frame */
            imageFrame = &imageData->frames[ i ];
            imageFrame->version = GUCEF_IMAGE_TIMAGEFRAME_VERSION;
            imageFrame->frameInfo.version = GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION;
            mipmapCount = ilGetInteger( IL_NUM_MIPMAPS );
            imageFrame->frameInfo.nrOfMipmapLevels = mipmapCount;
            imageFrame->mipmapLevel = (TImageMipMapLevel*) malloc( mipmapCount * sizeof( TImageMipMapLevel ) );
        
            for ( n=0; n<mipmapCount; ++n )
            {
                /* activate the mip-map */
                ilActiveMipmap( n );
                ilCompressFunc( IL_COMPRESS_NONE );
                
                /* Add all information for this image frame's mip-map level */
                imageMMInfo = &imageFrame->mipmapLevel[ n ];
                imageMMInfo->version = GUCEF_IMAGE_TIMAGEMIPMAPLEVEL_VERSION;
                imageMMInfo->mipLevelInfo.version = GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION;
                imageMMInfo->mipLevelInfo.channelComponentSize = 8; /* DevIL only supports UInt8 */
                imageMMInfo->mipLevelInfo.channelCountPerPixel = ilGetInteger( IL_IMAGE_BPP ) / 8;
                imageMMInfo->mipLevelInfo.frameHeight = ilGetInteger( IL_IMAGE_HEIGHT );
                imageMMInfo->mipLevelInfo.frameWidth = ilGetInteger( IL_IMAGE_WIDTH );
                imageMMInfo->mipLevelInfo.pixelComponentDataType = DT_UINT8; /* DevIL only supports this type */
                imageMMInfo->mipLevelInfo.pixelStorageFormat = ConvertILPixelFormatToGUCEFPixelFormat( ilGetInteger( IL_IMAGE_FORMAT ) );
                
                /* copy the pixel data */
                imageSize = ilGetInteger( IL_IMAGE_SIZE_OF_DATA );
                imageMMInfo->pixelData = malloc( imageSize );
                memcpy( imageMMInfo->pixelData, ilGetData(), imageSize );
            }
        }
    }
    
    /*
     *  the image has been loaded into our GUCEF structures so we have no need for
     *  DevIL to store the data any more 
     */
    ilDeleteImage( imageID );
    
    return 1;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_DetectImageType( void* plugdata        ,
                             TIOAccess* sourceData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return NULL;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_DeleteLoadedImage( void* plugdata         ,
                               void* pluginImageData  ,
                               TImage* imageData      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    int i=0, n=0, mipmapCount=0, frameCount=0;
    TImageFrame* imageFrame = NULL;
    TImageMipMapLevel* imageMipmap = NULL;
        
    if ( NULL != imageData )
    {       
        /* Iterate the image structure hierarchy, cleaning up as we go */
        frameCount = imageData->imageInfo.nrOfFramesInImage;                
        for ( i=0; i<frameCount; ++i )
        {
            imageFrame = &imageData->frames[ i ];
            mipmapCount = imageFrame->frameInfo.nrOfMipmapLevels;        
            for ( n=0; n<mipmapCount; ++n )
            {
                /* delete the pixel data */
                imageMipmap = &imageFrame->mipmapLevel[ n ];                
                free( imageMipmap->pixelData );                                
            }
            
            /* delete all mip-map structures for this frame */
            free( imageFrame->mipmapLevel );
        }
        
        /* delete all frame structures */
        free( imageData->frames );     
        
        return 1;
    }
    
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_SaveImage( void* plugdata           , 
                       const char* imageType    ,
                       TImage* inputImageData   ,
                       TIOAccess* outputMedia   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_GetFormatList( void* plugdata                 , 
                           const char*** supportedFormats ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    *supportedFormats = supportedTypes;
    return 1;
}

/*---------------------------------------------------------------------------*/