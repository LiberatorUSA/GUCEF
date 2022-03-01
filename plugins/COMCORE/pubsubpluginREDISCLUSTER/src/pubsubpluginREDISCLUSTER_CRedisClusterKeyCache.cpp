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
CRedisClusterKeyCache::GetRedisKeys( RedisClusterPtr redisCluster                        ,
                                     CORE::CString::StringSet& keys                      ,
                                     const CORE::CString& keyType                        ,
                                     const CORE::CString::StringSet& globPatternsToMatch )
{GUCEF_TRACE;

    if ( redisCluster.IsNULL() )
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
    return GetRedisKeys( redisCluster, keys, keyType, globPatternToMatch );
}

/*-------------------------------------------------------------------------*/

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace REDISCLUSTER */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
