/*
 *  pubsubpluginUDP: Generic GUCEF COMCORE plugin for providing pubsub approximation via UDP
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

#ifndef PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H
#define PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H

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

#ifndef GUCEF_COMCORE_CPUBSUBCLIENT_H
#include "gucefCOMCORE_CPubSubClient.h"
#define GUCEF_COMCORE_CPUBSUBCLIENT_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#include "gucefCOMCORE_CPubSubClientFactory.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H ? */

#ifndef PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPIC_H
#include "pubsubpluginUDP_CUdpPubSubClientTopic.h"
#define PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPIC_H
#endif /* PUBSUBPLUGIN_UDP_CMSMQPUBSUBCLIENTTOPIC_H ? */

#ifndef PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTCONFIG_H
#include "pubsubpluginUDP_CUdpPubSubClientConfig.h"
#define PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTCONFIG_H
#endif /* PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTCONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace UDP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class PUBSUBPLUGIN_UDP_PLUGIN_PRIVATE_CPP CUdpPubSubClient : public COMCORE::CPubSubClient
{
    public:

    static const CORE::CString TypeName; 

    CUdpPubSubClient( const COMCORE::CPubSubClientConfig& config );

    virtual ~CUdpPubSubClient() GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetSupportedFeatures( COMCORE::CPubSubClientFeatures& features ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const COMCORE::CPubSubClientTopicConfig* GetTopicConfig( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubClientTopic* CreateTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig ) GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubClientTopic* GetTopicAccess( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void DestroyTopicAccess( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Connect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;

    CUdpPubSubClientConfig& GetConfig( void );

    private:

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void RegisterEventHandlers( void );
    
    CUdpPubSubClient( void ); /**< not implemented */

    private:

    typedef CORE::CTEventHandlerFunctor< CUdpPubSubClient > TEventCallback;
    typedef std::map< CORE::CString, CUdpPubSubClientTopic* > TTopicMap;

    CUdpPubSubClientConfig m_config;
    CORE::CTimer* m_metricsTimer;
    TTopicMap m_topicMap;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace UDP */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H ? */
