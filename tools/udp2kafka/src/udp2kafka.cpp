/*
 *  udp2kafka: service which pushes UDP packets into kafka topics
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefCOM_CDummyHTTPServerResource.h"
#define GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H ? */

#include "udp2kafka.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <winsock2.h>
#endif

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const std::string Udp2KafkaChannel::KafkaMsgHeader_UdpOrigin = "UdpOrigin";
const std::string Udp2KafkaChannel::KafkaMsgHeader_ProducerHostname = "ProducerHostname";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

Udp2KafkaChannel::Udp2KafkaChannel()
    : CORE::CTaskConsumer()
    , RdKafka::EventCb()
    , RdKafka::DeliveryReportCb()
    , RdKafka::ConsumeCb()
    , RdKafka::RebalanceCb()
    , m_kafkaConf( GUCEF_NULL )
    , m_kafkaTopicConf( GUCEF_NULL )
    , m_kafkaProducer( GUCEF_NULL )
    , m_kafkaProducerTopic( GUCEF_NULL )
    , m_kafkaConsumer( GUCEF_NULL )
    , m_channelSettings()
    , m_udpSocket( GUCEF_NULL )
    , m_kafkaMsgQueueOverflowQueue()
    , m_metricsTimer( GUCEF_NULL )
    , m_metrics()
    , m_kafkaErrorReplies( 0 )
    , m_kafkaMsgsTransmitted( 0 )
    , m_kafkaMessagesReceived( 0 )
    , m_kafkaBrokers()
    , m_producerHostname( CORE::GetHostname() )
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

Udp2KafkaChannel::Udp2KafkaChannel( const Udp2KafkaChannel& src )
    : CORE::CTaskConsumer()
    , RdKafka::EventCb()
    , RdKafka::DeliveryReportCb()
    , RdKafka::ConsumeCb()
    , RdKafka::RebalanceCb()
    , m_kafkaConf( GUCEF_NULL )
    , m_kafkaTopicConf( GUCEF_NULL )
    , m_kafkaProducer( GUCEF_NULL )
    , m_kafkaProducerTopic( GUCEF_NULL )
    , m_kafkaConsumer( GUCEF_NULL )
    , m_channelSettings( src.m_channelSettings )
    , m_udpSocket( src.m_udpSocket )
    , m_kafkaMsgQueueOverflowQueue()
    , m_metricsTimer( src.m_metricsTimer )
    , m_metrics()
    , m_kafkaErrorReplies( 0 )
    , m_kafkaMsgsTransmitted( 0 )
    , m_kafkaMessagesReceived( 0 )
    , m_kafkaBrokers( src.m_kafkaBrokers )
    , m_producerHostname( CORE::GetHostname() )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2KafkaChannel::~Udp2KafkaChannel()
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
    
    delete m_udpSocket;
    m_udpSocket = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Register UDP socket event handlers
    TEventCallback callback( this, &Udp2KafkaChannel::OnUDPSocketError );
    SubscribeTo( m_udpSocket                              ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback                                 );
    TEventCallback callback2( this, &Udp2KafkaChannel::OnUDPSocketClosed );
    SubscribeTo( m_udpSocket                               ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback2                                 );
    TEventCallback callback3( this, &Udp2KafkaChannel::OnUDPSocketClosing );
    SubscribeTo( m_udpSocket                                ,
                 COMCORE::CUDPSocket::UDPSocketClosingEvent ,
                 callback3                                  );
    TEventCallback callback4( this, &Udp2KafkaChannel::OnUDPSocketOpened );
    SubscribeTo( m_udpSocket                               ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback4                                 );
    TEventCallback callback5( this, &Udp2KafkaChannel::OnUDPPacketRecieved );
    SubscribeTo( m_udpSocket                                 ,
                 COMCORE::CUDPSocket::UDPPacketRecievedEvent ,
                 callback5                                   );
    
    TEventCallback callback6( this, &Udp2KafkaChannel::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback6                      );
}

/*-------------------------------------------------------------------------*/

Udp2KafkaChannel::ChannelSettings::ChannelSettings( void )
    : kafkaBrokers( "127.0.0.1:9092" )
    , channelTopicName()
    , udpInterface()
    , udpMulticastToJoin()
    , collectMetrics( false )
    , metricsPrefix()
    , wantsTestPackage( false )
    , mode( Udp2KafkaChannel::EChannelMode::KAFKA_PRODUCER )
    , consumerModeStartOffset( "stored" )
    , consumerModeGroupId( "0" )
    , consumerModeUdpDestinations()
    , kafkaGlobalConfigSettings()
    , kafkaTopicConfigSettings()
    , kafkaMsgHeaderUsedForFiltering( KafkaMsgHeader_ProducerHostname )
    , kafkaMsgValueUsedForFiltering( CORE::GetHostname() )
    , addUdpOriginKafkaMsgHeader( true )
    , addProducerHostnameKafkaMsgHeader( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2KafkaChannel::ChannelSettings::ChannelSettings( const ChannelSettings& src )
    : kafkaBrokers( src.kafkaBrokers )
    , channelTopicName( src.channelTopicName )
    , udpInterface( src.udpInterface )
    , udpMulticastToJoin( src.udpMulticastToJoin )
    , collectMetrics( src.collectMetrics )
    , metricsPrefix( src.metricsPrefix )
    , wantsTestPackage( src.wantsTestPackage )
    , mode( src.mode )
    , consumerModeStartOffset( src.consumerModeStartOffset )
    , consumerModeGroupId( src.consumerModeGroupId )
    , consumerModeUdpDestinations( src.consumerModeUdpDestinations )
    , kafkaGlobalConfigSettings( src.kafkaGlobalConfigSettings )
    , kafkaTopicConfigSettings( src.kafkaTopicConfigSettings )
    , kafkaMsgHeaderUsedForFiltering( src.kafkaMsgHeaderUsedForFiltering )
    , kafkaMsgValueUsedForFiltering( src.kafkaMsgValueUsedForFiltering )
    , addUdpOriginKafkaMsgHeader( src.addUdpOriginKafkaMsgHeader )
    , addProducerHostnameKafkaMsgHeader( src.addProducerHostnameKafkaMsgHeader )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2KafkaChannel::ChannelSettings&
Udp2KafkaChannel::ChannelSettings::operator=( const ChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        kafkaBrokers = src.kafkaBrokers;
        channelTopicName = src.channelTopicName;
        udpInterface = src.udpInterface;
        udpMulticastToJoin = src.udpMulticastToJoin;
        collectMetrics = src.collectMetrics;
        metricsPrefix = src.metricsPrefix;
        wantsTestPackage = src.wantsTestPackage;
        mode = src.mode;
        consumerModeStartOffset = src.consumerModeStartOffset;
        consumerModeGroupId = src.consumerModeGroupId;
        consumerModeUdpDestinations = src.consumerModeUdpDestinations;
        kafkaGlobalConfigSettings = src.kafkaGlobalConfigSettings;
        kafkaTopicConfigSettings = src.kafkaTopicConfigSettings;
        kafkaMsgHeaderUsedForFiltering = src.kafkaMsgHeaderUsedForFiltering;
        kafkaMsgValueUsedForFiltering = src.kafkaMsgValueUsedForFiltering;
        addUdpOriginKafkaMsgHeader = src.addUdpOriginKafkaMsgHeader;
        addProducerHostnameKafkaMsgHeader = src.addProducerHostnameKafkaMsgHeader;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
Udp2KafkaChannel::LoadConfig( const ChannelSettings& channelSettings )
{GUCEF_TRACE;

    m_channelSettings = channelSettings;
    return true;
}

/*-------------------------------------------------------------------------*/

const Udp2KafkaChannel::ChannelSettings& 
Udp2KafkaChannel::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
Udp2KafkaChannel::GetKafkaErrorRepliesCounter( bool resetCounter )
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
Udp2KafkaChannel::GetKafkaMsgsTransmittedCounter( bool resetCounter )
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
Udp2KafkaChannel::GetKafkaMsgsReceivedCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 kafkaMsgsReceived = m_kafkaMessagesReceived;
        m_kafkaMsgsTransmitted = 0;
        return kafkaMsgsReceived;
    }
    else
        return m_kafkaMessagesReceived;
}

/*-------------------------------------------------------------------------*/

CORE::CString
Udp2KafkaChannel::GetType( void ) const
{GUCEF_TRACE;

    return "Udp2KafkaChannel";
}

/*-------------------------------------------------------------------------*/

Udp2KafkaChannel::ChannelMetrics::ChannelMetrics( void )
    : udpBytesReceived( 0 )
    , udpMessagesReceived( 0 )
    , kafkaMessagesTransmitted( 0 )
    , kafkaTransmitQueueSize( 0 )
    , kafkaTransmitOverflowQueueSize( 0 )
    , udpBytesTransmitted( 0 )
    , udpMessagesTransmitted( 0 )
    , kafkaMessagesReceived( 0 )
    , kafkaErrorReplies( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    m_metrics.kafkaErrorReplies = GetKafkaErrorRepliesCounter( true );
    if ( EChannelMode::KAFKA_PRODUCER == m_channelSettings.mode || EChannelMode::KAFKA_PRODUCER_AND_CONSUMER == m_channelSettings.mode )
    {
        m_metrics.udpBytesReceived = m_udpSocket->GetBytesReceived( true );
        m_metrics.udpMessagesReceived = m_udpSocket->GetNrOfDataReceivedEvents( true );
        m_metrics.kafkaTransmitQueueSize = (CORE::UInt32) m_kafkaProducer->outq_len();
        m_metrics.kafkaTransmitOverflowQueueSize = (CORE::UInt32) m_kafkaMsgQueueOverflowQueue.size();   
        m_metrics.kafkaMessagesTransmitted = GetKafkaMsgsTransmittedCounter( true );
    }
    if ( EChannelMode::KAFKA_CONSUMER == m_channelSettings.mode || EChannelMode::KAFKA_PRODUCER_AND_CONSUMER == m_channelSettings.mode )
    {
        m_metrics.udpBytesTransmitted = m_udpSocket->GetBytesTransmitted( true );
        m_metrics.udpMessagesTransmitted = m_udpSocket->GetNrOfDataSentEvents( true );
        m_metrics.kafkaMessagesReceived = GetKafkaMsgsReceivedCounter( true );
    }
}

/*-------------------------------------------------------------------------*/

const Udp2KafkaChannel::ChannelMetrics&
Udp2KafkaChannel::GetMetrics( void ) const
{GUCEF_TRACE;

    return m_metrics;
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPSocketError( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventID  ,
                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::TSocketErrorEventData* eData = static_cast< COMCORE::CUDPSocket::TSocketErrorEventData* >( eventData );    
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket experienced error " + CORE::Int32ToString( eData->GetData() ) );    
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPSocketClosed( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventID  ,
                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPSocketClosing( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventID  ,
                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket is going to close" );

    // Gracefully leave the multicast groups we joined
    ChannelSettings::HostAddressVector::iterator m = m_channelSettings.udpMulticastToJoin.begin();
    while ( m != m_channelSettings.udpMulticastToJoin.end() )
    {
        const COMCORE::CHostAddress& multicastAddr = (*m);
        if ( m_udpSocket->Leave( multicastAddr ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnUDPSocketClosing: Successfully to left multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnUDPSocketClosing: Failed to leave multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
        }
        ++m;
    }
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPSocketOpened( CORE::CNotifier* notifier   ,
                                     const CORE::CEvent& eventID ,
                                     CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket has been opened" );

    ChannelSettings::HostAddressVector::iterator m = m_channelSettings.udpMulticastToJoin.begin();
    while ( m != m_channelSettings.udpMulticastToJoin.end() )
    {
        const COMCORE::CHostAddress& multicastAddr = (*m);
        if ( m_udpSocket->Join( multicastAddr ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnUDPSocketOpened: Successfully to joined multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnUDPSocketOpened: Failed to join multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
        }
        ++m;
    }
}

/*-------------------------------------------------------------------------*/

bool
Udp2KafkaChannel::UdpTransmit( RdKafka::Message& message )
{GUCEF_TRACE;

    if ( message.len() > GUCEFCORE_UINT16MAX )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:consume_cb: Received " + CORE::UInt64ToString( CORE::UInt64( message.len() ) ) + " byte message on topic \"" +          
                m_channelSettings.channelTopicName + " at offset " + CORE::Int64ToString( message.offset() ) + ". This exceeds the max size of a UInt16 and cannot be sent as a UDP packet" );
        return false;
    }

    bool totalSuccess = true;
    HostAddressVector::iterator i = m_channelSettings.consumerModeUdpDestinations.begin();
    while ( i != m_channelSettings.consumerModeUdpDestinations.end() )
    {
        totalSuccess = ( 0 < m_udpSocket->SendPacketTo( (*i), message.payload(), (CORE::UInt16) message.len() ) ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

void 
Udp2KafkaChannel::rebalance_cb( RdKafka::KafkaConsumer* consumer                  ,
                                RdKafka::ErrorCode err                            ,
                                std::vector<RdKafka::TopicPartition*>& partitions )
{GUCEF_TRACE;

    CORE::CString partitionInfo = "Udp2KafkaChannel:rebalance_cb: ";
    for ( unsigned int i=0; i<partitions.size(); ++i )
    {
        partitionInfo += "Topic \"" + partitions[ i ]->topic() + "\" is at partition \"" + CORE::Int32ToString( partitions[ i ]->partition() ).STL_String() + "\". ";
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, partitionInfo );

    if ( err == RdKafka::ERR__ASSIGN_PARTITIONS ) 
    {
        //for ( unsigned int i=0; i<partitions.size(); ++i )
        //{
        //    partitions[ i ]->set_offset(  );
        //}
        m_kafkaConsumer->assign( partitions );
    } 
    else 
    {
        consumer->unassign();
    }
}

/*-------------------------------------------------------------------------*/

void 
Udp2KafkaChannel::consume_cb( RdKafka::Message& message, void* opaque )
{GUCEF_TRACE;

    switch ( message.err() ) 
    {
        case RdKafka::ERR__TIMED_OUT:
        {
            break;
        }
        case RdKafka::ERR_NO_ERROR:
        {
            #ifdef GUCEF_DEBUG_MODE
            if ( nullptr != message.key() )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Udp2KafkaChannel:consume_cb: Received " + CORE::UInt64ToString( CORE::UInt64( message.len() ) ) + " byte message on topic \"" +          
                        m_channelSettings.channelTopicName + "\" with key \"" + *message.key() + "\" and offset " + CORE::Int64ToString( message.offset() ) );
            }
            else
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Udp2KafkaChannel:consume_cb: Received " + CORE::UInt64ToString( CORE::UInt64( message.len() ) ) + " byte message on topic \"" +          
                        m_channelSettings.channelTopicName + " without a key and with offset " + CORE::Int64ToString( message.offset() ) );
            }
            #endif
            
            ++m_kafkaMessagesReceived;

            bool isFiltered = false;
            const RdKafka::Headers* headers = message.headers();
            if ( GUCEF_NULL != headers && !m_channelSettings.kafkaMsgHeaderUsedForFiltering.empty() ) 
            {
                std::vector<RdKafka::Headers::Header> hdrs = headers->get( m_channelSettings.kafkaMsgHeaderUsedForFiltering );
                for ( size_t i=0; i<hdrs.size(); ++i ) 
                {
                    const char* hdrValue = hdrs[ i ].value_string();
                    if ( GUCEF_NULL != hdrValue && m_channelSettings.kafkaMsgValueUsedForFiltering == hdrValue )
                    {
                        isFiltered = true;
                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Udp2KafkaChannel:consume_cb: Filtered message on topic \"" +          
                                m_channelSettings.channelTopicName + " with offset " + CORE::Int64ToString( message.offset() ) );    
                        break;
                    }
                }
            }

            if ( !isFiltered )
                UdpTransmit( message );
            break;
        }
        case RdKafka::ERR__PARTITION_EOF:
        {
            // Last message that was available has been read
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Kafka topic \"" + m_channelSettings.channelTopicName + "\" doesnt have any new messages waiting to be consumed" );
            break;
        }
        case RdKafka::ERR__UNKNOWN_PARTITION:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka consume error: UNKNOWN_PARTITION: " + message.errstr() );
            break;
        }
        case RdKafka::ERR__UNKNOWN_TOPIC:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka consume error: UNKNOWN_TOPIC: " + message.errstr() );
            break;
        }
        default:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka consume error: " + message.errstr() );
            break;
        }
    } 
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::event_cb( RdKafka::Event& event )
{GUCEF_TRACE;

    switch ( event.type() )
    {
        case RdKafka::Event::EVENT_ERROR:
        {
            #ifdef GUCEF_DEBUG_MODE
            if ( RdKafka::ERR__PARTITION_EOF == event.err() )
            {
                // Last message that was available has been read
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Kafka topic \"" + m_channelSettings.channelTopicName + "\" doesnt have any new messages waiting to be consumed" );
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
Udp2KafkaChannel::MsgStatusToString( RdKafka::Message::Status status )
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
Udp2KafkaChannel::dr_cb( RdKafka::Message& message )
{GUCEF_TRACE;

    if ( message.err() ) 
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Kafka delivery report: error: " + message.errstr() + 
                                                ", on topic: " + message.topic_name() + 
                                                ", key: " + ( message.key() ? (*message.key()) : std::string( "NULL" ) ) + 
                                                ", payload size: " + CORE::UInt32ToString( message.len() ).STL_String() +
                                                ", msg has " + MsgStatusToString( message.status() ) );
    }
    else 
    {
        ++m_kafkaMsgsTransmitted;
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Kafka delivery report: " + MsgStatusToString( message.status() ) + ": topic: " + message.topic_name() + 
                                                ", partition: " + CORE::Int32ToString( message.partition() ).STL_String() +
                                                ", offset: " + CORE::Int64ToString( message.offset() ).STL_String() +
                                                ", key: " + ( message.key() ? (*message.key()) : std::string( "NULL" ) ) + 
                                                ", payload size: " + CORE::UInt32ToString( message.len() ).STL_String() );
    }
}

/*-------------------------------------------------------------------------*/

RdKafka::ErrorCode
Udp2KafkaChannel::KafkaProduce( const COMCORE::CIPAddress& sourceAddress ,
                                const CORE::CDynamicBuffer& udpPacket    )
{GUCEF_TRACE;

    RdKafka::Headers* headers = 0;
    if ( m_channelSettings.addProducerHostnameKafkaMsgHeader ||
         m_channelSettings.addUdpOriginKafkaMsgHeader         )
    {
        headers = RdKafka::Headers::create();

        if ( m_channelSettings.addUdpOriginKafkaMsgHeader )
            headers->add( KafkaMsgHeader_UdpOrigin, sourceAddress.AddressAndPortAsString() );

        if ( m_channelSettings.addProducerHostnameKafkaMsgHeader )
            headers->add( KafkaMsgHeader_ProducerHostname, m_producerHostname );
    }

    RdKafka::ErrorCode retCode = 
        m_kafkaProducer->dvcustom_produce( m_kafkaProducerTopic, 
                                           RdKafka::Topic::PARTITION_UA,
                                           RdKafka::Producer::RK_MSG_COPY,                       // <- Copy payload, tradeoff against blocking on kafka produce
                                           const_cast< void* >( udpPacket.GetConstBufferPtr() ), // <- MSG_COPY flag will cause buffer to be copied, const cast to avoid copying again
                                           (size_t)udpPacket.GetDataSize(), 
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
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:KafkaProduce: Successfully queued packet for transmission" );
            break;
        }
        case RdKafka::ERR__QUEUE_FULL:
        {
            // We dont treat queue full as an error metrics wise. This is an expected and handled scenario
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:KafkaProduce: transmission queue is full" );
            break;
        }
        default:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:KafkaProduce: Kafka error: " + RdKafka::err2str( retCode ) + " from kafkaProducer->produce()" );
            ++m_kafkaErrorReplies;
            break;
        }
    }

    return retCode;
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPPacketRecieved( CORE::CNotifier* notifier   ,
                                       const CORE::CEvent& eventID ,
                                       CORE::CICloneable* evenData )
{GUCEF_TRACE;

    if ( EChannelMode::KAFKA_PRODUCER != m_channelSettings.mode && EChannelMode::KAFKA_PRODUCER_AND_CONSUMER != m_channelSettings.mode )
    {
        // If we are not a producer we ignore incoming UDP data
        return;
    }

    COMCORE::CUDPSocket::UDPPacketRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketRecievedEventData* >( evenData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUDPPacketRecievedEventData& data = udpPacketData->GetData();
        const CORE::CDynamicBuffer& udpPacketBuffer = data.dataBuffer.GetData();
        const COMCORE::CIPAddress& sourceAddress = data.sourceAddress;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel: UDP Socket received a packet from " + sourceAddress.AddressAndPortAsString() );
        
        if ( GUCEF_NULL != m_kafkaProducer )
        {
            RdKafka::ErrorCode retCode = RdKafka::ERR_NO_ERROR;
            while ( !m_kafkaMsgQueueOverflowQueue.empty() && ( retCode == RdKafka::ERR_NO_ERROR ) )
            {
                const COMCORE::CUDPSocket::TUDPPacketRecievedEventData& queuedUdpPacketEntry = m_kafkaMsgQueueOverflowQueue.front();
                retCode = KafkaProduce( queuedUdpPacketEntry.sourceAddress, queuedUdpPacketEntry.dataBuffer.GetData() );
                if ( retCode == RdKafka::ERR_NO_ERROR )
                {
                    m_kafkaMsgQueueOverflowQueue.pop_front();
                }
                else
                    break;            
            }

            if ( retCode == RdKafka::ERR_NO_ERROR )
            {
                retCode = retCode = KafkaProduce( sourceAddress, udpPacketBuffer );
                switch ( retCode )
                {
                    case RdKafka::ERR__QUEUE_FULL: 
                    {
                        m_kafkaMsgQueueOverflowQueue.push_back( data );
                        break;
                    }
                    case RdKafka::ERR_NO_ERROR: 
                    {
                        break;
                    }
                }
            }
            else
            {
                m_kafkaMsgQueueOverflowQueue.push_back( data );
            }
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

CORE::Int64
Udp2KafkaChannel::ConvertKafkaConsumerStartOffset( const CORE::CString& startOffsetDescription )
{GUCEF_TRACE;

    CORE::CString testString = startOffsetDescription.Lowercase();
    if ( testString.IsNULLOrEmpty() )
    {
        return RdKafka::Topic::OFFSET_STORED; 
    }
    if ( "beginning" == testString )
    {
        return RdKafka::Topic::OFFSET_BEGINNING;
    }
    if ( "end" == testString )
    {
        return RdKafka::Topic::OFFSET_END;
    }
    if ( "stored" == testString )
    {
        return RdKafka::Topic::OFFSET_STORED;
    }

    return CORE::StringToInt64( testString );
}

/*-------------------------------------------------------------------------*/

bool
Udp2KafkaChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

	m_udpSocket = new GUCEF::COMCORE::CUDPSocket( *GetPulseGenerator(), false );
    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
    m_metricsTimer->SetEnabled( m_channelSettings.collectMetrics );

    // Set the minimum number of cycles we will go full speed if a single cycle was not enough to handle
    // all the processing. This will cause a bypass of CPU yielding if/when the situation arises.
    // In such a case the thread will run at max speed for a least the below set nr of cycles.
    GetPulseGenerator()->RequestPulsesPerImmediatePulseRequest( 100 );

    RegisterEventHandlers();
            
    CORE::CString::StringVector kafkaBrokerList = m_channelSettings.kafkaBrokers.ParseElements( ',', false );
    CORE::CString::StringVector::iterator i = kafkaBrokerList.begin();
    while ( i != kafkaBrokerList.end() )
    {
        m_kafkaBrokers.push_back( COMCORE::CHostAddress( (*i) ) );
        ++i;
    }
    if ( m_kafkaBrokers.empty() )
        return false;

    std::string errStr;
    RdKafka::Conf* kafkaConf = RdKafka::Conf::create( RdKafka::Conf::CONF_GLOBAL );
    kafkaConf->set( "metadata.broker.list", m_channelSettings.kafkaBrokers, errStr );
	kafkaConf->set( "event_cb", static_cast< RdKafka::EventCb* >( this ), errStr );
	kafkaConf->set( "dr_cb", static_cast< RdKafka::DeliveryReportCb* >( this ), errStr );
    kafkaConf->set( "rebalance_cb", static_cast< RdKafka::RebalanceCb* >( this ), errStr );
    ChannelSettings::StringMap::iterator m = m_channelSettings.kafkaGlobalConfigSettings.begin();
    while ( m != m_channelSettings.kafkaGlobalConfigSettings.end() )
    {
        if ( RdKafka::Conf::CONF_OK != kafkaConf->set( (*m).first, (*m).second, errStr ) )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnTaskStart: Failed to set Kafka global config entry \"" + 
                    (*m).first + "\"=\"" + (*m).second + "\", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
        }
        ++m;
    }
    delete m_kafkaConf;
    m_kafkaConf = kafkaConf;

    RdKafka::Conf* kafkaTopicConfig = RdKafka::Conf::create( RdKafka::Conf::CONF_TOPIC );
    m = m_channelSettings.kafkaTopicConfigSettings.begin();
    while ( m != m_channelSettings.kafkaTopicConfigSettings.end() )
    {
        if ( RdKafka::Conf::CONF_OK != kafkaTopicConfig->set( (*m).first, (*m).second, errStr ) )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnTaskStart: Failed to set Kafka topic config entry \"" + 
                    (*m).first + "\"=\"" + (*m).second + "\", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
        }
        ++m;
    
    }
    delete m_kafkaTopicConf;
    m_kafkaTopicConf = kafkaTopicConfig;

	if ( TChannelMode::KAFKA_PRODUCER == m_channelSettings.mode || TChannelMode::KAFKA_PRODUCER_AND_CONSUMER == m_channelSettings.mode )
    {
        RdKafka::Producer* producer = RdKafka::Producer::create( m_kafkaConf, errStr );
	    if ( producer == nullptr ) 
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnTaskStart: Failed to create Kafka producerfor topic \"" + 
                m_channelSettings.channelTopicName + "\", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
	    }
        m_kafkaProducer = producer;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnTaskStart: Successfully created Kafka producer for topic \"" + 
            m_channelSettings.channelTopicName + "\"" );
        
        RdKafka::Topic* topic = RdKafka::Topic::create( m_kafkaProducer, m_channelSettings.channelTopicName, kafkaTopicConfig, errStr );
	    if ( topic == nullptr ) 
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnTaskStart: Failed to obtain Kafka Producer Topic handle for topic \"" + 
                m_channelSettings.channelTopicName + "\", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
	    }
        m_kafkaProducerTopic = topic;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnTaskStart: Successfully created Kafka Producer Topic handle for topic: " + m_channelSettings.channelTopicName );
    }

    if ( TChannelMode::KAFKA_CONSUMER == m_channelSettings.mode || TChannelMode::KAFKA_PRODUCER_AND_CONSUMER == m_channelSettings.mode )
    {
        #ifdef GUCEF_DEBUG_MODE
        m_kafkaConf->set( "enable.partition.eof", "true", errStr );
        #endif
        
        if ( RdKafka::Conf::CONF_OK != m_kafkaConf->set( "group.id", m_channelSettings.consumerModeGroupId, errStr ) )
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnTaskStart: Failed to set Kafka consumer group id to \"" + 
                m_channelSettings.consumerModeGroupId + "\", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
        }
        RdKafka::KafkaConsumer* consumer = RdKafka::KafkaConsumer::create( m_kafkaConf, errStr );
	    if ( consumer == nullptr ) 
        {
		    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnTaskStart: Failed to create Kafka consumer, error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
	    }
        m_kafkaConsumer = consumer;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnTaskStart: Successfully created Kafka consumer" );

        std::vector< std::string > topics;
        topics.push_back( m_channelSettings.channelTopicName );
        RdKafka::ErrorCode response = m_kafkaConsumer->subscribe( topics );
        if ( RdKafka::ERR_NO_ERROR != response ) 
        {
		    errStr = RdKafka::err2str( response );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnTaskStart: Failed to start Kafka Consumer for topic \"" + 
                m_channelSettings.channelTopicName + "\" at offset " + m_channelSettings.consumerModeStartOffset + ", error message: " + errStr );
            ++m_kafkaErrorReplies;
            return false;
        }
    }

    m_udpSocket->SetMaxUpdatesPerCycle( 50 );
    m_udpSocket->SetAutoReOpenOnError( true );
    m_udpSocket->SetAllowBroadcast( true );
    if ( m_udpSocket->Open( m_channelSettings.udpInterface ) )
    {
		GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnTaskStart: Successfully opened UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel:OnTaskStart: Failed to open UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
    }
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
Udp2KafkaChannel::OnTaskCycle( CORE::CICloneable* taskData )
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

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Udp2KafkaChannel:OnTaskCycle: poll() on the kafkaProducer served " + 
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
    }

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/
    
void
Udp2KafkaChannel::OnTaskEnd( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnTaskEnd" );

    if ( TChannelMode::KAFKA_PRODUCER == m_channelSettings.mode || TChannelMode::KAFKA_PRODUCER_AND_CONSUMER == m_channelSettings.mode )
    {
        // First stop the influx of new messages
        m_udpSocket->Close();

        // Push what we have left into the Kafka library's producer queue
        RdKafka::ErrorCode retCode = RdKafka::ERR_NO_ERROR;
        while ( !m_kafkaMsgQueueOverflowQueue.empty() && ( retCode == RdKafka::ERR_NO_ERROR ) )
        {
            COMCORE::CUDPSocket::TUDPPacketRecievedEventData& queuedUdpPacketEntry = m_kafkaMsgQueueOverflowQueue.front();
            retCode = KafkaProduce( queuedUdpPacketEntry.sourceAddress, queuedUdpPacketEntry.dataBuffer.GetData() );
            if ( retCode == RdKafka::ERR_NO_ERROR )
            {
                m_kafkaMsgQueueOverflowQueue.pop_front();
            }
            else
                break;            
        }

        int waited = 0;
        int queuedMsgs = m_kafkaProducer->outq_len();
        while ( queuedMsgs > 0 && waited <= 30000 ) 
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnTaskEnd: Waiting on Kafka Producer for topic: \"" + 
                m_channelSettings.channelTopicName + "\" to finish " + CORE::Int32ToString( queuedMsgs ) + " queued messages" );
            
            if ( RdKafka::ERR_NO_ERROR == m_kafkaProducer->flush( 1000 ) )
                break;

            waited += 1000;
            queuedMsgs = m_kafkaProducer->outq_len();
        }
        if ( waited > 30000 )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Udp2KafkaChannel:OnTaskEnd: Timed out waiting on Kafka Producer for topic: \"" + 
                m_channelSettings.channelTopicName + "\" to finish " + CORE::Int32ToString( queuedMsgs ) + " queued messages. Shutdown will continue regardless. Data loss will occur!" );
        }
        
        delete m_kafkaProducer;
        m_kafkaProducer = GUCEF_NULL;
        delete m_kafkaProducerTopic;
        m_kafkaProducerTopic = GUCEF_NULL;
    }

    if ( TChannelMode::KAFKA_CONSUMER == m_channelSettings.mode || TChannelMode::KAFKA_PRODUCER_AND_CONSUMER == m_channelSettings.mode )
    {
        // Stop the influx of new messages from Kafka
        m_kafkaConsumer->close();

        delete m_kafkaConsumer;
        m_kafkaConsumer = GUCEF_NULL;
    }

    delete m_kafkaTopicConf;
    m_kafkaTopicConf =  GUCEF_NULL;
    delete m_kafkaConf;
    m_kafkaConf =  GUCEF_NULL;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
    delete m_udpSocket;
    m_udpSocket = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

RestApiUdp2KafkaInfoResource::RestApiUdp2KafkaInfoResource( Udp2Kafka* app )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiUdp2KafkaInfoResource::~RestApiUdp2KafkaInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiUdp2KafkaInfoResource::Serialize( CORE::CDataNode& output             ,
                                         const CORE::CString& representation )
{GUCEF_TRACE;

    output.SetName( "info" );
    output.SetAttribute( "application", "udp2kafka" );
    output.SetAttribute( "buildDateTime", __TIMESTAMP__ );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiUdp2KafkaConfigResource::RestApiUdp2KafkaConfigResource( Udp2Kafka* app, bool appConfig )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiUdp2KafkaConfigResource::~RestApiUdp2KafkaConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiUdp2KafkaConfigResource::Serialize( CORE::CDataNode& output             ,
                                           const CORE::CString& representation )
{GUCEF_TRACE;

    if ( m_appConfig )
    {
        const CORE::CValueList& loadedConfig = m_app->GetAppConfig();
        return loadedConfig.SaveConfig( output );
    }

    const CORE::CDataNode& globalConfig = m_app->GetGlobalConfig();
    output.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

Udp2Kafka::Udp2Kafka( void )
    : CORE::CObserver()
    , m_udpStartPort( 20000 )
    , m_channelCount( 1 )
    , m_kafkaTopicStartChannelID( 0 )
    , m_kafkaTopicName( "udp-ingress-ch{channelID}" )
    , m_channels()
    , m_channelSettings()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_transmitMetrics( false )
    , m_testUdpSocket( false )
    , m_testPacketTransmitTimer()
{GUCEF_TRACE;

    TEventCallback callback1( this, &Udp2Kafka::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback1                      );

    TEventCallback callback2( this, &Udp2Kafka::OnTransmitTestPacketTimerCycle );
    SubscribeTo( &m_testPacketTransmitTimer     ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback2                      );    
}

/*-------------------------------------------------------------------------*/

Udp2Kafka::~Udp2Kafka()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
Udp2Kafka::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CORE::Int32 channelId = m_kafkaTopicStartChannelID;
    Udp2KafkaChannelVector::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        const Udp2KafkaChannel::ChannelMetrics& metrics = (*i).GetMetrics();
        const Udp2KafkaChannel::ChannelSettings& settings = (*i).GetChannelSettings();

        GUCEF_METRIC_COUNT( settings.metricsPrefix + "kafkaErrorReplies", metrics.kafkaErrorReplies, 1.0f );

        if ( Udp2KafkaChannel::EChannelMode::KAFKA_PRODUCER == settings.mode || Udp2KafkaChannel::EChannelMode::KAFKA_PRODUCER_AND_CONSUMER == settings.mode )
        {
            GUCEF_METRIC_COUNT( settings.metricsPrefix + "kafkaMessagesTransmitted", metrics.kafkaMessagesTransmitted, 1.0f );
            GUCEF_METRIC_GAUGE( settings.metricsPrefix + "kafkaTransmitQueueSize", metrics.kafkaTransmitQueueSize, 1.0f );
            GUCEF_METRIC_GAUGE( settings.metricsPrefix + "kafkaTransmitOverflowQueueSize", metrics.kafkaTransmitOverflowQueueSize, 1.0f );
            GUCEF_METRIC_COUNT( settings.metricsPrefix + "udpBytesReceived", metrics.udpBytesReceived, 1.0f );
            GUCEF_METRIC_COUNT( settings.metricsPrefix + "udpMessagesReceived", metrics.udpMessagesReceived, 1.0f );
        }
        if ( Udp2KafkaChannel::EChannelMode::KAFKA_CONSUMER == settings.mode || Udp2KafkaChannel::EChannelMode::KAFKA_PRODUCER_AND_CONSUMER == settings.mode )
        {
            GUCEF_METRIC_COUNT( settings.metricsPrefix + "kafkaMessagesReceived", metrics.kafkaMessagesTransmitted, 1.0f );
            GUCEF_METRIC_COUNT( settings.metricsPrefix + "udpBytesTransmitted", metrics.udpBytesReceived, 1.0f );
            GUCEF_METRIC_COUNT( settings.metricsPrefix + "udpMessagesTransmitted", metrics.udpMessagesReceived, 1.0f );
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
Udp2Kafka::OnTransmitTestPacketTimerCycle( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventId  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( !m_testUdpSocket.IsActive() )
        if ( !m_testUdpSocket.Open() )
            return;
    
    Udp2KafkaChannelVector::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        const Udp2KafkaChannel::ChannelSettings& settings = (*i).GetChannelSettings();
        if ( settings.wantsTestPackage )
        {
            m_testUdpSocket.SendPacketTo( settings.udpInterface, "TEST", 4 );
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
Udp2Kafka::Start( void )
{GUCEF_TRACE;

    bool errorOccured = false;
    m_channels.resize( m_channelCount );

    CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();

    CORE::UInt16 udpPort = m_udpStartPort;
    CORE::Int32 channelId = m_kafkaTopicStartChannelID;
    Udp2KafkaChannelVector::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        Udp2KafkaChannel& channel = (*i);
        ChannelSettingsMap::iterator n = m_channelSettings.find( channelId );
        if ( n != m_channelSettings.end() )
        {
            const Udp2KafkaChannel::ChannelSettings& channelSettings = (*n).second;
            if ( !channel.LoadConfig( channelSettings ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2Kafka:Start: Failed to set channel settings on channel " + CORE::Int32ToString( channelId ) );
                errorOccured = true;
                break;
            }

            if ( !taskManager.StartTask( channel ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2Kafka:Start: Failed to start task (dedicated thread) for channel " + CORE::Int32ToString( channelId ) );
                errorOccured = true;
                break;
            }
        }

        ++channelId;
        ++i;
    }

    if ( !errorOccured && m_transmitMetrics )
    {
        m_metricsTimer.SetInterval( 1000 );
        m_metricsTimer.SetEnabled( true );
    }

    if ( !errorOccured )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2Kafka: Opening REST API" );
        return m_httpServer.Listen();
    }
    return errorOccured;
}

/*-------------------------------------------------------------------------*/

bool 
Udp2Kafka::LoadConfig( const CORE::CValueList& appConfig   ,
                       const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    m_transmitMetrics = CORE::StringToBool( appConfig.GetValueAlways( "TransmitMetrics", "true" ) );
    
    m_udpStartPort = CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "UdpStartPort", "20000" ) ) );
    m_channelCount = CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "ChannelCount", "1" ) ) );
    m_kafkaTopicStartChannelID = CORE::StringToInt32( CORE::ResolveVars(  appConfig.GetValueAlways( "KafkaTopicStartChannelID", "1" ) ) );
    m_kafkaTopicName = CORE::ResolveVars( appConfig.GetValueAlways( "KafkaTopicName", "udp-ingress-ch{channelID}" ) );
    CORE::CString kafkaBrokers = CORE::ResolveVars( appConfig.GetValueAlways( "KafkaBrokers", "127.0.0.1:9092" ) );

    Udp2KafkaChannel::ChannelSettings::HostAddressVector consumerModeUdpDestinations;
    CORE::CValueList::TStringVector settingValues = appConfig.GetValueVectorAlways( "ConsumerMode.UdpDestinations" );
    CORE::CValueList::TStringVector::iterator n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        CORE::CString::StringVector splitValues = settingValue.ParseElements( ',', false );
        CORE::CString::StringVector::iterator m = splitValues.begin();
        while ( m != splitValues.end() )
        {
            COMCORE::CHostAddress udpDest( (*m) );
            consumerModeUdpDestinations.push_back( udpDest );
            ++m;
        }
        ++n;
    }

    CORE::UInt16 udpPort = m_udpStartPort;
    CORE::Int32 maxChannelId = m_kafkaTopicStartChannelID + m_channelCount;
    for ( CORE::Int32 channelId = m_kafkaTopicStartChannelID; channelId < maxChannelId; ++channelId )
    {
        Udp2KafkaChannel::ChannelSettings& channelSettings = m_channelSettings[ channelId ];

        channelSettings.kafkaBrokers = kafkaBrokers;
        channelSettings.collectMetrics = m_transmitMetrics;

        CORE::CString settingPrefix = "ChannelSetting." + CORE::Int32ToString( channelId );
        
        CORE::CString settingName = settingPrefix + ".KafkaTopicName";
        CORE::CString settingValue = appConfig.GetValueAlways( settingName );
        if ( !settingValue.IsNULLOrEmpty() )
        {
            channelSettings.channelTopicName = CORE::ResolveVars( settingValue.ReplaceSubstr( "{channelID}", CORE::Int32ToString( channelId ) ) );
        }
        else
        {
            // Use the auto naming and numbering scheme based on a single template name instead
            channelSettings.channelTopicName = CORE::ResolveVars( m_kafkaTopicName.ReplaceSubstr( "{channelID}", CORE::Int32ToString( channelId ) ) );
        }

        settingName = settingPrefix + ".UdpInterface";
        settingValue = CORE::ResolveVars( appConfig.GetValueAlways( settingName ) );
        if ( !settingValue.IsNULLOrEmpty() )
        {
            if ( settingValue.HasChar( ':' ) )
            {
                channelSettings.udpInterface.SetHostnameAndPort( settingValue );
            }
            else
            {
                channelSettings.udpInterface.SetHostname( settingValue );
            }
        }
        else
        {
            // Use the auto numbering scheme instead
            channelSettings.udpInterface.SetPortInHostByteOrder( udpPort );
        }

        // Allow overriding the specific port for a specific channel
        settingName = settingPrefix + ".UdpPort";
        settingValue = CORE::ResolveVars( appConfig.GetValueAlways( settingName ) );
        if ( !settingValue.IsNULLOrEmpty() )
        {
            channelSettings.udpInterface.SetPortInHostByteOrder( CORE::StringToUInt16( settingValue ) );    
        }

        settingName = settingPrefix + ".WantsTestPackage";
        channelSettings.wantsTestPackage = CORE::StringToBool( appConfig.GetValueAlways( settingName, "false" ) );

        settingName = settingPrefix + ".MetricsPrefix";
        channelSettings.metricsPrefix = CORE::ResolveVars( appConfig.GetValueAlways( settingName, "udp2kafka.ch{channelID}." ) );
        channelSettings.metricsPrefix = channelSettings.metricsPrefix.ReplaceSubstr( "{channelID}", CORE::Int32ToString( channelId ) );
        channelSettings.metricsPrefix = channelSettings.metricsPrefix.ReplaceSubstr( "{topicName}", channelSettings.channelTopicName );

        settingName = settingPrefix + ".Multicast.Join";
        CORE::CValueList::TStringVector settingValues = appConfig.GetValueVectorAlways( settingName );
        CORE::CValueList::TStringVector::iterator n = settingValues.begin();
        while ( n != settingValues.end() )
        {
            CORE::CString settingValue = CORE::ResolveVars( (*n) );
            CORE::CString::StringVector splitValues = settingValue.ParseElements( ',', false );
            CORE::CString::StringVector::iterator m = splitValues.begin();
            while ( m != splitValues.end() )
            {
                COMCORE::CHostAddress multicastAddress( (*m) );
                channelSettings.udpMulticastToJoin.push_back( multicastAddress );
                ++m;
            }
            ++n;
        }

        settingName = settingPrefix + ".Mode";
        settingValue = CORE::ResolveVars( appConfig.GetValueAlways( settingName, "producer" ) ).Lowercase();
        if ( settingValue == "consumer" )
        {
            channelSettings.mode = Udp2KafkaChannel::EChannelMode::KAFKA_CONSUMER;
        }
        else
        if ( settingValue == "both" || settingValue == "producer_and_consumer" )
        {
            channelSettings.mode = Udp2KafkaChannel::EChannelMode::KAFKA_PRODUCER_AND_CONSUMER;
        }
        else
        {
            channelSettings.mode = Udp2KafkaChannel::EChannelMode::KAFKA_PRODUCER;
        }

        settingName = settingPrefix + ".ConsumerMode.StartOffset";
        channelSettings.consumerModeStartOffset = CORE::ResolveVars( appConfig.GetValueAlways( settingName, "stored" ) ).Lowercase();

        settingName = settingPrefix + ".ConsumerMode.GroupID";
        channelSettings.consumerModeStartOffset = CORE::ResolveVars( appConfig.GetValueAlways( settingName, "0" ) );

        channelSettings.consumerModeUdpDestinations = consumerModeUdpDestinations;
        settingName = settingPrefix + ".ConsumerMode.UdpDestinations";
        settingValues = appConfig.GetValueVectorAlways( settingName );
        n = settingValues.begin();
        while ( n != settingValues.end() )
        {
            CORE::CString settingValue = CORE::ResolveVars( (*n) );
            CORE::CString::StringVector splitValues = settingValue.ParseElements( ',', false );
            CORE::CString::StringVector::iterator m = splitValues.begin();
            while ( m != splitValues.end() )
            {
                COMCORE::CHostAddress udpDest( (*m) );
                channelSettings.consumerModeUdpDestinations.push_back( udpDest );
                ++m;
            }
            ++n;
        }

        settingName = settingPrefix + ".KafkaGlobalConfig.";
        CORE::CValueList::TStringVector kafkaGlobalConfigKeys = appConfig.GetKeysWithWildcardKeyMatch( settingName + '*' );
        n = kafkaGlobalConfigKeys.begin();
        while ( n != kafkaGlobalConfigKeys.end() )
        {
            CORE::CString kafkaGlobalConfigSettingKey = (*n).CutChars( settingName.Length(), true, 0 );
            CORE::CString kafkaGlobalConfigSettingValue = appConfig.GetValueAlways( (*n) );
            if ( !kafkaGlobalConfigSettingKey.IsNULLOrEmpty() && !kafkaGlobalConfigSettingValue.IsNULLOrEmpty() )
            {
                channelSettings.kafkaGlobalConfigSettings[ kafkaGlobalConfigSettingKey ] = kafkaGlobalConfigSettingValue;
            }
            ++n;
        }
        settingName = settingPrefix + ".KafkaTopicConfig.";
        CORE::CValueList::TStringVector kafkaTopicConfigKeys = appConfig.GetKeysWithWildcardKeyMatch( settingName + '*' );
        n = kafkaTopicConfigKeys.begin();
        while ( n != kafkaTopicConfigKeys.end() )
        {
            CORE::CString kafkaTopicConfigSettingKey = (*n).CutChars( settingName.Length(), true, 0 );
            CORE::CString kafkaTopicConfigSettingValue = appConfig.GetValueAlways( (*n) );
            if ( !kafkaTopicConfigSettingKey.IsNULLOrEmpty() && !kafkaTopicConfigSettingValue.IsNULLOrEmpty() )
            {
                channelSettings.kafkaTopicConfigSettings[ kafkaTopicConfigSettingKey ] = kafkaTopicConfigSettingValue;
            }
            ++n;
        }

        ++udpPort;
    }

    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );

    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort", "10000" ) ) ) );

    m_httpRouter.SetResourceMapping( "/info", RestApiUdp2KafkaInfoResource::THTTPServerResourcePtr( new RestApiUdp2KafkaInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiUdp2KafkaConfigResource::THTTPServerResourcePtr( new RestApiUdp2KafkaConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiUdp2KafkaConfigResource::THTTPServerResourcePtr( new RestApiUdp2KafkaConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping(  CORE::ResolveVars( appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ) ), RestApiUdp2KafkaInfoResource::THTTPServerResourcePtr( new COM::CDummyHTTPServerResource() )  );

    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

    m_testPacketTransmitTimer.SetInterval( CORE::StringToUInt32( appConfig.GetValueAlways( "TestPacketTransmissionIntervalInMs", "1000" ) ) );
    m_testPacketTransmitTimer.SetEnabled( CORE::StringToBool( appConfig.GetValueAlways( "TransmitTestPackets", "false" ) ) );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList&
Udp2Kafka::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
Udp2Kafka::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/


