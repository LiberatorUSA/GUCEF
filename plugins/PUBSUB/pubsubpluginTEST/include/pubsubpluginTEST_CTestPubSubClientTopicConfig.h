/*
 *  pubsubpluginTEST: Generic GUCEF PUBSUB plugin for adding integration/system tests
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

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgContainerBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgSerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H
#include "gucefPUBSUB_CPubSubClientTopicConfig.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef PUBSUBPLUGIN_TEST_MACROS_H
#include "pubsubpluginTEST_macros.h"
#define PUBSUBPLUGIN_TEST_MACROS_H
#endif /* PUBSUBPLUGIN_TEST_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace TEST {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CTestPubSubClient;

/**
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_TEST_PLUGIN_PRIVATE_CPP CTestPubSubClientTopicConfig : public PUBSUB::CPubSubClientTopicConfig
{
    public:

    typedef std::vector< PUBSUB::CPubSubClientTopicConfig > TTopicConfigVector;
    enum EChannelMode : CORE::Int32
    {
        CHANNELMODE_UNKNOWN = 0 ,

        CHANNELMODE_PUBSUB_TO_TEST = 1,
        CHANNELMODE_TEST_TO_PUBSUB = 2
    };
    typedef enum EChannelMode TChannelMode;

    PUBSUB::CPubSubMsgBinarySerializerOptions pubsubBinarySerializerOptions;
    PUBSUB::CPubSubMsgSerializerOptions pubsubSerializerOptions;
    CORE::UInt32 desiredNrOfBuffers;
    CORE::UInt32 desiredMinimalSerializedBlockSize;
    CORE::UInt32 desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs;    
    CORE::CString vfsStorageRootPath;
    CORE::CString vfsFileExtention;
    CORE::CString encodeCodecFamily;
    CORE::CString encodeCodecName;
    CORE::CString decodeCodecFamily;
    CORE::CString decodeCodecName;
    CORE::UInt32 ticketRefillOnBusyCycle;
    bool performVfsOpsASync;
    bool performVfsOpsInDedicatedThread;
    bool applyDedicatedVfsOpsThreadCpuAffinity;
    CORE::UInt32 cpuAffinityForDedicatedVfsOpsThread;
    bool collectMetrics;
    TChannelMode mode;
    bool subscribeWithoutBookmarkIfNoneIsPersisted;
    bool autoPushAfterStartupIfStorageToPubSub;
    CORE::CDateTime youngestStoragePubSubMsgFileToLoad;
    CORE::CDateTime oldestStoragePubSubMsgFileToLoad;
    CORE::Float32 defaultCodecDecodeGrowthRatioExpectation;
    bool bestEffortDeserializeIsAllowed;
    CORE::Int32 maxStorageCorruptionDetectionsToBeHealthy;
    CORE::Int32 maxStorageDeserializationFailuresToBeHealthy;



    bool defaultIsHealthyStatus;
    bool toggleHealthyStatus;
    CORE::UInt32 healthyStatusToggleIntervalInMs;
    CORE::UInt32 healthyStatusToggleIntervalJitter;




    CTestPubSubClientTopicConfig( void );

    CTestPubSubClientTopicConfig( const CTestPubSubClientTopicConfig& src );
    
    CTestPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig );

    virtual ~CTestPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CTestPubSubClientTopicConfig& operator=( const PUBSUB::CPubSubClientTopicConfig& src );

    CTestPubSubClientTopicConfig& operator=( const CTestPubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );

    bool SaveCustomConfig( CORE::CDataNode& config ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace TEST */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H ? */
