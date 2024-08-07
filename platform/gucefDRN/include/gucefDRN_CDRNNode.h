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

#ifndef GUCEF_COMCORE_CUDPMASTERSOCKET_H
#include "CUDPMasterSocket.h"
#define GUCEF_COMCORE_CUDPMASTERSOCKET_H
#endif /* GUCEF_COMCORE_CUDPMASTERSOCKET_H ? */

#ifndef GUCEF_DRN_CDRNDATAGROUP_H
#include "gucefDRN_CDRNDataGroup.h"
#define GUCEF_DRN_CDRNDATAGROUP_H
#endif /* GUCEF_DRN_CDRNDATAGROUP_H ? */

#ifndef GUCEF_DRN_CDRNPEERLINK_H
#include "gucefDRN_CDRNPeerLink.h"
#define GUCEF_DRN_CDRNPEERLINK_H
#endif /* GUCEF_DRN_CDRNPEERLINK_H ? */

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

class CIDRNPeerValidator;
class CIDRNPeerLinkCrypter;

/*-------------------------------------------------------------------------*/

/**
 *  Main DRN connectivity class that manages a collection of DRN links.
 */
class GUCEF_DRN_EXPORT_CPP CDRNNode : public CORE::CObservingNotifier
{
    public:
    
    static const CORE::CEvent LinkEstablishedEvent;
    static const CORE::CEvent LinkDisconnectedEvent;
    static const CORE::CEvent LinkErrorEvent;
    
    typedef CORE::CTSharedPtr< CDRNPeerLink, MT::CMutex >       CDRNPeerLinkPtr;
    typedef std::vector< CDRNPeerLinkPtr >                      TPeerLinkList;
    typedef CORE::CTCloneableObj< CDRNPeerLinkPtr >             LinkEstablishedEventData;
    typedef CORE::CTCloneableObj< CDRNPeerLinkPtr >             LinkDisconnectedEventData;
    typedef CDRNPeerLinkData::TDRNDataStreamPtr                 TDRNDataStreamPtr;
    typedef CDRNPeerLinkData::TDRNDataGroupPtr                  TDRNDataGroupPtr;

    static void RegisterEvents( void );    
    
    public:
    
    typedef COMCORE::CHostAddress CHostAddress;
    
    CDRNNode( void );
    
    virtual ~CDRNNode();
    
    bool ListenOnPort( const UInt16 port );
    
    void StopListening( void );
    
    bool IsListening( void ) const;
    
    void Disconnect( void );
    
    void GetListenAddress( CHostAddress& listenAddress ) const;
    
    bool OpenUDPPort( const UInt16 port );
    
    void CloseUDPPort( void );
    
    bool IsUDPPortOpened( void ) const;
    
    UInt16 GetUDPPort( void ) const;
    
    bool HasOperationalLinksForUs( void ) const;
    
    UInt32 GetOperationalLinkForUsCount( void ) const;
    
    bool ConnectToPeer( const CORE::CString& address , 
                        const UInt16 port            );

    bool ConnectToPeer( const COMCORE::CHostAddress& hostAddress );

    void SetPeerLinkCrypter( CIDRNPeerLinkCrypter* peerLinkCrypter );
    
    CIDRNPeerLinkCrypter* GetPeerLinkCrypter( void ) const;
    
    void SetPeerValidator( CIDRNPeerValidator* peerValidator );
    
    CIDRNPeerValidator* GetPeervalidator( void ) const;

    void PublicizeStream( TDRNDataStreamPtr& dataStream );

    void StopStreamPublication( const CORE::CString& dataStreamName );
    
    void PublicizeDataGroup( TDRNDataGroupPtr& dataGroup );
    
    void StopDataGroupPublication( const CORE::CString& dataGroupName );
    
    void GetPeerLinkList( TPeerLinkList& peerLinkList );
    
    void SetOverrideConnectBackSettings( const bool overrideSettings );
    
    bool GetOverrideConnectBackSettings( void ) const;
    
    void SetConnectBackOverride( const CHostAddress& host ,
                                 const UInt16 udpPort     );

    void GetConnectBackOverride( CHostAddress& host ,
                                 UInt16& udpPort    ) const;
    
    protected:
    
    /**
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined user data
     */
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );    

    private:
    
    CDRNNode( const CDRNNode& src );             /**< not implemented, makes no sense */
    CDRNNode& operator=( const CDRNNode& src );  /**< not implemented, makes no sense */

    void GetAvailableTCPClient( COMCORE::CTCPClientSocket** tcpClient ,
                                UInt32& socketIndex                   );

    void CreateLink( COMCORE::CTCPConnection& tcpConnection );
    
    void SendConnectBackInfoToLinks( void );
    
    private:
    
    typedef std::vector< COMCORE::CTCPClientSocket* >       TTCPClients;
    typedef std::map< CORE::CString, TDRNDataStreamPtr >    TDataStreamList;
    typedef std::map< CORE::CString, TDRNDataGroupPtr >     TDataGroupList;
    
    COMCORE::CUDPMasterSocket m_udpSocket;
    COMCORE::CTCPServerSocket m_tcpServerSocket;
    TTCPClients m_tcpClientSockets;
    CIDRNPeerValidator* m_peerValidator;
    CIDRNPeerLinkCrypter* m_peerLinkCrypter;
    TDataStreamList m_dataStreamList;
    TDataGroupList m_dataGroupList;
    TPeerLinkList m_peerLinkList;
    bool m_overrideConnectBack;
    CHostAddress m_overrideHost;
    UInt16 m_overrideUDPPort;
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

---------------------------------------------------------------------------*/
