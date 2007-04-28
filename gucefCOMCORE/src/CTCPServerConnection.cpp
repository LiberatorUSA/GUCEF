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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CTCPServerConnection.h"       /* header for this class */

#include "CTCPServerSocket.h"           /* parent server socket */
#include "CTCPServerSocketInterface.h"  /* interface of parent server socket */
#include "tsprinting.h"			/* threadsafe printing */

#ifdef GUCEF_MSWIN_BUILD

#ifndef DVWINSOCK_H
#include "dvwinsock.h"
#define DVWINSOCK_H
#endif /* DVWINSOCK_H ? */

#include <winsock.h>                   /* windows networking API */

#endif /* GUCEF_MSWIN_BUILD ? */

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
};
typedef struct STCPServerConData TTCPServerConData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Constructor,
 *      init vars
 */
CTCPServerConnection::CTCPServerConnection( CTCPServerSocket *tcp_serversock ,
                                            UInt32 connection_idx            )
        : CTCPConnection()                    ,
          _active( false )                    ,
          m_keepbytes( 0 )                    ,
          m_maxreadbytes( 0 )                 ,
          m_parentsock( tcp_serversock )      ,
          m_connectionidx( connection_idx )
          
{GUCEF_TRACE;
        
        _data = new TTCPServerConData;        
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

CTCPServerConnection::TIPAddress
CTCPServerConnection::GetRemoteIP( void ) const
{GUCEF_TRACE;

    TIPAddress ip = { _data->clientaddr.sin_port, _data->clientaddr.sin_addr.S_un.S_addr };
    return ip;
}

/*-------------------------------------------------------------------------*/

UInt16
CTCPServerConnection::GetRemoteTCPPort( void ) const
{GUCEF_TRACE;

    return ntohs( _data->clientaddr.sin_port );
}

/*-------------------------------------------------------------------------*/

void
CTCPServerConnection::Close( void )
{GUCEF_TRACE;

        _datalock.Lock();
        
        closesocket( _data->sockid ); 
        _active = false;
        m_parentsock->OnClientConnectionClosed( this            ,
                                                m_connectionidx ,
                                                false           );
                                                                        
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CTCPServerConnection::Send( const void* dataSource , 
                            const UInt16 length    )
{GUCEF_TRACE;
        
        _datalock.Lock();
        int error;
        Int32 wbytes = WSTS_send( _data->sockid ,  
                                  dataSource    , 
                                  length        , 
                                  0             , 
                                  &error        );
                   
        _datalock.Unlock();                   

     /*   if ( Active() )
        {
        	if ( parent->IFace() ) parent->IFace()->OnClientWrite( *parent, index, data, length );
        }      */


        return wbytes >= 0;
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
        _datalock.Lock();
        m_maxreadbytes = mr;
        _datalock.Unlock();          
}

/*-------------------------------------------------------------------------*/

UInt32
CTCPServerConnection::GetMaxRead( void ) const
{GUCEF_TRACE;

        return m_maxreadbytes;
}

/*-------------------------------------------------------------------------*/

/**
 *	Read data from socket. Using this member function basicly means
 *	that you will wait for data to arrive which will then be written
 *	into the dest buffer provided up to size. Note that the primary
 *	application thread does the initial network messaging so don't
 *	call this function from the main application thread !!!.
 *	timeout is the time in millisecs to wait maximum. if timeout < 0
 *	then no timeout will be used. False is returned if a timeout
 *	occured. True if the wait was successful. It is best to call
 *	this member function from a consumer thread. wbytes is given the
 *	nuber of bytes that where actually written uppon a succesful
 *	read. This member function waits untill data that was recieved
 *	on the socket is processed by the OnRead() event handler before
 *	returning if there is one. Note that if the destination buffer
 *	is not large enough to hold the data then the remaining data
 *	will be lost.
 */
bool
CTCPServerConnection::Read( char *dest     , 
                            UInt32 size    , 
                            UInt32 &wbytes , 
                            Int32 timeout  )
{GUCEF_TRACE;

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
                                 
        /*
         *      Since this is a non-blocking socket we need to poll for recieved data
         */
        int bytesrecv;
        UInt16 totalrecieved( 0 );
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
                                       
                if ( (  0 == totalrecieved ) &&
                     (  0 == bytesrecv     )  )
                {
                        /*
                         *      we arrived here because the read flag was set, however no data is available
                         *      This means that the client closed the connection
                         */
                        closesocket( _data->sockid ); 
                        _active = false;                         
                        m_parentsock->OnClientConnectionClosed( this            ,
                                                                m_connectionidx ,
                                                                true            );                         
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
        
        if ( totalrecieved )
        {
                UInt16 keepbytes(0);
                m_parentsock->OnClientRead( this                                                    ,
                                            m_connectionidx                                         , 
                                            static_cast<const char*>( m_readbuffer.GetBufferPtr() ) ,
                                            totalrecieved + m_keepbytes                             ,
                                            keepbytes                                               );
                m_keepbytes = keepbytes;
        }
        else
        {
                m_keepbytes = 0;
        }                        
             
}

/*-------------------------------------------------------------------------*/

/**                 
 *      polls the socket ect. as needed and update stats.
 *
 *      @param tickcount the tick count when the Update process commenced.
 *      @param deltaticks ticks since the last Update process commenced.          
 */
void 
CTCPServerConnection::Update( void )
{GUCEF_TRACE;

        if ( !_blocking && _active )
        {       
                fd_set readfds;      /* Setup the read variable for the select function */        
                //fd_set writefds;     /* Setup the write variable for the select function */
                fd_set exceptfds;    /* Setup the except variable for the select function */

                FD_ZERO( &readfds );
                //FD_ZERO( &writefds );
                FD_ZERO( &exceptfds );
        
                _datalock.Lock();
                
                FD_SET( _data->sockid, &readfds );
                //FD_SET( _data->sockid, &writefds );
                FD_SET( _data->sockid, &exceptfds );                
                
                int errorcode;
                if ( WSTS_select( 0          , 
                                  &readfds   , 
                                  NULL       ,  //&writefds  , 
                                  &exceptfds , 
                                  0          ,
                                  &errorcode ) > 0 ) 
                {
                        /* something happened on the socket */
                        
                        if ( FD_ISSET( _data->sockid, &exceptfds ) )
                        {
                               // m_parentsock->OnClientSocketError( this              ,
                                 //                                  m_connectionidx   , 
                                 //                                  );
                                Close();
                                _datalock.Unlock();
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
                        //@TODO: handle
                }
                _datalock.Unlock(); 
        }               
}

/*-------------------------------------------------------------------------*/

bool 
CTCPServerConnection::IsActive( void ) const
{GUCEF_TRACE;

        return _active;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
