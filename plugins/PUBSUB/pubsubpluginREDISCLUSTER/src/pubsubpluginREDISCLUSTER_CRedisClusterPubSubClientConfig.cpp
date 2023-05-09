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

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#include "pubsubpluginREDISCLUSTER_CRedisClusterPubSubClientConfig.h"

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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_DEFAULT_REDIS_KEEPALIVE                               true
#define GUCEF_DEFAULT_REDIS_CONNECT_TIMEOUT_IN_MS                   1000
#define GUCEF_DEFAULT_REDIS_SOCKET_TIMEOUT_IN_MS                    1000

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CRedisClusterPubSubClientConfig::CRedisClusterPubSubClientConfig( void )
    : PUBSUB::CPubSubClientConfig()
    , redisConnectionOptionKeepAlive( GUCEF_DEFAULT_REDIS_KEEPALIVE )
    , redisConnectionOptionConnectTimeoutInMs( GUCEF_DEFAULT_REDIS_CONNECT_TIMEOUT_IN_MS )
    , redisConnectionOptionSocketTimeoutInMs( GUCEF_DEFAULT_REDIS_SOCKET_TIMEOUT_IN_MS )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientConfig::CRedisClusterPubSubClientConfig( const PUBSUB::CPubSubClientConfig& genericConfig )
    : PUBSUB::CPubSubClientConfig( genericConfig )
    , redisConnectionOptionKeepAlive( GUCEF_DEFAULT_REDIS_KEEPALIVE )
    , redisConnectionOptionConnectTimeoutInMs( GUCEF_DEFAULT_REDIS_CONNECT_TIMEOUT_IN_MS )
    , redisConnectionOptionSocketTimeoutInMs( GUCEF_DEFAULT_REDIS_SOCKET_TIMEOUT_IN_MS )
{GUCEF_TRACE;

    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientConfig::~CRedisClusterPubSubClientConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    redisConnectionOptionKeepAlive = config.GetAttributeValueOrChildValueByName( "redisConnectionOptionKeepAlive" ).AsBool( redisConnectionOptionKeepAlive, true );
    redisConnectionOptionConnectTimeoutInMs = config.GetAttributeValueOrChildValueByName( "redisConnectionOptionConnectTimeoutInMs" ).AsUInt64( redisConnectionOptionConnectTimeoutInMs, true );
    redisConnectionOptionSocketTimeoutInMs = config.GetAttributeValueOrChildValueByName( "redisConnectionOptionSocketTimeoutInMs" ).AsUInt64( redisConnectionOptionSocketTimeoutInMs, true );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientConfig::LoadConfig( const PUBSUB::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( src );
        return LoadCustomConfig( src.customConfig );    
    }
    return true;
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientConfig& 
CRedisClusterPubSubClientConfig::operator=( const PUBSUB::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( src );
        LoadCustomConfig( src.customConfig );    
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientConfig& 
CRedisClusterPubSubClientConfig::operator=( const CRedisClusterPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( src );

        redisConnectionOptionKeepAlive = src.redisConnectionOptionKeepAlive;
        redisConnectionOptionConnectTimeoutInMs = src.redisConnectionOptionConnectTimeoutInMs;
        redisConnectionOptionSocketTimeoutInMs = src.redisConnectionOptionSocketTimeoutInMs;
    }
    return *this;
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
