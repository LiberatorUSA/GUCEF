/*
 *  Udp2RedisCluster: service which pushes UDP packets into Redis streams
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

#include "udp2rediscluster.h"

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

#define GUCEF_DEFAULT_REDIS_QUEUE_SEND_BATCH_SIZE           25
#define GUCEF_DEFAULT_UDP_RECEIVE_BUFFERS                   100
#define GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE          10000
#define GUCEF_DEFAULT_UDP_OS_LEVEL_RECEIVE_BUFFER_SIZE      (1024 * 1024 * 10)
#define GUCEF_DEFAULT_UDP_MAX_SOCKET_CYCLES_PER_PULSE       25

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( void )
    : redisAddress()
    , channelStreamName()
    , udpInterface()
    , udpMulticastToJoin()
    , collectMetrics( false )
    , wantsTestPackage( false )
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , nrOfUdpReceiveBuffersPerSocket( GUCEF_DEFAULT_UDP_RECEIVE_BUFFERS )
    , udpSocketOsReceiveBufferSize( GUCEF_DEFAULT_UDP_OS_LEVEL_RECEIVE_BUFFER_SIZE )
    , udpSocketUpdateCyclesPerPulse( GUCEF_DEFAULT_UDP_MAX_SOCKET_CYCLES_PER_PULSE )
    , performRedisWritesInDedicatedThread( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( const ChannelSettings& src )
    : redisAddress( src.redisAddress )
    , channelStreamName( src.channelStreamName )
    , udpInterface( src.udpInterface )
    , udpMulticastToJoin( src.udpMulticastToJoin )
    , collectMetrics( src.collectMetrics )
    , wantsTestPackage( src.wantsTestPackage )
    , ticketRefillOnBusyCycle( src.ticketRefillOnBusyCycle )
    , nrOfUdpReceiveBuffersPerSocket( src.nrOfUdpReceiveBuffersPerSocket )
    , udpSocketOsReceiveBufferSize( src.udpSocketOsReceiveBufferSize )
    , udpSocketUpdateCyclesPerPulse( src.udpSocketUpdateCyclesPerPulse )
    , performRedisWritesInDedicatedThread( src.performRedisWritesInDedicatedThread )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings&
ChannelSettings::operator=( const ChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        redisAddress = src.redisAddress;
        channelStreamName = src.channelStreamName;
        udpInterface = src.udpInterface;
        udpMulticastToJoin = src.udpMulticastToJoin;
        collectMetrics = src.collectMetrics;
        wantsTestPackage = src.wantsTestPackage;
        ticketRefillOnBusyCycle = src.ticketRefillOnBusyCycle;
        nrOfUdpReceiveBuffersPerSocket = src.nrOfUdpReceiveBuffersPerSocket;
        udpSocketOsReceiveBufferSize = src.udpSocketOsReceiveBufferSize;
        udpSocketUpdateCyclesPerPulse = src.udpSocketUpdateCyclesPerPulse;
        performRedisWritesInDedicatedThread = src.performRedisWritesInDedicatedThread;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

ClusterChannelRedisWriter::ClusterChannelRedisWriter()
    : CORE::CTaskConsumer()
    , m_redisContext( GUCEF_NULL )
    , m_redisMsgQueueOverflowQueue()
    , m_redisErrorReplies( 0 )
    , m_redisTransmitQueueSize( 0 )
    , m_redisMsgsTransmitted( 0 )
    , m_channelSettings()
    , m_mailbox()
    , m_metricsTimer( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ClusterChannelRedisWriter::ClusterChannelRedisWriter( const ClusterChannelRedisWriter& src )
    : CORE::CTaskConsumer()
    , m_redisContext( src.m_redisContext )
    , m_redisMsgQueueOverflowQueue( src.m_redisMsgQueueOverflowQueue )
    , m_redisErrorReplies( src.m_redisErrorReplies )
    , m_redisTransmitQueueSize( src.m_redisTransmitQueueSize )
    , m_redisMsgsTransmitted( src.m_redisMsgsTransmitted )
    , m_channelSettings( src.m_channelSettings )
    , m_mailbox()
    , m_metricsTimer( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ClusterChannelRedisWriter::~ClusterChannelRedisWriter()
{GUCEF_TRACE;

    delete m_redisContext;
    m_redisContext = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
ClusterChannelRedisWriter::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &ClusterChannelRedisWriter::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );
}

/*-------------------------------------------------------------------------*/

CORE::CString
ClusterChannelRedisWriter::GetType( void ) const
{GUCEF_TRACE;

    return "ClusterChannelRedisWriter";
}

/*-------------------------------------------------------------------------*/

void
ClusterChannelRedisWriter::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_channelSettings.performRedisWritesInDedicatedThread )
    {
        // There can be lock contention on AmountOfMail but thats why we only do it at the interval of the metrics timer
        m_redisTransmitQueueSize = (CORE::UInt32) m_redisMsgQueueOverflowQueue.size() + m_mailbox.AmountOfMail();
    }
    else
    {
        m_redisTransmitQueueSize = (CORE::UInt32) m_redisMsgQueueOverflowQueue.size();
    }
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
    m_metricsTimer->SetEnabled( m_channelSettings.collectMetrics );

    if ( m_channelSettings.performRedisWritesInDedicatedThread )
    {
        // Set the minimum number of cycles we will go full speed if a single cycle was not enough to handle
        // all the processing. This will cause a bypass of CPU yielding if/when the situation arises.
        // In such a case the thread will run at max speed for a least the below set nr of cycles.
        GetPulseGenerator()->RequestPulsesPerImmediatePulseRequest( m_channelSettings.ticketRefillOnBusyCycle );
    }

    RegisterEventHandlers();

    // Setup connection to Redis 
    // Note that if there is an error here we will just keep on trying automatically
    RedisConnect();

    return true;
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;
    
    if ( m_channelSettings.performRedisWritesInDedicatedThread && m_redisMsgQueueOverflowQueue.empty() )
    {
        if ( m_redisMsgQueueOverflowQueue.empty() )
        {
            CORE::CICloneable* mail = GUCEF_NULL;
            CORE::UInt32 packetCount = 0;
            if ( m_mailbox.GetMail( packetCount, &mail ) )
            {
                TCloneablePacketEntryVector* udpPackets = static_cast< TCloneablePacketEntryVector* >( mail );
                if ( GUCEF_NULL != udpPackets )
                {
                    RedisSendSync( *udpPackets, packetCount );
                    delete mail;
                }
            }
        }
    }

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void 
ClusterChannelRedisWriter::OnTaskEnding( CORE::CICloneable* taskdata ,
                                         bool willBeForced           )
{GUCEF_TRACE;
    

}

/*-------------------------------------------------------------------------*/

void
ClusterChannelRedisWriter::OnTaskEnd( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32 
ClusterChannelRedisWriter::GetRedisTransmitQueueSize( void ) const
{GUCEF_TRACE;

    return m_redisTransmitQueueSize;    
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
ClusterChannelRedisWriter::GetRedisErrorRepliesCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 redisErrorReplies = m_redisErrorReplies;
        m_redisErrorReplies = 0;
        return redisErrorReplies;
    }
    else
        return m_redisErrorReplies;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
ClusterChannelRedisWriter::GetRedisMsgsTransmittedCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 redisMsgsTransmitted = m_redisMsgsTransmitted;
        m_redisMsgsTransmitted = 0;
        return redisMsgsTransmitted;
    }
    else
        return m_redisMsgsTransmitted;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
ClusterChannelRedisWriter::GetRedisPacketsInMsgsTransmittedCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 redisPacketsInMsgsTransmitted = m_redisPacketsInMsgsTransmitted;
        m_redisPacketsInMsgsTransmitted = 0;
        return redisPacketsInMsgsTransmitted;
    }
    else
        return m_redisPacketsInMsgsTransmitted;
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::RedisSendSync( const TPacketEntryVector& udpPackets , 
                                          CORE::UInt32 packetCount             )
{GUCEF_TRACE;

    if ( SendQueuedPackagesIfAny() )
    {
        if ( RedisSendSyncImpl( udpPackets, packetCount ) )
        {
            return true;
        }
        else
        {
            return AddToOverflowQueue( udpPackets, packetCount );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::RedisSendSyncImpl( const TPacketEntryVector& udpPackets , 
                                              CORE::UInt32 packetCount             )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
        return false;
    
    try
    {
        static const CORE::CString fieldName = "UDP";
        static const CORE::CString msgId = "*";

        sw::redis::StringView cnSV( m_channelSettings.channelStreamName.C_String(), m_channelSettings.channelStreamName.Length() );
        sw::redis::StringView idSV( msgId.C_String(), msgId.Length() );
        sw::redis::StringView fnSV( fieldName.C_String(), fieldName.Length() );
        
        std::vector< std::pair< sw::redis::StringView, sw::redis::StringView > > args;
        args.reserve( packetCount );

        for ( CORE::UInt32 i=0; i<packetCount; ++i )
        {
            const CORE::CDynamicBuffer& packetBuffer = udpPackets[ i ].dataBuffer.GetData();
            sw::redis::StringView fvSV( (const char*) packetBuffer.GetConstBufferPtr(), packetBuffer.GetDataSize() );
            args.push_back( std::pair< sw::redis::StringView, sw::redis::StringView >( fnSV, fvSV ) );
        }

        std::string clusterMsgId = m_redisContext->xadd( cnSV, idSV, args.begin(), args.end() );

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisSend: Successfully sent " + 
            CORE::UInt32ToString( packetCount ) + " UDP messages. MsgID=" + clusterMsgId );

        ++m_redisMsgsTransmitted;
        m_redisPacketsInMsgsTransmitted += packetCount;
        return true;
    }
    catch ( const sw::redis::MovedError& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisSend: Redis++ MovedError (Redirect failed?) exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisSend: Redis++ exception: " + e.what() );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisSend: exception: " + e.what() );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::RedisSendASync( const TPacketEntryVector& udpPackets , 
                                           CORE::UInt32 packetCount             )
{GUCEF_TRACE;

    // Since the Redis library is blocking we cannot do async unless we have a thread
    if ( !m_channelSettings.performRedisWritesInDedicatedThread )
        return false;
    
    // We dont want to copy empty buffers needlessly so we size the mail to match the actual nr of packets
    // We need to perform a memory copy regardless because the socket needs ownership of its buffers back
    
    TCloneablePacketEntryVector mail;
    mail.resize( packetCount );

    // Link the external buffers into the cut-to-size mail envelope
    // The mailbox will do the cloning, we just link here
    for ( CORE::UInt32 i=0; i<packetCount; ++i )
    {
        mail[ i ].dataBuffer.LinkTo( &udpPackets[ i ].dataBuffer.GetData() );
        mail[ i ].sourceAddress = udpPackets[ i ].sourceAddress;
    }
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "ClusterChannelRedisWriter(" + CORE::PointerToString( this ) + "):RedisSendASync: Adding group of "
            + CORE::UInt32ToString( packetCount ) + " packets to the mailbox" );

    m_mailbox.AddMail( packetCount, &mail ); 
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::SendQueuedPackagesIfAny( void )
{GUCEF_TRACE;

    TPacketEntryVector packetBundle;
    size_t i=0;
    TPacketEntryQueue::iterator b = m_redisMsgQueueOverflowQueue.begin();
    TPacketEntryQueue::iterator e = m_redisMsgQueueOverflowQueue.begin();
    while ( i < GUCEF_DEFAULT_REDIS_QUEUE_SEND_BATCH_SIZE && i < m_redisMsgQueueOverflowQueue.size() )
    {
        packetBundle.push_back( m_redisMsgQueueOverflowQueue.at( i ) );
        ++i; ++e;    
    }

    if ( !packetBundle.empty() )
    {
        if ( RedisSendSyncImpl( packetBundle, packetBundle.size() ) )
        {
            m_redisMsgQueueOverflowQueue.erase( b, e );
            return true;
        }
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::AddToOverflowQueue( const TPacketEntryVector& udpPackets ,
                                               CORE::UInt32 packetCount             )
{GUCEF_TRACE;

    for ( CORE::UInt32 i=0; i<packetCount; ++i )
    {
        m_redisMsgQueueOverflowQueue.push_back( udpPackets[ i ] );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::RedisConnect( void )
{GUCEF_TRACE;

    try
    {
        sw::redis::ConnectionOptions rppConnectionOptions;
        rppConnectionOptions.host = m_channelSettings.redisAddress.GetHostname();  // Required.
        rppConnectionOptions.port = m_channelSettings.redisAddress.GetPortInHostByteOrder(); // Optional. The default port is 6379.
        //rppConnectionOptions.password = "auth";   // Optional. No password by default.
        //rppConnectionOptions.db = 1;  // Optional. Use the 0th database by default.

        // Optional. Timeout before we successfully send request to or receive response from redis.
        // By default, the timeout is 0ms, i.e. never timeout and block until we send or receive successfuly.
        // NOTE: if any command is timed out, we throw a TimeoutError exception.
        rppConnectionOptions.socket_timeout = std::chrono::milliseconds( 100 );
        rppConnectionOptions.connect_timeout = std::chrono::milliseconds( 100 );

        rppConnectionOptions.keep_alive = true;
        
        // Connect to Redis server with a single connection
        delete m_redisContext;
        m_redisContext = new sw::redis::RedisCluster( rppConnectionOptions );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisConnect: Successfully created a Redis context" );
        return true;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisConnect: Redis++ exception: " + e.what() );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisConnect: exception: " + e.what() );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
ClusterChannelRedisWriter::LoadConfig( const ChannelSettings& channelSettings )
{GUCEF_TRACE;

    m_channelSettings = channelSettings;
    return true;
}

/*-------------------------------------------------------------------------*/

const ChannelSettings& 
ClusterChannelRedisWriter::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::Udp2RedisClusterChannel()
    : CORE::CTaskConsumer()
    , m_channelSettings()
    , m_udpSocket( GUCEF_NULL )
    , m_metricsTimer( GUCEF_NULL )
    , m_metrics()
    , m_redisWriter()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::Udp2RedisClusterChannel( const Udp2RedisClusterChannel& src )
    : CORE::CTaskConsumer()
    , m_channelSettings( src.m_channelSettings )
    , m_udpSocket( GUCEF_NULL )
    , m_metricsTimer( GUCEF_NULL )
    , m_metrics()
    , m_redisWriter( src.m_redisWriter )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::~Udp2RedisClusterChannel()
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
    
    delete m_udpSocket;
    m_udpSocket = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Register UDP socket event handlers
    TEventCallback callback( this, &Udp2RedisClusterChannel::OnUDPSocketError );
    SubscribeTo( m_udpSocket                              ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback                                 );
    TEventCallback callback2( this, &Udp2RedisClusterChannel::OnUDPSocketClosed );
    SubscribeTo( m_udpSocket                               ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback2                                 );
    TEventCallback callback3( this, &Udp2RedisClusterChannel::OnUDPSocketClosing );
    SubscribeTo( m_udpSocket                                ,
                 COMCORE::CUDPSocket::UDPSocketClosingEvent ,
                 callback3                                  );
    TEventCallback callback4( this, &Udp2RedisClusterChannel::OnUDPSocketOpened );
    SubscribeTo( m_udpSocket                               ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback4                                 );
    TEventCallback callback5( this, &Udp2RedisClusterChannel::OnUDPPacketsRecieved );
    SubscribeTo( m_udpSocket                                  ,
                 COMCORE::CUDPSocket::UDPPacketsRecievedEvent ,
                 callback5                                    );
    
    TEventCallback callback6( this, &Udp2RedisClusterChannel::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback6                      );
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisClusterChannel::LoadConfig( const ChannelSettings& channelSettings )
{GUCEF_TRACE;

    m_channelSettings = channelSettings;
    return m_redisWriter.LoadConfig( channelSettings );
}

/*-------------------------------------------------------------------------*/

const ChannelSettings& 
Udp2RedisClusterChannel::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}

/*-------------------------------------------------------------------------*/

CORE::CString
Udp2RedisClusterChannel::GetType( void ) const
{GUCEF_TRACE;

    return "Udp2RedisClusterChannel";
}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::ChannelMetrics::ChannelMetrics( void )
    : udpBytesReceived( 0 )
    , udpPacketsReceived( 0 )
    , redisMessagesTransmitted( 0 )
    , redisPacketsInMsgsTransmitted( 0 )
    , redisTransmitQueueSize( 0 )
    , redisErrorReplies( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    m_metrics.udpBytesReceived = m_udpSocket->GetBytesReceived( true );
    m_metrics.udpPacketsReceived = m_udpSocket->GetNrOfDataReceivedEvents( true );
    m_metrics.redisTransmitQueueSize = m_redisWriter.GetRedisTransmitQueueSize();
    m_metrics.redisMessagesTransmitted = m_redisWriter.GetRedisMsgsTransmittedCounter( true );
    m_metrics.redisPacketsInMsgsTransmitted = m_redisWriter.GetRedisPacketsInMsgsTransmittedCounter( true );
    m_metrics.redisErrorReplies = m_redisWriter.GetRedisErrorRepliesCounter( true );
}

/*-------------------------------------------------------------------------*/

const Udp2RedisClusterChannel::ChannelMetrics&
Udp2RedisClusterChannel::GetMetrics( void ) const
{GUCEF_TRACE;

    return m_metrics;
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnUDPSocketError( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventID  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::TSocketErrorEventData* eData = static_cast< COMCORE::CUDPSocket::TSocketErrorEventData* >( eventData );
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel: UDP Socket experienced error " + CORE::Int32ToString( eData->GetData() ) );
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnUDPSocketClosed( CORE::CNotifier* notifier   ,
                                            const CORE::CEvent& eventID ,
                                            CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel: UDP Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnUDPSocketClosing( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventID  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel: UDP Socket is going to close" );

    // Gracefully leave the multicast groups we joined
    ChannelSettings::HostAddressVector::iterator m = m_channelSettings.udpMulticastToJoin.begin();
    while ( m != m_channelSettings.udpMulticastToJoin.end() )
    {
        const COMCORE::CHostAddress& multicastAddr = (*m);
        if ( m_udpSocket->Leave( multicastAddr ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisClusterChannel:OnUDPSocketClosing: Successfully to left multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel:OnUDPSocketClosing: Failed to leave multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
        }
        ++m;
    }
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnUDPSocketOpened( CORE::CNotifier* notifier   ,
                                            const CORE::CEvent& eventID ,
                                            CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel: UDP Socket has been opened" );

    ChannelSettings::HostAddressVector::iterator m = m_channelSettings.udpMulticastToJoin.begin();
    while ( m != m_channelSettings.udpMulticastToJoin.end() )
    {
        const COMCORE::CHostAddress& multicastAddr = (*m);
        if ( m_udpSocket->Join( multicastAddr ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisClusterChannel:OnUDPSocketOpened: Successfully to joined multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel:OnUDPSocketOpened: Failed to join multicast " + multicastAddr.AddressAndPortAsString() +
                    " for UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
        }
        ++m;
    }
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnUDPPacketsRecieved( CORE::CNotifier* notifier   ,
                                               const CORE::CEvent& eventID ,
                                               CORE::CICloneable* evenData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketsRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketsRecievedEventData* >( evenData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUdpPacketsRecievedEventData& data = udpPacketData->GetData();

        if ( m_channelSettings.performRedisWritesInDedicatedThread )
        {
            if ( !m_redisWriter.RedisSendASync( data.packets, data.packetsReceived ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel:OnUDPPacketsRecieved: Failed to submit " + 
                        CORE::UInt32ToString( data.packetsReceived ) + " packets for async Redis transmission. Data loss will occur!!!" );
            }
        }
        else
        {
            if ( !m_redisWriter.RedisSendSync( data.packets, data.packetsReceived ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel:OnUDPPacketsRecieved: Failed to request sync blocking transmission of " + 
                        CORE::UInt32ToString( data.packetsReceived ) + " packets to Redis. Data loss will occur!!!" );
            }
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel: UDP Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisClusterChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    if ( m_channelSettings.performRedisWritesInDedicatedThread )
    {
        CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();
        if ( !taskManager.StartTask( m_redisWriter ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "Udp2RedisClusterChannel:OnTaskStart: Failed to start dedicated task (dedicated thread) for Redis writes" );
            return false;
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisClusterChannel:OnTaskStart: Successfully requested the launch of a dedicated task (dedicated thread) for Redis writes" );
    }
    else
    {
        if ( !m_redisWriter.OnTaskStart( taskData ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "Udp2RedisClusterChannel:OnTaskStart: Failed to start dedicated task (dedicated thread) for Redis writes" );
            return false;
        }
    }

	m_udpSocket = new COMCORE::CUDPSocket( *GetPulseGenerator(), false );
    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
    m_metricsTimer->SetEnabled( m_channelSettings.collectMetrics );

    // Set the minimum number of cycles we will go full speed if a single cycle was not enough to handle
    // all the processing. This will cause a bypass of CPU yielding if/when the situation arises.
    // In such a case the thread will run at max speed for a least the below set nr of cycles.
    GetPulseGenerator()->RequestPulsesPerImmediatePulseRequest( m_channelSettings.ticketRefillOnBusyCycle );

    RegisterEventHandlers();

    // Configure and open the UDP port.
    // Note that if there is an error here we will just keep on trying automatically
    m_udpSocket->SetMaxUpdatesPerCycle( m_channelSettings.udpSocketUpdateCyclesPerPulse );
    m_udpSocket->SetOsLevelSocketReceiveBufferSize( m_channelSettings.udpSocketOsReceiveBufferSize );
    m_udpSocket->SetNrOfReceiveBuffers( m_channelSettings.nrOfUdpReceiveBuffersPerSocket );
    m_udpSocket->SetAutoReOpenOnError( true );
    if ( m_udpSocket->Open( m_channelSettings.udpInterface ) )
    {
		GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisClusterChannel:OnTaskStart: Successfully opened UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel:OnTaskStart: Failed to open UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisClusterChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    if ( !m_channelSettings.performRedisWritesInDedicatedThread )
    {
        m_redisWriter.OnTaskCycle( taskData );
    }
    
    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void 
Udp2RedisClusterChannel::OnTaskEnding( CORE::CICloneable* taskdata ,
                                       bool willBeForced           )
{GUCEF_TRACE;
    
    if ( !m_channelSettings.performRedisWritesInDedicatedThread )
    {
        m_redisWriter.OnTaskEnding( taskdata, willBeForced );
    }
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnTaskEnd( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    m_udpSocket->Close();
    delete m_udpSocket;
    m_udpSocket = GUCEF_NULL;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    if ( !m_channelSettings.performRedisWritesInDedicatedThread )
    {
        m_redisWriter.OnTaskEnd( taskData );
    }
}

/*-------------------------------------------------------------------------*/

RestApiUdp2RedisInfoResource::RestApiUdp2RedisInfoResource( Udp2RedisCluster* app )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiUdp2RedisInfoResource::~RestApiUdp2RedisInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiUdp2RedisInfoResource::Serialize( CORE::CDataNode& output             ,
                                         const CORE::CString& representation )
{GUCEF_TRACE;

    output.SetName( "info" );
    output.SetAttribute( "application", "udp2rediscluster" );
    output.SetAttribute( "buildDateTime", __TIMESTAMP__ );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiUdp2RedisConfigResource::RestApiUdp2RedisConfigResource( Udp2RedisCluster* app, bool appConfig )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiUdp2RedisConfigResource::~RestApiUdp2RedisConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiUdp2RedisConfigResource::Serialize( CORE::CDataNode& output             ,
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

Udp2RedisCluster::Udp2RedisCluster( void )
    : CORE::CObserver()
    , m_udpStartPort()
    , m_channelCount()
    , m_redisStreamStartChannelID()
    , m_redisStreamName()
    , m_redisHost()
    , m_redisPort()
    , m_channels()
    , m_channelSettings()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_transmitMetrics( true )
    , m_testUdpSocket( false )
    , m_testPacketTransmitTimer()
{GUCEF_TRACE;

    TEventCallback callback1( this, &Udp2RedisCluster::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback1                      );

    TEventCallback callback2( this, &Udp2RedisCluster::OnTransmitTestPacketTimerCycle );
    SubscribeTo( &m_testPacketTransmitTimer     ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback2                      );
}

/*-------------------------------------------------------------------------*/

Udp2RedisCluster::~Udp2RedisCluster()
{GUCEF_TRACE;

    m_httpServer.Close();
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisCluster::Start( void )
{GUCEF_TRACE;

    bool errorOccured = false;
    m_channels.resize( m_channelCount );

    CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();

    CORE::UInt16 udpPort = m_udpStartPort;
    CORE::Int32 channelId = m_redisStreamStartChannelID;
    Udp2RedisClusterChannelVector::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        Udp2RedisClusterChannel& channel = (*i);
        ChannelSettingsMap::iterator n = m_channelSettings.find( channelId );
        if ( n != m_channelSettings.end() )
        {
            const ChannelSettings& channelSettings = (*n).second;
            if ( !channel.LoadConfig( channelSettings ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisCluster:Start: Failed to set channel settings on channel " + CORE::Int32ToString( channelId ) );
                errorOccured = true;
                break;
            }

            if ( !taskManager.StartTask( channel ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisCluster:Start: Failed to start task (dedicated thread) for channel " + CORE::Int32ToString( channelId ) );
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
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisCluster: Opening REST API" );
        return m_httpServer.Listen();
    }
    return errorOccured;
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisCluster::LoadConfig( const CORE::CValueList& appConfig   ,
                              const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    m_transmitMetrics = CORE::StringToBool( appConfig.GetValueAlways( "TransmitMetrics", "true" ) );

    m_udpStartPort = CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "UdpStartPort", "20000" ) ) );
    m_channelCount = CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "ChannelCount", "1" ) ) );
    m_redisStreamStartChannelID = CORE::StringToInt32( CORE::ResolveVars(  appConfig.GetValueAlways( "RedisStreamStartChannelID", "1" ) ) );
    m_redisStreamName = CORE::ResolveVars( appConfig.GetValueAlways( "RedisStreamName", "udp-ingress-ch{channelID}" ) );
    m_redisHost = CORE::ResolveVars( appConfig.GetValueAlways( "RedisHost", "127.0.0.1" ) );
    m_redisPort = CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RedisPort", "6379" ) ) );
    
    CORE::UInt32 ticketRefillOnBusyCycle = CORE::StringToUInt32( CORE::ResolveVars( appConfig.GetValueAlways( "TicketRefillOnBusyCycle" ) ), GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE );
    CORE::UInt32 nrOfUdpReceiveBuffersPerSocket = CORE::StringToUInt32( CORE::ResolveVars( appConfig.GetValueAlways( "NrOfUdpReceiveBuffersPerSocket" ) ), GUCEF_DEFAULT_UDP_RECEIVE_BUFFERS );
    CORE::UInt32 udpSocketOsReceiveBufferSize = CORE::StringToUInt32( CORE::ResolveVars( appConfig.GetValueAlways( "UdpSocketOsReceiveBufferSize" ) ), GUCEF_DEFAULT_UDP_OS_LEVEL_RECEIVE_BUFFER_SIZE );
    CORE::UInt32 udpSocketUpdateCyclesPerPulse = CORE::StringToUInt32( CORE::ResolveVars( appConfig.GetValueAlways( "MaxUdpSocketUpdateCyclesPerPulse" ) ), GUCEF_DEFAULT_UDP_MAX_SOCKET_CYCLES_PER_PULSE );
    bool performRedisWritesInDedicatedThread = CORE::StringToBool( CORE::ResolveVars( appConfig.GetValueAlways( "PerformRedisWritesInDedicatedThread" ) ), true );

    CORE::UInt16 udpPort = m_udpStartPort;
    CORE::Int32 maxChannelId = m_redisStreamStartChannelID + m_channelCount;
    for ( CORE::Int32 channelId = m_redisStreamStartChannelID; channelId < maxChannelId; ++channelId )
    {
        ChannelSettings& channelSettings = m_channelSettings[ channelId ];

        channelSettings.collectMetrics = m_transmitMetrics;
        channelSettings.redisAddress.SetHostname( m_redisHost );
        channelSettings.redisAddress.SetPortInHostByteOrder( m_redisPort );
        channelSettings.ticketRefillOnBusyCycle = ticketRefillOnBusyCycle;
        channelSettings.nrOfUdpReceiveBuffersPerSocket = nrOfUdpReceiveBuffersPerSocket;
        channelSettings.udpSocketOsReceiveBufferSize = udpSocketOsReceiveBufferSize;
        channelSettings.udpSocketUpdateCyclesPerPulse = udpSocketUpdateCyclesPerPulse;
        channelSettings.performRedisWritesInDedicatedThread = performRedisWritesInDedicatedThread;

        CORE::CString settingName = "ChannelSetting." + CORE::Int32ToString( channelId ) + ".RedisStreamName";
        CORE::CString settingValue = appConfig.GetValueAlways( settingName );
        if ( !settingValue.IsNULLOrEmpty() )
        {
            channelSettings.channelStreamName = CORE::ResolveVars( settingValue.ReplaceSubstr( "{channelID}", CORE::Int32ToString( channelId ) ) );
        }
        else
        {
            // Use the auto naming and numbering scheme based on a single template name instead
            channelSettings.channelStreamName = CORE::ResolveVars( m_redisStreamName.ReplaceSubstr( "{channelID}", CORE::Int32ToString( channelId ) ) );
        }

        settingName = "ChannelSetting." + CORE::Int32ToString( channelId ) + ".UdpInterface";
        settingValue = appConfig.GetValueAlways( settingName );
        if ( !settingValue.IsNULLOrEmpty() )
        {
            channelSettings.udpInterface.SetHostnameAndPort( settingValue );
        }
        else
        {
            // Use the auto numbering scheme instead
            channelSettings.udpInterface.SetPortInHostByteOrder( udpPort );
        }

        settingName = "ChannelSetting." + CORE::Int32ToString( channelId ) + ".WantsTestPackage";
        channelSettings.wantsTestPackage = CORE::StringToBool( appConfig.GetValueAlways( settingName, "false" ) );

        settingName = "ChannelSetting." + CORE::Int32ToString( channelId ) + ".Multicast.Join";
        CORE::CValueList::TStringVector settingValues = appConfig.GetValueVectorAlways( settingName );
        CORE::CValueList::TStringVector::iterator n = settingValues.begin();
        while ( n != settingValues.end() )
        {
            const CORE::CString& settingValue = (*n);
            COMCORE::CHostAddress multicastAddress( settingValue );
            channelSettings.udpMulticastToJoin.push_back( multicastAddress );
            ++n;
        }

        ++udpPort;
    }

    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );

    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort", "10000" ) ) ) );

    m_httpRouter.SetResourceMapping( "/info", RestApiUdp2RedisInfoResource::THTTPServerResourcePtr( new RestApiUdp2RedisInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiUdp2RedisInfoResource::THTTPServerResourcePtr( new RestApiUdp2RedisConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiUdp2RedisInfoResource::THTTPServerResourcePtr( new RestApiUdp2RedisConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping(  CORE::ResolveVars( appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ) ), RestApiUdp2RedisInfoResource::THTTPServerResourcePtr( new COM::CDummyHTTPServerResource() )  );

    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );
    
    m_testPacketTransmitTimer.SetInterval( CORE::StringToUInt32( appConfig.GetValueAlways( "TestPacketTransmissionIntervalInMs", "1000" ) ) );
    m_testPacketTransmitTimer.SetEnabled( CORE::StringToBool( appConfig.GetValueAlways( "TransmitTestPackets", "false" ) ) );
    return true;
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisCluster::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CORE::Int32 channelId = m_redisStreamStartChannelID;
    Udp2RedisClusterChannelVector::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        const Udp2RedisClusterChannel::ChannelMetrics& metrics = (*i).GetMetrics();
        CORE::CString metricPrefix = "udp2redis.ch" + CORE::Int32ToString( channelId ) + ".";

        GUCEF_METRIC_COUNT( metricPrefix + "redisErrorReplies", metrics.redisErrorReplies, 1.0f );
        GUCEF_METRIC_COUNT( metricPrefix + "redisMessagesTransmitted", metrics.redisMessagesTransmitted, 1.0f );
        GUCEF_METRIC_COUNT( metricPrefix + "redisPacketsInMessagesTransmitted", metrics.redisPacketsInMsgsTransmitted, 1.0f );
        GUCEF_METRIC_GAUGE( metricPrefix + "redisTransmitQueueSize", metrics.redisTransmitQueueSize, 1.0f );
        GUCEF_METRIC_COUNT( metricPrefix + "udpBytesReceived", metrics.udpBytesReceived, 1.0f );
        GUCEF_METRIC_COUNT( metricPrefix + "udpPacketsReceived", metrics.udpPacketsReceived, 1.0f );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisCluster::OnTransmitTestPacketTimerCycle( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( !m_testUdpSocket.IsActive() )
        if ( !m_testUdpSocket.Open() )
            return;
    
    Udp2RedisClusterChannelVector::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        const ChannelSettings& settings = (*i).GetChannelSettings();
        if ( settings.wantsTestPackage )
        {
            m_testUdpSocket.SendPacketTo( settings.udpInterface, "TEST", 4 );
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList&
Udp2RedisCluster::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
Udp2RedisCluster::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/
