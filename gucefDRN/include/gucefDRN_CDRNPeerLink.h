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

#ifndef GUCEF_DRN_CDRNPEERLINK_H
#define GUCEF_DRN_CDRNPEERLINK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_COMCORE_CSOCKET_H
#include "CSocket.h"
#define GUCEF_COMCORE_CSOCKET_H
#endif /* GUCEF_COMCORE_CSOCKET_H ? */

#ifndef GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#include "CTLinkedCloneableObj.h"
#define GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_DRN_CDRNPEERLINKDATA_H
#include "gucefDRN_CDRNPeerLinkData.h"
#define GUCEF_DRN_CDRNPEERLINKDATA_H
#endif /* GUCEF_DRN_CDRNPEERLINKDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { namespace COMCORE { class CTCPConnection; class CUDPChannel; class CUDPMasterSocket; } }

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

class CDRNNode;

/*-------------------------------------------------------------------------*/

/**
 *  Class that coordinates all the DRN functionality for a specific link
 *  once the initial connection has been established.
 */
class GUCEF_DRN_EXPORT_CPP CDRNPeerLink : public CORE::CObservingNotifier
{
    public:
    
    // Events related to link establishment
    static const CORE::CEvent ConnectedEvent;                     /**< We are now connected with the peer */
    static const CORE::CEvent DisconnectedEvent;                  /**< The link has been disconnected */
    static const CORE::CEvent SocketErrorEvent;                   /**< fatal error: A low-level socket error occured */
    static const CORE::CEvent LinkCorruptionEvent;                /**< fatal error: A malformed transmission was detected on the link */
    static const CORE::CEvent LinkProtocolMismatchEvent;          /**< fatal error: the peer is using a different version of the DRN protocol */
    static const CORE::CEvent LinkProtocolMatchEvent;             /**< the peer is using a compatible DRN protocol version */
    static const CORE::CEvent LinkIncompatibleEvent;              /**< fatal error: the link with the peer is incompatible and cannot be used */
    static const CORE::CEvent CompatibleServiceEvent;             /**< the service made available on the peer side is compatible with ours */
    static const CORE::CEvent IncompatibleServiceEvent;           /**< fatal error: the service made available on the peer side is not compatible with ours */
    static const CORE::CEvent PeerAuthenticationSuccessEvent;     /**< The peer successfully authenticated */
    static const CORE::CEvent AuthenticationSuccessEvent;         /**< We successfully authenticated at the peer */
    static const CORE::CEvent PeerAuthenticationFailureEvent;     /**< warning: The peer failed to provide valid authentication */
    static const CORE::CEvent AuthenticationFailureEvent;         /**< warning: We failed to provide valid authentication for the peer */
    
    // Events signaling a change in the operational status
    static const CORE::CEvent LinkOperationalForPeerEvent;        /**< the link is operational for the peer, the peer can now setup subscriptions etc*/
    static const CORE::CEvent LinkOperationalForUsEvent;          /**< the link is operational for us, we can now setup subscriptions etc*/

    // Events giving feedback to requests
    static const CORE::CEvent IllegalRequestEvent;                /**< warning: According to the peer we made an illegal request */
    static const CORE::CEvent PeerListReceivedFromPeerEvent;      /**< We received a list of peers from our peer */
    static const CORE::CEvent StreamListReceivedFromPeerEvent;    /**< We received a list of streams from our peer */
    static const CORE::CEvent DataGroupListReceivedFromPeerEvent; /**< We received a list of data groups from our peer */
    
    typedef std::vector< CORE::CString >              TStringList;
    typedef CORE::CTLinkedCloneableObj< TStringList > PeerListReceivedFromPeerEventData;   
    typedef CORE::CTLinkedCloneableObj< TStringList > StreamListReceivedFromPeerEventData;
    typedef CORE::CTLinkedCloneableObj< TStringList > DataGroupListReceivedFromPeerEventData;
    
    static void RegisterEvents( void );
    
    public:
    
    typedef COMCORE::CIPAddress CIPAddress;
    
    virtual ~CDRNPeerLink();
    
    CIPAddress GetPeerIP( void ) const;
    
    CORE::CString GetPeerHostName( void ) const;
    
    void CloseLink( void );
    
    UInt16 GetPeerTCPPort( void ) const;
    
    UInt16 GetPeerUDPPort( void ) const;
    
    bool IsUDPPossible( void ) const;
    
    bool IsAuthenticated( void ) const;
    
    bool IsPeerAuthenticated( void ) const;
    
    bool IsOperationalForUs( void ) const;
    
    bool IsOperationalForPeer( void ) const;
    
    CDRNNode& GetParentNode( void );
    
    CDRNPeerLinkData& GetLinkData( void );
                                    
    bool RequestDataGroupSubscription( const CORE::CString& dataGroupName );

    bool RequestStreamSubscription( const CORE::CString& dataSreamName );

    bool RequestPeerList( void );
    
    bool RequestStreamList( void );
    
    bool RequestDataGroupList( void );
    
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
    friend class CDRNNode;
    
    CDRNPeerLink( CDRNNode& parentNode                   ,
                  COMCORE::CTCPConnection& tcpConnection ,
                  COMCORE::CUDPMasterSocket& udpSocket   );

    COMCORE::CTCPConnection* GetTCPConnection( void );   
    
    bool SendData( const void* dataSource             ,
                   const UInt16 dataSize              ,
                   const bool allowUnreliable = false );
    private:
    
    CDRNPeerLink( const CDRNPeerLink& src );            /**< peer links are unique and cannot be copied */
    CDRNPeerLink& operator=( const CDRNPeerLink& src ); /**< peer links are unique and cannot be copied */

    void OnPublicizedDataGroupChange( CDRNDataGroup& dataGroup     ,
                                      CORE::CICloneable* eventdata );

    void OnPublicizedDataStreamSend( CDRNDataStream& dataStream   ,
                                     CORE::CICloneable* eventdata );
    
    void OnTCPConnectionEvent( CORE::CNotifier* notifier    ,
                               const CORE::CEvent& eventid  ,
                               CORE::CICloneable* eventdata );

    void OnUDPChannelEvent( CORE::CNotifier* notifier    ,
                            const CORE::CEvent& eventid  ,
                            CORE::CICloneable* eventdata );

    void OnTCPDataReceived( const GUCEF::CORE::CDynamicBuffer& buffer );

    void OnPeerDataReceived( const char* data      ,
                             const UInt32 dataSize );

    void OnPeerGreeting( const char* data      ,
                         const UInt32 dataSize );

    void OnPeerServiceType( const char* data      ,
                            const UInt32 dataSize );

    void OnPeerDataGroupListReceived( const char* data      ,
                                      const UInt32 dataSize );

    void OnPeerPeerListReceived( const char* data      ,
                                 const UInt32 dataSize );
    
    void OnPeerDataGroupItemUpdate( const char* data      ,
                                    const UInt32 dataSize );

    void OnPeerStreamListReceived( const char* data      ,
                                   const UInt32 dataSize );
    
    void OnPeerAuthentication( const char* data      ,
                               const UInt32 dataSize );
                               
    void OnPeerSubscribeToDataGroupRequest( const char* data      ,
                                            const UInt32 dataSize );
    
    void OnPeerSubscribeToStreamRequest( const char* data      ,
                                         const UInt32 dataSize );

    void OnPeerStreamDataReceived( const char* data      ,
                                   const UInt32 dataSize );

    void OnSubscribedToPeerDataGroup( const char* data      ,
                                      const UInt32 dataSize );

    void OnSubscribedToPeerDataStream( const char* data      ,
                                       const UInt32 dataSize );
                                                                           
    void OnPeerAuthenticationSuccess( void );
    
    void OnPeerAuthenticationFailed( void );
    
    void OnPeerLinkIncompatible( void );

    void OnPeerServicesCompatible( void );
    
    void OnPeerStreamListRequest( void );
    
    void OnPeerPeerListRequest( void );
    
    void OnPeerLinkOperational( void );
    
    void OnPeerDataGroupRequest( void );
    
    void OnPeerDataGroupItemUpdate( void );
    
    void OnPeerAuthenticationRequest( void );
    
    void OnPeerIllegalRequest( void );
    
    void SendGreetingMessage( void );
    
    void SendServiceTypeMessage( void );
    
    void SendIncompatibleLinkMessage( void );
    
    void SendAuthenticationRequiredMessage( void );
    
    void SendAuthenticationSuccess( void );
    
    void SendAuthenticationFailed( void );
    
    void SendLinkOperationalMessage( void );
    
    void SendNotAllowed( void );

    void SendAuthentication( const CORE::CString& ourAccountName ,
                             const CORE::CString& ourPassword    );

	void SendSubscribedToDataGroup( const CORE::CString& groupName ,
	                                const UInt16 id                );

	void SendSubscribedToDataStream( const CORE::CString& groupName ,
	                                 const UInt16 id                );
                                                           
    private:
    
    CDRNNode* m_parentNode;
    CDRNPeerLinkData* m_linkData;
    COMCORE::CUDPChannel* m_udpChannel;
    COMCORE::CUDPMasterSocket* m_udpSocket;
    COMCORE::CTCPConnection* m_tcpConnection;
    GUCEF::CORE::CDynamicBuffer m_tcpStreamBuffer;
    UInt32 m_tcpStreamKeepBytes;
    bool m_udpPossible;
    bool m_isAuthenticated;
    bool m_isPeerAuthenticated;
    bool m_isLinkOperationalForPeer;
    bool m_isLinkOperationalForUs;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DRN_CDRNPEERLINK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
