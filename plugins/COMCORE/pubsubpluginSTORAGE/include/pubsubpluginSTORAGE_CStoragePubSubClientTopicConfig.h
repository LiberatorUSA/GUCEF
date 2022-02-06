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

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefCOMCORE_CPubSubMsgBinarySerializer.h"
#define GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#include "gucefCOMCORE_CPubSubMsgContainerBinarySerializer.h"
#define GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#endif /* GUCEF_COMCORE_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H
#include "gucefCOMCORE_CPubSubClientTopicConfig.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef PUBSUBPLUGIN_STORAGE_MACROS_H
#include "pubsubpluginSTORAGE_macros.h"
#define PUBSUBPLUGIN_STORAGE_MACROS_H
#endif /* PUBSUBPLUGIN_STORAGE_MACROS_H ? */

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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CStoragePubSubClient;

/**
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_STORAGE_PLUGIN_PRIVATE_CPP CStoragePubSubClientTopicConfig : public COMCORE::CPubSubClientTopicConfig
{
    public:

    typedef std::vector< COMCORE::CPubSubClientTopicConfig > TTopicConfigVector;
    enum EChannelMode : CORE::Int32
    {
        CHANNELMODE_UNKNOWN = 0 ,

        CHANNELMODE_PUBSUB_TO_STORAGE = 1,
        CHANNELMODE_STORAGE_TO_PUBSUB = 2
    };
    typedef enum EChannelMode TChannelMode;

    COMCORE::CPubSubMsgBinarySerializerOptions pubsubBinarySerializerOptions;
    CORE::UInt32 desiredMinimalSerializedBlockSize;
    CORE::UInt32 desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs;    
    CORE::CString vfsStorageRootPath;
    CORE::CString vfsFileExtention;
    CORE::CString encodeCodecFamily;
    CORE::CString encodeCodecName;
    CORE::CString decodeCodecFamily;
    CORE::CString decodeCodecName;
    CORE::Int32 channelId;
    CORE::UInt32 ticketRefillOnBusyCycle;
    bool performPubSubInDedicatedThread;
    bool applyThreadCpuAffinity;
    CORE::UInt32 cpuAffinityForDedicatedPubSubThread;
    CORE::UInt32 cpuAffinityForMainChannelThread;
    bool collectMetrics;
    TChannelMode mode;
    bool subscribeWithoutBookmarkIfNoneIsPersisted;
    bool autoPushAfterStartupIfStorageToPubSub;
    CORE::CDateTime youngestStoragePubSubMsgFileToLoad;
    CORE::CDateTime oldestStoragePubSubMsgFileToLoad;
    CORE::Float32 defaultCodecDecodeGrowthRatioExpectation;

    CStoragePubSubClientTopicConfig( void );

    CStoragePubSubClientTopicConfig( const CStoragePubSubClientTopicConfig& src );
    
    CStoragePubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig );

    virtual ~CStoragePubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CStoragePubSubClientTopicConfig& operator=( const COMCORE::CPubSubClientTopicConfig& src );

    CStoragePubSubClientTopicConfig& operator=( const CStoragePubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );

    bool SaveCustomConfig( CORE::CDataNode& config ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace STORAGE */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENTTOPICCONFIG_H ? */
