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

#ifndef CDRNCLIENT_H
#define CDRNCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define CDYNAMICARRAY_H
#endif /* CDYNAMICARRAY_H ? */

#ifndef CBPTCPCLIENTSOCKET_H
#include "CBPTCPClientSocket.h"
#define CBPTCPCLIENTSOCKET_H
#endif /* CBPTCPCLIENTSOCKET_H ? */

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

#ifndef CDRNPEERDATAITEM_H
#include "CDRNPeerDataItem.h"
#define CDRNPEERDATAITEM_H
#endif /* CDRNPEERDATAITEM_H ? */

#ifndef CDRNLOCALDATAITEM_H
#include "CDRNLocalDataItem.h"
#define CDRNLOCALDATAITEM_H
#endif /* CDRNLOCALDATAITEM_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { namespace CORE { class CObserver; } }

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

class CDRNControllerToClientTCPHandler;
class CDRNControllerToClientUDPHandler;
class CDRNPeerToClientTCPHandler;
class CDRNPeerToClientUDPHandler;
class CDRNClientInterface;

/*-------------------------------------------------------------------------*/

class GUCEFCOM_EXPORT_CPP CDRNClient : public CDRNBroadcaster
{
        public:
        
        CDRNClient( CDRNClientInterface* iface );
        
        virtual ~CDRNClient();

        bool ConnectTo( const CORE::CString& address ,
                        UInt16 port                  );
                        
        void Close( void );
        
        void SetInterface( CDRNClientInterface* iface );
        
        CDRNClientInterface* GetInterface( void ) const;                        
        
        bool ObserveControllerEvents( CORE::CObserver* observer );
        
        bool ObservePeerEvents( CORE::CObserver* observer ,
                                Int32 event               );

        bool ObserveControllerDataItem( UInt32 itemindex          ,
                                        CORE::CObserver* observer );                
        
        bool ObservePeerDataItem( UInt32 peerindex          ,
                                  UInt32 itemindex          ,
                                  CORE::CObserver* observer );
                                  
        UInt32 GetPeerCount( void ) const;       

        protected:
        
        virtual void Broadcast( const CDRNLocalDataItem* item );
        
        private:       
        
        CORE::CDynamicArray m_controllerinfo;
        CORE::CDynamicArray m_localdata;
        CORE::CDynamicArray m_peerinfo;
        
        COM::CBPTCPClientSocket m_controllercom;
        
        CORE::CDynamicArray m_tcppeerclients;
        COM::CBPTCPServerSocket m_tcppeerserver;
        CPHUDPSocket m_udppeercom;
        
        CDRNClientInterface* m_iface;
        
        private:
        CDRNClient( const CDRNClient& src );
        CDRNClient& operator=( const CDRNClient& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CDRNCLIENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-07-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
