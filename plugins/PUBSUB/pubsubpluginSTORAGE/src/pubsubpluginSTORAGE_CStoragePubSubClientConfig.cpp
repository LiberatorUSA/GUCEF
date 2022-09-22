/*
 *  pubsubpluginSTORAGE: Generic GUCEF plugin for PUBSUB module functionality 
 *                       providing a storage interface
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_PUBSUB_CVFSPUBSUBBOOKMARKPERSISTENCE_H
#include "gucefPUBSUB_CVfsPubSubBookmarkPersistence.h"
#define GUCEF_PUBSUB_CVFSPUBSUBBOOKMARKPERSISTENCE_H
#endif /* GUCEF_PUBSUB_CVFSPUBSUBBOOKMARKPERSISTENCE_H ? */

#include "pubsubpluginSTORAGE_CStoragePubSubClientConfig.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace STORAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CStoragePubSubClientConfig::CStoragePubSubClientConfig( void )
    : PUBSUB::CPubSubClientConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientConfig::CStoragePubSubClientConfig( const PUBSUB::CPubSubClientConfig& genericConfig )
    : PUBSUB::CPubSubClientConfig( genericConfig )
{GUCEF_TRACE;

    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientConfig::~CStoragePubSubClientConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientConfig::SaveCustomConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;
    
    CORE::CDataNode* psBookmarkPersistenceConfig = config.Structure( "PubSubBookmarkPersistenceConfig", '/' );
    if ( !pubsubBookmarkPersistenceConfig.SaveConfig( *psBookmarkPersistenceConfig ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientConfig:SaveCustomConfig: config is malformed, failed to save PubSubBookmarkPersistenceConfig section" );
        return false;
    }  

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    bool psPersistanceConfigLoaded = false;
    const CORE::CDataNode* psBookmarkPersistenceConfig = config.Search( "PubSubBookmarkPersistenceConfig", '/', false );
    if ( GUCEF_NULL != psBookmarkPersistenceConfig )
    {
        if ( pubsubBookmarkPersistenceConfig.LoadConfig( *psBookmarkPersistenceConfig ) )
        {
            psPersistanceConfigLoaded = true;
        }
    }

    if ( !psPersistanceConfigLoaded )
    {
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientConfig:LoadCustomConfig: No PubSubBookmarkPersistenceConfig section found, will fall back to VFS based default" );

        PUBSUB::CVfsPubSubBookmarkPersistenceConfig defaultBmPersistenceCfg;
        defaultBmPersistenceCfg.vfsRootPath = "InstallPath";
        defaultBmPersistenceCfg.bookmarkNamespace = pubsubIdPrefix + "/internal_bookmarks/";
        defaultBmPersistenceCfg.SaveCustomConfig( defaultBmPersistenceCfg.customConfig );

        pubsubBookmarkPersistenceConfig = defaultBmPersistenceCfg;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientConfig& 
CStoragePubSubClientConfig::operator=( const PUBSUB::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( src );
        LoadCustomConfig( src.customConfig );    
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientConfig& 
CStoragePubSubClientConfig::operator=( const CStoragePubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( src );
        pubsubBookmarkPersistenceConfig = src.pubsubBookmarkPersistenceConfig;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientConfig::SerializeCustomConfigToGenericConfig( void )
{GUCEF_TRACE;

    return SaveCustomConfig( customConfig ); 
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientConfig::LoadConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;

    if ( PUBSUB::CPubSubClientConfig::LoadConfig( config ) )
    {
        return LoadCustomConfig( customConfig ); 
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientConfig::SaveConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    PUBSUB::CPubSubClientConfig cfgCopy( *this );
    if ( SaveCustomConfig( cfgCopy.customConfig ) )
    {
        // Now that the custom config is re-integrated we can just use the base class to save the config
        return cfgCopy.SaveConfig( config ); 
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientConfig::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg )
{GUCEF_TRACE;

    // For the base class portion there is nothing to parse as its a simple
    // assignment. Its the custom config where the actual parsed loading occurs
    if ( &cfg != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( cfg );
    }
    return LoadCustomConfig( cfg.customConfig ); 
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientConfig::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
{GUCEF_TRACE;

    // For the base class portion there is nothing to do except a simple
    // assignment. Its the custom config where the actual config serialization occurs
    cfg = *this;
    return SaveCustomConfig( cfg.customConfig );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAFKA */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
