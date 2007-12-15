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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define IMGPLUGIN_FREEIMAGE_MAYOR_VERSION       0
#define IMGPLUGIN_FREEIMAGE_MINOR_VERSION       1
#define IMGPLUGIN_FREEIMAGE_PATCH_VERSION       0
#define IMGPLUGIN_FREEIMAGE_RELEASE_VERSION     0

/*---------------------------------------------------------------------------*/

static FreeImageIO io;
static const UInt32 codecCount = 33;

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
    "bmp"    ,
    "psd"    ,
    "cut"    ,
    "xbm"    ,
    "xpm"    ,
    "dds"    ,
    "gif"    ,
    "hdr"    ,
    "faxg3"  ,
    "sgi"
};

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
            return DT_UINT8;
        }
        case FIT_COMPLEX :
        case FIT_UNKNOWN :
        {
            return DT_UNKNOWN;
        }
        case FIT_UINT32 :
        {
            return DT_UINT32;
        }        
        case FIT_INT32 :
        {
            return DT_INT32;
        }        
        case FIT_FLOAT :
        {
            return DT_FLOAT32;
        }
        case FIT_DOUBLE :
        {
            return DT_FLOAT64;
        }        
        case FIT_UINT16 :
        {
            return DT_UINT16;
        }        
        case FIT_INT16 :
        {
            return DT_INT16;
        }
        case FIT_RGBF :
        {
            return DT_FLOAT32;
        }        
        case FIT_RGB16 :
        {
            return DT_UINT16;
        }
        case FIT_RGBAF :
        {
            return DT_FLOAT32;
        }        
        case FIT_RGBA16 :
        {
            return DT_UINT16;
        }
        default :
        {
            return DT_UNKNOWN;
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
        case DT_INT8 :
        case DT_UINT8 :
        {
            return 1;
        }
        case DT_INT16 :
        case DT_UINT16 :
        {
            return 2;
        }        
        case DT_INT32 :
        case DT_UINT32 :
        case DT_FLOAT32 :
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
CODECPLUGIN_Init( void** plugdata    , 
                  const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
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
    return "Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.\0";
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
CODECPLUGIN_Decode( void* plugdata         ,
                    void* codecData        ,
                    const char* familyType ,
                    const char* codecType  ,
                    TIOAccess* input       ,
                    TIOAccess* output      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32 i=0, n=0, mipmapCount=0, frameCount=0;
    TImageInfo imageInfo;
    TImageFrameInfo imageFrameInfo;
    TImageMipMapLevelInfo imageMMInfo;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FREE_IMAGE_TYPE fit = FIT_UNKNOWN;
    FIBITMAP* dib = NULL;
    
    /* input sanity check */
    if ( ( NULL == familyType ) || ( NULL == codecType ) || ( NULL == input ) || ( NULL == output ) ) return 0;

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
            UInt32 bpp = FreeImage_GetBPP( dib );
            FREE_IMAGE_COLOR_TYPE colourType = FreeImage_GetColorType( dib );            
            fit = FreeImage_GetImageType( dib );
            
            if ( FIT_BITMAP == fit )
            {
                if ( bpp < 8 )
                {
				    /* we do not support a format that has less that 8 bits per pixel */
				    FIBITMAP* newDIB = FreeImage_ConvertTo8Bits( dib );
				    FreeImage_Unload( newDIB );
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
				    FreeImage_Unload( newDIB );
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
				    FreeImage_Unload( newDIB );
				    dib = newDIB;
				    colourType = FreeImage_GetColorType( dib );
				    bpp = FreeImage_GetBPP( dib );
			    }
			    
			    /* At this point images are always greyscale or RGB or RGBA and BBP is always >= 8 */
            }
			

            /* write the TImageInfo section */
            imageInfo.version = GUCEF_IMAGE_TIMAGEINFO_VERSION;
            frameCount = imageInfo.nrOfFramesInImage = (UInt32) 1; /* multiple frames are not supported atm */
            output->write( output, &imageInfo, sizeof( imageInfo ), 1  );

            for ( i=0; i<frameCount; ++i )
            {
                /* write the TImageFrameInfo section */
                imageFrameInfo.version = GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION;
                mipmapCount = imageFrameInfo.nrOfMipmapLevels = (UInt32) 1; /* mip-mapping is not supported atm */           
                output->write( output, &imageFrameInfo, sizeof( imageFrameInfo ), 1 );
        
                for ( n=0; n<mipmapCount; ++n )
                {
                    /* write the TImageMipMapLevelInfo section */
                    imageMMInfo.version = GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION;
                    imageMMInfo.frameHeight = FreeImage_GetHeight( dib );
                    imageMMInfo.frameWidth = FreeImage_GetWidth( dib );
                    imageMMInfo.pixelComponentDataType = ConvertToPCDT( fit );
                    imageMMInfo.pixelStorageFormat = ConvertToPSF( fit, colourType );
                    output->write( output, &imageMMInfo, sizeof( imageMMInfo ), 1 );
                }
            }
        }
        
        /* now we append the pixel data */
        for ( i=0; i<frameCount; ++i )
        {
            for ( n=0; n<mipmapCount; ++n )
            {            
                /* write the pixel data */
           
                unsigned char* srcData = FreeImage_GetBits( dib );
                unsigned char* pSrc = NULL;
                UInt32 dstPitch = imageMMInfo.frameWidth * GetPixelSize( imageMMInfo.pixelStorageFormat, imageMMInfo.pixelComponentDataType );
                UInt32 srcPitch = FreeImage_GetPitch( dib );
                UInt32 y = 0;
                
                for ( y = 0; y < imageMMInfo.frameHeight; ++y )
                {
                    pSrc = srcData + ( imageMMInfo.frameHeight - y - 1 ) * srcPitch;
                    if ( dstPitch != output->write( output, pSrc, dstPitch, 1 ) )
                    {
                        /*
                         *  we where unable to write all the data to the output resource
                         *  Image loading has failed
                         */
                        FreeImage_Unload( dib );    
                        return 0;                         
                    }
                }
            }            
        }

        /*
         *  the image has been loaded into our GUCEF structures so we have no need for
         *  FreeImage to store the data any more 
         */
        FreeImage_Unload( dib );
        return 1;
    }
    
    /*
     *  the image load failed, no need for FreeImage to store the data any more 
     */
    FreeImage_Unload( dib );    
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