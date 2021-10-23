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

#ifndef GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#include "gucefCOMCORE_CPubSubMsgContainerBinarySerializer.h"
#define GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#endif /* GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H ? */

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

class PubSubSideChannelSettings : public CORE::CIConfigurable
{
    public:

    PubSubSideChannelSettings( void );
    PubSubSideChannelSettings( const PubSubSideChannelSettings& src );
    PubSubSideChannelSettings& operator=( const PubSubSideChannelSettings& src );

    COMCORE::CPubSubClientConfig pubsubClientConfig;
    bool performPubSubInDedicatedThread;
    bool applyThreadCpuAffinity;
    CORE::UInt32 cpuAffinityForPubSubThread;
    bool subscribeWithoutBookmarkIfNoneIsPersisted;
    bool retryFailedPublishAttempts;
    bool allowOutOfOrderPublishRetry;
    CORE::Int32 maxMsgPublishRetryAttempts;
    CORE::Int32 maxMsgPublishRetryTotalTimeInMs;
    
    bool needToTrackInFlightPublishedMsgsForAck;       //< this setting is derived and cached from other settings 

    COMCORE::CPubSubClientTopicConfig* GetTopicConfig( const CORE::CString& topicName );

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

class ChannelSettings : public CORE::CIConfigurable
{
    public:

    typedef std::map< char, PubSubSideChannelSettings > TCharToPubSubSideChannelSettingsMap;

    ChannelSettings( void );
    ChannelSettings( const ChannelSettings& src );
    ChannelSettings& operator=( const ChannelSettings& src );

    PubSubSideChannelSettings* GetPubSubSideSettings( char side );

    TCharToPubSubSideChannelSettingsMap pubSubSideChannelSettingsMap;
    CORE::Int32 channelId;
    CORE::UInt32 ticketRefillOnBusyCycle;
    bool collectMetrics;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

class CIPubSubBookmarkPersistance
{
    public:

    virtual bool GetPersistedBookmark( CORE::Int32 channelId              , 
                                       const CORE::CString& topicName     , 
                                       COMCORE::CPubSubBookmark& bookmark ) = 0;
};

/*-------------------------------------------------------------------------*/

class CPubSubClientSide : public CORE::CTaskConsumer
{
    public:

    typedef CORE::CTEventHandlerFunctor< CPubSubClientSide > TEventCallback;
    typedef std::vector< CPubSubClientSide* >                TPubSubClientSideVector;

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

    bool ConnectPubSubClient( void );

    bool DisconnectPubSubClient( bool destroyClient = false );

    bool IsRunningInDedicatedThread( void ) const;

    virtual bool GetAllSides( TPubSubClientSideVector*& sides ) = 0;

    virtual bool AcknowledgeReceiptForSide( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg ,
                                            CPubSubClientSide* msgReceiverSide          ) = 0;

    bool AcknowledgeReceiptASync( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg );

    bool AcknowledgeReceiptSync( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg );
    
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

    bool PublishMsgs( const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector& msgs );
    
    protected:

    bool PublishMsgsASync( const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector& msgs );

    template < typename TMsgCollection >
    bool PublishMsgsSync( const TMsgCollection& msgs );

    bool PublishMailboxMsgs( void );

    bool RetryPublishFailedMsgs( void );

    void ProcessAcknowledgeReceiptsMailbox( void );

    static CORE::CString GetMsgAttributesForLog( const COMCORE::CIPubSubMsg& msg );

    CPubSubClientSide( const CPubSubClientSide& src ); // not implemented

    class TopicLink
    {
        public:

        class MsgRetryEntry
        {
            public:
            CORE::UInt32 retryCount;
            CORE::CDateTime firstPublishAttempt;
            CORE::CDateTime lastPublishAttempt;
            CORE::UInt64 originalPublishActionId;
            COMCORE::CIPubSubMsg::TNoLockSharedPtr msg;

            MsgRetryEntry( void );
            MsgRetryEntry( CORE::UInt64 publishActionId, COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg );
        };
        
        typedef std::map< CORE::UInt64, COMCORE::CIPubSubMsg::TNoLockSharedPtr >    TUInt64ToNoLockSharedMsgPtrMap;
        typedef MT::CTMailBox< COMCORE::CIPubSubMsg::TNoLockSharedPtr >             TPubSubMsgPtrMailbox;
        typedef std::list< MsgRetryEntry >                                          TMsgRetryEntryList;

        COMCORE::CPubSubClientTopic* topic;                                              /**< the actual backend topic access object */ 
        COMCORE::CPubSubClientTopic::TPublishActionIdVector currentPublishActionIds;     /**< temp placeholder to help prevent allocations per invocation */         
        TUInt64ToNoLockSharedMsgPtrMap inFlightMsgs;
        TPubSubMsgPtrMailbox publishAckdMsgsMailbox;
        TMsgRetryEntryList publishFailedMsgs;
        bool awaitingFailureReport;

        TopicLink( void );
        TopicLink( COMCORE::CPubSubClientTopic* t );
        
        void AddInFlightMsgs( const COMCORE::CPubSubClientTopic::TPublishActionIdVector& publishActionIds ,
                              const COMCORE::CPubSubClientTopic::TIPubSubMsgSPtrVector& msgs              );

        void AddInFlightMsgs( const COMCORE::CPubSubClientTopic::TPublishActionIdVector& publishActionIds ,
                              const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector& msgs               );

        void AddInFlightMsg( CORE::UInt64 publishActionId                ,
                             COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg );

        void AddFailedToPublishMsg( CORE::UInt64 publishActionId                ,
                                    COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg );
    };
    
    typedef std::map< COMCORE::CPubSubClientTopic*, TopicLink > TopicMap;
    typedef CORE::CTMailboxForSharedCloneables< COMCORE::CIPubSubMsg, MT::CNoLock > TPubSubMsgMailbox;

    COMCORE::CPubSubClientPtr m_pubsubClient;
    TopicMap m_topics;
    ChannelSettings m_channelSettings;
    PubSubSideChannelSettings* m_sideSettings;
    TPubSubMsgMailbox m_mailbox;
    CORE::CTimer* m_metricsTimer;
    CORE::CTimer* m_pubsubClientReconnectTimer;    
    CIPubSubBookmarkPersistance* m_persistance;
    char m_side;
    bool m_awaitingFailureReport;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CPubSubClientSide, MT::CMutex > CPubSubClientSidePtr;

/*-------------------------------------------------------------------------*/

class CPubSubClientChannel;

class CPubSubClientOtherSide : public CPubSubClientSide
{
    public:

    CPubSubClientOtherSide( CPubSubClientChannel* parentChannel, char side );
        
    virtual ~CPubSubClientOtherSide();

    virtual bool GetAllSides( TPubSubClientSideVector*& sides ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool AcknowledgeReceiptForSide( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg ,
                                            CPubSubClientSide* msgReceiverSide          ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    CPubSubClientOtherSide( void );

    CPubSubClientChannel* m_parentChannel;
};

/*-------------------------------------------------------------------------*/

class CPubSubClientChannel : public CPubSubClientSide
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

    virtual bool AcknowledgeReceiptForSide( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg ,
                                            CPubSubClientSide* msgReceiverSide          ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    CPubSubClientSide* m_sideBPubSub;
    TPubSubClientSideVector m_sides;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CPubSubClientChannel, MT::CMutex > CPubSubClientChannelPtr;

/*-------------------------------------------------------------------------*/

class PubSub2PubSub;

class RestApiPubSub2PubSubInfoResource : public WEB::CCodecBasedHTTPServerResource
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

class RestApiPubSub2PubSubConfigResource : public WEB::CCodecBasedHTTPServerResource
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

    private:

    PubSub2PubSub* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class PubSub2PubSub : public CORE::CObserver ,
                      public CORE::CIConfigurable
{
    public:

    PubSub2PubSub( void );
    virtual ~PubSub2PubSub();

    bool Start( void );

    bool SetStandbyMode( bool newMode );

    bool IsGlobalStandbyEnabled( void ) const;

    bool LoadConfig( const CORE::CValueList& appConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    static const CORE::CDateTime& GetAppCompileDateTime( void );

    private:

    typedef CORE::CTEventHandlerFunctor< PubSub2PubSub > TEventCallback;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

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
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;
    CORE::CTimer m_metricsTimer;
    bool m_transmitMetrics;
};

/*-------------------------------------------------------------------------*/
