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

#ifndef GUCEF_COMCORE_CSOCKET_H
#include "CSocket.h"
#define GUCEF_COMCORE_CSOCKET_H
#endif /* GUCEF_COMCORE_CSOCKET_H ? */

#ifndef GUCEF_DRN_MACROS_H
#include "gucefDRN_macros.h"
#define GUCEF_DRN_MACROS_H
#endif /* GUCEF_DRN_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { namespace COMCORE { class CTCPConnection; class CUDPSocket; } }

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
 *  Class that stores information about a peer link
 */
class GUCEF_DRN_EXPORT_CPP CDRNPeerLink
{
    public:
    
    typedef COMCORE::CSocket::TIPAddress TIPAddress;
    
    TIPAddress GetPeerIP( void ) const;
    
    const CORE::CString& GetPeerHostName( void ) const;
    
    UInt16 GetPeerTCPPort( void ) const;
    
    UInt16 GetPeerUDPPort( void ) const;
    
    bool IsUDPPossible( void ) const;
    
    bool IsAuthenticated( void ) const;
 
    private:
    friend class CDRNNode;
    
    CDRNPeerLink( void );

    virtual ~CDRNPeerLink();

    COMCORE::CTCPConnection* GetTCPConnection( void );
    
    void SetTCPConnection( COMCORE::CTCPConnection& tcpConnection );
    
    COMCORE::CUDPSocket* GetUDPSocket( void );
    
    void SetUDPSocket( COMCORE::CUDPSocket& socket );
    
    bool SendData( const void* dataSource             ,
                   const UInt16 dataSize              ,
                   const bool allowUnreliable = false );
                   
    bool SetAuthenticatedFlag( const bool authenticated );
    
    private:
    
    CDRNPeerLink( const CDRNPeerLink& src );            /**< peer links are unique and cannot be copied */
    CDRNPeerLink& operator=( const CDRNPeerLink& src ); /**< peer links are unique and cannot be copied */

    private:
    
    COMCORE::CUDPSocket* m_udpSocket;
    COMCORE::CTCPConnection* m_tcpConnection;
    bool m_udpPossible;
    bool m_isAuthenticated;
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
