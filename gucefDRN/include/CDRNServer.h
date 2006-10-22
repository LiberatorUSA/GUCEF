/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#ifndef GUCEF_DRN_CDRNSERVER_H
#define GUCEF_DRN_CDRNSERVER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>
#include <map>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#include "gucefDRN_macros.h"

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

class CIDRNClientAuthenticator;

class GUCEFDRN_EXPORT_CPP CDRNServer : public CObservingNotifier
{
    public:
    
    static const CORE::CEvent ClientConnected;
    static const CORE::CEvent ClientDisconnected;
    static const CORE::CEvent ClientDataMapItemModified;
    static const CORE::CEvent ClientFailedUDPTest;
    static const CORE::CEvent ClientFailedAuthentication;

    typedef CORE::CTNotifyingMap< UInt32, CORE::CDynamicBuffer > TDRNDataMap;
    
    typedef enum TComType
    {
        COMTYPE_CLIENT_SERVER ,
        COMTYPE_PEER_TO_PEER
    };
    
    typedef enum TComMethod
    {
        COMMETHOD_TCP         ,
        COMMETHOD_UDP         ,
        COMMETHOD_TCP_AND_UDP
    };
    
    bool Activate( void );
    
    void Deactivate( void );
    
    void SubscribeToServerDataMap( const TDRNDataMap& dataMap );
    
    void UnsubscribeFromServerDataMap( const TDRNDataMap& dataMap );
    
    TDRNDataMap& GetClientDataMap( const UInt32 clientID );
        
    void SetTCPPort( const UInt32 serverPort );
    
    UInt32 GetTCPPort( void ) const;
    
    void SetUDPPort( const UInt32 serverPort );
    
    UInt32 GetUDPPort( void ) const;
    
    void SetDataItemComMethod( const UInt32 itemID        ,
                               const TComType comType     ,
                               const TComMethod comMethod );
    
    TComMethod GetDataItemComMethod( const UInt32 itemID    ,
                                     const TComType comType ) const;

    void SetDRNClientAuthenticator( const CIDRNClientAuthenticator& clientAuthenticator );
    
    static void RegisterEvents( void );
    
    private:
    
    std::set< typename TDRNDataMap* > m_serverMaps;
    std::map< UInt32, typename TDRNDataMap > m_clientMaps;    /**< maps a client ID to a TDRNDataMap */
    std::map< UInt32, TComMethod > m_csDataMapComMethod;      /**< maps a data ID to a communication method for client-server data */
    std::map< UInt32, TComMethod > m_p2pDataMapComMethod;     /**< maps a data ID to a communication method for peer-to-peer data */
    COMCORE::CTCPServerSocket m_tcpServerSocket;              /**< server socket for TCP client-server communications */
    COMCORE::CUDPSocket m_udpSocket;                          /**< server socket for UDP client-server communications */
    CIDRNClientAuthenticator* m_clientAuthenticator;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DRN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DRN_CDRNSERVER_H ? */