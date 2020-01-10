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

#include <string.h>

#include "CTCPServerSocket.h"           /* parent server socket */

#ifndef GUCEF_COMCORE_SOCKETUTILS_H
#include "socketutils.h"
#define GUCEF_COMCORE_SOCKETUTILS_H
#endif /* GUCEF_COMCORE_SOCKETUTILS_H ? */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

#include "CTCPServerConnection.h"       /* header for this class */

#if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    #include <netinet/tcp.h>
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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define BUFFER_READ_SIZE	25

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct STCPServerConData
{
    UInt32 sockid;
    CORE::CString clientip;
    struct sockaddr_in clientaddr;
    struct timeval timeout;         /* timeout for blocking operations */
};
typedef struct STCPServerConData TTCPServerConData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTCPServerConnection::CTCPServerConnection( CTCPServerSocket *tcp_serversock ,
                                            UInt32 connection_idx            )
        : CTCPConnection()                    ,
          _data( NULL )                       ,
          _blocking( false )                  ,
          _active( false )                    ,
          m_readbuffer()                      ,
          m_sendBuffer()                      ,
          m_sendOpBuffer()                    ,
          _datalock()                         ,
          m_maxreadbytes( 0 )                 ,
          m_connectionidx( connection_idx )   ,
          m_parentsock( tcp_serversock )      ,
          m_coaleseDataSends( true )
{GUCEF_TRACE;

    _data = new TTCPServerConData;
    _data->sockid = 0;
    memset( &_data->timeout, 0, sizeof( _data->timeout ) );
    memset( &_data->clientaddr, 0, sizeof(_data->clientaddr) );
}

/*-------------------------------------------------------------------------*/

CTCPServerConnection::~CTCPServerConnection()
{GUCEF_TRACE;

    Close();

    delete _data;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CTCPServerConnection::GetRemoteHostName( void ) const
{GUCEF_TRACE;

    return _data->clientip;
}

/*-------------------------------------------------------------------------*/

CIPAddress
CTCPServerConnection::GetRemoteIP( void ) const
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return CIPAddress( _data->clientaddr.sin_addr.S_un.S_addr, _data->clientaddr.sin_port );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return CIPAddress( _data->clientaddr.sin_addr.s_addr, _data->clientaddr.sin_port );

    #endif
}

/*-------------------------------------------------------------------------*/

CHostAddress
CTCPServerConnection::GetRemoteHostAddress( void ) const
{GUCEF_TRACE;

    return CHostAddress( GetRemoteIP(), GetRemoteHostName() );
}

/*-------------------------------------------------------------------------*/

UInt16
CTCPServerConnection::GetRemoteTCPPort( void ) const
{GUCEF_TRACE;

    return ntohs( _data->clientaddr.sin_port );
}

/*-------------------------------------------------------------------------*/

void
CTCPServerConnection::LockData( void ) const
{GUCEF_TRACE;

    _datalock.Lock();
}

/*-------------------------------------------------------------------------*/

void
CTCPServerConnection::UnlockData( void ) const
{GUCEF_TRACE;

    _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CTCPServerConnection::Close( void )
{GUCEF_TRACE;

    LockData();

    if ( _active )
    {
        int errorCode;
        dvsocket_closesocket( _data->sockid, &errorCode );
        _active = false;

        NotifyObservers( DisconnectedEvent );

        m_parentsock->OnClientConnectionClosed( this            ,
                                                m_connectionidx ,
                                                false           );

		m_readbuffer.Clear( false );
		m_sendBuffer.Clear( false );
		m_sendOpBuffer.Clear( false );
    }
    UnlockData();
}

/*-------------------------------------------------------------------------*/

bool
CTCPServerConnection::Send( const void* dataSource ,
                            const UInt32 length    )
{GUCEF_TRACE;

    if ( IsActive() )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Sending data of length " + CORE::UInt32ToString( length ) );

        LockData();

        // notify observers that we are sending data
        CORE::CDynamicBuffer linkedBuffer;
        linkedBuffer.LinkTo( dataSource, length );
        TDataRecievedEventData cData( &linkedBuffer );
        if ( !NotifyObservers( DataSentEvent, &cData ) ) return false;

        // Check if we still have data queued to be sent,..
        // TCP has to be in-order so we will have to queue the new data behind the already queued data
        if ( m_sendBuffer.HasBufferedData() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Delaying sending of data because there is still data queued to be send from the previous send" );
            m_sendBuffer.Write( dataSource, 1, length );
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
            wbytes = dvsocket_send( _data->sockid                      ,
                                    ((Int8*)dataSource)+totalBytesSent ,
                                    remnant                            ,
                                    0                                  ,
                                    &error                             );
            if ( wbytes != SOCKET_ERROR )
            {
                // we where able to send at least some of the data
                totalBytesSent += wbytes;

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Succeeded in sending " + CORE::UInt32ToString( wbytes ) + " bytes of data" );
            }
            else
            {
                // Socket error,..
                // Check if we have to delay sending the data
                if ( DVSOCKET_EWOULDBLOCK == error && !_blocking )
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPClientSocket(" + CORE::PointerToString( this ) + "): Delaying sending of data" );
                    m_sendBuffer.Write( ((Int8*)dataSource)+totalBytesSent, 1, remnant );
                }
                else
                {
                    UnlockData();

                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( error ) );

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
CTCPServerConnection::SetMaxRead( UInt32 mr )
{GUCEF_TRACE;

    /*
     *      Although the setting of the int value may be atomic we must
     *      use a mutex lock so that a current read proccess will not be
     *      affected. Thus the setting will take effect on the next read
     *      cycle.
     */
    LockData();
    m_maxreadbytes = mr;
    UnlockData();
}

/*-------------------------------------------------------------------------*/

UInt32
CTCPServerConnection::GetMaxRead( void ) const
{GUCEF_TRACE;

    return m_maxreadbytes;
}

/*-------------------------------------------------------------------------*/

bool
CTCPServerConnection::Read( char *dest     ,
                            UInt32 size    ,
                            UInt32 &wbytes ,
                            Int32 timeout  )
{GUCEF_TRACE;
            //@TODO: blocking socket support
     //   _datalock.Lock();
     //   recv( _data->sockid              ,
     //         _readbuffer.GetBufferPtr() ,
     //         _readbuffer.GetSize()      ,
     //         0                          );


     /*
        if ( !datalock.Lock_Mutex() ) return false;
	rb_dest = dest;
        rb_size = size;
        rb_wbytes = &wbytes;
	read_flag = true;
        datalock.Unlock_Mutex();

        return Wait_Untill_Read( timeout );   */
        return false;
}

/*-------------------------------------------------------------------------*/

void
CTCPServerConnection::CheckRecieveBuffer( void )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Checking the recieve buffer for incoming data" );

    /*
     *      Since this is a non-blocking socket we need to poll for received data
     */
    int bytesrecv;
    UInt16 totalrecieved( 0 );
    int errorcode;
    UInt32 readblocksize;
    m_maxreadbytes ? readblocksize = m_maxreadbytes : readblocksize = 1024;
    do
    {
        m_readbuffer.SetDataSize( m_readbuffer.GetDataSize()+readblocksize );
        bytesrecv = dvsocket_recv( _data->sockid                                                 ,
                                   static_cast<char*>(m_readbuffer.GetBufferPtr())+totalrecieved ,
                                   readblocksize                                                 ,
                                   0                                                             ,
                                   &errorcode                                                    );

        // Check for an error
        if ( ( bytesrecv == SOCKET_ERROR ) || ( errorcode != 0 ) )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( errorcode ) );

            // After a socket error you must always close the connection.
            dvsocket_closesocket( _data->sockid, &errorcode );
            _active = false;

            // Notify our users of the error
            TSocketErrorEventData eData( errorcode );
            NotifyObservers( SocketErrorEvent, &eData );
            return;
        }

        if ( (  0 == totalrecieved ) &&
             (  0 == bytesrecv     )  )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Client closed the connection" );

            /*
             *      we arrived here because the read flag was set, however no data is available
             *      This means that the client closed the connection
             */
            dvsocket_closesocket( _data->sockid, &errorcode );
            _active = false;

            NotifyObservers( DisconnectedEvent );

            m_parentsock->OnClientConnectionClosed( this            ,
                                                    m_connectionidx ,
                                                    true            );
            return;
        }
        totalrecieved += bytesrecv;
        m_readbuffer.SetDataSize( totalrecieved );

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
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Received " + CORE::UInt32ToString( totalrecieved ) + " bytes of data" );

        UInt16 keepbytes(0);

        TDataRecievedEventData eData( &m_readbuffer );
        NotifyObservers( DataRecievedEvent, &eData );
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CTCPServerConnection::GetConnectionIndex( void ) const
{GUCEF_TRACE;

    return m_connectionidx;
}

/*-------------------------------------------------------------------------*/

void
CTCPServerConnection::Update( UInt32 maxUpdatesPerCycle )
{GUCEF_TRACE;

    if ( !_blocking && _active )
    {
        fd_set readfds;      /* Setup the read variable for the select function */
        fd_set exceptfds;    /* Setup the except variable for the select function */

        FD_ZERO( &readfds );
        FD_ZERO( &exceptfds );

        LockData();

        FD_SET( _data->sockid, &readfds );
        FD_SET( _data->sockid, &exceptfds );

        int errorCode = 0;
        if ( dvsocket_select( _data->sockid+1   ,
                              &readfds          ,
                              NULL              , // We don't care about socket writes here
                              &exceptfds        ,
                              &_data->timeout   ,
                              &errorCode        ) != SOCKET_ERROR )
        {
            /* something happened on the socket */

            if ( FD_ISSET( _data->sockid, &exceptfds ) )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( errorCode ) );

                int lastError = errorCode;
                dvsocket_closesocket( _data->sockid, &errorCode );
                _active = false;

                TSocketErrorEventData eData( lastError );
                if ( !NotifyObservers( SocketErrorEvent, &eData ) ) return;

                UnlockData();
                return;
            }
            else
            if ( FD_ISSET( _data->sockid, &readfds ) )
            {
                /* data can be read from the socket */
                CheckRecieveBuffer();
            }
        }
        else
        {
            /* select call failed */
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Socket error occured (select call failed): " + CORE::Int32ToString( errorCode ) );

            TSocketErrorEventData eData( errorCode );
            NotifyObservers( SocketErrorEvent, &eData );
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
                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Succeeded in delayed sending " + CORE::Int32ToString( wbytes ) + "bytes of queued data" );
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

                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Unable to delayed send queued data at this time" );

                            UnlockData();
                            return;
                        }
                        else
                        {
                            UnlockData();

                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CTCPServerConnection(" + CORE::PointerToString( this ) + "): Socket error occured: " + CORE::Int32ToString( error ) );

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

bool
CTCPServerConnection::IsActive( void ) const
{GUCEF_TRACE;

    return _active;
}

/*-------------------------------------------------------------------------*/

bool
CTCPServerConnection::SetUseTcpSendCoalescing( bool coaleseData )
{
    if ( _active )
    {
        int flag = (coaleseData ? 1 : 0);
        if ( -1 == setsockopt( _data->sockid, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) ) )
        {
            return false;
        }
    }

    m_coaleseDataSends = coaleseData;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTCPServerConnection::GetUseTcpSendCoalescing( void ) const
{
    return m_coaleseDataSends;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
