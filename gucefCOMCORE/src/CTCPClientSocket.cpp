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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_COMCORE_DVSOCKET_H
#include "dvwinsock.h"
#define GUCEF_COMCORE_DVSOCKET_H
#endif /* GUCEF_COMCORE_DVSOCKET_H ? */

#ifndef GUCEF_COMCORE_SOCKETUTILS_H
#include "socketutils.h"
#define GUCEF_COMCORE_SOCKETUTILS_H
#endif /* GUCEF_COMCORE_SOCKETUTILS_H ? */

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

#define MAX_PULSE_INTERVAL_IN_MS    10 
#define BUFFER_READ_SIZE            16

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
    LPHOSTENT hostent;
    SOCKET sockid;
    SOCKADDR_IN serverinfo;
    struct timeval timeout;         /* timeout for blocking operations */ 
};
typedef struct CTCPClientSocket::STCPClientSockData TTCPClientSockData;

//-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------//

CTCPClientSocket::CTCPClientSocket( CORE::CPulseGenerator& pulseGenerator ,
                                    bool blocking                         ) 
        : CTCPConnection()                    ,
          _blocking( blocking )               ,
          _active( false )                    ,
          datalock()                          ,
          m_readbuffer()                      ,
          m_sendBuffer()                      ,
          m_sendOpBuffer()                    ,
          m_maxreadbytes( 0 )                 ,
          m_hostAddress()                     ,
          m_isConnecting( false )             ,
          m_pulseGenerator( &pulseGenerator )
{GUCEF_TRACE;

    RegisterEvents();
    
    _data = new TTCPClientSockData;             
    assert( _data != NULL );
    memset( &_data->timeout, 0, sizeof( struct timeval ) ); 
    
    SubscribeTo( m_pulseGenerator                                    , 
                 CORE::CPulseGenerator::PulseEvent                   ,
                 &TEventCallback( this, &CTCPClientSocket::OnPulse ) );
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket::CTCPClientSocket( bool blocking ) 
        : CTCPConnection()         ,
          _blocking( blocking )    ,
          _active( false )         ,
          datalock()               ,
          m_readbuffer()           ,
          m_sendBuffer()           ,
          m_sendOpBuffer()         ,
          m_maxreadbytes( 0 )      ,
          m_hostAddress()          ,
          m_isConnecting( false )  ,
          m_pulseGenerator( &CORE::CGUCEFApplication::Instance()->GetPulseGenerator() )
{GUCEF_TRACE;

    RegisterEvents();
    
    _data = new TTCPClientSockData;             
    assert( _data != NULL );
    memset( &_data->timeout, 0, sizeof( struct timeval ) ); 
    
    SubscribeTo( m_pulseGenerator                                    , 
                 CORE::CPulseGenerator::PulseEvent                   ,
                 &TEventCallback( this, &CTCPClientSocket::OnPulse ) );
}

/*-------------------------------------------------------------------------*/

CTCPClientSocket::~CTCPClientSocket()
{GUCEF_TRACE;

    Close();
    delete _data;
    _data = NULL;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::IsActive( void ) const
{GUCEF_TRACE;
        
        return _active;
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::RegisterEvents( void )
{GUCEF_TRACE;
    
    ConnectingEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

bool 
CTCPClientSocket::IsBlocking( void ) const
{GUCEF_TRACE;
    
    return _blocking;
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::SetMaxRead( UInt32 mr )
{GUCEF_TRACE;
        
    LockData();        
    m_maxreadbytes = mr;
    UnlockData();
}

/*-------------------------------------------------------------------------*/

UInt32
CTCPClientSocket::GetMaxRead( void ) const
{GUCEF_TRACE;
        return m_maxreadbytes;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::Reconnect( void )
{GUCEF_TRACE;

        LockData();
        bool retval = ConnectTo( m_hostAddress.AddressAsString()        ,
                                 m_hostAddress.GetPortInHostByteOrder() );
        UnlockData();
        return retval;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::ConnectTo( const CORE::CString& remoteaddr , 
                             UInt16 port                     )
{GUCEF_TRACE;

    if ( remoteaddr.Length() == 0 ) return false;  

    LockData();
    
    Close();        
    
    _active = true;
    
    int errorcode = 0;
    if ( CORE::Check_If_IP( remoteaddr.C_String() ) == 1 )
    {
            UInt32 addr = inet_addr( remoteaddr.C_String() );
            _data->hostent = dvsocket_gethostbyaddr( (char*) &addr , 
                                                      4            , 
                                                      AF_INET      ,
                                                      &errorcode   );                                             
    }
    else
    {
            _data->hostent = dvsocket_gethostbyname( remoteaddr.C_String() ,
                                                     &errorcode            );
    }
    
    // Check for an error
    if ( errorcode != 0 )
    {
        // Notify our users of the error
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( errorcode ) );
        TSocketErrorEventData eData( errorcode );
        if ( !NotifyObservers( SocketErrorEvent, &eData ) ) return false;
        
        // After a socket error you must always close the connection.
        _active = false;
        
        UnlockData();
        return false;
    }    
                                        
    if ( NULL == _data->hostent ) return false;

    _data->sockid = dvsocket_socket( AF_INET     ,  /* Go over TCP/IP */
                                     SOCK_STREAM ,  /* This is a stream-oriented socket */
                                     IPPROTO_TCP ,  /* Use TCP rather than UDP */
                                     &errorcode  );

    // Check for an error
    if ( errorcode != 0 )
    {
        // Notify our users of the error
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( errorcode ) );
        TSocketErrorEventData eData( errorcode );
        if ( !NotifyObservers( SocketErrorEvent, &eData ) ) return false;
        
        // After a socket error you must always close the connection.
        _active = false;
        
        UnlockData();
        return false;
    }
                                 
    if ( _data->sockid == INVALID_SOCKET ) return false;
    
    /* Set the desired blocking mode */
    if ( !SetBlockingMode( _data->sockid ,
                           _blocking     ) )
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
    if ( dvsocket_connect( _data->sockid                  ,
                           (LPSOCKADDR)&_data->serverinfo ,
                           sizeof( struct sockaddr)       ,
                           &errorcode                     ) == SOCKET_ERROR )
    {
        // It is normal for DVSOCKET_EWOULDBLOCK to be reported as the result from calling 
        // connect on a nonblocking SOCK_STREAM socket, since some time must elapse 
        // for the connection to be established.
        if ( errorcode != DVSOCKET_EWOULDBLOCK )
        {
            _active = false;
            
            // Check for an error
            if ( errorcode != 0 )
            {
                // Notify our users of the error
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( errorcode ) );
                TSocketErrorEventData eData( errorcode );
                if ( !NotifyObservers( SocketErrorEvent, &eData ) ) return false;
            }        
                    
            // After a socket error you must always close the connection.
            _active = false;
            
            UnlockData();
            return false;
        }
    }
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Connecting to " + remoteaddr + ":" + CORE::UInt16ToString( port ) );
    
    m_readbuffer.Clear();
    m_pulseGenerator->RequestPeriodicPulses( this, MAX_PULSE_INTERVAL_IN_MS );
    
    m_isConnecting = true;        
    m_hostAddress.SetHostname( remoteaddr );
    m_hostAddress.SetPortInHostByteOrder( port );	                   

    UnlockData();        
    
    NotifyObservers( ConnectingEvent );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::ConnectTo( const CHostAddress& address )
{GUCEF_TRACE;

    return ConnectTo( address.AddressAsString()        ,
                      address.GetPortInHostByteOrder() );
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CTCPClientSocket::GetRemoteHostName( void ) const
{GUCEF_TRACE;

    return m_hostAddress.GetHostname();
}

/*-------------------------------------------------------------------------*/
    
UInt16
CTCPClientSocket::GetRemoteTCPPort( void ) const
{GUCEF_TRACE;
    
    return ntohs( _data->serverinfo.sin_port );
}

/*-------------------------------------------------------------------------*/
    
CIPAddress
CTCPClientSocket::GetRemoteIP( void ) const
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )    
    return CIPAddress( _data->serverinfo.sin_addr.S_un.S_addr, _data->serverinfo.sin_port );
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    return CIPAddress( _data->serverinfo.sin_addr.s_addr, _data->serverinfo.sin_port );
    #endif
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::CheckRecieveBuffer( void )
{GUCEF_TRACE;        

    if ( !_blocking && _active )
    {       
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Checking the recieve buffer for incoming data" );
        
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
            m_readbuffer.SetBufferSize( m_readbuffer.GetDataSize()+readblocksize, false );
            
            // read an additional block
            bytesrecv = dvsocket_recv( _data->sockid                                                                , 
                                       static_cast<char*>(m_readbuffer.GetBufferPtr()) + m_readbuffer.GetDataSize() ,
                                       readblocksize                                                                ,
                                       0                                                                            ,
                                       &errorcode                                                                   );
                                  
            // Check for an error
            if ( ( bytesrecv == SOCKET_ERROR ) || ( errorcode != 0 ) )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( errorcode ) );
                
                // Notify our users of the error
                TSocketErrorEventData eData( errorcode );
                if ( !NotifyObservers( SocketErrorEvent, &eData ) ) return;
                
                // After a socket error you must always close the connection.
                Close();
                
                UnlockData();
                return;
            }
            else 
            if ( bytesrecv == 0 )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): The server has closed the connection" );
                
                // The connection has been closed on us
                Close();

                UnlockData();
                return;
            }
            
            // Increase the logical data size delimiter by the amount we just copied into the buffer
            m_readbuffer.SetDataSize( m_readbuffer.GetDataSize() + bytesrecv );
            
            // Is there a limit on the number of bytes we want to read ?            
            if ( m_maxreadbytes > 0 )
            {
                // Check to make sure we do not exceed the maximum
                if ( m_maxreadbytes <= m_readbuffer.GetDataSize() )
                {
                    break;
                }
            }
        }        
        while ( bytesrecv == readblocksize );
        
        if ( m_readbuffer.GetDataSize() > 0 )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Received " + CORE::UInt32ToString( m_readbuffer.GetDataSize() ) + " bytes" );
            
            CORE::CDynamicBuffer linkedBuffer;
            linkedBuffer.LinkTo( m_readbuffer.GetConstBufferPtr(), m_readbuffer.GetDataSize() );
            TDataRecievedEventData cData( &linkedBuffer );
            if ( !NotifyObservers( DataRecievedEvent, &cData ) ) return;
        }
                      
        UnlockData();      
    }                 
}

/*-------------------------------------------------------------------------*/

void 
CTCPClientSocket::OnPulse( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventid               ,
                           CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;        
    
    if ( !_blocking && _active )
    {       
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_EVERYTHING, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Pulse received" );
        
        fd_set writefds;     /* Setup the write variable for the select function */
        fd_set readfds;      /* Setup the read variable for the select function */        
        fd_set exceptfds;    /* Setup the except variable for the select function */

        FD_ZERO( &writefds );
        FD_ZERO( &readfds );
        FD_ZERO( &exceptfds );

        LockData();
        
        FD_SET( _data->sockid, &writefds );
        FD_SET( _data->sockid, &readfds );
        FD_SET( _data->sockid, &exceptfds );                
        
        int errorcode = select( (int)_data->sockid+1 , 
                                 &readfds            , 
                                 &writefds           ,
                                 &exceptfds          , 
                                 &_data->timeout     );
                                 
        if ( errorcode != SOCKET_ERROR ) 
        {
            /* something happened on the socket */
            
            if ( FD_ISSET( _data->sockid, &exceptfds ) )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( errorcode ) );
                
                TSocketErrorEventData eData( errorcode );
                if ( !NotifyObservers( SocketErrorEvent, &eData ) ) return;             
                Close();
                UnlockData();
                return;
            }
            else
            if ( FD_ISSET( _data->sockid, &readfds ) )
            {
                /* data can be read from the socket */
                CheckRecieveBuffer();
            }
            else
            if ( m_isConnecting )
            {
                // Check if the socket is now ready for writing
                if ( FD_ISSET( _data->sockid, &writefds ) )
                {   
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket is now connected to the server, TCP channel established" );
                    
                    // We are now connected
                    m_isConnecting = false;
                    if ( !NotifyObservers( ConnectedEvent ) ) return;
                }
            }                                                
        }
        else
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket error occured (select call failed): " + CORE::Int32ToString( errorcode ) );
            
            /* select call failed */
            TSocketErrorEventData eData( errorcode );
            if ( !NotifyObservers( SocketErrorEvent, &eData ) ) return;
            Close();
        }
        
        // Check if we still have data queued to be sent,.. 
        while ( m_sendBuffer.HasBufferedData() )
        {
            // read an entire block
            if ( 0 != m_sendBuffer.ReadBlockTo( m_sendOpBuffer ) )
            {
                // We will try looping until we have transmitted all the data
                int error;
                UInt32 totalBytesSent = 0;
                Int32 wbytes;
                while ( totalBytesSent < m_sendOpBuffer.GetDataSize() )
                {
                    // perform a send, trying to send as much of the given data as possible
                    const Int8* data = static_cast< const Int8* >( m_sendOpBuffer.GetConstBufferPtr() );
                    Int32 remnant = m_sendOpBuffer.GetDataSize() - totalBytesSent;
                    wbytes = dvsocket_send( _data->sockid       ,  
                                        data+totalBytesSent , 
                                        remnant             , 
                                        0                   , 
                                        &error              );
                    if ( wbytes != SOCKET_ERROR )
                    {
                        // we where able to send at least some of the data
                        totalBytesSent += wbytes;
                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Succeeded in delayed sending " + CORE::Int32ToString( wbytes ) + "bytes of queued data" );
                    }
                    else
                    {
                        // Socket error,..
                        // Check if we have to delay sending the data
                        if ( DVSOCKET_EWOULDBLOCK == error && !_blocking )
                        {
                            // Cannot send now,... try again next pulse
                            // We have to place remaining data we grabbed from the send buffer back in
                            // the send buffer in a FILO manner
                            m_sendBuffer.WriteAtFrontOfQueue( data+totalBytesSent ,
                                                              remnant             ,
                                                              1                   );
                            
                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Unable to delayed send queued data at this time" );
                            
                            UnlockData();
                            return;
                        }
                        else
                        {
                            UnlockData();
                            
                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( error ) );
                            
                            TSocketErrorEventData eData( error );
                            NotifyObservers( SocketErrorEvent, &eData );
                            return;
                        }
                    }
                }
            }
        }        
        
        UnlockData(); 
    }
}                          

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::Close( void )
{GUCEF_TRACE;
        
    /*
     *      close the socket connection
     */
    LockData();
    if ( IsActive() )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Closing active socket" );
        
        /* prevent any further sends on the socket */
        shutdown( _data->sockid , 
                  1             );
                                  
        /* close the socket, this de-allocates the winsock buffers for the socket */          
        int errorCode;
        dvsocket_closesocket( _data->sockid, &errorCode );
        _active = false;
        m_isConnecting = false;
        
        m_pulseGenerator->RequestStopOfPeriodicUpdates( this );
        
        // Wipe the buffers
        m_sendBuffer.Clear( false );
        m_sendOpBuffer.Clear( false );
        m_readbuffer.Clear( false );
        
        if ( !NotifyObservers( DisconnectedEvent ) ) return;
    }
    UnlockData();
}

/*-------------------------------------------------------------------------*/

bool
CTCPClientSocket::Send( const void* dataSource , 
                        const UInt32 dataSize  )
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
{GUCEF_TRACE;

    if ( IsActive() )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Sending data of length " + CORE::UInt32ToString( length ) + " with timeout " + CORE::UInt32ToString( timeout ) );
        
        LockData();
        
        // notify observers that we are sending data
        CORE::CDynamicBuffer linkedBuffer;
        linkedBuffer.LinkTo( data, length );
        TDataRecievedEventData cData( &linkedBuffer );
        if ( !NotifyObservers( DataSentEvent, &cData ) ) return false;
        
        // Check if we still have data queued to be sent,.. 
        // TCP has to be in-order so we will have to queue the new data behind the already queued data
        if ( m_sendBuffer.HasBufferedData() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Delaying sending of data because there is still data queued to be send from the previous send" );
            m_sendBuffer.Write( data, 1, length );
            UnlockData();
            return true;
        }
                                
        // We will try looping until we have transmitted all the data
        int error;
        UInt32 totalBytesSent = 0;
        Int32 wbytes;
        while ( totalBytesSent < length )
        {
            // perform a send, trying to send as much of the given data as possible
            Int32 remnant = length - totalBytesSent;
            wbytes = dvsocket_send( _data->sockid                 ,  
                                ((Int8*)data)+totalBytesSent  , 
                                remnant                       , 
                                0                             , 
                                &error                        );
            if ( wbytes != SOCKET_ERROR )
            {
                // we where able to send at least some of the data
                totalBytesSent += wbytes;
                
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Succeeded in sending " + CORE::UInt32ToString( wbytes ) + " bytes of data" );
            }
            else
            {
                // Socket error,..
                // Check if we have to delay sending the data
                if ( DVSOCKET_EWOULDBLOCK == error && !_blocking )
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Delaying sending of data" );
                    m_sendBuffer.Write( ((Int8*)data)+totalBytesSent, 1, remnant );
                }
                else
                {
                    UnlockData();
                    
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( error ) );
                    
                    TSocketErrorEventData eData( error );
                    NotifyObservers( SocketErrorEvent, &eData );
                    return false;
                }
            }
        }
        
        UnlockData();
        return true;
    }
     
    return false;
}

/*-------------------------------------------------------------------------*/

void
CTCPClientSocket::LockData( void ) const
{GUCEF_TRACE;

    datalock.Lock();
}

/*-------------------------------------------------------------------------*/
    
void
CTCPClientSocket::UnlockData( void ) const
{GUCEF_TRACE;
    
    datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CTCPClientSocket::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString typeName = "GUCEF::COMCORE::CTCPClientSocket";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
