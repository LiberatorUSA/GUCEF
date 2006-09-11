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

#include <string.h>       /* needed for memset() */

#ifndef DVSTRUTILS_H
#include "dvstrutils.h"
#define DVSTRUTILS_H
#endif /* DVSTRUTILS_H ? */ 

#include "CIUDPSocketEventHandler.h"

#include "CUDPSocket.h" /* definition of the class implemented here */

#ifdef MSWIN_BUILD
  #define FD_SETSIZE 1      /* should set the size of the FD set struct to 1 for VC */
  #include <winsock2.h>
#else
 #ifdef LINUX_BUILD
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
 #endif
#endif 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Forward declarations of types used
 */
struct CUDPSocket::SUDPSockData
{
        #ifdef MSWIN_BUILD
        SOCKET sockid;
        struct sockaddr_in localaddress;
        #endif
};
typedef struct CUDPSocket::SUDPSockData TUDPSockData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CUDPSocket::CUDPSocket( bool blocking )
        : CSocket( ST_UDP )     ,
          _blocking( blocking ) ,
          m_port( 0 )
{
        _data = new TUDPSockData;
        memset( _data, 0, sizeof( TUDPSockData ) );
}

/*-------------------------------------------------------------------------*/

CUDPSocket::~CUDPSocket()
{
        Close( true );
        
        delete _data;
}

/*-------------------------------------------------------------------------*/

bool 
CUDPSocket::ResolveDest( const CORE::CString& destaddrstr ,
                         UInt16 destport                  ,  
                         TIPAddress& dest                 )
{
        if ( CORE::Check_If_IP( destaddrstr.C_String() ) )
        {
                dest.netaddr = inet_addr( destaddrstr.C_String() );                
                if ( dest.netaddr == INADDR_NONE ) return false;                
                dest.port = htons( destport );
                return true;
        }
        else
        {              
                struct hostent* retval;                              
                retval = gethostbyname( destaddrstr.C_String() );
                if ( retval )
                {
                        dest.netaddr = inet_addr( retval->h_addr_list[ 0 ] ); // <- werkt niet ??
                        dest.port = htons( destport );
                        return true;
                }
                return false;                   
        }
}

/*-------------------------------------------------------------------------*/                         

Int32
CUDPSocket::SendPacketTo( const TIPAddress& dest ,
                          const void* data       , 
                          UInt16 datasize        )
{
        if ( !_data->sockid )
        {
                Open();
        }
        
        struct sockaddr_in remote;
        //CLEAR_ADDR( &remote );
        remote.sin_addr.s_addr = dest.netaddr;
        remote.sin_port = dest.port;
        remote.sin_family = AF_INET;
        return sendto( _data->sockid             , 
                       (const char*)data         , 
                       datasize                  , 
                       0                         , 
                       (struct sockaddr*)&remote , 
                       sizeof(remote)            );
}

/*-------------------------------------------------------------------------*/

Int32 
CUDPSocket::SendPacketTo( const CORE::CString& dnsname ,
                          UInt16 port                  ,
                          const void* data             , 
                          UInt16 datasize              )
{
        TIPAddress dest;
        if ( ResolveDest( dnsname, port, dest ) )
        {
                return SendPacketTo( dest, data, datasize );
        }
        return -1;
}

/*-------------------------------------------------------------------------*/

void 
CUDPSocket::Update( UInt32 tickcount  ,
                    UInt32 deltaticks )
{
        if ( _checkfordata )
        {
                CheckForData();
        } 
}                    

/*-------------------------------------------------------------------------*/

void
CUDPSocket::CheckForData( void )
{
        TIMEVAL time = { 0, 0 };
        fd_set sockset;
        FD_ZERO( &sockset );
        FD_SET( _data->sockid, &sockset ); 

        if ( select( 2, &sockset, NULL, NULL, &time ) > 0 )
        {
                _checkfordata = false;
                SendEvent( UDP_PACKET_RECIEVED );                                        
        }
        _checkfordata = true;
}
/*-------------------------------------------------------------------------*/

bool 
CUDPSocket::ResolveSrc( const TIPAddress& src     ,
                        CORE::CString& srcaddrstr ,
                        UInt16& srcport           )
{                   // @todo MAKEME
        if ( src.netaddr == INADDR_ANY )
        {
                srcaddrstr = "localhost";       
        }
        return true;        
}                        

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::Recieve( TIPAddress& src ,
                     void* destbuf   , 
                     UInt16 bufsize  )
{        
        struct sockaddr_in remote;        
        int structsize( sizeof( remote ) );
        int retval = recvfrom( _data->sockid             , 
                               (char*)destbuf            , 
                               bufsize                   , 
                               0                         , 
                               (struct sockaddr*)&remote ,
                               &structsize               );
        if ( retval < 0 ) 
        {
                return -1;
        }                               
        if ( retval == 0 )
        {
                Close( true );
                return 0;       
        }                         
                       
        src.port = remote.sin_port;
        src.netaddr = remote.sin_addr.s_addr;                       
                        
        CheckForData();
        return retval;                             
}

/*-------------------------------------------------------------------------*/

Int32 
CUDPSocket::Recieve( void* destbuf      , 
                     UInt16 bufsize     )
{
        int retval = recvfrom( _data->sockid  ,
                               (char*)destbuf , 
                               bufsize        , 
                               0              , 
                               NULL           ,
                               NULL           );
        if ( retval < 0 ) 
        {
                return -1;
        }                               
        if ( retval == 0 )
        {
                Close( true );
                return 0;       
        }                         
                
        CheckForData();
        return retval;
}                     

/*-------------------------------------------------------------------------*/
       
bool 
CUDPSocket::Open( void )
{
        Close( true );           
        
        //----------- BEGIN MS WINDOWS IMPLEMENTATION        
        #ifdef MSWIN_BUILD
        
        _data->sockid = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
        if ( _data->sockid == INVALID_SOCKET ) return false;
        
        // Set the desired blocking mode
        int mode = _blocking;
        if ( ioctlsocket( _data->sockid      , 
                          FIONBIO            , 
                          (u_long FAR*)&mode ) == SOCKET_ERROR )
        {
                return false;
        }        
        
        int size( sizeof(_data->localaddress) );
        getsockname( _data->sockid, (struct sockaddr*)&_data->localaddress, &size );
        SendEvent( UDP_SOCKET_OPENED );
        //m_port = ;  @MAKEME
        return true;
        
        #endif /* MSWIN_BUILD ? */
        //----------- END MS WINDOWS IMPLEMENTATION
}       

/*-------------------------------------------------------------------------*/ 
        
bool 
CUDPSocket::Open( UInt16 port )
{
        Close( true );
        
        //----------- BEGIN MS WINDOWS IMPLEMENTATION
        #ifdef MSWIN_BUILD
        
        _data->sockid = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
        if ( _data->sockid == INVALID_SOCKET ) return false;

        // Set the desired blocking mode
        int mode = _blocking;
        if ( ioctlsocket( _data->sockid      , 
                          FIONBIO            , 
                          (u_long FAR*)&mode ) == SOCKET_ERROR )
        {
                return false;
        }
        
        //CLEAR_ADDR( &_data->localaddress );
        _data->localaddress.sin_family = AF_INET;
        _data->localaddress.sin_port = htons( port );
        _data->localaddress.sin_addr.s_addr = htonl( INADDR_ANY );        
        
        if ( bind( _data->sockid                            , 
                   (struct sockaddr *) &_data->localaddress , 
                   sizeof(struct sockaddr_in)               ) == 0 )
        {
                m_port = port;
                SendEvent( UDP_SOCKET_OPENED );
                return true;
        }
        return false;
        
        #endif /* MSWIN_BUILD ? */
        //----------- END MS WINDOWS IMPLEMENTATION
}

/*-------------------------------------------------------------------------*/

bool 
CUDPSocket::Open( const CORE::CString& localaddr ,
                  UInt16 port                    )
{
        Close( true );
        
        //----------- BEGIN MS WINDOWS IMPLEMENTATION
        #ifdef MSWIN_BUILD
        
        _data->sockid = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
        if ( _data->sockid != INVALID_SOCKET ) return false;

        // Set the desired blocking mode
        int mode = _blocking;
        if ( ioctlsocket( _data->sockid      , 
                          FIONBIO            , 
                          (u_long FAR*)&mode ) == SOCKET_ERROR )
        {
                return false;
        }
        
        //CLEAR_ADDR( &_data->localaddress );
        _data->localaddress.sin_family = AF_INET;
        _data->localaddress.sin_port = htons( port );
        _data->localaddress.sin_addr.s_addr = inet_addr( localaddr.C_String() );
        
        if ( bind( _data->sockid                            , 
                   (struct sockaddr *) &_data->localaddress , 
                   sizeof(struct sockaddr_in)               ) == 0 )
        {
                m_port = port;
                SendEvent( UDP_SOCKET_OPENED );
                return true;
        }
        return false;
        
        #endif /* MSWIN_BUILD ? */
        //----------- END MS WINDOWS IMPLEMENTATION       
}

/*-------------------------------------------------------------------------*/  
        
void 
CUDPSocket::Close( bool force )
{
        if ( IsActive() )
        {
                force ? closesocket( _data->sockid ) : shutdown( _data->sockid, 1 );
        
                SendEvent( UDP_SOCKET_CLOSED );
        }                
}

/*-------------------------------------------------------------------------*/
 
bool 
CUDPSocket::IsBlocking( void ) const
{
       return _blocking;
}

/*-------------------------------------------------------------------------*/

bool 
CUDPSocket::IsActive( void ) const
{
        return _data->sockid > 0;
}

/*-------------------------------------------------------------------------*/

void 
CUDPSocket::SendEvent( TUDPSocketEvent event )
{
        CIUDPSocketEventHandler* handler;
        UInt32 count = _ifaces.GetCount();
        for ( UInt32 i=0; i<count; ++i )
        {
                handler = static_cast<CIUDPSocketEventHandler*>(_ifaces[ i ]);
                if ( handler )
                {
                        handler->OnUDPSocketEvent( *this, event );
                }
        }                
}
 
/*-------------------------------------------------------------------------*/
 
void 
CUDPSocket::RegisterEventHandler( CIUDPSocketEventHandler* handler )
{
        handler->_index = _ifaces.AddEntry( handler );
}

/*-------------------------------------------------------------------------*/
        
void 
CUDPSocket::UnregisterEventHandler( CIUDPSocketEventHandler* handler )
{
        _ifaces.SetEntry( handler->_index, NULL );
}

/*-------------------------------------------------------------------------*/

UInt16 
CUDPSocket::GetPort( void ) const
{
        return m_port;
}
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
 