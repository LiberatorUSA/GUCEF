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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef COBSERVER_H
#include "CObserver.h"
#define COBSERVER_H
#endif /* COBSERVER_H ? */

#include "CDRNClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SDRNPeerInfo
{
        CORE::CDynamicArray data;
        COMCORE::CSocket::TIPAddress ipaddress;        
};
typedef struct SDRNPeerInfo TDRNPeerInfo; 

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CDRNClient::CDRNClient( CDRNClientInterface* iface )
        : m_iface( iface )         ,
          m_tcppeerserver( false ) ,
          m_controllercom( false )
{TRACE;
//        m_controllercom.SetInterface( new CDRNControllerToClientHandler( this ) );
}

/*-------------------------------------------------------------------------*/

CDRNClient::CDRNClient( const CDRNClient& src )
        : m_tcppeerserver( false ) ,
          m_controllercom( false )
{TRACE;
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CDRNClient::~CDRNClient()
{TRACE;
        Close();
}

/*-------------------------------------------------------------------------*/

CDRNClient&
CDRNClient::operator=( const CDRNClient& src )
{TRACE;
        /* dummy, do not use */
        return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDRNClient::ConnectTo( const CORE::CString& address ,
                       UInt16 port                  )
{TRACE;        
        return m_controllercom.ConnectTo( address, port ); 
        
        
}

/*-------------------------------------------------------------------------*/

void
CDRNClient::Close( void )
{TRACE;
}

/*-------------------------------------------------------------------------*/                       
        
bool 
CDRNClient::ObserveControllerDataItem( UInt32 itemindex          ,
                                       CORE::CObserver* observer )
{TRACE;
       /* MT::CScopeMutex mutex;
        
        if ( m_controllerinfo.data.size() < itemindex )
        {
                return observer->SubscribeTo( &m_controllerinfo.data[ itemindex ] ,
                                              STD_NOTIFIER_EVENT_UPDATE           );
        }  */
        return false;
}

/*-------------------------------------------------------------------------*/
                                       
bool 
CDRNClient::ObservePeerDataItem( UInt32 peerindex          ,
                                 UInt32 itemindex          ,
                                 CORE::CObserver* observer )
{TRACE;      
     /*   if ( m_peerdata.count() < peerindex )
        {
                TDRNPeerInfo& peerinfo = m_peerinfo[ peerindex ];
                if ( peerinfo.data.size() < itemindex )
                {
                        return observer->SubscribeTo( &peerinfo.data[ itemindex ] ,
                                                      STD_NOTIFIER_EVENT_UPDATE   );                        
                }
        }    */   
        return false;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDRNClient::GetPeerCount( void ) const
{TRACE;
        return 0;//m_peerdata.count();
}

/*-------------------------------------------------------------------------*/

void 
CDRNClient::Broadcast( const CDRNLocalDataItem* item )
{TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/