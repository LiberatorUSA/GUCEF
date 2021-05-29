/*
 *  pubsub2storage: service which transfers between pubsub and storage
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <deque>

#ifndef GUCEF_MT_CTMAILBOX_H
#include "gucefMT_CTMailBox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#include "gucefCORE_CDynamicBufferSwap.h"
#define GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERSWAP_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENT_H
#include "gucefCOMCORE_CPubSubClient.h"
#define GUCEF_COMCORE_CPUBSUBCLIENT_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#include "gucefCOMCORE_CPubSubClientFactory.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefCOMCORE_CPubSubMsgBinarySerializer.h"
#define GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H ? */

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class ChannelSettings : public CORE::CIConfigurable
{
    public:

    typedef std::vector< COMCORE::CPubSubClientTopicConfig > TTopicConfigVector;

    ChannelSettings( void );
    ChannelSettings( const ChannelSettings& src );
    ChannelSettings& operator=( const ChannelSettings& src );

    COMCORE::CPubSubClientConfig pubsubClientConfig;
    TTopicConfigVector pubsubClientTopicConfigs;
    COMCORE::CPubSubMsgBinarySerializerOptions pubsubBinarySerializerOptions;
    CORE::UInt32 desiredMinimalSerializedBlockSize;
    CORE::UInt32 desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs;    
    CORE::CString vfsStoragePath;
    CORE::CString encodeCodecFamily;
    CORE::CString encodeCodecName;
    CORE::Int32 channelId;
    CORE::UInt32 ticketRefillOnBusyCycle;
    bool performPubSubInDedicatedThread;
    CORE::Int32 maxSizeOfDedicatedPubSubBulkMailRead;
    bool applyThreadCpuAffinity;
    CORE::UInt32 cpuAffinityForDedicatedPubSubThread;
    CORE::UInt32 cpuAffinityForMainChannelThread;
    bool collectMetrics;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

class CPubSubClientChannel : public CORE::CTaskConsumer
{
    public:

    typedef CORE::CTEventHandlerFunctor< CPubSubClientChannel > TEventCallback;

    CPubSubClientChannel();
    virtual ~CPubSubClientChannel();

    virtual bool OnTaskStart( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                               bool wasForced             ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    bool LoadConfig( const ChannelSettings& channelSettings );

    const ChannelSettings& GetChannelSettings( void ) const;

    bool ConnectPubSubClient( void );

    bool DisconnectPubSubClient( bool destroyClient = false );

    CORE::CDynamicBufferSwap& GetSerializedMsgBuffers( void );

    private:

    void RegisterEventHandlers( void );

    void RegisterTopicEventHandlers( COMCORE::CPubSubClientTopic& topic );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnPubSubClientReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData );

    void
    OnPubSubTopicMsgsReceived( CORE::CNotifier* notifier    ,
                               const CORE::CEvent& eventId  ,
                               CORE::CICloneable* eventData );

    private:

    CPubSubClientChannel( const CPubSubClientChannel& src ); // not implemented

    typedef std::vector< COMCORE::CPubSubClientTopic* > TopicVector;
    typedef MT::CTMailBox< CORE::UInt32 > TBufferMailbox;

    COMCORE::CPubSubClientPtr m_pubsubClient;
    TopicVector m_topics;
    ChannelSettings m_channelSettings;
    TBufferMailbox m_mailbox;
    TBufferMailbox::TMailList m_bulkMail;
    CORE::CTimer* m_metricsTimer;
    CORE::CTimer* m_pubsubClientReconnectTimer;
    CORE::CDynamicBufferSwap m_buffers;
    CORE::CDynamicBuffer* m_msgReceiveBuffer;
    CORE::CDateTime m_lastWriteBlockCompletion;    
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CPubSubClientChannel, MT::CMutex > CPubSubClientChannelPtr;

/*-------------------------------------------------------------------------*/

class CStorageChannel : public CORE::CTaskConsumer
{
    public:

    typedef CORE::CTEventHandlerFunctor< CStorageChannel > TEventCallback;

    CStorageChannel();
    CStorageChannel( const CStorageChannel& src );
    virtual ~CStorageChannel();

    virtual bool OnTaskStart( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                              bool wasForced              ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool WaitForTaskToFinish( CORE::Int32 timeoutInMs ) GUCEF_VIRTUAL_OVERRIDE;

    bool LoadConfig( const ChannelSettings& channelSettings );

    const ChannelSettings& GetChannelSettings( void ) const;

    class ChannelMetrics
    {
        public:

        ChannelMetrics( void );

        CORE::UInt32 udpBytesReceived;
        CORE::UInt32 udpPacketsReceived;
        CORE::UInt32 redisMessagesTransmitted;
        CORE::UInt32 redisPacketsInMsgsTransmitted;
        CORE::UInt32 redisPacketsInMsgsRatio;
        CORE::UInt32 redisTransmitQueueSize;
        CORE::UInt32 redisErrorReplies;
    };

    const ChannelMetrics& GetMetrics( void ) const;

    private:

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    private:

    void RegisterEventHandlers( void );

    private:

    ChannelSettings m_channelSettings;
    CORE::CTimer* m_metricsTimer;
    ChannelMetrics m_metrics;
    CPubSubClientChannelPtr m_pubsubClient;
    CORE::CDynamicBuffer* m_msgReceiveBuffer;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CStorageChannel, MT::CMutex > CStorageChannelPtr;

/*-------------------------------------------------------------------------*/

class PubSub2Storage;

class RestApiPubSub2StorageInfoResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiPubSub2StorageInfoResource( PubSub2Storage* app );

    virtual ~RestApiPubSub2StorageInfoResource();

    virtual bool Serialize( const CORE::CString& resourcePath   ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    PubSub2Storage* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiPubSub2StorageConfigResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiPubSub2StorageConfigResource( PubSub2Storage* app, bool appConfig );

    virtual ~RestApiPubSub2StorageConfigResource();

    virtual bool Serialize( const CORE::CString& resourcePath   ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    virtual TDeserializeState Deserialize( const CORE::CString& resourcePath   ,
                                           const CORE::CDataNode& input        ,
                                           const CORE::CString& representation ,
                                           bool isDeltaUpdateOnly              ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    PubSub2Storage* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class PubSub2Storage : public CORE::CObserver ,
                       public CORE::CIConfigurable
{
    public:

    PubSub2Storage( void );
    virtual ~PubSub2Storage();

    bool Start( void );

    bool SetStandbyMode( bool newMode );

    bool IsGlobalStandbyEnabled( void ) const;

    bool LoadConfig( const CORE::CValueList& appConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    typedef CORE::CTEventHandlerFunctor< PubSub2Storage > TEventCallback;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    private:

    typedef std::map< CORE::Int32, ChannelSettings > ChannelSettingsMap;
    typedef std::map< CORE::Int32, CStorageChannelPtr > StorageChannelMap;
    typedef std::map< CORE::CString, CORE::CDataNode::TConstDataNodeSet > TChannelCfgMap;

    bool m_isInStandby;
    bool m_globalStandbyEnabled;
    CORE::UInt16 m_udpStartPort;
    CORE::UInt16 m_channelCount;
    CORE::Int32 m_storageStartChannelID;
    CORE::CString m_redisStreamName;
    CORE::CString m_redisHost;
    CORE::UInt16 m_redisPort;
    StorageChannelMap m_channels;
    ChannelSettingsMap m_channelSettings;
    ChannelSettings m_templateChannelSettings;
    WEB::CHTTPServer m_httpServer;
    WEB::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;
    CORE::CTimer m_metricsTimer;
    bool m_transmitMetrics;
};

/*-------------------------------------------------------------------------*/
