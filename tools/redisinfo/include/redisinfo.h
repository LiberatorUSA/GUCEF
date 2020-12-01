/*
 *  redisinfo: Service for obtaining redis info from a cluster
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

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

#ifndef GUCEF_COM_CHTTPSERVER_H
#include "gucefCOM_CHTTPServer.h"
#define GUCEF_COM_CHTTPSERVER_H
#endif /* GUCEF_COM_CHTTPSERVER_H ? */

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H
#include "gucefCOM_CDefaultHTTPServerRouter.h"
#define GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H
#endif /* GUCEF_COM_CDEFAULTHTTPSERVERROUTER_H ? */

#ifndef GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefCOM_CCodecBasedHTTPServerResource.h"
#define GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_COM_CTCONFIGURABLEMAPHTTPSERVERRESOURCE_H
#include "gucefCOM_CTConfigurableMapHttpServerResource.h"
#define GUCEF_COM_CTCONFIGURABLEMAPHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CTCONFIGURABLEMAPHTTPSERVERRESOURCE_H ? */

#include "hiredis.h"
#include "async.h"

#include "redis++.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class Settings : public CORE::CIConfigurable
{
    public:

    typedef std::vector< COMCORE::CHostAddress > HostAddressVector;

    Settings( void );
    Settings( const Settings& src );
    Settings& operator=( const Settings& src );

    COMCORE::CHostAddress redisAddress;
    bool collectMetrics;
    CORE::CString metricPrefix;
    bool gatherInfoReplication;
    bool gatherInfoPersistence;
    bool gatherInfoStats;
    bool gatherInfoCommandStats;
    bool gatherInfoMemory;
    bool gatherStreamInfo;
    bool gatherInfoClients;
    bool gatherInfoCpu;
    bool gatherInfoKeyspace;
    bool gatherClusterInfo;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

class RedisNode
{
    public:

    COMCORE::CHostAddress host;
    CORE::CString nodeId;
    CORE::UInt32 startSlot;
    CORE::UInt32 endSlot;

    RedisNode& operator=( const RedisNode& other );

    RedisNode( void );
};

typedef std::map< CORE::UInt32, RedisNode > RedisNodeMap;

/*-------------------------------------------------------------------------*/

class RedisNodeWithPipe : public RedisNode
{
    public:

    sw::redis::Pipeline* redisPipe;

    RedisNodeWithPipe& operator=( const RedisNode& other );

    RedisNodeWithPipe( void );
};

typedef std::map< CORE::UInt32, RedisNodeWithPipe > RedisNodeWithPipeMap;

/*-------------------------------------------------------------------------*/

class RedisInfoService : public CORE::CTaskConsumer
{
    public:

    typedef CORE::CTEventHandlerFunctor< RedisInfoService > TEventCallback;
    typedef std::set< CORE::CString > TStringSet;
    typedef std::map< CORE::UInt32, TStringSet > TUInt32ToStringSetMap;

    static const CORE::CString RedisInfoService::HashSlotFileCodec;
    static const CORE::CString RedisInfoService::HashSlotFile;

    RedisInfoService();
    virtual ~RedisInfoService();

    virtual bool OnTaskStart( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnTaskCycle( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                               bool wasForced             ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    bool LoadConfig( const Settings& settings );

    const Settings& GetSettings( void ) const;
    
    private:

    bool RedisConnect( void );

    void RegisterEventHandlers( void );

    bool RefreshRedisNodePipes( void );

    bool GetRedisStreamInfo( struct redisReply* replyNode           ,
                             CORE::CValueList& info                 ,
                             const CORE::CString& optionalKeyPrefix ,
                             bool statLikeValuesOnly                );

    bool SerializeRedisClusterNodeMap( const RedisNodeMap& nodeMap, CORE::CDataNode& doc ) const;

    bool DeserializeKeysForHashSlots( TUInt32ToStringSetMap& hashMap, const CORE::CDataNode& doc ) const;

    bool SaveDocTo( const CORE::CDataNode& doc     , 
                    const CORE::CString& codecName ,
                    const CORE::CString& vfsPath   ) const;

    bool LoadDocFrom( CORE::CDataNode& doc           , 
                      const CORE::CString& codecName , 
                      const CORE::CString& vfsPath   ) const;

    CORE::UInt32 CalculateRedisHashSlot( const CORE::CString& keyStr ) const;

    bool CalculateKeysForAllHashSlots( TUInt32ToStringSetMap& hashMap ,
                                       CORE::UInt32 minKeysPerSlot    , 
                                       CORE::UInt32 maxKeysPerSlot    ) const;

    bool SerializeKeysForHashSlots( const TUInt32ToStringSetMap& hashMap, CORE::CDataNode& doc ) const;

    bool GetRedisClusterNodeMap( RedisNodeMap& nodeMap );

    bool LoadHashSlotMap( void );

    bool ProvideHashSlotMapDoc( void );

    bool ProvideRedisNodesDoc( void );

    bool GetRedisKeys( CORE::CString::StringVector& keys ,
                       const CORE::CString& keyType      );

    bool GetRedisKeysForNode( RedisNodeWithPipe& node           ,
                              CORE::CString::StringVector& keys ,
                              const CORE::CString& keyType      );

    bool GetRedisStreamInfo( const CORE::CString& streamName        , 
                             CORE::CValueList& info                 ,
                             const CORE::CString& optionalKeyPrefix ,
                             bool statLikeValuesOnly                );

    bool GetRedisStreamInfoForAllStreams( CORE::CValueList& info  ,
                                          bool statLikeValuesOnly );
    
    bool GetRedisInfoReplication( CORE::CValueList& kv );

    bool GetRedisInfoPersistence( CORE::CValueList& kv );

    bool GetRedisInfoStats( CORE::CValueList& kv );
    
    bool GetRedisInfoCommandStats( CORE::CValueList& kv );
    
    bool GetRedisInfoMemory( CORE::CValueList& kv );

    bool GetRedisInfoClients( CORE::CValueList& kv );

    bool GetRedisInfoCpu( CORE::CValueList& kv );

    bool GetRedisInfoKeyspace( CORE::CValueList& kv );

    bool GetRedisClusterInfo( CORE::CValueList& kv );

    bool GetRedisInfo( const CORE::CString& cmd             ,
                       const CORE::CString& type            ,  
                       CORE::CValueList& kv                 , 
                       RedisNodeWithPipe* node = GUCEF_NULL );

    bool GetRedisInfo( struct redisReply* replyNode ,
                       const CORE::CString& type    ,
                       CORE::CValueList& kv         );

    RedisNodeWithPipe* FindNodeInfo( const CORE::CString& hashable );

    RedisNodeWithPipe* FindNodeInfo( CORE::UInt32 hashSlot );

    void SendKeyValueStats( const CORE::CValueList& kv        ,
                            const CORE::CString& metricPrefix );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    private:

    RedisInfoService( const RedisInfoService& src ); // not implemented

    typedef std::vector< std::pair< sw::redis::StringView, sw::redis::StringView > > TRedisArgs;

    sw::redis::RedisCluster* m_redisContext;
    Settings m_settings;
    TRedisArgs m_redisPacketArgs;
    CORE::CTimer* m_metricsTimer;
    CORE::CString::StringVector m_redisKeys;
    RedisNodeWithPipeMap m_redisNodesMap;
    TUInt32ToStringSetMap m_hashSlotOriginStrMap;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< RedisInfoService, MT::CMutex > RedisInfoServicePtr;

/*-------------------------------------------------------------------------*/

class RedisInfo;

class RestApiRedisInfoInfoResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiRedisInfoInfoResource( RedisInfo* app );

    virtual ~RestApiRedisInfoInfoResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    RedisInfo* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiRedisInfoConfigResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiRedisInfoConfigResource( RedisInfo* app, bool appConfig );

    virtual ~RestApiRedisInfoConfigResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    virtual TDeserializeState Deserialize( const CORE::CDataNode& input        ,
                                           const CORE::CString& representation ,
                                           bool isDeltaUpdateOnly              ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    RedisInfo* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class RedisInfo : public CORE::CObserver
{
    public:

    RedisInfo( void );
    virtual ~RedisInfo();

    bool Start( void );

    bool SetStandbyMode( bool newMode );

    bool IsGlobalStandbyEnabled( void ) const;

    bool LoadConfig( const CORE::CValueList& appConfig   ,
                     const CORE::CDataNode& globalConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    private:

    typedef CORE::CTEventHandlerFunctor< RedisInfo > TEventCallback;

    private:

    bool m_isInStandby;
    bool m_globalStandbyEnabled;
    CORE::CString m_redisHost;
    CORE::UInt16 m_redisPort;
    COM::CHTTPServer m_httpServer;
    COM::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;    
    bool m_transmitMetrics;
    RedisInfoServicePtr m_infoService;
    Settings m_settings;
};

/*-------------------------------------------------------------------------*/
