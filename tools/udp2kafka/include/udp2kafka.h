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

 #ifndef GUCEF_APP_UDP2KAFKA
 #define GUCEF_APP_UDP2KAFKA

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

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_COM_CHTTPSERVER_H
#include "gucefCOM_CHTTPServer.h"
#define GUCEF_COM_CHTTPSERVER_H
#endif /* GUCEF_COM_CHTTPSERVER_H ? */

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H
#include "gucefCOM_CDefaultHTTPServerRouter.h"
#define GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H
#endif /* GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H ? */

#ifndef GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefCOM_CCodecBasedHTTPServerResource.h"
#define GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H ? */

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

class Udp2KafkaChannel : public CORE::CTaskConsumer ,
                         private RdKafka::EventCb ,
                         private RdKafka::DeliveryReportCb
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

    class ChannelSettings
    {
        public:

        typedef std::vector< COMCORE::CHostAddress > HostAddressVector;

        ChannelSettings( void );
        ChannelSettings( const ChannelSettings& src );
        ChannelSettings& operator=( const ChannelSettings& src );

        CORE::CString kafkaBrokers;
        CORE::CString channelTopicName;
        COMCORE::CHostAddress udpInterface;
        HostAddressVector udpMulticastToJoin;
        bool collectMetrics;
        CORE::CString metricsPrefix;
        bool wantsTestPackage;
    };

    bool LoadConfig( const ChannelSettings& channelSettings );

    const ChannelSettings& GetChannelSettings( void ) const;

    class ChannelMetrics
    {
        public:

        ChannelMetrics( void );

        CORE::UInt32 udpBytesReceived;
        CORE::UInt32 udpMessagesReceived;
        CORE::UInt32 kafkaMessagesTransmitted;
        CORE::UInt32 kafkaTransmitOverflowQueueSize;
        CORE::UInt32 kafkaErrorReplies;
    };

    const ChannelMetrics& GetMetrics( void ) const;

    private:

    virtual void event_cb( RdKafka::Event& event );

    virtual void dr_cb( RdKafka::Message& message );

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

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    CORE::UInt32 GetKafkaErrorRepliesCounter( bool resetCounter );

    CORE::UInt32 GetKafkaMsgsTransmittedCounter( bool resetCounter );
    
    private:

    void RegisterEventHandlers( void );
    
    RdKafka::ErrorCode KafkaProduce( const CORE::CDynamicBuffer& udpPacket );

    private:

    typedef std::deque< CORE::CDynamicBuffer > TDynamicBufferQueue;
    typedef ChannelSettings::HostAddressVector HostAddressVector;

    RdKafka::Conf* m_kafkaConf;
    RdKafka::Producer* m_kafkaProducer;
    RdKafka::Topic* m_kafkaTopic;

    ChannelSettings m_channelSettings;
    GUCEF::COMCORE::CUDPSocket* m_udpSocket;
    TDynamicBufferQueue m_kafkaMsgQueueOverflowQueue;
    CORE::CTimer* m_metricsTimer;
    ChannelMetrics m_metrics;
    CORE::UInt32 m_kafkaErrorReplies;
    CORE::UInt32 m_kafkaMsgsTransmitted;
    HostAddressVector m_kafkaBrokers;
};

/*-------------------------------------------------------------------------*/

class Udp2Kafka;

class RestApiUdp2KafkaInfoResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiUdp2KafkaInfoResource( Udp2Kafka* app );

    virtual ~RestApiUdp2KafkaInfoResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    Udp2Kafka* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiUdp2KafkaConfigResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiUdp2KafkaConfigResource( Udp2Kafka* app, bool appConfig );

    virtual ~RestApiUdp2KafkaConfigResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    Udp2Kafka* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class Udp2Kafka : public CORE::CObserver 
{
    public:

    Udp2Kafka( void );
    virtual ~Udp2Kafka();

    bool Start( void );

    bool LoadConfig( const CORE::CValueList& appConfig   ,
                     const CORE::CDataNode& globalConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    private:

    typedef CORE::CTEventHandlerFunctor< Udp2Kafka > TEventCallback;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData ); 

    void
    OnTransmitTestPacketTimerCycle( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventId  ,
                                    CORE::CICloneable* eventData );

    private:

    typedef std::map< CORE::Int32, Udp2KafkaChannel::ChannelSettings > ChannelSettingsMap;
    typedef std::vector< Udp2KafkaChannel > Udp2KafkaChannelVector;

    CORE::UInt16 m_udpStartPort;
    CORE::UInt16 m_channelCount;
    CORE::Int32 m_kafkaTopicStartChannelID;
    CORE::CString m_kafkaTopicName;
    Udp2KafkaChannelVector m_channels;
    ChannelSettingsMap m_channelSettings;
    COM::CHTTPServer m_httpServer;
    COM::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;
    CORE::CTimer m_metricsTimer;
    bool m_transmitMetrics;
    COMCORE::CUDPSocket m_testUdpSocket;
    CORE::CTimer m_testPacketTransmitTimer;
};

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_APP_UDP2KAFKA ? */
