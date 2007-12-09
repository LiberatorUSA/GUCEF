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
    FREE_IMAGE_FORMAT fif;
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
                    imageMMInfo.channelComponentSize = 8; /* FreeImage only supports UInt8 when using the default flags */
                    imageMMInfo.channelCountPerPixel = FreeImage_GetBPP(dib );
                    imageMMInfo.frameHeight = FreeImage_GetHeight( dib );
                    imageMMInfo.frameWidth = FreeImage_GetWidth( dib );
                    imageMMInfo.pixelComponentDataType = DT_UINT8; /* FreeImage only supports this type when using the default flags */
                    imageMMInfo.pixelStorageFormat = PSF_RGB;  /* FreeImage only supports this type when using the default flags */
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
                output->write( output, FreeImage_GetBits( dib ), FreeImage_GetDIBSize( dib ), 1 );
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