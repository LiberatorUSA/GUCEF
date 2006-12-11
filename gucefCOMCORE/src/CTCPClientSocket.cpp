/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef DVWINSOCK_H
#include "dvwinsock.h"                  /* threadsafe winsock wrapper */
#define DVWINSOCK_H
#endif /* DVWINSOCK_H ? */

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
const CORE::CEvent CTCPClientSocket::ConnectedEvent = "GUCEF::COMCORE::CTCPClientSocket::ConnectedEvent";
const CORE::CEvent CTCPClientSocket::DisconnectedEvent = "GUCEF::COMCORE::CTCPClientSocket::DisconnectedEvent";
const CORE::CEvent CTCPClientSocket::DataRecievedEvent = "GUCEF::COMCORE::CTCPClientSocket::DataRecievedEvent";
const CORE::CEvent CTCPClientSocket::DataSentEvent = "GUCEF::COMCORE::CTCPClientSocket::DataSentEvent";
const CORE::CEvent CTCPClientSocket::SocketErrorEvent = "GUCEF::COMCORE::CTCPClientSocket::SocketErrorEvent";

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
        : CSocket()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket::CTCPClientSocket( bool blocking ) 
        : CSocket()                ,
          _blocking( blocking )    ,
          _active( false )         ,
          m_maxreadbytes( 0 )
{TRACE;

        /*
         *      Constructor,
         *      init vars
         */
         
        _data = new TTCPClientSockData;             
        assert( _data );
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket::~CTCPClientSocket()
{TRACE;

        /*
         *      Constructor,
         *      init vars
         */
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
    ConnectedEvent.Initialize();
    DisconnectedEvent.Initialize();
    DataRecievedEvent.Initialize();
    DataSentEvent.Initialize();
    SocketErrorEvent.Initialize();    
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
    if ( CORE::Check_If_IP( remoteaddr.C_String() ) )
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
                                        
    if ( !_data->hostent ) return false;

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
        UInt32 totalrecieved( 0 );
        int errorcode = 0;
        UInt32 readblocksize;
        m_maxreadbytes ? readblocksize = m_maxreadbytes : readblocksize = 1024;
        do
        {                 
            m_readbuffer.SetDataSize( m_readbuffer.GetDataSize()+readblocksize );
            bytesrecv = WSTS_recv( _data->sockid                                                 , 
                                   static_cast<char*>(m_readbuffer.GetBufferPtr())+totalrecieved ,
                                   readblocksize                                                 ,
                                   0                                                             ,
                                   &errorcode                                                    );
            
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
            
            totalrecieved += bytesrecv;
            if ( m_maxreadbytes )
            {
                if ( m_maxreadbytes <= totalrecieved )
                {
                        break;
                }
            }
        }
        
        while ( bytesrecv == readblocksize );
        
        if ( totalrecieved > 0 )
        {
            CORE::CDynamicBuffer linkedBuffer;
            linkedBuffer.LinkTo( m_readbuffer.GetBufferPtr(), totalrecieved );
            TDataRecievedEventData cData( &linkedBuffer );
            NotifyObservers( DataRecievedEvent, &cData );
        }
                      
        UnlockData();      
    }                 
}

/*-------------------------------------------------------------------------*/

void 
CTCPClientSocket::Update( UInt32 tickcount  ,
                          UInt32 deltaticks )
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
