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

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#include "pubsubpluginREDISCLUSTER_CRedisClusterPubSubClient.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <winsock2.h>
#endif

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CRedisClusterPubSubClient::TypeName = "ClusteredRedis";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

RedisNode::RedisNode( void )
    : host()
    , nodeId()
    , startSlot( 0 )
    , endSlot( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisPubSubMsg::CRedisPubSubMsg( void )
    : COMCORE::CIPubSubMsg()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisPubSubMsg::CRedisPubSubMsg( const CRedisPubSubMsg& src )
    : COMCORE::CIPubSubMsg( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisPubSubMsg::~CRedisPubSubMsg()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CVariant& 
CRedisPubSubMsg::GetMsgId( void ) const
{GUCEF_TRACE;

    return m_msgId;
}

/*-------------------------------------------------------------------------*/

const CORE::CDateTime& 
CRedisPubSubMsg::GetMsgDateTime( void ) const
{GUCEF_TRACE;

    return m_msgDateTime;
}

/*-------------------------------------------------------------------------*/

const CORE::TLinkedCloneableBuffer& 
CRedisPubSubMsg::GetPrimaryPayload( void ) const
{GUCEF_TRACE;

    return m_primaryPayloadLink;
}

/*-------------------------------------------------------------------------*/

const CRedisPubSubMsg::TKeyValuePayloadLinks& 
CRedisPubSubMsg::GetKeyValuePairs( void ) const
{GUCEF_TRACE;

    return m_keyValueLinks;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CRedisPubSubMsg::Clone( void ) const
{GUCEF_TRACE;

    return new CRedisPubSubMsg( *this );
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicConfig::CRedisClusterPubSubClientTopicConfig( void )
    : COMCORE::CPubSubClientTopicConfig()
    , redisXAddMaxLen( -1 )
    , redisXAddMaxLenIsApproximate( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicConfig::CRedisClusterPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig )
    : COMCORE::CPubSubClientTopicConfig( genericConfig )
    , redisXAddMaxLen( -1 )
    , redisXAddMaxLenIsApproximate( true )
{GUCEF_TRACE;

    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicConfig::~CRedisClusterPubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    redisXAddMaxLen = config.GetAttributeValueOrChildValueByName( "redisXAddMaxLen" ).AsInt32( redisXAddMaxLen ); 
    redisXAddMaxLenIsApproximate = config.GetAttributeValueOrChildValueByName( "redisXAddMaxLenIsApproximate" ).AsBool( redisXAddMaxLenIsApproximate );
    return true;
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicConfig& 
CRedisClusterPubSubClientTopicConfig::operator=( const COMCORE::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );
        LoadCustomConfig( src.customConfig );    
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicConfig& 
CRedisClusterPubSubClientTopicConfig::operator=( const CRedisClusterPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );
        redisXAddMaxLen = src.redisXAddMaxLen; 
        redisXAddMaxLenIsApproximate = src.redisXAddMaxLenIsApproximate;   
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicReader::CRedisClusterPubSubClientTopicReader( CRedisClusterPubSubClientTopic* ownerTopic )
    : CORE::CTaskConsumer()
    , m_ownerTopic( ownerTopic )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicReader::~CRedisClusterPubSubClientTopicReader()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopicReader::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopicReader::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    m_ownerTopic->RedisRead();
    return false;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClientTopicReader::OnTaskEnding( CORE::CICloneable* taskdata ,
                                                    bool willBeForced           )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClientTopicReader::OnTaskEnded( CORE::CICloneable* taskdata ,
                                                   bool wasForced              )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CString 
CRedisClusterPubSubClientTopicReader::GetType( void ) const
{GUCEF_TRACE;

    static const CORE::CString taskTypeName = "RedisClusterPubSubClientTopicReader";
    return taskTypeName;
}

/*-------------------------------------------------------------------------*/

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
    , m_readOffset( "$" )
    , m_readerThread()
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

    if ( client->GetConfig().desiredFeatures.supportsSubscribing )
    {
        m_readerThread = RedisClusterPubSubClientTopicReaderPtr( new CRedisClusterPubSubClientTopicReader( this ) );
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

    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::Publish( const CORE::CString& msgKey    ,  
                                         const CORE::CString& fieldName , 
                                         const CORE::CString& payload   )
{GUCEF_TRACE;

    sw::redis::StringView idSV( msgKey.C_String(), msgKey.ByteSize() );
    
    m_redisMsgArgs.clear();
    m_redisMsgArgs.reserve( 1 );

    sw::redis::StringView fnSV( fieldName.C_String(), fieldName.ByteSize() );
    sw::redis::StringView fvSV( (const char*) payload.C_String(), payload.ByteSize() );
    
    m_redisMsgArgs.push_back( std::pair< sw::redis::StringView, sw::redis::StringView >( fnSV, fvSV ) );
    
    return RedisSendSyncImpl( idSV );
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::Publish( const CORE::CString& msgKey         , 
                                         const CORE::CString& fieldName      , 
                                         const CORE::CDynamicBuffer& payload )
{GUCEF_TRACE;

    sw::redis::StringView idSV( msgKey.C_String(), msgKey.ByteSize() );
    
    m_redisMsgArgs.clear();
    m_redisMsgArgs.reserve( 1 );

    sw::redis::StringView fnSV( fieldName.C_String(), fieldName.ByteSize() );
    sw::redis::StringView fvSV( (const char*) payload.GetConstBufferPtr(), payload.GetDataSize() );
    
    m_redisMsgArgs.push_back( std::pair< sw::redis::StringView, sw::redis::StringView >( fnSV, fvSV ) );
    
    return RedisSendSyncImpl( idSV );
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::Publish( const CORE::CString& msgKey     , 
                                         const CORE::CValueList& payload )
{GUCEF_TRACE;

    sw::redis::StringView idSV( msgKey.C_String(), msgKey.ByteSize() );
    
    m_redisMsgArgs.clear();
    m_redisMsgArgs.reserve( (size_t) payload.GetCount() );

    CORE::CValueList::TValueMap::const_iterator i = payload.GetDataBeginIterator();
    while ( i != payload.GetDataEndIterator() )
    {
        const CORE::CString& fieldName = (*i).first;
        sw::redis::StringView fnSV( fieldName.C_String(), fieldName.ByteSize() );
        
        const CORE::CValueList::TStringVector& fieldValues = (*i).second;
        CORE::CValueList::TStringVector::const_iterator n = fieldValues.begin();
        while ( n != fieldValues.end() )
        {
            sw::redis::StringView fvSV( (const char*) (*n).C_String(), (*n).ByteSize() );
            m_redisMsgArgs.push_back( std::pair< sw::redis::StringView, sw::redis::StringView >( fnSV, fvSV ) );
            ++n;
        }
        ++i;
    }
    
    return RedisSendSyncImpl( idSV );
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    m_config = config;
    m_redisHashSlot = CalculateRedisHashSlot( config.topicName );

    m_redisXreadCount = m_config.customConfig.GetAttributeValueOrChildValueByName( "xreadCount" ).AsUInt32( m_redisXreadCount );
    m_redisXreadBlockTimeoutInMs = m_config.customConfig.GetAttributeValueOrChildValueByName( "xreadBlockTimeoutInMs" ).AsUInt32( m_redisXreadBlockTimeoutInMs );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::RedisSendSyncImpl( const sw::redis::StringView& msgId )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext || GUCEF_NULL == m_redisPipeline )
        return false;

    try
    {
        sw::redis::StringView cnSV( m_config.topicName.C_String(), m_config.topicName.ByteSize() );

        if ( m_config.redisXAddMaxLen >= 0 )
            m_redisPipeline->xadd( cnSV, msgId, m_redisMsgArgs.begin(), m_redisMsgArgs.end(), m_config.redisXAddMaxLen, m_config.redisXAddMaxLenIsApproximate );
        else
            m_redisPipeline->xadd( cnSV, msgId, m_redisMsgArgs.begin(), m_redisMsgArgs.end() );

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
                        CORE::ToString( m_redisMsgArgs.size() ) + " fields. MsgID=" + CORE::ToString( reply.str ) );

                    ++m_redisMsgsTransmitted;
                    m_redisFieldsInMsgsTransmitted += (CORE::UInt32) m_redisMsgArgs.size();
                    m_redisFieldsInMsgsRatio = (CORE::UInt32) m_redisMsgArgs.size();
                    break;
                }
                default:
                case REDIS_ERR:
                {
                    totalSuccess = false;
                    ++m_redisErrorReplies;

                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):RedisSendSyncImpl: Error sending message with " +
                        CORE::ToString( m_redisMsgArgs.size() ) + " fields. Error=" + CORE::ToString( reply.str ) );
                    break;
                }
            }
        }

        return totalSuccess;
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

                    m_pubsubMsgs.push_back( CRedisPubSubMsg() );
                    CRedisPubSubMsg& pubsubMsg = m_pubsubMsgs.back();
                    msgRefs.push_back( TPubSubMsgRef() );
                    TPubSubMsgRef& pubsubMsgRef = msgRefs.back();
                    pubsubMsgRef.LinkTo( &pubsubMsg );

                    // set basic message properties
                    
                    pubsubMsg.m_msgId.LinkTo( msgId );
                    CORE::UInt64 unixUtcDt = CORE::StringToUInt64( msgId );
                    pubsubMsg.m_msgDateTime.SetFromUnixEpochBasedTickInMillisecs( unixUtcDt );
                
                    // set the message attributes

                    if ( msgAttribs.size() > m_pubsubMsgAttribs.size() )
                        m_pubsubMsgAttribs.resize( msgAttribs.size() );
                    pubsubMsg.m_keyValueLinks.clear();
                    pubsubMsg.m_keyValueLinks.reserve( msgAttribs.size() );
                
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
                    
                        pubsubMsg.m_keyValueLinks.push_back( CRedisPubSubMsg::TKeyValueLinkPair() );
                        CRedisPubSubMsg::TKeyValueLinkPair& kvLink = pubsubMsg.m_keyValueLinks.back();

                        kvLink.first.LinkTo( &keyAttBuffer );
                        kvLink.second.LinkTo( &valueAttBuffer );
                    
                        ++n; ++a;
                    }

                    m_readOffset = msgId;
                    ++i;
                }
                                
                ++s;
            }

            // Communicate all the messages received via an event notification
            if ( !NotifyObservers( MsgsRecievedEvent, &m_pubsubMsgsRefs ) ) return false;
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

    int s, e; /* start-end indexes of { and } */

    /* Search the first occurrence of '{'. */
    for (s = 0; s < keylen; s++)
        if (key[s] == '{') break;

    /* No '{' ? Hash the whole key. This is the base case. */
    if ( s == keylen)
        return sw::redis::crc16(key,keylen) & 16383;

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
CRedisClusterPubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    return GUCEF_NULL != m_redisContext && ( !m_config.preferDedicatedConnection || GUCEF_NULL != m_redisPipeline );
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopic::Connect( void )
{GUCEF_TRACE;

    Disconnect();
    
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

        if ( m_config.needSubscribeSupport && !m_readerThread.IsNULL() )
        {
            // We use long polling reads which means we will need a dedicated thread to block until there is data
            if ( !m_client->GetThreadPool()->StartTask( m_readerThread ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClientTopic(" + CORE::PointerToString( this ) + "):Connect: Failed to start blocking reader thread for async subscription" );
                return false;
            }
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

    if ( Connect() )
    {
        m_redisReconnectTimer->SetEnabled( false );
    }
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClient::CRedisClusterPubSubClient( const COMCORE::CPubSubClientConfig& config )
    : COMCORE::CPubSubClient()
    , m_config( config )
    , m_nodeMap()
    , m_redisContext( GUCEF_NULL )
    , m_redisErrorReplies( 0 )
    , m_metricsTimer( GUCEF_NULL )
    , m_redisReconnectTimer( GUCEF_NULL )
    , m_topicMap()
    , m_threadPool()
{GUCEF_TRACE;

    if ( GUCEF_NULL != config.pulseGenerator )
    {
        if ( config.desiredFeatures.supportsMetrics )
        {
            m_metricsTimer = new CORE::CTimer( *config.pulseGenerator, 1000 );
            m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
        }
        if ( config.desiredFeatures.supportsAutoReconnect )
        {
            m_redisReconnectTimer = new CORE::CTimer( *config.pulseGenerator, config.reconnectDelayInMs );
        }
    }
    else
    {
        if ( config.desiredFeatures.supportsMetrics )
        {
            m_metricsTimer = new CORE::CTimer( 1000 );        
            m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
        }
        if ( config.desiredFeatures.supportsAutoReconnect )
        {
            m_redisReconnectTimer = new CORE::CTimer( config.reconnectDelayInMs );
        }
    }
    
    if ( config.desiredFeatures.supportsSubscribing )
        m_threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetOrCreateThreadPool( "RedisClusterPubSubClient(" + CORE::ToString( this ) + ")", true );

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClient::~CRedisClusterPubSubClient()
{GUCEF_TRACE;

    if ( !m_threadPool.IsNULL() )
        m_threadPool->RequestAllThreadsToStop( true, false );
    
    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    delete m_redisReconnectTimer;
    m_redisReconnectTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientConfig& 
CRedisClusterPubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

CORE::ThreadPoolPtr 
CRedisClusterPubSubClient::GetThreadPool( void )
{GUCEF_TRACE;

    return m_threadPool;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::GetSupportedFeatures( COMCORE::CPubSubClientFeatures& features )
{GUCEF_TRACE;

    features.supportsBinaryPayloads = true;
    features.supportsDuplicateKeysPerMsg = true;
    features.supportsKeyValuePerMsg = true;
    features.supportsMsgKeysPerTopic = true;
    features.supportsMultiHostSharding = true;
    features.supportsPublishing = true;
    features.supportsSubscribing = true;
    features.supportsMetrics = true;
    features.supportsAutoReconnect = true;
    return true;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic*
CRedisClusterPubSubClient::CreateTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    CRedisClusterPubSubClientTopic* rcTopic = new CRedisClusterPubSubClientTopic( this );
    if ( rcTopic->LoadConfig( topicConfig ) )
    {
        m_topicMap[ topicConfig.topicName ] = rcTopic;
    }
    return rcTopic;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic* 
CRedisClusterPubSubClient::GetTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        return (*i).second;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        delete (*i).second;
        m_topicMap.erase( i );
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CRedisClusterPubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClient::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClient::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

const RedisNodeMap& 
CRedisClusterPubSubClient::GetRedisNodeMap( void ) const
{GUCEF_TRACE;

    return m_nodeMap;
}

/*-------------------------------------------------------------------------*/

sw::redis::RedisCluster* 
CRedisClusterPubSubClient::GetRedisContext( void ) const
{GUCEF_TRACE;

    return m_redisContext;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::GetRedisClusterNodeMap( RedisNodeMap& nodeMap )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_redisContext )
        return false;

    try
    {
        CORE::CString clusterCmd( "CLUSTER" );
        CORE::CString slotsParam( "SLOTS" );

        sw::redis::StringView clusterCmdSV( clusterCmd.C_String(), clusterCmd.Length() );
        sw::redis::StringView slotsParamSV( slotsParam.C_String(), slotsParam.Length() );

        auto reply = m_redisContext->command( clusterCmdSV, slotsParamSV );
        if ( reply )
        {
            int type = reply->type;
            if ( REDIS_REPLY_ARRAY == type )
            {
                size_t max = reply->elements;
                for ( size_t i=0; i<max; ++i )
                {
                    const struct redisReply* e = reply->element[ i ];
                    int eType = e->type;
                    if ( REDIS_REPLY_ARRAY == eType )
                    {
                        size_t eMax = e->elements;

                        long long startSlot = -1;
                        if ( eMax > 0 && REDIS_REPLY_INTEGER == e->element[ 0 ]->type )
                            startSlot = e->element[ 0 ]->integer;
                        else
                            return false;

                        long long endSlot = -1;
                        if ( eMax > 1 && REDIS_REPLY_INTEGER == e->element[ 1 ]->type )
                            endSlot = e->element[ 1 ]->integer;
                        else
                            return false;

                        for ( size_t m=2; m<eMax; ++m )
                        {
                            const struct redisReply* slotSegment = e->element[ m ];
                            int sType = slotSegment->type;
                            if ( REDIS_REPLY_ARRAY == sType )
                            {
                                size_t slotSegAttCount = slotSegment->elements;

                                CORE::CString ip;
                                if ( slotSegAttCount > 0 && REDIS_REPLY_STRING == slotSegment->element[ 0 ]->type )
                                    ip = slotSegment->element[ 0 ]->str;
                                else
                                    return false;

                                long long port = -1;
                                if ( slotSegAttCount > 1 && REDIS_REPLY_INTEGER == slotSegment->element[ 1 ]->type )
                                    port = slotSegment->element[ 1 ]->integer;
                                else
                                    return false;

                                // Read the optional Node ID if present. This depends on the Redis version
                                CORE::CString nodeId;
                                if ( slotSegAttCount > 2 && REDIS_REPLY_STRING == slotSegment->element[ 2 ]->type )
                                    nodeId = slotSegment->element[ 2 ]->str;

                                RedisNode& node = nodeMap[ (CORE::UInt32) startSlot ];
                                node.startSlot = (CORE::UInt32) startSlot;
                                node.endSlot = (CORE::UInt32) endSlot;
                                node.nodeId = nodeId;
                                node.host.SetHostnameAndPort( ip, port );
                            }
                        }
                    }
                }
            }
            else
            {
                if ( REDIS_REPLY_ERROR == type )
                {
                    ++m_redisErrorReplies;
                    return false;
                }
            }
        }

    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: Redis++ exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):GetRedisClusterNodeMap: exception: " + e.what() );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::Disconnect( void )
{GUCEF_TRACE;

    try
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisDisconnect: Beginning cleanup" );
        
        TTopicMap::iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            (*i).second->Disconnect();
            ++i;
        }

        delete m_redisContext;
        m_redisContext = GUCEF_NULL;

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisDisconnect: Finished cleanup" );
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisDisconnect: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisDisconnect: Redis++ exception: " + e.what() );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisDisconnect: exception: " + e.what() );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::Connect( void )
{GUCEF_TRACE;

    Disconnect();
    
    try
    {
        sw::redis::ConnectionOptions rppConnectionOptions;
        rppConnectionOptions.host = m_config.remoteAddress.GetHostname();  // Required.
        rppConnectionOptions.port = m_config.remoteAddress.GetPortInHostByteOrder(); // Optional. The default port is 6379.
        //rppConnectionOptions.password = "auth";   // Optional. No password by default.
        //rppConnectionOptions.db = 1;  // Optional. Use the 0th database by default.

        // Optional. Timeout before we successfully send request to or receive response from redis.
        // By default, the timeout is 0ms, i.e. never timeout and block until we send or receive successfuly.
        // NOTE: if any command is timed out, we throw a TimeoutError exception.
        rppConnectionOptions.socket_timeout = std::chrono::milliseconds( 100 );
        rppConnectionOptions.connect_timeout = std::chrono::milliseconds( 100 );

        rppConnectionOptions.keep_alive = true;

        // Connect to the Redis cluster
        delete m_redisContext;
        m_redisContext = new sw::redis::RedisCluster( rppConnectionOptions );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisConnect: Successfully created a Redis context" );

        // The following is not a must-have for connectivity
        if ( GetRedisClusterNodeMap( m_nodeMap ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisConnect: Successfully obtained Redis cluster nodes" );
        }

        TTopicMap::iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            (*i).second->Connect();
            ++i;
        }

        return true;
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisConnect: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisConnect: Redis++ exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):RedisConnect: exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::IsConnected( void )
{GUCEF_TRACE;

    return GUCEF_NULL != m_redisContext;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_metricsTimer )
    {
        TEventCallback callback( this, &CRedisClusterPubSubClient::OnMetricsTimerCycle );
        SubscribeTo( m_metricsTimer                 ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
    if ( GUCEF_NULL != m_redisReconnectTimer )
    {
        TEventCallback callback( this, &CRedisClusterPubSubClient::OnRedisReconnectTimerCycle );
        SubscribeTo( m_redisReconnectTimer          ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        (*i).second->OnMetricsTimerCycle( notifier  ,
                                          eventId   ,
                                          eventData );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::OnRedisReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                                       const CORE::CEvent& eventId  ,
                                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( Connect() )
    {
        m_redisReconnectTimer->SetEnabled( false );
    }
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
