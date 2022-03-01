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

#include "pubsubpluginREDISCLUSTER_CRedisClusterPubSubClient.h"

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

CRedisClusterPubSubClient::CRedisClusterPubSubClient( const COMCORE::CPubSubClientConfig& config )
    : COMCORE::CPubSubClient()
    , m_config( config )
    , m_nodeMap()
    , m_redisContext( GUCEF_NULL )
    , m_redisErrorReplies( 0 )
    , m_metricsTimer( GUCEF_NULL )
    , m_redisReconnectTimer( GUCEF_NULL )
    , m_streamIndexingTimer( GUCEF_NULL )
    , m_topicMap()
    , m_threadPool()
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_config.pulseGenerator )
        m_config.pulseGenerator = &CORE::CCoreGlobal::Instance()->GetPulseGenerator();    

    if ( m_config.desiredFeatures.supportsMetrics )
    {
        m_metricsTimer = new CORE::CTimer( *config.pulseGenerator, 1000 );
        m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
    }
    if ( m_config.desiredFeatures.supportsAutoReconnect )
    {
        m_redisReconnectTimer = new CORE::CTimer( *config.pulseGenerator, config.reconnectDelayInMs );
    }
    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames )
    {                                                                      // @TODO: interval
        m_streamIndexingTimer = new CORE::CTimer( *config.pulseGenerator, 100000 );
    }

    m_config.metricsPrefix += "credis.";

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

    delete m_streamIndexingTimer;
    m_streamIndexingTimer = GUCEF_NULL;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        delete (*i).second;
        (*i).second = GUCEF_NULL;
        ++i;
    }
    m_topicMap.clear();
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

    features.supportsBinaryPayloads = true;             // Redis strings are binary safe so yes redis natively supports binary data
    features.supportsPerMsgIds = true;                  // the auto-generated id 
    features.supportsMsgIndex = true;                   // the auto-generated id we get acts as a timestamp index into the stream
    features.supportsPrimaryPayloadPerMsg = false;      // We can fake this best effort but not natively supported
    features.supportsAbsentPrimaryPayloadPerMsg = true; // A primary payload concept is not supported to begin with
    features.supportsKeyValueSetPerMsg = true;          // This is the native Redis way of communicating message data
    features.supportsDuplicateKeysPerMsg = true;        // Redis does not care about duplicate keys, they are just "fields"
    features.supportsMultiHostSharding = true;          // Redis doesnt support this but clustered Redis does which is what this plugin supports
    features.supportsPublishing = true;                 // We support being a Redis producer in this plugin
    features.supportsSubscribing = true;                // We support being a Redis consumer in this plugin
    features.supportsMetrics = true;                    // this plugin adds metrics support
    features.supportsAutoReconnect = true;              // Our plugin adds auto reconnect out of the box
    features.supportsSubscriberMsgReceivedAck = false;  // since offsets are managed client-side there really is no such concept but could be implemented as a plugin specific add-on - todo?
    features.supportsAutoMsgReceivedAck = false;        // not supported right now
    features.supportsAbsentMsgReceivedAck = true;       // no such inherent concept
    features.supportsAckUsingLastMsgInBatch = false;    // not supported right now
    features.supportsAckUsingBookmark = false;          // not supported right now
    features.supportsBookmarkingConcept = true;         // Redis does not support this server-side but does support it via passing your "bookmark" back to Redis as an offset
    features.supportsServerSideBookmarkPersistance = false; // no such support
    features.supportsSubscribingUsingBookmark = true;   // supported via giving Redis the starting offset
    features.supportsAutoBookmarking = false;           // Redis does not support this concept. The client needs to take care of remembering the offset
    features.supportsMsgIdBasedBookmark = true;         // This is the native Redis "bookmark" method and thus preferered
    features.supportsMsgIndexBasedBookmark = true;      // Same as supportsMsgIdBasedBookmark. This is the native Redis "bookmark" method and thus preferered
    features.supportsTopicIndexBasedBookmark = true;    // Same as supportsMsgIdBasedBookmark. This is the native Redis "bookmark" method and thus preferered
    features.supportsMsgDateTimeBasedBookmark = true;   // The auto-generated msgId is a timestamp so its essentially the same thing for Redis
    features.supportsDiscoveryOfAvailableTopics = true; // we support scanning for available Redis streams
    features.supportsGlobPatternTopicNames = true;      // we support glob pattern matching the scan of available Redis streams
    return true;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopic*
CRedisClusterPubSubClient::CreateTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    // Check to see if this logical/conceptual 'topic' represents multiple pattern matched Redis Streams
    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames && 
         topicConfig.topicName.HasChar( '*' ) > -1               )
    {
        PubSubClientTopicSet allTopicAccess;
        if ( CreateMultiTopicAccess( topicConfig, allTopicAccess ) && !allTopicAccess.empty() )
        {
            // Caller should really use the CreateMultiTopicAccess() variant
            COMCORE::CPubSubClientTopic* tAccess = *(allTopicAccess.begin());
            return tAccess;
        }
        return GUCEF_NULL;
    }
    else
    {
        CRedisClusterPubSubClientTopic* topicAccess = GUCEF_NULL;
        {
            MT::CObjectScopeLock lock( this );

            topicAccess = new CRedisClusterPubSubClientTopic( this );
            if ( topicAccess->LoadConfig( topicConfig ) )
            {
                m_topicMap[ topicConfig.topicName ] = topicAccess;

                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):CreateTopicAccess: created topic access for topic \"" + topicConfig.topicName + "\"" );

                TopicAccessCreatedEventData eData( topicConfig.topicName );
                NotifyObservers( TopicAccessCreatedEvent, &eData );
            }
            else
            {
                delete topicAccess;
                topicAccess = GUCEF_NULL;
            }
        }
    }
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

bool
CRedisClusterPubSubClient::GetMultiTopicAccess( const CORE::CString& topicName    ,
                                                PubSubClientTopicSet& topicAccess )
{GUCEF_TRACE;

    // Check to see if this logical/conceptual 'topic' name represents multiple pattern matched Redis Streams
    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames && 
         topicName.HasChar( '*' ) > -1                           )
    {
        // We create the actual topic objects from the wildcard glob pattern topic which is used
        // as a template. As such we need to match the pattern again to find the various topics that could have been spawned
        bool matchesFound = false;
        TTopicMap::iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            if ( (*i).first.WildcardEquals( topicName, '*', true ) )
            {
                topicAccess.insert( (*i).second );
                matchesFound = true;
            }
            ++i;
        }
        return matchesFound;
    }
    else
    {
        TTopicMap::iterator i = m_topicMap.find( topicName );
        if ( i != m_topicMap.end() )
        {
            topicAccess.insert( (*i).second );
            return true;
        }
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::AutoCreateMultiTopicAccess( const COMCORE::CPubSubClientTopicConfig& templateTopicConfig ,
                                                       const CORE::CString::StringSet& topicNameList                ,
                                                       PubSubClientTopicSet& topicAccess                            )
{GUCEF_TRACE;

    bool totalSuccess = true;
    CORE::CString::StringSet::const_iterator i = topicNameList.begin();
    while ( i != topicNameList.end() )
    {        
        COMCORE::CPubSubClientTopicConfig topicConfig( templateTopicConfig );
        topicConfig.topicName = (*i);

        CRedisClusterPubSubClientTopic* tAccess = GUCEF_NULL;
        {
            MT::CObjectScopeLock lock( this );

            tAccess = new CRedisClusterPubSubClientTopic( this );
            if ( tAccess->LoadConfig( topicConfig ) )
            {
                m_topicMap[ topicConfig.topicName ] = tAccess;
                topicAccess.insert( tAccess );

                TopicAccessCreatedEventData eData( topicConfig.topicName );
                NotifyObservers( TopicAccessAutoCreatedEvent, &eData );
            }
            else
            {
                delete tAccess;
                tAccess = GUCEF_NULL;
                totalSuccess = false;
            }
        }
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::CreateMultiTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig ,
                                                   PubSubClientTopicSet& topicAccess                    )
{GUCEF_TRACE;

    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames && 
         topicConfig.topicName.HasChar( '*' ) > -1               )
    {
        CORE::CString::StringSet topicNameList;
        if ( CRedisClusterKeyCache::Instance()->GetRedisKeys( m_redisContext, topicNameList, "streams", topicConfig.topicName ) )
        {
            m_streamIndexingTimer->SetEnabled( true );

            return AutoCreateMultiTopicAccess( topicConfig, topicNameList, topicAccess );
        }
        return false;
    }
    else
    {
        COMCORE::CPubSubClientTopic* tAccess = CreateTopicAccess( topicConfig );
        if ( GUCEF_NULL != tAccess )
        {
            topicAccess.insert( tAccess );
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::DestroyTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        CRedisClusterPubSubClientTopic* topicAccess = (*i).second;
        m_topicMap.erase( i );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):DestroyTopicAccess: destroyed topic access for topic \"" + topicName + "\"" );

        TopicAccessDestroyedEventData eData( topicName );
        NotifyObservers( TopicAccessDestroyedEvent, &eData );

        delete topicAccess;        
    }
}

/*-------------------------------------------------------------------------*/

const COMCORE::CPubSubClientTopicConfig* 
CRedisClusterPubSubClient::GetTopicConfig( const CORE::CString& topicName )
{GUCEF_TRACE;

    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        if ( topicName == (*i).topicName )
        {
            return &(*i);
        }
        ++i;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterPubSubClient::GetAvailableTopicNameList( CORE::CString::StringSet& topicNameList            ,
                                                      const CORE::CString::StringSet& globPatternFilters )
{GUCEF_TRACE;

    return CRedisClusterKeyCache::Instance()->GetRedisKeys( m_redisContext, topicNameList, "stream", globPatternFilters );
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        topicNameList.insert( (*i).topicName );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::GetCreatedTopicAccessNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        topicNameList.insert( (*i).first );
        ++i;
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

RedisClusterPtr
CRedisClusterPubSubClient::GetRedisContext( void ) const
{GUCEF_TRACE;

    return m_redisContext;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CRedisClusterPubSubClient::GetRedisClusterErrorRepliesCounter( bool resetCounter )
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
                                node.host.SetHostnameAndPort( ip, (CORE::UInt16) port );
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

        m_redisContext.Unlink();

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
        if ( m_config.remoteAddresses.empty() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):Connect: No remote address was specified" );
            return false;
        }
        
        sw::redis::ConnectionOptions rppConnectionOptions;
        rppConnectionOptions.host = m_config.remoteAddresses.front().GetHostname();  // Required.
        rppConnectionOptions.port = m_config.remoteAddresses.front().GetPortInHostByteOrder(); // Optional. The default port is 6379.
        //rppConnectionOptions.password = "auth";   // Optional. No password by default.
        //rppConnectionOptions.db = 1;  // Optional. Use the 0th database by default.

        // Optional. Timeout before we successfully send request to or receive response from redis.
        // By default, the timeout is 0ms, i.e. never timeout and block until we send or receive successfuly.
        // NOTE: if any command is timed out, we throw a TimeoutError exception.
        rppConnectionOptions.socket_timeout = std::chrono::milliseconds( 100 );
        rppConnectionOptions.connect_timeout = std::chrono::milliseconds( 100 );

        rppConnectionOptions.keep_alive = true;

        // Connect to the Redis cluster
        m_redisContext.Unlink();
        m_redisContext = RedisClusterPtr( new sw::redis::RedisCluster( rppConnectionOptions ) );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):Connect: Successfully created a Redis context" );

        // The following is not a must-have for connectivity
        if ( GetRedisClusterNodeMap( m_nodeMap ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):Connect: Successfully obtained Redis cluster nodes" );
        }

        TTopicMap::iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            (*i).second->InitializeConnectivity();
            ++i;
        }

        return true;
    }
    catch ( const sw::redis::OomError& e )
    {
		++m_redisErrorReplies;
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):Connect: Redis++ OOM exception: " + e.what() );
        return false;
    }
    catch ( const sw::redis::Error& e )
    {
		++m_redisErrorReplies;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):Connect: Redis++ exception: " + e.what() );
        m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):Connect: exception: " + e.what() );
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
    if ( GUCEF_NULL != m_streamIndexingTimer )
    {
        TEventCallback callback( this, &CRedisClusterPubSubClient::OnStreamIndexingTimerCycle );
        SubscribeTo( m_streamIndexingTimer          ,
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


    // Quickly grab a snapshot of metric values for all topics 
    // we don't combine this with metrics publishing as it adds to metrics timeframe drift across topics
    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        (*i).second->OnMetricsTimerCycle( notifier  ,
                                          eventId   ,
                                          eventData );
        ++i;
    }

    // Now publish all the metrics to the metrics system
    i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        CRedisClusterPubSubClientTopic* topic = (*i).second;
        const CRedisClusterPubSubClientTopic::TopicMetrics& topicMetrics = topic->GetMetrics();
        const CORE::CString& topicName = topic->GetMetricFriendlyTopicName();
        //const CRedisClusterPubSubClientTopicConfig& topicConfig = topic->GetTopicConfig();
        CORE::CString metricsPrefix = m_config.metricsPrefix + topicName;
        
        GUCEF_METRIC_COUNT( m_config.metricsPrefix + topicName + ".redisErrorReplies", topicMetrics.redisClusterErrorReplies, 1.0f );
        
        ++i;
    }

    GUCEF_METRIC_COUNT( m_config.metricsPrefix + ".redisErrorReplies", GetRedisClusterErrorRepliesCounter( true ), 1.0f );
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

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::GetAllGlobPatternTopicNames( CORE::CString::StringSet& allGlobPatternTopicNames )
{GUCEF_TRACE;

    // Check config'd topic
    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        if ( (*i).topicName.HasChar( '*' ) > -1 )
        {
            allGlobPatternTopicNames.insert( (*i).topicName );
        }
        ++i;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::IsStreamIndexingNeeded( void )
{GUCEF_TRACE;

    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames )
    {
        // Check config'd topic
        COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
        while ( i != m_config.topics.end() )
        {
            if ( (*i).topicName.HasChar( '*' ) > -1 )
            {
                return true;
            }
            ++i;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const COMCORE::CPubSubClientTopicConfig*
CRedisClusterPubSubClient::FindTemplateConfigForTopicName( const CORE::CString& topicName ) const
{GUCEF_TRACE;

    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::const_iterator i = m_config.topics.begin();
    while ( i != m_config.topics.end() )
    {
        if ( (*i).topicName.HasChar( '*' ) > -1 )
        {
            if ( topicName.WildcardEquals( (*i).topicName, '*', true ) )
            {
                return &(*i);
            }
        }
        ++i;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::OnStreamIndexingTimerCycle( CORE::CNotifier* notifier    ,
                                                       const CORE::CEvent& eventId  ,
                                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):OnStreamIndexingTimerCycle: Checking for new streams matching glob pattterns" );
    
    // First get a new list of all glob patterns we need to match
    CORE::CString::StringSet allGlobPatternTopicNames;
    if( !GetAllGlobPatternTopicNames( allGlobPatternTopicNames ) )
        return;
    
    // Next we will need to fetch all stream names in the cluster
    CORE::CString::StringSet allMatchingStreamNames;
    if ( CRedisClusterKeyCache::Instance()->GetRedisKeys( m_redisContext, allMatchingStreamNames, "stream", allGlobPatternTopicNames ) )
    {
        // Check created topics, filtering the ones we already created
        TTopicMap::iterator t = m_topicMap.begin();
        while ( t != m_topicMap.end() )
        {
            CORE::CString::StringSet::iterator i = allMatchingStreamNames.find( (*t).first );
            if ( i != allMatchingStreamNames.end() )
            {
                allMatchingStreamNames.erase( i );
            }
            ++t;
        }

        // Now we automatically create topic access for all the remaining topics
        // Note that in most cases event notifications from the cache should have caught the changes already so this is just a fallback for race conditions

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):OnStreamIndexingTimerCycle: Found " + 
            CORE::ToString( allMatchingStreamNames.size() ) + " new streams that match glob patterns. Will attempt to auto create topics for these" );        

        CORE::CString::StringSet::iterator n = allMatchingStreamNames.begin();
        while ( n != allMatchingStreamNames.end() )
        {
            const COMCORE::CPubSubClientTopicConfig* templateConfig = FindTemplateConfigForTopicName( (*n) );
            if ( GUCEF_NULL != templateConfig )
            {
                CORE::CString::StringSet topicNameList;
                topicNameList.insert( (*n) );

                PubSubClientTopicSet topicAccess;
                
                AutoCreateMultiTopicAccess( *templateConfig, topicNameList, topicAccess ); 
            }
            ++n;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::OnRedisKeyCacheUpdate( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

        //CORE::CString::StringVector filteredStreamNames;
        //CORE::CString::StringVector::iterator i = m_settings.streamsToGatherInfoFrom.begin();
        //while ( i != m_settings.streamsToGatherInfoFrom.end() )
        //{
        //    if ( (*i).HasChar( '*' ) > -1 )
        //    {
        //        CORE::CString::StringSet::iterator n = allGlobPatternTopicNames.begin();
        //        while ( n != allGlobPatternTopicNames.end() )
        //        {
        //            if ( (*n).WildcardEquals( (*i), '*', true ) )
        //            {
        //                filteredStreamNames.push_back( (*n) );
        //            }
        //            ++n;
        //        }
        //    }
        //    ++i;
        //}
        //m_filteredStreamNames = filteredStreamNames;

        //GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):OnStreamIndexingTimerCycle: Downloaded and found " + 
        //    CORE::ToString( allStreamNames.size() ) + " streams. Filtered those using wilcard pattern matching down to " + CORE::ToString( m_filteredStreamNames.size() ) +
        //    " streams which we will collect metric for"  );
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
