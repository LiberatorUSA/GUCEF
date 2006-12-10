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

#include "CUDPSocket.h" /* definition of the class implemented here */

#ifdef GUCEF_MSWIN_BUILD
  #define FD_SETSIZE 1      /* should set the size of the FD set struct to 1 for VC */
  #include <winsock2.h>
#else
 #ifdef GUCEF_LINUX_BUILD
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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CUDPSocket::UDPSocketErrorEvent = "GUCEF::COMCORE::CUDPSocket::UDPSocketErrorEvent";
const CORE::CEvent CUDPSocket::UDPSocketClosedEvent = "GUCEF::COMCORE::CUDPSocket::UDPSocketClosedEvent";
const CORE::CEvent CUDPSocket::UDPSocketOpenedEvent = "GUCEF::COMCORE::CUDPSocket::UDPSocketOpenedEvent";
const CORE::CEvent CUDPSocket::UDPPacketRecievedEvent = "GUCEF::COMCORE::CUDPSocket::UDPPacketRecievedEvent";

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
        #ifdef GUCEF_MSWIN_BUILD
        SOCKET sockid;
        struct sockaddr_in localaddress;
        #endif
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CUDPSocket::CUDPSocket( bool blocking )
        : CSocket()             ,
          _blocking( blocking ) ,
          m_port( 0 )           ,
          _data( NULL )         ,
          m_buffer()
{TRACE;
        _data = new TUDPSockData;
        memset( _data, 0, sizeof( TUDPSockData ) );
        
        m_buffer.SetBufferSize( 1024 );
}

/*-------------------------------------------------------------------------*/

CUDPSocket::~CUDPSocket()
{TRACE;
        Close( true );
        
        delete _data;
        _data = NULL;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::RegisterEvents( void )
{TRACE;

    UDPSocketErrorEvent.Initialize();
    UDPSocketClosedEvent.Initialize();
    UDPSocketOpenedEvent.Initialize();
    UDPPacketRecievedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/                         

Int32
CUDPSocket::SendPacketTo( const TIPAddress& dest ,
                          const void* data       , 
                          UInt16 datasize        )
{TRACE;
        if ( !_data->sockid )
        {
                Open();
        }
        
        struct sockaddr_in remote;
        memset( &remote, 0, sizeof( remote ) ); // should this be CLEAR_ADDR( &remote ); ?
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
{TRACE;
        TIPAddress dest;
        if ( ConvertToIPAddress( dnsname, port, dest ) )
        {
                return SendPacketTo( dest, data, datasize );
        }
        return -1;
}

/*-------------------------------------------------------------------------*/

void 
CUDPSocket::Update( UInt32 tickcount  ,
                    UInt32 deltaticks )
{TRACE;

    if ( !_blocking )
    {
        TIPAddress src;
        while ( IsIncomingDataQueued() )
        {
            Recieve( src  ,
                     NULL ,
                     0    );
        }
    }
}                    

/*-------------------------------------------------------------------------*/

bool
CUDPSocket::IsIncomingDataQueued( void ) const
{TRACE;
        TIMEVAL time = { 0, 0 };
        fd_set sockset;
        FD_ZERO( &sockset );
        FD_SET( _data->sockid, &sockset ); 

        if ( select( 2, &sockset, NULL, NULL, &time ) > 0 )
        {
                return true;
        }
        return false;
}                    

/*-------------------------------------------------------------------------*/

UInt32
CUDPSocket::GetRecievedDataBufferSize( void ) const
{TRACE;
    
    return m_buffer.GetBufferSize();
}

/*-------------------------------------------------------------------------*/
    
void
CUDPSocket::SetRecievedDataBufferSize( const UInt32 newBufferSize )
{TRACE;
    
    m_buffer.SetBufferSize( newBufferSize );
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::Recieve( TIPAddress& src ,
                     void* destbuf   , 
                     UInt16 bufsize  )
{TRACE;        

    struct sockaddr_in remote;        
    int structsize( sizeof( remote ) );
    int retval = recvfrom( _data->sockid                  , 
                           (char*)m_buffer.GetBufferPtr() , 
                           m_buffer.GetBufferSize()       , 
                           0                              , 
                           (struct sockaddr*)&remote      ,
                           &structsize                    );
    if ( retval < 0 ) 
    {
            return -1;
    }                               
    if ( retval == 0 )
    {
            Close( true );
            return 0;       
    }                         
    
    // Set the actual usefull bytes of data in the buffer 
    // to match the number of retrieved bytes
    m_buffer.SetDataSize( retval );
                   
    // Copy the data directly to our user
    src.port = remote.sin_port;
    src.netaddr = remote.sin_addr.s_addr;
    
    if ( NULL != destbuf )
    {
        UInt32  copyLimit = bufsize;
        if ( m_buffer.GetBufferSize() < copyLimit )
        {
            copyLimit = m_buffer.GetBufferSize(); 
        }                       
        memcpy( destbuf, m_buffer.GetBufferPtr(), copyLimit );
    }
                     
    // Send an event notifying people that the data was recieved
    struct SUDPPacketRecievedEventData infoStruct;
    infoStruct.sourceAddress.port = remote.sin_port;
    infoStruct.sourceAddress.netaddr = remote.sin_addr.s_addr;
    infoStruct.dataBuffer = CORE::TLinkedCloneableBuffer( &m_buffer );
    
    UDPPacketRecievedEventData eventData( infoStruct );
    NotifyObservers( UDPPacketRecievedEvent, &eventData );        
    
    return retval;                             
}

/*-------------------------------------------------------------------------*/

Int32 
CUDPSocket::Recieve( void* destbuf  ,
                     UInt16 bufsize )
{TRACE;

    TIPAddress src;
    return Recieve( src     , 
                    destbuf ,
                    bufsize );
}                     

/*-------------------------------------------------------------------------*/
       
bool 
CUDPSocket::Open( void )
{TRACE;
        GUCEF_ASSERT_ALWAYS;
        
        Close( true );           
        
        //----------- BEGIN MS WINDOWS IMPLEMENTATION        
        #ifdef GUCEF_MSWIN_BUILD
        
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
        NotifyObservers( UDPSocketOpenedEvent );
        //m_port = ;  @MAKEME
        return true;
        
        #endif /* GUCEF_MSWIN_BUILD ? */
        //----------- END MS WINDOWS IMPLEMENTATION
}       

/*-------------------------------------------------------------------------*/ 
        
bool 
CUDPSocket::Open( UInt16 port )
{TRACE;
        Close( true );
        
        //----------- BEGIN MS WINDOWS IMPLEMENTATION
        #ifdef GUCEF_MSWIN_BUILD
        
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
                NotifyObservers( UDPSocketOpenedEvent );
                return true;
        }
        return false;
        
        #endif /* GUCEF_MSWIN_BUILD ? */
        //----------- END MS WINDOWS IMPLEMENTATION
}

/*-------------------------------------------------------------------------*/

bool 
CUDPSocket::Open( const CORE::CString& localaddr ,
                  UInt16 port                    )
{TRACE;
        Close( true );
        
        //----------- BEGIN MS WINDOWS IMPLEMENTATION
        #ifdef GUCEF_MSWIN_BUILD
        
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
                NotifyObservers( UDPSocketOpenedEvent );
                return true;
        }
        return false;
        
        #endif /* GUCEF_MSWIN_BUILD ? */
        //----------- END MS WINDOWS IMPLEMENTATION       
}

/*-------------------------------------------------------------------------*/  
        
void 
CUDPSocket::Close( bool force )
{TRACE;

    if ( IsActive() )
    {
        force ? closesocket( _data->sockid ) : shutdown( _data->sockid, 1 );

        NotifyObservers( UDPSocketClosedEvent );
    }                
}

/*-------------------------------------------------------------------------*/
 
bool 
CUDPSocket::IsBlocking( void ) const
{TRACE;
       return _blocking;
}

/*-------------------------------------------------------------------------*/

bool 
CUDPSocket::IsActive( void ) const
{TRACE;
        return _data->sockid > 0;
}

/*-------------------------------------------------------------------------*/

UInt16 
CUDPSocket::GetPort( void ) const
{TRACE;
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
 