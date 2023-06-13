/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
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

#ifndef GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H
#define GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#include "gucefCORE_CDynamicBufferSwap.h"
#define GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERSWAP_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENT_H
#include "gucefPUBSUB_CPubSubClient.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENT_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H
#include "gucefPUBSUB_CPubSubClientFactory.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTFACTORY_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgContainerBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class that conveys config for a pub sub channel's side
 */
class GUCEF_PUBSUB_EXPORT_CPP CPubSubSideChannelConfig : public CORE::CIConfigurable
{
    public:

    CPubSubSideChannelConfig( void );
    CPubSubSideChannelConfig( const CPubSubSideChannelConfig& src );
    virtual ~CPubSubSideChannelConfig();
    CPubSubSideChannelConfig& operator=( const CPubSubSideChannelConfig& src );    

    CPubSubClientConfig pubsubClientConfig;
    CPubSubBookmarkPersistenceConfig pubsubBookmarkPersistenceConfig;
    bool performPubSubInDedicatedThread;
    bool useBackendTopicThreadForTopicIfAvailable;
    bool applyThreadCpuAffinity;
    CORE::UInt32 cpuAffinityForPubSubThread;
    bool subscribeWithoutBookmarkIfNoneIsPersisted;
    bool treatPublishWithoutTargetTopicAsBroadcast;
    bool retryFailedPublishAttempts;
    bool allowOutOfOrderPublishRetry;
    CORE::Int32 maxMsgPublishRetryAttempts;
    CORE::Int32 maxMsgPublishRetryTotalTimeInMs;
    CORE::Int32 maxPublishedMsgInFlightTimeInMs;
    bool allowTimedOutPublishedInFlightMsgsRetryOutOfOrder;
    CORE::Int32 maxMsgPublishAckRetryAttempts;
    CORE::Int32 maxMsgPublishAckRetryTotalTimeInMs;
    CORE::UInt32 ticketRefillOnBusyCycle;
    bool collectMetrics;                                                   
    CORE::UInt32 metricsIntervalInMs;
    CORE::CString pubsubIdPrefix;

    CPubSubClientTopicConfigPtr GetTopicConfig( const CORE::CString& topicName );

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

/**
 *  Class that conveys runtime relevant settings and config for a pub sub channel's side
 */
class GUCEF_PUBSUB_EXPORT_CPP CPubSubSideChannelSettings : public CPubSubSideChannelConfig
{
    public:

    CPubSubSideChannelSettings( void );
    CPubSubSideChannelSettings( const CPubSubSideChannelSettings& src );
    virtual ~CPubSubSideChannelSettings();
    CPubSubSideChannelSettings& operator=( const CPubSubSideChannelSettings& src );
    
    bool needToTrackInFlightPublishedMsgsForAck;       //< this setting is derived and cached from other settings 
    CORE::CString metricsPrefix;                       //< this setting is derived and cached from other settings   

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H ? */

