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

#ifndef PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H
#define PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <deque>

#ifndef GUCEF_MT_CTMAILBOX_H
#include "gucefMT_CTMailBox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_CORE_CTHREADPOOL_H
#include "gucefCORE_CThreadPool.h"
#define GUCEF_CORE_CTHREADPOOL_H
#endif /* GUCEF_CORE_CTHREADPOOL_H ? */

#ifndef GUCEF_COMCORE_CIPUBSUBMSG_H
#include "gucefCOMCORE_CIPubSubMsg.h"
#define GUCEF_COMCORE_CIPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CIPUBSUBMSG_H ? */

#ifndef GUCEF_COMCORE_CBASICPUBSUBMSG_H
#include "gucefCOMCORE_CBasicPubSubMsg.h"
#define GUCEF_COMCORE_CBASICPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CBASICPUBSUBMSG_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENT_H
#include "gucefCOMCORE_CPubSubClient.h"
#define GUCEF_COMCORE_CPUBSUBCLIENT_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#include "gucefCOMCORE_CPubSubClientFactory.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H ? */

#include "hiredis.h"
#include "async.h"

#include "redis++.h"

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

class RedisNode
{
    public:

    COMCORE::CHostAddress host;
    CORE::CString nodeId;
    CORE::UInt32 startSlot;
    CORE::UInt32 endSlot;

    RedisNode( void );
};

typedef std::map< CORE::UInt32, RedisNode > RedisNodeMap;

/*-------------------------------------------------------------------------*/

class CRedisClusterPubSubClient;

class CRedisClusterPubSubClientTopicConfig : public COMCORE::CPubSubClientTopicConfig
{
    public:

    CORE::Int32 redisXAddMaxLen;
    bool redisXAddMaxLenIsApproximate;
    
    CRedisClusterPubSubClientTopicConfig( void );
    
    CRedisClusterPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig );

    virtual ~CRedisClusterPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CRedisClusterPubSubClientTopicConfig& operator=( const COMCORE::CPubSubClientTopicConfig& src );

    CRedisClusterPubSubClientTopicConfig& operator=( const CRedisClusterPubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );
};

/*-------------------------------------------------------------------------*/

class CRedisClusterPubSubClientTopic;

class CRedisClusterPubSubClientTopicReader : public CORE::CTaskConsumer
{
    public:

    CRedisClusterPubSubClientTopicReader( CRedisClusterPubSubClientTopic* ownerTopic );
    virtual ~CRedisClusterPubSubClientTopicReader() GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskStart( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                               bool wasForced             ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    CRedisClusterPubSubClientTopicReader( void );                                            // not implemented
    CRedisClusterPubSubClientTopicReader( const CRedisClusterPubSubClientTopicReader& src ); // not implemented

    CRedisClusterPubSubClientTopic* m_ownerTopic;
};

typedef CORE::CTSharedPtr< CRedisClusterPubSubClientTopicReader, MT::CMutex >   RedisClusterPubSubClientTopicReaderPtr;

/*-------------------------------------------------------------------------*/

class CRedisClusterPubSubClientTopic : public COMCORE::CPubSubClientTopic
{
    public:

    CRedisClusterPubSubClientTopic( CRedisClusterPubSubClient* client );

    virtual ~CRedisClusterPubSubClientTopic() GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Connect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsPublishingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsSubscribingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetTopicName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Publish( const COMCORE::CIPubSubMsg& msg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const;

    virtual bool LoadConfig( const COMCORE::CPubSubClientTopicConfig& config );

    bool RedisSendSyncImpl( const sw::redis::StringView& msgId );

    bool RedisRead( void );

    CORE::UInt32 CalculateRedisHashSlot( const CORE::CString& keyStr ) const;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnRedisReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData );
    
    private:

    void RegisterEventHandlers( void );

    void PrepStorageForReadMsgs( CORE::UInt32 msgCount );

    private:

    typedef CORE::CTEventHandlerFunctor< CRedisClusterPubSubClientTopic > TEventCallback;
    
    // Types to read from redis-plus-plus
    typedef std::vector< std::pair< sw::redis::StringView, sw::redis::StringView > > TRedisArgs;
    typedef std::pair< std::string, std::string > TRedisMsgAttribute;
    typedef std::vector< TRedisMsgAttribute > TRedisMsgAttributes;
    typedef std::pair< std::string, TRedisMsgAttributes > TRedisMsg;
    typedef std::vector< TRedisMsg > TRedisMsgVector;
    typedef std::unordered_map< std::string, TRedisMsgVector > TRedisMsgByStream;
    typedef std::insert_iterator< TRedisMsgByStream > TRedisMsgByStreamInserter;

    // Types to implement/hook-up topic interface
    typedef std::vector< COMCORE::CBasicPubSubMsg > TPubSubMsgsVector;
    typedef std::pair< CORE::CDynamicBuffer, CORE::CDynamicBuffer > TBufferPair;
    typedef std::vector< TBufferPair > TBufferVector;

    CRedisClusterPubSubClient* m_client;
    sw::redis::Pipeline* m_redisPipeline;
    sw::redis::RedisCluster* m_redisContext;
    CORE::UInt32 m_redisErrorReplies;
    CORE::UInt32 m_redisTransmitQueueSize;
    CORE::UInt32 m_redisMsgsTransmitted;
    CORE::UInt32 m_redisFieldsInMsgsTransmitted;
    CORE::UInt32 m_redisFieldsInMsgsRatio;
    CORE::UInt32 m_redisHashSlot;
    CORE::UInt32 m_redisXreadCount;
    CORE::UInt32 m_redisXreadBlockTimeoutInMs;
    TRedisMsgByStream m_redisReadMsgs;
    TPubSubMsgsVector m_pubsubMsgs;
    TMsgsRecievedEventData m_pubsubMsgsRefs;
    TBufferVector m_pubsubMsgAttribs;
    TRedisArgs m_redisMsgArgs;
    COMCORE::CHostAddress m_redisShardHost;
    CORE::CString m_redisShardNodeId;
    CORE::CTimer* m_redisReconnectTimer;
    CRedisClusterPubSubClientTopicConfig m_config;
    std::string m_readOffset;    
    RedisClusterPubSubClientTopicReaderPtr m_readerThread;
};

/*-------------------------------------------------------------------------*/

class CRedisClusterPubSubClient : public COMCORE::CPubSubClient
{
    public:

    static const CORE::CString TypeName; 

    CRedisClusterPubSubClient( const COMCORE::CPubSubClientConfig& config );

    virtual ~CRedisClusterPubSubClient() GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetSupportedFeatures( COMCORE::CPubSubClientFeatures& features ) GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubClientTopic* CreateTopicAccess( const COMCORE::CPubSubClientTopicConfig& topicConfig ) GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubClientTopic* GetTopicAccess( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void DestroyTopicAccess( const CORE::CString& topicName ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Connect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;

    COMCORE::CPubSubClientConfig& GetConfig( void );

    CORE::ThreadPoolPtr GetThreadPool( void );
    
    const RedisNodeMap& GetRedisNodeMap( void ) const;

    sw::redis::RedisCluster* GetRedisContext( void ) const;

    private:

    bool GetRedisClusterNodeMap( RedisNodeMap& nodeMap );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnRedisReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData );

    void RegisterEventHandlers( void );
    
    CRedisClusterPubSubClient( void ); /**< not implemented */

    private:

    typedef CORE::CTEventHandlerFunctor< CRedisClusterPubSubClient > TEventCallback;
    typedef std::map< CORE::CString, CRedisClusterPubSubClientTopic* > TTopicMap;

    COMCORE::CPubSubClientConfig m_config;
    RedisNodeMap m_nodeMap;
    sw::redis::RedisCluster* m_redisContext;
    CORE::UInt32 m_redisErrorReplies;
    CORE::CTimer* m_metricsTimer;
    CORE::CTimer* m_redisReconnectTimer;
    TTopicMap m_topicMap;
    CORE::ThreadPoolPtr m_threadPool;
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

#endif /* PUBSUBPLUGIN_REDISCLUSTER_CREDISCLUSTERPUBSUBCLIENT_H ? */
