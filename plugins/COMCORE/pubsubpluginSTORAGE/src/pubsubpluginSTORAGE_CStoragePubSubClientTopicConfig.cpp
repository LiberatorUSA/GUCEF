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
    : COMCORE::CPubSubClientTopicConfig()
    , pubsubBinarySerializerOptions()
    , desiredMinimalSerializedBlockSize( GUCEF_DEFAULT_MINIMAL_PUBSUB_BLOCK_STORAGE_SIZE_IN_BYTES )
    , desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs( GUCEF_DEFAULT_MAXIMAL_PUBSUB_BLOCK_STORE_GROW_DELAY_IN_MS )
    , vfsStorageRootPath()
    , vfsFileExtention()
    , encodeCodecFamily()
    , encodeCodecName()
    , decodeCodecFamily()
    , decodeCodecName()
    , channelId( -1 )
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , performPubSubInDedicatedThread( true )
    , applyThreadCpuAffinity( false )
    , cpuAffinityForDedicatedPubSubThread( 0 )
    , cpuAffinityForMainChannelThread( 0 )
    , collectMetrics( true )
    , mode( CHANNELMODE_PUBSUB_TO_STORAGE )
    , subscribeWithoutBookmarkIfNoneIsPersisted( true )
    , autoPushAfterStartupIfStorageToPubSub( true )
    , youngestStoragePubSubMsgFileToLoad( CORE::CDateTime::FutureMax )
    , oldestStoragePubSubMsgFileToLoad( CORE::CDateTime::PastMax )  
    , defaultCodecDecodeGrowthRatioExpectation( GUCEF_DEFAULT_DECODE_GROWTH_RATIO_EXPECTATION )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig::CStoragePubSubClientTopicConfig( const CStoragePubSubClientTopicConfig& src )
    : COMCORE::CPubSubClientTopicConfig( src )
    , pubsubBinarySerializerOptions( src.pubsubBinarySerializerOptions )
    , desiredMinimalSerializedBlockSize( src.desiredMinimalSerializedBlockSize )
    , desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs( src.desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs )
    , vfsStorageRootPath( src.vfsStorageRootPath )
    , vfsFileExtention( src.vfsFileExtention )
    , encodeCodecFamily( src.encodeCodecFamily )
    , encodeCodecName( src.encodeCodecName )
    , decodeCodecFamily( src.decodeCodecFamily )
    , decodeCodecName( src.decodeCodecName )
    , channelId( src.channelId )
    , ticketRefillOnBusyCycle( src.ticketRefillOnBusyCycle )
    , performPubSubInDedicatedThread( src.performPubSubInDedicatedThread )
    , applyThreadCpuAffinity( src.applyThreadCpuAffinity )
    , cpuAffinityForDedicatedPubSubThread( src.cpuAffinityForDedicatedPubSubThread )
    , cpuAffinityForMainChannelThread( src.cpuAffinityForMainChannelThread )
    , collectMetrics( src.collectMetrics )
    , mode( src.mode )
    , subscribeWithoutBookmarkIfNoneIsPersisted( src.subscribeWithoutBookmarkIfNoneIsPersisted )
    , autoPushAfterStartupIfStorageToPubSub( src.autoPushAfterStartupIfStorageToPubSub )
    , youngestStoragePubSubMsgFileToLoad( src.youngestStoragePubSubMsgFileToLoad )
    , oldestStoragePubSubMsgFileToLoad( src.oldestStoragePubSubMsgFileToLoad )
    , defaultCodecDecodeGrowthRatioExpectation( src.defaultCodecDecodeGrowthRatioExpectation )
{GUCEF_TRACE;
    
    customConfig = src.customConfig;  
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig::CStoragePubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig )
    : COMCORE::CPubSubClientTopicConfig( genericConfig )
    , pubsubBinarySerializerOptions()
    , desiredMinimalSerializedBlockSize( GUCEF_DEFAULT_MINIMAL_PUBSUB_BLOCK_STORAGE_SIZE_IN_BYTES )
    , desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs( GUCEF_DEFAULT_MAXIMAL_PUBSUB_BLOCK_STORE_GROW_DELAY_IN_MS )
    , vfsStorageRootPath()
    , vfsFileExtention()
    , encodeCodecFamily()
    , encodeCodecName()
    , decodeCodecFamily()
    , decodeCodecName()
    , channelId( -1 )
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , performPubSubInDedicatedThread( true )
    , applyThreadCpuAffinity( false )
    , cpuAffinityForDedicatedPubSubThread( 0 )
    , cpuAffinityForMainChannelThread( 0 )
    , collectMetrics( true )
    , mode( CHANNELMODE_PUBSUB_TO_STORAGE )
    , subscribeWithoutBookmarkIfNoneIsPersisted( true )
    , autoPushAfterStartupIfStorageToPubSub( true )
    , youngestStoragePubSubMsgFileToLoad( CORE::CDateTime::FutureMax )
    , oldestStoragePubSubMsgFileToLoad( CORE::CDateTime::PastMax )  
    , defaultCodecDecodeGrowthRatioExpectation( GUCEF_DEFAULT_DECODE_GROWTH_RATIO_EXPECTATION )
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
    
    desiredMinimalSerializedBlockSize = config.GetAttributeValueOrChildValueByName( "desiredMinimalSerializedBlockSize" ).AsUInt32( desiredMinimalSerializedBlockSize, true );
    desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs = config.GetAttributeValueOrChildValueByName( "desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs" ).AsUInt32( desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs, true );
    vfsStorageRootPath = config.GetAttributeValueOrChildValueByName( "vfsStorageRootPath" ).AsString( vfsStorageRootPath, true );
    vfsFileExtention = config.GetAttributeValueOrChildValueByName( "vfsFileExtention" ).AsString( vfsFileExtention, true );
    encodeCodecFamily = config.GetAttributeValueOrChildValueByName( "encodeCodecFamily" ).AsString( encodeCodecFamily, true );
    encodeCodecName = config.GetAttributeValueOrChildValueByName( "encodeCodecName" ).AsString( encodeCodecName, true );
    decodeCodecFamily = config.GetAttributeValueOrChildValueByName( "decodeCodecFamily" ).AsString( decodeCodecFamily, true );
    decodeCodecName = config.GetAttributeValueOrChildValueByName( "decodeCodecName" ).AsString( decodeCodecName, true );
    channelId = config.GetAttributeValueOrChildValueByName( "channelId" ).AsInt32( channelId, true );
    ticketRefillOnBusyCycle = config.GetAttributeValueOrChildValueByName( "ticketRefillOnBusyCycle" ).AsUInt32( ticketRefillOnBusyCycle, true );
    performPubSubInDedicatedThread = config.GetAttributeValueOrChildValueByName( "performPubSubInDedicatedThread" ).AsBool( performPubSubInDedicatedThread, true );
    applyThreadCpuAffinity = config.GetAttributeValueOrChildValueByName( "applyThreadCpuAffinity" ).AsBool( applyThreadCpuAffinity, true );
    cpuAffinityForDedicatedPubSubThread = config.GetAttributeValueOrChildValueByName( "cpuAffinityForDedicatedPubSubThread" ).AsUInt32( cpuAffinityForDedicatedPubSubThread, true );
    cpuAffinityForMainChannelThread = config.GetAttributeValueOrChildValueByName( "cpuAffinityForMainChannelThread" ).AsUInt32( cpuAffinityForMainChannelThread, true );
    collectMetrics = config.GetAttributeValueOrChildValueByName( "collectMetrics" ).AsBool( collectMetrics, true );
    mode = (TChannelMode) config.GetAttributeValueOrChildValueByName( "mode" ).AsInt32( mode, true );
    subscribeWithoutBookmarkIfNoneIsPersisted = config.GetAttributeValueOrChildValueByName( "subscribeWithoutBookmarkIfNoneIsPersisted" ).AsBool( subscribeWithoutBookmarkIfNoneIsPersisted, true );
    autoPushAfterStartupIfStorageToPubSub = config.GetAttributeValueOrChildValueByName( "autoPushAfterStartupIfStorageToPubSub" ).AsBool( autoPushAfterStartupIfStorageToPubSub, true ); 
    youngestStoragePubSubMsgFileToLoad.FromIso8601DateTimeString( config.GetAttributeValueOrChildValueByName( "youngestStoragePubSubMsgFileToLoad" ).AsString( youngestStoragePubSubMsgFileToLoad.ToIso8601DateTimeString( true, true ), true ) );
    oldestStoragePubSubMsgFileToLoad.FromIso8601DateTimeString( config.GetAttributeValueOrChildValueByName( "oldestStoragePubSubMsgFileToLoad" ).AsString( oldestStoragePubSubMsgFileToLoad.ToIso8601DateTimeString( true, true ), true ) );
    defaultCodecDecodeGrowthRatioExpectation = config.GetAttributeValueOrChildValueByName( "defaultCodecDecodeGrowthRatioExpectation" ).AsFloat32( defaultCodecDecodeGrowthRatioExpectation, true );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopicConfig::SaveCustomConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    config.SetAttribute( "desiredMinimalSerializedBlockSize", desiredMinimalSerializedBlockSize );
    config.SetAttribute( "desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs", desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs );
    config.SetAttribute( "vfsStorageRootPath", vfsStorageRootPath );
    config.SetAttribute( "vfsFileExtention", vfsFileExtention );
    config.SetAttribute( "encodeCodecFamily", encodeCodecFamily );
    config.SetAttribute( "encodeCodecName", encodeCodecName );
    config.SetAttribute( "decodeCodecFamily", decodeCodecFamily );
    config.SetAttribute( "decodeCodecName", decodeCodecName );
    config.SetAttribute( "channelId", channelId );
    config.SetAttribute( "ticketRefillOnBusyCycle", ticketRefillOnBusyCycle );
    config.SetAttribute( "performPubSubInDedicatedThread", performPubSubInDedicatedThread );
    config.SetAttribute( "applyThreadCpuAffinity", applyThreadCpuAffinity );
    config.SetAttribute( "cpuAffinityForDedicatedPubSubThread", cpuAffinityForDedicatedPubSubThread );
    config.SetAttribute( "cpuAffinityForMainChannelThread", cpuAffinityForMainChannelThread );
    config.SetAttribute( "collectMetrics", collectMetrics );
    config.SetAttribute( "mode", mode );
    config.SetAttribute( "subscribeWithoutBookmarkIfNoneIsPersisted", subscribeWithoutBookmarkIfNoneIsPersisted );
    config.SetAttribute( "autoPushAfterStartupIfStorageToPubSub", autoPushAfterStartupIfStorageToPubSub );
    config.SetAttribute( "youngestStoragePubSubMsgFileToLoad", youngestStoragePubSubMsgFileToLoad.ToIso8601DateTimeString( true, true ) );
    config.SetAttribute( "oldestStoragePubSubMsgFileToLoad", oldestStoragePubSubMsgFileToLoad.ToIso8601DateTimeString( true, true ) );
    config.SetAttribute( "defaultCodecDecodeGrowthRatioExpectation", defaultCodecDecodeGrowthRatioExpectation );    

    return true;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig& 
CStoragePubSubClientTopicConfig::operator=( const COMCORE::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );


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
        COMCORE::CPubSubClientTopicConfig::operator=( src );

        pubsubBinarySerializerOptions = src.pubsubBinarySerializerOptions;
        desiredMinimalSerializedBlockSize = src.desiredMinimalSerializedBlockSize;
        desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs = src.desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs;
        vfsStorageRootPath = src.vfsStorageRootPath;
        vfsFileExtention = src.vfsFileExtention;
        encodeCodecFamily = src.encodeCodecFamily;
        encodeCodecName = src.encodeCodecName;
        decodeCodecFamily = src.decodeCodecFamily;
        decodeCodecName = src.decodeCodecName;
        channelId = src.channelId;
        ticketRefillOnBusyCycle = src.ticketRefillOnBusyCycle;
        performPubSubInDedicatedThread = src.performPubSubInDedicatedThread;
        applyThreadCpuAffinity = src.applyThreadCpuAffinity;
        cpuAffinityForDedicatedPubSubThread = src.cpuAffinityForDedicatedPubSubThread;
        cpuAffinityForMainChannelThread = src.cpuAffinityForMainChannelThread;
        collectMetrics = src.collectMetrics;
        mode = src.mode;
        subscribeWithoutBookmarkIfNoneIsPersisted = src.subscribeWithoutBookmarkIfNoneIsPersisted;
        autoPushAfterStartupIfStorageToPubSub = src.autoPushAfterStartupIfStorageToPubSub;
        youngestStoragePubSubMsgFileToLoad = src.youngestStoragePubSubMsgFileToLoad;
        oldestStoragePubSubMsgFileToLoad = src.oldestStoragePubSubMsgFileToLoad;
        defaultCodecDecodeGrowthRatioExpectation = src.defaultCodecDecodeGrowthRatioExpectation;

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
