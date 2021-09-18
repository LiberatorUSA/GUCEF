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
    : COMCORE::CPubSubClientTopic()
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
    , m_metrics()
    , m_lock()
{GUCEF_TRACE;
        
    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientTopic::~CKafkaPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();
    Clear();
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
CKafkaPubSubClientTopic::Publish( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    RdKafka::ErrorCode retCode = RdKafka::ERR_NO_ERROR;
    
    MT::CScopeMutex lock( m_lock );
    
    // First write any backend auto generated headers per config
    RdKafka::Headers* headers = GUCEF_NULL;
    if ( m_config.addProducerHostnameAsKafkaMsgHeader )
    {
        headers = RdKafka::Headers::create();
        retCode = headers->add( KafkaMsgHeader_ProducerHostname, m_producerHostname );
        if ( retCode != RdKafka::ERR_NO_ERROR )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: Failed to add auto generated Producer Hostname Kafka Msg header: " + RdKafka::err2str( retCode ) );
        }
    }

    // Next we add the message's meta-data key-value headers if any
    const COMCORE::CIPubSubMsg::TKeyValuePairs& metaDataKvPairs = msg.GetMetaDataKeyValuePairs();
    if ( !metaDataKvPairs.empty() )
    {
        if ( GUCEF_NULL == headers )
            headers = RdKafka::Headers::create();

        COMCORE::CIPubSubMsg::TKeyValuePairs::const_iterator k = metaDataKvPairs.begin();
        while ( k != metaDataKvPairs.end() )
        {
            retCode = headers->add( m_config.prefixToAddForMetaDataKvPairs + (*k).first.AsString(), (*k).second.AsVoidPtr(), (*k).second.ByteSize() );
            if ( retCode != RdKafka::ERR_NO_ERROR )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: Failed to add msg meta-data key \"" + m_config.prefixToAddForMetaDataKvPairs + (*k).first.AsString() + "\" as Kafka Msg header: " + RdKafka::err2str( retCode ) + ". Value=" + (*k).second.AsString() );
                return false;
            }
            ++k;
        }
    }

    // Next we add the message's key-value headers if any
    const COMCORE::CIPubSubMsg::TKeyValuePairs& kvPairs = msg.GetKeyValuePairs();
    if ( !kvPairs.empty() )
    {
        if ( GUCEF_NULL == headers )
            headers = RdKafka::Headers::create();

        COMCORE::CIPubSubMsg::TKeyValuePairs::const_iterator k = kvPairs.begin();
        while ( k != kvPairs.end() )
        {
            retCode = headers->add( m_config.prefixToAddForKvPairs + (*k).first.AsString(), (*k).second.AsVoidPtr(), (*k).second.ByteSize() );
            if ( retCode != RdKafka::ERR_NO_ERROR )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: Failed to add msg key-value entry \"" + m_config.prefixToAddForKvPairs + (*k).first.AsString() + "\" as Kafka Msg header: " + RdKafka::err2str( retCode ) + ". Value=" + (*k).second.AsString() );
                return false;
            }
            ++k;
        }
    }

    retCode = 
        m_kafkaProducer->dvcustom_produce( m_kafkaProducerTopic, 
                                           RdKafka::Topic::PARTITION_UA,
                                           RdKafka::Producer::RK_MSG_COPY,                             // <- Copy payload, tradeoff against blocking on kafka produce
                                           const_cast< void* >( msg.GetPrimaryPayload().AsVoidPtr() ), // <- MSG_COPY flag will cause buffer to be copied, const cast to avoid copying again due to bad constness on library API
                                           (size_t) msg.GetPrimaryPayload().ByteSize(), 
                                           NULL, 0,
                                           headers,
                                           NULL );
    
    if ( retCode != RdKafka::ERR_NO_ERROR )
    {
        // Headers are auto-deleted on success but not on failure!
        delete headers;
    }
    
    switch ( retCode )
    {
        case RdKafka::ERR_NO_ERROR:
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: Successfully queued packet for transmission" );
            return true;
        }
        case RdKafka::ERR__QUEUE_FULL:
        {
            // We dont treat queue full as an error metrics wise. This is an expected and handled scenario
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:Publish: transmission queue is full" );
            return false;
        }
        default:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:Publish: Kafka error: " + RdKafka::err2str( retCode ) + " from kafkaProducer->produce()" );
            ++m_kafkaErrorReplies;
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool 
CKafkaPubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CKafkaPubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
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

bool
CKafkaPubSubClientTopic::SetupBasedOnConfig( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_client )
        return false;

   const CKafkaPubSubClientConfig& clientConfig = m_client->GetConfig();

    if ( clientConfig.desiredFeatures.supportsMetrics )
    {    
        if ( GUCEF_NULL != clientConfig.pulseGenerator )
        {
            m_metricsTimer = new CORE::CTimer( *clientConfig.pulseGenerator, 1000 );
            m_metricsTimer->SetEnabled( clientConfig.desiredFeatures.supportsMetrics );
        }
        else
        {
            m_metricsTimer = new CORE::CTimer( 1000 );        
            m_metricsTimer->SetEnabled( clientConfig.desiredFeatures.supportsMetrics );
        }
    }

    RegisterEventHandlers();

    // The library wants the addresses as a csv list on its config obj
    // we convert and prep as such
    CORE::CString csvKafkaBrokerList;
    COMCORE::CPubSubClientConfig::THostAddressVector::const_iterator h = clientConfig.remoteAddresses.begin();
    while ( h != clientConfig.remoteAddresses.end() )
    {
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
        m_kafkaConsumerConf->set( "enable.partition.eof", "true", errStr );
        #endif

        // Apply default topic config as an overlay
        // start with the client level defined config first
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

        // Now the topic level config if any
        m = m_config.kafkaConsumerTopicConfigSettings.begin();
        while ( m != clientConfig.kafkaConsumerDefaultTopicConfigSettings.end() )
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

        if ( !m_config.consumerGroupName.IsNULLOrEmpty() )
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
	    if ( consumer == nullptr ) 
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to create Kafka consumer, error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
	    }
        m_kafkaConsumer = consumer;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:LoadConfig: Successfully created Kafka consumer" );

        RdKafka::Topic* topic = RdKafka::Topic::create( m_kafkaConsumer, m_config.topicName, m_kafkaConsumerConf, errStr );
	    if ( topic == GUCEF_NULL ) 
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to obtain Kafka Consumer Topic handle for topic \"" + 
                m_config.topicName + "\", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
	    }
        m_kafkaConsumerTopic = topic;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic:LoadConfig: Successfully created Kafka Consumer Topic handle for topic: " + m_config.topicName );

        



        std::vector< std::string > topics;
        topics.push_back( m_config.topicName );
        RdKafka::ErrorCode response = m_kafkaConsumer->subscribe( topics );
        if ( RdKafka::ERR_NO_ERROR != response ) 
        {
		    errStr = RdKafka::err2str( response );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:LoadConfig: Failed to start Kafka Consumer for topic \"" + 
                m_config.topicName + ", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
CKafkaPubSubClientTopic::PrepStorageForReadMsgs( CORE::UInt32 msgCount )
{GUCEF_TRACE;

    // reset size, note this does not dealloc the underlying memory
    m_pubsubMsgs.clear();
    TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs.GetData();
    msgRefs.clear();

    if ( msgCount > m_pubsubMsgs.capacity() )
    {
        // Grow the actual storage, this is allowed to become larger than msgCount to limit
        // the nr of dynamic allocations
        m_pubsubMsgs.reserve( msgCount );        
        msgRefs.reserve( msgCount );
    }
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
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
        RdKafka::ErrorCode response = m_kafkaProducer->flush( 10000 );
        if ( RdKafka::ERR_NO_ERROR != response ) 
        {
		    std::string errStr = RdKafka::err2str( response );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:Disconnect: Failed to flush Kafka Producer for topic \"" + 
                m_config.topicName + ", error message: " + errStr );
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

    MT::CScopeMutex lock( m_lock );
    return SetupBasedOnConfig();
}

/*-------------------------------------------------------------------------*/

bool 
CKafkaPubSubClientTopic::SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark )
{GUCEF_TRACE;

    // This is not supported in Kafka
    // You can seek a stream based on index (offset) or with some effort get said index based on a timestamp
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

  //  RdKafka::Metadata* topicMetaData = GUCEF_NULL;
  //  RdKafka::ErrorCode response = m_kafkaConsumer->metadata( false, m_kafkaConsumerTopic, &topicMetaData, 10000 );
  //  if ( RdKafka::ERR_NO_ERROR != response ) 
  //  {
		//std::string errStr = RdKafka::err2str( response );
  //      GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic:SubscribeStartingAtMsgDateTime: Failed to obtain metadata() for topic \"" + 
  //          m_config.topicName + ", error message: " + errStr );
  //      ++m_kafkaErrorReplies;
  //      delete topicMetaData;
  //      return false;
  //  }

    // @TODO
    //RdKafka::ErrorCode response = m_kafkaConsumer->offsetsForTimes( 
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CKafkaPubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    // Snapshot in time as such no lock needed. 
    // note that connected is a very relative term here, more like prepared to connect as needed
    return ( m_config.needPublishSupport && GUCEF_NULL != m_kafkaProducer ) || ( m_config.needSubscribeSupport && GUCEF_NULL != m_kafkaConsumer );
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    Disconnect();
    
    MT::CScopeMutex lock( m_lock );

    return false;
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
        //m_metrics.kafkaTransmitOverflowQueueSize = (CORE::UInt32) m_kafkaMsgQueueOverflowQueue.size();   
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
{
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
CKafkaPubSubClientTopic::NotifyOfReceivedMsg( RdKafka::Message& message )
{GUCEF_TRACE;

    PrepStorageForReadMsgs( 1 );
    
    COMCORE::CBasicPubSubMsg& msgWrap = m_pubsubMsgs[ 0 ];
    msgWrap.Clear();    

    if ( GUCEF_NULL != message.key() )
    {
        msgWrap.GetMsgId().LinkTo( message.key()->c_str(), (CORE::UInt32) message.key_len(), GUCEF_DATATYPE_UTF8_STRING );
    }
    else
    {
        msgWrap.GetMsgId().LinkTo( message.key_pointer(), (CORE::UInt32) message.key_len(), GUCEF_DATATYPE_BINARY );
    }
   
    
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
            break;
        }
        case RdKafka::ERR_NO_ERROR:
        {
            #ifdef GUCEF_DEBUG_MODE
            if ( GUCEF_NULL != message.key() )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "KafkaPubSubClientTopic:consume_cb: Received " + CORE::UInt64ToString( CORE::UInt64( message.len() ) ) + " byte message on topic \"" +          
                        m_config.topicName + "\" with key \"" + *message.key() + "\" and offset " + CORE::Int64ToString( message.offset() ) );
            }
            else
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "KafkaPubSubClientTopic:consume_cb: Received " + CORE::UInt64ToString( CORE::UInt64( message.len() ) ) + " byte message on topic \"" +          
                        m_config.topicName + " without a key and with offset " + CORE::Int64ToString( message.offset() ) );
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
                                    m_consumerOffsets[ message.partition() ] = message.offset();

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
                NotifyOfReceivedMsg( message );
            }
            break;
        }
        case RdKafka::ERR__PARTITION_EOF:
        {
            // Last message that was available has been read
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "KafkaPubSubClientTopic: Kafka topic \"" + m_config.topicName + "\" doesnt have any new messages waiting to be consumed" );
            break;
        }
        case RdKafka::ERR__UNKNOWN_PARTITION:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Kafka consume error: UNKNOWN_PARTITION: " + message.errstr() );
            break;
        }
        case RdKafka::ERR__UNKNOWN_TOPIC:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Kafka consume error: UNKNOWN_TOPIC: " + message.errstr() );
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
            #ifdef GUCEF_DEBUG_MODE
            if ( RdKafka::ERR__PARTITION_EOF == event.err() )
            {
                // Last message that was available has been read
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Kafka topic \"" + m_config.topicName + "\" doesnt have any new messages waiting to be consumed" );
                break;
            }
            #endif

            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka error: " + RdKafka::err2str( event.err() ) + " from KafkaEventCallback()" );
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

    if ( message.err() ) 
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Kafka delivery report: error: " + message.errstr() + 
                                                ", on topic: " + message.topic_name() + 
                                                ", key: " + ( message.key() ? (*message.key()) : std::string( "NULL" ) ) + 
                                                ", payload size: " + CORE::ToString( message.len() ).STL_String() +
                                                ", msg has " + MsgStatusToString( message.status() ) );
    }
    else 
    {
        ++m_kafkaMsgsTransmitted;
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Kafka delivery report: " + MsgStatusToString( message.status() ) + ": topic: " + message.topic_name() + 
                                                ", partition: " + CORE::ToString( message.partition() ).STL_String() +
                                                ", offset: " + CORE::ToString( message.offset() ).STL_String() +
                                                ", key: " + ( message.key() ? (*message.key()) : std::string( "NULL" ) ) + 
                                                ", payload size: " + CORE::ToString( message.len() ).STL_String() );
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
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KafkaPubSubClientTopic: Successfully triggered async commit of current offsets" );
                return true;
            }
            else
            {
                std::string errStr = RdKafka::err2str( err );
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "KafkaPubSubClientTopic: Cannot commit consumer offsets: Failed to trigger async commit of current offets. ErrorCode : " + errStr );
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
        return false;
    }
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

    //if ( m_kafkaErrorReplies > 0 )
    //{
    //    HostAddressVector::iterator h = m_kafkaBrokers.begin();
    //    while ( h != m_kafkaBrokers.end() )
    //    {
    //        (*h).Refresh();
    //        ++h;
    //    }    
    //}
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
