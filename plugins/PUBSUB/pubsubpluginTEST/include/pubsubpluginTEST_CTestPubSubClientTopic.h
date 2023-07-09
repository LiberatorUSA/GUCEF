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

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPIC_H
#include "pubsubpluginSTORAGE_CStoragePubSubClientTopic.h"
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPIC_H
#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPIC_H ? */

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H
#include "pubsubpluginTEST_CTestPubSubClientTopicConfig.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H ? */

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

class PUBSUBPLUGIN_TEST_PLUGIN_PRIVATE_CPP CTestPubSubClientTopic : public STORAGE::CStoragePubSubClientTopic ,
                                                                    public CORE::CTSharedObjCreator< CTestPubSubClientTopic, MT::CMutex >
{
    public:

    CTestPubSubClientTopic( CTestPubSubClient* client              ,
                            CORE::PulseGeneratorPtr pulseGenerator );

    virtual ~CTestPubSubClientTopic() GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsHealthy( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( PUBSUB::CPubSubClientTopicConfig& config ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const PUBSUB::CPubSubClientTopicConfig& config ) GUCEF_VIRTUAL_OVERRIDE;

    const CTestPubSubClientTopicConfig& GetTopicConfig( void ) const;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual void Shutdown( void );

    private:

    typedef CORE::CTEventHandlerFunctor< CTestPubSubClientTopic > TEventCallback;

    void RegisterEventHandlers( void );

    void
    OnHealthyStatusToggleTimerCycle( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventId  ,
                                     CORE::CICloneable* eventData );


    mutable bool m_hasFakeHealthIssue;
    mutable CORE::CTimer m_healthyStatusToggleTimer;
    CTestPubSubClientTopicConfig m_testConfig;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedObjCreator< CTestPubSubClientTopic, MT::CMutex >::TSharedPtrType  CTestPubSubClientTopicPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace TEST */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H ? */
