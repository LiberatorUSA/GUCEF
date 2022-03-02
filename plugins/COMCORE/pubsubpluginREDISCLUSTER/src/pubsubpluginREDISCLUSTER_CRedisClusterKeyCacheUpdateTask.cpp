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

#include "pubsubpluginREDISCLUSTER_CRedisClusterKeyCacheUpdateTask.h"

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

#define GUCEF_DEFAULT_CACHE_UPDATE_TIMER_INTERVAL       ( 5 * 60 * 1000 ) // 5mins

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CRedisClusterKeyCacheUpdateTask::CRedisClusterKeyCacheUpdateTask( void )
    : CORE::CTaskConsumer()
    , CORE::CTSharedPtrCreator< CRedisClusterKeyCacheUpdateTask, MT::CMutex >( this )
    , m_indexingTimer( GUCEF_NULL )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CRedisClusterKeyCacheUpdateTask::~CRedisClusterKeyCacheUpdateTask()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CRedisClusterKeyCacheUpdateTask::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CRedisClusterKeyCacheUpdateTask::OnIndexingTimerCycle );
    SubscribeTo( m_indexingTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );
}   

/*-------------------------------------------------------------------------*/

bool 
CRedisClusterKeyCacheUpdateTask::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    CORE::CPulseGenerator* pulseGenerator = GetPulseGenerator();
    if ( GUCEF_NULL == pulseGenerator )
        return false;

    m_indexingTimer = new CORE::CTimer( pulseGenerator, GUCEF_DEFAULT_CACHE_UPDATE_TIMER_INTERVAL );

    RegisterEventHandlers();

    m_indexingTimer->SetEnabled( true );
    m_indexingTimer->TriggerNow();
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterKeyCacheUpdateTask::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    // We are never done.
    // Perpetual task running on a timer
    return false;
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterKeyCacheUpdateTask::OnIndexingTimerCycle( CORE::CNotifier* notifier    ,
                                                       const CORE::CEvent& eventId  ,
                                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CRedisClusterKeyCache::RedisClusterPtrSet clusterAccessSet;
    if ( CRedisClusterKeyCache::Instance()->GetRedisClusterAccess( clusterAccessSet ) )
    {
        CRedisClusterKeyCache::RedisClusterPtrSet::iterator i = clusterAccessSet.begin();
        while ( i != clusterAccessSet.end() && !IsDeactivationRequested() )
        {
            CRedisClusterKeyCache::CacheUpdateInfo updateInfo;
            updateInfo.redisCluster = (*i);
            updateInfo.keyType = "stream";

            // Get the existing cached keys
            // Note we do so via pointer access to improve performance since this task and the cache itself are tightly coupled regardless
            MT::CScopeReaderLock readerLock( CRedisClusterKeyCache::Instance()->g_dataLock );
            CORE::CString::StringSet* allOldKeys = GUCEF_NULL;
            if ( CRedisClusterKeyCache::Instance()->GetRedisKeyCache( updateInfo.redisCluster, updateInfo.keyType, &allOldKeys ) && !IsDeactivationRequested() )
            {
                // Get all keys from the cluster and determine the key delta on the fly
                if ( GetRedisKeys( updateInfo.redisCluster, updateInfo.keyType, *allOldKeys, updateInfo.newKeys, updateInfo.deletedKeys ) && !IsDeactivationRequested() )
                {
                    // Now that we have determined the key delta we can release the reader lock
                    readerLock.EarlyUnlock();

                    // Now we perform the cache write action of applying the delta
                    CRedisClusterKeyCache::Instance()->ApplyKeyDelta( updateInfo );
                }
            }
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterKeyCacheUpdateTask::OnTaskEnding( CORE::CICloneable* taskdata ,
                                               bool willBeForced           )
{GUCEF_TRACE;

    CORE::CTaskConsumer::OnTaskEnding( taskdata, willBeForced );
}

/*-------------------------------------------------------------------------*/

void
CRedisClusterKeyCacheUpdateTask::OnTaskEnded( CORE::CICloneable* taskdata ,
                                              bool wasForced              )
{GUCEF_TRACE;

    delete m_indexingTimer;
    m_indexingTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CORE::CString 
CRedisClusterKeyCacheUpdateTask::GetType( void ) const
{GUCEF_TRACE;

    static const CORE::CString taskTypeName = "RedisClusterKeyCacheUpdateTask";
    return taskTypeName;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterKeyCacheUpdateTask::GetRedisKeys( RedisClusterPtr& clusterAccess              ,
                                               const CORE::CString& keyType                ,
                                               const CORE::CString::StringSet& currentKeys ,
                                               CORE::CString::StringSet& newKeys           ,
                                               CORE::CString::StringSet& deletedKeys       )
{GUCEF_TRACE;

    if ( clusterAccess.IsNULL() )
        return false;

    CORE::CString::StringSet stillExistsKeys;
    CORE::UInt32 keysFound = 0;
    try
    {
        static const CORE::CString scanCmd( "SCAN" );
        CORE::CString itteratorParam( "0" );
        static const CORE::CString typeParam( "TYPE" );
        //static const CORE::CString matchParam( "MATCH" );

        CORE::UInt32 iterationCounter = 0;
        sw::redis::StringView scanCmdSV( scanCmd.C_String(), scanCmd.Length() );        
        do
        {
            sw::redis::StringView itteratorParamSV( itteratorParam.C_String(), itteratorParam.Length() );
            sw::redis::StringView typeParamSV( typeParam.C_String(), typeParam.Length() );
            sw::redis::StringView typeValueParamSV( keyType.C_String(), keyType.Length() );
            //sw::redis::StringView matchParamSV( matchParam.C_String(), matchParam.Length() );
            //sw::redis::StringView matchValueParamSV( keyType.C_String(), keyType.Length() );
            
            sw::redis::ReplyUPtr cmdReply = clusterAccess->command( scanCmdSV, itteratorParamSV, typeParamSV, typeValueParamSV );
            if ( cmdReply )
            {
                if ( REDIS_REPLY_ARRAY == cmdReply->type )
                {
                    size_t ittCount = cmdReply->elements;

                    if ( ittCount > 0 && REDIS_REPLY_STRING == cmdReply->element[ 0 ]->type )
                        itteratorParam = cmdReply->element[ 0 ]->str;
                    else
                        return false;

                    if ( ittCount > 1 && REDIS_REPLY_ARRAY == cmdReply->element[ 1 ]->type )
                    {
                        size_t keyCount = cmdReply->element[ 1 ]->elements;
                        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "RedisClusterKeyCacheUpdateTask(" + CORE::PointerToString( this ) + "):GetRedisKeys: Received " + CORE::ToString( keyCount ) + " keys in SCAN iteration nr " + CORE::ToString( iterationCounter ) );

                        struct redisReply** keyList = cmdReply->element[ 1 ]->element;
                        for ( size_t n=0; n<keyCount; ++n )
                        {
                            if ( REDIS_REPLY_STRING == keyList[ n ]->type )
                            {
                                CORE::CString keyName( keyList[ n ]->str );
                                if ( currentKeys.find( keyName ) == currentKeys.end() )
                                {
                                    newKeys.insert( keyName );
                                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "RedisClusterKeyCacheUpdateTask(" + CORE::PointerToString( this ) + "):GetRedisKeys: New key found: " + keyName );
                                }
                                else
                                {
                                    stillExistsKeys.insert( keyName );
                                }
                                
                                ++keysFound;
                            }
                        }
                    }
                    else
                        return false;
                }
            }
            ++iterationCounter;
        }
        while ( itteratorParam != "0" && !IsDeactivationRequested() );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "RedisClusterKeyCacheUpdateTask(" + CORE::PointerToString( this ) + "):GetRedisKeys: Found " + CORE::ToString( keysFound ) + " keys in Redis cluster of type " + keyType );
    }
    catch ( const sw::redis::Error& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterKeyCacheUpdateTask(" + CORE::PointerToString( this ) + "):GetRedisKeys: Redis++ exception: " + e.what() );
        //++m_redisErrorReplies;
        //Disconnect();
        //if ( GUCEF_NULL != m_redisReconnectTimer )
        //    m_redisReconnectTimer->SetEnabled( true );
        return false;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, "RedisClusterPubSubClient(" + CORE::PointerToString( this ) + "):GetRedisKeys: exception: " + e.what() );
        return false;
    }

    if ( !IsDeactivationRequested() )
    {
        // Now we determine the deleted keys
        CORE::CString::StringSet::iterator o = currentKeys.begin();
        while ( o != currentKeys.end() )
        {
            if ( stillExistsKeys.find( (*o) ) == stillExistsKeys.end() )                
            {
                deletedKeys.insert( (*o) );
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "RedisClusterKeyCacheUpdateTask(" + CORE::PointerToString( this ) + "):GetRedisKeys: Deleted key found: " + (*o) );
            }
            ++o;
        }
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
