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

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_CORE_CTHREADPOOL_H
#include "gucefCORE_CThreadPool.h"
#define GUCEF_CORE_CTHREADPOOL_H
#endif /* GUCEF_CORE_CTHREADPOOL_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENT_H
#include "gucefPUBSUB_CPubSubClient.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENT_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H
#include "gucefPUBSUB_CPubSubClientFactory.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H ? */

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H
#include "pubsubpluginTEST_CTestPubSubClientTopic.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H ? */

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H
#include "pubsubpluginTEST_CTestPubSubClientConfig.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H ? */

#ifndef GUCEF_WEB_CHTTPSERVER_H
#include "gucefWEB_CHTTPServer.h"
#define GUCEF_WEB_CHTTPSERVER_H
#endif /* GUCEF_WEB_CHTTPSERVER_H ? */

#ifndef GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H
#include "gucefWEB_CDefaultHTTPServerRouter.h"
#define GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H
#endif /* GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H ? */

#ifndef GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefWEB_CCodecBasedHTTPServerResource.h"
#define GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_WEB_CTCONFIGURABLEMAPHTTPSERVERRESOURCE_H
#include "gucefWEB_CTConfigurableMapHttpServerResource.h"
#define GUCEF_WEB_CTCONFIGURABLEMAPHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CTCONFIGURABLEMAPHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefWEB_CDummyHTTPServerResource.h"
#define GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_WEB_CTREADABLEMAPINDEXHTTPSERVERRESOURCE_H
#include "gucefWEB_CTReadableMapIndexHttpServerResource.h"
#define GUCEF_WEB_CTREADABLEMAPINDEXHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CTREADABLEMAPINDEXHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_WEB_CTDATANODESERIALIZABLEMAPHTTPSERVERRESOURCE_H
#include "gucefWEB_CTDataNodeSerializableMapHttpServerResource.h"
#define GUCEF_WEB_CTDATANODESERIALIZABLEMAPHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CTDATANODESERIALIZABLEMAPHTTPSERVERRESOURCE_H ? */

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

class PUBSUBPLUGIN_TEST_PLUGIN_PRIVATE_CPP CTestPubSubClient : public PUBSUB::CPubSubClient
{
    public:

    static const CORE::CString TypeName; 

    CTestPubSubClient( const PUBSUB::CPubSubClientConfig& config );

    virtual ~CTestPubSubClient() GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetSupportedFeatures( PUBSUB::CPubSubClientFeatures& features ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual const PUBSUB::CPubSubClientTopicConfig* GetTopicConfig( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual PUBSUB::CPubSubClientTopic* CreateTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig ) GUCEF_VIRTUAL_OVERRIDE;

    virtual PUBSUB::CPubSubClientTopic* GetTopicAccess( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void DestroyTopicAccess( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool BeginTopicDiscovery( const CORE::CString::StringSet& globPatternFilters = CORE::CString::EmptyStringSet ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual void GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void GetAllCreatedTopicAccess( PubSubClientTopicSet& topicAccess ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Connect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsHealthy( void ) const GUCEF_VIRTUAL_OVERRIDE;

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

    CTestPubSubClientConfig& GetConfig( void );

    CORE::ThreadPoolPtr GetThreadPool( void );

    protected:

    virtual bool Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnTopicHealthStatusChange( CORE::CNotifier* notifier    ,
                               const CORE::CEvent& eventId  ,
                               CORE::CICloneable* eventData );
    
    void RegisterEventHandlers( void );

    void RegisterTopicEventHandlers( PUBSUB::CPubSubClientTopic* topic );
    
    CTestPubSubClient( void ); /**< not implemented */

    private:

    typedef CORE::CTEventHandlerFunctor< CTestPubSubClient > TEventCallback;
    typedef std::map< CORE::CString, CTestPubSubClientTopic* > TTopicMap;

    CTestPubSubClientConfig m_config;
    CORE::CTimer* m_metricsTimer;
    TTopicMap m_topicMap;
    CORE::ThreadPoolPtr m_threadPool;
    mutable bool m_isHealthy;
    WEB::CHTTPServer m_httpServer;
    WEB::CDefaultHTTPServerRouter m_httpRouter;
    MT::CMutex m_lock;
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
