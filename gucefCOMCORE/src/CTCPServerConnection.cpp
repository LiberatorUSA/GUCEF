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

#ifndef DVWINSOCK_H
#include "dvwinsock.h"
#define DVWINSOCK_H
#endif /* DVWINSOCK_H ? */

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
        : CSocket()                           ,
          _active( false )                    ,
          m_keepbytes( 0 )                    ,
          m_maxreadbytes( 0 )                 ,
          m_parentsock( tcp_serversock )      ,
          m_connectionidx( connection_idx )
          
{TRACE;
        
        _data = new TTCPServerConData;        
}

/*-------------------------------------------------------------------------*/

/**
 *	Decontructor, close connection and cleanup.
 */
CTCPServerConnection::~CTCPServerConnection()
{
        Close();      
        
        delete _data;
}

/*-------------------------------------------------------------------------*/

/**
 *      Closes the connection with the client
 */
void
CTCPServerConnection::Close( void )
{
        _datalock.Lock();
        
        closesocket( _data->sockid ); 
        _active = false;
        m_parentsock->OnClientConnectionClosed( this            ,
                                                m_connectionidx ,
                                                false           );
                                                                        
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

/**
 *      Write data to socket
 */
bool
CTCPServerConnection::Send( const char *data , 
                            UInt32 length    )
{TRACE;
        
        _datalock.Lock();
        int error;
        Int32 wbytes = WSTS_send( _data->sockid ,  
                                  data          , 
                                  length        , 
                                  0             , 
                                  &error        );
                   
        _datalock.Unlock();                   

     /*   if ( Active() )
        {
        	if ( parent->IFace() ) parent->IFace()->OnClientWrite( *parent, index, data, length );
        }      */


        return false;//( success );
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
 *      keepbytes value in the OnClientRead() event handler.
 *      In short, this helps prevent a DOS attack on the software. 
 */
void
CTCPServerConnection::SetMaxRead( UInt32 mr )
{
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
{
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
{TRACE;

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
{                                 
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
{
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
{
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
