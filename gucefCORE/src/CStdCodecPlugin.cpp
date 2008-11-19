/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#include <assert.h>

#ifndef GUCEF_CORE_CODECPLUGINLINK_H
#include "CodecPluginLink.h"
#define GUCEF_CORE_CODECPLUGINLINK_H
#endif /* GUCEF_CORE_CODECPLUGINLINK_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_CSTDCODECPLUGINITEM_H
#include "CStdCodecPluginItem.h"
#define GUCEF_CORE_CSTDCODECPLUGINITEM_H
#endif /* GUCEF_CORE_CSTDCODECPLUGINITEM_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

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
        STDCODEC_FREECODECITERATOR      ,
        STDCODEC_CODECSETNEXT           ,
        
        STDCODEC_FUNCTIONTABLESIZE
};

/*-------------------------------------------------------------------------*/

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Init ) ( void** plugdata, const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Shutdown ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Copyright ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef TVersion ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Version ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_Description ) ( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_GetCodecSetBegin ) ( void* plugdata, void** iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_GetCodecLink ) ( void* plugdata, void* iterator, TCodecPluginLink** codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_FreeCodecLink ) ( void* plugdata, TCodecPluginLink* codecLink ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TCODECPLUGFPTR_FreeCodecIterator ) ( void* plugdata, void* iterator ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
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
{GUCEF_TRACE;

    memset( m_fpTable, 0, STDCODEC_FUNCTIONTABLESIZE );
}

/*-------------------------------------------------------------------------*/
    
CStdCodecPlugin::~CStdCodecPlugin()
{GUCEF_TRACE;

    Unload();
}

/*-------------------------------------------------------------------------*/

void
CStdCodecPlugin::LinkCodecSet( void )
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        UnlinkCodecSet();

        CCodecRegistry* codecRegistry = CCodecRegistry::Instance();
        void* iterator = NULL;
        if ( ( (TCODECPLUGFPTR_GetCodecSetBegin) m_fpTable[ STDCODEC_CODECSETBEGIN ] )( m_pluginData, &iterator ) != 0 )
        {
            do
            {
                TCodecPluginLink* codecLink = NULL;
                if ( ( (TCODECPLUGFPTR_GetCodecLink) m_fpTable[ STDCODEC_CODECLINK ] )( m_pluginData, iterator, &codecLink ) != 0 )
                {
                    assert( codecLink != NULL );
                    CCodecPtr codecItem = CCodecPtr( new CStdCodecPluginItem( m_pluginData, codecLink ) );
                    CCodecFamilySet& familySet = m_codecSet[ codecLink->codecFamily ];
                    familySet.insert( std::pair< CString, CCodecPtr >( codecLink->codecType, codecItem ) );
                    CCodecFamilyList& familyList = m_codecList[ codecLink->codecFamily ];
                    familyList.push_back( codecLink->codecType );
                    
                    // Now we register the codecs from this plugin in the GUCEF codec registry
                    // First we check if we have to add a new registry for this family
                    if ( !codecRegistry->IsRegistered( codecLink->codecFamily ) )
                    {
                        codecRegistry->Register( codecLink->codecFamily, new CCodecRegistry::TCodecFamilyRegistry() );
                    }
                    
                    // we now have access to a registry for this codec family
                    // We will add this codec if there no codec already registered with such a name
                    CCodecRegistry::TCodecFamilyRegistryPtr familyRegistry = codecRegistry->Lookup( codecLink->codecFamily );
                    if ( !familyRegistry->IsRegistered( codecLink->codecType ) )
                    {
                        familyRegistry->Register( codecLink->codecType, codecItem );
                    }
                }
            }
            while ( ( (TCODECPLUGFPTR_GetCodecSetNextItem) m_fpTable[ STDCODEC_CODECSETNEXT ] )( m_pluginData, iterator ) != 0 );
            
            ( (TCODECPLUGFPTR_FreeCodecIterator) m_fpTable[ STDCODEC_FREECODECITERATOR ] )( m_pluginData, iterator );
        }
        return;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::LinkCodecSet(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

void
CStdCodecPlugin::UnlinkCodecSet( void )
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        CCodecRegistry* codecRegistry = CCodecRegistry::Instance();
        CStdCodecPluginItem* pluginItem = NULL;
        CCodecSet::iterator i = m_codecSet.begin();
        while ( i != m_codecSet.end() )
        {
            CCodecFamilySet& codecFamily = (*i).second;
            CCodecFamilySet::iterator n = codecFamily.begin();
            while ( n != codecFamily.end() )
            {
                pluginItem = static_cast< CStdCodecPluginItem* >( &( *( (*n).second ) ) );
                TCodecPluginLink* codecLink = pluginItem->GetCodecPluginLink();
                
                // Now we unregister the codecs from this plugin from the GUCEF codec registry
                // First we check if family registry still exists                
                if ( codecRegistry->IsRegistered( codecLink->codecFamily ) )
                {
                    // we now have access to a registry for this codec family
                    // We will add this codec if there no codec already registered with such a name
                    CCodecRegistry::TCodecFamilyRegistryPtr familyRegistry = codecRegistry->Lookup( codecLink->codecFamily );
                    if ( !familyRegistry->IsRegistered( codecLink->codecType ) )
                    {
                        familyRegistry->Unregister( codecLink->codecType );
                    }                    
                }

                ( (TCODECPLUGFPTR_FreeCodecLink) m_fpTable[ STDCODEC_FREECODECLINK ] )( pluginItem->GetPluginData()      ,
                                                                                        pluginItem->GetCodecPluginLink() );
                ++n;
            }
            ++i;
        }
        
        m_codecSet.clear();
        m_codecList.clear();
        return;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::LinkCodecSet(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::GetCodecs( CCodecSet& codecSet )
{GUCEF_TRACE;
    
    codecSet = m_codecSet;
    return true;
}
    
/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::GetCodecList( CCodecList& codecList )
{GUCEF_TRACE;
    
    codecList = m_codecList;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStdCodecPlugin::Encode( CIOAccess& source         ,   
                         CIOAccess& dest           ,
                         const CString& familyName ,
                         const CString& typeName   )
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        CCodecSet::iterator i = m_codecSet.find( familyName );
        if ( m_codecSet.end() != i )
        {
            CCodecFamilySet& familySet = (*i).second;
            CCodecFamilySet::iterator n = familySet.find( typeName );
            if ( familySet.end() != n )
            {
                return (*n).second->Encode( source ,
                                            dest   );
            }
        }
        return false;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::Encode(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

bool
CStdCodecPlugin::Decode( CIOAccess& source         ,
                         CIOAccess& dest           ,
                         const CString& familyName ,
                         const CString& typeName   )
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        CCodecSet::iterator i = m_codecSet.find( familyName );
        if ( m_codecSet.end() != i )
        {
            CCodecFamilySet& familySet = (*i).second;
            CCodecFamilySet::iterator n = familySet.find( typeName );
            if ( familySet.end() != n )
            {
                return (*n).second->Decode( source ,
                                            dest   );
            }
        }
        return false;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::Decode(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CString
CStdCodecPlugin::GetDescription( void ) const
{GUCEF_TRACE;

    if ( IsLoaded() )
    {
        return ( (TCODECPLUGFPTR_Description) m_fpTable[ STDCODEC_DESCRIPTION ] )( m_pluginData );
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::GetDescription(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/

CString
CStdCodecPlugin::GetCopyright( void ) const
{GUCEF_TRACE;
    
    if ( IsLoaded() )
    {
        return ( (TCODECPLUGFPTR_Copyright) m_fpTable[ STDCODEC_COPYRIGHT ] )( m_pluginData );
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::GetCopyright(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
TVersion
CStdCodecPlugin::GetVersion( void ) const
{GUCEF_TRACE;
    
    if ( IsLoaded() )
    {
        return ( (TCODECPLUGFPTR_Version) m_fpTable[ STDCODEC_VERSION ] )( m_pluginData );
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::GetVersion(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
CString
CStdCodecPlugin::GetModulePath( void ) const
{GUCEF_TRACE;
    
    if ( IsLoaded() )
    {
        return m_modulePath;
    }
    
    GUCEF_EMSGTHROW( ENotLoaded, "GUCEF::CORE::CStdCodecPlugin::GetModulePath(): No module is loaded" );
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::IsLoaded( void ) const
{GUCEF_TRACE;

    return m_soHandle != NULL;
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::Load( const CString& pluginPath )
{GUCEF_TRACE;

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
        m_soHandle = NULL;
        return false;        
    }
    
    // If we get here then the module is a loadable program extension, so now
    // let's see if it is a codec plugin
    m_fpTable[ STDCODEC_DESCRIPTION ] = GetFunctionAddress( m_soHandle                ,
                                                            "CODECPLUGIN_Description" ,
                                                            sizeof(void*)             );
    m_fpTable[ STDCODEC_COPYRIGHT ] = GetFunctionAddress( m_soHandle              ,
                                                          "CODECPLUGIN_Copyright" ,
                                                          sizeof(void*)           );
    m_fpTable[ STDCODEC_VERSION ] = GetFunctionAddress( m_soHandle            ,
                                                        "CODECPLUGIN_Version" ,
                                                        sizeof(void*)         );
    m_fpTable[ STDCODEC_INIT ] = GetFunctionAddress( m_soHandle                      ,
                                                     "CODECPLUGIN_Init"              ,
                                                     2*sizeof(void*)+sizeof(char***) ); 
    m_fpTable[ STDCODEC_SHUTDOWN ] = GetFunctionAddress( m_soHandle             ,
                                                         "CODECPLUGIN_Shutdown" ,
                                                         sizeof(void*)          );
    m_fpTable[ STDCODEC_CODECSETBEGIN ] = GetFunctionAddress( m_soHandle                     ,
                                                              "CODECPLUGIN_GetCodecSetBegin" ,
                                                              sizeof(void*)+sizeof(void**)   );
    m_fpTable[ STDCODEC_CODECLINK ] = GetFunctionAddress( m_soHandle                                 ,
                                                          "CODECPLUGIN_GetCodecLink"                 ,
                                                          2*sizeof(void*)+sizeof(TCodecPluginLink**) );
    m_fpTable[ STDCODEC_FREECODECLINK ] = GetFunctionAddress( m_soHandle                              ,
                                                              "CODECPLUGIN_FreeCodecLink"             ,
                                                              sizeof(void*)+sizeof(TCodecPluginLink*) );
    m_fpTable[ STDCODEC_FREECODECITERATOR ] = GetFunctionAddress( m_soHandle                      ,
                                                                  "CODECPLUGIN_FreeCodecIterator" ,
                                                                  2*sizeof(void*)                 );
    m_fpTable[ STDCODEC_CODECSETNEXT ] = GetFunctionAddress( m_soHandle                        ,
                                                             "CODECPLUGIN_GetCodecSetNextItem" ,
                                                             2*sizeof(void*)                   );
                                                                                                                                                                                                                                           
    // Verify that we have obtained a function address for each of the functions
    if ( ( !m_fpTable[ STDCODEC_INIT ] )              ||
         ( !m_fpTable[ STDCODEC_SHUTDOWN ] )          ||
         ( !m_fpTable[ STDCODEC_DESCRIPTION ] )       ||
         ( !m_fpTable[ STDCODEC_COPYRIGHT ] )         ||
         ( !m_fpTable[ STDCODEC_VERSION ] )           ||
         ( !m_fpTable[ STDCODEC_CODECSETBEGIN ] )     ||
         ( !m_fpTable[ STDCODEC_CODECLINK ] )         ||
         ( !m_fpTable[ STDCODEC_FREECODECLINK ] )     ||
         ( !m_fpTable[ STDCODEC_FREECODECITERATOR ] ) ||
         ( !m_fpTable[ STDCODEC_CODECSETNEXT ] )    )
    {        
            UnloadModuleDynamicly( m_soHandle );
            m_soHandle = NULL;
            memset( m_fpTable, 0, STDCODEC_FUNCTIONTABLESIZE );
            return false;
    }
    
    // We will now try to initialize the module
    if ( ( (TCODECPLUGFPTR_Init) m_fpTable[ STDCODEC_INIT ] )( &m_pluginData, NULL ) == 0 )
    {
        UnloadModuleDynamicly( m_soHandle );
        m_soHandle = NULL;
        memset( m_fpTable, 0, STDCODEC_FUNCTIONTABLESIZE );
        return false;
    }
    
    m_modulePath = pluginPath;
    
    // The module and been successfully loaded and linked
    // we will now generate a list of codec's
    LinkCodecSet();
        
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPlugin::Unload( void )
{GUCEF_TRACE;

    // Check if we have something to unload
    if ( m_soHandle == NULL ) return true;
    
    // Check for outstanding references to our codec's
    CCodecSet::iterator i = m_codecSet.begin();
    while ( i != m_codecSet.end() )
    {
        CCodecFamilySet& codecFamily = (*i).second;
        CCodecFamilySet::iterator n = codecFamily.begin();
        while ( n != codecFamily.end() )
        {
            if ( (*n).second.GetReferenceCount() > 1 )
            {
                // Cannot unload the module if someone is still using one of it's codec's
                return false;
            }
            ++n;
        }
        ++i;
    }    
    
    // Free all codec links
    UnlinkCodecSet();

    // We will now try to shut the module down
    if ( ( (TCODECPLUGFPTR_Shutdown) m_fpTable[ STDCODEC_SHUTDOWN ] )( m_pluginData ) == 0 )
    {
        return false;
    }

    // Module shutdown complete + no outstanding references,.. unload module
    UnloadModuleDynamicly( m_soHandle );
    memset( m_fpTable, 0, STDCODEC_FUNCTIONTABLESIZE );
    m_soHandle = NULL;
    m_modulePath = NULL;
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