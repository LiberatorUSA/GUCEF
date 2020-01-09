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

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#endif /* GUCEF_COMCORE_CTCPCLIENTSOCKET_H ? */

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

class UdpViaTcp;

class RestApiUdpViaTcpInfoResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiUdpViaTcpInfoResource( UdpViaTcp* app );

    virtual ~RestApiUdpViaTcpInfoResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    UdpViaTcp* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiUdpViaTcpConfigResource : public COM::CCodecBasedHTTPServerResource
{
    public:

    RestApiUdpViaTcpConfigResource( UdpViaTcp* app, bool appConfig );

    virtual ~RestApiUdpViaTcpConfigResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    UdpViaTcp* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class UdpViaTcp : public CORE::CObservingNotifier
{
    public:

    UdpViaTcp( void );
    virtual ~UdpViaTcp();

    bool Start( void );

    bool LoadConfig( const CORE::CValueList& appConfig   ,
                     const CORE::CDataNode& globalConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    private:

    void RegisterEventHandlers( void );

    void
    OnUDPReceiveSocketError( CORE::CNotifier* notifier   ,
                             const CORE::CEvent& eventID ,
                             CORE::CICloneable* evenData );

    void
    OnUDPReceiveSocketClosed( CORE::CNotifier* notifier   ,
                              const CORE::CEvent& eventID ,
                              CORE::CICloneable* evenData );

    void
    OnUDPReceiveSocketOpened( CORE::CNotifier* notifier   ,
                              const CORE::CEvent& eventID ,
                              CORE::CICloneable* evenData );

    void
    OnUDPReceiveSocketPacketRecieved( CORE::CNotifier* notifier   ,
                                      const CORE::CEvent& eventID ,
                                      CORE::CICloneable* evenData );

    void
    OnUDPTransmitSocketError( CORE::CNotifier* notifier   ,
                              const CORE::CEvent& eventID ,
                              CORE::CICloneable* evenData );

    void
    OnUDPTransmitSocketClosed( CORE::CNotifier* notifier   ,
                               const CORE::CEvent& eventID ,
                               CORE::CICloneable* evenData );

    void
    OnUDPTransmitSocketOpened( CORE::CNotifier* notifier   ,
                               const CORE::CEvent& eventID ,
                               CORE::CICloneable* evenData );

    void
    OnUDPTransmitSocketPacketRecieved( CORE::CNotifier* notifier   ,
                                       const CORE::CEvent& eventID ,
                                       CORE::CICloneable* evenData );

    void
    OnTCPServerClientConnected( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData );
    
    void
    OnTCPServerClientDisconnected( CORE::CNotifier* notifier    ,
                                   const CORE::CEvent& eventId  ,
                                   CORE::CICloneable* eventData );
    void
    OnTCPServerClientError( CORE::CNotifier* notifier    ,
                            const CORE::CEvent& eventId  ,
                            CORE::CICloneable* eventData );
    
    void 
    OnTCPServerSocketOpened( CORE::CNotifier* notifier    ,
                             const CORE::CEvent& eventId  ,
                             CORE::CICloneable* eventData );
    
    void
    OnTCPServerSocketClosed( CORE::CNotifier* notifier    ,
                             const CORE::CEvent& eventId  ,
                             CORE::CICloneable* eventData );

    void
    OnTCPServerSocketError( CORE::CNotifier* notifier    ,
                            const CORE::CEvent& eventId  ,
                            CORE::CICloneable* eventData );

    void
    OnTCPServerSocketClientError( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventId  ,
                                  CORE::CICloneable* eventData );

    void
    OnTCPServerSocketMaxConnectionsChanged( CORE::CNotifier* notifier    ,
                                            const CORE::CEvent& eventId  ,
                                            CORE::CICloneable* eventData );

    void
    OnTCPServerConnectionDataRecieved( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData );

    private:

    typedef CORE::CTEventHandlerFunctor< UdpViaTcp > TEventCallback;

    enum EUdpViaTcpMode
    {
        UDPVIATCPMODE_UDP_RECEIVER_ONLY    ,
        UDPVIATCPMODE_UDP_TRANSMITTER_ONLY ,
        UDPVIATCPMODE_BIDIRECTIONAL_UDP
    };
    typedef enum EUdpViaTcpMode TUdpViaTcpMode;

    COMCORE::CTCPServerSocket m_tcpServerSocket;
    COMCORE::CTCPClientSocket m_tcpClientSocket;
    COMCORE::CUDPSocket m_udpTransmitSocket;
    COMCORE::CUDPSocket m_udpReceiveSocket;
    std::vector< CORE::CDynamicBuffer > m_receivePacketBuffers;
    
    TUdpViaTcpMode m_mode;
    COMCORE::CHostAddress m_tcpDestination;
    COMCORE::CHostAddress m_tcpReceiver;
    COMCORE::CHostAddress m_udpDestination;
    COMCORE::CHostAddress m_udpReceiver;

    COM::CHTTPServer m_httpServer;
    COM::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;
};