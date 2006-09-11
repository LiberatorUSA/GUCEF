/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef CDRNCONTROLLER_H
#define CDRNCONTROLLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define CDYNAMICARRAY_H
#endif /* CDYNAMICARRAY_H ? */

#ifndef CBPTCPSERVERSOCKET_H
#include "CBPTCPServerSocket.h"
#define CBPTCPSERVERSOCKET_H
#endif /* CBPTCPSERVERSOCKET_H ? */

#ifndef CPHUDPSOCKET_H
#include "CPHUDPSocket.h"
#define CPHUDPSOCKET_H
#endif /* CPHUDPSOCKET_H ? */

#ifndef CDRNBROADCASTER_H
#include "CDRNBroadcaster.h"
#define CDRNBROADCASTER_H
#endif /* CDRNBROADCASTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CDRNControllerMainComHandler;
class CDRNControllerTCPPeerComHandler;
class CDRNControllerPHUDPPeerComHandler;
class CDRNControllerInterface;

/*-------------------------------------------------------------------------*/

/**
 *      Main control class for a DRN.
 *      It acts as the server in the client-server part of the architecture.
 *      
 */
class EXPORT_CPP CDRNController : public CDRNBroadcaster                            
{
        public:
        
        CDRNController( UInt32 controlitems            ,
                        UInt32 peeritems               ,
                        CDRNControllerInterface* iface );
        
        virtual ~CDRNController();

        bool ListnenOnPort( UInt16 port );
        
        void Close( void );
        
        void SetMaxPeers( Int32 maxpeers );
        
        Int32 GetMaxPeers( void ) const;
                
        bool GetMainComEncoding( void ) const;
        
        void SetTCPPeerComPort( UInt16 port );
        
        UInt16 GetTCPPeerComPort( void ) const;
        
        void SetUDPPeerComPort( UInt16 port );
        
        UInt16 GetUDPPeerComPort( void ) const;
        
        void SetLoginTimeout( Int32 maxticks );
        
        Int32 GetLoginTimeout( void ) const;
        
        bool SendEventToPeers( UInt32 eventid );
                
        bool ObservePeerEvents( CORE::CObserver* observer ,
                                Int32 event               );                
        
        bool ObservePeerDataItem( UInt32 peerindex          ,
                                  UInt32 itemindex          ,
                                  CORE::CObserver* observer );        

        protected:
        
        virtual void Broadcast( const CDRNLocalDataItem* item );
        
        private:
        friend class CDRNControllerMainComHandler;
        
        typedef COM::CBPTCPServerSocket::TSocketError TSocketError;
        
        void OnMainComClientRead( COM::CBPTCPServerSocket &socket     ,
                                  COM::CBPTCPServerConnection& client , 
                                  UInt32 connection                   ,
                                  const char *data                    ,
                                  UInt32 size                         ,
                                  UInt32 &keepbytes                   );

        void OnMainComClientWrite( COM::CBPTCPServerSocket &socket     ,
                                   COM::CBPTCPServerConnection& client ,
                                   UInt32 connection                   ,
                                   const char *data                    ,
                                   UInt32 size                         );

        void OnMainComClientConnected( COM::CBPTCPServerSocket &socket        ,
                                       COM::CBPTCPServerConnection& clientcon ,
                                       UInt32 connection                      );

        void OnMainComClientDisconnect( COM::CBPTCPServerSocket& socket     ,
                                        COM::CBPTCPServerConnection& client ,
                                        UInt32 connection                   ,
                                        bool clientclose                    );

        void OnMainComClientError( COM::CBPTCPServerSocket& socket     ,
                                   COM::CBPTCPServerConnection& client ,
                                   UInt32 connection                   ,
                                   TSocketError *errorcode             );

        void OnMainComOpen( COM::CBPTCPServerSocket &socket );
        
        void OnMainComClose( COM::CBPTCPServerSocket &socket );

        void OnMainComMaxConnectionsChanged( COM::CBPTCPServerSocket &socket ,
                                             UInt32 old_max                  ,
                                             UInt32 new_max                  );

        void OnMainComError( COM::CBPTCPServerSocket &socket ,
                             TSocketError errorcode          );        

        private:
        friend class CDRNControllerTCPPeerComHandler;       
        
        void OnTCPPeerComClientRead( COM::CBPTCPServerSocket& socket     ,
                                     COM::CBPTCPServerConnection& client ,
                                     UInt32 connection                   ,
                                     const char *data                    ,
                                     UInt32 size                         ,
                                     UInt32 &keepbytes                   );

        void OnTCPPeerComClientWrite( COM::CBPTCPServerSocket& socket     ,
                                      COM::CBPTCPServerConnection& client ,
                                      UInt32 connection                   ,
                                      const char *data                    ,
                                      UInt32 size                         );

        void OnTCPPeerComClientConnected( COM::CBPTCPServerSocket& socket        ,
                                          COM::CBPTCPServerConnection& clientcon ,
                                          UInt32 connection                      );

        void OnTCPPeerComClientDisconnect( COM::CBPTCPServerSocket& socket     ,
                                           COM::CBPTCPServerConnection& client ,
                                           UInt32 connection                   ,
                                           bool clientclose                    );

        void OnTCPPeerComClientError( COM::CBPTCPServerSocket& socket     ,
                                      COM::CBPTCPServerConnection& client ,
                                      UInt32 connection                   ,
                                      TSocketError *errorcode             );

        void OnTCPPeerComOpen( COM::CBPTCPServerSocket &socket );
        
        void OnTCPPeerComClose( COM::CBPTCPServerSocket &socket );

        void OnTCPPeerComMaxConnectionsChanged( COM::CBPTCPServerSocket &socket ,
                                                UInt32 old_max                  ,
                                                UInt32 new_max                  );

        void OnTCPPeerComError( COM::CBPTCPServerSocket &socket ,
                                TSocketError errorcode          );

        private:
        friend class CDRNControllerPHUDPPeerComHandler;
        
        virtual void OnUDPPeerPacketRecieved( CPHUDPSocket& socket                ,
                                              const CPHUDPSocket::TIPAddress& src ,
                                              const void* data                    ,
                                              const UInt32 datasize               ,
                                              const UInt16 packettype             ,
                                              const UInt32 packetnumber           );
                                              
        private:
                                   
        void MainComTCPSendData( COM::CBPTCPServerConnection& client ,
                                 const char* data                    ,
                                 UInt32 size                         );                                   
        
        void ProcessMainComClientData( COM::CBPTCPServerSocket &socket     ,
                                       COM::CBPTCPServerConnection &client ,
                                       UInt32 connection                   ,
                                       const char *data                    ,
                                       UInt32 size                         );
                                       
        void ProcessTCPPeerComClientData( COM::CBPTCPServerSocket &socket     ,
                                          COM::CBPTCPServerConnection &client ,
                                          UInt32 connection                   ,
                                          const char *data                    ,
                                          UInt32 size                         );
                                          
        void OnNewUserLoggedIn( COM::CBPTCPServerConnection& client ,
                                UInt32 connection                   );
                                
        void OnPeerDataRecieved( UInt32 peerid    ,
                                 const char* data ,
                                 UInt32 size      );
        
        CDRNController( void );
        CDRNController( const CDRNController& src );
        CDRNController& operator=( const CDRNController& src );
        
        private:
        
        CORE::CDynamicArray m_localdata;
        CORE::CDynamicArray m_peerinfo;
        CORE::CDynamicArray m_maincominfo;
        Int32 m_maxpeers;
        Int32 m_maxloginticks;
        bool m_usemaincomencoder;
        UInt32 m_controlitems;
        UInt32 m_peeritems;
        
        COM::CBPTCPServerSocket m_maincom;
        CDRNControllerMainComHandler* m_maincomproxy;
        
        COM::CBPTCPServerSocket m_tcppeercom;
        UInt16 m_tcppeerport;
        CDRNControllerTCPPeerComHandler* m_tcppeerproxy;
        
        CPHUDPSocket m_udppeercom;
        UInt16 m_udppeerport;
        CDRNControllerPHUDPPeerComHandler* m_udppeerproxy;
        
        CDRNControllerInterface* m_iface;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CDRNCONTROLLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-07-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
