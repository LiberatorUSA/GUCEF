/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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
{GUCEF_TRACE;
        
        RegisterEvents();
        
        _data = new TUDPSockData;
        memset( _data, 0, sizeof( TUDPSockData ) );
        
        m_buffer.SetBufferSize( 1024 );
}

/*-------------------------------------------------------------------------*/

CUDPSocket::~CUDPSocket()
{GUCEF_TRACE;
        Close( true );
        
        delete _data;
        _data = NULL;
}

/*-------------------------------------------------------------------------*/

void
CUDPSocket::RegisterEvents( void )
{GUCEF_TRACE;

    UDPSocketErrorEvent.Initialize();
    UDPSocketClosedEvent.Initialize();
    UDPSocketOpenedEvent.Initialize();
    UDPPacketRecievedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/                         

Int32
CUDPSocket::SendPacketTo( const CIPAddress& dest ,
                          const void* data       , 
                          UInt16 datasize        )
{GUCEF_TRACE;
        if ( !_data->sockid )
        {
                Open();
        }
        
        struct sockaddr_in remote;
        memset( &remote, 0, sizeof( remote ) ); // should this be CLEAR_ADDR( &remote ); ?
        remote.sin_addr.s_addr = dest.GetAddress();
        remote.sin_port = dest.GetPort();
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
{GUCEF_TRACE;
        CIPAddress dest;
        if ( ConvertToIPAddress( dnsname, port, dest ) )
        {
                return SendPacketTo( dest, data, datasize );
        }
        return -1;
}

/*-------------------------------------------------------------------------*/

void 
CUDPSocket::Update( void )
{GUCEF_TRACE;

    if ( !_blocking )
    {
        CIPAddress src;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
    
    return m_buffer.GetBufferSize();
}

/*-------------------------------------------------------------------------*/
    
void
CUDPSocket::SetRecievedDataBufferSize( const UInt32 newBufferSize )
{GUCEF_TRACE;
    
    m_buffer.SetBufferSize( newBufferSize );
}

/*-------------------------------------------------------------------------*/

Int32
CUDPSocket::Recieve( CIPAddress& src ,
                     void* destbuf   , 
                     UInt16 bufsize  )
{GUCEF_TRACE;        

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
    src.SetPort( remote.sin_port );
    src.SetAddress( remote.sin_addr.s_addr );
    
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
    infoStruct.sourceAddress.SetPort( remote.sin_port );
    infoStruct.sourceAddress.SetAddress( remote.sin_addr.s_addr );
    infoStruct.dataBuffer = CORE::TLinkedCloneableBuffer( &m_buffer );
    
    UDPPacketRecievedEventData eventData( infoStruct );
    NotifyObservers( UDPPacketRecievedEvent, &eventData );        
    
    return retval;                             
}

/*-------------------------------------------------------------------------*/

Int32 
CUDPSocket::Recieve( void* destbuf  ,
                     UInt16 bufsize )
{GUCEF_TRACE;

    CIPAddress src;
    return Recieve( src     , 
                    destbuf ,
                    bufsize );
}                     

/*-------------------------------------------------------------------------*/
       
bool 
CUDPSocket::Open( void )
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;

    if ( IsActive() )
    {
        force ? closesocket( _data->sockid ) : shutdown( _data->sockid, 1 );

        NotifyObservers( UDPSocketClosedEvent );
    }                
}

/*-------------------------------------------------------------------------*/
 
bool 
CUDPSocket::IsBlocking( void ) const
{GUCEF_TRACE;
       return _blocking;
}

/*-------------------------------------------------------------------------*/

bool 
CUDPSocket::IsActive( void ) const
{GUCEF_TRACE;
        return _data->sockid > 0;
}

/*-------------------------------------------------------------------------*/

UInt16 
CUDPSocket::GetPort( void ) const
{GUCEF_TRACE;
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
 