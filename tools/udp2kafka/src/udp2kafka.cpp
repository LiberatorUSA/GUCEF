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

#include "udp2kafka.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

Udp2KafkaChannel::Udp2KafkaChannel()
    : CORE::CTaskConsumer()
    , m_udpSocket( true )
    , m_kafkaProducer( nullptr )
    , m_kafkaTopic( nullptr )
{GUCEF_TRACE;

    RegisterEventHandlers();

    m_kafkaMsgQueueOverflowQueue.reserve( 1024 );
}

/*-------------------------------------------------------------------------*/

Udp2KafkaChannel::~Udp2KafkaChannel()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Register UDP socket event handlers
    TEventCallback callback( this, &Udp2KafkaChannel::OnUDPSocketError );
    SubscribeTo( &m_udpSocket                             ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback                                 );
    TEventCallback callback2( this, &Udp2KafkaChannel::OnUDPSocketClosed );
    SubscribeTo( &m_udpSocket                              ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback2                                 );
    TEventCallback callback3( this, &Udp2KafkaChannel::OnUDPSocketOpened );
    SubscribeTo( &m_udpSocket                              ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback3                                 );
    TEventCallback callback4( this, &Udp2KafkaChannel::OnUDPPacketRecieved );
    SubscribeTo( &m_udpSocket                                ,
                 COMCORE::CUDPSocket::UDPPacketRecievedEvent ,
                 callback4                                   );
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPSocketError( CORE::CNotifier* notifier   ,
                                    const CORE::CEvent& eventID ,
                                    CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket experienced an error" );
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPSocketClosed( CORE::CNotifier* notifier   ,
                                     const CORE::CEvent& eventID ,
                                     CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPSocketOpened( CORE::CNotifier* notifier   ,
                                     const CORE::CEvent& eventID ,
                                     CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket has been opened" );
}

/*-------------------------------------------------------------------------*/

RdKafka::ErrorCode
Udp2KafkaChannel::KafkaProduce( const CORE::CDynamicBuffer& udpPacket )
{GUCEF_TRACE;

    RdKafka::ErrorCode retCode = m_kafkaProducer->produce( m_kafkaTopic, 
                                                           RdKafka::Topic::PARTITION_UA,
                                                           RdKafka::Producer::RK_MSG_COPY,                       // <- Copy payload, tradeoff against blocking on kafka produce
                                                           const_cast< void* >( udpPacket.GetConstBufferPtr() ), // <- MSG_COPY flag will cause buffer to be copied, const cast to avoid copying again
                                                           udpPacket.GetDataSize(), 
                                                           NULL, NULL );

    if ( retCode != RdKafka::ERR_NO_ERROR )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka error: " + RdKafka::err2str( retCode ) + " from kafkaProducer->produce()" );
    }
    return retCode;
}

/*-------------------------------------------------------------------------*/

void
Udp2KafkaChannel::OnUDPPacketRecieved( CORE::CNotifier* notifier   ,
                                       const CORE::CEvent& eventID ,
                                       CORE::CICloneable* evenData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketRecievedEventData* >( evenData );
    if ( NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUDPPacketRecievedEventData& data = udpPacketData->GetData();
        const CORE::CDynamicBuffer& udpPacketBuffer = data.dataBuffer.GetData();

        #ifdef GUCEF_DEBUG_MODE
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket received a packet from " + data.sourceAddress.AddressAndPortAsString() );
        #endif
        
        if ( NULL != m_kafkaProducer )
        {
            RdKafka::ErrorCode retCode = RdKafka::ERR_NO_ERROR;

            do
            {
                const CORE::CDynamicBuffer& queuedUdpPacket = m_kafkaMsgQueueOverflowQueue.front();
                retCode = KafkaProduce( queuedUdpPacket );
                if ( retCode == RdKafka::ERR_NO_ERROR )
                {
                    m_kafkaMsgQueueOverflowQueue.pop_front();
                }
                else
                    break;            
            }
            while ( !m_kafkaMsgQueueOverflowQueue.empty() && ( retCode == RdKafka::ERR_NO_ERROR ) );
            
            
            if ( retCode == RdKafka::ERR_NO_ERROR )
            {
                retCode = retCode = KafkaProduce( udpPacketBuffer );
                switch ( retCode )
                {
                    case RdKafka::ERR__QUEUE_FULL: 
                    {
                        m_kafkaMsgQueueOverflowQueue.push_back( udpPacketBuffer );
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
                m_kafkaMsgQueueOverflowQueue.push_back( udpPacketBuffer );
            }
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: UDP Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

bool
Udp2KafkaChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

	std::string errStr;
	RdKafka::Producer* producer = RdKafka::Producer::create( conf, errStr );
	if ( producer == nullptr ) 
    {
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: Failed to create Kafka producer, error message: " + errStr );
        return false;
	}

    RdKafka::Topic* topic = RdKafka::Topic::create( m_kafkaProducer, m_kafkaTopicName, NULL, errStr );
	if ( topic == nullptr ) 
    {
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2KafkaChannel: Failed to obtain Kafka Topic handle for topic " + m_kafkaTopicName + " error message: " + errStr );
        return false;
	}
    m_kafkaTopic = topic;

}

/*-------------------------------------------------------------------------*/
    
bool
Udp2KafkaChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    m_udpSocket.Recieve();

    // You are required to periodically call poll() on a producer to trigger queud callbacks if any
    if ( NULL != m_kafkaProducer ) 
        m_kafkaProducer->poll( 0 );

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/
    
void
Udp2KafkaChannel::OnTaskEnd( CORE::CICloneable* taskData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2Kafka::Udp2Kafka( void )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

Udp2Kafka::~Udp2Kafka()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
Udp2Kafka::event_cb( RdKafka::Event& event )
{GUCEF_TRACE;

    switch ( event.type() )
    {
        case RdKafka::Event::EVENT_ERROR:
        {
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
        default:
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Kafka event: " + CORE::Int32ToString( (CORE::Int32) event.type() ) + ", with error code " + RdKafka::err2str( event.err() ) );
	        break;
        }
    }
}

/*-------------------------------------------------------------------------*/

void 
Udp2Kafka::dr_cb( RdKafka::Message& message )
{
    if ( message.err() ) 
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Kafka delivery report: error: " + message.errstr() + 
                                                        ", on topic: " + message.topic_name() + 
                                                        ", key: " + ( message.key() ? (*message.key()) : std::string( "NULL" ) ) + 
                                                        ", payload size: " + CORE::UInt32ToString( message.len() ).STL_String() );
    }
    else 
    {
        #ifdef GUCEF_DEBUG_MODE
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Kafka delivery report: success: topic: " + message.topic_name() + 
                                                        ", partition: " + CORE::Int32ToString( message.partition() ).STL_String() +
                                                        ", offset: " + CORE::Int32ToString( message.offset() ).STL_String() +
                                                        ", key: " + ( message.key() ? (*message.key()) : std::string( "NULL" ) ) + 
                                                        ", payload size: " + CORE::UInt32ToString( message.len() ).STL_String() );
        #endif
    }
}

/*-------------------------------------------------------------------------*/

bool
Udp2Kafka::Setup( void )
{GUCEF_TRACE;

    RdKafka::Conf* kafkaConf = RdKafka::Conf::create( RdKafka::Conf::CONF_GLOBAL );

	std::string errStr;

    kafkaConf->set( "metadata.broker.list", brokers, errStr );
    // Set the event callback
	kafkaConf->set( "event_cb", static_cast< RdKafka::EventCb* >( this ), errStr );
    // Set the delivery report callback
	kafkaConf->set( "dr_cb", static_cast< RdKafka::DeliveryReportCb* >( this ), errStr );
}

