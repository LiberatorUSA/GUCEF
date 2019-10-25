/*
 *  Udp2Redis: service which pushes UDP packets into kafka topics
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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#include "udp2redis.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <winsock2.h>
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

Udp2RedisChannel::Udp2RedisChannel()
    : CORE::CTaskConsumer()
    , m_udpPort( 0 )
    , m_redisStreamName()
    , m_redisHost()
    , m_redisPort()
    , m_redisReconnectTimer( GUCEF_NULL )
    , m_redisContext( GUCEF_NULL )
    , m_udpSocket( GUCEF_NULL )
    , m_redisMsgQueueOverflowQueue()
    , m_redisOptions()
{GUCEF_TRACE;

    memset( &m_redisOptions, 0, sizeof(m_redisOptions) );
}

/*-------------------------------------------------------------------------*/

Udp2RedisChannel::Udp2RedisChannel( const Udp2RedisChannel& src )
    : CORE::CTaskConsumer()
    , m_udpPort( src.m_udpPort )
    , m_redisStreamName( src.m_redisStreamName )
    , m_redisHost( src.m_redisHost )
    , m_redisPort( src.m_redisPort )
    , m_redisReconnectTimer( GUCEF_NULL )
    , m_redisContext( src.m_redisContext )
    , m_udpSocket( GUCEF_NULL )
    , m_redisMsgQueueOverflowQueue( src.m_redisMsgQueueOverflowQueue )
    , m_redisOptions( src.m_redisOptions )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Udp2RedisChannel::~Udp2RedisChannel()
{GUCEF_TRACE;

    delete m_redisReconnectTimer;
    m_redisReconnectTimer = GUCEF_NULL;

    delete m_redisOptions.timeout;
    m_redisOptions.timeout = GUCEF_NULL;

    delete m_udpSocket;
    m_udpSocket = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Register UDP socket event handlers
    TEventCallback callback( this, &Udp2RedisChannel::OnUDPSocketError );
    SubscribeTo( m_udpSocket                              ,
                 COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                 callback                                 );
    TEventCallback callback2( this, &Udp2RedisChannel::OnUDPSocketClosed );
    SubscribeTo( m_udpSocket                               ,
                 COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                 callback2                                 );
    TEventCallback callback3( this, &Udp2RedisChannel::OnUDPSocketOpened );
    SubscribeTo( m_udpSocket                               ,
                 COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                 callback3                                 );
    TEventCallback callback4( this, &Udp2RedisChannel::OnUDPPacketRecieved );
    SubscribeTo( m_udpSocket                                 ,
                 COMCORE::CUDPSocket::UDPPacketRecievedEvent ,
                 callback4                                   );
    TEventCallback callback5( this, &Udp2RedisChannel::OnRedisReconnectTimer );
    SubscribeTo( m_redisReconnectTimer          ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback5                      );
}

/*-------------------------------------------------------------------------*/

bool 
Udp2RedisChannel::LoadConfig( CORE::UInt16 udpPort                   ,
                              const CORE::CString& redisHost         ,
                              CORE::UInt16 redisPort                 ,
                              const CORE::CString& channelStreamName )
{GUCEF_TRACE;

    m_udpPort = udpPort;
    m_redisStreamName = channelStreamName;
    m_redisHost = redisHost;
    m_redisPort = redisPort;
    return true;
}

/*-------------------------------------------------------------------------*/

CORE::CString
Udp2RedisChannel::GetType( void ) const
{GUCEF_TRACE;

    return "Udp2RedisChannel";
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::OnUDPSocketError( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventID  ,
                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::TSocketErrorEventData* eData = static_cast< COMCORE::CUDPSocket::TSocketErrorEventData* >( eventData );    
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: UDP Socket experienced error " + CORE::Int32ToString( eData->GetData() ) );
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::OnUDPSocketClosed( CORE::CNotifier* notifier   ,
                                     const CORE::CEvent& eventID ,
                                     CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: UDP Socket has been closed" );
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::OnUDPSocketOpened( CORE::CNotifier* notifier   ,
                                     const CORE::CEvent& eventID ,
                                     CORE::CICloneable* evenData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: UDP Socket has been opened" );
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::OnRedisASyncReply( redisAsyncContext* context , 
                                     void *reply                , 
                                     void *privdata             )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::OnRedisASyncConnect( const struct redisAsyncContext* context , 
                                       int status                              )
{GUCEF_TRACE;

    if ( status == REDIS_OK )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: Connected to Redis" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: Failed to connect to Redis due to an error, retrying" );
        if ( context != GUCEF_NULL && context->data != GUCEF_NULL )
        {
            Udp2RedisChannel* thisObj = static_cast< Udp2RedisChannel* >( context->data );
            
            redisAsyncContext* disconnectedContext = thisObj->m_redisContext;
            thisObj->m_redisContext = GUCEF_NULL;
            thisObj->m_oldRedisContexts.push_back( disconnectedContext );

            thisObj->m_redisReconnectTimer->SetEnabled( true );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::OnRedisASyncDisconnect( const struct redisAsyncContext* context , 
                                          int status                              )
{GUCEF_TRACE;
    
    if ( status == REDIS_OK )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: Disconnected from Redis as requested" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: Disconnected from Redis due to an error, attempting to reconnect" );
        if ( context != GUCEF_NULL && context->data != GUCEF_NULL )
        {
            Udp2RedisChannel* thisObj = static_cast< Udp2RedisChannel* >( context->data );
            
            redisAsyncContext* disconnectedContext = thisObj->m_redisContext;
            thisObj->m_redisContext = GUCEF_NULL;
            thisObj->m_oldRedisContexts.push_back( disconnectedContext );
            //redisAsyncFree( disconnectedContext );

            thisObj->m_redisReconnectTimer->SetEnabled( true );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::OnRedisReconnectTimer( CORE::CNotifier* notifier   ,
                                         const CORE::CEvent& eventID ,
                                         CORE::CICloneable* evenData )
{GUCEF_TRACE;

    m_redisReconnectTimer->SetEnabled( false );
    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel:OnRedisReconnectTimer: Trying to reconnect" );
    RedisConnect();
}

/*-------------------------------------------------------------------------*/

int
Udp2RedisChannel::RedisSend( const CORE::CDynamicBuffer& udpPacket )
{GUCEF_TRACE;

    int retCode = redisAsyncCommand( m_redisContext, 
                                     &OnRedisASyncReply, 
                                     this, 
                                     m_redisStreamSendCmd.C_String(), 
                                     udpPacket.GetConstBufferPtr(), 
                                     udpPacket.GetDataSize() );
    if ( retCode != REDIS_OK )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel:RedisSend: Failed executing async send command" );
    }    
    #ifdef GUCEF_DEBUG_MODE
    else
    {    
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_EVERYTHING, "Udp2RedisChannel:RedisSend: Successfully executed async send command" );
    }
    #endif
    return retCode;
}

/*-------------------------------------------------------------------------*/
                                                                             
bool
Udp2RedisChannel::SendQueuedPackagesIfAny( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_redisContext )
    {
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
    return false;
}

/*-------------------------------------------------------------------------*/

void
Udp2RedisChannel::OnUDPPacketRecieved( CORE::CNotifier* notifier   ,
                                       const CORE::CEvent& eventID ,
                                       CORE::CICloneable* evenData )
{GUCEF_TRACE;

    COMCORE::CUDPSocket::UDPPacketRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketRecievedEventData* >( evenData );
    if ( GUCEF_NULL != udpPacketData )
    {
        const COMCORE::CUDPSocket::TUDPPacketRecievedEventData& data = udpPacketData->GetData();
        const CORE::CDynamicBuffer& udpPacketBuffer = data.dataBuffer.GetData();

        #ifdef GUCEF_DEBUG_MODE
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: UDP Socket received a packet from " + data.sourceAddress.AddressAndPortAsString() );
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
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel: UDP Socket has a data received event but no data was provided" );
    }
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisChannel::RedisConnect( void )
{
    delete m_redisOptions.timeout;
    m_redisOptions.timeout = GUCEF_NULL;

    memset( &m_redisOptions, 0, sizeof(m_redisOptions) );
    REDIS_OPTIONS_SET_TCP( &m_redisOptions, m_redisHost.C_String(), m_redisPort );
    struct timeval* timeoutSetting = new struct timeval;
    timeoutSetting->tv_sec = 10;
    timeoutSetting->tv_usec = 0;
    m_redisOptions.timeout = timeoutSetting;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisChannel:RedisConnect: Connecting to Redis on " + m_redisHost + ":" + CORE::UInt16ToString( m_redisPort ) );
    
    redisAsyncContext* rContext = redisAsyncConnectWithOptions( &m_redisOptions );
	if ( rContext == GUCEF_NULL ) 
    {
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel:RedisConnect: Failed to create Redis context" );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
	}

    // Allow ourselves to find this object again from the C callbacks
    rContext->data = this;

    // Set the stream publish command formatting here which will remain constant for this session
    m_redisStreamSendCmd = "XADD " + m_redisStreamName + " %b";

    redisAsyncSetConnectCallback( rContext, &OnRedisASyncConnect );
    redisAsyncSetDisconnectCallback( rContext, &OnRedisASyncDisconnect );

    m_redisContext = rContext;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisChannel:RedisConnect: Successfully created a Redis context" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
Udp2RedisChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

	m_udpSocket = new GUCEF::COMCORE::CUDPSocket( *GetPulseGenerator(), true );
    m_redisReconnectTimer = new GUCEF::CORE::CTimer( *GetPulseGenerator(), 10 );
        
    RegisterEventHandlers();

    // Setup connection to Redis and open the UDP port.
    // Note that if there is an error here we will just keep on trying
    RedisConnect();
    m_udpSocket->SetAutoReOpenOnError( true );
    if ( !m_udpSocket->Open( m_udpPort ) )
    {
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Udp2RedisChannel:OnTaskStart: Failed to open UDP socket on port " + CORE::UInt16ToString( m_udpPort ) );
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Udp2RedisChannel:OnTaskStart: Successfully opened UDP socket on port " + CORE::UInt16ToString( m_udpPort ) );
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
Udp2RedisChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    // Poll the Redis socket etc,..
    if ( GUCEF_NULL != m_redisContext )
        redisAsyncHandleRead( m_redisContext );
    if ( GUCEF_NULL != m_redisContext )
        redisAsyncHandleWrite( m_redisContext );
    if ( GUCEF_NULL != m_redisContext )
        redisAsyncHandleTimeout( m_redisContext );
        
    auto i = m_oldRedisContexts.begin();
    while ( i != m_oldRedisContexts.end() )
    {
        //redisAsyncFree( (*i) );
        ++i;
    }
    m_oldRedisContexts.clear();
    
    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/
    
void
Udp2RedisChannel::OnTaskEnd( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    redisAsyncDisconnect( m_redisContext );
}

/*-------------------------------------------------------------------------*/

Udp2Redis::Udp2Redis( void )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

Udp2Redis::~Udp2Redis()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
Udp2Redis::Start( void )
{GUCEF_TRACE;

    m_channels.resize( m_channelCount );

    CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();
    
    CORE::UInt16 udpPort = m_udpStartPort;
    CORE::Int32 channelId = m_redisStreamStartChannelID;
    auto& i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        Udp2RedisChannel& channel = (*i);
        
        CORE::CString channelTopicName = m_redisStreamName.ReplaceSubstr( "{channelID}", CORE::Int32ToString( channelId ) ); 
        channel.LoadConfig( udpPort, m_redisHost, m_redisPort, channelTopicName );

        taskManager.StartTask( channel );
        
        ++udpPort;
        ++i;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
Udp2Redis::LoadConfig( const CORE::CValueList& config )
{GUCEF_TRACE;

    m_udpStartPort = CORE::StringToUInt16( config.GetValueAlways( "UdpStartPort", "20000" ) );
    m_channelCount = CORE::StringToUInt16( config.GetValueAlways( "ChannelCount", "1" ) );
    m_redisStreamStartChannelID = CORE::StringToInt32( config.GetValueAlways( "RedisStreamStartChannelID", "1" ) );
    m_redisStreamName = config.GetValueAlways( "RedisStreamName", "udp-ingress-ch{channelID}" );
    m_redisHost = config.GetValueAlways( "RedisHost", "127.0.0.1" );
    m_redisPort = CORE::StringToUInt16( config.GetValueAlways( "RedisPort", "6379" ) );

    return true;
}
