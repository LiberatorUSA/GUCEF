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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENT_H
#include "pubsubpluginKAFKA_CKafkaPubSubClient.h"
#define PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_KAFKA_CKAFKAPUBSUBCLIENT_H ? */

#include "pubsubpluginKAFKA_CKafkaPubSubClientTopic.h"

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

const std::string CKafkaPubSubClientTopic::KafkaMsgHeader_ProducerHostname = "ProducerHostname";

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientTopic::CKafkaPubSubClientTopic( CKafkaPubSubClient* client )
    : PUBSUB::CPubSubClientTopic()
    , m_client( client )
    , m_metricsTimer( GUCEF_NULL )
    , m_config()
    , m_kafkaProducerTopicConf( GUCEF_NULL )
    , m_kafkaConsumerTopicConf( GUCEF_NULL )
    , m_kafkaProducerConf( GUCEF_NULL )
    , m_kafkaConsumerConf( GUCEF_NULL )
    , m_kafkaProducer( GUCEF_NULL )
    , m_kafkaProducerTopic( GUCEF_NULL )
    , m_kafkaConsumer( GUCEF_NULL )
    , m_kafkaErrorReplies( 0 )
    , m_kafkaMsgsTransmitted( 0 )
    , m_kafkaMessagesReceived( 0 )
    , m_kafkaMessagesFiltered( 0 )
    , m_producerHostname( CORE::GetHostname() )
    , m_firstPartitionAssignment( true )
    , m_consumerOffsets()
    , m_tickCountAtLastOffsetCommit( 0 )
    , m_tickCountAtConsumeDelayRequest( 0 )
    , m_requestedConsumeDelayInMs( 0 )
    , m_msgsReceivedSinceLastOffsetCommit( false )
    , m_consumerOffsetWaitsForExplicitMsgAck( false )
    , m_currentPublishActionId( 1 )
    , m_currentReceiveActionId( 1 )
    , m_publishSuccessActionIds()
    , m_publishSuccessActionEventData()
    , m_publishFailureActionIds()
    , m_publishFailureActionEventData()
    , m_metrics()
    , m_shouldBeConnected( false )
    , m_isHealthy( true )
    , m_lock()
{GUCEF_TRACE;

    m_publishSuccessActionEventData.LinkTo( &m_publishSuccessActionIds );
    m_publishFailureActionEventData.LinkTo( &m_publishFailureActionIds );

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientTopic::~CKafkaPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();
    Clear();
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClient*
CKafkaPubSubClientTopic::GetClient( void )
{GUCEF_TRACE;

    return m_client;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::Clear( void )
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    delete m_kafkaProducerConf;
    m_kafkaProducerConf = GUCEF_NULL;

    delete m_kafkaConsumerConf;
    m_kafkaConsumerConf = GUCEF_NULL;

    delete m_kafkaProducerTopicConf;
    m_kafkaProducerTopicConf = GUCEF_NULL;

    delete m_kafkaConsumerTopicConf;
    m_kafkaConsumerTopicConf = GUCEF_NULL;

    delete m_kafkaProducerTopic;
    m_kafkaProducerTopic = GUCEF_NULL;

    delete m_kafkaProducer;
    m_kafkaProducer = GUCEF_NULL;

    delete m_kafkaConsumer;
    m_kafkaConsumer = GUCEF_NULL;

    m_currentPublishActionId = 1;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CKafkaPubSubClientTopic::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }

    TEventCallback callback2( this, &CKafkaPubSubClientTopic::OnPulseCycle );
    SubscribeTo( m_client->GetConfig().pulseGenerator ,
                 CORE::CPulseGenerator::PulseEvent    ,
                 callback2                            );
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;

    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return m_config.needSubscribeSupport;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CKafkaPubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::Publish( CORE::UInt64& publishActionId, const PUBSUB::CIPubSubMsg& msg, bool notify )
{GUCEF_TRACE;

    RdKafka::ErrorCode retCode = RdKafka::ERR_NO_ERROR;

    MT::CScopeMutex lock( m_lock );

    if ( 0 == publishActionId )
    {
        publishActionId = m_currentPublishActionId;
        ++m_currentPublishActionId;
    }

    // First write any backend auto generated headers per config
    RdKafka::Headers* headers = GUCEF_NULL;
    if ( m_config.addProducerHostnameAsKafkaMsgHeader )
    {
        headers = RdKafka::Headers::create();
        retCode = headers->add( KafkaMsgHeader_ProducerHostname, m_producerHostname );
        if ( retCode != RdKafka::ERR_NO_ERROR )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: Failed to add auto generated Producer Hostname Kafka Msg header: " + RdKafka::err2str( retCode ) +
                ". publishActionId=" + CORE::ToString( publishActionId ).STL_String() + ". receiveActionId=" + CORE::ToString( msg.GetReceiveActionId() ).STL_String() );
        }
    }

    // Next we add the message's meta-data key-value headers if any
    const PUBSUB::CIPubSubMsg::TKeyValuePairs& metaDataKvPairs = msg.GetMetaDataKeyValuePairs();
    if ( !metaDataKvPairs.empty() )
    {
        if ( GUCEF_NULL == headers )
            headers = RdKafka::Headers::create();

        PUBSUB::CIPubSubMsg::TKeyValuePairs::const_iterator k = metaDataKvPairs.begin();
        while ( k != metaDataKvPairs.end() )
        {
            retCode = headers->add( m_config.prefixToAddForMetaDataKvPairs + (*k).first.AsString(), (*k).second.AsVoidPtr(), (*k).second.ByteSize() );
            if ( retCode != RdKafka::ERR_NO_ERROR )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: Failed to add msg meta-data key \"" + m_config.prefixToAddForMetaDataKvPairs + (*k).first.AsString() +
                    "\" as Kafka Msg header: " + RdKafka::err2str( retCode ) + ". Value=" + (*k).second.AsString() +
                    ". publishActionId=" + CORE::ToString( publishActionId ) + ". receiveActionId=" + CORE::ToString( msg.GetReceiveActionId() ) );
                return false;
            }
            ++k;
        }
    }

    // Next we add the message's key-value headers if any
    const PUBSUB::CIPubSubMsg::TKeyValuePairs& kvPairs = msg.GetKeyValuePairs();
    if ( !kvPairs.empty() )
    {
        if ( GUCEF_NULL == headers )
            headers = RdKafka::Headers::create();

        PUBSUB::CIPubSubMsg::TKeyValuePairs::const_iterator k = kvPairs.begin();
        while ( k != kvPairs.end() )
        {
            retCode = headers->add( m_config.prefixToAddForKvPairs + (*k).first.AsString(), (*k).second.AsVoidPtr(), (*k).second.ByteSize() );
            if ( retCode != RdKafka::ERR_NO_ERROR )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: Failed to add msg key-value entry \"" + m_config.prefixToAddForKvPairs + (*k).first.AsString() +
                        "\" as Kafka Msg header: " + RdKafka::err2str( retCode ) + ". Value=" + (*k).second.AsString() +
                        ". publishActionId=" + CORE::ToString( publishActionId ) + ". receiveActionId=" + CORE::ToString( msg.GetReceiveActionId() ) );
                return false;
            }
            ++k;
        }
    }

    void* msg_opaque = GUCEF_NULL;
    if ( notify )
    {
        // We encode the publish action Id into the passthrough opaque void*
        // This saves us from having to perform another memory allocation
        #ifdef GUCEF_64BIT
        msg_opaque = reinterpret_cast< void* >( publishActionId );
        #else
        if ( publishActionId > GUCEF_MT_UINT32MAX )
            msg_opaque = reinterpret_cast< void* >( publishActionId - GUCEF_MT_UINT32MAX );
        else
            msg_opaque = reinterpret_cast< void* >( static_cast< CORE::UInt32 >( publishActionId ) );
        #endif
    }

    retCode =
        m_kafkaProducer->dvcustom_produce( m_kafkaProducerTopic,
                                           RdKafka::Topic::PARTITION_UA,                               // <- use configured partitioner to keep it config driven
                                           RdKafka::Producer::RK_MSG_COPY,                             // <- Copy payload, tradeoff against blocking on kafka produce
                                           const_cast< void* >( msg.GetPrimaryPayload().AsVoidPtr() ), // <- MSG_COPY flag will cause buffer to be copied, const cast to avoid copying again due to bad constness on library API
                                           (size_t) msg.GetPrimaryPayload().ByteSize( false ),
                                           const_cast< void* >( msg.GetMsgId().AsVoidPtr() ),
                                           (size_t) msg.GetMsgId().ByteSize( false ),
                                           headers,
                                           msg_opaque );

    if ( retCode != RdKafka::ERR_NO_ERROR )
    {
        // Headers are auto-deleted on success but not on failure!
        delete headers;
    }

    switch ( retCode )
    {
        case RdKafka::ERR_NO_ERROR:
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: Successfully queued message for transmission with publishActionId " + CORE::ToString( publishActionId ) +
                    ". receiveActionId=" + CORE::ToString( msg.GetReceiveActionId() ) );
            return true;
        }
        case RdKafka::ERR__QUEUE_FULL:
        {
            // We dont treat queue full as an error metrics wise. This is an expected and handled scenario
            ++m_metrics.kafkaTransmitOverflowQueueSize;
            m_metrics.kafkaTransmitQueueSize = (CORE::UInt32) m_kafkaProducer->outq_len();

            // We will not be getting a delivery report callback on this message so we add it here
            // to messages for which a failure needs to be notified
            m_publishFailureActionIds.push_back( publishActionId );

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: transmission queue is full. publishActionId=" + CORE::ToString( publishActionId ) +
                    ". receiveActionId=" + CORE::ToString( msg.GetReceiveActionId() ) );

            NotifyObservers( LocalPublishQueueFullEvent );
            return false;
        }
        default:
        {
            // We will not be getting a delivery report callback on this message so we add it here
            // to messages for which a failure needs to be notified
            m_publishFailureActionIds.push_back( publishActionId );

            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:Publish: Kafka error: " + RdKafka::err2str( retCode ) +
                    " from kafkaProducer->produce(). publishActionId=" + CORE::ToString( publishActionId ).STL_String() +
                    ". receiveActionId=" + CORE::ToString( msg.GetReceiveActionId() ).STL_String() );
            ++m_kafkaErrorReplies;
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::SaveConfig( PUBSUB::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::LoadConfig( const PUBSUB::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    Clear();
    m_config = config;
    return SetupBasedOnConfig();
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::LoadConfig( const CKafkaPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    Clear();
    m_config = config;
    return SetupBasedOnConfig();
}

/*-------------------------------------------------------------------------*/

int
CKafkaPubSubClientTopic::RdKafkaStatsCallback( rd_kafka_t *rk  ,
                                               char *json      ,
                                               size_t json_len ,
						                       void *opaque    )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::RdKafkaLogCallback( const rd_kafka_t *rk , 
                                             int level            ,
                                             const char *fac      , 
                                             const char *buf      )
{GUCEF_TRACE;

    // When using this approach to getting logs from RdKafka note that this callback is called
    // directly from various internal RdKafka threads

	switch ( level )
    {
        case RdKafka::Event::EVENT_SEVERITY_ALERT:
        case RdKafka::Event::EVENT_SEVERITY_EMERG:
        case RdKafka::Event::EVENT_SEVERITY_CRITICAL:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "Kafka log: " + CORE::ToString( fac ) + " : " + CORE::ToString( buf ) );
            break;
        }
        case RdKafka::Event::EVENT_SEVERITY_ERROR:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Kafka log: " + CORE::ToString( fac ) + " : " + CORE::ToString( buf ) );
            break;
        }
        case RdKafka::Event::EVENT_SEVERITY_WARNING:
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Kafka log: " + CORE::ToString( fac ) + " : " + CORE::ToString( buf ) );
            break;
        }
        case RdKafka::Event::EVENT_SEVERITY_INFO:
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Kafka log: " + CORE::ToString( fac ) + " : " + CORE::ToString( buf ) );
            break;
        }
        case RdKafka::Event::EVENT_SEVERITY_DEBUG:
        {
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Kafka log: " + CORE::ToString( fac ) + " : " + CORE::ToString( buf ) );
            break;
        }

        case RdKafka::Event::EVENT_SEVERITY_NOTICE:
        default:
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka log: " + CORE::ToString( fac ) + " : " + CORE::ToString( buf ) );
            break;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::SetupBasedOnConfig( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_client )
        return false;

   const CKafkaPubSubClientConfig& clientConfig = m_client->GetConfig();

    if ( clientConfig.desiredFeatures.supportsMetrics )
    {
        if ( GUCEF_NULL == m_metricsTimer )
            m_metricsTimer = new CORE::CTimer( clientConfig.pulseGenerator, 1000 );
        m_metricsTimer->SetEnabled( clientConfig.desiredFeatures.supportsMetrics );
    }

    RegisterEventHandlers();

    // The library wants the addresses as a csv list on its config obj
    // we convert and prep as such
    CORE::CString csvKafkaBrokerList;
    PUBSUB::CPubSubClientConfig::THostAddressVector::const_iterator h = clientConfig.remoteAddresses.begin();
    while ( h != clientConfig.remoteAddresses.end() )
    {
        // The RdKafka library will re-resolve DNSs on reconnects so we should feed it DNSs
        csvKafkaBrokerList += (*h).HostnameAndPortAsString() + ',';
        ++h;
    }

	if ( clientConfig.desiredFeatures.supportsPublishing )
    {
        std::string errStr;

        RdKafka::Conf* kafkaConf = RdKafka::Conf::create( RdKafka::Conf::CONF_GLOBAL );
        kafkaConf->set( "metadata.broker.list", csvKafkaBrokerList, errStr );
	    kafkaConf->set( "event_cb", static_cast< RdKafka::EventCb* >( this ), errStr );
	    kafkaConf->set( "dr_cb", static_cast< RdKafka::DeliveryReportCb* >( this ), errStr );
        kafkaConf->set( "rebalance_cb", static_cast< RdKafka::RebalanceCb* >( this ), errStr );

        rd_kafka_conf_set_log_cb( kafkaConf->c_ptr_global(), &RdKafkaLogCallback );  

        CKafkaPubSubClientConfig::StringMap::const_iterator m = clientConfig.kafkaProducerGlobalConfigSettings.begin();
        while ( m != clientConfig.kafkaProducerGlobalConfigSettings.end() )
        {
            if ( RdKafka::Conf::CONF_OK != kafkaConf->set( (*m).first, (*m).second, errStr ) )
            {
		        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to set Kafka Producer global config entry \"" +
                        (*m).first + "\"=\"" + (*m).second + "\" for topic \"" + m_config.topicName + "\", error message: " + errStr );
                ++m_kafkaErrorReplies;
                return false;
            }
            ++m;
        }
        delete m_kafkaProducerConf;
        m_kafkaProducerConf = kafkaConf;

        RdKafka::Conf* kafkaProducerTopicConfig = RdKafka::Conf::create( RdKafka::Conf::CONF_TOPIC );
        m = m_config.kafkaProducerTopicConfigSettings.begin();
        while ( m != m_config.kafkaProducerTopicConfigSettings.end() )
        {
            if ( RdKafka::Conf::CONF_OK != m_kafkaProducerTopicConf->set( (*m).first, (*m).second, errStr ) )
            {
		        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to set Kafka Producer topic config entry \"" +
                        (*m).first + "\"=\"" + (*m).second + "\" for topic \"" + m_config.topicName + "\", error message: " + errStr );
                ++m_kafkaErrorReplies;
                return false;
            }
            ++m;

        }
        delete m_kafkaProducerTopicConf;
        m_kafkaProducerTopicConf = kafkaProducerTopicConfig;

        RdKafka::Producer* producer = RdKafka::Producer::create( m_kafkaProducerConf, errStr );
	    if ( producer == GUCEF_NULL )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to create Kafka producer for topic \"" + m_config.topicName + "\", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
	    }
        m_kafkaProducer = producer;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:LoadConfig: Successfully created Kafka producer for topic \"" + m_config.topicName + "\"" );

        RdKafka::Topic* topic = RdKafka::Topic::create( m_kafkaProducer, m_config.topicName, m_kafkaProducerTopicConf, errStr );
	    if ( topic == GUCEF_NULL )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to obtain Kafka Producer Topic handle for topic \"" +
                m_config.topicName + "\", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
	    }
        m_kafkaProducerTopic = topic;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:LoadConfig: Successfully created Kafka Producer Topic handle for topic: " + m_config.topicName );

    }

    if ( clientConfig.desiredFeatures.supportsSubscribing )
    {
        std::string errStr;

        RdKafka::Conf* kafkaConf = RdKafka::Conf::create( RdKafka::Conf::CONF_GLOBAL );
        kafkaConf->set( "metadata.broker.list", csvKafkaBrokerList, errStr );
	    kafkaConf->set( "event_cb", static_cast< RdKafka::EventCb* >( this ), errStr );
	    kafkaConf->set( "dr_cb", static_cast< RdKafka::DeliveryReportCb* >( this ), errStr );
        kafkaConf->set( "rebalance_cb", static_cast< RdKafka::RebalanceCb* >( this ), errStr );

        rd_kafka_conf_set_log_cb( kafkaConf->c_ptr_global(), &RdKafkaLogCallback );
        rd_kafka_conf_set_stats_cb( kafkaConf->c_ptr_global(), &RdKafkaStatsCallback );

        CKafkaPubSubClientConfig::StringMap::const_iterator m = clientConfig.kafkaConsumerGlobalConfigSettings.begin();
        while ( m != clientConfig.kafkaConsumerGlobalConfigSettings.end() )
        {
            if ( RdKafka::Conf::CONF_OK != kafkaConf->set( (*m).first, (*m).second, errStr ) )
            {
		        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to set Kafka Consumer global config entry \"" +
                        (*m).first + "\"=\"" + (*m).second + "\" for topic \"" + m_config.topicName + "\", error message: " + errStr );
                ++m_kafkaErrorReplies;
                return false;
            }
            ++m;
        }
        delete m_kafkaConsumerConf;
        m_kafkaConsumerConf = kafkaConf;

        #ifdef GUCEF_DEBUG_MODE
        bool tracePartitionEof = true;
        #else
        bool tracePartitionEof = clientConfig.desiredFeatures.supportsSubscriptionEndOfDataEvent;
        #endif
        m_kafkaConsumerConf->set( "enable.partition.eof", tracePartitionEof ? "true" : "false", errStr );

        // Apply default client level topic config as an overlay
        RdKafka::Conf* kafkaConsumerTopicConfig = RdKafka::Conf::create( RdKafka::Conf::CONF_TOPIC );
        m = clientConfig.kafkaConsumerDefaultTopicConfigSettings.begin();
        while ( m != clientConfig.kafkaConsumerDefaultTopicConfigSettings.end() )
        {
            if ( RdKafka::Conf::CONF_OK != kafkaConsumerTopicConfig->set( (*m).first, (*m).second, errStr ) )
            {
		        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClient:LoadConfig: Failed to set Kafka consumer topic config entry based on client wide config \"" +
                        (*m).first + "\"=\"" + (*m).second + "\" KafkaPubSubClientTopic topic \"" + m_config.topicName + "\", error message: " + errStr );
                ++m_kafkaErrorReplies;
                return false;
            }
            ++m;

        }

        // Now the topic level topic config (if any)
        m = m_config.kafkaConsumerTopicConfigSettings.begin();
        while ( m != m_config.kafkaConsumerTopicConfigSettings.end() )
        {
            if ( RdKafka::Conf::CONF_OK != kafkaConsumerTopicConfig->set( (*m).first, (*m).second, errStr ) )
            {
		        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to set Kafka consumer topic config entry \"" +
                        (*m).first + "\"=\"" + (*m).second + "\" for topic \"" + m_config.topicName + "\", error message: " + errStr );
                ++m_kafkaErrorReplies;
                return false;
            }
            ++m;

        }

        delete m_kafkaConsumerTopicConf;
        m_kafkaConsumerTopicConf = kafkaConsumerTopicConfig;

        if ( RdKafka::Conf::CONF_OK != m_kafkaConsumerConf->set( "default_topic_conf", m_kafkaConsumerTopicConf, errStr ) )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to set Kafka consumer default topic config, error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
        }

        bool autoCommit = clientConfig.desiredFeatures.supportsAutoMsgReceivedAck;
        if ( RdKafka::Conf::CONF_OK != m_kafkaConsumerConf->set( "enable.auto.commit", CORE::ToString( autoCommit ), errStr ) )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to set Kafka consumer global config's enable.auto.commit, error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
        }
        if ( RdKafka::Conf::CONF_OK != m_kafkaConsumerConf->set( "enable.auto.offset.store", CORE::ToString( autoCommit ), errStr ) )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to set Kafka consumer global config's enable.auto.offset.store, error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
        }
        m_consumerOffsetWaitsForExplicitMsgAck = !autoCommit;

        // Check for the mandatory group ID property.
        // This may have already been set by the generic custom property setting that occured above
        std::string confValue;
        m_kafkaConsumerConf->get( "group.id", confValue );
        if ( confValue.empty() )
        {
            if ( RdKafka::Conf::CONF_OK != m_kafkaConsumerConf->set( "group.id", m_config.consumerGroupName, errStr ) )
            {
		        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to set Kafka consumer group id to \"" +
                    m_config.consumerGroupName + "\", error message: " + errStr );
                ++m_kafkaErrorReplies;
                return false;
            }
        }

        RdKafka::KafkaConsumer* consumer = RdKafka::KafkaConsumer::create( m_kafkaConsumerConf, errStr );
	    if ( consumer == GUCEF_NULL )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to create Kafka consumer, error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
	    }
        m_kafkaConsumer = consumer;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:LoadConfig: Successfully created Kafka consumer" );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::PrepStorageForReadMsgs( CORE::UInt32 msgCount )
{GUCEF_TRACE;

    if ( msgCount > m_pubsubMsgs.size() )
    {
        // Add extra slots
        m_pubsubMsgs.resize( msgCount );
    }

    // reset size, note this does not dealloc the underlying memory
    TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs;
    msgRefs.clear();

    if ( msgCount > msgRefs.capacity() )
    {
        // Grow the actual storage, this is allowed to become larger than msgCount to limit
        // the nr of dynamic allocations
        msgRefs.reserve( msgCount );
    }
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    m_shouldBeConnected = false;

    bool totalSuccess = true;

    if ( GUCEF_NULL != m_kafkaConsumer )
    {
        RdKafka::ErrorCode response = m_kafkaConsumer->unsubscribe();
        if ( RdKafka::ERR_NO_ERROR != response )
        {
		    std::string errStr = RdKafka::err2str( response );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:Disconnect: Failed to unsubscribe() Kafka Consumer for topic \"" +
                m_config.topicName + ", error message: " + errStr );
            ++m_kafkaErrorReplies;
            totalSuccess = false;
        }
        response = m_kafkaConsumer->close();
        if ( RdKafka::ERR_NO_ERROR != response )
        {
		    std::string errStr = RdKafka::err2str( response );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:Disconnect: Failed to close() Kafka Consumer for topic \"" +
                m_config.topicName + ", error message: " + errStr );
            ++m_kafkaErrorReplies;
            totalSuccess = false;
        }
    }

    if ( GUCEF_NULL != m_kafkaProducer )
    {
        RdKafka::ErrorCode response = RdKafka::ERR_NO_ERROR;
        int waited = 0;
        int queuedMsgs = m_kafkaProducer->outq_len();
        while ( queuedMsgs > 0 && waited <= 30000 )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Disconnect: Waiting on Kafka Producer for topic: \"" +
                m_config.topicName + "\" to finish " + CORE::Int32ToString( queuedMsgs ) + " queued messages" );

            response = m_kafkaProducer->flush( 1000 );
            if ( RdKafka::ERR_NO_ERROR == response )
                break;

            waited += 1000;
            queuedMsgs = m_kafkaProducer->outq_len();
        }
        if ( waited > 30000 || RdKafka::ERR_NO_ERROR != response )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Disconnect: Timed out waiting on Kafka Producer for topic: \"" +
                m_config.topicName + "\" to finish " + CORE::Int32ToString( queuedMsgs ) + " queued messages. Shutdown will continue regardless. Data loss will occur!" );
            ++m_kafkaErrorReplies;
            totalSuccess = false;
        }
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    /*
        Per RdKafka docs:

        The high-level Kafka consumer (KafkaConsumer in C++) will start consuming at the last committed offset by default,
        if there is no previously committed offset for the topic+partition and group it will fall back on the topic configuration property auto.offset.reset which defaults to latest,
        thus starting to consume at the end of the partition (only new messages will be consumed).
    */

    MT::CScopeMutex lock( m_lock );
    m_shouldBeConnected = true;

    if ( !IsConnected() )
        if ( !SetupBasedOnConfig() )
            return false;

    std::vector< std::string > topics;
    topics.push_back( m_config.topicName );
    RdKafka::ErrorCode response = m_kafkaConsumer->subscribe( topics );
    if ( RdKafka::ERR_NO_ERROR != response )
    {
		std::string errStr = RdKafka::err2str( response );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to start Kafka Consumer for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::VariantToPartitionOffset( const CORE::CVariant& partitionOffsetBlob, TPartitionOffset& offset )
{GUCEF_TRACE;

    try
    {
        CORE::CDynamicBuffer blob;
        blob.LinkTo( partitionOffsetBlob );
        CORE::UInt32 byteOffset = 0;

        CORE::UInt32 partitionId = blob.AsConstType< CORE::UInt32 >( byteOffset );
        byteOffset += sizeof( CORE::UInt32 );
        offset.partitionId = partitionId;

        offset.partitionOffset = blob.AsConstType< CORE::Int64 >( byteOffset );
        byteOffset += sizeof( CORE::Int64 );

        return true;
    }
    catch ( CORE::CDynamicBuffer::EIllegalCast& )
    {

    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::VariantToPartitionOffsets( const CORE::CVariant& indexBookmarkBlob, TPartitionOffsetMap& offsets )
{GUCEF_TRACE;

    try
    {
        CORE::CDynamicBuffer blob;
        blob.LinkTo( indexBookmarkBlob );

        CORE::UInt32 partitionCount = (CORE::UInt32) blob.GetDataSize() / sizeof( TPartitionOffset );

        CORE::UInt32 offset = 0;
        for ( CORE::UInt32 i=0; i<partitionCount; ++i )
        {
            CORE::UInt32 partitionId = blob.AsConstType< CORE::UInt32 >( offset );
            offset += sizeof( CORE::UInt32 );

            TPartitionOffset& offsetEntry = offsets[ partitionId ];
            offsetEntry.partitionId = partitionId;

            offsetEntry.partitionOffset = blob.AsConstType< CORE::Int64 >( offset );
            offset += sizeof( CORE::Int64 );
        }

        return true;
    }
    catch ( CORE::CDynamicBuffer::EIllegalCast& )
    {

    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::SubscribeStartingAtTopicIndex( const CORE::CVariant& indexBookmark )
{GUCEF_TRACE;

    /*
        Per RdKafka docs:

        To manually set the starting offset for a partition the assign() API allows you to specify the start offset for each partition
        by setting the .offset field in the topic_partition_t element to either an absolute offset (>=0) or one of the logical offsets (BEGINNING, END, STORED, TAIL(..)).
    */

    std::string errStr;
    RdKafka::ErrorCode response;

    // First we need to set up RdKafka with the relevant internal stuctures for our topics
    std::vector< std::string > topics;
    topics.push_back( m_config.topicName );
    response = m_kafkaConsumer->subscribe( topics );
    if ( RdKafka::ERR_NO_ERROR != response )
    {
		std::string errStr = RdKafka::err2str( response );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: Failed to start Kafka Consumer for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    std::vector< RdKafka::TopicPartition* > partitions;

    // Now obtain the relevant partitions for our topic
    response = m_kafkaConsumer->position( partitions );
    if ( RdKafka::ERR_NO_ERROR != response )
    {
		m_kafkaConsumer->unsubscribe();
        std::string errStr = RdKafka::err2str( response );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: Failed to obtain current partitions for Kafka Consumer for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    // If a BLOB is passed we assume we are trying to set exact dictated offsets per partition
    // Using the same concept as in GetCurrentBookmark()
    if ( indexBookmark.IsBlob() )
    {
        TPartitionOffsetMap offsets;
        if ( !VariantToPartitionOffsets( indexBookmark, offsets ) )
        {
            m_kafkaConsumer->unsubscribe();
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: Failed to convert bookmark variant blob into partition offsets for Kafka Consumer for topic: " + m_config.topicName );
            return false;
        }

        // Now locally modify the offsets for the partitions that are still valid
        std::vector< RdKafka::TopicPartition* >::iterator i = partitions.begin();
        while ( i != partitions.end() )
        {
            TPartitionOffsetMap::iterator o = offsets.find( (*i)->partition() );
            if ( o != offsets.end() )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: Found offset stored in the blob bookmark for partition " +
                    CORE::ToString( (CORE::Int64)(*i)->partition() ) + ". Existing offset " + CORE::ToString( (CORE::Int64)(*i)->offset() ) + " will be replaced by " + CORE::ToString( (*o).second.partitionOffset ) + " for topic: " + m_config.topicName );

                // The offset given could be a numerical alias, we account for that here as well
                // if its truly a numerical offset this will just be pass through
                CORE::Int64 newOffset = ConvertKafkaConsumerStartOffset( (*o).second.partitionOffset ,
                                                                         (*i)->partition()           ,
                                                                         10000                       );

                if ( RdKafka::Topic::OFFSET_INVALID == newOffset )
                {

                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: Failed to convert numerical offset description for Kafka Consumer for topic \"" +
                        m_config.topicName + " to a valid offset, offset alias=" + CORE::ToString( (*o).second.partitionOffset ) + " partition=" + CORE::ToString( (*i)->partition() ) );
                    ++m_kafkaErrorReplies;

                    // We don't allow for partial success as that would leave us in an unpredictable state
                    m_kafkaConsumer->unsubscribe();
                    return false;
                }

                (*i)->set_offset( (*o).second.partitionOffset );
            }
            else
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: No offset was stored in the blob bookmark for partition " +
                    CORE::ToString( (CORE::Int64)(*i)->partition() ) + ". Will use existing offset " + CORE::ToString( (CORE::Int64)(*i)->offset() ) + " instead for topic: " + m_config.topicName );
            }

            ++i;
        }
    }
    else
    {
        CORE::CString startOffsetDescription = indexBookmark.AsString();

        // Now locally modify the offsets for the partitions based on the alias given
        std::vector< RdKafka::TopicPartition* >::iterator i = partitions.begin();
        while ( i != partitions.end() )
        {
            CORE::Int64 startOffset = ConvertKafkaConsumerStartOffset( startOffsetDescription ,
                                                                       (*i)->partition()      ,
                                                                       10000                  );


            if ( RdKafka::Topic::OFFSET_INVALID == startOffset )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: Failed to convert start offset description for Kafka Consumer for topic \"" +
                    m_config.topicName + " to a valid offset, startOffsetDescription=" + startOffsetDescription + " partition=" + CORE::ToString( (*i)->partition() ) );
                ++m_kafkaErrorReplies;

                // We don't allow for partial success as that would leave us in an unpredictable state
                m_kafkaConsumer->unsubscribe();
                return false;
            }
            (*i)->set_offset( startOffset );

            ++i;
        }
    }

    // Now persist our now locally modified offsets into RdKafka
    response = m_kafkaConsumer->assign( partitions );
    if ( RdKafka::ERR_NO_ERROR != response )
    {
		m_kafkaConsumer->unsubscribe();
        std::string errStr = RdKafka::err2str( response );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: Failed to assign() new offsets to current partitions for Kafka Consumer for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    // And finally persist our changes server-side
    if ( !CommitConsumerOffsets() )
    {
        m_kafkaConsumer->unsubscribe();
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtTopicIndex: Failed to commit new parition offsets for consumer to Kafka for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    std::string errStr;
    RdKafka::ErrorCode response = RdKafka::ERR_NO_ERROR;
    CORE::UInt64 ticksSinceEpoch = msgDtBookmark.ToUnixEpochBasedTicksInMillisecs();

    // First we need to set up RdKafka with the relevant internal stuctures for our topics
    std::vector< std::string > topics;
    topics.push_back( m_config.topicName );
    response = m_kafkaConsumer->subscribe( topics );
    if ( RdKafka::ERR_NO_ERROR != response )
    {
		std::string errStr = RdKafka::err2str( response );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtMsgDateTime: Failed to start Kafka Consumer for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    std::vector< RdKafka::TopicPartition* > partitions;

    // Now obtain the relevant partitions for our topic
    response = m_kafkaConsumer->position( partitions );
    if ( RdKafka::ERR_NO_ERROR != response )
    {
		m_kafkaConsumer->unsubscribe();
        std::string errStr = RdKafka::err2str( response );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtMsgDateTime: Failed to obtain current partitions for Kafka Consumer for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    // Now locally modify the offsets for the partitions based on the timestamp given
    std::vector< RdKafka::TopicPartition* >::iterator i = partitions.begin();
    while ( i != partitions.end() )
    {
        // IMPORTANT: The API for getting time based offsets is a bit wonkey in that you need to abuse the
        // offset field on input as a ticks since Epoch field. Due to the calling function context the RdKafka library will
        // know you actually filled it with time ticks and not with an absolute offset nr
        (*i)->set_offset( ticksSinceEpoch );
        ++i;
    }

    response = m_kafkaConsumer->offsetsForTimes( partitions, 10000 );
    if ( RdKafka::ERR_NO_ERROR != response )
    {
		m_kafkaConsumer->unsubscribe();
        std::string errStr = RdKafka::err2str( response );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtMsgDateTime: Failed to obtain offsetsForTimes() for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    // Now persist our now locally modified offsets into RdKafka
    response = m_kafkaConsumer->assign( partitions );
    if ( RdKafka::ERR_NO_ERROR != response )
    {
		m_kafkaConsumer->unsubscribe();
        std::string errStr = RdKafka::err2str( response );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtMsgDateTime: Failed to assign() new offsets to current partitions for Kafka Consumer for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    // And finally persist our changes server-side
    if ( !CommitConsumerOffsets() )
    {
        m_kafkaConsumer->unsubscribe();
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtMsgDateTime: Failed to commit new parition offsets for consumer to Kafka for topic \"" +
            m_config.topicName + ", error message: " + errStr );
        ++m_kafkaErrorReplies;
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CVariant& msgDtBookmark )
{GUCEF_TRACE;

    CORE::CDateTime msgDt;
    if ( msgDtBookmark.IsInteger() )
    {
        CORE::UInt64 dtInt = msgDtBookmark.AsUInt64( GUCEF_MT_UINT64MAX );
        if ( GUCEF_MT_UINT64MAX != dtInt  )
        {
            msgDt.FromUnixEpochBasedTicksInMillisecs( dtInt );
            return SubscribeStartingAtMsgDateTime( msgDt );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:SubscribeStartingAtBookmark: Failed to use Unix epoch based ticks in millisecs based timestamp. Bookmark=" + msgDtBookmark.AsString() );
        }
    }
    else
    {
        if ( msgDt.FromIso8601DateTimeString( msgDtBookmark.AsString() ) )
        {
            return SubscribeStartingAtMsgDateTime( msgDt );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:SubscribeStartingAtBookmark: Failed to parse ISO 8601 string based timestamp. Bookmark=" + msgDtBookmark.AsString() );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::SubscribeStartingAtBookmark( const PUBSUB::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    m_shouldBeConnected = true;

    if ( !IsConnected() )
        if ( !SetupBasedOnConfig() )
            return false;

    switch ( bookmark.GetBookmarkType() )
    {
        case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_MSG_ID:
        {
            // This is not supported in Kafka
            // You can seek a stream based on index (offset) or with some effort get said index based on a timestamp
            // duplicate IDs (keys) are to be expected as they are used for partition routing/sharding purposes
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:SubscribeStartingAtBookmark: Cannot subscribe with a Msg ID based bookmark when using Kafka. Bookmark=" + bookmark.GetBookmarkData().AsString() );
            break;
        }
        case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_MSG_DATETIME:
        {
            return SubscribeStartingAtMsgDateTime( bookmark.GetBookmarkData() );
        }
        case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_MSG_INDEX:
        case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_TOPIC_INDEX:
        {
            return SubscribeStartingAtTopicIndex( bookmark.GetBookmarkData() );
        }
        default:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:SubscribeStartingAtBookmark: Cannot subscribe with the bookmark type given when using Kafka. Type=" + CORE::ToString( (CORE::Int32) bookmark.GetBookmarkType() ) + ". Bookmark=" + bookmark.GetBookmarkData().AsString() );
            break;
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubBookmark
CKafkaPubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    try
    {
        MT::CScopeMutex lock( m_lock );

        // In Kafka there is no singular offset or such per topic. Such bookmarks are per partition instead for which a topic represents 1-N of them
        // This poses a problem when trying to represent this as a unified bookmark concept
        // To remedy we encode/serialize all partition IDs and their offsets into a blob bookmark
        // This will achieve the aim, to the extent possible with Kafka, of being able to retain this information client-side as a unified concept

        if ( !m_consumerOffsets.empty() )
        {
            CORE::CDynamicBuffer consumerOffsetsBuffer( m_consumerOffsets.size() * ( sizeof(CORE::UInt32) + sizeof(CORE::Int64) ) );

            CORE::UInt32 byteOffset = 0;
            TInt32ToInt64Map::iterator i = m_consumerOffsets.begin();
            while ( i != m_consumerOffsets.end() )
            {
                *consumerOffsetsBuffer.AsTypePtr< CORE::UInt32 >( byteOffset ) = (*i).first;
                byteOffset += sizeof(CORE::UInt32);
                *consumerOffsetsBuffer.AsTypePtr< CORE::Int64 >( byteOffset ) = (*i).second;
                byteOffset += sizeof(CORE::Int64);

                consumerOffsetsBuffer.SetDataSize( byteOffset );
                ++i;
            }

            return PUBSUB::CPubSubBookmark( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_TOPIC_INDEX, CORE::CVariant( consumerOffsetsBuffer ) );
        }
    }
    catch ( CORE::CDynamicBuffer::EIllegalCast& )
    {
    }

    // Cannot provide a bookmark at this time
    return PUBSUB::CPubSubBookmark( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_NOT_AVAILABLE );
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::AcknowledgeReceipt( const TPartitionOffset& offset )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    TInt32ToInt64Map::iterator p = m_consumerOffsets.find( offset.partitionId );
    if ( p != m_consumerOffsets.end() )
    {
        CORE::Int64& currentOffset = (*p).second;
        if ( offset.partitionOffset > currentOffset )
        {
            currentOffset = offset.partitionOffset;
            m_msgsReceivedSinceLastOffsetCommit = true;
        }

        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::AcknowledgeReceipt( const PUBSUB::CIPubSubMsg& msg )
{GUCEF_TRACE;

    TPartitionOffset offset;
    if ( VariantToPartitionOffset( msg.GetMsgIndex(), offset ) )
    {
        return AcknowledgeReceipt( offset );
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:AcknowledgeReceipt: Failed to obtain partition offset from Msg Index field" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::AcknowledgeReceipt( const PUBSUB::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    if ( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_MSG_INDEX == bookmark.GetBookmarkType() )
    {
        TPartitionOffset offset;
        if ( VariantToPartitionOffset( bookmark.GetBookmarkData(), offset ) )
        {
            return AcknowledgeReceipt( offset );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:AcknowledgeReceipt: Failed to obtain partition offset from bookmark data" );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:AcknowledgeReceipt: Unsupported bookmark type, expected bookmark type is MSG_INDEX" );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::IsConnected( void ) const
{GUCEF_TRACE;

    // Snapshot in time as such no lock needed.
    // note that connected is a very relative term here, more like prepared to connect as needed
    return ( m_config.needPublishSupport && GUCEF_NULL != m_kafkaProducer ) || ( m_config.needSubscribeSupport && GUCEF_NULL != m_kafkaConsumer );
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::IsHealthy( void ) const
{GUCEF_TRACE;

    bool newHealthyState = true;

    if ( m_config.maxKafkaErrorsToBeHealthy >= 0 )
    {
        // Current and last metrics cycle error count counts against the max
        if ( m_kafkaErrorReplies + m_metrics.kafkaErrorReplies > (CORE::UInt32) m_config.maxKafkaErrorsToBeHealthy )
            newHealthyState = false;
    }

    // Aside from error count, having to reconnect while we should be connected is also not good
    if ( newHealthyState && m_shouldBeConnected && !IsConnected() )
        newHealthyState = false;

    // See if this is a change from the current persisted state
    if ( newHealthyState != m_isHealthy )
    {
        m_isHealthy = newHealthyState;

        if ( m_isHealthy )
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:IsHealthy: Topic " + m_config.topicName + " is now healthy" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:IsHealthy: Topic " + m_config.topicName + " is now unhealthy" );         
        }

        THealthStatusChangeEventData eData( newHealthyState ); 
        NotifyObservers( HealthStatusChangeEvent, &eData ); 
    }

    return newHealthyState;
}            

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    // We dont have any per-topic connectivity setup work that is
    // not tied directly to publishing/subscribing. RdKafka takes care of all that.

    // We can take the oppertunity to perform a settings sanity check though...

    MT::CScopeMutex lock( m_lock );

    if ( GUCEF_NULL == m_client )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:InitializeConnectivity: Internal error: Parent client object is missing" );
        return false;
    }

    const CKafkaPubSubClientConfig& clientConfig = m_client->GetConfig();

    if ( clientConfig.remoteAddresses.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:InitializeConnectivity: No Kafka broker addressess have been provided" );
        return false;
    }

    if ( m_config.topicName.IsNULLOrEmpty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:InitializeConnectivity: No Kafka topic name" );
        return false;
    }

    if ( !m_config.needPublishSupport && !m_config.needSubscribeSupport )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:InitializeConnectivity: topic config claims no need for publishing or subscribing and neither will be available. Dummy mode only" );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CKafkaPubSubClientTopic::GetKafkaErrorRepliesCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 redisErrorReplies = m_kafkaErrorReplies;
        m_kafkaErrorReplies = 0;
        return redisErrorReplies;
    }
    else
        return m_kafkaErrorReplies;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CKafkaPubSubClientTopic::GetKafkaMsgsTransmittedCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 kafkaMsgsTransmitted = m_kafkaMsgsTransmitted;
        m_kafkaMsgsTransmitted = 0;
        return kafkaMsgsTransmitted;
    }
    else
        return m_kafkaMsgsTransmitted;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CKafkaPubSubClientTopic::GetKafkaMsgsReceivedCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 kafkaMsgsReceived = m_kafkaMessagesReceived;
        m_kafkaMessagesReceived = 0;
        return kafkaMsgsReceived;
    }
    else
        return m_kafkaMessagesReceived;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CKafkaPubSubClientTopic::GetKafkaMsgsFilteredCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 kafkaMsgsFiltered = m_kafkaMessagesFiltered;
        m_kafkaMessagesFiltered = 0;
        return kafkaMsgsFiltered;
    }
    else
        return m_kafkaMessagesFiltered;
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientTopic::TopicMetrics::TopicMetrics( void )
    : kafkaMessagesTransmitted( 0 )
    , kafkaTransmitQueueSize( 0 )
    , kafkaTransmitOverflowQueueSize( 0 )
    , kafkaMessagesReceived( 0 )
    , kafkaMessagesFiltered( 0 )
    , kafkaErrorReplies( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    const CKafkaPubSubClientConfig& clientConfig = m_client->GetConfig();

    m_metrics.kafkaErrorReplies = GetKafkaErrorRepliesCounter( true );
    if ( clientConfig.desiredFeatures.supportsPublishing )
    {
        m_metrics.kafkaTransmitQueueSize = (CORE::UInt32) m_kafkaProducer->outq_len();
        m_metrics.kafkaMessagesTransmitted = GetKafkaMsgsTransmittedCounter( true );
    }
    if ( clientConfig.desiredFeatures.supportsSubscribing )
    {
        m_metrics.kafkaMessagesReceived = GetKafkaMsgsReceivedCounter( true );
        m_metrics.kafkaMessagesFiltered = GetKafkaMsgsFilteredCounter( true );
    }
}

/*-------------------------------------------------------------------------*/

const CKafkaPubSubClientTopic::TopicMetrics&
CKafkaPubSubClientTopic::GetMetrics( void ) const
{GUCEF_TRACE;

    return m_metrics;
}

/*-------------------------------------------------------------------------*/

const MT::CILockable*
CKafkaPubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CKafkaPubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------*/

CORE::Int64
CKafkaPubSubClientTopic::ConvertKafkaConsumerStartOffset( const CORE::CString& startOffsetDescription ,
                                                          CORE::Int32 partitionId                     ,
                                                          CORE::Int32 timeoutInMs                     )
{GUCEF_TRACE;

    CORE::CString testString = startOffsetDescription.Lowercase();
    if ( testString.IsNULLOrEmpty() )
    {
        testString = "stored";
    }

    if ( "beginning" == testString )
    {
        return ConvertKafkaConsumerStartOffset( RdKafka::Topic::OFFSET_BEGINNING, partitionId, timeoutInMs );
    }
    if ( "end" == testString )
    {
        return ConvertKafkaConsumerStartOffset( RdKafka::Topic::OFFSET_END, partitionId, timeoutInMs );
    }
    if ( "stored" == testString )
    {
        return ConvertKafkaConsumerStartOffset( RdKafka::Topic::OFFSET_STORED, partitionId, timeoutInMs );
    }

    return ConvertKafkaConsumerStartOffset( CORE::StringToInt64( testString ), partitionId, timeoutInMs );
}

/*-------------------------------------------------------------------------*/

CORE::Int64
CKafkaPubSubClientTopic::ConvertKafkaConsumerStartOffset( CORE::Int64 startOffsetDescription ,
                                                          CORE::Int32 partitionId            ,
                                                          CORE::Int32 timeoutInMs            )
{GUCEF_TRACE;

    if ( RdKafka::Topic::OFFSET_BEGINNING == startOffsetDescription )
    {
        int64_t high = 0; int64_t low = 0;
        RdKafka::ErrorCode err = m_kafkaConsumer->query_watermark_offsets( m_config.topicName, partitionId, &low, &high, timeoutInMs );
        if ( RdKafka::ERR_NO_ERROR == err )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Converted offset description \"BEGINNING\" into offset " + CORE::Int64ToString( low ) + " for partition " + CORE::Int32ToString( partitionId ) );
            return low;
        }

        std::string errStr = RdKafka::err2str( err );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Failed to convert offset description \"BEGINNING\" into offset for partition " +
                CORE::Int32ToString( partitionId ) + ". ErrorCode : " + errStr );
        return RdKafka::Topic::OFFSET_INVALID;
    }
    if ( RdKafka::Topic::OFFSET_END == startOffsetDescription )
    {
        int64_t high = 0; int64_t low = 0;
        RdKafka::ErrorCode err = m_kafkaConsumer->query_watermark_offsets( m_config.topicName, partitionId, &low, &high, timeoutInMs );
        if ( RdKafka::ERR_NO_ERROR == err )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Converted offset description \"END\" into offset " + CORE::Int64ToString( low ) + " for partition " + CORE::Int32ToString( partitionId ) );
            return high;
        }

        std::string errStr = RdKafka::err2str( err );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Failed to convert offset description \"END\" into offset for partition " +
                CORE::Int32ToString( partitionId ) + ". ErrorCode : " + errStr );
        return RdKafka::Topic::OFFSET_INVALID;
    }
    if ( RdKafka::Topic::OFFSET_STORED == startOffsetDescription )
    {
        std::vector<RdKafka::TopicPartition*> partitions;
        RdKafka::ErrorCode err = m_kafkaConsumer->assignment( partitions );
        if ( RdKafka::ERR_NO_ERROR != err )
        {
            std::string errStr = RdKafka::err2str( err );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Failed to convert offset description \"STORED\" into offset for partition " +
                    CORE::Int32ToString( partitionId ) + ". Cannot obtain partition assignment. ErrorCode : " + errStr );
            return RdKafka::Topic::OFFSET_INVALID;
        }

        err = m_kafkaConsumer->committed( partitions, timeoutInMs );
        if ( RdKafka::ERR_NO_ERROR != err )
        {
            std::string errStr = RdKafka::err2str( err );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Failed to convert offset description \"STORED\" into offset for partition " +
                    CORE::Int32ToString( partitionId ) + ". Cannot obtain commited offsets . ErrorCode : " + errStr );
            return RdKafka::Topic::OFFSET_INVALID;
        }

        std::vector<RdKafka::TopicPartition*>::iterator i = partitions.begin();
        while ( i != partitions.end() )
        {
            if ( (*i)->partition() == partitionId )
            {
                CORE::Int64 storedOffset = (*i)->offset();
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Converted offset description \"STORED\" into offset " +
                        CORE::Int64ToString( storedOffset ) + " for partition " + CORE::Int32ToString( partitionId ) );
                return storedOffset;
            }
            ++i;
        }


        return RdKafka::Topic::OFFSET_STORED;
    }

    // Assume its already a numeric offset
    return startOffsetDescription;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CKafkaPubSubClientTopic::ConvertKafkaConsumerStartOffset( CORE::Int64 offset )
{GUCEF_TRACE;

    if ( RdKafka::Topic::OFFSET_STORED == offset )
        return "stored";
    if ( RdKafka::Topic::OFFSET_BEGINNING == offset )
        return "beginning";
    if ( RdKafka::Topic::OFFSET_END == offset )
        return "end";
    if ( RdKafka::Topic::OFFSET_INVALID == offset )
        return "invalid";

    return CORE::Int64ToString( offset );
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::offset_commit_cb( RdKafka::ErrorCode err                         ,
                                           std::vector<RdKafka::TopicPartition*>& offsets )
{GUCEF_TRACE;

    if ( RdKafka::ERR_NO_ERROR != err && RdKafka::ERR__NO_OFFSET != err )
    {
        std::string errStr = RdKafka::err2str( err );
        CORE::CString commitInfo = "KafkaPubSubClientTopic:offset_commit_cb: Member ID \"" + m_kafkaConsumer->memberid() + "\": Code: " + errStr + " : ";
        for ( unsigned int i=0; i<offsets.size(); ++i )
        {
            commitInfo += "Topic \"" + offsets[ i ]->topic() + "\" is at partition \"" + CORE::Int32ToString( offsets[ i ]->partition() ).STL_String() + "\" at offset \"" + ConvertKafkaConsumerStartOffset( offsets[ i ]->offset() ).STL_String() + "\". ";
        }
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, commitInfo );
    }
    #ifdef GUCEF_DEBUG_MODE
    else
    {
        std::string errStr = RdKafka::err2str( err );
        CORE::CString commitInfo = "KafkaPubSubClientTopic:offset_commit_cb: Member ID \"" + m_kafkaConsumer->memberid() + "\": Code: " + errStr + " : ";
        for ( unsigned int i=0; i<offsets.size(); ++i )
        {
            commitInfo += "Topic \"" + offsets[ i ]->topic() + "\" is at partition \"" + CORE::Int32ToString( offsets[ i ]->partition() ).STL_String() + "\" at offset \"" + ConvertKafkaConsumerStartOffset( offsets[ i ]->offset() ).STL_String() + "\". ";
        }
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, commitInfo );
    }
    #endif
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::rebalance_cb( RdKafka::KafkaConsumer* consumer                  ,
                                       RdKafka::ErrorCode err                            ,
                                       std::vector<RdKafka::TopicPartition*>& partitions )
{GUCEF_TRACE;

    std::string actionStr = "<UNKNOWN>";
    if ( err == RdKafka::ERR__ASSIGN_PARTITIONS )
    {
        RdKafka::ErrorCode assignSuccess = consumer->assign( partitions );
        for ( unsigned int i=0; i<partitions.size(); ++i )
        {
            if ( m_firstPartitionAssignment || RdKafka::Topic::OFFSET_INVALID == partitions[ i ]->offset() )
            {
                CORE::Int64 startOffset = ConvertKafkaConsumerStartOffset( m_config.consumerModeStartOffset, partitions[ i ]->partition(), 10000 );
                partitions[ i ]->set_offset( startOffset );
                m_consumerOffsets[ partitions[ i ]->partition() ] = startOffset;
            }
        }
        if (  m_firstPartitionAssignment )
            m_firstPartitionAssignment = false;

        assignSuccess = consumer->assign( partitions );
        if ( RdKafka::ERR_NO_ERROR != assignSuccess )
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:rebalance_cb: Failed to assign new partitions" );

        RdKafka::ErrorCode ec = consumer->offsets_store( partitions );
        RdKafka::ErrorCode ec2 = consumer->commitSync( this );
        actionStr = "ASSIGN_PARTITIONS";
    }
    else
    if ( err == RdKafka::ERR__REVOKE_PARTITIONS )
    {
        consumer->unassign();
        for ( unsigned int i=0; i<partitions.size(); ++i )
        {
            m_consumerOffsets.erase( partitions[ i ]->partition() );
        }
        actionStr = "REVOKE_PARTITIONS";
    }

    CORE::CString partitionInfo = "KafkaPubSubClientTopic:rebalance_cb: Member ID \"" + consumer->memberid() + "\": Action " + actionStr + " : ";
    for ( unsigned int i=0; i<partitions.size(); ++i )
    {
        partitionInfo += "Topic \"" + partitions[ i ]->topic() + "\" is at partition \"" + CORE::Int32ToString( partitions[ i ]->partition() ).STL_String() +
                "\" at offset \"" + ConvertKafkaConsumerStartOffset( partitions[ i ]->offset() ).STL_String() + "\". ";

        int64_t high = 0; int64_t low = 0;
        RdKafka::ErrorCode err = m_kafkaConsumer->get_watermark_offsets( m_config.topicName, partitions[ i ]->partition(), &low, &high );
        if ( RdKafka::ERR_NO_ERROR == err )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic: Offsets for topic \"" + m_config.topicName +
                    "\" and partition " + CORE::Int32ToString( partitions[ i ]->partition() ) + ": Low=" + ConvertKafkaConsumerStartOffset( low ) +
                    ", High=" + ConvertKafkaConsumerStartOffset( high ) );
        }
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, partitionInfo );
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::UpdateIsHealthyStatus( bool newStatus )
{GUCEF_TRACE;

    // See if this is a change from the current persisted state
    if ( newStatus != m_isHealthy )
    {
        // Make sure we use the same consistent logic to determine health
        // the caller might only know about one aspect of health
        // The IsHealthy check itself will update the status and perform eventing if needed
        newStatus = IsHealthy();
    }    
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::NotifyOfReceivedMsg( RdKafka::Message& message )
{GUCEF_TRACE;

    PrepStorageForReadMsgs( 1 );

    // Grab a message wrapper from pre-allocated storage
    PUBSUB::CBasicPubSubMsg& msgWrap = m_pubsubMsgs[ 0 ];
    msgWrap.Clear();
    msgWrap.SetOriginClientTopic( this );

    msgWrap.SetReceiveActionId( m_currentReceiveActionId );
    ++m_currentReceiveActionId;

    if ( message.key_len() > 0 )
    {
        if ( GUCEF_NULL != message.key() )
        {
            msgWrap.GetMsgId().LinkTo( message.key()->c_str(), (CORE::UInt32) message.key_len(), GUCEF_DATATYPE_UTF8_STRING );
        }
        else
        {
            msgWrap.GetMsgId().LinkTo( message.key_pointer(), (CORE::UInt32) message.key_len(), GUCEF_DATATYPE_BINARY_BLOB );
        }
    }

    TPartitionOffset* partitionOffset = msgWrap.GetMsgIndex().AsBsobPtr< TPartitionOffset >();
    partitionOffset->partitionId = message.partition();
    partitionOffset->partitionOffset = message.offset();

    msgWrap.GetPrimaryPayload().LinkTo( message.payload(), (CORE::UInt32) message.len() );

    RdKafka::MessageTimestamp msgTimestamp = message.timestamp();
    switch ( msgTimestamp.type )
    {
        case RdKafka::MessageTimestamp::MSG_TIMESTAMP_CREATE_TIME:      // Message creation time (source)
        case RdKafka::MessageTimestamp::MSG_TIMESTAMP_LOG_APPEND_TIME:  // Message log append time (broker)
        {
            msgWrap.GetMsgDateTime().FromUnixEpochBasedTicksInMillisecs( (CORE::UInt64 ) msgTimestamp.timestamp );
            break;
        }
        case RdKafka::MessageTimestamp::MSG_TIMESTAMP_NOT_AVAILABLE:    // Timestamp not available
        default:
        {
            msgWrap.GetMsgDateTime() = CORE::CDateTime::NowUTCDateTime();
        }
    }

    // Kafka does not differentiate between meta-data vs non-meta-data key-value pairs
    // as a feature to enhance out-of-the-box compatibility with other backend types we support stripping a prefix that allows detecting
    // of such a differentiation
    const RdKafka::Headers* headers = message.headers();
    if ( GUCEF_NULL != headers )
    {
        std::vector<RdKafka::Headers::Header> hdrs = headers->get_all();
        for ( size_t i=0; i<hdrs.size(); ++i )
        {
            // sadly not available as a reference... dynamic allocation
            CORE::CString headerKeyStr = hdrs[ i ].key();
            CORE::CVariant headerKey;
            CORE::CVariant headerValue( hdrs[ i ].value(), (CORE::UInt32) hdrs[ i ].value_size() );

            if ( !m_config.stripPrefixForMetaDataKvPairs && !m_config.stripPrefixForKvPairs )
            {
                headerKey = headerKeyStr;
                msgWrap.AddKeyValuePair( headerKey, headerValue );
            }
            else
            if ( m_config.stripPrefixForMetaDataKvPairs && 0 == headerKeyStr.HasSubstr( m_config.prefixToAddForMetaDataKvPairs, true ) )
            {
                headerKey = headerKeyStr.CutChars( m_config.prefixToAddForMetaDataKvPairs.Length(), true, 0 );
                msgWrap.AddMetaDataKeyValuePair( headerKey, headerValue );
            }
            else
            if ( m_config.stripPrefixForKvPairs && 0 == headerKeyStr.HasSubstr( m_config.prefixToAddForKvPairs, true ) )
            {
                headerKey = headerKeyStr.CutChars( m_config.prefixToAddForKvPairs.Length(), true, 0 );
                msgWrap.AddKeyValuePair( headerKey, headerValue );
            }
            else
            {
                msgWrap.AddKeyValuePair( headerKey, headerValue );
            }
        }
    }

    // Now that we have prepared our wrapped message let's link it
    // as a reference in the list of received messages we send out
    TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs;
    msgRefs.push_back( CPubSubClientTopic::TPubSubMsgRef() );
    msgRefs.back().LinkTo( &msgWrap );

    // Communicate all the messages received via an event notification
    NotifyObservers( MsgsRecievedEvent, &m_pubsubMsgsRefs );
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::consume_cb( RdKafka::Message& message, void* opaque )
{GUCEF_TRACE;

    switch ( message.err() )
    {
        case RdKafka::ERR__TIMED_OUT:
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Kafka consume error: TIMED_OUT: " + message.errstr() );
            UpdateIsHealthyStatus( false );
            break;
        }
        case RdKafka::ERR_NO_ERROR:
        {
            #ifdef GUCEF_DEBUG_MODE
            if ( GUCEF_NULL != message.key() )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "KafkaPubSubClientTopic:consume_cb: Received " + CORE::UInt64ToString( CORE::UInt64( message.len() ) ) + " byte message on topic \"" +
                        m_config.topicName + "\" with key \"" + *message.key() + "\" and offset " + CORE::Int64ToString( message.offset() ) + " for partition " + CORE::ToString( message.partition() ) );
            }
            else
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "KafkaPubSubClientTopic:consume_cb: Received " + CORE::UInt64ToString( CORE::UInt64( message.len() ) ) + " byte message on topic \"" +
                        m_config.topicName + " without a key and with offset " + CORE::Int64ToString( message.offset() ) + " for partition " + CORE::ToString( message.partition() ) );
            }
            #endif

            ++m_kafkaMessagesReceived;

            bool isFiltered = false;
            if ( m_config.useKafkaMsgHeadersForConsumerFiltering )
            {
                const RdKafka::Headers* headers = message.headers();
                if ( GUCEF_NULL != headers && !m_config.kafkaMsgHeaderUsedForFiltering.empty() )
                {
                    std::vector<RdKafka::Headers::Header> hdrs = headers->get( m_config.kafkaMsgHeaderUsedForFiltering );
                    for ( size_t i=0; i<hdrs.size(); ++i )
                    {
                        const char* hdrValue = hdrs[ i ].value_string();
                        if ( GUCEF_NULL != hdrValue )
                        {
                            CORE::CString::StringVector::iterator v = m_config.kafkaMsgValuesUsedForFiltering.begin();
                            while ( v != m_config.kafkaMsgValuesUsedForFiltering.end() )
                            {
                                if ( (*v) == hdrValue )
                                {
                                    isFiltered = true;
                                    ++m_kafkaMessagesFiltered;

                                    // A filtered message also counts as successfully handled
                                    // As such we need to update the offset so that its taken into account for a later commit of said offsets
                                    CORE::Int64& offset = m_consumerOffsets[ message.partition() ];
                                    if ( message.offset() > offset )
                                    {
                                        offset = message.offset();

                                        // Even when using an explicit ack this counts as the ack since the msg will never leave the backend as it was filtered
                                        m_msgsReceivedSinceLastOffsetCommit = true;
                                    }

                                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "KafkaPubSubClientTopic:consume_cb: Filtered message on topic \"" +
                                            m_config.topicName + " with offset " + CORE::Int64ToString( message.offset() ) );
                                    break;
                                }
                                ++v;
                            }
                        }
                        if ( isFiltered )
                            break;
                    }
                }
            }

            if ( !isFiltered )
            {
                if ( !m_consumerOffsetWaitsForExplicitMsgAck )
                {
                    // Right away update the local offsets, don't wait for AcknowledgeReceipt()
                    CORE::Int64& offset = m_consumerOffsets[ message.partition() ];
                    if ( message.offset() > offset )
                    {
                        offset = message.offset();
                        m_msgsReceivedSinceLastOffsetCommit = true;  // Since something was recieved we want to set the flag to ensure we start commiting offsets again
                    }
                }
                NotifyOfReceivedMsg( message );
            }
            
            // We will consider the ability to receive and process messages as healthy
            UpdateIsHealthyStatus( true );
            break;
        }
        case RdKafka::ERR__PARTITION_EOF:
        {
            // Last message that was available has been read
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "KafkaPubSubClientTopic: Kafka topic \"" + m_config.topicName + "\" doesnt have any new messages waiting to be consumed" );

            if ( m_client->GetConfig().desiredFeatures.supportsSubscriptionEndOfDataEvent )
            {
                // We need to see if we have reached EOF across all partitions not just the partition for which the RdKafka event was sent.
                // @TODO
            }

            break;
        }
        case RdKafka::ERR__UNKNOWN_PARTITION:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Kafka consume error: UNKNOWN_PARTITION: " + message.errstr() );
            UpdateIsHealthyStatus( false );
            break;
        }
        case RdKafka::ERR__UNKNOWN_TOPIC:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Kafka consume error: UNKNOWN_TOPIC: " + message.errstr() );
            UpdateIsHealthyStatus( false );
            break;
        }
        case RdKafka::ERR__MAX_POLL_EXCEEDED:
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Kafka consume error: MAX_POLL_EXCEEDED: " + message.errstr() );
            break;
        }
        default:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Kafka consume error: " + message.errstr() );
            UpdateIsHealthyStatus( false );
            break;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::event_cb( RdKafka::Event& event )
{GUCEF_TRACE;

    switch ( event.type() )
    {
        case RdKafka::Event::EVENT_ERROR:
        {
            switch (  event.err() )
            {
                #ifdef GUCEF_DEBUG_MODE
                case RdKafka::ERR__PARTITION_EOF:
                {
                    // Last message that was available has been read
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Kafka topic \"" + m_config.topicName + "\" doesnt have any new messages waiting to be consumed" );
                    break;
                }
                #endif
                case RdKafka::ERR__RESOLVE:
                {
                    // Per GitHub comment from edenhill on Dec 18, 2018 for issue #2159:
                    //  "It will re-resolve the address on each re-connect attempt, but it will not log equal sub-sequent errors."
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:event_cb: Unable to resolve Kafka broker DNS for Kafka topic \"" + m_config.topicName +
                        "\". Wrong DNS? event msg=\"" + event.str() + "\". error as string =\"" + RdKafka::err2str( event.err() ) + "\"" );
                    if ( !NotifyObservers( ConnectionErrorEvent ) ) return;
                    UpdateIsHealthyStatus( false );
                    break;
                }
                case RdKafka::ERR__TRANSPORT:
                {             
                    // Per GitHub comment from edenhill on Dec 18, 2018 for issue #2159:
                    //  "It will re-resolve the address on each re-connect attempt, but it will not log equal sub-sequent errors."
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:event_cb: Unable to establish or retain connection to Kafka brokers for Kafka topic \"" + m_config.topicName + 
                        "\". event msg=\"" + event.str() + "\". error as string =\"" + RdKafka::err2str( event.err() ) + "\"" );
                    if ( !NotifyObservers( ConnectionErrorEvent ) ) return;
                    UpdateIsHealthyStatus( false );
                    break;
                }
                default:
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:event_cb: Kafka error: " + RdKafka::err2str( event.err() ) + " from KafkaEventCallback()" );
                    break;
                }
            }
            break;
        }
        case RdKafka::Event::EVENT_STATS:
        {
	        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Kafka stats: " + event.str() );
	        break;
        }
        case RdKafka::Event::EVENT_LOG:
        {
	        switch ( event.severity() )
            {
                case RdKafka::Event::EVENT_SEVERITY_ALERT:
                case RdKafka::Event::EVENT_SEVERITY_EMERG:
                case RdKafka::Event::EVENT_SEVERITY_CRITICAL:
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "Kafka log: " + event.fac() + " : " + event.str() );
                    break;
                }
                case RdKafka::Event::EVENT_SEVERITY_ERROR:
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Kafka log: " + event.fac() + " : " + event.str() );
                    break;
                }
                case RdKafka::Event::EVENT_SEVERITY_WARNING:
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Kafka log: " + event.fac() + " : " + event.str() );
                    break;
                }
                case RdKafka::Event::EVENT_SEVERITY_INFO:
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Kafka log: " + event.fac() + " : " + event.str() );
                    break;
                }
                case RdKafka::Event::EVENT_SEVERITY_DEBUG:
                {
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Kafka log: " + event.fac() + " : " + event.str() );
                    break;
                }

                case RdKafka::Event::EVENT_SEVERITY_NOTICE:
                default:
                {
                    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka log: " + event.fac() + " : " + event.str() );
                    break;
                }
            }
	        break;
        }
        case RdKafka::Event::EVENT_THROTTLE:
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Kafka Throttle event: throttled for " + CORE::Int32ToString( event.throttle_time() ) + "ms by broker " +
                event.broker_name() + " with ID " + CORE::Int32ToString( event.broker_id() ) );
            NotifyObservers( PublishThrottleEvent );
            break;
        }
        default:
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka event: " + CORE::Int32ToString( (CORE::Int32) event.type() ) + ", with error code " + RdKafka::err2str( event.err() ) );
	        break;
        }
    }
}

/*-------------------------------------------------------------------------*/

const std::string&
CKafkaPubSubClientTopic::MsgStatusToString( RdKafka::Message::Status status )
{GUCEF_TRACE;

    switch ( status )
    {
        case RdKafka::Message::Status::MSG_STATUS_NOT_PERSISTED:
        {
            static const std::string statusStr = "status:not_persisted";
            return statusStr;
        }
        case RdKafka::Message::Status::MSG_STATUS_POSSIBLY_PERSISTED:
        {
            static const std::string statusStr = "status:possibly_persisted";
            return statusStr;
        }
        case RdKafka::Message::Status::MSG_STATUS_PERSISTED:
        {
            static const std::string statusStr = "status:persisted";
            return statusStr;
        }
        default:
        {
            static const std::string statusStr = "<unknown status>";
            return statusStr;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::dr_cb( RdKafka::Message& message )
{GUCEF_TRACE;

    static const std::string NULLstr = "NULL";

    bool notify = GUCEF_NULL != message.msg_opaque();
    CORE::UInt64 publishActionId = 0;

    // We decode the publish action Id from the passthrough opaque void*
    // This saved us from having to perform another memory allocation
    #ifdef GUCEF_64BIT
    publishActionId = reinterpret_cast< CORE::UInt64 >( message.msg_opaque() );
    #else
    publishActionId = reinterpret_cast< CORE::UInt32 >( message.msg_opaque() );
    if ( m_currentPublishActionId > GUCEF_MT_UINT32MAX || publishActionId < m_currentPublishActionId )
    {
        publishActionId += GUCEF_MT_UINT32MAX;
    }
    #endif

    if ( message.err() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Kafka delivery report: publishActionId: " + CORE::ToString( publishActionId ) +
                                                ". error: \"" + message.errstr() +
                                                "\", on topic: \"" + message.topic_name() +
                                                "\", key: " + ( message.key() ? (*message.key()) : NULLstr ) +
                                                ", payload size: " + CORE::ToString( message.len() ).STL_String() +
                                                ", msg has status: " + MsgStatusToString( message.status() ) );

        if ( notify )
        {
            m_publishFailureActionIds.push_back( publishActionId );
        }
    }
    else
    {
        ++m_kafkaMsgsTransmitted;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Kafka delivery report: publishActionId: " + CORE::ToString( publishActionId ) +
                                                ". status: " + MsgStatusToString( message.status() ) +
                                                " , topic: \"" + message.topic_name() +
                                                "\", partition: " + CORE::ToString( (CORE::Int64)message.partition() ).STL_String() +
                                                ", offset: " + CORE::ToString( (CORE::Int64)message.offset() ).STL_String() +
                                                ", key: " + ( message.key() ? (*message.key()) : NULLstr ) +
                                                ", payload size: " + CORE::ToString( message.len() ).STL_String() );

        if ( notify )
        {
            m_publishSuccessActionIds.push_back( publishActionId );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::CommitConsumerOffsets( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_kafkaConsumer )
        return false;

    // Now commit the latest offsets
    std::vector<RdKafka::TopicPartition*> partitions;
    RdKafka::ErrorCode err = m_kafkaConsumer->assignment( partitions );
    if ( RdKafka::ERR_NO_ERROR == err )
    {
        // Match the current Topic objects with our simplistic bookkeeping and sync them
        std::vector<RdKafka::TopicPartition*>::iterator p = partitions.begin();
        while ( p != partitions.end() )
        {
            CORE::Int32 partitionId = (*p)->partition();
            TInt32ToInt64Map::iterator o = m_consumerOffsets.find( partitionId );
            if ( o != m_consumerOffsets.end() )
            {
                (*p)->set_offset( (*o).second );
            }
            ++p;
        }

        // Now we actually tell the client library locally about the new offsets
        err = m_kafkaConsumer->offsets_store( partitions );
        if ( RdKafka::ERR_NO_ERROR == err )
        {
            // Now we request to send the local offset bookkeeping to Kafka
            err = m_kafkaConsumer->commitAsync( partitions );
            if ( RdKafka::ERR_NO_ERROR == err )
            {
                // Reset our flag so that we do not commit needlessly
                m_msgsReceivedSinceLastOffsetCommit = false;

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic: Successfully triggered async commit of current offsets" );
                return true;
            }
            else
            {
                std::string errStr = RdKafka::err2str( err );
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Cannot commit consumer offsets: Failed to trigger async commit of current offets. ErrorCode : " + errStr );
                UpdateIsHealthyStatus( false );
                return false;
            }
        }
        else
        {
            std::string errStr = RdKafka::err2str( err );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Cannot commit consumer offsets: Failed to store current offets on local topic objects. ErrorCode : " + errStr );
            return false;
        }
    }
    else
    {
        std::string errStr = RdKafka::err2str( err );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Cannot commit consumer offsets: Failed to obtain current partition assignment. ErrorCode : " + errStr );
        UpdateIsHealthyStatus( false );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::RequestSubscriptionMsgArrivalDelay( CORE::UInt32 minDelayInMs )
{GUCEF_TRACE;

    if ( 0 == m_requestedConsumeDelayInMs )
    {
        m_tickCountAtConsumeDelayRequest = GetPulseGenerator()->GetTickCount();
        m_requestedConsumeDelayInMs = minDelayInMs;
    }
    else
    {
        // We are still working on the last delay
        // We leave the last request timestamp but merge the requests by adding the remnant time to the newly requested time
        CORE::UInt32 remainingMs = (CORE::UInt32) GetPulseGenerator()->GetTimeSinceTickCountInMilliSecs( m_tickCountAtConsumeDelayRequest );
        m_requestedConsumeDelayInMs = minDelayInMs + remainingMs;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

CORE::CPulseGenerator*
CKafkaPubSubClientTopic::GetPulseGenerator( void )
{GUCEF_TRACE;

    return m_client->GetConfig().pulseGenerator;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::OnPulseCycle( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( !m_publishFailureActionIds.empty() )
    {
        if ( !NotifyObservers( MsgsPublishFailureEvent, &m_publishFailureActionEventData ) ) return;
        m_publishFailureActionIds.clear();
    }
    if ( !m_publishSuccessActionIds.empty() )
    {
        if ( !NotifyObservers( MsgsPublishedEvent, &m_publishSuccessActionEventData ) ) return;
        m_publishSuccessActionIds.clear();
    }

    // You are required to periodically call poll() on a producer to trigger queued callbacks if any
    if ( GUCEF_NULL != m_kafkaProducer )
    {
        int i=0;
        for ( ; i<50; ++i )
        {
            CORE::Int32 opsServed = (CORE::Int32) m_kafkaProducer->poll( 0 );
            if ( 0 == opsServed )
            {
                break;
            }

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "KafkaPubSubClientTopic:OnPulseCycle: poll() on the kafkaProducer served " +
                    CORE::Int32ToString( opsServed ) + " events");
        }
        if ( i == 50 )
        {
            // We have more work to do. Make sure we dont go to sleep
            GetPulseGenerator()->RequestImmediatePulse();
        }
    }

    if ( GUCEF_NULL != m_kafkaConsumer )
    {
        if ( m_requestedConsumeDelayInMs == 0 || 
             m_requestedConsumeDelayInMs <= GetPulseGenerator()->GetTimeSinceTickCountInMilliSecs( m_tickCountAtConsumeDelayRequest ) )
        {        
            m_requestedConsumeDelayInMs = 0;
            
            int i=0;
            for ( ; i<50; ++i )
            {
                RdKafka::Message* msg = m_kafkaConsumer->consume( 0 );
                int errState = msg->err();
                if ( RdKafka::ERR__TIMED_OUT == errState || RdKafka::ERR__PARTITION_EOF == errState )
                {
                    delete msg;
                    break;
                }

                consume_cb( *msg, GUCEF_NULL );
                delete msg;
            }
            if ( i == 50 )
            {
                // We have more work to do. Make sure we dont go to sleep
                GetPulseGenerator()->RequestImmediatePulse();
            }

            if ( m_msgsReceivedSinceLastOffsetCommit )
            {
                // Periodically commit our offsets
                // This can slow things down so we dont want to do this too often
                if ( 5000 < GetPulseGenerator()->GetTimeSinceTickCountInMilliSecs( m_tickCountAtLastOffsetCommit ) )
                {
                    if ( CommitConsumerOffsets() )
                    {
                        m_tickCountAtLastOffsetCommit = GetPulseGenerator()->GetTickCount();
                    }
                }
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::DeriveBookmarkFromMsg( const PUBSUB::CIPubSubMsg& msg, PUBSUB::CPubSubBookmark& bookmark ) const
{
    // Note that this does not require a lock as data we access is local to the message
    // The code is here because only the backend logic knows which bookmark type can be derived (if any)
    // from a message for the given backend

    // For Kafka we know that we encode the partition offset on the Index property
    if ( msg.GetMsgIndex().ByteSize() > 0 && msg.GetMsgIndex().GetTypeId() == GUCEF_DATATYPE_BINARY_BSOB )
    {
        bookmark.SetBookmarkType( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_MSG_INDEX );
        bookmark.SetBookmarkData( msg.GetMsgIndex() );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAFKA */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
