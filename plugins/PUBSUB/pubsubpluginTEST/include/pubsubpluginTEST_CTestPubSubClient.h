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

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENT_H
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H
#include "pubsubpluginSTORAGE_CStoragePubSubClient.h"
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H
#endif /* #define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H ? */

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H
#include "pubsubpluginTEST_CTestPubSubClientTopic.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H ? */

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H
#include "pubsubpluginTEST_CTestPubSubClientConfig.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H ? */

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

class PUBSUBPLUGIN_TEST_PLUGIN_PRIVATE_CPP CTestPubSubClient : public STORAGE::CStoragePubSubClient
{
    public:

    static const CORE::CString TypeName; 

    CTestPubSubClient( const PUBSUB::CPubSubClientConfig& config );

    virtual ~CTestPubSubClient() GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      @param cfg the data node structure where you'd like to serialize the config to
     *      @return wheter storing the config was successfull
     */
    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      @param cfg the config storage object where you'd like to serialize the config to
     *      @return wheter storing the config was successfull
     */
    virtual bool SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      @param cfg pertinent node in the config document from which to load the config
     *      @return success or failure to load all required settings correctly from the given config
     */
    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      @param cfg the config storage object from which to load the config
     *      @return success or failure to load all required settings correctly from the given config
     */
    virtual bool LoadConfig( const PUBSUB::CPubSubClientConfig& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual PUBSUB::CPubSubClientTopic* CreateTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig ) GUCEF_VIRTUAL_OVERRIDE;

    CTestPubSubClientConfig& GetConfig( void );

    private:
                                                                               
    CTestPubSubClient( void ); /**< not implemented */

    private:

    typedef CORE::CTEventHandlerFunctor< CTestPubSubClient > TEventCallback;

    CTestPubSubClientConfig m_testConfig;
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

#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENT_H ? */
