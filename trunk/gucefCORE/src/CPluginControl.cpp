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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"          /* framework string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"   /* Additional utilities for manipulating string class objects */ 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CPLUGINMANAGER_H
#include "CPluginManager.h"    /* base class for all framework plugin managers */
#define GUCEF_CORE_CPLUGINMANAGER_H
#endif /* GUCEF_CORE_CPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CLOADERDELEGATEDPLUGINLOADLOGIC_H
#include "gucefCORE_CLoaderDelegatedPluginLoadLogic.h"
#define GUCEF_CORE_CLOADERDELEGATEDPLUGINLOADLOGIC_H
#endif /* GUCEF_CORE_CLOADERDELEGATEDPLUGINLOADLOGIC_H ? */

#ifndef GUCEF_CORE_CSIMPLISTICPLUGINLOADLOGIC_H
#include "gucefCORE_CSimplisticPluginLoadLogic.h"
#define GUCEF_CORE_CSIMPLISTICPLUGINLOADLOGIC_H
#endif /* GUCEF_CORE_CSIMPLISTICPLUGINLOADLOGIC_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#include "CPluginControl.h"     /* definition of the class implemented here */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex CPluginControl::g_mutex;
CPluginControl* CPluginControl::g_instance = NULL;

static CLoaderDelegatedPluginLoadLogic loaderDelegatedPluginLoadLogic;
static CSimplisticPluginLoadLogic simplisticPluginLoadLogic;

const CEvent CPluginControl::PluginLoadedEvent = "GUCEF::CORE::CPluginControl::PluginLoadedEvent";
const CEvent CPluginControl::PluginUnloadedEvent = "GUCEF::CORE::CPluginControl::PluginUnloadedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CPluginControl::RegisterEvents( void )
{GUCEF_TRACE;
        
    PluginLoadedEvent.Initialize();
    PluginUnloadedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPluginControl*
CPluginControl::Instance( void )
{GUCEF_TRACE;
    
    if ( NULL == g_instance )
    {
        g_mutex.Lock();
        if ( NULL == g_instance )
        {
            g_instance = new CPluginControl();
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CPluginControl Singleton created" );
        }
        g_mutex.Unlock();
    }
    return g_instance; 
}

/*-------------------------------------------------------------------------*/

void 
CPluginControl::Deinstance( void )
{GUCEF_TRACE;

    g_mutex.Lock();
    delete g_instance;
    g_instance = NULL;
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CPluginControl Singleton destroyed" );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

CPluginControl::~CPluginControl()
{GUCEF_TRACE;

    UnloadAll();
}

/*-------------------------------------------------------------------------*/

CPluginControl::CPluginControl( void )
    : CNotifier()                                  ,
      m_pluginLoadLogicProviders()                 ,
      m_defaultPluginLoadLogicType( "Simplistic" ) ,
      m_pluginGroups()                             ,
      m_rootDirs()                                 ,
      m_pluginManagers()                           
{GUCEF_TRACE;

    // Register the load logic implementations available by default
    RegisterPluginLoadLogic( &simplisticPluginLoadLogic );
    RegisterPluginLoadLogic( &loaderDelegatedPluginLoadLogic );

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::LoadAllPluginsOfType( const CString& pluginTypeToLoad )
{GUCEF_TRACE;

    g_mutex.Lock();
    bool success = true;
    
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Loading all modules of type \"" + pluginTypeToLoad + '\"' );
    
    // Loop trough groups looking for modules of the given type
    TPluginGroupMap::iterator i = m_pluginGroups.begin();
    while ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        CPluginGroup::TPluginMetaDataSet& pluginMetaDataSet = pluginGroup.GetPluginMetaData();

        CPluginGroup::TPluginMetaDataSet::iterator n = pluginMetaDataSet.begin();
        while ( n != pluginMetaDataSet.end() )
        {
            TPluginMetaDataPtr pluginMetaData = (*n);
            
            if ( pluginTypeToLoad == pluginMetaData->GetPluginType() )
            {
                // Try to load this module since it is of the type we are looking for
                success = success & LoadPlugin( pluginMetaData ,
                                                pluginGroup    ,
                                                (*i).first     );
            }
            ++n;
        }
        ++i;
    }
    g_mutex.Unlock();

    return success;
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::LoadAllPluginsOfTypeInGroup( const CString& pluginTypeToLoad ,
                                             const CString& groupName        )
{GUCEF_TRACE;

    g_mutex.Lock();
    bool success = true;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Loading all modules in group \"" + groupName + "\" of type \"" + pluginTypeToLoad + '\"' );
    
    // Loop trough groups looking for modules of the given type
    TPluginGroupMap::iterator i = m_pluginGroups.find( groupName );
    if ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        CPluginGroup::TPluginMetaDataSet& pluginMetaDataSet = pluginGroup.GetPluginMetaData();

        CPluginGroup::TPluginMetaDataSet::iterator n = pluginMetaDataSet.begin();
        while ( n != pluginMetaDataSet.end() )
        {
            TPluginMetaDataPtr pluginMetaData = (*n);
            
            if ( pluginTypeToLoad == pluginMetaData->GetPluginType() )
            {
                // Try to load this module since it is of the type we are looking for
                success = success & LoadPlugin( pluginMetaData ,
                                                pluginGroup    ,
                                                (*i).first     );
            }
            ++n;
        }
    }
    g_mutex.Unlock();
    
    return success;    
}

/*-------------------------------------------------------------------------*/

bool 
CPluginControl::LoadPluginGroup( const CString& groupName )
{GUCEF_TRACE;

    g_mutex.Lock();
    bool success = true;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Loading all modules in group \"" + groupName + '\"' );
    
    // Loop trough groups looking for modules of the given type
    TPluginGroupMap::iterator i = m_pluginGroups.find( groupName );
    if ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        CPluginGroup::TPluginMetaDataSet& pluginMetaDataSet = pluginGroup.GetPluginMetaData();

        CPluginGroup::TPluginMetaDataSet::iterator n = pluginMetaDataSet.begin();
        while ( n != pluginMetaDataSet.end() )
        {
            TPluginMetaDataPtr pluginMetaData = (*n);

            // Try to load this module since it is of the type we are looking for
            success = success & LoadPlugin( pluginMetaData ,
                                            pluginGroup    ,
                                            (*i).first     );
            ++n;
        }
    }
    g_mutex.Unlock();
    
    return success; 
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::LoadAll( void )
{GUCEF_TRACE;

    g_mutex.Lock();
    bool success = true;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Loading all modules" );

    // Loop trough all groups
    TPluginGroupMap::iterator i = m_pluginGroups.begin();
    while ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        CPluginGroup::TPluginMetaDataSet& pluginMetaDataSet = pluginGroup.GetPluginMetaData();

        // Load all metadata as a plugin
        CPluginGroup::TPluginMetaDataSet::iterator n = pluginMetaDataSet.begin();
        while ( n != pluginMetaDataSet.end() )
        {
            TPluginMetaDataPtr pluginMetaData = (*n);

            // Try to load this module since it is of the type we are looking for
            success = success & LoadPlugin( pluginMetaData ,
                                            pluginGroup    ,
                                            (*i).first     );
            ++n;
        }
        ++i;
    }
    g_mutex.Unlock();
    
    return success; 
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::LoadPlugin( TPluginMetaDataPtr& pluginMetaData ,
                            CPluginGroup& pluginGroup          ,
                            const CString& groupName           )
{GUCEF_TRACE;

    // Check to see whether the module has a specific loader logic type defined
    // if not we will use the default.
    CString pluginLoaderLogicToUse = pluginMetaData->GetLoaderLogicTypeName().IsNULLOrEmpty() ? 
            m_defaultPluginLoadLogicType :  pluginMetaData->GetLoaderLogicTypeName();
                
    TPluginLoadLogicMap::iterator m = m_pluginLoadLogicProviders.find( pluginLoaderLogicToUse );
    if ( m != m_pluginLoadLogicProviders.end() )
    {
        // Check if the module has its own full path defined
        void* modulePtr = NULL;
        if ( pluginMetaData->GetFullModulePath().IsNULLOrEmpty() )
        {
            // Use the globally defined module root dirs to try and load the module
            TVersion versionInfo = pluginMetaData->GetVersion();
            TStringSet::iterator i = m_rootDirs.begin();
            while ( i != m_rootDirs.end() && modulePtr == NULL )
            {
                modulePtr = (*m).second->LoadPlugin( (*i)                                ,
                                                     pluginMetaData->GetModuleFilename() ,
                                                     groupName                           ,
                                                     &versionInfo                        );
                ++i;
            }
        }
        else
        {
            // Use the path defined with the plugin as the root to load the module from
            TVersion versionInfo = pluginMetaData->GetVersion();
            modulePtr = (*m).second->LoadPlugin( pluginMetaData->GetFullModulePath() ,
                                                 pluginMetaData->GetModuleFilename() ,
                                                 groupName                           ,
                                                 &versionInfo                        );
        }

        if ( NULL == modulePtr )
        {
            // We were not able to load the module, no point in proceeding
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "PluginControl: Failed to load module with name \"" +
                                    pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                    VersionToString( pluginMetaData->GetVersion() ) );
            return false;
        }

        // We loaded the module
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Loaded module with name \"" +
                                pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                VersionToString( pluginMetaData->GetVersion() ) );

        // Now that the module itself has been successfully loaded we can hand of the module specifics to the appropriote
        // plugin manager which in turn can check and load all symbols etc. The plugin manager will hand us a CIPlugin based
        // object in exchange for our module pointer upon a successfull registration.
        CString pluginType = pluginMetaData->GetPluginType();
        if ( pluginType.IsNULLOrEmpty() )
        {
            // Without a plugin type predefined we will have to use trial and error
            
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Module with name \"" +
                                    pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                    VersionToString( pluginMetaData->GetVersion() ) + 
                                    " does not have a plugin type defined, an attempt will be made to register at any of the available plugin managers via trial and error" );

            TPluginManagerSet::iterator w = m_pluginManagers.begin();
            while ( w != m_pluginManagers.end() )
            {
                // This plugin manager can handle a module of this type.
                // We will try to register the module here as a plugin
                TPluginPtr pluginPtr = (*w)->RegisterPlugin( modulePtr, pluginMetaData );
                if ( NULL != pluginPtr )
                {
                    // Remove the metadata entry and instead add a plugin entry
                    // Note that it is the plugin managers responsibility to correctly transfer 
                    // metadata to the plugin object
                    pluginGroup.GetPluginMetaData().erase( pluginMetaData );
                    pluginGroup.GetPlugins().insert( pluginPtr );

                    // We registerd the module
                    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Registered module with name \"" +
                                            pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                            VersionToString( pluginMetaData->GetVersion() ) + " with plugin manager of type \"" + (*w)->GetPluginType() + "\"" );

                    return true;
                }
                ++w;
            }

            // We were not able to register the module with any of the managers even though we could load it
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "PluginControl: Failed to register module with name \"" +
                                    pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                    VersionToString( pluginMetaData->GetVersion() ) + " with any plugin manager" );

            (*m).second->UnloadPlugin( modulePtr);
            return false;
        }
        else
        {
            // A plugin type was defined so we know what kind of manager we want to use
            CPluginManager* pluginManager = GetPluginManagerForType( pluginType );
            if ( pluginManager != NULL )
            {
                // This plugin manager can handle a module of this type.
                // We will try to register the module here as a plugin
                TPluginPtr pluginPtr = pluginManager->RegisterPlugin( modulePtr, pluginMetaData );
                if ( NULL != pluginPtr )
                {
                    // Remove the metadata entry and instead add a plugin entry
                    // Note that it is the plugin managers responsibility to correctly transfer 
                    // metadata to the plugin object
                    pluginGroup.GetPluginMetaData().erase( pluginMetaData );
                    pluginGroup.GetPlugins().insert( pluginPtr );

                    // We registerd the module
                    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Registered module with name \"" +
                                            pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                            VersionToString( pluginMetaData->GetVersion() ) + " with plugin manager of type \"" + pluginType + "\"" );

                    return true;
                }

                // We were not able to register the module even though we could load it
                GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "PluginControl: Failed to register module with name \"" +
                                        pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                        VersionToString( pluginMetaData->GetVersion() ) + " with plugin manager of type \"" + pluginType + "\"" );
            }

            (*m).second->UnloadPlugin( modulePtr);
            return false;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CPluginManager*
CPluginControl::GetPluginManagerForType( const CString& pluginType )
{GUCEF_TRACE;

    TPluginManagerSet::iterator w = m_pluginManagers.begin();
    while ( w != m_pluginManagers.end() )
    {
        if ( (*w)->GetPluginType() == pluginType )
        {
            return (*w);
        }
        ++w;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::UnloadPlugin( TPluginPtr& pluginPtr     ,
                              CPluginGroup& pluginGroup ,
                              const CString& groupName  )
{GUCEF_TRACE;

    TPluginMetaDataPtr pluginMetaData =  pluginPtr->GetMetaData();
    if ( NULL == pluginMetaData )
    {
        // This plugin does not have any meta data set thus we cannot unload it
        // It will have to stay in process memmory
        return false;
    }

    // First we unregister the plugin with its plugin manager
    CString pluginType = pluginMetaData->GetPluginType();
    CPluginManager* pluginManager = GetPluginManagerForType( pluginType );
    if ( pluginManager != NULL )
    {
        // This plugin manager can handle a module of this type.
        // We will try to unregister the module here as a plugin
        pluginManager->UnregisterPlugin( pluginPtr );

        // We unregisterd the module
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Unregistered module with name \"" +
                                pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                VersionToString( pluginMetaData->GetVersion() ) + " with plugin manager of type \"" + pluginType + "\"" );

    }
    else
    {
        // this should not happen but unload the module anyway
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "PluginControl: Failed to find a plugin manager capable of handling type \"" + pluginType + "\" when unregistering module with name \"" +
                            pluginMetaData->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " + VersionToString( pluginMetaData->GetVersion() ) );
    }

    // Remove the plugin entry and instead add a metadata plugin entry
    // Note that we make a copy since the interface we have might be pointing to a implementation which needs
    // the module to be loaded which is the plugin managers right to do so.
    TPluginMetaDataPtr metaDataCopyPtr = new CPluginMetaData( *pluginMetaData );
    pluginGroup.GetPlugins().erase( pluginPtr );
    pluginGroup.GetPluginMetaData().insert( metaDataCopyPtr );
    
    // Check to see whether the module has a specific loader logic type defined
    // if not we will use the default.
    CString pluginLoaderLogicToUse = metaDataCopyPtr->GetLoaderLogicTypeName().IsNULLOrEmpty() ? 
            m_defaultPluginLoadLogicType :  metaDataCopyPtr->GetLoaderLogicTypeName();
                
    TPluginLoadLogicMap::iterator m = m_pluginLoadLogicProviders.find( pluginLoaderLogicToUse );
    if ( m != m_pluginLoadLogicProviders.end() )
    {
        (*m).second->UnloadPlugin( pluginPtr->GetModulePointer() );

        // We will unload the module
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Unloaded module with name \"" +
                                metaDataCopyPtr->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " +
                                VersionToString( metaDataCopyPtr->GetVersion() ) + " using plugin loader logic of type \"" + pluginLoaderLogicToUse + '\"' );
        return true;
    }

    GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "PluginControl: Failed to find plugin loader logic of type \"" + pluginLoaderLogicToUse + "\" for unloading a plugn of type \"" + pluginType + "\" and name \"" +
                        metaDataCopyPtr->GetModuleFilename() + "\" and group \"" + groupName + "\" and version " + VersionToString( metaDataCopyPtr->GetVersion() ) );    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::UnloadAllPluginsOfTypeInGroup( const CString& pluginTypeToLoad ,
                                               const CString& groupName        )
{GUCEF_TRACE;

    g_mutex.Lock();
    bool success = true;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Unloading all modules in group \"" + groupName + "\" of type \"" + pluginTypeToLoad + '\"' );
    
    // Find the groups specified
    TPluginGroupMap::iterator i = m_pluginGroups.find( groupName );
    if ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        CPluginGroup::TPluginSet& pluginSet = pluginGroup.GetPlugins();

        // Look for a plugin of the given type
        CPluginGroup::TPluginSet::iterator n = pluginSet.begin();
        while ( n != pluginSet.end() )
        {
            TPluginPtr plugin = (*n);
            TPluginMetaDataPtr pluginMetaData = plugin->GetMetaData();
            
            if ( NULL != pluginMetaData )
            {
                if ( pluginTypeToLoad == pluginMetaData->GetPluginType() )
                {
                    // Try to unload this module since it is of the type we are looking for
                    success = success & UnloadPlugin( plugin      ,
                                                      pluginGroup ,
                                                      (*i).first  );
                }
            }
            ++n;
        }
    }
    g_mutex.Unlock();
    
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::UnloadAllPluginsOfType( const CString& pluginTypeToUnload )
{GUCEF_TRACE;

    g_mutex.Lock();
    bool success = true;
    
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Unloading all modules of type \"" + pluginTypeToUnload + '\"' );
    
    // Loop trough groups looking for modules of the given type
    TPluginGroupMap::iterator i = m_pluginGroups.begin();
    while ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        CPluginGroup::TPluginSet& pluginSet = pluginGroup.GetPlugins();

        CPluginGroup::TPluginSet::iterator n = pluginSet.begin();
        while ( n != pluginSet.end() )
        {
            TPluginPtr plugin = (*n);
            TPluginMetaDataPtr pluginMetaData = plugin->GetMetaData();
            
            if ( NULL != pluginMetaData )
            {
                if ( pluginTypeToUnload == pluginMetaData->GetPluginType() )
                {
                    // Try to unload this module since it is of the type we are looking for
                    success = success & UnloadPlugin( plugin      ,
                                                      pluginGroup ,
                                                      (*i).first  );
                }
            }
            ++n;
        }
        ++i;
    }
    g_mutex.Unlock();

    return success;
}

/*-------------------------------------------------------------------------*/
    
bool
CPluginControl::UnloadPluginGroup( const CString& groupName )
{GUCEF_TRACE;

    g_mutex.Lock();
    bool success = true;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Unloading all modules in group \"" + groupName + '\"' );
    
    // Loop trough groups looking for modules of the given type
    TPluginGroupMap::iterator i = m_pluginGroups.find( groupName );
    if ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        CPluginGroup::TPluginSet& pluginSet = pluginGroup.GetPlugins();

        CPluginGroup::TPluginSet::iterator n = pluginSet.begin();
        while ( n != pluginSet.end() )
        {
            TPluginPtr plugin = (*n);

            // Try to unload this module since it is of the type we are looking for
            success = success & UnloadPlugin( plugin      ,
                                              pluginGroup ,
                                              (*i).first  );
            ++n;
        }
    }
    g_mutex.Unlock();
    
    return success; 
}

/*-------------------------------------------------------------------------*/
        
bool 
CPluginControl::UnloadAll( void )
{GUCEF_TRACE;

    g_mutex.Lock();
    bool success = true;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Unloading all modules" );

    // Loop trough all groups
    TPluginGroupMap::iterator i = m_pluginGroups.begin();
    while ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        CPluginGroup::TPluginSet& pluginSet = pluginGroup.GetPlugins();

        CPluginGroup::TPluginSet::iterator n = pluginSet.begin();
        while ( n != pluginSet.end() )
        {
            TPluginPtr plugin = (*n);

            // Try to load this module since it is of the type we are looking for
            success = success & UnloadPlugin( plugin      ,
                                              pluginGroup ,
                                              (*i).first  );
            ++n;
        }
        ++i;
    }
    g_mutex.Unlock();
    
    return success;
}

/*-------------------------------------------------------------------------*/

void
CPluginControl::ClearMetaDataFromGroup( const CString& groupName )
{GUCEF_TRACE;

    g_mutex.Lock();
    TPluginGroupMap::iterator i = m_pluginGroups.find( groupName );
    if ( i != m_pluginGroups.end() )
    {
        CPluginGroup& pluginGroup = (*i).second;
        pluginGroup.GetPluginMetaData().clear();
        
        // If no plugins are loaded & we have no metadata then get rid of the group alltogether
        if ( pluginGroup.GetPlugins().empty() )
        {
            m_pluginGroups.erase( i );
        }
    }
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CPluginControl::GetSupportedPluginTypes( TStringVector& supportedTypes ) const
{GUCEF_TRACE;

    g_mutex.Lock();
    TPluginManagerSet::iterator i = m_pluginManagers.begin();
    while ( i != m_pluginManagers.end() )
    {
        supportedTypes.push_back( (*i)->GetPluginType() );        
        ++i;
    }
    g_mutex.Unlock();    
}

/*-------------------------------------------------------------------------*/

void 
CPluginControl::Register( CPluginManager* pman )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_pluginManagers.insert( pman );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/
        
void 
CPluginControl::Unregister( CPluginManager* pman )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_pluginManagers.erase( pman );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CPluginControl::RegisterPluginLoadLogic( CIPluginLoadLogic* pluginLoaderLogic )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_pluginLoadLogicProviders[ pluginLoaderLogic->GetLoaderLogicTypeName() ] = pluginLoaderLogic;
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CPluginControl::UnregisterPluginLoadLogic( const CString& loaderLogicTypeName )
{GUCEF_TRACE;

    g_mutex.Lock();
    TPluginLoadLogicMap::iterator i = m_pluginLoadLogicProviders.find( loaderLogicTypeName );
    if ( i != m_pluginLoadLogicProviders.end() )
    {
        m_pluginLoadLogicProviders.erase( i );
    }
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CPluginControl::SetDefaultPluginLoadLogicType( const CString& loaderLogicTypeName )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_defaultPluginLoadLogicType = loaderLogicTypeName;
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

CString
CPluginControl::GetDefaultPluginLoadLogicType( void ) const
{GUCEF_TRACE;
    
    g_mutex.Lock();
    CString returnValue = m_defaultPluginLoadLogicType;
    g_mutex.Unlock();
    return returnValue;
}

/*-------------------------------------------------------------------------*/

void
CPluginControl::AddPluginDir( const CString& path )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_rootDirs.insert( RelativePath( path ) );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CPluginControl::RemovePluginDir( const CString& path )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_rootDirs.erase( RelativePath( path ) );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::AddPluginFromDir( const CString& pluginPath ,
                                  const CString& groupName  ,
                                  bool loadImmediatly       )
{GUCEF_TRACE;

    CString pathToPlugin = RelativePath( pluginPath );
    
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Defining file " + pathToPlugin + " as a plugin" );

    // All we can define using this method is the directory and the filename
    // The module will have to be matched to a plugin manager via trial and error
    CString filename = ExtractFilename( pathToPlugin );
    CPluginMetaData pluginMetaData;
    pluginMetaData.SetFullModulePath( pathToPlugin.CutChars( filename.Length(), false ) );
    pluginMetaData.SetModuleFilename( filename );
                 
    return AddPluginMetaData( pluginMetaData , 
                              groupName      ,
                              loadImmediatly );
}

/*-------------------------------------------------------------------------*/

bool 
CPluginControl::AddAllPluginsFromDir( const CString& pluginDir ,
                                      const CString& groupName ,
                                      bool loadImmediatly      )
{GUCEF_TRACE;

    bool success = true;
    struct SDI_Data* did = DI_First_Dir_Entry( pluginDir.C_String() );
    if ( did )
    {
        do 
        {                
            if ( DI_Is_It_A_File( did ) == 1 )
            {
                CString filename = DI_Name( did );
                CString fileExt = ExtractFileExtention( filename );

                #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
                if ( fileExt.Lowercase() == "dll" )
                #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
                if ( fileExt.Lowercase() == "so" )
                #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_MACOS )
                if ( fileExt.Lowercase() == "dynlib" ) 
                #endif
                {
                    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "PluginControl: Defining file " + filename + " as a plugin" );
                    
                    // All we can define using this method is the directory and the filename
                    // The module will have to be matched to a plugin manager via trial and error
                    CPluginMetaData pluginMetaData;
                    pluginMetaData.SetFullModulePath( pluginDir );
                    pluginMetaData.SetModuleFilename( filename );
                 
                    success = success && AddPluginMetaData( pluginMetaData , 
                                                            groupName      ,
                                                            loadImmediatly );
                }
            }
        }
        while ( DI_Next_Dir_Entry( did ) != 0 );
        
        DI_Cleanup( did );
    }

    return success;
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::AddPluginMetaData( const CIPluginMetaData& pluginMetaData ,
                                   const CString& groupName               ,
                                   bool loadImmediatly                    )
{GUCEF_TRACE;

    bool success = true;
    
    // Add the metadata
    g_mutex.Lock();
    TPluginMetaDataPtr pluginMetaDataCopy = new CPluginMetaData( pluginMetaData );
    CPluginGroup& pluginGroup = m_pluginGroups[ groupName ];
    pluginGroup.GetPluginMetaData().insert( pluginMetaDataCopy );
    
    // If an immediate load is desired we invoke the load function as well
    if ( loadImmediatly )
    {
        success = LoadPlugin( pluginMetaDataCopy ,
                              pluginGroup        ,
                              groupName          );
    }

    g_mutex.Unlock();
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::SaveConfig( CDataNode& tree )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPluginControl::LoadConfig( const CDataNode& treeroot )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
