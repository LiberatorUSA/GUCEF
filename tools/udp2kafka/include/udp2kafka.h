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

#include <deque>

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#include "rdkafkacpp.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class Udp2KafkaChannel : public CORE::CTaskConsumer
{
    public:

    typedef CORE::CTEventHandlerFunctor< Udp2KafkaChannel > TEventCallback;

    Udp2KafkaChannel();
    Udp2KafkaChannel( const Udp2KafkaChannel& src );
    virtual ~Udp2KafkaChannel();

    virtual bool OnTaskStart( CORE::CICloneable* taskData );
    
    virtual bool OnTaskCycle( CORE::CICloneable* taskData );
    
    virtual void OnTaskEnd( CORE::CICloneable* taskData );

    virtual CORE::CString GetType( void ) const;

    bool LoadConfig( CORE::UInt16 udpPort                ,
                     const CORE::CString& kafkaTopicName ,
                     RdKafka::Conf* kafkaConf            );

    private:

    void
    OnUDPSocketError( CORE::CNotifier* notifier   ,
                      const CORE::CEvent& eventID ,
                      CORE::CICloneable* evenData );

    void
    OnUDPSocketClosed( CORE::CNotifier* notifier   ,
                       const CORE::CEvent& eventID ,
                       CORE::CICloneable* evenData );

    void
    OnUDPSocketOpened( CORE::CNotifier* notifier   ,
                       const CORE::CEvent& eventID ,
                       CORE::CICloneable* evenData );

    void
    OnUDPPacketRecieved( CORE::CNotifier* notifier   ,
                         const CORE::CEvent& eventID ,
                         CORE::CICloneable* evenData );
    
    private:

    void RegisterEventHandlers( void );
    
    RdKafka::ErrorCode KafkaProduce( const CORE::CDynamicBuffer& udpPacket );

    private:

    typedef std::deque< CORE::CDynamicBuffer > TDynamicBufferQueue;

    CORE::UInt16 m_udpPort;
    RdKafka::Conf* m_kafkaConf;
    CORE::CString m_kafkaTopicName;

    GUCEF::COMCORE::CUDPSocket m_udpSocket;
    TDynamicBufferQueue m_kafkaMsgQueueOverflowQueue;
    RdKafka::Producer* m_kafkaProducer;
    RdKafka::Topic* m_kafkaTopic;
};

/*-------------------------------------------------------------------------*/

class Udp2Kafka : private RdKafka::EventCb ,
                  private RdKafka::DeliveryReportCb 
{
    public:

    Udp2Kafka( void );
    virtual ~Udp2Kafka();

    bool Start( void );

    bool LoadConfig( const CORE::CValueList& config );

    private:

    virtual void event_cb( RdKafka::Event& event );
    virtual void dr_cb( RdKafka::Message& message );

    private:

    RdKafka::Conf* m_kafkaConf;
    CORE::UInt16 m_udpStartPort;
    CORE::UInt16 m_channelCount;
    CORE::Int32 m_kafkaTopicStartChannelID;
    CORE::CString m_kafkaTopicName;
    CORE::CString m_kafkaBrokers;
    std::vector< Udp2KafkaChannel > m_channels;
};