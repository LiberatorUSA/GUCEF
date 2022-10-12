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

CRedisClusterPubSubClient::CRedisClusterPubSubClient( const PUBSUB::CPubSubClientConfig& config )
    : PUBSUB::CPubSubClient()
    , m_config()
    , m_nodeMap()
    , m_redisContext()
    , m_redisErrorReplies( 0 )
    , m_metricsTimer( GUCEF_NULL )
    , m_redisReconnectTimer( GUCEF_NULL )
    , m_streamIndexingTimer( GUCEF_NULL )
    , m_topicMap()
    , m_threadPool()
    , m_needToTrackAcks( true )
    , m_isHealthy( true )
    , m_lock()
{GUCEF_TRACE;

    if ( !LoadConfig( config ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient: Failed to load config at construction" );
    }

    if ( m_config.pulseGenerator.IsNULL() )
        m_config.pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();

    if ( m_config.desiredFeatures.supportsMetrics )
    {
        m_metricsTimer = GUCEF_NEW CORE::CTimer( config.pulseGenerator, 1000 );
        m_metricsTimer->SetEnabled( config.desiredFeatures.supportsMetrics );
    }
    if ( m_config.desiredFeatures.supportsAutoReconnect )
    {
        m_redisReconnectTimer = GUCEF_NEW CORE::CTimer( config.pulseGenerator, config.reconnectDelayInMs );
    }
    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames )
    {                                                                      // @TODO: interval
        m_streamIndexingTimer = GUCEF_NEW CORE::CTimer( config.pulseGenerator, 100000 );
    }

    m_config.metricsPrefix += "redis.";

    if ( config.desiredFeatures.supportsSubscribing )
        m_threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetOrCreateThreadPool( 
                "RedisClusterPubSubClient(" + CORE::ToString( this ) + ")", 
                m_config.pulseGenerator, 
                true );

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClient::~CRedisClusterPubSubClient()
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    if ( !m_threadPool.IsNULL() )
        m_threadPool->RequestAllThreadsToStop( true, false );

    GUCEF_DELETE m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    GUCEF_DELETE m_redisReconnectTimer;
    m_redisReconnectTimer = GUCEF_NULL;

    GUCEF_DELETE m_streamIndexingTimer;
    m_streamIndexingTimer = GUCEF_NULL;

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        (*i).second->UnlinkFromParentClient();
        (*i).second.Unlink();
        ++i;
    }
    m_topicMap.clear();
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientConfig&
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
CRedisClusterPubSubClient::IsTrackingAcksNeeded( void ) const
{GUCEF_TRACE;

    return m_needToTrackAcks;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::GetSupportedFeatures( PUBSUB::CPubSubClientFeatures& features ) const 
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
    features.supportsSimultaneousPubAndSub = true;      // this plugin supports pub and sub on a topic at the same time
    features.supportsMetrics = true;                    // this plugin adds metrics support
    features.supportsAutoReconnect = true;              // Our plugin adds auto reconnect out of the box
    features.supportsSubscriberMsgReceivedAck = true;   // we can handle acks
    features.supportsAutoMsgReceivedAck = false;        // not supported right now
    features.supportsAbsentMsgReceivedAck = true;       // no such inherent concept
    features.supportsAckUsingLastMsgInBatch = false;    // not supported right now
    features.supportsAckUsingBookmark = true;           // we can handle acks
    features.supportsBookmarkingConcept = true;         // Redis does not support this server-side but does support it via passing your "bookmark" back to Redis as an offset
    features.supportsServerSideBookmarkPersistance = false; // no such support
    features.supportsSubscribingUsingBookmark = true;   // supported via giving Redis the starting offset
    features.supportsAutoBookmarking = false;           // Redis does not support this concept. The client needs to take care of remembering the offset
    features.supportsMsgIdBasedBookmark = true;         // This is the native Redis "bookmark" method and thus preferered
    features.supportsMsgIndexBasedBookmark = true;      // Same as supportsMsgIdBasedBookmark. This is the native Redis "bookmark" method and thus preferered
    features.supportsTopicIndexBasedBookmark = true;    // Same as supportsMsgIdBasedBookmark. This is the native Redis "bookmark" method and thus preferered
    features.supportsMsgDateTimeBasedBookmark = true;   // The Redis auto-generated msgId is a timestamp so its essentially the same thing for Redis
    features.supportsDerivingBookmarkFromMsg = true;    // The Redis auto-generated msgId acts as a topic index and as such we can derive a bookmark from a message
    features.supportsDiscoveryOfAvailableTopics = true; // we support scanning for available Redis streams
    features.supportsGlobPatternTopicNames = true;      // we support glob pattern matching the scan of available Redis streams
    features.supportsSubscriptionMsgArrivalDelayRequests = true;    // we support delaying the redis read thread on a per read cycle basis
    
    // the following features we enable based on expresed desired functionality, we support either
    features.supportsSubscriptionEndOfDataEvent = m_config.desiredFeatures.supportsSubscriptionEndOfDataEvent;
    return true;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopicPtr
CRedisClusterPubSubClient::CreateTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig )
{GUCEF_TRACE;

    // Check to see if this logical/conceptual 'topic' represents multiple pattern matched Redis Streams
    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames &&
         topicConfig.topicName.HasChar( '*' ) > -1               )
    {
        PubSubClientTopicSet allTopicAccess;
        if ( CreateMultiTopicAccess( topicConfig, allTopicAccess ) && !allTopicAccess.empty() )
        {
            // Caller should really use the CreateMultiTopicAccess() variant
            PUBSUB::CPubSubClientTopicPtr tAccess( *(allTopicAccess.begin()) );
            return tAccess;
        }
        return PUBSUB::CPubSubClientTopicPtr();
    }
    else
    {
        CRedisClusterPubSubClientTopicPtr topicAccess;
        {
            MT::CObjectScopeLock lock( this );

            topicAccess = ( GUCEF_NEW CRedisClusterPubSubClientTopic( this ) )->CreateSharedPtr();
            if ( topicAccess->LoadConfig( topicConfig ) )
            {
                m_topicMap[ topicConfig.topicName ] = topicAccess;

                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):CreateTopicAccess: created topic access for topic \"" + topicConfig.topicName + "\"" );

                TopicAccessCreatedEventData eData( topicConfig.topicName );
                NotifyObservers( TopicAccessCreatedEvent, &eData );
            }
            else
            {
                topicAccess.Unlink();
            }
        }
        return topicAccess;
    }
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopicPtr
CRedisClusterPubSubClient::GetTopicAccess( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    TTopicMap::iterator i = m_topicMap.find( topicName );
    if ( i != m_topicMap.end() )
    {
        return (*i).second;
    }
    return PUBSUB::CPubSubClientTopicPtr();
}

/*-------------------------------------------------------------------------*/

void 
CRedisClusterPubSubClient::GetAllCreatedTopicAccess( PubSubClientTopicSet& topicAccess )
{GUCEF_TRACE;
        
    MT::CObjectScopeLock lock( this );

    TTopicMap::iterator i = m_topicMap.begin();
    while ( i != m_topicMap.end() )
    {
        topicAccess.insert( (*i).second );
        ++i;
    }
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
CRedisClusterPubSubClient::GetMultiTopicAccess( const CORE::CString::StringSet& topicNames ,
                                                PubSubClientTopicSet& topicAccess          )
{GUCEF_TRACE;

    bool totalSuccess = true;
    CORE::CString::StringSet::const_iterator i = topicNames.begin();
    while ( i != topicNames.end() )
    {
        totalSuccess = GetMultiTopicAccess( (*i), topicAccess ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::AutoCreateMultiTopicAccess( const TTopicConfigPtrToStringSetMap& topicsToCreate ,
                                                       PubSubClientTopicSet& topicAccess                   )
{GUCEF_TRACE;

    CORE::UInt32 newTopicAccessCount = 0;
    bool totalSuccess = true;
    {
        MT::CObjectScopeLock lock( this );

        TTopicConfigPtrToStringSetMap::const_iterator m = topicsToCreate.begin();
        while ( m != topicsToCreate.end() )
        {
            const PUBSUB::CPubSubClientTopicConfig& templateTopicConfig = *((*m).first);
            const CORE::CString::StringSet& topicNameList = (*m).second;

            CORE::CString::StringSet::const_iterator i = topicNameList.begin();
            while ( i != topicNameList.end() )
            {
                PUBSUB::CPubSubClientTopicConfig topicConfig( templateTopicConfig );
                topicConfig.topicName = (*i);

                CRedisClusterPubSubClientTopicPtr tAccess;
                {
                    MT::CObjectScopeLock lock( this );

                    tAccess = ( GUCEF_NEW CRedisClusterPubSubClientTopic( this ) )->CreateSharedPtr();
                    if ( tAccess->LoadConfig( topicConfig ) )
                    {
                        m_topicMap[ topicConfig.topicName ] = tAccess;
                        topicAccess.insert( tAccess );
                        ++newTopicAccessCount;

                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):AutoCreateMultiTopicAccess: Auto created topic \"" +
                                topicConfig.topicName + "\" based on template config \"" + templateTopicConfig.topicName + "\"" );
                    }
                    else
                    {
                        tAccess.Unlink();
                        totalSuccess = false;

                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):AutoCreateMultiTopicAccess: Failed to load config for topic \"" +
                                topicConfig.topicName + "\" based on template config \"" + templateTopicConfig.topicName + "\"" );
                    }
                }
                ++i;
            }
            ++m;
        }
    }

    if ( newTopicAccessCount > 0 )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):AutoCreateMultiTopicAccess: Auto created " +
            CORE::ToString( newTopicAccessCount ) + " topics based on template configs" );

        TopicsAccessAutoCreatedEventData eData( topicAccess );
        NotifyObservers( TopicsAccessAutoCreatedEvent, &eData );
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::AutoCreateMultiTopicAccess( const PUBSUB::CPubSubClientTopicConfig& templateTopicConfig ,
                                                       const CORE::CString::StringSet& topicNameList                ,
                                                       PubSubClientTopicSet& topicAccess                            )
{GUCEF_TRACE;

    TTopicConfigPtrToStringSetMap topicToCreate;
    topicToCreate[ &templateTopicConfig ] = topicNameList;
    return AutoCreateMultiTopicAccess( topicToCreate, topicAccess );
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::CreateMultiTopicAccess( const PUBSUB::CPubSubClientTopicConfig& topicConfig ,
                                                   PubSubClientTopicSet& topicAccess                    )
{GUCEF_TRACE;

    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames &&
         topicConfig.topicName.HasChar( '*' ) > -1               )
    {
        CORE::CString::StringSet topicNameList;
        if ( CRedisClusterKeyCache::Instance()->GetRedisKeys( m_redisContext, topicNameList, "streams", topicConfig.topicName ) )
        {
            m_streamIndexingTimer->SetEnabled( true );

            if ( !topicNameList.empty() )
                return AutoCreateMultiTopicAccess( topicConfig, topicNameList, topicAccess );
            return true; // Since its pattern based potential creation at a later time also counts as success
        }
        return false;
    }
    else
    {
        PUBSUB::CPubSubClientTopicPtr tAccess = CreateTopicAccess( topicConfig );
        if ( !tAccess.IsNULL() )
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

    {
        MT::CScopeMutex lock( m_lock );

        TTopicMap::iterator i = m_topicMap.find( topicName );
        if ( i != m_topicMap.end() )
        {
            CRedisClusterPubSubClientTopicPtr topicAccess = (*i).second;
            m_topicMap.erase( i );

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):DestroyTopicAccess: destroyed topic access for topic \"" + topicName + "\"" );

            topicAccess->UnlinkFromParentClient();
            topicAccess.Unlink();
        }
    }

    TopicAccessDestroyedEventData eData( topicName );
    NotifyObservers( TopicAccessDestroyedEvent, &eData );
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::AutoDestroyTopicAccess( const CORE::CString::StringSet& topicNames )
{GUCEF_TRACE;

    PubSubClientTopicSet topicAccess;
    {
        MT::CScopeMutex lock( m_lock );

        CORE::CString::StringSet::const_iterator t = topicNames.begin();
        while ( t != topicNames.end() )
        {
            const CORE::CString& topicName = (*t);
            TTopicMap::iterator i = m_topicMap.find( topicName );
            if ( i != m_topicMap.end() )
            {
                CRedisClusterPubSubClientTopicPtr tAccess = (*i).second;
                topicAccess.insert( tAccess );
            }
            ++t;
        }
    }

    if ( !topicAccess.empty() )
    {
        TopicsAccessAutoDestroyedEventData eData( topicAccess );
        NotifyObservers( TopicsAccessAutoDestroyedEvent, &eData );

        {
            // Now that everyone has been duely warned we can proceed with the actual destruction
            MT::CObjectScopeLock lock( this );

            CORE::UInt32 destroyedTopicAccessCount = 0;
            PubSubClientTopicSet::iterator t = topicAccess.begin();
            while ( t != topicAccess.end() )
            {
                PUBSUB::CPubSubClientTopicBasicPtr tAccess = (*t);

                CORE::CString topicName = tAccess->GetTopicName();
                m_topicMap.erase( topicName );                
                {
                    CRedisClusterPubSubClientTopicBasicPtr topicAccess = tAccess.StaticCast< CRedisClusterPubSubClientTopic >();
                    topicAccess->UnlinkFromParentClient();
                    topicAccess.Unlink();
                }
                tAccess.Unlink();

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):AutoDestroyTopicAccess: destroyed topic access for topic \"" + topicName + "\"" );

                ++destroyedTopicAccessCount;
                ++t;
            }

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):AutoDestroyTopicAccess: destroyed topic access for " +
                CORE::ToString( destroyedTopicAccessCount ) + "topics" );
        }
    }
}

/*-------------------------------------------------------------------------*/

const PUBSUB::CPubSubClientTopicConfig*
CRedisClusterPubSubClient::GetTopicConfig( const CORE::CString& topicName )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
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
CRedisClusterPubSubClient::BeginTopicDiscovery( const CORE::CString::StringSet& globPatternFilters )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    TopicDiscoveryEventData topicNames;
    if ( CRedisClusterKeyCache::Instance()->GetRedisKeys( m_redisContext, topicNames, "stream", globPatternFilters ) )
    {
        if ( !topicNames.empty() )
            NotifyObservers( TopicDiscoveryEvent, &topicNames );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::GetConfiguredTopicNameList( CORE::CString::StringSet& topicNameList )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
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
CRedisClusterPubSubClient::DetermineIfTrackingAcksIsNeeded( void ) const
{GUCEF_TRACE;

    PUBSUB::CPubSubClientFeatures features;
    GetSupportedFeatures( features );
    
    // Whether we need to track successfull message handoff (garanteed handling) depends both on whether we want that extra reliability per the config
    // (optional since nothing is free and this likely degrades performance a bit) but also whether the backend even supports it.
    // If the backend doesnt support it all we will be able to do between the sides is fire-and-forget
    
    bool doWeWantIt = ( m_config.desiredFeatures.supportsSubscribing &&                         // <- does it apply in this context ?
                        ( m_config.desiredFeatures.supportsSubscriberMsgReceivedAck ||          // <- do we want it?
                          m_config.desiredFeatures.supportsSubscribingUsingBookmark  )
                      );

    bool isItSupported = features.supportsSubscriberMsgReceivedAck ||
                         ( features.supportsBookmarkingConcept && features.supportsSubscribingUsingBookmark );

    bool canWeNotWantIt = features.supportsAbsentMsgReceivedAck &&          // <- Is it even an option to not do it regardless of desired features
                          ( !features.supportsBookmarkingConcept ||         // <- if we need to perform client-side bookmarking then its not really an option to forgo acks if you want a reliable handoff and thus bookmark progression
                             features.supportsBookmarkingConcept && features.supportsSubscribingUsingBookmark && features.supportsServerSideBookmarkPersistance );
                              
    bool acksNeeded =  ( doWeWantIt && isItSupported ) || 
                       ( !canWeNotWantIt && isItSupported );

    return acksNeeded;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    cfg = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    PUBSUB::CPubSubClientConfig parsedCfg;
    if ( parsedCfg.LoadConfig( cfg ) )
    {
        return LoadConfig( parsedCfg );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg  )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    m_config = cfg;
    m_needToTrackAcks = DetermineIfTrackingAcksIsNeeded();
    return true;
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

    if ( m_redisContext.IsNULL() )
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
        MT::CScopeMutex lock( m_lock );
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
        MT::CScopeMutex lock( m_lock );

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
        m_redisContext = RedisClusterPtr( GUCEF_NEW sw::redis::RedisCluster( rppConnectionOptions ) );

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
CRedisClusterPubSubClient::IsConnected( void ) const
{GUCEF_TRACE;

    return !m_redisContext.IsNULL();
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::IsHealthy( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    if ( !m_topicMap.empty() )
    {
        // Aggregate the health status of all topics
        bool allHealthy = true;
        TTopicMap::const_iterator i = m_topicMap.begin();
        while ( i != m_topicMap.end() )
        {
            allHealthy = (*i).second->IsHealthy() && allHealthy;
            ++i;
        }

        // Notify if there was a change in status
        if ( allHealthy != m_isHealthy )
        {
            m_isHealthy = allHealthy;        

            if ( m_isHealthy )
            {
                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient:IsHealthy: overall health is now Ok" );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient:IsHealthy: overall health status is now unhealthy" );         
            }

            lock.EarlyUnlock();
            THealthStatusChangeEventData eData( allHealthy ); 
            NotifyObservers( HealthStatusChangeEvent, &eData );         
        }

        return allHealthy;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::IsInitialized( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

void 
CRedisClusterPubSubClient::RegisterTopicEventHandlers( PUBSUB::CPubSubClientTopic* topic )
{GUCEF_TRACE;

    if ( GUCEF_NULL != topic )
    {
        TEventCallback callback( this, &CRedisClusterPubSubClient::OnTopicHealthStatusChange );
        SubscribeTo( topic                                                   ,
                     CRedisClusterPubSubClientTopic::HealthStatusChangeEvent ,
                     callback                                                );
    }
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterPubSubClient::OnTopicHealthStatusChange( CORE::CNotifier* notifier    ,
                                                      const CORE::CEvent& eventId  ,
                                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // (Re)determine the aggregate health status
    IsHealthy();
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
    //if ( GUCEF_NULL != m_streamIndexingTimer )
    //{
    //    TEventCallback callback( this, &CRedisClusterPubSubClient::OnStreamIndexingTimerCycle );
    //    SubscribeTo( m_streamIndexingTimer          ,
    //                 CORE::CTimer::TimerUpdateEvent ,
    //                 callback                       );
    //}
    if ( m_config.desiredFeatures.supportsGlobPatternTopicNames )
    {
        TEventCallback callback( this, &CRedisClusterPubSubClient::OnRedisKeyCacheUpdate );
        SubscribeTo( CRedisClusterKeyCache::Instance()       ,
                     CRedisClusterKeyCache::CacheUpdateEvent ,
                     callback                                );
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
        CRedisClusterPubSubClientTopicPtr topic = (*i).second;
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
    PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
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
        PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = m_config.topics.begin();
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

const PUBSUB::CPubSubClientTopicConfig*
CRedisClusterPubSubClient::FindTemplateConfigForTopicName( const CORE::CString& topicName ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    PUBSUB::CPubSubClientConfig::TPubSubClientTopicConfigVector::const_iterator i = m_config.topics.begin();
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
        if ( !allMatchingStreamNames.empty() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):OnStreamIndexingTimerCycle: Found " +
                CORE::ToString( allMatchingStreamNames.size() ) + " new streams that match glob patterns. Will attempt to auto create topics for these" );
        }

        CORE::CString::StringSet::iterator n = allMatchingStreamNames.begin();
        while ( n != allMatchingStreamNames.end() )
        {
            const PUBSUB::CPubSubClientTopicConfig* templateConfig = FindTemplateConfigForTopicName( (*n) );
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

    // get event data and sanity check
    CRedisClusterKeyCache::CacheUpdateEventData* updateInfo = static_cast< CRedisClusterKeyCache::CacheUpdateEventData* >( eventData );
    if ( GUCEF_NULL == updateInfo )
        return;

    // See if this update applies to us
    if ( m_redisContext.GetPointerAlways() != updateInfo->redisCluster.GetPointerAlways() )
        return;
    if ( updateInfo->keyType != "stream" )
        return;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):OnRedisKeyCacheUpdate: " +
        CORE::ToString( updateInfo->newKeys.size() ) + " new streams, " + CORE::ToString( updateInfo->deletedKeys.size() ) + " deleted streams" );

    // Generally notify of any new topics that were discovered
    if ( !updateInfo->newKeys.empty() )
    {
        if ( !NotifyObservers( TopicDiscoveryEvent, &updateInfo->newKeys ) ) return;
    }
    
    // Build a bulk creation map linking template config to stream names
    // This allows for better batch processing down the line, reducing overhead
    TTopicConfigPtrToStringSetMap bulkCreationMap;
    CORE::CString::StringSet::iterator n = updateInfo->newKeys.begin();
    while ( n != updateInfo->newKeys.end() )
    {
        const PUBSUB::CPubSubClientTopicConfig* templateConfig = FindTemplateConfigForTopicName( (*n) );
        if ( GUCEF_NULL != templateConfig )
        {
            bulkCreationMap[ templateConfig ].insert( (*n) );
        }
        ++n;
    }

    if ( !bulkCreationMap.empty() )
    {
        // Bulk Auto generate topics for the new Redis keys
        PubSubClientTopicSet topicAccess;
        AutoCreateMultiTopicAccess( bulkCreationMap, topicAccess );
    }
    if ( !updateInfo->deletedKeys.empty() )
    {
        // Bulk Auto delete topics for deleted Redis keys
        AutoDestroyTopicAccess( updateInfo->deletedKeys );
    }
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClient::Unlock( void ) const 
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
