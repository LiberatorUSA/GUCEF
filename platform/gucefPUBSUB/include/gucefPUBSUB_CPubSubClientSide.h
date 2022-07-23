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

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTSIDE_H
#define GUCEF_PUBSUB_CPUBSUBCLIENTSIDE_H

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

#ifndef GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H
#include "gucefPUBSUB_CPubSubSideChannelSettings.h"
#define GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H
#endif /* GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCHANNELSETTINGS_H
#include "gucefPUBSUB_CPubSubChannelSettings.h"
#define GUCEF_PUBSUB_CPUBSUBCHANNELSETTINGS_H
#endif /* GUCEF_PUBSUB_CPUBSUBCHANNELSETTINGS_H ? */

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

class CPubSubFlowRouter;

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

    CPubSubClientSide( const CORE::CString& sideId                ,
                       CPubSubFlowRouter* flowRouter = GUCEF_NULL );
    
    virtual ~CPubSubClientSide();

    virtual bool OnTaskStart( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                               bool wasForced             ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CPubSubSideChannelSettings& sideSettings );

    CPubSubSideChannelSettings& GetSideSettings( void );

    const CPubSubSideChannelSettings& GetSideSettings( void ) const;

    CORE::CString GetSideId( void ) const;

    const StringToPubSubClientSideMetricsMap& GetSideMetrics( void ) const;

    bool ConnectPubSubClient( void );

    bool DisconnectPubSubClient( bool destroyClient = false );

    bool IsRunningInDedicatedThread( void ) const;

    bool HasSubscribersNeedingAcks( void ) const;

    bool PublishMsgs( const CPubSubClientTopic::TPubSubMsgsRefVector& msgs );

    virtual bool IsHealthy( void ) const;

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

    bool ConfigureTopicLink( const CPubSubSideChannelSettings& pubSubSideSettings ,
                             CPubSubClientTopic& topic                            );
    
    bool ConnectPubSubClientTopic( CPubSubClientTopic& topic                            ,
                                   const CPubSubClientFeatures& clientFeatures          ,
                                   const CPubSubSideChannelSettings& pubSubSideSettings );
    
    protected:

    bool PublishMsgsASync( const CPubSubClientTopic::TPubSubMsgsRefVector& msgs );

    template < typename TMsgCollection >
    bool PublishMsgsSync( const TMsgCollection& msgs );

    bool PublishMailboxMsgs( void );

    bool RetryPublishFailedMsgs( void );

    void ProcessAcknowledgeReceiptsMailbox( void );

    static CORE::CString GetMsgAttributesForLog( const CIPubSubMsg& msg );

    static CORE::CString GenerateMetricsFriendlyTopicName( const CORE::CString& topicName );   

    bool GetLatestBookmark( const CPubSubClientTopic& topic ,
                            CPubSubBookmark& bookmark       );

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
        typedef std::map< CORE::UInt64, CPubSubBookmark >                           TUInt64ToBookmarkMap;
        typedef std::set< CORE::UInt64 >                                            TUInt64Set;

        CPubSubClientTopic* topic;                                              /**< the actual backend topic access object */ 
        CPubSubClientTopic::TPublishActionIdVector currentPublishActionIds;     /**< temp placeholder to help prevent allocations per invocation */         
        TUInt64ToMsgTrackingEntryMap inFlightMsgs;
        TUInt64Set publishFailedMsgs;
        TPubSubMsgPtrMailbox publishAckdMsgsMailbox;
        CORE::CString metricFriendlyTopicName;
        CPubSubClientSideMetrics* metrics;
        CORE::CDateTime lastBookmarkPersistSuccess;
        CORE::Int32 msgsSinceLastBookmarkPersist;
        TUInt64ToBookmarkMap bookmarksOnMsgReceived;

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

    bool AcknowledgeReceiptSync( CIPubSubMsg::TNoLockSharedPtr& msg, TopicLink& topicLink );
    
    /**
     *  Attempts to update the bookmark persistance with the given bookmark
     *  Note that if bookmarking is not supported then this is treated as a fyi no-op 
     */
    bool UpdateReceivedMessagesBookmarkAsNeeded( const CIPubSubMsg& msg                  , 
                                                 const CPubSubBookmark& msgBatchBookmark ,
                                                 TopicLink& topicLink                    );

    /**
     *  Same functionality as the 'UpdateReceivedMessagesBookmarkAsNeeded' variant that accepts a bookmark except it
     *  has the additional overhead of obtaining the bookmark itself through whatever means are supported.
     *  If you already have the relevant bookmark you should use the other variant instead 
     */
    bool UpdateReceivedMessagesBookmarkAsNeeded( const CIPubSubMsg& msg , 
                                                 TopicLink& topicLink   );
    
    bool FindClosestMsgBatchBookmarkToMsg( const CIPubSubMsg& msg            ,                                                            
                                           const TopicLink& topicLink        ,
                                           UInt64& msgBatchBookmarkReceiveId ,
                                           CPubSubBookmark& msgBatchBookmark );

    void CleanupMsgBatchBookmarksUpTo( TopicLink& topicLink             , 
                                       UInt64 msgBatchBookmarkReceiveId );
    
    typedef std::map< CPubSubClientTopic*, TopicLink > TopicMap;
    typedef CORE::CTMailboxForSharedCloneables< CIPubSubMsg, MT::CNoLock > TPubSubMsgMailbox;

    CPubSubClientPtr m_pubsubClient;
    CPubSubClientFeatures m_clientFeatures;
    TIPubSubBookmarkPersistenceBasicPtr m_pubsubBookmarkPersistence;
    CORE::CString m_bookmarkNamespace;
    TopicMap m_topics;
    StringToPubSubClientSideMetricsMap m_metricsMap;
    CPubSubSideChannelSettings m_sideSettings;
    TPubSubMsgMailbox m_mailbox;
    CORE::CTimer* m_metricsTimer;
    CORE::CTimer* m_pubsubClientReconnectTimer;    
    CORE::CTimer* m_timedOutInFlightMessagesCheckTimer;
    CORE::CString m_sideId;
    bool m_awaitingFailureReport;
    CORE::UInt64 m_totalMsgsInFlight;
    CPubSubFlowRouter* m_flowRouter;

    private:

    CPubSubClientSide( const CPubSubClientSide& src ); // not implemented
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CPubSubClientSide, MT::CMutex >  CPubSubClientSidePtr;
typedef std::vector< CPubSubClientSidePtr >                 TPubSubClientSidePtrVector;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTSIDE_H ? */

