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
class PUBSUBPLUGIN_REDISCLUSTER_PLUGIN_PRIVATE_CPP CRedisClusterPubSubClientTopicConfig : public PUBSUB::CPubSubClientTopicConfig ,
                                                                                          public CORE::CTSharedObjCreator< CRedisClusterPubSubClientTopicConfig, MT::CMutex >
{
    public:

    bool autoGenerateRedisAddMinId;                   /**< whether to auto generate the min id for the XADD command. This is a Unix epoch timestamp of the publish time at the node */
    CORE::UInt64 maxAgeInMsForMinId;                  /**< the max age of a message in the stream in milliseconds based on current system time. Used when autoGenerateRedisAddMinId is true */
    bool redisXAddMaxAgeIsApproximate;                /**< whether the max age is approximate or exact thus resulting in a ~ or = passed to redis. exact has a performance penalty for Redis */
    CORE::Int32 redisXAddMaxLen;                      /**< the max len of the stream. A value smaller than 0 means maxlen is not used */
    bool redisXAddMaxLenIsApproximate;                /**< whether the max len is approximate or exact thus resulting in a ~ or = passed to redis. exact has a performance penalty for Redis */
    bool redisXAddIgnoreMsgId;                        /**< whether to ignore the msg id on the message object and instead have the Redis node generate the key which is a Unix epoch timestamp of the publish time at the node */
    CORE::CString redisXReadDefaultOffset;
    CORE::Int32 redisXReadCount;
    CORE::UInt32 redisXReadBlockTimeoutInMs;
    bool treatXReadBlockTimeoutAsEndOfDataEvent;
    CORE::Int32 minAvailableInFlightSlotsBeforeRead;  /**< if a max nr of msgs in flight is being enforced then this controls the min free slots there have to be before the overhead of another read attemps */
    bool useDedicatedReadThread;                      /**< whether to use a dedicated thread to perform reads from Redis. Can be used in combo with the blocking settings for long poll */

    CRedisClusterPubSubClientTopicConfig( void );
    
    CRedisClusterPubSubClientTopicConfig( const CRedisClusterPubSubClientTopicConfig& src );

    CRedisClusterPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig );

    virtual ~CRedisClusterPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CRedisClusterPubSubClientTopicConfig& operator=( const PUBSUB::CPubSubClientTopicConfig& src );

    CRedisClusterPubSubClientTopicConfig& operator=( const CRedisClusterPubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );

    bool LoadConfig( const PUBSUB::CPubSubClientTopicConfig& src );

    virtual CORE::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*--------------------------------------------------------------------------*/

typedef CRedisClusterPubSubClientTopicConfig::TSharedPtrType    CRedisClusterPubSubClientTopicConfigPtr;

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
