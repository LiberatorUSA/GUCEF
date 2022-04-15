/*
 *  pubsubpluginSTORAGE: Generic GUCEF COMCORE plugin for providing pubsub storage
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

#include "pubsubpluginSTORAGE_CStoragePubSubClientTopicConfig.h"

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace STORAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE                  10000
#define GUCEF_DEFAULT_PUBSUB_RECONNECT_DELAY_IN_MS                  100
#define GUCEF_DEFAULT_MINIMAL_PUBSUB_BLOCK_STORAGE_SIZE_IN_BYTES    (1024*1024*50)// 50MB
#define GUCEF_DEFAULT_MAXIMAL_PUBSUB_BLOCK_STORE_GROW_DELAY_IN_MS   (1000*60*5)   // 5mins
#define GUCEF_DEFAULT_DECODE_GROWTH_RATIO_EXPECTATION               6.0f

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig::CStoragePubSubClientTopicConfig( void )
    : PUBSUB::CPubSubClientTopicConfig()
    , pubsubBinarySerializerOptions()
    , pubsubSerializerOptions()
    , desiredMinimalSerializedBlockSize( GUCEF_DEFAULT_MINIMAL_PUBSUB_BLOCK_STORAGE_SIZE_IN_BYTES )
    , desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs( GUCEF_DEFAULT_MAXIMAL_PUBSUB_BLOCK_STORE_GROW_DELAY_IN_MS )
    , vfsStorageRootPath()
    , vfsFileExtention()
    , encodeCodecFamily()
    , encodeCodecName()
    , decodeCodecFamily()
    , decodeCodecName()
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , performVfsOpsASync( true )
    , performVfsOpsInDedicatedThread( true )
    , applyDedicatedVfsOpsThreadCpuAffinity( false )
    , cpuAffinityForDedicatedVfsOpsThread( 0 )
    , collectMetrics( true )
    , mode( CHANNELMODE_PUBSUB_TO_STORAGE )
    , subscribeWithoutBookmarkIfNoneIsPersisted( true )
    , autoPushAfterStartupIfStorageToPubSub( true )
    , youngestStoragePubSubMsgFileToLoad( CORE::CDateTime::FutureMax )
    , oldestStoragePubSubMsgFileToLoad( CORE::CDateTime::PastMax )  
    , defaultCodecDecodeGrowthRatioExpectation( GUCEF_DEFAULT_DECODE_GROWTH_RATIO_EXPECTATION )
    , bestEffortDeserializeIsAllowed( false )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig::CStoragePubSubClientTopicConfig( const CStoragePubSubClientTopicConfig& src )
    : PUBSUB::CPubSubClientTopicConfig( src )
    , pubsubBinarySerializerOptions( src.pubsubBinarySerializerOptions )
    , pubsubSerializerOptions( src.pubsubSerializerOptions )
    , desiredMinimalSerializedBlockSize( src.desiredMinimalSerializedBlockSize )
    , desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs( src.desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs )
    , vfsStorageRootPath( src.vfsStorageRootPath )
    , vfsFileExtention( src.vfsFileExtention )
    , encodeCodecFamily( src.encodeCodecFamily )
    , encodeCodecName( src.encodeCodecName )
    , decodeCodecFamily( src.decodeCodecFamily )
    , decodeCodecName( src.decodeCodecName )
    , ticketRefillOnBusyCycle( src.ticketRefillOnBusyCycle )
    , performVfsOpsASync( src.performVfsOpsASync )
    , performVfsOpsInDedicatedThread( src.performVfsOpsInDedicatedThread )
    , applyDedicatedVfsOpsThreadCpuAffinity( src.applyDedicatedVfsOpsThreadCpuAffinity )
    , cpuAffinityForDedicatedVfsOpsThread( src.cpuAffinityForDedicatedVfsOpsThread )
    , collectMetrics( src.collectMetrics )
    , mode( src.mode )
    , subscribeWithoutBookmarkIfNoneIsPersisted( src.subscribeWithoutBookmarkIfNoneIsPersisted )
    , autoPushAfterStartupIfStorageToPubSub( src.autoPushAfterStartupIfStorageToPubSub )
    , youngestStoragePubSubMsgFileToLoad( src.youngestStoragePubSubMsgFileToLoad )
    , oldestStoragePubSubMsgFileToLoad( src.oldestStoragePubSubMsgFileToLoad )
    , defaultCodecDecodeGrowthRatioExpectation( src.defaultCodecDecodeGrowthRatioExpectation )
    , bestEffortDeserializeIsAllowed( src.bestEffortDeserializeIsAllowed )
{GUCEF_TRACE;
    
    customConfig = src.customConfig;  
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig::CStoragePubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig )
    : PUBSUB::CPubSubClientTopicConfig( genericConfig )
    , pubsubBinarySerializerOptions()
    , pubsubSerializerOptions()
    , desiredMinimalSerializedBlockSize( GUCEF_DEFAULT_MINIMAL_PUBSUB_BLOCK_STORAGE_SIZE_IN_BYTES )
    , desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs( GUCEF_DEFAULT_MAXIMAL_PUBSUB_BLOCK_STORE_GROW_DELAY_IN_MS )
    , vfsStorageRootPath()
    , vfsFileExtention()
    , encodeCodecFamily()
    , encodeCodecName()
    , decodeCodecFamily()
    , decodeCodecName()
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , performVfsOpsASync( true )
    , performVfsOpsInDedicatedThread( true )
    , applyDedicatedVfsOpsThreadCpuAffinity( false )
    , cpuAffinityForDedicatedVfsOpsThread( 0 )
    , collectMetrics( true )
    , mode( CHANNELMODE_PUBSUB_TO_STORAGE )
    , subscribeWithoutBookmarkIfNoneIsPersisted( true )
    , autoPushAfterStartupIfStorageToPubSub( true )
    , youngestStoragePubSubMsgFileToLoad( CORE::CDateTime::FutureMax )
    , oldestStoragePubSubMsgFileToLoad( CORE::CDateTime::PastMax )  
    , defaultCodecDecodeGrowthRatioExpectation( GUCEF_DEFAULT_DECODE_GROWTH_RATIO_EXPECTATION )
    , bestEffortDeserializeIsAllowed( false )
{GUCEF_TRACE;
    
    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig::~CStoragePubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    bool success = true;
    desiredMinimalSerializedBlockSize = config.GetAttributeValueOrChildValueByName( "desiredMinimalSerializedBlockSize" ).AsUInt32( desiredMinimalSerializedBlockSize, true );
    desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs = config.GetAttributeValueOrChildValueByName( "desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs" ).AsUInt32( desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs, true );
    vfsStorageRootPath = config.GetAttributeValueOrChildValueByName( "vfsStorageRootPath" ).AsString( vfsStorageRootPath, true );
    vfsFileExtention = config.GetAttributeValueOrChildValueByName( "vfsFileExtention" ).AsString( vfsFileExtention, true );
    encodeCodecFamily = config.GetAttributeValueOrChildValueByName( "encodeCodecFamily" ).AsString( encodeCodecFamily, true );
    encodeCodecName = config.GetAttributeValueOrChildValueByName( "encodeCodecName" ).AsString( encodeCodecName, true );
    decodeCodecFamily = config.GetAttributeValueOrChildValueByName( "decodeCodecFamily" ).AsString( decodeCodecFamily, true );
    decodeCodecName = config.GetAttributeValueOrChildValueByName( "decodeCodecName" ).AsString( decodeCodecName, true );
    ticketRefillOnBusyCycle = config.GetAttributeValueOrChildValueByName( "ticketRefillOnBusyCycle" ).AsUInt32( ticketRefillOnBusyCycle, true );
    performVfsOpsASync = config.GetAttributeValueOrChildValueByName( "performVfsOpsASync" ).AsBool( performVfsOpsASync, true );
    performVfsOpsInDedicatedThread = config.GetAttributeValueOrChildValueByName( "performVfsOpsInDedicatedThread" ).AsBool( performVfsOpsInDedicatedThread, true );
    applyDedicatedVfsOpsThreadCpuAffinity = config.GetAttributeValueOrChildValueByName( "applyDedicatedVfsOpsThreadCpuAffinity" ).AsBool( applyDedicatedVfsOpsThreadCpuAffinity, true );
    cpuAffinityForDedicatedVfsOpsThread = config.GetAttributeValueOrChildValueByName( "cpuAffinityForDedicatedVfsOpsThread" ).AsUInt32( cpuAffinityForDedicatedVfsOpsThread, true );
    collectMetrics = config.GetAttributeValueOrChildValueByName( "collectMetrics" ).AsBool( collectMetrics, true );
    mode = (TChannelMode) config.GetAttributeValueOrChildValueByName( "mode" ).AsInt32( mode, true );
    subscribeWithoutBookmarkIfNoneIsPersisted = config.GetAttributeValueOrChildValueByName( "subscribeWithoutBookmarkIfNoneIsPersisted" ).AsBool( subscribeWithoutBookmarkIfNoneIsPersisted, true );
    autoPushAfterStartupIfStorageToPubSub = config.GetAttributeValueOrChildValueByName( "autoPushAfterStartupIfStorageToPubSub" ).AsBool( autoPushAfterStartupIfStorageToPubSub, true ); 
    youngestStoragePubSubMsgFileToLoad.FromIso8601DateTimeString( config.GetAttributeValueOrChildValueByName( "youngestStoragePubSubMsgFileToLoad" ).AsString( youngestStoragePubSubMsgFileToLoad.ToIso8601DateTimeString( true, true ), true ) );
    oldestStoragePubSubMsgFileToLoad.FromIso8601DateTimeString( config.GetAttributeValueOrChildValueByName( "oldestStoragePubSubMsgFileToLoad" ).AsString( oldestStoragePubSubMsgFileToLoad.ToIso8601DateTimeString( true, true ), true ) );
    defaultCodecDecodeGrowthRatioExpectation = config.GetAttributeValueOrChildValueByName( "defaultCodecDecodeGrowthRatioExpectation" ).AsFloat32( defaultCodecDecodeGrowthRatioExpectation, true );
    bestEffortDeserializeIsAllowed = config.GetAttributeValueOrChildValueByName( "bestEffortDeserializeIsAllowed" ).AsBool( bestEffortDeserializeIsAllowed, true );

    CORE::CDataNode* binarySerializerOptionsCfg = config.FindChild( "PubSubMsgBinarySerializerOptions" );
    if ( GUCEF_NULL != binarySerializerOptionsCfg )
        success = pubsubBinarySerializerOptions.LoadConfig( *binarySerializerOptionsCfg ) && success;

    CORE::CDataNode* serializerOptionsCfg = config.FindChild( "PubSubMsgSerializerOptions" );
    if ( GUCEF_NULL != serializerOptionsCfg )
        success = pubsubSerializerOptions.LoadConfig( *serializerOptionsCfg ) && success;

    return success;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopicConfig::SaveCustomConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    bool success = true;
    success = config.SetAttribute( "desiredMinimalSerializedBlockSize", desiredMinimalSerializedBlockSize ) && success;
    success = config.SetAttribute( "desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs", desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs ) && success;
    success = config.SetAttribute( "vfsStorageRootPath", vfsStorageRootPath ) && success;
    success = config.SetAttribute( "vfsFileExtention", vfsFileExtention ) && success;
    success = config.SetAttribute( "encodeCodecFamily", encodeCodecFamily ) && success;
    success = config.SetAttribute( "encodeCodecName", encodeCodecName ) && success;
    success = config.SetAttribute( "decodeCodecFamily", decodeCodecFamily ) && success;
    success = config.SetAttribute( "decodeCodecName", decodeCodecName ) && success;
    success = config.SetAttribute( "ticketRefillOnBusyCycle", ticketRefillOnBusyCycle ) && success;
    success = config.SetAttribute( "performVfsOpsASync", performVfsOpsASync ) && success;
    success = config.SetAttribute( "performVfsOpsInDedicatedThread", performVfsOpsInDedicatedThread ) && success;
    success = config.SetAttribute( "applyDedicatedVfsOpsThreadCpuAffinity", applyDedicatedVfsOpsThreadCpuAffinity ) && success;
    success = config.SetAttribute( "cpuAffinityForDedicatedVfsOpsThread", cpuAffinityForDedicatedVfsOpsThread ) && success;
    success = config.SetAttribute( "collectMetrics", collectMetrics ) && success;
    success = config.SetAttribute( "mode", mode ) && success;
    success = config.SetAttribute( "subscribeWithoutBookmarkIfNoneIsPersisted", subscribeWithoutBookmarkIfNoneIsPersisted ) && success;
    success = config.SetAttribute( "autoPushAfterStartupIfStorageToPubSub", autoPushAfterStartupIfStorageToPubSub ) && success;
    success = config.SetAttribute( "youngestStoragePubSubMsgFileToLoad", youngestStoragePubSubMsgFileToLoad.ToIso8601DateTimeString( true, true ) ) && success;
    success = config.SetAttribute( "oldestStoragePubSubMsgFileToLoad", oldestStoragePubSubMsgFileToLoad.ToIso8601DateTimeString( true, true ) ) && success;
    success = config.SetAttribute( "defaultCodecDecodeGrowthRatioExpectation", defaultCodecDecodeGrowthRatioExpectation ) && success;    
    success = config.SetAttribute( "bestEffortDeserializeIsAllowed", bestEffortDeserializeIsAllowed ) && success;    
    
    CORE::CDataNode* binarySerializerOptionsCfg = config.AddChild( "PubSubMsgBinarySerializerOptions" );
    if ( GUCEF_NULL != binarySerializerOptionsCfg )
        success = pubsubBinarySerializerOptions.SaveConfig( *binarySerializerOptionsCfg ) && success;

    CORE::CDataNode* serializerOptionsCfg = config.AddChild( "PubSubMsgSerializerOptions" );
    if ( GUCEF_NULL != serializerOptionsCfg )
        success = pubsubSerializerOptions.SaveConfig( *serializerOptionsCfg ) && success;

    return success;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig& 
CStoragePubSubClientTopicConfig::operator=( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );


        LoadCustomConfig( src.customConfig );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig& 
CStoragePubSubClientTopicConfig::operator=( const CStoragePubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );

        pubsubBinarySerializerOptions = src.pubsubBinarySerializerOptions;
        pubsubSerializerOptions = src.pubsubSerializerOptions;
        desiredMinimalSerializedBlockSize = src.desiredMinimalSerializedBlockSize;
        desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs = src.desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs;
        vfsStorageRootPath = src.vfsStorageRootPath;
        vfsFileExtention = src.vfsFileExtention;
        encodeCodecFamily = src.encodeCodecFamily;
        encodeCodecName = src.encodeCodecName;
        decodeCodecFamily = src.decodeCodecFamily;
        decodeCodecName = src.decodeCodecName;
        ticketRefillOnBusyCycle = src.ticketRefillOnBusyCycle;
        performVfsOpsASync = src.performVfsOpsASync;
        performVfsOpsInDedicatedThread = src.performVfsOpsInDedicatedThread;
        applyDedicatedVfsOpsThreadCpuAffinity = src.applyDedicatedVfsOpsThreadCpuAffinity;
        cpuAffinityForDedicatedVfsOpsThread = src.cpuAffinityForDedicatedVfsOpsThread;
        collectMetrics = src.collectMetrics;
        mode = src.mode;
        subscribeWithoutBookmarkIfNoneIsPersisted = src.subscribeWithoutBookmarkIfNoneIsPersisted;
        autoPushAfterStartupIfStorageToPubSub = src.autoPushAfterStartupIfStorageToPubSub;
        youngestStoragePubSubMsgFileToLoad = src.youngestStoragePubSubMsgFileToLoad;
        oldestStoragePubSubMsgFileToLoad = src.oldestStoragePubSubMsgFileToLoad;
        defaultCodecDecodeGrowthRatioExpectation = src.defaultCodecDecodeGrowthRatioExpectation;
        bestEffortDeserializeIsAllowed = src.bestEffortDeserializeIsAllowed;

        customConfig = src.customConfig;
    }
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace STORAGE */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
