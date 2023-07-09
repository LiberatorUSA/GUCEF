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
    : PUBSUB::CPubSubClientTopicConfig()
    , CORE::CTSharedObjCreator< CRedisClusterPubSubClientTopicConfig, MT::CMutex >( this )
    , redisXAddMaxLen( -1 )
    , redisXAddMaxLenIsApproximate( true )    
    , redisXAddIgnoreMsgId( true )
    , redisXReadDefaultOffset( "0" )
    , redisXReadCount( -1 )
    , redisXReadBlockTimeoutInMs( 1000 )
    , treatXReadBlockTimeoutAsEndOfDataEvent( true )
    , minAvailableInFlightSlotsBeforeRead( 100 )
    , useDedicatedReadThread( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicConfig::CRedisClusterPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig )
    : PUBSUB::CPubSubClientTopicConfig( genericConfig )
    , CORE::CTSharedObjCreator< CRedisClusterPubSubClientTopicConfig, MT::CMutex >( this )
    , redisXAddMaxLen( -1 )
    , redisXAddMaxLenIsApproximate( true )    
    , redisXAddIgnoreMsgId( true )
    , redisXReadDefaultOffset( "0" )
    , redisXReadCount( -1 )
    , redisXReadBlockTimeoutInMs( 1000 )
    , treatXReadBlockTimeoutAsEndOfDataEvent( true )
    , minAvailableInFlightSlotsBeforeRead( 100 )
    , useDedicatedReadThread( true )
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
    redisXAddIgnoreMsgId = config.GetAttributeValueOrChildValueByName( "redisXAddIgnoreMsgId" ).AsBool( redisXAddIgnoreMsgId, true ); 
    redisXReadDefaultOffset = config.GetAttributeValueOrChildValueByName( "redisXReadDefaultOffset" ).AsString( redisXReadDefaultOffset, true );
    redisXReadCount = config.GetAttributeValueOrChildValueByName( "redisXReadCount" ).AsInt32( redisXReadCount, true ); 
    redisXReadBlockTimeoutInMs = config.GetAttributeValueOrChildValueByName( "redisXReadBlockTimeoutInMs" ).AsUInt32( redisXReadBlockTimeoutInMs );
    treatXReadBlockTimeoutAsEndOfDataEvent = config.GetAttributeValueOrChildValueByName( "treatXReadBlockTimeoutAsEndOfDataEvent" ).AsBool( treatXReadBlockTimeoutAsEndOfDataEvent, true ); 
    minAvailableInFlightSlotsBeforeRead = config.GetAttributeValueOrChildValueByName( "minAvailableInFlightSlotsBeforeRead" ).AsInt32( minAvailableInFlightSlotsBeforeRead );
    useDedicatedReadThread = config.GetAttributeValueOrChildValueByName( "useDedicatedReadThread" ).AsBool( useDedicatedReadThread, true );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CRedisClusterPubSubClientTopicConfig::LoadConfig( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        return LoadCustomConfig( src.customConfig );    
    }
    return true;
}

/*-------------------------------------------------------------------------*/

CRedisClusterPubSubClientTopicConfig& 
CRedisClusterPubSubClientTopicConfig::operator=( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
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
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        redisXAddMaxLen = src.redisXAddMaxLen; 
        redisXAddMaxLenIsApproximate = src.redisXAddMaxLenIsApproximate;   
        redisXAddIgnoreMsgId = src.redisXAddIgnoreMsgId;
        redisXReadDefaultOffset = src.redisXReadDefaultOffset;
        redisXReadCount = src.redisXReadCount;
        redisXReadBlockTimeoutInMs = src.redisXReadBlockTimeoutInMs;
        treatXReadBlockTimeoutAsEndOfDataEvent = src.treatXReadBlockTimeoutAsEndOfDataEvent;
        minAvailableInFlightSlotsBeforeRead = src.minAvailableInFlightSlotsBeforeRead;
        useDedicatedReadThread = src.useDedicatedReadThread;
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
