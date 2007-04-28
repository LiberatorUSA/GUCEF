/*
 *  gucefDRN: GUCEF module providing RAD networking trough data replication
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_DRN_CDRNNODE_H
#define GUCEF_DRN_CDRNNODE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <vector>

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#endif /* GUCEF_COMCORE_CTCPCLIENTSOCKET_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_DRN_CDRNDATAGROUP_H
#include "gucefDRN_CDRNDataGroup.h"
#define GUCEF_DRN_CDRNDATAGROUP_H
#endif /* GUCEF_DRN_CDRNDATAGROUP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace DRN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CDRNPeerLink;
class CIDRNPeerValidator;
class CIDRNPeerLinkCrypter;

/*-------------------------------------------------------------------------*/

class GUCEF_DRN_EXPORT_CPP CDRNNode : private CORE::CObservingNotifier
{
    public:
    
    typedef std::vector< CORE::CString >    TStringList;
    
    static const CORE::CEvent PeerListReceivedFromPeerEvent;
    static const CORE::CEvent StreamListReceivedFromPeerEvent;
    static const CORE::CEvent DataGroupListReceivedFromPeerEvent;
    
    struct SPeerRelatedStringList
    {
        CDRNPeerLink& peerLink; /**< origin of the string list */
        TStringList stringList; /**< list of received strings */
    };
    
    typedef CORE::CTCloneableObj< struct SPeerRelatedStringList > PeerListReceivedFromPeerEventData;   
    typedef CORE::CTCloneableObj< struct SPeerRelatedStringList > StreamListReceivedFromPeerEventData;
    typedef CORE::CTCloneableObj< struct SPeerRelatedStringList > DataGroupListReceivedFromPeerEventData;
    
    static void RegisterEvents( void );    
    
    public:
    
    CDRNNode( void );
    
    virtual ~CDRNNode();

    void SetServiceName( const CORE::CString& serviceName );
    
    const CORE::CString& GetServiceName( void ) const;
    
    bool ListenOnPort( const UInt16 port );
    
    void StopListening( void );
    
    bool IsListening( void ) const;
    
    void Disconnect( void );
    
    bool ConnectToPeer( const CORE::CString& address , 
                        const UInt16 port            );

    void SetPeerLinkCrypter( CIDRNPeerLinkCrypter* peerLinkCrypter );
    
    CIDRNPeerLinkCrypter* GetPeerLinkCrypter( void ) const;
    
    void SetPeerValidator( CIDRNPeerValidator* peerValidator );
    
    CIDRNPeerValidator* GetPeervalidator( void ) const;
    
    bool RequestPeerList( CDRNPeerLink& peerLink );
    
    bool RequestStreamList( CDRNPeerLink& peerLink );
    
    bool RequestDataGroupList( CDRNPeerLink& peerLink );
    
/*    
    void SubscribeLocalGlobalDataGroup( CDRNDataGroup );
    
    void SubscribeLocalPeerDataGroup( CDRNDataGroup );
    
    void SubscribeRemoteGlobalDataGroup( CDRNDataGroup );
    
    void SubscribeRemotePeerDataGroup( CDRNDataGroup );
  */

    protected:
    
    /**
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );    

    private:
    
    CDRNNode( const CDRNNode& src );             /**< not implemented, makes no sense */
    CDRNNode& operator=( const CDRNNode& src );  /**< not implemented, makes no sense */
    
    void OnTCPServerSocketEvent( CORE::CNotifier* notifier    ,
                                 const CORE::CEvent& eventid  ,
                                 CORE::CICloneable* eventdata );

    void OnTCPClientPeerSocketEvent( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventid  ,
                                     CORE::CICloneable* eventdata );

    void GetAvailableTCPClient( COMCORE::CTCPClientSocket** tcpClient ,
                                UInt32& socketIndex                   );
    
    private:
    
    typedef std::vector< COMCORE::CTCPClientSocket* > TTCPClients;
    
    CORE::CString m_serviceName;
    COMCORE::CUDPSocket m_udpSocket;
    COMCORE::CTCPServerSocket m_tcpServerSocket;
    COMCORE::CTCPClientSocket m_tcpClientToServerSocket;
    TTCPClients m_tcpClientSockets;
    CIDRNPeerValidator* m_peerValidator;
    CIDRNPeerLinkCrypter* m_peerLinkCrypter;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DRN_CDRNNODE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
