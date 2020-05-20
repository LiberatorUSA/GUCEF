/*
 *  Udp2RedisCluster: service which pushes UDP packets into kafka topics
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

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class Udp2RedisClusterChannel : public CORE::CTaskConsumer
{
    public:

    typedef CORE::CTEventHandlerFunctor< Udp2RedisClusterChannel > TEventCallback;

    Udp2RedisClusterChannel();
    Udp2RedisClusterChannel( const Udp2RedisClusterChannel& src );
    virtual ~Udp2RedisClusterChannel();

    virtual bool OnTaskStart( CORE::CICloneable* taskData );
    
    virtual bool OnTaskCycle( CORE::CICloneable* taskData );
    
    virtual void OnTaskEnd( CORE::CICloneable* taskData );

    virtual CORE::CString GetType( void ) const;

    class ChannelSettings
    {
        public:

        typedef std::vector< COMCORE::CHostAddress > HostAddressVector;

        ChannelSettings( void );
        ChannelSettings( const ChannelSettings& src );
        ChannelSettings& operator=( const ChannelSettings& src );

        COMCORE::CHostAddress redisAddress;
        CORE::CString channelStreamName;
        COMCORE::CHostAddress udpInterface;
        HostAddressVector udpMulticastToJoin;
    };

    bool LoadConfig( const ChannelSettings& channelSettings );

    class ChannelMetrics
    {
        public:

        ChannelMetrics( void );

        CORE::UInt32 udpBytesReceived;
        CORE::UInt32 udpMessagesReceived;
        CORE::UInt32 redisMessagesTransmitted;
        CORE::UInt32 redisTransmitOverflowQueueSize;
        CORE::UInt32 redisErrorReplies;
    };

    const ChannelMetrics& GetMetrics( void ) const;
    
    private:

    void
    OnUDPSocketError( CORE::CNotifier* notifier   ,
                      const CORE::CEvent& eventID ,
                      CORE::CICloneable* evenData );

    void
    OnUDPSocketClosed( CORE::CNotifier* notifier   ,
                       const CORE::CEvent& eventID ,
                       CORE::CICloneable* evenData );

    void
    OnUDPSocketOpened( CORE::CNotifier* notifier   ,
                       const CORE::CEvent& eventID ,
                       CORE::CICloneable* evenData );

    void
    OnUDPPacketRecieved( CORE::CNotifier* notifier   ,
                         const CORE::CEvent& eventID ,
                         CORE::CICloneable* evenData );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    CORE::UInt32 GetRedisErrorRepliesCounter( bool resetCounter );

    private:

    void RegisterEventHandlers( void );
    
    int RedisSend( const CORE::CDynamicBuffer& udpPacket );

    bool SendQueuedPackagesIfAny( void );

    bool RedisConnect( void );

    private:

    typedef std::deque< CORE::CDynamicBuffer > TDynamicBufferQueue;
    typedef std::vector< redisAsyncContext* > redisAsyncContextVector;

    ChannelSettings m_channelSettings;   
    CORE::CString m_redisStreamSendCmd;
    sw::redis::Redis* m_redisContext;
    GUCEF::COMCORE::CUDPSocket* m_udpSocket;
    TDynamicBufferQueue m_redisMsgQueueOverflowQueue;
    redisOptions m_redisOptions;
    bool m_redisReadFlag;
    bool m_redisWriteFlag;
    bool m_redisTimeoutFlag;
    CORE::CTimer* m_metricsTimer;
    ChannelMetrics m_metrics;
    CORE::UInt32 m_redisErrorReplies;
};

/*-------------------------------------------------------------------------*/

class Udp2RedisCluster;

class RestApiUdp2RedisInfoResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiUdp2RedisInfoResource( Udp2RedisCluster* app );

    virtual ~RestApiUdp2RedisInfoResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    Udp2RedisCluster* m_app;
};

class RestApiUdp2RedisConfigResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiUdp2RedisConfigResource( Udp2RedisCluster* app, bool appConfig );

    virtual ~RestApiUdp2RedisConfigResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    Udp2RedisCluster* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class Udp2RedisCluster : public CORE::CObserver
{
    public:

    Udp2RedisCluster( void );
    virtual ~Udp2RedisCluster();

    bool Start( void );

    bool LoadConfig( const CORE::CValueList& appConfig   ,
                     const CORE::CDataNode& globalConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    private:
    
    typedef CORE::CTEventHandlerFunctor< Udp2RedisCluster > TEventCallback;
    
    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );    

    private:

    typedef std::map< CORE::Int32, Udp2RedisClusterChannel::ChannelSettings > ChannelSettingsMap;
    typedef std::vector< Udp2RedisClusterChannel > Udp2RedisClusterChannelVector;
    
    CORE::UInt16 m_udpStartPort;
    CORE::UInt16 m_channelCount;
    CORE::Int32 m_redisStreamStartChannelID;
    CORE::CString m_redisStreamName;
    CORE::CString m_redisHost;
    CORE::UInt16 m_redisPort;
    Udp2RedisClusterChannelVector m_channels;
    ChannelSettingsMap m_channelSettings;
    COM::CHTTPServer m_httpServer;
    COM::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;

    CORE::CTimer m_metricsTimer;
    bool m_transmitMetrics;
};