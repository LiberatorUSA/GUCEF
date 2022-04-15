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

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERKEYCACHEUPDATETASK_H
#include "pubsubpluginREDISCLUSTER_CRedisClusterKeyCacheUpdateTask.h"
#define PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERKEYCACHEUPDATETASK_H
#endif /* PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERKEYCACHEUPDATETASK_H */

#include "pubsubpluginREDISCLUSTER_CRedisClusterKeyCache.h"

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

const CORE::CEvent CRedisClusterKeyCache::CacheUpdateEvent = "GUCEF::PUBSUBPLUGIN::REDISCLUSTER::CRedisClusterKeyCache::CacheUpdateEvent";

MT::CReadWriteLock CRedisClusterKeyCache::g_dataLock( true );
CRedisClusterKeyCache* CRedisClusterKeyCache::g_instance = GUCEF_NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CRedisClusterKeyCache::CacheUpdateInfo::CacheUpdateInfo( void )
    : CORE::CICloneable()
    , redisCluster()
    , keyType()
    , newKeys()
    , deletedKeys()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisClusterKeyCache::CacheUpdateInfo::CacheUpdateInfo( const CacheUpdateInfo& src )
    : CORE::CICloneable( src )
    , redisCluster( src.redisCluster )
    , keyType( src.keyType )
    , newKeys( src.newKeys )
    , deletedKeys( src.deletedKeys )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisClusterKeyCache::CacheUpdateInfo::~CacheUpdateInfo()
{GUCEF_TRACE;

    redisCluster.Unlink();
    keyType.Clear();
    newKeys.clear();
    deletedKeys.clear();
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CRedisClusterKeyCache::CacheUpdateInfo::Clone( void ) const
{GUCEF_TRACE;

    return new CacheUpdateInfo( *this );
}

/*-------------------------------------------------------------------------*/

CRedisClusterKeyCache::CRedisClusterKeyCache( void )
    : CORE::CObservingNotifier()
    , m_threadPool()
    , m_cacheUpdateTask()
    , m_cache()
{GUCEF_TRACE;

    m_threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetOrCreateThreadPool( "PUBSUBPLUGIN::REDISCLUSTER::RedisClusterKeyCache", true );
    m_cacheUpdateTask = ( new CRedisClusterKeyCacheUpdateTask() )->CreateSharedPtr().StaticCast< CORE::CTaskConsumer >();
}

/*-------------------------------------------------------------------------*/

 CRedisClusterKeyCache::~CRedisClusterKeyCache( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( g_dataLock );
    
    if ( !m_threadPool.IsNULL() )
        m_threadPool->RequestAllThreadsToStop( true, false );
    m_threadPool.Unlink();

    m_cacheUpdateTask.Unlink();

    m_cache.clear();
}

/*-------------------------------------------------------------------------*/

CRedisClusterKeyCache*
CRedisClusterKeyCache::Instance()
{GUCEF_TRACE;

    if ( GUCEF_NULL == g_instance )
    {
        MT::CScopeWriterLock lock( g_dataLock );
        if ( GUCEF_NULL == g_instance )
        {
            RegisterEvents();
            g_instance = new CRedisClusterKeyCache();
        }
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterKeyCache::Deinstance( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( g_dataLock );
    delete g_instance;
    g_instance = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterKeyCache::RegisterEvents( void )
{GUCEF_TRACE;

    CacheUpdateEvent.Initialize();
}


/*-------------------------------------------------------------------------*/

bool
CRedisClusterKeyCache::LazyStartCacheUpdateTask( void )
{GUCEF_TRACE;

    {
        MT::CScopeReaderLock lock( g_dataLock );
        if ( !m_cacheUpdateTask.IsNULL() && 0 != m_cacheUpdateTask->GetDelegatorThreadId() )
            return true;
    }
    {
        MT::CScopeWriterLock lock( g_dataLock );
        return m_threadPool->StartTask( m_cacheUpdateTask );
    }
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterKeyCache::GetRedisKeys( RedisClusterPtr redisCluster                        ,
                                     CORE::CString::StringSet& keys                      ,
                                     const CORE::CString& keyType                        ,
                                     const CORE::CString::StringSet& globPatternsToMatch )
{GUCEF_TRACE;

    if ( redisCluster.IsNULL() || !LazyStartCacheUpdateTask() )
        return false;

    MT::CScopeReaderLock lock( g_dataLock );
    
    TStringToStringSetMap& keyMap = m_cache[ redisCluster ];
    CORE::CString::StringSet& cachedKeys = keyMap[ keyType ]; 

    if ( globPatternsToMatch.empty() )
    {
        // No pattern matching, return everything
        keys = cachedKeys;
        return true;
    }

    // Match the glob patterns given for all cached keys
    CORE::CString::StringSet::iterator i = cachedKeys.begin();
    while ( i != cachedKeys.end() )
    {
        const CORE::CString& key = (*i);        
        CORE::CString::StringSet::iterator g = globPatternsToMatch.begin();
        while ( g != globPatternsToMatch.end() )
        {
            const CORE::CString& globPattern = (*g);
            if ( key.WildcardEquals( globPattern, '*', true ) )
            {
                keys.insert( key );
                break;
            }
            ++g;
        }
        ++i;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterKeyCache::GetRedisKeys( RedisClusterPtr redisCluster            ,
                                     CORE::CString::StringSet& keys          ,
                                     const CORE::CString& keyType            ,
                                     const CORE::CString& globPatternToMatch )
{GUCEF_TRACE;

    CORE::CString::StringSet globPatternsToMatch;
    globPatternsToMatch.insert( globPatternToMatch );
    return GetRedisKeys( redisCluster, keys, keyType, globPatternsToMatch );
}

/*-------------------------------------------------------------------------*/

// Only intended to be used by the closely coupled task class
bool 
CRedisClusterKeyCache::GetRedisClusterAccess( RedisClusterPtrSet& set )
{GUCEF_TRACE;

    try
    {
        MT::CScopeReaderLock lock( g_dataLock );
    
        TRedisClusterPtrToTypeKeysMap::iterator i = m_cache.begin();
        while ( i != m_cache.end() )
        {
            set.insert( (*i).first );
            ++i;
        }
    }
    catch ( const std::exception& e )
    {
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

// Only intended to be used by the closely coupled task class
bool 
CRedisClusterKeyCache::GetRedisKeyCache( RedisClusterPtr& redisCluster    ,
                                         const CORE::CString& keyType     ,
                                         CORE::CString::StringSet** keys  )
{GUCEF_TRACE;

    if ( redisCluster.IsNULL() || keys == GUCEF_NULL )
        return false;

    TStringToStringSetMap& keyMap = m_cache[ redisCluster ];
    CORE::CString::StringSet& cachedKeys = keyMap[ keyType ];
    
    *keys = &cachedKeys;
    return true;
}

/*-------------------------------------------------------------------------*/

// Only intended to be used by the closely coupled task class
bool 
CRedisClusterKeyCache::ApplyKeyDelta( CacheUpdateInfo& updateInfo )
{GUCEF_TRACE;

    if ( updateInfo.redisCluster.IsNULL() )
        return false;

    try
    {
        // First we apply the updates to the actual cache
        // We keep a write lock on the cache during this time
        {
            MT::CScopeWriterLock lock( g_dataLock );

            TStringToStringSetMap& keyMap = m_cache[ updateInfo.redisCluster ];
            CORE::CString::StringSet& cachedKeys = keyMap[ updateInfo.keyType ];    

            // delete the deleted keys
            CORE::CString::StringSet::iterator i = updateInfo.deletedKeys.begin();
            while ( i != updateInfo.deletedKeys.end() )
            {
                cachedKeys.erase( (*i) );
                ++i;
            }

            // Add the new keys
            i = updateInfo.newKeys.begin();
            while ( i != updateInfo.newKeys.end() )
            {
                cachedKeys.insert( (*i) );
                ++i;
            }
        }

        // Now that the changes have been applied and the write lock released we 
        // notify of the changes
        NotifyObservers( CacheUpdateEvent, &updateInfo );
    }
    catch ( const std::exception& e )
    {
        return false;
    }
    return true;    
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
