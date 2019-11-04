/*
 *  Udp2Redis: service which pushes UDP packets into kafka topics
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

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_COM_CHTTPSERVER_H
#include "gucefCOM_CHTTPServer.h"
#define GUCEF_COM_CHTTPSERVER_H
#endif /* GUCEF_COM_CHTTPSERVER_H ? */

#include "hiredis.h"
#include "async.h"

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

class Udp2RedisChannel : public CORE::CTaskConsumer
{
    public:

    typedef CORE::CTEventHandlerFunctor< Udp2RedisChannel > TEventCallback;

    Udp2RedisChannel();
    Udp2RedisChannel( const Udp2RedisChannel& src );
    virtual ~Udp2RedisChannel();

    virtual bool OnTaskStart( CORE::CICloneable* taskData );
    
    virtual bool OnTaskCycle( CORE::CICloneable* taskData );
    
    virtual void OnTaskEnd( CORE::CICloneable* taskData );

    virtual CORE::CString GetType( void ) const;

    bool LoadConfig( CORE::UInt16 udpPort                   ,
                     const CORE::CString& redisHost         ,
                     CORE::UInt16 redisPort                 ,
                     const CORE::CString& channelStreamName );

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
    OnRedisReconnectTimer( CORE::CNotifier* notifier   ,
                           const CORE::CEvent& eventID ,
                           CORE::CICloneable* evenData );
    private:

    void RegisterEventHandlers( void );
    
    int RedisSend( const CORE::CDynamicBuffer& udpPacket );

    bool SendQueuedPackagesIfAny( void );

    bool RedisConnect( void );

    static void 
    OnRedisASyncVoidReply( redisAsyncContext* context , 
                           void *reply                , 
                           void *privdata             );

    void 
    OnRedisASyncReply( redisAsyncContext* context , 
                       redisReply* reply          );

    static void
    OnRedisASyncConnect( const struct redisAsyncContext* context , 
                         int status                              );

    static void
    OnRedisASyncDisconnect( const struct redisAsyncContext* context , 
                            int status                              );

    static void
    OnRedisAddReadEvent( void* privData );

    static void
    OnRedisDelReadEvent( void* privData );

    static void
    OnRedisAddWriteEvent( void* privData );

    static void
    OnRedisDelWriteEvent( void* privData );

    static void
    OnRedisCleanupEvent( void* privData );

    static void
    OnRedisScheduleTimerEvent( void* privData, struct timeval tv );

    private:

    typedef std::deque< CORE::CDynamicBuffer > TDynamicBufferQueue;
    typedef std::vector< redisAsyncContext* > redisAsyncContextVector;

    CORE::UInt16 m_udpPort;
    CORE::CString m_redisStreamName;
    CORE::CString m_redisStreamSendCmd;
    CORE::CString m_redisHost;
    CORE::UInt16 m_redisPort;
    CORE::CTimer *m_redisReconnectTimer;
    redisAsyncContext* m_redisContext;
    GUCEF::COMCORE::CUDPSocket* m_udpSocket;
    TDynamicBufferQueue m_redisMsgQueueOverflowQueue;
    redisOptions m_redisOptions;
    bool m_redisReadFlag;
    bool m_redisWriteFlag;
    bool m_redisTimeoutFlag;
};

/*-------------------------------------------------------------------------*/

class Udp2Redis
{
    public:

    Udp2Redis( void );
    virtual ~Udp2Redis();

    bool Start( void );

    bool LoadConfig( const CORE::CValueList& config );


    private:

    CORE::UInt16 m_udpStartPort;
    CORE::UInt16 m_channelCount;
    CORE::Int32 m_redisStreamStartChannelID;
    CORE::CString m_redisStreamName;
    CORE::CString m_redisHost;
    CORE::UInt16 m_redisPort;
    std::vector< Udp2RedisChannel > m_channels;
    COM::CHTTPServer m_httpServer;
};