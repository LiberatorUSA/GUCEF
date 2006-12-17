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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CMFILEACCESS_H
#include "CMFileAccess.h"
#define GUCEF_CORE_CMFILEACCESS_H
#endif /* GUCEF_CORE_CMFILEACCESS_H ? */

#include "CIMGCodecPluginItem.h"
#include "CIMGCodecPlugin.h"        /* definition of this class */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum
{
        IMGPLUG_INIT       = 0         ,        
        IMGPLUG_SHUTDOWN               ,
        IMGPLUG_LOAD                   ,
        IMGPLUG_UNLOAD                 ,         
        IMGPLUG_SAVE                   ,               
        IMGPLUG_NAME                   ,
        IMGPLUG_COPYRIGHT              ,
        IMGPLUG_VERSION                ,
        IMGPLUG_DETECTTYPE             ,
        IMGPLUG_FORMATLIST             ,
        
        IMGPLUG_FUNCTIONTABLESIZE
};

/*-------------------------------------------------------------------------*/

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPRT_Load ) ( void* plugindata, const char* imageType, CORE::TIOAccess* sourceData, TImage** outputImageData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPRT_UnloadLoaded ) ( void* plugdata, TImage* imageData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPRT_Save ) ( void* plugdata, const char* imageType, TImage* inputImageData, CORE::TIOAccess* outputMedia ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef const char*   ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPRT_DetectType ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPRT_FormatList ) ( void* plugdata, const char*** supportedFormats ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPTR_Init ) ( void** plugdata, const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPTR_Shutdown ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPTR_Name ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPTR_Copyright ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const CORE::TVersion* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TIMGPLUGFPTR_Version ) ( void ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CIMGCodecPlugin::CIMGCodecPlugin( void )
    : m_sohandle( NULL ) ,
      m_modulePath()     ,
      m_codecList()
{TRACE;
    
    memset( m_fptable, 0, IMGPLUG_FUNCTIONTABLESIZE );
}

/*-------------------------------------------------------------------------*/

CIMGCodecPlugin::~CIMGCodecPlugin()
{TRACE;
    
    UnloadPlugin();
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodecPlugin::LoadPlugin( const CORE::CString& filename )
{TRACE;
   
    // First we try to load the module itself
    try 
    {
        m_sohandle = CORE::LoadModuleDynamicly( filename.C_String() );        
        if ( m_sohandle == NULL ) return false;        
    }
    catch ( ... )
    {
        m_sohandle = NULL;
        m_modulePath = NULL;
        return false;        
    }
    
    // If we get here then the module is a loadable program extension, so now
    // let's see if it is an image codec
    m_fptable[ IMGPLUG_NAME ] = CORE::GetFunctionAddress( m_sohandle         ,
                                                          "IMAGEPLUGIN_Name" ,
                                                          1*PTRSIZE          );
    m_fptable[ IMGPLUG_COPYRIGHT ] = CORE::GetFunctionAddress( m_sohandle              ,
                                                               "IMAGEPLUGIN_Copyright" ,
                                                               1*PTRSIZE               );
    m_fptable[ IMGPLUG_VERSION ] = CORE::GetFunctionAddress( m_sohandle            ,
                                                             "IMAGEPLUGIN_Version" ,
                                                             1*PTRSIZE             );
    m_fptable[ IMGPLUG_INIT ] = CORE::GetFunctionAddress( m_sohandle         ,
                                                          "IMAGEPLUGIN_Init" ,
                                                          2*PTRSIZE          ); 
    m_fptable[ IMGPLUG_SHUTDOWN ] = CORE::GetFunctionAddress( m_sohandle             ,
                                                              "IMAGEPLUGIN_Shutdown" ,
                                                              1*PTRSIZE              );
    m_fptable[ IMGPLUG_LOAD ] = CORE::GetFunctionAddress( m_sohandle         ,
                                                          "IMAGEPLUGIN_Load" ,
                                                          5*PTRSIZE          );                                                          
    m_fptable[ IMGPLUG_SAVE ] = CORE::GetFunctionAddress( m_sohandle         ,
                                                          "IMAGEPLUGIN_Save" ,
                                                          4*PTRSIZE          );
    m_fptable[ IMGPLUG_UNLOAD ] = CORE::GetFunctionAddress( m_sohandle                      ,
                                                            "IMAGEPLUGIN_DeleteLoadedImage" ,
                                                            3*PTRSIZE                       );
    m_fptable[ IMGPLUG_DETECTTYPE ] = CORE::GetFunctionAddress( m_sohandle                    ,
                                                                "IMAGEPLUGIN_DetectImageType" ,
                                                                2*PTRSIZE                     );                                                            
    m_fptable[ IMGPLUG_FORMATLIST ] = CORE::GetFunctionAddress( m_sohandle                  ,
                                                                "IMAGEPLUGIN_GetFormatList" ,
                                                                2*PTRSIZE                   );                                                            
    
    // Verify that we have obtained a function address for each of the functions
    if ( ( !m_fptable[ IMGPLUG_INIT ] ) ||
         ( !m_fptable[ IMGPLUG_SHUTDOWN ] ) ||
         ( !m_fptable[ IMGPLUG_FORMATLIST ] ) ||
         ( !m_fptable[ IMGPLUG_DETECTTYPE ] ) ||
         ( !m_fptable[ IMGPLUG_LOAD ] ) ||
         ( !m_fptable[ IMGPLUG_SAVE ] ) ||
         ( !m_fptable[ IMGPLUG_UNLOAD ] ) ||
         ( !m_fptable[ IMGPLUG_NAME ] ) ||
         ( !m_fptable[ IMGPLUG_COPYRIGHT ] ) ||
         ( !m_fptable[ IMGPLUG_VERSION ] ) )
    {        
            CORE::UnloadModuleDynamicly( m_sohandle );
            m_sohandle = NULL;
            m_modulePath = NULL;
            return false;
    }    
    m_modulePath = filename;
    
    // The module and been successfully loaded and linked
    // we will now generate a list of codec's
    const char** formatList = NULL;
    if ( 0 != ((TIMGPLUGFPRT_FormatList) m_fptable[ IMGPLUG_FORMATLIST ])( m_pluginData, &formatList ) )
    {
        while ( NULL != formatList )
        {
            if ( NULL != *formatList )
            {
                m_codecList.push_back( CIMGCodecPtr( new CIMGCodecPluginItem( this, *formatList ) ) );
            }
            
            ++formatList;
        }
    }       
    
    return true;
}

/*-------------------------------------------------------------------------*/
        
bool
CIMGCodecPlugin::UnloadPlugin( void )
{TRACE;

    // Check for outstanding references to our codec's
    CIMGCodecPtrList::iterator i = m_codecList.begin();
    while ( i != m_codecList.end() )
    {
        if ( (*i).GetReferenceCount() > 1 )
        {
            // Cannot unload the module if someone is still using one of it's codec's
            return false;
        }
        ++i;
    }    
    
    // No outstanding references,.. unload module
    CORE::UnloadModuleDynamicly( m_sohandle );
    m_sohandle = NULL;
    return true;
}

/*-------------------------------------------------------------------------*/
        
bool
CIMGCodecPlugin::IsPluginLoaded( void ) const
{TRACE;

    return NULL != m_sohandle;
}

/*-------------------------------------------------------------------------*/
        
CORE::CString
CIMGCodecPlugin::GetModulePath( void ) const
{TRACE;
    
    if ( IsPluginLoaded() )
    {
        return m_modulePath;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::IMAGE::CIMGCodecPlugin::GetPluginModulePath(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIMGCodecPlugin::GetName( void ) const
{TRACE;

    if ( IsPluginLoaded() )
    {
    }
   
   GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::IMAGE::CIMGCodecPlugin::GetName(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIMGCodecPlugin::GetCopyright( void ) const
{TRACE;

    if ( IsPluginLoaded() )
    {
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::IMAGE::CIMGCodecPlugin::GetCopyright(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CORE::TVersion
CIMGCodecPlugin::GetVersion( void ) const
{TRACE;

    if ( IsPluginLoaded() )
    {
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::IMAGE::CIMGCodecPlugin::GetVersion(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodecPlugin::GetCodecList( CIMGCodecPtrList& codecList )
{TRACE;

    if ( IsPluginLoaded() )
    {    
        codecList = m_codecList;
        return true;
    }

    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::IMAGE::CIMGCodecPlugin::GetCodecList(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodecPlugin::Encode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         TDynamicBufferList& dest       ,
                         UInt32& destBuffersUsed        ,
                         const CORE::CString& typeName  )
{TRACE;

    return false; //@todo makeme
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodecPlugin::Decode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         TDynamicBufferList& dest       ,
                         UInt32& destBuffersUsed        ,
                         const CORE::CString& typeName  )
{TRACE;

    CORE::CMFileAccess mfile( sourceData, sourceBuffersSize );
    TImage* image;
    memset( &image, 0, sizeof( TImage ) );
    
    if ( 0 != ((TIMGPLUGFPRT_Load) m_fptable[ IMGPLUG_LOAD ])( m_pluginData         , 
                                                               typeName.C_String()  ,
                                                               mfile.CStyleAccess() ,
                                                               &image               ) )
    {
        // The image data has been successfully loaded.
        // We must now shove the data into the buffer
        CORE::CDynamicBuffer& buffer = dest[ destBuffersUsed ];
        buffer.Clear();
        buffer.Append( &image->imageInfo, sizeof( TImageInfo ) );
        for ( UInt32 i=0; i<image->imageInfo.nrOfFramesInImage; ++i )
        {            
            TImageFrame* imageFrame = &image->frames[ i ];
            buffer.Append( &imageFrame->frameInfo, sizeof( TImageFrameInfo ) );
            
            for ( UInt32 n=0; n<imageFrame->frameInfo.nrOfMipmapLevels; ++n )
            {
                TImageMipMapLevel* mipmapLevel = &imageFrame->mipmapLevel[ n ];
                UInt32 pixelDataSize = ( ( mipmapLevel->mipLevelInfo.frameHeight * mipmapLevel->mipLevelInfo.frameWidth )                    *  // pixels in image
                                         ( mipmapLevel->mipLevelInfo.channelCountPerPixel * mipmapLevel->mipLevelInfo.channelComponentSize ) ); // size of a pixel in bytes
                buffer.Append( &mipmapLevel->mipLevelInfo, sizeof( TImageMipMapLevelInfo ) );
                buffer.Append( mipmapLevel->pixelData, pixelDataSize );
            }
        }
        
        // Now that we finished the copy operation into the buffer we can unload the image again
        ((TIMGPLUGFPRT_UnloadLoaded) m_fptable[ IMGPLUG_UNLOAD ])( m_pluginData , 
                                                                   image        );
        
        return true;
    }
    
    // Loading failed
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
