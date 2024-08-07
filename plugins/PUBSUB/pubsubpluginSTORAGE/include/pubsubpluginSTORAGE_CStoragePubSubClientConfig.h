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

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTCONFIG_H
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTCONFIG_H
#include "gucefPUBSUB_CPubSubClientConfig.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTCONFIG_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTCONFIG_H ? */

#ifndef PUBSUBPLUGIN_STORAGE_MACROS_H
#include "pubsubpluginSTORAGE_macros.h"
#define PUBSUBPLUGIN_STORAGE_MACROS_H
#endif /* PUBSUBPLUGIN_STORAGE_MACROS_H ? */

#ifndef GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H
#include "gucefPUBSUB_CIPubSubBookmarkPersistence.h"
#define GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H
#endif /* GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H ? */

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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CStoragePubSubClient;

/**
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_STORAGE_PLUGIN_PRIVATE_CPP CStoragePubSubClientConfig : public PUBSUB::CPubSubClientConfig
{
    public:
    
    PUBSUB::CPubSubBookmarkPersistenceConfig pubsubBookmarkPersistenceConfig;
    CORE::CString vfsStorageRootPath;
    bool dirTopicDiscoveryIsRecursive;
    bool includeDirParentInTopicName;
    bool destroyTopicWhenDirIsDeleted;
    
    CStoragePubSubClientConfig( void );
    
    CStoragePubSubClientConfig( const PUBSUB::CPubSubClientConfig& genericConfig );

    virtual ~CStoragePubSubClientConfig() GUCEF_VIRTUAL_OVERRIDE;

    CStoragePubSubClientConfig& operator=( const PUBSUB::CPubSubClientConfig& src );

    CStoragePubSubClientConfig& operator=( const CStoragePubSubClientConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );

    bool SaveCustomConfig( CORE::CDataNode& config ) const;

    bool SerializeCustomConfigToGenericConfig( void );

    bool LoadConfig( const CORE::CDataNode& config );

    bool SaveConfig( CORE::CDataNode& config ) const;

    bool LoadConfig( const PUBSUB::CPubSubClientConfig& cfg );

    bool SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace STORAGE */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTCONFIG_H ? */
