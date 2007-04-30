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

//-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------//

#include <assert.h>

#ifndef DVSTRUTILS_H
#include "dvstrutils.h"
#define DVSTRUTILS_H
#endif /* DVSTRUTILS_H ? */

#ifdef GUCEF_MSWIN_BUILD

#ifndef DVWINSOCK_H
#include "dvwinsock.h"
#define DVWINSOCK_H
#endif /* DVWINSOCK_H ? */

#ifdef GUCEF_MSWIN_BUILD
  #define FD_SETSIZE 1      /* should set the size of the FD set struct to 1 for VC */
  #include <winsock2.h>
  #include <Ws2tcpip.h>
  #include <Wspiapi.h>
#else
 #ifdef GUCEF_LINUX_BUILD
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
 #endif
#endif

#endif /* GUCEF_MSWIN_BUILD ? */

#include "CTCPClientSocket.h"           /* Header with class for these utils */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

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

const CORE::CEvent CTCPClientSocket::ConnectingEvent = "GUCEF::COMCORE::CTCPClientSocket::ConnectingEvent";

//-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------//

#define BUFFER_READ_SIZE        16

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      struct with os implementation specific data
 */
struct CTCPClientSocket::STCPClientSockData
{
        #ifdef GUCEF_MSWIN_BUILD
        LPHOSTENT hostent;
        SOCKET sockid;
        SOCKADDR_IN serverinfo;
        #else
          #ifdef GUCEF_LINUX_BUILD
          #endif
        #endif  
};
typedef struct CTCPClientSocket::STCPClientSockData TTCPClientSockData;

//-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------//

CTCPClientSocket::CTCPClientSocket( void )
        : CTCPConnection()
{TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket::CTCPClientSocket( bool blocking ) 
        : CTCPConnection()         ,
          _blocking( blocking )    ,
          _active( false )         ,
          m_maxreadbytes( 0 )
{TRACE;

    RegisterEvents();
    
    _data = new TTCPClientSockData;             
    assert( _data != NULL );
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket::~CTCPClientSocket()
{TRACE;

    Close();
    delete _data;
    _data = NULL;
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket&
CTCPClientSocket::operator=( const CTCPClientSocket& src )
{TRACE;                    
        /* dummy, do not use */   
        return *this;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::IsActive( void ) const
{TRACE;
        
        return _active;
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::RegisterEvents( void )
{TRACE;
    
    ConnectingEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

bool 
CTCPClientSocket::IsBlocking( void ) const
{TRACE;
    
    return _blocking;
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::SetMaxRead( UInt32 mr )
{TRACE;
        
    LockData();        
    m_maxreadbytes = mr;
    UnlockData();
}

/*-------------------------------------------------------------------------*/

UInt32
CTCPClientSocket::GetMaxRead( void ) const
{TRACE;
        return m_maxreadbytes;
}

/*-------------------------------------------------------------------------*/

/**
 *	Attempts to reconnect to the server provided with
 *      Connect_To(). If Connect_To() has not yet been called then this
 *	member function has no effect.
 */
bool
CTCPClientSocket::Reconnect( void )
{TRACE;

        LockData();
        bool retval = ConnectTo( _remoteaddr ,
                                 _remoteport );
        UnlockData();
        return retval;
}

/*-------------------------------------------------------------------------*/

/**
 *      attempt connection to server given
 */
bool
CTCPClientSocket::ConnectTo( const CORE::CString& remoteaddr , 
                             UInt16 port                     )
{TRACE;

    if ( remoteaddr.Length() == 0 ) return false;  

    LockData();
    
    Close();        
    
    _active = true;
    
    NotifyObservers( ConnectingEvent );
    
    int errorcode = 0;
    if ( CORE::Check_If_IP( remoteaddr.C_String() ) == 1 )
    {
            UInt32 addr = inet_addr( remoteaddr.C_String() );
            _data->hostent = WSTS_gethostbyaddr( (char*) &addr , 
                                                  4            , 
                                                  AF_INET      ,
                                                  &errorcode   );                                             
    }
    else
    {
            _data->hostent = WSTS_gethostbyname( remoteaddr.C_String() ,
                                                 &errorcode            );
    }
    
    // Check for an error
    if ( errorcode != 0 )
    {
        // Notify our users of the error
        TSocketErrorEventData eData( errorcode );
        NotifyObservers( SocketErrorEvent, &eData );
        
        // After a socket error you must always close the connection.
        _active = false;
        
        UnlockData();
        return false;
    }    
                                        
    if ( NULL == _data->hostent ) return false;

    _data->sockid = WSTS_socket( AF_INET     ,  /* Go over TCP/IP */
                                 SOCK_STREAM ,  /* This is a stream-oriented socket */
                                 IPPROTO_TCP ,  /* Use TCP rather than UDP */
                                 &errorcode  );

    // Check for an error
    if ( errorcode != 0 )
    {
        // Notify our users of the error
        TSocketErrorEventData eData( errorcode );
        NotifyObservers( SocketErrorEvent, &eData );
        
        // After a socket error you must always close the connection.
        _active = false;
        
        UnlockData();
        return false;
    }
                                 
    if ( _data->sockid == INVALID_SOCKET ) return false;
    
    /* Set the desired blocking mode */
    int mode = _blocking;
    if ( ioctlsocket( _data->sockid      , 
                      FIONBIO            , 
                      (u_long FAR*)&mode ) == SOCKET_ERROR )
    {
            _active = false;
            return false;
    }        

    /* we want to use the internet protocol */
    _data->serverinfo.sin_family = AF_INET;
        
	/*
	 *      At this point, we've successfully retrieved vital information about the server,
	 *      including its hostname, aliases, and IP addresses.
	 */
	_data->serverinfo.sin_addr = *( (LPIN_ADDR)(*_data->hostent->h_addr_list) );

        /*      
         *      Change port number to network-byte order and
         *      then insert into the server port info field
         */
	_data->serverinfo.sin_port = htons( port );
	
	/*
	 *      We now attempt to create a connection.
	 */
    if ( WSTS_connect( _data->sockid                  ,
                       (LPSOCKADDR)&_data->serverinfo ,
                       sizeof( struct sockaddr)       ,
                       &errorcode                     ) == SOCKET_ERROR )
    {
        _active = false;
        
        // Check for an error
        if ( errorcode != 0 )
        {
            // Notify our users of the error
            TSocketErrorEventData eData( errorcode );
            NotifyObservers( SocketErrorEvent, &eData );
        }        
                
        // After a socket error you must always close the connection.
        _active = false;
        
        UnlockData();
        return false;
    }
        
    _remoteaddr = remoteaddr;
    _remoteport = port;
    
    NotifyObservers( ConnectedEvent );	                   

    UnlockData();        
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::ConnectTo( const TIPAddress& address )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CTCPClientSocket::GetRemoteHostName( void ) const
{GUCEF_TRACE;

    return _remoteaddr;
}

/*-------------------------------------------------------------------------*/
    
UInt16
CTCPClientSocket::GetRemoteTCPPort( void ) const
{GUCEF_TRACE;
    
    return ntohs( _data->serverinfo.sin_port );
}

/*-------------------------------------------------------------------------*/
    
CTCPClientSocket::TIPAddress
CTCPClientSocket::GetRemoteIP( void ) const
{GUCEF_TRACE;

    TIPAddress ip = { _data->serverinfo.sin_port, _data->serverinfo.sin_addr.S_un.S_addr };
    return ip;
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::CheckRecieveBuffer( void )
{TRACE;        

    if ( !_blocking && _active )
    {       
        LockData();
                 
        /*
         *      Since this is a non-blocking socket we need to poll for received data
         */
        int bytesrecv;
        int errorcode = 0;
        UInt32 readblocksize;
        m_maxreadbytes > 0 ? readblocksize = m_maxreadbytes : readblocksize = 1024;
        
        // reset our logical data size delimiter
        m_readbuffer.SetDataSize( 0 );
        
        do
        {                 
            // make sure the buffer can hold another read block
            m_readbuffer.SetBufferSize( m_readbuffer.GetDataSize()+readblocksize );
            
            // read an additional block
            bytesrecv = WSTS_recv( _data->sockid                                                                , 
                                   static_cast<char*>(m_readbuffer.GetBufferPtr()) + m_readbuffer.GetDataSize() ,
                                   readblocksize                                                                ,
                                   0                                                                            ,
                                   &errorcode                                                                   );
            
            // Increase the logical data size delimiter by the amount we just copied into the buffer
            m_readbuffer.SetDataSize( m_readbuffer.GetDataSize() + bytesrecv );
                        
            // Check for an error
            if ( ( bytesrecv < 0 ) || ( errorcode != 0 ) )
            {
                // Notify our users of the error
                TSocketErrorEventData eData( errorcode );
                NotifyObservers( SocketErrorEvent, &eData );
                
                // After a socket error you must always close the connection.
                Close();
                
                UnlockData();
                return;
            }
            else if ( bytesrecv == 0 )
            {
                // The connection has been closed on us
                Close();

                UnlockData();
                return;
            }

            if ( m_maxreadbytes > 0 )
            {
                if ( m_maxreadbytes <= m_readbuffer.GetDataSize() )
                {
                        break;
                }
            }
        }
        
        while ( bytesrecv == readblocksize );
        
        if ( m_readbuffer.GetDataSize() > 0 )
        {
            CORE::CDynamicBuffer linkedBuffer;
            linkedBuffer.LinkTo( m_readbuffer.GetConstBufferPtr(), m_readbuffer.GetDataSize() );
            TDataRecievedEventData cData( &linkedBuffer );
            NotifyObservers( DataRecievedEvent, &cData );
        }
                      
        UnlockData();      
    }                 
}

/*-------------------------------------------------------------------------*/

void 
CTCPClientSocket::Update( void )
{TRACE;        
    
    CheckRecieveBuffer();
}                          

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::Close( void )
{TRACE;

        /*
         *      close the socket connection
         */
        LockData();
        if ( IsActive() )
        {
                /* prevent any further sends on the socket */
                shutdown( _data->sockid , 
                          1             );
                        
                /* check if there is any data still in the buffer */
        //        fd_set readset;
        //        FD_SET( readset,           
          //      select( 0, /* ignored parameter for compatibility */ 
            //             ,
              //           NULL,
                //         NULL,
                  //       );
                                          
                //recv( _data->sockid );
                                          
                /* close the socket, this de-allocates the winsock buffers for the socket */          
                closesocket( _data->sockid );
                _active = false;
                
                NotifyObservers( DisconnectedEvent );
        }
        UnlockData();
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::Send( const void* dataSource , 
                        const UInt16 dataSize  )
{GUCEF_TRACE;

    return Send( dataSource ,
                 dataSize   ,
                 0          );
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::Send( const void* data ,
                        UInt32 length    ,
                        UInt32 timeout   )
{TRACE;

        /*
         *      Write data to socket
         */
        if ( IsActive() )
        {
                LockData();
                
                CORE::CDynamicBuffer linkedBuffer;
                linkedBuffer.LinkTo( data, length );
                TDataRecievedEventData cData( &linkedBuffer );
                NotifyObservers( DataSentEvent, &cData );
                                        
                int error;
                Int32 wbytes = WSTS_send( _data->sockid ,  
                                          data          , 
                                          length        , 
                                          0             , 
                                          &error        );
                
                UnlockData();
                return wbytes == length;
        } 
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::Send( const CORE::CString& data )
{TRACE;

        return Send( data.C_String() ,
                     data.Length()   ,
                     0               );
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::LockData( void ) const
{TRACE;

    datalock.Lock();
}

/*-------------------------------------------------------------------------*/
    
void
CTCPClientSocket::UnlockData( void ) const
{TRACE;
    
    datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

CORE::CString
CTCPClientSocket::GetType( void ) const
{TRACE;

    return "GUCEF::COMCORE::CTCPClientSocket";
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
