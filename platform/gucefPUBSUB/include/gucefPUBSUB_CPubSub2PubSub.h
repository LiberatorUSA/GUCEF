/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
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

#ifndef GUCEF_PUBSUB_CPUBSUB2PUBSUB_H
#define GUCEF_PUBSUB_CPUBSUB2PUBSUB_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <deque>

#ifndef GUCEF_MT_CNOLOCK_H
#include "gucefMT_CNoLock.h"
#define GUCEF_MT_CNOLOCK_H
#endif /* GUCEF_MT_CNOLOCK_H ? */

#ifndef GUCEF_MT_CTMAILBOX_H
#include "gucefMT_CTMailBox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

#ifndef GUCEF_CORE_CTMAILBOXFORSHAREDCLONEABLES_H
#include "gucefCORE_CTMailboxForSharedCloneables.h"
#define GUCEF_CORE_CTMAILBOXFORSHAREDCLONEABLES_H
#endif /* GUCEF_CORE_CTMAILBOXFORSHAREDCLONEABLES_H ? */

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#include "gucefCORE_CGloballyConfigurable.h"
#define GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#endif /* GUCEF_CORE_CGLOBALLYCONFIGURABLE_H ? */

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

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENT_H
#include "gucefPUBSUB_CPubSubClient.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENT_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H
#include "gucefPUBSUB_CPubSubClientFactory.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgContainerBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H ? */

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

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP PubSubSideChannelSettings : public CORE::CIConfigurable
{
    public:

    PubSubSideChannelSettings( void );
    PubSubSideChannelSettings( const PubSubSideChannelSettings& src );
    PubSubSideChannelSettings& operator=( const PubSubSideChannelSettings& src );

    CPubSubClientConfig pubsubClientConfig;
    bool performPubSubInDedicatedThread;
    bool applyThreadCpuAffinity;
    CORE::UInt32 cpuAffinityForPubSubThread;
    bool subscribeWithoutBookmarkIfNoneIsPersisted;
    bool retryFailedPublishAttempts;
    bool allowOutOfOrderPublishRetry;
    CORE::Int32 maxMsgPublishRetryAttempts;
    CORE::Int32 maxMsgPublishRetryTotalTimeInMs;
    CORE::Int32 maxPublishedMsgInFlightTimeInMs;
    bool allowTimedOutPublishedInFlightMsgsRetryOutOfOrder;
    CORE::Int32 maxMsgPublishAckRetryAttempts;
    CORE::Int32 maxMsgPublishAckRetryTotalTimeInMs;
    CORE::Int64 maxTotalMsgsInFlight;
    
    bool needToTrackInFlightPublishedMsgsForAck;       //< this setting is derived and cached from other settings 
    CORE::CString metricsPrefix;                       //< this setting is derived and cached from other settings   

    CPubSubClientTopicConfig* GetTopicConfig( const CORE::CString& topicName );

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP ChannelSettings : public CORE::CIConfigurable
{
    public:

    typedef std::map< char, PubSubSideChannelSettings > TCharToPubSubSideChannelSettingsMap;

    ChannelSettings( void );
    ChannelSettings( const ChannelSettings& src );
    ChannelSettings& operator=( const ChannelSettings& src );

    PubSubSideChannelSettings* GetPubSubSideSettings( char side );

    void UpdateDerivedSettings( void );

    TCharToPubSubSideChannelSettingsMap pubSubSideChannelSettingsMap;
    CORE::Int32 channelId;
    CORE::UInt32 ticketRefillOnBusyCycle;
    bool collectMetrics;                                                   
    CORE::UInt32 metricsIntervalInMs;
    CORE::CString metricsPrefix;                                          //< this setting is derived and cached from other settings

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP CIPubSubBookmarkPersistance
{
    public:

    virtual bool GetPersistedBookmark( CORE::Int32 channelId          , 
                                       const CORE::CString& topicName , 
                                       CPubSubBookmark& bookmark      ) = 0;
};

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP CPubSubClientSide : public CORE::CTaskConsumer
{
    public:

    typedef CORE::CTEventHandlerFunctor< CPubSubClientSide > TEventCallback;
    typedef std::vector< CPubSubClientSide* >                TPubSubClientSideVector;
    
    class CPubSubClientSideMetrics
    {
        public:

        CORE::UInt64 publishedMsgsInFlight;
        CORE::UInt64 publishOrAckFailedMsgs;
        CORE::UInt64 lastPublishBatchSize;
        CORE::UInt32 queuedReceiveSuccessAcks;

        bool hasSupportForPublishing;
        bool hasSupportForSubscribing;

        CPubSubClientSideMetrics( void );
        CPubSubClientSideMetrics( const CPubSubClientSideMetrics& src );
        ~CPubSubClientSideMetrics();        
    };
    typedef std::map< CORE::CString, CPubSubClientSideMetrics > StringToPubSubClientSideMetricsMap;

    CPubSubClientSide( char side );
    
    virtual ~CPubSubClientSide();

    virtual bool OnTaskStart( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                               bool wasForced             ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const ChannelSettings& channelSettings );

    const ChannelSettings& GetChannelSettings( void ) const;

    PubSubSideChannelSettings* GetSideSettings( void );

    const StringToPubSubClientSideMetricsMap& GetSideMetrics( void ) const;

    bool ConnectPubSubClient( void );

    bool DisconnectPubSubClient( bool destroyClient = false );

    bool IsRunningInDedicatedThread( void ) const;

    virtual bool GetAllSides( TPubSubClientSideVector*& sides ) = 0;

    virtual bool AcknowledgeReceiptForSide( CIPubSubMsg::TNoLockSharedPtr& msg ,
                                            CPubSubClientSide* msgReceiverSide ) = 0;

    bool AcknowledgeReceiptASync( CIPubSubMsg::TNoLockSharedPtr& msg );

    bool AcknowledgeReceiptSync( CIPubSubMsg::TNoLockSharedPtr& msg );
    
    private:

    void RegisterEventHandlers( void );

    void RegisterTopicEventHandlers( CPubSubClientTopic& topic );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnTopicsAccessAutoCreated( CORE::CNotifier* notifier    ,
                               const CORE::CEvent& eventId  ,
                               CORE::CICloneable* eventData );

    void
    OnTopicsAccessAutoDestroyed( CORE::CNotifier* notifier    ,
                                 const CORE::CEvent& eventId  ,
                                 CORE::CICloneable* eventData );

    void
    OnCheckForTimedOutInFlightMessagesTimerCycle( CORE::CNotifier* notifier    ,
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

    void
    OnPubSubTopicMsgsPublished( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData );

    void
    OnPubSubTopicMsgsPublishFailure( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventId  ,
                                     CORE::CICloneable* eventData );

    void
    OnPubSubTopicLocalPublishQueueFull( CORE::CNotifier* notifier    ,
                                        const CORE::CEvent& eventId  ,
                                        CORE::CICloneable* eventData );

    bool PublishMsgs( const CPubSubClientTopic::TPubSubMsgsRefVector& msgs );

    bool ConfigureTopicLink( const PubSubSideChannelSettings& pubSubSideSettings ,
                             CPubSubClientTopic& topic                           );
    
    bool ConnectPubSubClientTopic( CPubSubClientTopic& topic                           ,
                                   const CPubSubClientFeatures& clientFeatures         ,
                                   const PubSubSideChannelSettings& pubSubSideSettings );
    
    protected:

    bool PublishMsgsASync( const CPubSubClientTopic::TPubSubMsgsRefVector& msgs );

    template < typename TMsgCollection >
    bool PublishMsgsSync( const TMsgCollection& msgs );

    bool PublishMailboxMsgs( void );

    bool RetryPublishFailedMsgs( void );

    void ProcessAcknowledgeReceiptsMailbox( void );

    static CORE::CString GetMsgAttributesForLog( const CIPubSubMsg& msg );

    static CORE::CString GenerateMetricsFriendlyTopicName( const CORE::CString& topicName );   

    class TopicLink
    {
        public:

        class MsgTrackingEntry
        {
            public:
            CORE::UInt32 retryCount;
            CORE::CDateTime firstPublishAttempt;
            CORE::CDateTime lastPublishAttempt;
            CORE::UInt64 publishActionId;
            CIPubSubMsg::TNoLockSharedPtr msg;
            bool isInFlight;
            bool waitingForInFlightConfirmation;
            bool readyToAckPublishSuccessButAckFailed;

            MsgTrackingEntry( void );
            MsgTrackingEntry( const MsgTrackingEntry& src );
            MsgTrackingEntry( CORE::UInt64 publishActionId, CIPubSubMsg::TNoLockSharedPtr& msg, bool isInFlightState = false );
            MsgTrackingEntry& operator=( const MsgTrackingEntry& src );
        };
        
        typedef MT::CTMailBox< CIPubSubMsg::TNoLockSharedPtr >                      TPubSubMsgPtrMailbox;
        typedef std::map< CORE::UInt64, MsgTrackingEntry >                          TUInt64ToMsgTrackingEntryMap;
        typedef std::set< CORE::UInt64 >                                            TUInt64Set;

        CPubSubClientTopic* topic;                                              /**< the actual backend topic access object */ 
        CPubSubClientTopic::TPublishActionIdVector currentPublishActionIds;     /**< temp placeholder to help prevent allocations per invocation */         
        TUInt64ToMsgTrackingEntryMap inFlightMsgs;
        TUInt64Set publishFailedMsgs;
        TPubSubMsgPtrMailbox publishAckdMsgsMailbox;
        CORE::CString metricFriendlyTopicName;
        CPubSubClientSideMetrics* metrics;

        TopicLink( void );
        TopicLink( CPubSubClientTopic* t );
        
        void AddInFlightMsgs( const CPubSubClientTopic::TPublishActionIdVector& publishActionIds ,
                              const CPubSubClientTopic::TIPubSubMsgSPtrVector& msgs              ,
                              bool inFlightDefaultState                                          );

        void AddInFlightMsgs( const CPubSubClientTopic::TPublishActionIdVector& publishActionIds ,
                              const CPubSubClientTopic::TPubSubMsgsRefVector& msgs               ,
                              bool inFlightDefaultState                                          );

        void AddInFlightMsg( CORE::UInt64 publishActionId       ,
                             CIPubSubMsg::TNoLockSharedPtr& msg );
    };

    void UpdateTopicMetrics( TopicLink& topicLink );
    
    typedef std::map< CPubSubClientTopic*, TopicLink > TopicMap;
    typedef CORE::CTMailboxForSharedCloneables< CIPubSubMsg, MT::CNoLock > TPubSubMsgMailbox;

    CPubSubClientPtr m_pubsubClient;
    TopicMap m_topics;
    StringToPubSubClientSideMetricsMap m_metricsMap;
    ChannelSettings m_channelSettings;
    PubSubSideChannelSettings* m_sideSettings;
    TPubSubMsgMailbox m_mailbox;
    CORE::CTimer* m_metricsTimer;
    CORE::CTimer* m_pubsubClientReconnectTimer;    
    CORE::CTimer* m_timedOutInFlightMessagesCheckTimer;
    CIPubSubBookmarkPersistance* m_persistance;
    char m_side;
    bool m_awaitingFailureReport;
    CORE::UInt64 m_totalMsgsInFlight;

    private:

    CPubSubClientSide( const CPubSubClientSide& src ); // not implemented
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CPubSubClientSide, MT::CMutex > CPubSubClientSidePtr;

/*-------------------------------------------------------------------------*/

class CPubSubClientChannel;

class GUCEF_PUBSUB_EXPORT_CPP CPubSubClientOtherSide : public CPubSubClientSide
{
    public:

    CPubSubClientOtherSide( CPubSubClientChannel* parentChannel, char side );
        
    virtual ~CPubSubClientOtherSide();

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                               bool wasForced             ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetAllSides( TPubSubClientSideVector*& sides ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool AcknowledgeReceiptForSide( CIPubSubMsg::TNoLockSharedPtr& msg ,
                                            CPubSubClientSide* msgReceiverSide ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    CPubSubClientOtherSide( void );

    CPubSubClientChannel* m_parentChannel;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CPubSubClientOtherSide, MT::CMutex > CPubSubClientOtherSidePtr;

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP CPubSubClientChannel : public CPubSubClientSide
{
    public:

    CPubSubClientChannel( void );
        
    virtual ~CPubSubClientChannel();

    virtual bool OnTaskStart( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                               bool wasForced             ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const ChannelSettings& channelSettings ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetAllSides( TPubSubClientSideVector*& sides ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool AcknowledgeReceiptForSide( CIPubSubMsg::TNoLockSharedPtr& msg ,
                                            CPubSubClientSide* msgReceiverSide          ) GUCEF_VIRTUAL_OVERRIDE;
    
    bool AcknowledgeReceiptForSideImpl( CORE::UInt32 invokerThreadId       ,
                                        CIPubSubMsg::TNoLockSharedPtr& msg ,
                                        CPubSubClientSide* msgReceiverSide );
    
    void PublishChannelMetrics( void ) const;

    private:

    CPubSubClientOtherSidePtr m_sideBPubSub;
    TPubSubClientSideVector m_sides;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CPubSubClientChannel, MT::CMutex > CPubSubClientChannelPtr;

/*-------------------------------------------------------------------------*/

class PubSub2PubSub;

class GUCEF_PUBSUB_EXPORT_CPP RestApiPubSub2PubSubInfoResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiPubSub2PubSubInfoResource( PubSub2PubSub* app );

    virtual ~RestApiPubSub2PubSubInfoResource();

    virtual bool Serialize( const CORE::CString& resourcePath   ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    PubSub2PubSub* m_app;
};

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP RestApiPubSub2PubSubConfigResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiPubSub2PubSubConfigResource( PubSub2PubSub* app, bool appConfig );

    virtual ~RestApiPubSub2PubSubConfigResource();

    virtual bool Serialize( const CORE::CString& resourcePath   ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    virtual TDeserializeState Deserialize( const CORE::CString& resourcePath   ,
                                           const CORE::CDataNode& input        ,
                                           const CORE::CString& representation ,
                                           bool isDeltaUpdateOnly              ) GUCEF_VIRTUAL_OVERRIDE;

    TDeserializeState UpdateGlobalConfig( const CORE::CDataNode& cfg );
    
    private:

    PubSub2PubSub* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP RestApiPubSubClientChannelConfigResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiPubSubClientChannelConfigResource( PubSub2PubSub* app );

    virtual ~RestApiPubSubClientChannelConfigResource();

    virtual bool Serialize( const CORE::CString& resourcePath   ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    PubSub2PubSub* m_app;
};

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP PubSub2PubSub : public CORE::CObserver             ,
                                              public CORE::CGloballyConfigurable
{
    public:

    PubSub2PubSub( void );
    virtual ~PubSub2PubSub();

    bool Start( void );

    bool SetStandbyMode( bool newMode );

    bool IsGlobalStandbyEnabled( void ) const;

    bool LoadConfig( const CORE::CValueList& appConfig );

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    static const CORE::CDataNode* GetAppConfig( const CORE::CDataNode& globalConfig );

    static CORE::CDataNode* GetAppConfig( CORE::CDataNode& globalConfig );

    const CORE::CDataNode* GetAppConfig( void ) const;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    static const CORE::CDateTime& GetAppCompileDateTime( void );

    CPubSubClientChannelPtr GetChannelByChannelId( CORE::Int32 cid ) const;

    private:

    typedef CORE::CTEventHandlerFunctor< PubSub2PubSub > TEventCallback;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnAppShutdown( CORE::CNotifier* notifier    ,
                   const CORE::CEvent& eventId  ,
                   CORE::CICloneable* eventData );

    void RegisterEventHandlers( void );
    
    private:

    typedef std::map< CORE::Int32, ChannelSettings > ChannelSettingsMap;
    typedef std::map< CORE::Int32, CPubSubClientChannelPtr > PubSubClientChannelMap;
    typedef std::map< CORE::CString, CORE::CDataNode > TChannelCfgMap;
    typedef std::set< CORE::Int32 > Int32Set;

    bool m_isInStandby;
    bool m_globalStandbyEnabled;
    CORE::UInt16 m_udpStartPort;
    CORE::UInt16 m_channelCount;
    CORE::Int32 m_pubSub2PubSubStartChannelID;
    PubSubClientChannelMap m_channels;
    ChannelSettingsMap m_channelSettings;
    ChannelSettings m_templateChannelSettings;
    WEB::CHTTPServer m_httpServer;
    WEB::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CDataNode m_globalConfig;
    CORE::CTimer m_metricsTimer;
    bool m_transmitMetrics;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBGLOBAL_H ? */

