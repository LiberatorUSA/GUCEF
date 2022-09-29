/*
 *  pubsubpluginKAFKA: Generic GUCEF COMCORE plugin for providing pubsub via Kafka
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

#ifndef PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENTTOPIC_H
#define PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENTTOPIC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#include "gucefPUBSUB_CPubSubClientTopic.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H ? */

#ifndef PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENTTOPICCONFIG_H
#include "pubsubpluginKAFKA_CKafkaPubSubClientTopicConfig.h"
#define PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENTTOPICCONFIG_H
#endif /* PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef GUCEF_DYNNEWOFF_H
#include "gucef_dynnewoff.h"
#define GUCEF_DYNNEWOFF_H
#endif /* GUCEF_DYNNEWOFF_H ? */

#include "rdkafka.h"
#include "rdkafkacpp.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace KAFKA {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CKafkaPubSubClient;

/**
 *  KAFKA implementation of the conceptual pub-sub "topic"
 */
class PUBSUBPLUGIN_KAFKA_PLUGIN_PRIVATE_CPP CKafkaPubSubClientTopic : public PUBSUB::CPubSubClientTopic ,
                                                                      private RdKafka::EventCb           ,
                                                                      private RdKafka::DeliveryReportCb  ,
                                                                      private RdKafka::ConsumeCb         ,
                                                                      private RdKafka::RebalanceCb       ,
                                                                      private RdKafka::OffsetCommitCb
{
    public:

    static const std::string KafkaMsgHeader_ProducerHostname;

    CKafkaPubSubClientTopic( CKafkaPubSubClient* client );

    virtual ~CKafkaPubSubClientTopic() GUCEF_VIRTUAL_OVERRIDE;

    virtual PUBSUB::CPubSubClient* GetClient( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool InitializeConnectivity( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool IsHealthy( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsPublishingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsSubscribingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Subscribe( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SubscribeStartingAtTopicIndex( const CORE::CVariant& msgIndexBookmark );
    
    virtual bool SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark );

    virtual bool SubscribeStartingAtMsgDateTime( const CORE::CVariant& msgDtBookmark );

    virtual bool SubscribeStartingAtBookmark( const PUBSUB::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;

    virtual PUBSUB::CPubSubBookmark GetCurrentBookmark( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetTopicName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Publish( CORE::UInt64& publishActionId, const PUBSUB::CIPubSubMsg& msg, bool notify ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool AcknowledgeReceipt( const PUBSUB::CIPubSubMsg& msg ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool AcknowledgeReceipt( const PUBSUB::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool DeriveBookmarkFromMsg( const PUBSUB::CIPubSubMsg& msg, PUBSUB::CPubSubBookmark& bookmark ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool RequestSubscriptionMsgArrivalDelay( CORE::UInt32 minDelayInMs ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( PUBSUB::CPubSubClientTopicConfig& config ) const;

    virtual bool LoadConfig( const PUBSUB::CPubSubClientTopicConfig& config );

    virtual bool LoadConfig( const CKafkaPubSubClientTopicConfig& config );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnPulseCycle( CORE::CNotifier* notifier    ,
                  const CORE::CEvent& eventId  ,
                  CORE::CICloneable* eventData );

    CORE::UInt32 GetKafkaErrorRepliesCounter( bool resetCounter );

    CORE::UInt32 GetKafkaConnectionErrorsCounter( bool resetCounter );

    CORE::UInt32 GetKafkaMsgsTransmittedCounter( bool resetCounter );

    CORE::UInt32 GetKafkaMsgsReceivedCounter( bool resetCounter );

    CORE::UInt32 GetKafkaMsgsFilteredCounter( bool resetCounter );
    
    class TopicMetrics
    {
        public:

        TopicMetrics( void );

        CORE::UInt32 kafkaMessagesTransmitted;
        CORE::UInt32 kafkaTransmitQueueSize;
        CORE::UInt32 kafkaTransmitOverflowQueueSize;

        CORE::UInt32 kafkaMessagesReceived;
        CORE::UInt32 kafkaMessagesFiltered;

        CORE::UInt32 kafkaErrorReplies;
        CORE::UInt32 kafkaConnectionErrors;
    };

    const TopicMetrics& GetMetrics( void ) const;

    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( CORE::UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    #pragma pack(push, 1)  // No structure packing, we want to use this as a BSOB interpretation
    struct SPartitionOffset
    {
        CORE::Int32 partitionId;
        CORE::Int64 partitionOffset;
    };
    typedef struct SPartitionOffset TPartitionOffset;
    #pragma pack(pop)

    typedef std::vector< TPartitionOffset > TPartitionOffsetVector;
    typedef std::map< CORE::Int32, TPartitionOffset > TPartitionOffsetMap;

    static bool VariantToPartitionOffsets( const CORE::CVariant& indexBookmarkBlob, TPartitionOffsetMap& offsets );
    static bool VariantToPartitionOffset( const CORE::CVariant& partitionOffsetBlob, TPartitionOffset& offset );

    static void RdKafkaLogCallback( const rd_kafka_t* rk , 
                                    int level            ,
                                    const char* fac      , 
                                    const char* buf      );

    static int RdKafkaStatsCallback( rd_kafka_t *rk  ,
                                     char *json      ,
 				                     size_t json_len ,
						             void *opaque    );

    void RegisterEventHandlers( void );

    void Clear( void );

    void PrepStorageForReadMsgs( CORE::UInt32 msgCount );

    bool SetupBasedOnConfig( void );

    bool CommitConsumerOffsets( void );

    bool AcknowledgeReceipt( const TPartitionOffset& offset );

    CORE::Int64 ConvertKafkaConsumerStartOffset( const CORE::CString& startOffsetDescription ,
                                                 CORE::Int32 partitionId                     ,
                                                 CORE::Int32 timeoutInMs                     );

    CORE::Int64 ConvertKafkaConsumerStartOffset( CORE::Int64 startOffsetDescription ,
                                                 CORE::Int32 partitionId            ,
                                                 CORE::Int32 timeoutInMs            );

    static CORE::CString ConvertKafkaConsumerStartOffset( CORE::Int64 offset );

    static const std::string& MsgStatusToString( RdKafka::Message::Status status );

    void NotifyOfReceivedMsg( RdKafka::Message& message );

    void UpdateIsHealthyStatus( bool newStatus );

    virtual void event_cb( RdKafka::Event& event ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void dr_cb( RdKafka::Message& message ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void consume_cb( RdKafka::Message& message, void* opaque ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void rebalance_cb( RdKafka::KafkaConsumer* consumer                  ,
                               RdKafka::ErrorCode err                            ,
                               std::vector<RdKafka::TopicPartition*>& partitions ) GUCEF_VIRTUAL_OVERRIDE;

  /**
   * @brief Set offset commit callback for use with consumer groups
   *
   * The results of automatic or manual offset commits will be scheduled
   * for this callback and is served by RdKafka::KafkaConsumer::consume().
   *
   * If no partitions had valid offsets to commit this callback will be called
   * with \p err == ERR__NO_OFFSET which is not to be considered an error.
   *
   * The \p offsets list contains per-partition information:
   *   - \c topic      The topic committed
   *   - \c partition  The partition committed
   *   - \c offset:    Committed offset (attempted)
   *   - \c err:       Commit error
   */
    virtual void offset_commit_cb( RdKafka::ErrorCode err                         ,
                                   std::vector<RdKafka::TopicPartition*>& offsets ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    typedef CORE::CTEventHandlerFunctor< CKafkaPubSubClientTopic > TEventCallback;
   
    // Types to implement/hook-up topic interface
    typedef std::vector< PUBSUB::CBasicPubSubMsg >                     TPubSubMsgsVector;
    typedef std::pair< CORE::CDynamicBuffer, CORE::CDynamicBuffer >     TBufferPair;
    typedef std::vector< TBufferPair >                                  TBufferVector;
    typedef std::map< CORE::Int32, CORE::Int64 >                        TInt32ToInt64Map;

    CKafkaPubSubClient* m_client;
    CORE::CTimer* m_metricsTimer;
    TPubSubMsgsVector m_pubsubMsgs;
    TMsgsRecievedEventData m_pubsubMsgsRefs;
    TBufferVector m_pubsubMsgAttribs;
    CKafkaPubSubClientTopicConfig m_config;
    RdKafka::Conf* m_kafkaProducerTopicConf;
    RdKafka::Conf* m_kafkaConsumerTopicConf;
    RdKafka::Conf* m_kafkaProducerConf;
    RdKafka::Conf* m_kafkaConsumerConf;
    RdKafka::Producer* m_kafkaProducer;
    RdKafka::Topic* m_kafkaProducerTopic;
    RdKafka::KafkaConsumer* m_kafkaConsumer;
    CORE::UInt32 m_kafkaErrorReplies;
    CORE::UInt32 m_kafkaConnectionErrors;
    CORE::UInt32 m_kafkaMsgsTransmitted;
    CORE::UInt32 m_kafkaMessagesReceived;
    CORE::UInt32 m_kafkaMessagesFiltered;
    std::string m_producerHostname;
    bool m_firstPartitionAssignment;
    TInt32ToInt64Map m_consumerOffsets;
    CORE::UInt64 m_tickCountAtLastOffsetCommit;
    CORE::UInt64 m_tickCountAtConsumeDelayRequest;
    CORE::UInt32 m_requestedConsumeDelayInMs;
    bool m_msgsReceivedSinceLastOffsetCommit;
    bool m_consumerOffsetWaitsForExplicitMsgAck;
    CORE::UInt64 m_currentPublishActionId;
    CORE::UInt64 m_currentReceiveActionId;
    TPublishActionIdVector m_publishSuccessActionIds;
    TMsgsPublishedEventData m_publishSuccessActionEventData;
    TPublishActionIdVector m_publishFailureActionIds;
    TMsgsPublishFailureEventData m_publishFailureActionEventData;
    TopicMetrics m_metrics;
    bool m_shouldBeConnected;
    mutable bool m_isHealthy;
    MT::CMutex m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAFKA */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENTTOPIC_H ? */
