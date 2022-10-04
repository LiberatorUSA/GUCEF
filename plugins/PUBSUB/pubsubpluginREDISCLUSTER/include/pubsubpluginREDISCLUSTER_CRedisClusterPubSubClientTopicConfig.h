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

#ifndef PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENTTOPICCONFIG_H
#define PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENTTOPICCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H
#include "gucefPUBSUB_CPubSubClientTopicConfig.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef PUBSUBPLUGIN_REDISCLUSTER_MACROS_H
#include "pubsubpluginREDISCLUSTER_macros.h"
#define PUBSUBPLUGIN_REDISCLUSTER_MACROS_H
#endif /* PUBSUBPLUGIN_REDISCLUSTER_MACROS_H ? */

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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CRedisClusterPubSubClient;

/**
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_REDISCLUSTER_PLUGIN_PRIVATE_CPP CRedisClusterPubSubClientTopicConfig : public PUBSUB::CPubSubClientTopicConfig
{
    public:

    CORE::Int32 redisXAddMaxLen;
    bool redisXAddMaxLenIsApproximate;
    bool redisXAddIgnoreMsgId;
    CORE::CString redisXReadDefaultOffset;
    CORE::Int32 redisXReadCount;
    CORE::UInt32 redisXReadBlockTimeoutInMs;
    bool treatXReadBlockTimeoutAsEndOfDataEvent;
    CORE::UInt32 minAvailableInFlightSlotsBeforeRead;  /**< if a max nr of msgs in flight is being enforced then this controls the min free slots there have to be before the overhead of another read attemps */
    
    CRedisClusterPubSubClientTopicConfig( void );
    
    CRedisClusterPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig );

    virtual ~CRedisClusterPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CRedisClusterPubSubClientTopicConfig& operator=( const PUBSUB::CPubSubClientTopicConfig& src );

    CRedisClusterPubSubClientTopicConfig& operator=( const CRedisClusterPubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace REDISCLUSTER */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENTTOPICCONFIG_H ? */
