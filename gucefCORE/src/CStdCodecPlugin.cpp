/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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

#ifndef GUCEF_CORE_CODECPLUGINLINK_H
#include "CodecPluginLink.h"
#define GUCEF_CORE_CODECPLUGINLINK_H
#endif /* GUCEF_CORE_CODECPLUGINLINK_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#include "CStdCodecPlugin.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum
{
        STDCODEC_INIT       = 0         ,        
        STDCODEC_SHUTDOWN               ,
        STDCODEC_COPYRIGHT              ,
        STDCODEC_VERSION                ,
        STDCODEC_DESCRIPTION            ,
        
        STDCODEC_CODECSETBEGIN          ,
        STDCODEC_CODECLINK              ,
        STDCODEC_FREECODECLINK          ,
        STDCODEC_CODECSETNEXT           ,
        
        STDCODEC_FUNCTIONTABLESIZE
};

/*-------------------------------------------------------------------------*/

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Init ) ( void** plugdata, const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Shutdown ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Copyright ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef TVersion ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Version ) ( void ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Description ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_GetCodecSetBegin ) ( void* plugdata, void** iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_GetCodecLink ) ( void* plugdata, void* iterator, TCodecPluginLink** codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_FreeCodecLink ) ( void* plugdata, TCodecPluginLink* codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_GetCodecSetNextItem ) ( void* plugdata, void* iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CStdCodecPlugin::CStdCodecPlugin( void )
    : CICodecPlugin()    ,
      m_soHandle( NULL ) ,
      m_modulePath()     ,
      m_pluginData()     ,
      m_codecSet()       ,
      m_codecList()
{TRACE;

    memset( m_fpTable, 0, STDCODEC_FUNCTIONTABLESIZE );
}

/*-------------------------------------------------------------------------*/
    
CStdCodecPlugin::~CStdCodecPlugin()
{TRACE;

    Unload();
}

/*-------------------------------------------------------------------------*/

void
CStdCodecPlugin::LinkCodecSet( void )
{TRACE;

    if ( IsPluginLoaded() )
    {
        m_codecSet.clear();
        m_codecList.clear();
        
        void* iterator = NULL;
        if ( ( (TCODECPLUGFPTR_GetCodecSetBegin) m_fpTable[ STDCODEC_CODECSETBEGIN ] )( m_pluginData, &iterator ) != 0 )
        {
            do
            {
                TCodecPluginLink* codecLink = NULL;
                if ( ( (TCODECPLUGFPTR_GetCodecLink) m_fpTable[ STDCODEC_CODECLINK ] )( m_pluginData, iterator, &codecLink ) != 0 )
                {
                    CStdCodecPluginItem* codecItem = new CStdCodecPluginItem( codecLink );
                    CCodecFamilySet& familySet = m_codecSet[ codecLink->codecFamily ];
                    familySet.insert( codecLink->codecType, CCodecPtr( codecItem ) );
                    
                    ( (TCODECPLUGFPTR_FreeCodecLink) m_fpTable[ STDCODEC_FREECODECLINK ] )( m_pluginData, codecLink );
                }
            }
            while ( ( (TCODECPLUGFPTR_GetCodecSetNextItem) m_fpTable[ STDCODEC_CODECSETNEXT ] )( m_pluginData, iterator ) != 0 );
        }
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::LinkCodecSet(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::GetCodecs( CCodecSet& codecSet )
{TRACE;
    
    codecSet = m_codecSet;
    return true;
}
    
/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::GetCodecList( CCodecList& codecList )
{TRACE;
    
    codecList = m_codecList;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStdCodecPlugin::Encode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         CIOAccess& dest                ,
                         UInt32& destBytesWritten       ,
                         const CString& familyName      ,
                         const CString& typeName        )
{TRACE;

    if ( IsPluginLoaded() )
    {
        CCodecFamilySet::iterator i = m_codecSet.find( familyName );
        if ( m_codecSet.end() != i )
        {
            CCodecFamilySet& familySet = i.second;
            CCodecFamilySet::iterator n = familySet.find( typeName );
            if ( familySet.end() != n )
            {
                return n.second->Encode( sourceData        ,
                                         sourceBuffersSize ,
                                         dest              ,
                                         destBytesWritten  );
            }
        }
        return false;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::Encode(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

bool
CStdCodecPlugin::Decode( const void* sourceData         ,
                         const UInt32 sourceBuffersSize ,
                         CIOAccess& dest                ,
                         UInt32& destBytesWritten       ,
                         const CString& familyName      ,
                         const CString& typeName        )
{TRACE;

    if ( IsPluginLoaded() )
    {
        CCodecFamilySet::iterator i = m_codecSet.find( familyName );
        if ( m_codecSet.end() != i )
        {
            CCodecFamilySet& familySet = i.second;
            CCodecFamilySet::iterator n = familySet.find( typeName );
            if ( familySet.end() != n )
            {
                return n.second->Decode( sourceData        ,
                                         sourceBuffersSize ,
                                         dest              ,
                                         destBytesWritten  );
            }
        }
        return false;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::Decode(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CString
CStdCodecPlugin::GetDescription( void ) const
{TRACE;

    if ( IsPluginLoaded() )
    {
        return ( (TCODECPLUGFPTR_Description) m_fpTable[ STDCODEC_DESCRIPTION ] )( m_soHandle );
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::GetDescription(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CString
CStdCodecPlugin::GetCopyright( void ) const
{TRACE;
    
    if ( IsPluginLoaded() )
    {
        return ( (TCODECPLUGFPTR_Copyright) m_fpTable[ STDCODEC_COPYRIGHT ] )( m_soHandle );
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::GetCopyright(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
TVersion
CStdCodecPlugin::GetVersion( void ) const
{TRACE;
    
    if ( IsPluginLoaded() )
    {
        return ( (TCODECPLUGFPTR_Version) m_fpTable[ STDCODEC_VERSION ] )( m_soHandle );
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::GetVersion(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
CString
CStdCodecPlugin::GetModulePath( void ) const
{TRACE;
    
    if ( IsPluginLoaded() )
    {
        return m_modulePath;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::GetModulePath(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::IsLoaded( void ) const
{TRACE;

    return m_soHandle != NULL;
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::Load( const CString& pluginPath )
{TRACE;

    // make sure no module is already loaded
    if ( m_soHandle != NULL ) return false;
    
    // First we try to load the module itself
    try 
    {
        m_soHandle = LoadModuleDynamicly( pluginPath.C_String() );        
        if ( m_soHandle == NULL ) return false;        
    }
    catch ( ... )
    {
        m_sohandle = NULL;
        return false;        
    }
    
    // If we get here then the module is a loadable program extension, so now
    // let's see if it is an image codec
    m_fpTable[ STDCODECPLUG_DESCRIPTION ] = GetFunctionAddress( m_sohandle                ,
                                                                "CODECPLUGIN_Description" ,
                                                                1*sizeof(void*)           );
    m_fpTable[ STDCODECPLUG_COPYRIGHT ] = GetFunctionAddress( m_sohandle              ,
                                                              "CODECPLUGIN_Copyright" ,
                                                              1*sizeof(void*)         );
    m_fpTable[ STDCODECPLUG_VERSION ] = GetFunctionAddress( m_sohandle            ,
                                                            "CODECPLUGIN_Version" ,
                                                            1*sizeof(void*)       );
    m_fpTable[ STDCODECPLUG_INIT ] = GetFunctionAddress( m_sohandle         ,
                                                         "CODECPLUGIN_Init" ,
                                                         2*sizeof(void*)    ); 
    m_fpTable[ STDCODECPLUG_SHUTDOWN ] = GetFunctionAddress( m_sohandle             ,
                                                             "CODECPLUGIN_Shutdown" ,
                                                             1*sizeof(void*)        );
    m_fpTable[ STDCODEC_CODECSET ] = GetFunctionAddress( m_sohandle                ,
                                                         "CODECPLUGIN_GetCodecSet" ,
                                                         1*sizeof(void*)           );
    m_fpTable[ STDCODEC_CODECLIST ] = GetFunctionAddress( m_sohandle                 ,
                                                          "CODECPLUGIN_GetCodecList" ,
                                                          1*sizeof(void*)            );
    m_fpTable[ STDCODEC_ENCODE ] = GetFunctionAddress( m_sohandle           ,
                                                       "CODECPLUGIN_Encode" ,
                                                       1*sizeof(void*)      );
    m_fpTable[ STDCODEC_DECODE ] = GetFunctionAddress( m_sohandle           ,
                                                       "CODECPLUGIN_Decode" ,
                                                       1*sizeof(void*)      );
                                                                                                                                                                                                                                           
    // Verify that we have obtained a function address for each of the functions
    if ( ( !m_fpTable[ STDCODECPLUG_INIT ] )        ||
         ( !m_fpTable[ STDCODECPLUG_SHUTDOWN ] )    ||
         ( !m_fpTable[ STDCODECPLUG_DESCRIPTION ] ) ||
         ( !m_fpTable[ STDCODECPLUG_COPYRIGHT ] )   ||
         ( !m_fpTable[ STDCODECPLUG_VERSION ] ) )   ||
         ( !m_fpTable[ STDCODEC_CODECSET ] )        ||
         ( !m_fpTable[ STDCODEC_CODECLIST ] )       ||
         ( !m_fpTable[ STDCODEC_ENCODE ] )          ||
         ( !m_fpTable[ STDCODEC_DECODE ] )           )
    {        
            UnloadModuleDynamicly( m_soHandle );
            m_sohandle = NULL;
            m_modulePath = NULL;
            return false;
    }    
    m_modulePath = pluginPath;
    
    // The module and been successfully loaded and linked
    // we will now generate a list of codec's
    const char** formatList = NULL;
    if ( 0 != ((TIMGPLUGFPRT_FormatList) m_fptable[ IMGPLUG_FORMATLIST ])( m_pluginData, &formatList ) )
    {
        while ( NULL != formatList )
        {
            if ( NULL != *formatList )
            {
                CIMGCodecPtr codecObjPtr( new CORE::CTCodecPluginItem< CIMGCodec >( *this, *formatList ) );
                m_codecList.push_back( codecObjPtr );
            }
            
            ++formatList;
        }
    }       
    
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::Unload( void )
{TRACE;

    // Check for outstanding references to our codec's
    CIMGCodecList::iterator i = m_codecList.begin();
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace CORE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/