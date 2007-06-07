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

#ifndef GUCEF_DRN_MACROS_H
#include "gucefDRN_macros.h"
#define GUCEF_DRN_MACROS_H
#endif /* GUCEF_DRN_MACROS_H ? */

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
class CDRNPeerLinkData;

/*-------------------------------------------------------------------------*/

/**
 *  Class that coordinates all the DRN functionality for a specific link
 *  once the link has been established.
 */
class GUCEF_DRN_EXPORT_CPP CDRNPeerLink : public CORE::CObservingNotifier
{
    public:
    
    static const CORE::CEvent ConnectedEvent;
    static const CORE::CEvent DisconnectedEvent;
    static const CORE::CEvent SocketErrorEvent;
    static const CORE::CEvent LinkCorruptionEvent;
    static const CORE::CEvent LinkProtocolMismatchEvent;
    static const CORE::CEvent LinkIncompatibleEvent;
    static const CORE::CEvent PeerListReceivedFromPeerEvent;
    static const CORE::CEvent StreamListReceivedFromPeerEvent;
    static const CORE::CEvent DataGroupListReceivedFromPeerEvent;
    
    typedef std::vector< CORE::CString >        TStringList;
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
    
    bool IsOperational( void ) const;
    
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
                   
    void SetAuthenticatedFlag( const bool authenticated );
    
    private:
    
    CDRNPeerLink( const CDRNPeerLink& src );            /**< peer links are unique and cannot be copied */
    CDRNPeerLink& operator=( const CDRNPeerLink& src ); /**< peer links are unique and cannot be copied */

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
    
    void OnPeerLinkIncompatible( void );

    void OnPeerServicesCompatible( void );
    
    void OnPeerStreamListRequest( void );
    
    void OnPeerLinkOperational( void );
    
    void OnPeerDataGroupRequest( void );
    
    void OnPeerDataGroupItemUpdate( void );
                                    
    void SendGreetingMessage( void );
    
    void SendServiceTypeMessage( void );
    
    void SendIncompatibleLinkMessage( void );
    
    void SendAuthenticationRequiredMessage( void );
    
    void SendLinkOperationalMessage( void );
                         
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
    bool m_linkOperational;
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
