/*
 *  imgpluginFreeImage: GUCEF image codec plugin using FreeImage as the backend
 *  Copyright (C) 2002 - 2012.  Dinand Vanvelzen
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>             /* for memory utils */

#include "imgpluginFreeImage.h" /* function prototypes for this plugin */

#ifndef FREEIMAGE_H
#include "FreeImage.h"          /* main API include for the FreeImage library */
#define FREEIMAGE_H
#endif /* FREEIMAGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* make sure no structure packing is enabled, this allows us to use the structures directly */
#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif /* WIN32 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SFreeImageDataLink
{
    void* pixelData;
    FIBITMAP* fiData;
};
typedef struct SFreeImageDataLink TFreeImageDataLink;

/*---------------------------------------------------------------------------*/

struct SFreeImageDataLinks
{
    UInt32 linkCount;
    TFreeImageDataLink* links;
};
typedef struct SFreeImageDataLinks TFreeImageDataLinks;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define IMGPLUGIN_FREEIMAGE_MAYOR_VERSION       0
#define IMGPLUGIN_FREEIMAGE_MINOR_VERSION       1
#define IMGPLUGIN_FREEIMAGE_PATCH_VERSION       0
#define IMGPLUGIN_FREEIMAGE_RELEASE_VERSION     0

/*---------------------------------------------------------------------------*/

static FreeImageIO io;

static char* supportedTypes[] = {
    "bmp"    ,
    "ico"    ,
    "jpeg"   ,
    "jpg"    ,
    "jng"    ,
    "koala"  ,
    "lbm"    ,
    "iff"    ,
    "mng"    ,
    "pbm"    ,
    "pbmraw" ,
    "pcd"    ,
    "pcx"    ,
    "pgm"    ,
    "pgmraw" ,
    "png"    ,
    "ppm"    ,
    "ppmraw" ,
    "ras"    ,
    "tga"    ,
    "targa"  ,
    "tiff"   ,
    "wbmp"   ,
    "psd"    ,
    "cut"    ,
    "xbm"    ,
    "xpm"    ,
    "gif"    ,
    "hdr"    ,
    "faxg3"  ,
    "sgi"
};

static const UInt32 codecCount = sizeof( supportedTypes ) / sizeof(char*); // 31;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

unsigned DLL_CALLCONV
myReadProc( void *buffer     ,
            unsigned size    ,
            unsigned count   ,
            fi_handle handle )
{
    TIOAccess* input = (TIOAccess*) handle;
    if ( NULL != input )
    {
        return input->read( input, buffer, size, count );
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

int DLL_CALLCONV
mySeekProc( fi_handle handle ,
            long offset      ,
            int origin       )
{
    TIOAccess* input = (TIOAccess*) handle;
    if ( NULL != input )
    {
        return input->seek( input, offset, origin );
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

long DLL_CALLCONV
myTellProc( fi_handle handle )
{
    TIOAccess* input = (TIOAccess*) handle;
    return input->tell( input );
}

/*---------------------------------------------------------------------------*/

unsigned DLL_CALLCONV
myWriteProc( void *buffer     ,
             unsigned size    ,
             unsigned count   ,
             fi_handle handle )
{
    TIOAccess* output = (TIOAccess*) handle;
    return output->write( output, buffer, size, count );
}

/*---------------------------------------------------------------------------*/

FREE_IMAGE_FORMAT
GetFileTypeFromExt( const char* ext )
{
    if ( ext != NULL )
    {
        if ( strcmp( "tga", ext ) == 0 )
        {
            return FIF_TARGA;
        }
        if ( strcmp( "ppm", ext ) == 0 )
        {
            return FIF_PPM;
        }
    }
    return FIF_UNKNOWN;
}

/*---------------------------------------------------------------------------*/

int
ConvertToPCDT( const FREE_IMAGE_TYPE imageType )
{
    switch( imageType )
    {
        case FIT_BITMAP :
        {
            return DATATYPE_UINT8;
        }
        case FIT_COMPLEX :
        case FIT_UNKNOWN :
        {
            return DATATYPE_UNKNOWN;
        }
        case FIT_UINT32 :
        {
            return DATATYPE_UINT32;
        }
        case FIT_INT32 :
        {
            return DATATYPE_INT32;
        }
        case FIT_FLOAT :
        {
            return DATATYPE_FLOAT32;
        }
        case FIT_DOUBLE :
        {
            return DATATYPE_FLOAT64;
        }
        case FIT_UINT16 :
        {
            return DATATYPE_UINT16;
        }
        case FIT_INT16 :
        {
            return DATATYPE_INT16;
        }
        case FIT_RGBF :
        {
            return DATATYPE_FLOAT32;
        }
        case FIT_RGB16 :
        {
            return DATATYPE_UINT16;
        }
        case FIT_RGBAF :
        {
            return DATATYPE_FLOAT32;
        }
        case FIT_RGBA16 :
        {
            return DATATYPE_UINT16;
        }
        default :
        {
            return DATATYPE_UNKNOWN;
        }
    }
}

/*---------------------------------------------------------------------------*/

int
ConvertToPSF( const FREE_IMAGE_TYPE imageType       ,
              const FREE_IMAGE_COLOR_TYPE colorType )
{
    switch( imageType )
    {
        case FIT_BITMAP :
        {
            switch ( colorType )
            {
                case FIC_RGB :
                {
                    return PSF_RGB;
                }
                case FIC_RGBALPHA :
                {
                    return PSF_RGBA;
                }
                default :
                {
                    return PSF_UNKNOWN;
                }
            }
        }
        case FIT_UNKNOWN :
        {
            return PSF_UNKNOWN;
        }
        case FIT_COMPLEX :
        case FIT_UINT32 :
        case FIT_INT32 :
        case FIT_FLOAT :
        case FIT_DOUBLE :
        case FIT_UINT16 :
        case FIT_INT16 :
        {
            return PSF_SINGLE_CHANNEL;
        }
        case FIT_RGBF :
        case FIT_RGB16 :
        {
            return PSF_RGB;
        }
        case FIT_RGBAF :
        case FIT_RGBA16 :
        {
            return PSF_RGBA;
        }
        default :
        {
            return PSF_UNKNOWN;
        }
    }
}

/*---------------------------------------------------------------------------*/

UInt32
GetChannelCountForFormat( const int pixelStorageFormat )
{
    switch ( pixelStorageFormat )
    {
        case PSF_BGR :
        case PSF_RGB :
        {
            return 3;
        }
        case PSF_BGRA :
        case PSF_RGBA :
        {
            return 4;
        }
        case PSF_SINGLE_CHANNEL_RED :
        case PSF_SINGLE_CHANNEL_GREEN :
        case PSF_SINGLE_CHANNEL_BLUE :
        case PSF_SINGLE_CHANNEL_LUMINANCE :
        case PSF_SINGLE_CHANNEL_ALPHA :
        {
            return 1;
        }
        default :
        {
            return 0;
        }
    }
}

/*---------------------------------------------------------------------------*/

UInt32
GetPixelChannelSize( const int pixelComponentDataType )
{
    switch ( pixelComponentDataType )
    {
        case DATATYPE_INT8 :
        case DATATYPE_UINT8 :
        {
            return 1;
        }
        case DATATYPE_INT16 :
        case DATATYPE_UINT16 :
        {
            return 2;
        }
        case DATATYPE_INT32 :
        case DATATYPE_UINT32 :
        case DATATYPE_FLOAT32 :
        {
            return 4;
        }
        default :
        {
            return 0;
        }
    }
}

/*---------------------------------------------------------------------------*/

UInt32
GetPixelSize( const int pixelStorageFormat     ,
              const int pixelComponentDataType )
{
    return GetChannelCountForFormat( pixelStorageFormat ) * GetPixelChannelSize( pixelComponentDataType );
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Init( void** plugdata   ,
                  const int argc    ,
                  const char** argv ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
	int fiMayorVersion=0, fiMinorVersion=0, fiReleaseVersion=0;

	/* call this ONLY when linking with FreeImage as a static library */
    #ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
    #endif /* FREEIMAGE_LIB ? */

    /* make sure that the linked back-end library is the same as the one we compiled against */
    sscanf( FreeImage_GetVersion(), "%d %*c %d %*c %d", &fiMayorVersion, &fiMinorVersion, &fiReleaseVersion );
    if ( fiMayorVersion != FREEIMAGE_MAJOR_VERSION   ||
         fiMinorVersion != FREEIMAGE_MINOR_VERSION   ||
         fiReleaseVersion != FREEIMAGE_RELEASE_SERIAL ) return 0;

    io.read_proc  = myReadProc;
    io.write_proc = myWriteProc;
    io.seek_proc  = mySeekProc;
    io.tell_proc  = myTellProc;

    *plugdata = NULL;
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    memset( &io, 0, sizeof( io ) );

    /* call this ONLY when linking with FreeImage as a static library */
    #ifdef FREEIMAGE_LIB
    FreeImage_DeInitialise();
    #endif /* FREEIMAGE_LIB ? */

    return 1;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Description( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "imgpluginFreeImage: image codec collection using FreeImage as a backend\0";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "Copyright (C) Dinand Vanvelzen. 2002 - 2012.  LGPLv3 license. Refer to FreeImage for its license\0";
}

/*---------------------------------------------------------------------------*/

TVersion GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TVersion version;

    version.major = IMGPLUGIN_FREEIMAGE_MAYOR_VERSION;
    version.minor = IMGPLUGIN_FREEIMAGE_MINOR_VERSION;
    version.patch = IMGPLUGIN_FREEIMAGE_PATCH_VERSION;
    version.release = IMGPLUGIN_FREEIMAGE_RELEASE_VERSION;

    return version;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Encode( void* plugdata         ,
                    void* codecData        ,
                    const char* familyType ,
                    const char* codecType  ,
                    TIOAccess* input       ,
                    TIOAccess* output      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    /* Saving images is not supported atm */
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Decode( void* pluginData       ,
                    void* codecData        ,
                    const char* familyType ,
                    const char* codecType  ,
                    TIOAccess* input       ,
                    TIOAccess* output      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TImage* image;
    void* imageData;

    /* input sanity check */
    if ( ( NULL == familyType ) || ( NULL == codecType ) || ( NULL == input ) || ( NULL == output ) ) return 0;
    if ( 0 != strcmp( familyType, "ImageCodec" ) ) return 0;

    /* this plugin only supports image codecs so we can redirect to the specialized image API */
    if ( 0 != IMGCODECPLUGIN_DecodeImage( pluginData ,
                                          codecData  ,
                                          codecType  ,
                                          input      ,
                                          &image     ,
                                          &imageData ) )
    {
        /* now that we have the image data we can write it to the output, we start with the metadata */
        UInt32 i, n;

        output->write( output, &image->imageInfo, sizeof( TImageInfo ), 1  );

        for ( i=0; i<image->imageInfo.nrOfFramesInImage; ++i )
        {
            TImageFrame* frame = &image->frames[ i ];
            output->write( output, &frame->frameInfo, sizeof( TImageFrameInfo ), 1 );

            for ( n=0; n<frame->frameInfo.nrOfMipmapLevels; ++n )
            {
                TImageMipMapLevelInfo* imageMMInfo = &frame->mipmapLevel[ n ].mipLevelInfo;
                output->write( output, imageMMInfo, sizeof( TImageMipMapLevelInfo ), 1 );
            }
        }

        /* now write the pixel data */
        for ( i=0; i<image->imageInfo.nrOfFramesInImage; ++i )
        {
            TImageFrame* frame = &image->frames[ i ];
            for ( n=0; n<frame->frameInfo.nrOfMipmapLevels; ++n )
            {
                TImageMipMapLevel* mipMapLevel = &frame->mipmapLevel[ n ];
                output->write( output, mipMapLevel->pixelData, mipMapLevel->pixelDataSizeInBytes, 1 );
            }
        }

        /* Now that we have written the image to output we can deallocate the access structures */
        IMGCODECPLUGIN_FreeImageStorage( image, imageData );

        return 1;
    }

    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_GetCodecSetBegin( void* plugdata  ,
                              void** iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    *iterator = malloc( sizeof( UInt32 ) );
    *( (UInt32*) *iterator ) = 0;
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_GetCodecLink( void* plugdata               ,
                          void* iterator               ,
                          TCodecPluginLink** codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32 codecIndex = *( (UInt32*) iterator );

    if ( codecIndex < codecCount )
    {
        *codecLink = (TCodecPluginLink*) malloc( sizeof( TCodecPluginLink ) );
        (*codecLink)->codecData = NULL;
        (*codecLink)->codecFamily = "ImageCodec";
        (*codecLink)->codecType = supportedTypes[ codecIndex ];
        (*codecLink)->encode = CODECPLUGIN_Encode;
        (*codecLink)->decode = CODECPLUGIN_Decode;
        return 1;
    }

    /* There is no codec with the given index */
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_FreeCodecLink( void* plugdata              ,
                           TCodecPluginLink* codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    free( codecLink );
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_FreeCodecIterator( void* plugdata ,
                               void* iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32* codecIndex = (UInt32*) iterator;
    free( codecIndex );
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_GetCodecSetNextItem( void* plugdata ,
                                 void* iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32* codecIndex = ( (UInt32*) iterator );
    ++(*codecIndex);

    if ( (*codecIndex) < codecCount )
    {
        return 1;
    }

    /* There are no more codecs */
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMGCODECPLUGIN_FreeImageStorage( TImage* image   ,
                                 void* imageData )
{
    if ( NULL != image )
    {
        TFreeImageDataLinks* allImageDataLinks = (TFreeImageDataLinks*) imageData;

        /* free the image access structures */
        UInt32 i;
        UInt32 frameCount = image->imageInfo.nrOfFramesInImage;
        for ( i=0; i<frameCount; ++i )
        {
            TImageFrame* frame = &image->frames[ i ];
            free( frame->mipmapLevel );
        }
        free( image->frames );
        free( image );

        /* free the actual FreeImage image storage */
        for ( i=0; i<frameCount; ++i )
        {
            TFreeImageDataLink* imageDataLink = &allImageDataLinks->links[ i ];
            FreeImage_Unload( imageDataLink->fiData );
        }
        free( allImageDataLinks );

        return 1;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32
GetNrOfPixelBuffers( TImage* image )
{
    if ( NULL != image )
    {
        UInt32 i, count=0;
        for ( i=0; i<image->imageInfo.nrOfFramesInImage; ++i )
        {
            TImageFrame* frame = &image->frames[ i ];
            count += frame->frameInfo.nrOfMipmapLevels;
        }
        return count;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMGCODECPLUGIN_DecodeImage( void* pluginData      ,
                            void* codecData       ,
                            const char* codecType ,
                            TIOAccess* input      ,
                            TImage** imageOutput  ,
                            void** imageData      )
{
    UInt32 i=0, n=0, mipmapCount=0, frameCount=0;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FREE_IMAGE_TYPE fit = FIT_UNKNOWN;
    FIBITMAP* dib = NULL;
    TFreeImageDataLinks* imageDataLinks = NULL;

    /* input sanity check */
    if ( ( NULL == imageOutput ) || ( NULL == codecType ) || ( NULL == input ) || ( NULL == imageOutput ) ) return 0;

    fif = FreeImage_GetFileTypeFromHandle( &io, (fi_handle) input, 0 );

    if ( fif == FIF_UNKNOWN )
    {
        fif = GetFileTypeFromExt( codecType );
    }

    if( fif != FIF_UNKNOWN )
    {
        dib = FreeImage_LoadFromHandle( fif, &io, (fi_handle) input, 0 );
        if ( dib != NULL )
        {
            /* First we get convert image types we cannot handle */
            TImage* image = NULL;
            UInt32 linkIndex = 0;
            UInt32 bpp = FreeImage_GetBPP( dib );
            FREE_IMAGE_COLOR_TYPE colourType = FreeImage_GetColorType( dib );
            fit = FreeImage_GetImageType( dib );

            if ( FIT_BITMAP == fit )
            {
                if ( bpp < 8 )
                {
				    /* we do not support a format that has less that 8 bits per pixel */
				    FIBITMAP* newDIB = FreeImage_ConvertTo8Bits( dib );
				    FreeImage_Unload( dib );
				    dib = newDIB;
				    colourType = FreeImage_GetColorType( dib );
				    bpp = FreeImage_GetBPP( dib );
                }

			    /* Now that we know the BPP is always >= 8 we can check the color format */
			    if ( ( colourType == FIC_MINISWHITE ) ||
			         ( colourType == FIC_MINISBLACK )  )
			    {
				    /* custom color ranges are not supported */
				    FIBITMAP* newDIB = FreeImage_ConvertToGreyscale( dib );
				    FreeImage_Unload( dib );
				    dib = newDIB;
				    colourType = FreeImage_GetColorType( dib );
				    bpp = FreeImage_GetBPP( dib );
                }
                else
			    if ( ( colourType == FIC_PALETTE ) ||
			         ( colourType == FIC_CMYK )     )
			    {
				    /* palettes are not supported */
				    FIBITMAP* newDIB = FreeImage_ConvertTo24Bits( dib );
				    FreeImage_Unload( dib );
				    dib = newDIB;
				    colourType = FreeImage_GetColorType( dib );
				    bpp = FreeImage_GetBPP( dib );
			    }

			    /* At this point images are always greyscale or RGB or RGBA and BBP is always >= 8 */
            }

            if ( bpp == 32 && colourType == FIC_RGB )
            {
                /* This is 32bpp thus it is actually RGBA but the Alpha layer has no values thus FreeImage says its RGB 
                   This screws up usage, correct here */
                colourType = FIC_RGBALPHA;
            }

            image = (TImage*) malloc( sizeof( TImage ) );

            /* build the TImageInfo section */
            image->version = GUCEF_IMAGE_TIMAGE_VERSION;
            image->imageInfo.version = GUCEF_IMAGE_TIMAGEINFO_VERSION;
            image->imageInfo.nrOfFramesInImage = frameCount = (UInt32) 1; /* @TODO: multiple frames are not supported atm */

            /* Build the TImageFrameInfo sections */
            image->frames = (TImageFrame*) malloc( frameCount * sizeof(TImageFrame) );
            for ( i=0; i<frameCount; ++i )
            {
                TImageFrame* frame = &image->frames[ i ];
                frame->version = GUCEF_IMAGE_TIMAGEFRAME_VERSION;
                frame->frameInfo.version = GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION;
                frame->frameInfo.nrOfMipmapLevels = mipmapCount = (UInt32) 1; /* @TODO: mip-mapping is not supported atm */

                /* Build the TImageMipMapLevelInfo sections */
                frame->mipmapLevel = (TImageMipMapLevel*) malloc( mipmapCount * sizeof(TImageMipMapLevel) );
                for ( n=0; n<mipmapCount; ++n )
                {
                    TImageMipMapLevel* mipmapLevel = &frame->mipmapLevel[ n ];
                    mipmapLevel->version = GUCEF_IMAGE_TIMAGEMIPMAPLEVEL_VERSION;
                    mipmapLevel->mipLevelInfo.version = GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION;
                    mipmapLevel->mipLevelInfo.frameHeight = FreeImage_GetHeight( dib );
                    mipmapLevel->mipLevelInfo.frameWidth = FreeImage_GetWidth( dib );
                    mipmapLevel->mipLevelInfo.pixelComponentDataType = ConvertToPCDT( fit );
                    mipmapLevel->mipLevelInfo.pixelStorageFormat = ConvertToPSF( fit, colourType );
                    mipmapLevel->pixelData = NULL;
                    mipmapLevel->pixelDataSizeInBytes = 0;
                }
            }

            /* now that we know the total size/layout we can make our link array which will allow us to link pixel data to our FreeImage structs */
            imageDataLinks = (TFreeImageDataLinks*) malloc( sizeof( TFreeImageDataLinks ) );
            imageDataLinks->linkCount = GetNrOfPixelBuffers( image );
            imageDataLinks->links = (TFreeImageDataLink*) malloc( imageDataLinks->linkCount * sizeof( TFreeImageDataLink ) );
            memset( imageDataLinks->links, 0, imageDataLinks->linkCount * sizeof( TFreeImageDataLink ) );

            /* now we assign the pixel data */
            linkIndex = 0;
            for ( i=0; i<frameCount; ++i )
            {
                TImageFrame* frame = &image->frames[ i ];
                for ( n=0; n<mipmapCount; ++n )
                {
                    /* assign the pixel data. */
                    TImageMipMapLevel* mipmapLevel = &frame->mipmapLevel[ n ];
                    TFreeImageDataLink* dataLink = &imageDataLinks->links[ linkIndex ];

                    /* @TODO: note that GetBits/dib needs to be something else once we support frames/mipmapping */
                    mipmapLevel->pixelData = FreeImage_GetBits( dib );
                    mipmapLevel->pixelDataSizeInBytes = ( FreeImage_GetBPP( dib ) / 8 ) * mipmapLevel->mipLevelInfo.frameWidth * mipmapLevel->mipLevelInfo.frameHeight; 
                    dataLink->pixelData = mipmapLevel->pixelData;
                    dataLink->fiData = dib;

                    ++linkIndex;
                }
            }

            /* we successfully loaded and processed the image */
            *imageOutput = image;
            *imageData = imageDataLinks;
            return 1;
        }
    }

    /*
     *  the image decode failed
     */
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMGCODECPLUGIN_EncodeImage( void* pluginData      ,
                            void* codecData       ,
                            const char* codecType ,
                            TImage* inputImage    ,
                            TIOAccess* output     )
{
    /* @TODO */
    return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* restore structure packing */
#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(pop)
#else
#pragma pack()
#endif /* WIN32 */

/*---------------------------------------------------------------------------*/
