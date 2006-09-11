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

#ifndef DVSTRUTILS_H
#include "dvstrutils.h"
#define DVSTRUTILS_H
#endif /* DVSTRUTILS_H ? */

#ifndef DVWINSOCK_H
#include "dvwinsock.h"                  /* threadsafe winsock wrapper */
#define DVWINSOCK_H
#endif /* DVWINSOCK_H ? */

#ifndef CTCPCLIENTSOCKETINTERFACE_H
#include "CTCPClientSocketInterface.h"  /* virtual Event handling interface */
#define CTCPCLIENTSOCKETINTERFACE_H
#endif /* CTCPCLIENTSOCKETINTERFACE_H ? */

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
        #ifdef MSWIN_BUILD
        LPHOSTENT hostent;
        SOCKET sockid;
        SOCKADDR_IN serverinfo;
        #else
          #ifdef LINUX_BUILD
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
        : CSocket( ST_TCP_CLIENT )
{
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket::CTCPClientSocket( bool blocking ) 
        : CSocket( ST_TCP_CLIENT ) ,
          _blocking( blocking )    ,
          _active( false )         ,
          m_iface( NULL )          ,
          m_maxreadbytes( 0 )      ,
          m_keepbytes( 0 )
{
        DEBUGOUTPUT( "CTCPClientSocket::Constructor()" );

        /*
         *      Constructor,
         *      init vars
         */
         
        _data = new TTCPClientSockData;             
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket::~CTCPClientSocket()
{
        DEBUGOUTPUT( "CTCPClientSocket::~CTCPClientSocket()" );

        /*
         *      Constructor,
         *      init vars
         */
        Close();
        delete _data;
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket&
CTCPClientSocket::operator=( const CTCPClientSocket& src )
{                    
        /* dummy, do not use */   
        return *this;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::IsActive( void ) const
{
        DEBUGOUTPUT( "CTCPClientSocket::IsActive( void ) const" );
        
        return _active;
}

/*-------------------------------------------------------------------------*/

bool 
CTCPClientSocket::IsBlocking( void ) const
{
        return _blocking;
}

/*-------------------------------------------------------------------------*/

/**
 *      Set's the maximum number of bytes to be read from the socket
 *      recieved data buffer. A value of 0 means infinite. Setting this
 *      value to non-zero allows you to avoid the server connection or
 *      even the entire server socket (if the server socket is not using
 *      a seperate thread) from getting stuck reading data. If data is
 *      being sent in such a fast rate that the continues stream will
 *      be considdered to be a single transmission the server will not
 *      be able to stop reading. This is where the max read value comes
 *      in. No matter how much new data is available the reading will
 *      stop at the set number of bytes. If you have a fixed transmission
 *      length then this is easy to deal with by using a factor of the
 *      transmission length as the max read value. Otherwise you will
 *      have to check what data you need and what data should be kept.
 *      The data you think that should be kept will be prefixed to the
 *      next data buffer. You can control this proccess by setting the
 *      keepbytes value in the OnRecieve() event handler.
 *      In short, this helps prevent a DOS attack on the software. 
 */
void
CTCPClientSocket::SetMaxRead( UInt32 mr )
{
        DEBUGOUTPUT( "CTCPClientSocket::SetMaxRead()" );
        
        datalock.Lock();        
        m_maxreadbytes = mr;
    
        datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

UInt32
CTCPClientSocket::GetMaxRead( void ) const
{
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
{
        DEBUGOUTPUT( "CTCPClientSocket::Reconnect( void )" );

        datalock.Lock();
        CORE::CString remoteaddr = _remoteaddr;
        UInt16 port = _remoteport;
        bool retval = ConnectTo( remoteaddr ,
                                 port       );
        datalock.Unlock();
        return retval;
}

/*-------------------------------------------------------------------------*/

/**
 *      attempt connection to server given
 */
bool
CTCPClientSocket::ConnectTo( const CORE::CString& remoteaddr , 
                             UInt16 port                     )
{
        DEBUGOUTPUT( "CTCPClientSocket::ConnectTo()" );

        if ( remoteaddr.Length() == 0 ) return false;  

        datalock.Lock();
        
        Close();        
        
        _active = true;
        
        if ( m_iface )
        {
                m_iface->OnConnecting( *this );
        }
        
        int errorcode;
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
                                            
        if ( !_data->hostent ) return false;

	_data->sockid = WSTS_socket( AF_INET     ,  /* Go over TCP/IP */
			             SOCK_STREAM ,  /* This is a stream-oriented socket */
			             IPPROTO_TCP ,  /* Use TCP rather than UDP */
                                     &errorcode  );
                                     
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
                return false;
        }
        
        _remoteaddr = remoteaddr;
        _remoteport = port;
        
        if ( m_iface )
        {
                m_iface->OnConnected( *this );
        }	                   

        datalock.Unlock();        
        return true;
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::CheckRecieveBuffer( void )
{        
        if ( !_blocking && _active )
        {       
                datalock.Lock();
                         
                /*
                 *      Since this is a non-blocking socket we need to poll for recieved data
                 */
                int bytesrecv;
                UInt32 totalrecieved( 0 );
                int errorcode;
                UInt32 readblocksize;
                m_maxreadbytes ? readblocksize = m_maxreadbytes : readblocksize = 1024;
                do
                {                 
                        m_readbuffer.SetDataSize( m_readbuffer.GetDataSize()+readblocksize+m_keepbytes );
                        bytesrecv = WSTS_recv( _data->sockid                                                             , 
                                               static_cast<char*>(m_readbuffer.GetBufferPtr())+totalrecieved+m_keepbytes ,
                                               readblocksize                                                             ,
                                               0                                                                         ,
                                               &errorcode                                                                );
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
                
                if ( totalrecieved && m_iface )
                {
                        UInt32 keepbytes(0);
                        m_iface->OnRead( *this                                                   , 
                                         static_cast<const char*>( m_readbuffer.GetBufferPtr() ) ,
                                         totalrecieved + m_keepbytes                             ,
                                         keepbytes                                               );
                        m_keepbytes = keepbytes;
                }
                else
                {
                        m_keepbytes = 0;
                }                        
                datalock.Unlock();      
        }                 
}

/*-------------------------------------------------------------------------*/

/** 
 *      polla the socket ect. as needed and update stats.
 *
 *      @param tickcount the tick count when the Update process commenced.
 *      @param deltaticks ticks since the last Update process commenced.          
 */
void 
CTCPClientSocket::Update( UInt32 tickcount  ,
                          UInt32 deltaticks )
{        
        CheckRecieveBuffer();
}                          

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::Close( void )
{
        DEBUGOUTPUT( "CTCPClientSocket::Close( void )" );

        /*
         *      close the socket connection
         */
        datalock.Lock();
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
                
                if ( m_iface )
                {       
                        m_iface->OnDisconnect( *this );
                }
        }
        datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::Send( const void* data ,
                        UInt32 length    ,
                        UInt32 timeout   )
{
        DEBUGOUTPUT( "CTCPClientSocket::Send( const char *data , UInt32 length )" );

        /*
         *      Write data to socket
         */
        if ( IsActive() )
        {
                datalock.Lock();
                if ( m_iface ) m_iface->OnWrite( *this, data, length );
                                        
                int error;
                Int32 wbytes = WSTS_send( _data->sockid ,  
                                          data          , 
                                          length        , 
                                          0             , 
                                          &error        );
                
                datalock.Unlock();
                return wbytes == length;
        } 
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::Send( const CORE::CString& data )
{
        DEBUGOUTPUT( "CTCPClientSocket::Send( CORE::CString& data )" );
        
        return Send( data.C_String() ,
                     data.Length()   ,
                     0               );
}

/*-------------------------------------------------------------------------*/

void 
CTCPClientSocket::SetInterface( CTCPClientSocketInterface* iface )
{
        DEBUGOUTPUT( "CTCPClientSocket::SetInterface( CTCPClientSocketInterface* iface )" );
        
        m_iface = iface;
}

/*-------------------------------------------------------------------------*/
        
CTCPClientSocketInterface* 
CTCPClientSocket::GetInterface( void ) const
{
        DEBUGOUTPUT( "CTCPClientSocket::GetInterface( void ) const" );
        
        return m_iface;
}


/*-------------------------------------------------------------------------*/

#ifdef NETWORK_ICMP_SUPPORT
/**
 *	This will allow you to ping a server.
 *	active sets wheter or not we should ping.
 *	pings is the number of ping to be performed. If this is smaller
 *	then 0 a single ping will be performed with the caller thread
 *	returning when the ping has been completed. In all other cases
 *      a pinging task will be spawned after which this member function
 *	will return. A value of 0 for ping equals infinite.
 *	maxhops is the maximum number of hops to try before we abort the
 *	ping.
 */
void
CTCPClientSocket::Ping( bool active, Int32 pings, UInt32 maxhops )
{
        if ( !datalock.Lock_Mutex()) return;

        if ( pings < 0 )
        {
                /*
                 *      Do the ping directly
                 */
                UInt32 hops, rtt;
                if ( ts_SDLNet_ICMP_fast_routeping( ip.host, maxhops, &hops, &rtt ) )
                {
                        if ( iface ) iface->OnPing( *this, hops, (Int32)rtt );
                }
                else
                {
                        if ( iface ) iface->OnPing( *this, hops, -1 );
                }
        }
        datalock.Unlock_Mutex();
}

/*-------------------------------------------------------------------------*/

#endif /* NETWORK_ICMP_SUPPORT */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
