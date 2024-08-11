/*
 *  imgpluginITV: GUCEF image codec plugin providing support for ITV images
 *
 *  Copyright (C) 1998 - 2024.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdlib.h>             /* for memory utils */
#include "flic.h"
#include "imgpluginFLIC.h" /* function prototypes for this plugin */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

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

#define IMGPLUGIN_FLIC_MAYOR_VERSION       0
#define IMGPLUGIN_FLIC_MINOR_VERSION       1
#define IMGPLUGIN_FLIC_PATCH_VERSION       0
#define IMGPLUGIN_FLIC_RELEASE_VERSION     0

/*---------------------------------------------------------------------------*/

static char* supportedTypes[] = {
    "flic"
};

static const UInt32 codecCount = sizeof( supportedTypes ) / sizeof(char*); 

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class IOAccessAdapter : public ::flic::FileInterface 
{
    private:
    CORE::TIOAccess* m_ioAccess;
    
    public:
    IOAccessAdapter( CORE::TIOAccess* ioAccess )
        : m_ioAccess( ioAccess )
    {}

    virtual ~IOAccessAdapter() {}

    bool ok() const override
    {
        return NULL != m_ioAccess && 0 == m_ioAccess->eof( m_ioAccess );
    }
    
    size_t tell() override
    {
        return (size_t) m_ioAccess->tell( m_ioAccess );
    }
    
    void seek( size_t absPos ) override
    {
        m_ioAccess->setpos( m_ioAccess, (UInt32) absPos );
        //m_ioAccess->seek( m_ioAccess, (Int32) absPos, SEEK_CUR );
    }
    
    uint8_t read8() override
    {
        if ( 0 == m_ioAccess->eof( m_ioAccess ) )
            return m_ioAccess->getc( m_ioAccess );
        else
            return 0;
    }
    
    void write8( uint8_t value ) override
    {
        m_ioAccess->write( m_ioAccess, &value, 1, 1 );
    }
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Init( void** plugdata   ,
                  const int argc    ,
                  const char** argv ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{

    *plugdata = NULL;
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{

    return 1;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Description( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "imgpluginFLIC: image codec allowing use of FLIC files";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "Copyright (C) Dinand Vanvelzen. 2002 - 2017.  LGPLv3 license\0";
}

/*---------------------------------------------------------------------------*/

CORE::TVersion GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    CORE::TVersion version;

    version.major = IMGPLUGIN_FLIC_MAYOR_VERSION;
    version.minor = IMGPLUGIN_FLIC_MINOR_VERSION;
    version.patch = IMGPLUGIN_FLIC_PATCH_VERSION;
    version.release = IMGPLUGIN_FLIC_RELEASE_VERSION;

    return version;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Encode( void* plugdata          ,
                    void* codecData         ,
                    const char* familyType  ,
                    const char* codecType   ,
                    CORE::TIOAccess* input  ,
                    CORE::TIOAccess* output ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    /* Saving images is not supported atm */
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
CODECPLUGIN_Decode( void* pluginData        ,
                    void* codecData         ,
                    const char* familyType  ,
                    const char* codecType   ,
                    CORE::TIOAccess* input  ,
                    CORE::TIOAccess* output ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
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
CODECPLUGIN_GetCodecLink( void* plugdata                     ,
                          void* iterator                     ,
                          CORE::TCodecPluginLink** codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    UInt32 codecIndex = *( (UInt32*) iterator );

    if ( codecIndex < codecCount )
    {
        *codecLink = (CORE::TCodecPluginLink*) malloc( sizeof( CORE::TCodecPluginLink ) );
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
CODECPLUGIN_FreeCodecLink( void* plugdata                    ,
                           CORE::TCodecPluginLink* codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
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

        return 1;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMGCODECPLUGIN_DecodeImage( void* pluginData       ,
                            void* codecData        ,
                            const char* codecType  ,
                            CORE::TIOAccess* input ,
                            TImage** imageOutput   ,
                            void** imageData       )
{

    /* input sanity check */
    if ( ( NULL == imageOutput ) || ( NULL == codecType ) || ( NULL == input ) || ( NULL == imageOutput ) ) 
        return 0;

    IOAccessAdapter ioAccessAdapter( input );
    flic::Decoder flicDecoder( &ioAccessAdapter );

    flic::Header flicHeader;
    if ( !flicDecoder.readHeader( flicHeader ) )
        return 0;

    // setup the image struct in the same way for any simple single frame/level image
    TImage* image = (TImage*) malloc( sizeof(TImage) );
    image->imageInfo.version = GUCEF_IMAGE_TIMAGEINFO_VERSION;
    image->imageInfo.nrOfFramesInImage = (UInt32) flicHeader.frames;
    image->frames = (TImageFrame*) malloc( sizeof(TImageFrame) * flicHeader.frames );
    memset( image->frames, 0, sizeof(TImageFrame) * flicHeader.frames );

    for ( int i=0; i<flicHeader.frames; ++i )
    {
        // Read the actual frame from the resource
        flic::Frame flicFrame;
        flicFrame.pixels = GUCEF_NULL;
        flicFrame.rowstride = 0;
        if ( !flicDecoder.readFrame( flicFrame ) )
            return 0;
        if ( GUCEF_NULL == flicFrame.pixels )
            continue;
        
        // Set basic frame info
        TImageFrame& frame = image->frames[ i ];
        frame.version = GUCEF_IMAGE_TIMAGEFRAME_VERSION;        
        frame.frameInfo.version = GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION;
        frame.frameInfo.nrOfMipmapLevels = 1;
        
        // FLIC only has 1 level per frame
        frame.mipmapLevel = (TImageMipMapLevel*) malloc( sizeof(TImageMipMapLevel) ); 
        TImageMipMapLevel& mipmapLevel = frame.mipmapLevel[ 0 ];
        mipmapLevel.version = GUCEF_IMAGE_TIMAGEMIPMAPLEVEL_VERSION;
        mipmapLevel.mipLevelInfo.version = GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION;        
        
        // width and height are the same for every frame
        mipmapLevel.mipLevelInfo.frameHeight = (UInt32) flicHeader.height;
        mipmapLevel.mipLevelInfo.frameWidth = (UInt32) flicHeader.width;

        // There is a palette per frame but we inflate that right away, not worth the effort
        int pixelCount = flicHeader.width * flicHeader.height;
        mipmapLevel.mipLevelInfo.pixelComponentDataType = GUCEF_DATATYPE_UINT8;
        mipmapLevel.mipLevelInfo.pixelStorageFormat = PSF_RGB;
        mipmapLevel.pixelData = malloc( pixelCount * 3 );
        if ( GUCEF_NULL == mipmapLevel.pixelData )
            return 0;
        memset( mipmapLevel.pixelData, 0, pixelCount * 3 );
        int m=0;
        UInt8* pixelData = (UInt8*) mipmapLevel.pixelData;
        for ( int n=0; n<pixelCount; ++n )
        {
            flic::Color& pixel = flicFrame.colormap[ flicFrame.pixels[ n ] ];
            pixelData[ m ] = pixel.r; ++m;
            pixelData[ m ] = pixel.g; ++m;
            pixelData[ m ] = pixel.b; ++m;
        }

    }
    
    *imageOutput = image;
    *imageData = NULL;
    return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMGCODECPLUGIN_DecodeImageMetaData( void* pluginData                             ,
                                    void* codecData                              ,
                                    const char* codecType                        ,
                                    CORE::TIOAccess* input                       ,
                                    CORE::TValueMapParserCallbacks* mapCallbacks )
{
    /* this format does not support metadata */
    if ( GUCEF_NULL != mapCallbacks )
    {
        mapCallbacks->OnValueMapBegin( mapCallbacks->privateData, 0, 0 );
        mapCallbacks->OnValueMapEnd( mapCallbacks->privateData );
        return 1;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMGCODECPLUGIN_EncodeImage( void* pluginData        ,
                            void* codecData         ,
                            const char* codecType   ,
                            TImage* inputImage      ,
                            CORE::TIOAccess* output )
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
