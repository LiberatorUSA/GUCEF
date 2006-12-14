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

#ifndef __IL_H__
#include "il.h"                 /* main API include for the DevIL library */
#define __IL_H__
#endif /* __IL_H__ ? */

#include "imgpluginDEVIL.h"     /* function prototypes for this plugin */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

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
  //      currentRecource->close();
    }
}

/*---------------------------------------------------------------------------*/

ILboolean ILAPIENTRY 
ilfEofProc( ILHANDLE handle )
{
    if ( NULL != currentRecource )
    {
    //    currentRecource->close();
    }
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfGetcProc( ILHANDLE handle )
{
}

/*---------------------------------------------------------------------------*/

ILHANDLE ILAPIENTRY 
ilfOpenRProc( const ILstring ilstring )
{
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfReadProc( void* data, 
             ILuint a, 
             ILuint  b    , 
             ILHANDLE handle )
{
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfSeekRProc( ILHANDLE handle , 
              ILint a, 
              ILint  b         )
{
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfTellRProc( ILHANDLE handle )
{
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Init( void** plugdata    , 
                  const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    if ( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION ) return 0;    
    
    if ( ilInit() == IL_FALSE ) return 0;
    
    ilSetRead( ilfOpenRProc  , 
               ilfCloseRProc , 
               ilfEofProc    , 
               ilfGetcProc   , 
               ilfReadProc   , 
               ilfSeekRProc  , 
               ilfTellRProc  );
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
    //TVersion version;
    
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
    ILint imageID = 0;
    TImage* imageData = NULL;
    TImageFrame* imageFrame = NULL;
    TImageMipMapLevel* imageMMInfo = NULL;
    
    if ( ( NULL == pluginImageData ) || ( NULL == imageType ) || ( NULL == sourceData ) || ( NULL == outputImageData ) ) return 0;
    
    /* generate an image ID and make that ID the ID of the current image */
    imageID = ilGenImage();
    ilBindImage( imageID );
    
    if ( IL_TRUE == ilLoadF( ilTypeFromExt( imageType ), sourceData ) )
    {
        /* this plugin does not need to store additional data per image */
        *pluginImageData = NULL;
        
        /* Create the image structure hierarchy */
        imageData = (TImage*) malloc( sizeof( TImage ) );
        imageData->version = GUCEF_IMAGE_TIMAGE_VERSION;
        imageData->imageInfo.version = GUCEF_IMAGE_TIMAGEINFO_VERSION;
        frameCount = ilGetInteger( IL_NUM_IMAGES );
        imageData->imageInfo.nrOfFramesInImage = frameCount;
        imageData->frames = (TImageFrame*) malloc( x * sizeof(TImageFrame) );
        
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
                /* activate the mipmap */
                ilActiveMipmap( n );
                
                /* Add all information for this image frame's mip-map level */
                imageMMInfo = &imageFrame->mipmapLevel[ n ];
                imageMMInfo->version = GUCEF_IMAGE_TIMAGEMIPMAPLEVEL_VERSION;
                imageMMInfo->mipLevelInfo.version = GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION;
                imageMMInfo->pixelData = ilGetData();
                imageMMInfo->mipLevelInfo.channelComponentSize = ilGetInteger( IL_IMAGE_BPP );
                imageMMInfo->mipLevelInfo.channelCountPerPixel;
                imageMMInfo->mipLevelInfo.frameHeight = ilGetInteger( IL_IMAGE_HEIGHT );
                imageMMInfo->mipLevelInfo.frameWidth = ilGetInteger( IL_IMAGE_WIDTH );
                imageMMInfo->mipLevelInfo.pixelComponentDataType = DT_UINT8; /* DevIL only supports this type */
                imageMMInfo->mipLevelInfo.pixelStorageFormat                
            }
        }
    }
    
    /*
     *  the image has been loaded into our GUCEF structures so we have no need for
     *  DevIL to store the data any more 
     */
    ilDeleteImage( imageID );
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
    TImage* imageData = NULL;
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
            
            /* delete all mipmap structures for this frame */
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
IMAGEPLUGIN_SaveImage( void* plugdata         , 
                       TImage* inputImageData ,
                       TIOAccess* outputMedia ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
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