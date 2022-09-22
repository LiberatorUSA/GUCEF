/*
 *  pubsubpluginTEST: Generic GUCEF PUBSUB plugin for adding integration/system tests
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H
#include "pubsubpluginSTORAGE_CStoragePubSubClientTopicConfig.h"
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H
#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef PUBSUBPLUGIN_TEST_MACROS_H
#include "pubsubpluginTEST_macros.h"
#define PUBSUBPLUGIN_TEST_MACROS_H
#endif /* PUBSUBPLUGIN_TEST_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace TEST {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CTestPubSubClient;

/**
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_TEST_PLUGIN_PRIVATE_CPP CTestPubSubClientTopicConfig : public STORAGE::CStoragePubSubClientTopicConfig
{
    public:

    bool defaultIsHealthyStatus;
    bool toggleHealthyStatus;
    CORE::UInt32 healthyStatusToggleIntervalInMs;
    CORE::UInt32 healthyStatusToggleIntervalJitter;

    CTestPubSubClientTopicConfig( void );

    CTestPubSubClientTopicConfig( const CTestPubSubClientTopicConfig& src );
    
    CTestPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig );

    virtual ~CTestPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CTestPubSubClientTopicConfig& operator=( const PUBSUB::CPubSubClientTopicConfig& src );

    CTestPubSubClientTopicConfig& operator=( const CTestPubSubClientTopicConfig& src );

    virtual bool LoadCustomConfig( const CORE::CDataNode& config ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveCustomConfig( CORE::CDataNode& config ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace TEST */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H ? */
