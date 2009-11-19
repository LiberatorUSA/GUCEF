/*
 *  ServerPortExtender: Generic networking tool for connecting clients
 *  to a server that is behind a NAT firewall.
 *
 *  Copyright (C) 2002 - 2009.  Dinand Vanvelzen
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

#ifndef CSERVERPORTEXTENDERCLIENT_H
#define CSERVERPORTEXTENDERCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#endif /* GUCEF_COMCORE_CTCPCLIENTSOCKET_H ? */

#ifndef GUCEF_CORE_CCYCLICDYNAMICBUFFER_H
#include "CCyclicDynamicBuffer.h"
#define GUCEF_CORE_CCYCLICDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CCYCLICDYNAMICBUFFER_H ? */

#ifndef SERVERPORTEXTENDERPROTOCOL_H
#include "ServerPortExtenderProtocol.h"
#define SERVERPORTEXTENDERPROTOCOL_H
#endif /* SERVERPORTEXTENDERPROTOCOL_H ? */

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

/** 
 *  The client application is run on the machine that has the server port 
 *  you which to extend through the inverted connection. The extender server
 *  should be run from where the client application is location which will be
 *  connecting via the server port extender connection.
 */
class CServerPortExtenderClient : public CORE::CObserver
{
    public:
    
    CServerPortExtenderClient( void );
    
    bool ConnectToSPEControlSocket( const COMCORE::CHostAddress& host );
    
    bool ConnectToSPEControlSocket( const CORE::CString& hostname, CORE::UInt16 controlPort );
    
    void SetLocalServer( const COMCORE::CHostAddress& host );    

    void SetLocalServer( const CORE::CString& hostname, CORE::UInt16 port );
    
    void SetRemoteServerSocket( CORE::UInt16 port );

    protected:
    
    typedef CORE::CTEventHandlerFunctor< CServerPortExtenderClient > TEventCallback;
    
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );
     
    private:
    
    COMCORE::CTCPClientSocket* GetRemoteConnectionForLocalConnection( const CORE::UInt32 socketId );
    
    COMCORE::CTCPClientSocket* GetLocalConnectionForRemoteConnection( const CORE::UInt32 socketId );

    void RemoveConnectionUsingLocalClient( COMCORE::CTCPClientSocket* localSocket );
    
    void RemoveConnectionUsingSPEClient( COMCORE::CTCPClientSocket* speClientSocket );
    
    void MapLocalToRemoteConnection( COMCORE::CTCPClientSocket* localSocket  ,
                                     COMCORE::CTCPClientSocket* remoteSocket );

    void SendAllDataInBuffer( CORE::CCyclicDynamicBuffer& buffer ,
                              COMCORE::CTCPClientSocket& socket  );
                              
    void OnControlMsg( TServerPortExtenderProtocolEnum msgType , 
                       const CORE::UInt8* data                 );

    void OnControlClientConnected( CORE::CNotifier* notifier    ,
                                   const CORE::CEvent& eventid  ,
                                   CORE::CICloneable* eventdata );
                                     
    void OnControlClientNotify( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventid  ,
                                CORE::CICloneable* eventdata );

   void OnClientToRemoteSPEConnected( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventid  ,
                                      CORE::CICloneable* eventdata );
                                      
    void OnClientToRemoteSPEDisconnected( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventid  ,
                                          CORE::CICloneable* eventdata );
                                               
    void OnClientToRemoteSPEDataRecieved( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventid  ,
                                          CORE::CICloneable* eventdata );
                                               
    void OnClientToRemoteSPEDataSent( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventid  ,
                                      CORE::CICloneable* eventdata );
                                           
    void OnClientToRemoteSPESocketError( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventid  ,
                                         CORE::CICloneable* eventdata );
                                              
   void OnClientToActualServerConnected( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventid  ,
                                         CORE::CICloneable* eventdata );
                                      
    void OnClientToActualServerDisconnected( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventid  ,
                                             CORE::CICloneable* eventdata );
                                               
    void OnClientToActualServerDataRecieved( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventid  ,
                                             CORE::CICloneable* eventdata );
                                               
    void OnClientToActualServerDataSent( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventid  ,
                                         CORE::CICloneable* eventdata );
                                           
    void OnClientToActualServerSocketError( CORE::CNotifier* notifier    ,
                                            const CORE::CEvent& eventid  ,
                                            CORE::CICloneable* eventdata );

    private:
    typedef std::map< COMCORE::CTCPClientSocket*, CORE::CCyclicDynamicBuffer > TClientConnectionBufferMap;
    typedef std::map< CORE::UInt32, COMCORE::CTCPClientSocket* > TSocketIdMap;
    typedef std::set< COMCORE::CTCPClientSocket* > TClientConnectionSet;
    
    COMCORE::CTCPClientSocket m_controlClient;
    TClientConnectionBufferMap m_rsClientConnections;
    TClientConnectionBufferMap m_localClientConnections;
    TSocketIdMap m_remoteToLocalConnectionMap;
    TSocketIdMap m_localToRemoteConnectionMap;
    COMCORE::CHostAddress m_localServer;
    COMCORE::CHostAddress m_remoteSPEServerControl;
    COMCORE::CHostAddress m_remoteSPEReversedServer;
    bool m_controlConnectionInitialized;
};

/*-------------------------------------------------------------------------*/

#endif /* CSERVERPORTEXTENDERCLIENT_H ? */