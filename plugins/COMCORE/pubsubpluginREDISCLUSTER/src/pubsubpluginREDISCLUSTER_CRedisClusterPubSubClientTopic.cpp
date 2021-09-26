/*
 *  pubsubpluginREDISCLUSTER: Generic GUCEF COMCORE plugin for providing pubsub via Redis
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

#ifndef PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H
#include "pubsubpluginREDISCLUSTER_CRedisClusterPubSubClient.h"
#define PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H ? */

#include "pubsubpluginREDISCLUSTER_CRedisClusterPubSubClientTopic.h"

#include "crc16.h"
#include "redis_cluster.hpp"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace REDISCLUSTER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopic::CRedisClusterPubSubClientTopic( CRedisClusterPubSubClient* client )
    : COMCORE::CPubSubClientTopic()
    , m_client( client )
    , m_redisPipeline( GUCEF_NULL )
    , m_redisContext( GUCEF_NULL )
    , m_redisErrorReplies( 0 )
    , m_redisTransmitQueueSize( 0 )
    , m_redisMsgsTransmitted( 0 )
    , m_redisFieldsInMsgsTransmitted( 0 )
    , m_redisFieldsInMsgsRatio( 0 )
    , m_redisHashSlot( GUCEFMT_UINT32MAX )
    , m_redisXreadCount( 1000 )
    , m_redisXreadBlockTimeoutInMs( 1000 )
    , m_redisReadMsgs()
    , m_redisMsgArgs()
    , m_redisShardHost()
    , m_redisShardNodeId()
    , m_redisReconnectTimer( GUCEF_NULL )
    , m_config()
    , m_readOffset( "0" )
    , m_readerThread()
    , m_lock()
{GUCEF_TRACE;

    if ( GUCEF_NULL != client->GetConfig().pulseGenerator )
    {
        if ( client->GetConfig().desiredFeatures.supportsAutoReconnect )
        {
            m_redisReconnectTimer = new CORE::CTimer( *client->GetConfig().pulseGenerator, client->GetConfig().reconnectDelayInMs );
        }
    }
    else
    {
        if ( client->GetConfig().desiredFeatures.supportsAutoReconnect )
        {
            m_redisReconnectTimer = new CORE::CTimer( client->GetConfig().reconnectDelayInMs );
        }
    }
        
    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopic::~CRedisClusterPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();
}


/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_redisReconnectTimer )
    {
        TEventCallback callback( this, &CRedisClusterPubSubClientTopic::OnRedisReconnectTimerCycle );
        SubscribeTo( m_redisReconnectTimer          ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CRedisClusterPubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::Publish( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    const CORE::CVariant& msgId = msg.GetMsgId();
    sw::redis::StringView idSV( msgId.AsCharPtr(), msgId.ByteSize( false ) );
    
    const CORE::CVariant& primaryPayload = msg.GetPrimaryPayload();
    const COMCORE::CIPubSubMsg::TKeyValuePairs& kvPairs = msg.GetKeyValuePairs();
    bool hasNoPrimaryPayload = primaryPayload.IsNULLOrEmpty();

    // We use a data member for argument reference storage so we dont have to keep
    // allocating a new array (vector) on every call to Publish()
    m_redisMsgArgs.clear();
    m_redisMsgArgs.reserve( (size_t) kvPairs.size() + ( hasNoPrimaryPayload ? 0 : 1 ) );
    
    if ( !hasNoPrimaryPayload )
    {
        // This is really not appropriote use of the redis backend
        // its strongly prefered for the application to specify a proper key name to ensure its non-conflicting
        
        static const CORE::CString& fieldName = "PRIMARYPAYLOAD";
        sw::redis::StringView fnSV( fieldName.C_String(), fieldName.ByteSize()-1 );

        sw::redis::StringView fvSV( primaryPayload.AsCharPtr(), primaryPayload.ByteSize( false ) );

        m_redisMsgArgs.push_back( std::pair< sw::redis::StringView, sw::redis::StringView >( fnSV, fvSV ) );
    }    
    
    COMCORE::CIPubSubMsg::TKeyValuePairs::const_iterator i = kvPairs.begin();
    while ( i != kvPairs.end() )
    {
        const CORE::CVariant& fieldName = (*i).first;
        sw::redis::StringView fnSV( fieldName.AsCharPtr(), fieldName.ByteSize( false ) );

        const CORE::CVariant& fieldValue = (*i).second;
        sw::redis::StringView fvSV( fieldValue.AsCharPtr(), fieldValue.ByteSize( false ) );

        m_redisMsgArgs.push_back( std::pair< sw::redis::StringView, sw::redis::StringView >( fnSV, fvSV ) );
        
        ++i;
    }
    
    return RedisSendSyncImpl( idSV, m_redisMsgArgs );
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;
    m_redisHashSlot = CalculateRedisHashSlot( config.topicName );
    //m_readOffset = config.redisXReadDefaultOffset;

    m_redisXreadCount = m_config.customConfig.GetAttributeValueOrChildValueByName( "xreadCount" ).AsUInt32( m_redisXreadCount );
    m_redisXreadBlockTimeoutInMs = m_config.customConfig.GetAttributeValueOrChildValueByName( "xreadBlockTimeoutInMs" ).AsUInt32( m_redisXreadBlockTimeoutInMs );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::RedisSendSyncImpl( const sw::redis::StringView& msgId, const TRedisArgs& kvPairs )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext || GUCEF_NULL == m_redisPipeline )
        return false;

    static const sw::redis::StringView autoGenMsgId = sw::redis::StringView( "*", 1 );
    const sw::redis::StringView& msgIdToUse = m_config.redisXAddIgnoreMsgId ? autoGenMsgId : msgId;

    try
    {
        sw::redis::StringView cnSV( m_config.topicName.C_String(), m_config.topicName.ByteSize() );

        if ( m_config.redisXAddMaxLen >= 0 )
            m_redisPipeline->xadd( cnSV, msgIdToUse, kvPairs.begin(), kvPairs.end(), m_config.redisXAddMaxLen, m_config.redisXAddMaxLenIsApproximate );
        else
            m_redisPipeline->xadd( cnSV, msgIdToUse, kvPairs.begin(), kvPairs.end() );

        sw::redis::QueuedReplies redisReplies = m_redisPipeline->exec();

        bool totalSuccess = true;
        size_t replyCount = redisReplies.size();
        for ( size_t r=0; r<replyCount; ++r )
        {
            redisReply& reply = redisReplies.get( r );

            switch ( reply.integer )
            {
                case REDIS_OK:
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Successfully sent message with " +
                        CORE::ToString( kvPairs.size() ) + " fields. MsgID=" + CORE::ToString( reply.str ) );

                    ++m_redisMsgsTransmitted;
                    m_redisFieldsInMsgsTransmitted += (CORE::UInt32) kvPairs.size();
                    m_redisFieldsInMsgsRatio = (CORE::UInt32) kvPairs.size();
                    break;
                }
                default:
                case REDIS_ERR:
                {
                    totalSuccess = false;
                    ++m_redisErrorReplies;

                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Error sending message with " +
                        CORE::ToString( kvPairs.size() ) + " fields. Error=" + CORE::ToString( reply.str ) );
                    break;
                }
            }
        }

        return totalSuccess;
    }
    catch ( const sw::redis::MovedError& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ MovedError (Redirect failed?) . Current slot: " +
                                CORE::ToString( m_redisHashSlot ) + ", new slot: " + CORE::ToString( e.slot() ) + " at node " + e.node().host + ":" + CORE::ToString( e.node().port ) +
                                " exception: " + e.what() );
        Disconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const sw::redis::RedirectionError& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ RedirectionError (rebalance? node failure?). Current slot: " +
                                CORE::ToString( m_redisHashSlot ) + ", new slot: " + CORE::ToString( e.slot() ) + " at node " + e.node().host + ":" + CORE::ToString( e.node().port ) +
                                " exception: " + e.what() );

        Disconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const sw::redis::ReplyError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ Reply error exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ exception: " + e.what() );
        Disconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: exception: " + e.what() );
        return false;
    }
} 

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClientTopic::PrepStorageForReadMsgs( CORE::UInt32 msgCount )
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
CRedisClusterPubSubClientTopic::RedisRead( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    if ( GUCEF_NULL == m_redisContext || m_config.topicName.IsNULLOrEmpty() )
        return false;

    try
    {       
        sw::redis::StringView topicSV( m_config.topicName.C_String(), m_config.topicName.ByteSize()-1 );
        sw::redis::StringView readOffsetSV( m_readOffset.c_str(), m_readOffset.size() );
        std::chrono::milliseconds readBlockTimeout( m_redisXreadBlockTimeoutInMs );
        
        TRedisMsgByStreamInserter inserter = std::inserter( m_redisReadMsgs, m_redisReadMsgs.end() );
        
        if ( m_redisXreadCount > 0 )
            m_redisContext->xread< TRedisMsgByStreamInserter >( topicSV, readOffsetSV, readBlockTimeout, (long long) m_redisXreadCount, inserter );
        else
            m_redisContext->xread< TRedisMsgByStreamInserter >( topicSV, readOffsetSV, readBlockTimeout, inserter );

        // Do we have data from any streams?
        if ( !m_redisReadMsgs.empty() )
        {
            // Process messages for every stream
            TRedisMsgByStream::iterator s = m_redisReadMsgs.begin();
            while ( s != m_redisReadMsgs.end() )
            {
                // Get the actual msgs
                TRedisMsgVector& msgs = (*(m_redisReadMsgs.begin())).second;

                // Make sure we have enough storage to construct our generic representations
                PrepStorageForReadMsgs( (CORE::UInt32) msgs.size() );

                // Cycle through the messages received and build the generic representations
                TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs.GetData();
                TRedisMsgVector::iterator i = msgs.begin();
                while ( i != msgs.end() )
                {
                    std::string& msgId = (*i).first;                
                    TRedisMsgAttributes& msgAttribs = (*i).second;                                    

                    m_pubsubMsgs.push_back( COMCORE::CBasicPubSubMsg() );
                    COMCORE::CBasicPubSubMsg& pubsubMsg = m_pubsubMsgs.back();
                    msgRefs.push_back( TPubSubMsgRef() );
                    TPubSubMsgRef& pubsubMsgRef = msgRefs.back();
                    pubsubMsgRef.LinkTo( &pubsubMsg );

                    // set basic message properties
                    
                    pubsubMsg.GetMsgId().LinkTo( msgId );
                    CORE::UInt64 unixUtcDt = CORE::StringToUInt64( msgId );
                    pubsubMsg.GetMsgDateTime().FromUnixEpochBasedTicksInMillisecs( unixUtcDt );
                
                    // set the message attributes

                    if ( msgAttribs.size() > m_pubsubMsgAttribs.size() )
                        m_pubsubMsgAttribs.resize( msgAttribs.size() );
                    pubsubMsg.GetKeyValuePairs().clear();
                    pubsubMsg.GetKeyValuePairs().reserve( msgAttribs.size() );
                
                    TBufferVector::iterator a = m_pubsubMsgAttribs.begin();
                    TRedisMsgAttributes::iterator n = msgAttribs.begin();
                    while ( n != msgAttribs.end() )
                    {
                        std::string& keyAtt = (*n).first;
                        std::string& valueAtt = (*n).second;
                    
                        CORE::CDynamicBuffer& keyAttBuffer = (*a).first;
                        CORE::CDynamicBuffer& valueAttBuffer = (*a).second;

                        keyAttBuffer.LinkTo( keyAtt );
                        valueAttBuffer.LinkTo( valueAtt );
                    
                        pubsubMsg.GetKeyValuePairs().push_back( COMCORE::CBasicPubSubMsg::TKeyValuePair() );
                        COMCORE::CBasicPubSubMsg::TKeyValuePair& kvLink = pubsubMsg.GetKeyValuePairs().back();

                        kvLink.first.LinkTo( keyAttBuffer );
                        kvLink.second.LinkTo( valueAttBuffer );
                    
                        ++n; ++a;
                    }
                    ++i;
                }
                
                m_readOffset = (*msgs.rbegin()).first;

                ++s;
            }

            // Communicate all the messages received via an event notification
            if ( !NotifyObservers( MsgsRecievedEvent, &m_pubsubMsgsRefs ) ) return false;

            m_redisReadMsgs.clear();
            m_pubsubMsgsRefs.GetData().clear();
            m_pubsubMsgAttribs.clear();
        }
        return true;
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::MovedError& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ MovedError (Redirect failed?) . Current slot: " +
                                CORE::ToString( m_redisHashSlot ) + ", new slot: " + CORE::ToString( e.slot() ) + " at node " + e.node().host + ":" + CORE::ToString( e.node().port ) +
                                " exception: " + e.what() );
        Disconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const sw::redis::RedirectionError& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ RedirectionError (rebalance? node failure?). Current slot: " +
                                CORE::ToString( m_redisHashSlot ) + ", new slot: " + CORE::ToString( e.slot() ) + " at node " + e.node().host + ":" + CORE::ToString( e.node().port ) +
                                " exception: " + e.what() );

        Disconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Redis++ exception: " + e.what() );
        Disconnect();
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: exception: " + e.what() );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CRedisClusterPubSubClientTopic::CalculateRedisHashSlot( const CORE::CString& keyStr ) const
{GUCEF_TRACE;

    // The following code is copied from: https://redis.io/topics/cluster-spec

    // Minor changes were made to accept a string object and use platform types
    CORE::UInt32 keylen = keyStr.Length();
    const char* key = keyStr.C_String();

    CORE::UInt32 s, e; /* start-end indexes of { and } */

    /* Search the first occurrence of '{'. */
    for (s = 0; s < keylen; s++)
        if (key[s] == '{') break;

    /* No '{' ? Hash the whole key. This is the base case. */
    if ( s == keylen)
        return sw::redis::crc16( key,(int) keylen) & 16383;

    /* '{' found? Check if we have the corresponding '}'. */
    for (e = s+1; e < keylen; e++)
        if (key[e] == '}') break;

    /* No '}' or nothing between {} ? Hash the whole key. */
    if (e == keylen || e == s+1)
        return sw::redis::crc16(key,keylen) & 16383;

    /* If we are here there is both a { and a } on its right. Hash
     * what is in the middle between { and }. */
    return sw::redis::crc16(key+s+1,e-s-1) & 16383;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    try
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Disconnect: Beginning cleanup" );

        delete m_redisPipeline;
        m_redisPipeline = GUCEF_NULL;
        
        // the parent client owns the context, we just null it
        m_redisContext = GUCEF_NULL;

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Disconnect: Finished cleanup" );
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Disconnect: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Disconnect: Redis++ exception: " + e.what() );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Disconnect: exception: " + e.what() );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::SubscribeImpl( const std::string& readOffset )
{
    MT::CScopeMutex lock( m_lock );
    
    try
    {
        // We use blocking "long polling" reads which means we will need a dedicated thread to block until there is data
        // Redis does not support pushing of data directly

        if ( m_readerThread.IsNULL() )
        {
            m_readerThread = RedisClusterPubSubClientTopicReaderPtr( new CRedisClusterPubSubClientTopicReader( this ) );
        }
        if ( !m_readerThread.IsNULL() )
        {
            if ( !m_client->GetThreadPool()->StartTask( m_readerThread ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):SubscribeImpl: Failed to start blocking reader thread for async subscription" );
                return false;
            }
        }
        return true;
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):SubscribeImpl: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):SubscribeImpl: Redis++ exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):SubscribeImpl: exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    // Since Redis does not store the offset itself we will use whatever was last in-process which if this
    // is the first use means its the default starting offset
    MT::CScopeMutex lock( m_lock );
    return SubscribeImpl( m_readOffset );
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::SubscribeStartingAtBookmark( const COMCORE::CPubSubBookmark& bookmark ) 
{GUCEF_TRACE;

    // With Redis the default ID format is a Unix Epoch based datetime
    // For streaming Redis the msg ID and offset/index are the same thing
    MT::CScopeMutex lock( m_lock );
    return SubscribeImpl( bookmark.GetBookmarkData().AsString() );
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubBookmark 
CRedisClusterPubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return COMCORE::CPubSubBookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_TOPIC_INDEX, CORE::CVariant( m_readOffset ) );
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::AcknowledgeReceipt( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    // Does not apply to Redis wrt what this plugin supports
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::AcknowledgeReceipt( const COMCORE::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    // Does not apply to Redis wrt what this plugin supports
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return GUCEF_NULL != m_redisContext && ( !m_config.preferDedicatedConnection || GUCEF_NULL != m_redisPipeline );
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    Disconnect();
    
    MT::CScopeMutex lock( m_lock );
    try
    {
        m_redisContext = m_client->GetRedisContext();
        if ( GUCEF_NULL == m_redisContext )
            return false;
        
        // The following is not a must-have for connectivity
        const RedisNodeMap& nodeMap = m_client->GetRedisNodeMap();
        RedisNodeMap::const_iterator i = nodeMap.begin();
        while ( i != nodeMap.end() )
        {
            if ( (*i).first > m_redisHashSlot )
                break;
            if ( m_redisHashSlot >= (*i).first && m_redisHashSlot <= (*i).second.endSlot )
            {
                m_redisShardHost = (*i).second.host;
                m_redisShardNodeId = (*i).second.nodeId;
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Connect: Stream \"" + m_config.topicName +
                    "\" hashes to hash slot " + CORE::ToString( m_redisHashSlot ) + " which lives at " + (*i).second.host.HostnameAndPortAsString() + " with node id " + (*i).second.nodeId );
                break;
            }
            ++i;
        }

        if ( m_config.preferDedicatedConnection )
        {        
            // Connect to the specific shard used for this channel's stream with a single dedicated connection
            sw::redis::StringView cnSV( m_config.topicName.C_String(), m_config.topicName.Length() );
            delete m_redisPipeline;
            m_redisPipeline = new sw::redis::Pipeline( m_redisContext->pipeline( cnSV ) );

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Connect: Successfully created a Redis pipeline. Hash Slot " + CORE::ToString( m_redisHashSlot ) );
        }

        return true;
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Connect: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Connect: Redis++ exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Connect: exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                                     const CORE::CEvent& eventId  ,
                                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClientTopic::OnRedisReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                                            const CORE::CEvent& eventId  ,
                                                            CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( InitializeConnectivity() )
    {
        m_redisReconnectTimer->SetEnabled( false );
    }
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CRedisClusterPubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CRedisClusterPubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace REDISCLUSTER */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
