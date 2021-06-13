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

#include "pubsubpluginREDISCLUSTER_CRedisClusterPubSubClientTopicConfig.h"

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

CRedisClusterPubSubClientTopicConfig::CRedisClusterPubSubClientTopicConfig( void )
    : COMCORE::CPubSubClientTopicConfig()
    , redisXAddMaxLen( -1 )
    , redisXAddMaxLenIsApproximate( true )
    , redisXReadDefaultOffset( "0" )
    , redisXAddIgnoreMsgId( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicConfig::CRedisClusterPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig )
    : COMCORE::CPubSubClientTopicConfig( genericConfig )
    , redisXAddMaxLen( -1 )
    , redisXAddMaxLenIsApproximate( true )
    , redisXReadDefaultOffset( "0" )
    , redisXAddIgnoreMsgId( true )
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
    
    redisXAddMaxLen = config.GetAttributeValueOrChildValueByName( "redisXAddMaxLen" ).AsInt32( redisXAddMaxLen, true ); 
    redisXAddMaxLenIsApproximate = config.GetAttributeValueOrChildValueByName( "redisXAddMaxLenIsApproximate" ).AsBool( redisXAddMaxLenIsApproximate, true );
    redisXReadDefaultOffset = config.GetAttributeValueOrChildValueByName( "redisXReadDefaultOffset" ).AsString( redisXReadDefaultOffset, true ); 
    redisXAddIgnoreMsgId = config.GetAttributeValueOrChildValueByName( "redisXAddIgnoreMsgId" ).AsBool( redisXAddIgnoreMsgId, true ); 
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace REDISCLUSTER */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
