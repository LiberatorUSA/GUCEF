/*
 *  Udp2RedisCluster: service which pushes UDP packets into Redis
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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::Udp2RedisClusterChannel()
    : CORE::CTaskConsumer()
    , m_channelSettings()
    , m_redisContext( GUCEF_NULL )
    , m_udpSocket( GUCEF_NULL )
    , m_redisMsgQueueOverflowQueue()
    , m_metricsTimer( GUCEF_NULL )
    , m_metrics()
    , m_redisErrorReplies( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::Udp2RedisClusterChannel( const Udp2RedisClusterChannel& src )
    : CORE::CTaskConsumer()
    , m_channelSettings( src.m_channelSettings )
    , m_redisContext( src.m_redisContext )
    , m_udpSocket( GUCEF_NULL )
    , m_redisMsgQueueOverflowQueue( src.m_redisMsgQueueOverflowQueue )
    , m_metricsTimer( GUCEF_NULL )
    , m_metrics()
    , m_redisErrorReplies( src.m_redisErrorReplies )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::~Udp2RedisClusterChannel()
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    delete m_redisContext;
    m_redisContext = GUCEF_NULL;
    
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
    TEventCallback callback3( this, &Udp2RedisClusterChannel::OnUDPSocketOpened );
    SubscribeTo( m_udpSocket                               ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback3                                 );
    TEventCallback callback4( this, &Udp2RedisClusterChannel::OnUDPPacketRecieved );
    SubscribeTo( m_udpSocket                                 ,
                 COMCORE::CUDPSocket::UDPPacketRecievedEvent ,
                 callback4                                   );
    TEventCallback callback5( this, &Udp2RedisClusterChannel::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback5                      );
}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::ChannelSettings::ChannelSettings( void )
    : redisAddress()
    , channelStreamName()
    , udpInterface()
    , udpMulticastToJoin()
    , collectMetrics( false )
    , wantsTestPackage( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::ChannelSettings::ChannelSettings( const ChannelSettings& src )
    : redisAddress( src.redisAddress )
    , channelStreamName( src.channelStreamName )
    , udpInterface( src.udpInterface )
    , udpMulticastToJoin( src.udpMulticastToJoin )
    , collectMetrics( src.collectMetrics )
    , wantsTestPackage( src.wantsTestPackage )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::ChannelSettings&
Udp2RedisClusterChannel::ChannelSettings::operator=( const ChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        redisAddress = src.redisAddress;
        channelStreamName = src.channelStreamName;
        udpInterface = src.udpInterface;
        udpMulticastToJoin = src.udpMulticastToJoin;
        collectMetrics = src.collectMetrics;
        wantsTestPackage = src.wantsTestPackage;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisClusterChannel::LoadConfig( const ChannelSettings& channelSettings )
{GUCEF_TRACE;

    m_channelSettings = channelSettings;
    return true;
}

/*-------------------------------------------------------------------------*/

const Udp2RedisClusterChannel::ChannelSettings& 
Udp2RedisClusterChannel::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
Udp2RedisClusterChannel::GetRedisErrorRepliesCounter( bool resetCounter )
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

CORE::CString
Udp2RedisClusterChannel::GetType( void ) const
{GUCEF_TRACE;

    return "Udp2RedisClusterChannel";
}

/*-------------------------------------------------------------------------*/

Udp2RedisClusterChannel::ChannelMetrics::ChannelMetrics( void )
    : udpBytesReceived( 0 )
    , udpMessagesReceived( 0 )
    , redisMessagesTransmitted( 0 )
    , redisTransmitOverflowQueueSize( 0 )
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
    m_metrics.udpMessagesReceived = m_udpSocket->GetNrOfDataReceivedEvents( true );
    m_metrics.redisTransmitOverflowQueueSize = (CORE::UInt32) m_redisMsgQueueOverflowQueue.size();
    m_metrics.redisErrorReplies = GetRedisErrorRepliesCounter( true );
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

bool
Udp2RedisClusterChannel::RedisSend( const CORE::CDynamicBuffer& udpPacket )
{GUCEF_TRACE;

    try
    {
        static const CORE::CString fieldName = "UDP";
        static const CORE::CString msgId = "*";

        sw::redis::StringView cnSV( m_channelSettings.channelStreamName.C_String(), m_channelSettings.channelStreamName.Length() );
        sw::redis::StringView idSV( msgId.C_String(), msgId.Length() );
        sw::redis::StringView fnSV( fieldName.C_String(), fieldName.Length() );
        sw::redis::StringView fvSV( (const char*) udpPacket.GetConstBufferPtr(), udpPacket.GetDataSize() );

        std::vector< std::pair< sw::redis::StringView, sw::redis::StringView > > args;
        args.push_back( std::pair< sw::redis::StringView, sw::redis::StringView >( fnSV, fvSV ) );

        std::string clusterMsgId = m_redisContext->xadd( cnSV, idSV, args.begin(), args.end() );

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisClusterChannel(" + CORE::PointerToString( this ) + "):RedisConnect: Successfully sent UDP message of " + 
            CORE::UInt32ToString( udpPacket.GetDataSize() ) + " bytes. MsgID=" + clusterMsgId );
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
Udp2RedisClusterChannel::SendQueuedPackagesIfAny( void )
{GUCEF_TRACE;

    int retCode = REDIS_OK;

    while ( !m_redisMsgQueueOverflowQueue.empty() && ( retCode == REDIS_OK ) )
    {
        const CORE::CDynamicBuffer& queuedUdpPacket = m_redisMsgQueueOverflowQueue.front();
        retCode = RedisSend( queuedUdpPacket );
        if ( retCode == REDIS_OK )
        {
            m_redisMsgQueueOverflowQueue.pop_front();
        }
        else
            break;
    }

    return retCode == REDIS_OK;
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnUDPPacketRecieved( CORE::CNotifier* notifier   ,
                                              const CORE::CEvent& eventID ,
                                              CORE::CICloneable* evenData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketRecievedEventData* >( evenData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUDPPacketRecievedEventData& data = udpPacketData->GetData();
        const CORE::CDynamicBuffer& udpPacketBuffer = data.dataBuffer.GetData();

        #ifdef GUCEF_DEBUG_MODE
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel: UDP Socket received a packet from " + data.sourceAddress.AddressAndPortAsString() );
        #endif

        if ( SendQueuedPackagesIfAny() )
        {
            int retCode = RedisSend( udpPacketBuffer );
            if ( retCode == REDIS_ERR )
                m_redisMsgQueueOverflowQueue.push_back( udpPacketBuffer );
        }
        else
        {
            m_redisMsgQueueOverflowQueue.push_back( udpPacketBuffer );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisClusterChannel: UDP Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisClusterChannel::RedisConnect( void )
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
Udp2RedisClusterChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

	m_udpSocket = new COMCORE::CUDPSocket( *GetPulseGenerator(), false );
    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
    m_metricsTimer->SetEnabled( m_channelSettings.collectMetrics );

    RegisterEventHandlers();

    // Setup connection to Redis and open the UDP port.
    // Note that if there is an error here we will just keep on trying automatically
    RedisConnect();
    m_udpSocket->SetMaxUpdatesPerCycle( 10 );
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
    
    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisClusterChannel::OnTaskEnd( CORE::CICloneable* taskData )
{GUCEF_TRACE;

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
            const Udp2RedisClusterChannel::ChannelSettings& channelSettings = (*n).second;
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

    CORE::UInt16 udpPort = m_udpStartPort;
    CORE::Int32 maxChannelId = m_redisStreamStartChannelID + m_channelCount;
    for ( CORE::Int32 channelId = m_redisStreamStartChannelID; channelId < maxChannelId; ++channelId )
    {
        Udp2RedisClusterChannel::ChannelSettings& channelSettings = m_channelSettings[ channelId ];

        channelSettings.collectMetrics = m_transmitMetrics;
        channelSettings.redisAddress.SetHostname( m_redisHost );
        channelSettings.redisAddress.SetPortInHostByteOrder( m_redisPort );

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
        GUCEF_METRIC_GAUGE( metricPrefix + "redisTransmitOverflowQueueSize", metrics.redisTransmitOverflowQueueSize, 1.0f );
        GUCEF_METRIC_COUNT( metricPrefix + "udpBytesReceived", metrics.udpBytesReceived, 1.0f );
        GUCEF_METRIC_COUNT( metricPrefix + "udpMessagesReceived", metrics.udpMessagesReceived, 1.0f );
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
        const Udp2RedisClusterChannel::ChannelSettings& settings = (*i).GetChannelSettings();
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
