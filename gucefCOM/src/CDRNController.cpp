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

#ifndef DVOSWRAP_H
#include "dvoswrap.h"
#define DVOSWRAP_H
#endif /* DVOSWRAP_H ? */

#ifndef CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define CDYNAMICBUFFER_H
#endif /* CDYNAMICBUFFER_H ? */

#ifndef CDRNCONTROLLERMAINCOMHANDLER_H
#include "CDRNControllerMainComHandler.h"
#define CDRNCONTROLLERMAINCOMHANDLER_H
#endif /* CDRNCONTROLLERMAINCOMHANDLER_H ? */

#ifndef CDRNCONTROLLERTCPPEERCOMHANDLER_H
#include "CDRNControllerTCPPeerComHandler.h"
#define CDRNCONTROLLERTCPPEERCOMHANDLER_H
#endif /* CDRNCONTROLLERTCPPEERCOMHANDLER_H ? */

#ifndef CDRNCONTROLLERPHUDPPEERCOMHANDLER_H
#include "CDRNControllerPHUDPPeerComHandler.h"
#define CDRNCONTROLLERPHUDPPEERCOMHANDLER_H
#endif /* CDRNCONTROLLERPHUDPPEERCOMHANDLER_H ? */

#ifndef CDRNCONTROLLERINTERFACE_H
#include "CDRNControllerInterface.h"
#define CDRNCONTROLLERINTERFACE_H
#endif /* CDRNCONTROLLERINTERFACE_H ? */

#ifndef CDRNLOCALDATAITEM_H
#include "CDRNLocalDataItem.h"
#define CDRNLOCALDATAITEM_H
#endif /* CDRNLOCALDATAITEM_H ? */

#ifndef CDRNPEERDATAITEM_H
#include "CDRNPeerDataItem.h"
#define CDRNPEERDATAITEM_H
#endif /* CDRNPEERDATAITEM_H ? */

#ifndef DRNPROTOCOL_H
#include "DRNProtocol.h"
#define DRNPROTOCOL_H
#endif /* DRNPROTOCOL_H ? */

#include "CDRNController.h"

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

struct SMainComInfo
{
        CORE::CString username;
        UInt32 peerid;
        bool loggedin;
        bool connected;
        UInt32 connecttime;               
};
typedef struct SMainComInfo TMainComInfo;

/*-------------------------------------------------------------------------*/

struct SPeerInfo
{
        CORE::CDynamicArray data;
        COMCORE::CSocket::TIPAddress address;
        bool udppossible;
        UInt32 comkey;
        TMainComInfo* maincominfo;
};
typedef struct SPeerInfo TPeerInfo;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILTIES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

CDRNController::CDRNController( UInt32 controlitems            ,
                                UInt32 peeritems               ,
                                CDRNControllerInterface* iface )
        : m_maincom( false )             ,
          m_tcppeercom( false )          ,
          m_localdata( controlitems )    ,
          m_maxloginticks( 2000 )        ,
          m_maxpeers( -1 )               ,
          m_usemaincomencoder( true )    ,
          m_controlitems( controlitems ) ,
          m_peeritems( peeritems )
{TRACE;
        m_maincomproxy = new CDRNControllerMainComHandler( this );
        m_maincom.SetInterface( m_maincomproxy );
        
        m_tcppeerproxy = new CDRNControllerTCPPeerComHandler( this );
        m_tcppeercom.SetInterface( m_tcppeerproxy );
        
        m_udppeerproxy = new CDRNControllerPHUDPPeerComHandler( this );
        m_udppeercom.RegisterEventHandler( m_udppeerproxy );
        
        for ( UInt32 i=0; i<controlitems; ++i )
        {
                m_localdata.SetEntry( i, new CDRNLocalDataItem( this, i ) ); 
        }
}

/*-------------------------------------------------------------------------*/

CDRNController::CDRNController( void )
        : m_maincom( false )    ,
          m_tcppeercom( false )
{TRACE;       
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CDRNController::CDRNController( const CDRNController& src )
        : m_maincom( false )    ,
          m_tcppeercom( false )
{TRACE;       
        /* dummy, do not use */
}

/*-------------------------------------------------------------------------*/

CDRNController::~CDRNController()
{TRACE;
        Close();
        
        delete m_maincomproxy;
        delete m_tcppeerproxy;
        delete m_udppeerproxy;
}

/*-------------------------------------------------------------------------*/

CDRNController&
CDRNController::operator=( const CDRNController& src )
{TRACE;
        /* dummy, do not use */
        return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDRNController::ListnenOnPort( UInt16 port )
{TRACE;
        return m_maincom.ListenOnPort( port );
}

/*-------------------------------------------------------------------------*/

void 
CDRNController::Close( void )
{TRACE;
        m_maincom.Close();
        m_tcppeercom.Close();
        m_udppeercom.Close();        
}

/*-------------------------------------------------------------------------*/
        
bool 
CDRNController::GetMainComEncoding( void ) const
{TRACE;
        return m_usemaincomencoder;
}

/*-------------------------------------------------------------------------*/

void 
CDRNController::SetMaxPeers( Int32 maxpeers )
{TRACE;
        m_maxpeers = maxpeers;
}

/*-------------------------------------------------------------------------*/
        
Int32 
CDRNController::GetMaxPeers( void ) const
{TRACE;
        return m_maxpeers;
}

/*-------------------------------------------------------------------------*/
        
void 
CDRNController::SetTCPPeerComPort( UInt16 port )
{TRACE;
        m_tcppeerport = port;
}

/*-------------------------------------------------------------------------*/
        
UInt16 
CDRNController::GetTCPPeerComPort( void ) const
{TRACE;
        return m_tcppeercom.GetPort();
}

/*-------------------------------------------------------------------------*/
        
void 
CDRNController::SetUDPPeerComPort( UInt16 port )
{TRACE;
        m_udppeerport = port;
}
        
/*-------------------------------------------------------------------------*/
        
UInt16 
CDRNController::GetUDPPeerComPort( void ) const
{TRACE;
        return m_udppeercom.GetPort();
}

/*-------------------------------------------------------------------------*/

void
CDRNController::OnNewUserLoggedIn( COM::CBPTCPServerConnection& client ,
                                   UInt32 connection                   )
{TRACE;
        /*
         *      The user logged in and now needs the peer network information.
         *      we will send the info here.
         */                                                 //payloadsize                   controlitems   peeritems  handoffkey
        char netinfodata[ 13 ] = { DRN_CONTROLLERCOM_NETWORK_INFO, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
        UInt16 payloadsize( 13 ); 
        memcpy( netinfodata+1, &payloadsize, 2 );
        memcpy( netinfodata+4, &m_controlitems, 4 );
        memcpy( netinfodata+8, &m_peeritems, 4 );
        
        UInt32 handoffkey(0); //@TODO makeme
        memcpy( netinfodata+12, &handoffkey, 4 );
        
        MainComTCPSendData( client, netinfodata, 17 );
        
        /*
         *      Send peer list
         */
         
        
        /*
         *      Send inits for all control items
         */
        CORE::CDynamicBuffer databuffer;
        CORE::CDynamicBuffer buffer;
        CDRNLocalDataItem* dataitem;
        UInt16 datasize;
        UInt32 bufsize( 0 ), itemsize( 0 );               // payloadsize                        itemindex  datasize
        char transheader[ 7 ] = { DRN_CONTROLLERCOM_DATAITEM_INIT, 0,0,0,0, 0,0 };
        for ( UInt32 i=0; i<m_controlitems; ++i )
        {
                dataitem = static_cast<CDRNLocalDataItem*>( m_localdata[ i ] );                
                dataitem->CopyTo( databuffer );
                datasize = static_cast<UInt16>(databuffer.GetDataSize());
                payloadsize = datasize+7;
                
                memcpy( transheader+1, &payloadsize, 2 );
                memcpy( transheader+4, &i, 4 );
                memcpy( transheader+8, &datasize, 2 );
                                
                buffer.Clear();
                buffer.CopyFrom( 10          ,
                                 transheader );
                buffer.CopyFrom( 10                        ,
                                 databuffer.GetDataSize()  ,
                                 databuffer.GetBufferPtr() );               
                                  
                MainComTCPSendData( client                                            ,
                                    static_cast<const char*>( buffer.GetBufferPtr() ) ,
                                    buffer.GetDataSize()                              );
        }
        
        /*
         *      Send inits for all user items
         */         
         
}

/*-------------------------------------------------------------------------*/

bool 
CDRNController::SendEventToPeers( UInt32 eventid )
{TRACE;
        return false;        
}

/*-------------------------------------------------------------------------*/

void
CDRNController::MainComTCPSendData( COM::CBPTCPServerConnection& client ,
                                    const char* data                    ,
                                    UInt32 size                         )
{TRACE;
        if ( m_usemaincomencoder )
        {
                CORE::CDynamicBuffer buffer;
                m_iface->OnDRNMainComDataEncode( *this  ,
                                                 data   ,
                                                 size   ,
                                                 buffer );
                client.Send( static_cast<const char*>( buffer.GetBufferPtr() ) ,
                             buffer.GetDataSize()                              );
        }
        else
        {
                client.Send( data ,
                             size );                                        
        }        
}

/*-------------------------------------------------------------------------*/

void
CDRNController::OnPeerDataRecieved( UInt32 peerid    ,
                                    const char* data ,
                                    UInt32 size      )
{TRACE;
    /*    if ( size < 3 ) return;
        if ( peerid >  )
        
        UInt16 comkey = *(static_cast<UInt16*>( data ));
        
        switch ( data[ 3 ] )
        {
                case DRN_PEERCOM_EVENT :
                {
                        m_peerinfo        
                }
                case DRN_PEERCOM_DATAITEM_UPDATE :
                {
                }
                case DRN_PEERCOM_USERDATA :
                {
                }
                default :
                {
                        return;
                }
        } */       
}                                                                       

/*-------------------------------------------------------------------------*/

void
CDRNController::ProcessMainComClientData( COM::CBPTCPServerSocket &socket     ,
                                          COM::CBPTCPServerConnection& client ,
                                          UInt32 connection                   ,
                                          const char *data                    ,
                                          UInt32 size                         )
{TRACE;
        TMainComInfo* info = static_cast<TMainComInfo*>( m_maincominfo[ connection ] );
        
        if ( !info->loggedin )
        {       
                if ( data[ 0 ] == DRN_CONTROLLERCOM_LOGIN )
                {
                        //@TODO take maxpeers into account
                        
                        CORE::CString username;
                        CORE::CString password;
                        for ( UInt32 i=1; i<size; ++i )
                        {
                                if ( data[ i ] == DRN_TCP_STRINGSEPERATOR )
                                {
                                        username.Set( data+1, i-1 );
                                        if ( i+1 < size )
                                        {
                                                password.Set( data+i+1, size-i );
                                        }
                                }
                        }
                        
                        info->loggedin = m_iface->OnDRNLogin( *this    ,
                                                              username ,
                                                              password );
                                                                                                              
                        if ( info->loggedin )
                        {
                                info->username = username;
                                
                                char senddata = DRN_CONTROLLERCOM_LOGIN_SUCCESS;
                                MainComTCPSendData( client, &senddata, 1 );
                                
                                OnNewUserLoggedIn( client     ,
                                                   connection );
                        }
                        else
                        {
                                char senddata = DRN_CONTROLLERCOM_LOGIN_FAILED;
                                MainComTCPSendData( client, &senddata, 1 );                      
                        }
                }
                else
                {
                        client.Close();
                }
        }                        
}                                          

/*-------------------------------------------------------------------------*/

void 
CDRNController::OnMainComClientRead( COM::CBPTCPServerSocket &socket     ,
                                     COM::CBPTCPServerConnection& client ,
                                     UInt32 connection                   ,
                                     const char *data                    ,
                                     UInt32 size                         ,
                                     UInt32 &keepbytes                   )
{TRACE;
        if ( m_usemaincomencoder )
        {
                CORE::CDynamicBuffer buffer;
                m_iface->OnDRNMainComDataDecode( *this  ,
                                                 data   ,
                                                 size   ,
                                                 buffer );
                ProcessMainComClientData( socket                                            ,
                                          client                                            ,
                                          connection                                        ,  
                                          static_cast<const char*>( buffer.GetBufferPtr() ) ,
                                          buffer.GetDataSize()                              );
        }
        else
        {
                ProcessMainComClientData( socket     ,
                                          client     ,
                                          connection ,  
                                          data       ,
                                          size       );
        }                                                                                            
}                                     

/*-------------------------------------------------------------------------*/

void 
CDRNController::OnMainComClientWrite( COM::CBPTCPServerSocket &socket     ,
                                      COM::CBPTCPServerConnection& client ,
                                      UInt32 connection                   ,
                                      const char *data                    ,
                                      UInt32 size                         )
{TRACE;
}                                      

/*-------------------------------------------------------------------------*/

void 
CDRNController::OnMainComClientConnected( COM::CBPTCPServerSocket &socket        ,
                                          COM::CBPTCPServerConnection& clientcon ,
                                          UInt32 connection                      )
{TRACE;
        TMainComInfo* info = static_cast<TMainComInfo*>( m_maincominfo[ connection ] );
        info->connecttime = CORE::GUCEFGetTickCount();
        info->connected = true;
}

/*-------------------------------------------------------------------------*/                                          

void 
CDRNController::OnMainComClientDisconnect( COM::CBPTCPServerSocket &socket     ,
                                           COM::CBPTCPServerConnection& client ,
                                           UInt32 connection                     ,
                                           bool clientclose                      )
{TRACE;
        TMainComInfo* info = static_cast<TMainComInfo*>( m_maincominfo[ connection ] );
        if ( info->loggedin )
        {
                m_iface->OnDRNLogout( *this, info->username );
        }
        
        info->connected = false;                
}

/*-------------------------------------------------------------------------*/                                           

void 
CDRNController::OnMainComClientError( COM::CBPTCPServerSocket &socket     ,
                                      COM::CBPTCPServerConnection& client ,
                                      UInt32 connection                     ,
                                      TSocketError *errorcode               )
{TRACE;
}

/*-------------------------------------------------------------------------*/                                      

void 
CDRNController::OnMainComOpen( COM::CBPTCPServerSocket &socket )
{TRACE;
        if ( !m_tcppeercom.ListenOnPort( m_tcppeerport ) )
        {
                m_iface->OnDRNControllerOpenSocketsFailed( *this );
                Close();
                return;
        }
        
        if ( !m_udppeercom.Open( m_udppeerport ) )
        {
                m_iface->OnDRNControllerOpenSocketsFailed( *this );
                Close();
                return;        
        }        
}

/*-------------------------------------------------------------------------*/
        
void 
CDRNController::OnMainComClose( COM::CBPTCPServerSocket &socket )
{TRACE;
        m_tcppeercom.Close();
        m_udppeercom.Close();
}

/*-------------------------------------------------------------------------*/

void 
CDRNController::OnMainComMaxConnectionsChanged( COM::CBPTCPServerSocket &socket ,
                                                UInt32 old_max                  ,
                                                UInt32 new_max                  )
{TRACE;
        if ( old_max < new_max )
        {
                m_maincominfo.SetArraySize( new_max );
                
                TMainComInfo* info;
                for ( UInt32 i=old_max; i<new_max; ++i )
                {
                        info = new TMainComInfo;
                        info->peerid = 0;
                        info->connecttime = 0;
                        info->loggedin = false; 
                        info->connected = false;                
                        m_maincominfo.SetEntry( i, info );         
                }
                return;
        }
        
        for ( UInt32 i=new_max; i<old_max; ++i )
        {
                delete static_cast<TMainComInfo*>( m_maincominfo[ i ] );                
        }
        m_maincominfo.SetArraySize( new_max );        
}

/*-------------------------------------------------------------------------*/                                                

void 
CDRNController::OnMainComError( COM::CBPTCPServerSocket &socket ,
                                TSocketError errorcode            )
{TRACE;
} 

/*-------------------------------------------------------------------------*/

void 
CDRNController::ProcessTCPPeerComClientData( COM::CBPTCPServerSocket &socket     ,
                                             COM::CBPTCPServerConnection &client ,
                                             UInt32 connection                     ,
                                             const char *data                      ,
                                             UInt32 size                           )
{TRACE;
        
}

/*-------------------------------------------------------------------------*/                                             

void 
CDRNController::OnTCPPeerComClientRead( COM::CBPTCPServerSocket &socket     ,
                                        COM::CBPTCPServerConnection& client ,
                                        UInt32 connection                     ,
                                        const char *data                      ,
                                        UInt32 size                           ,
                                        UInt32 &keepbytes                     )
{TRACE;
        ProcessTCPPeerComClientData( socket     ,
                                     client     , 
                                     connection ,  
                                     data       ,
                                     size       );
}                                     

/*-------------------------------------------------------------------------*/

void 
CDRNController::OnTCPPeerComClientWrite( COM::CBPTCPServerSocket &socket     ,
                                         COM::CBPTCPServerConnection& client ,
                                         UInt32 connection                     ,
                                         const char *data                      ,
                                         UInt32 size                           )
{TRACE;
}                                      

/*-------------------------------------------------------------------------*/

void 
CDRNController::OnTCPPeerComClientConnected( COM::CBPTCPServerSocket &socket        ,
                                             COM::CBPTCPServerConnection& clientcon ,
                                             UInt32 connection                        )
{TRACE;        
        
}

/*-------------------------------------------------------------------------*/                                          

void 
CDRNController::OnTCPPeerComClientDisconnect( COM::CBPTCPServerSocket &socket     ,
                                              COM::CBPTCPServerConnection& client ,
                                              UInt32 connection                     ,
                                              bool clientclose                      )
{TRACE;
}

/*-------------------------------------------------------------------------*/                                           

void 
CDRNController::OnTCPPeerComClientError( COM::CBPTCPServerSocket &socket     ,
                                         COM::CBPTCPServerConnection& client ,
                                         UInt32 connection                     ,
                                         TSocketError *errorcode               )
{TRACE;
}

/*-------------------------------------------------------------------------*/                                      

void 
CDRNController::OnTCPPeerComOpen( COM::CBPTCPServerSocket &socket )
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
void 
CDRNController::OnTCPPeerComClose( COM::CBPTCPServerSocket &socket )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CDRNController::OnTCPPeerComMaxConnectionsChanged( COM::CBPTCPServerSocket &socket ,
                                                   UInt32 old_max                  ,
                                                   UInt32 new_max                  )
{TRACE;
}

/*-------------------------------------------------------------------------*/                                                

void 
CDRNController::OnTCPPeerComError( COM::CBPTCPServerSocket &socket ,
                                   TSocketError errorcode          )
{TRACE;
}

/*-------------------------------------------------------------------------*/

void 
CDRNController::OnUDPPeerPacketRecieved( CPHUDPSocket& socket                ,
                                         const CPHUDPSocket::TIPAddress& src ,
                                         const void* data                    ,
                                         const UInt32 datasize               ,
                                         const UInt16 packettype             ,
                                         const UInt32 packetnumber           )
{TRACE;
        
}

/*-------------------------------------------------------------------------*/

bool 
CDRNController::ObservePeerEvents( CORE::CObserver* observer ,
                                   Int32 event               )
{TRACE;
        return false;
}
                                   
/*-------------------------------------------------------------------------*/
        
bool 
CDRNController::ObservePeerDataItem( UInt32 peerindex          ,
                                     UInt32 itemindex          ,
                                     CORE::CObserver* observer )
{TRACE;
        return false;
}

/*-------------------------------------------------------------------------*/

void 
CDRNController::SetLoginTimeout( Int32 maxticks )
{TRACE;
        m_maxloginticks = maxticks;
}

/*-------------------------------------------------------------------------*/
        
Int32 
CDRNController::GetLoginTimeout( void ) const
{TRACE;
        return m_maxloginticks;
}

/*-------------------------------------------------------------------------*/

void 
CDRNController::Broadcast( const CDRNLocalDataItem* item )
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
