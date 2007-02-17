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

#include <string.h>                     /* standard string utils */
#include <malloc.h>                     /* Memory management */

#include "dvstrutils.h"                 /* my own string utils */
#include "tsprinting.h"			/* threadsafe printing */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"              /* we want the log manager for its defines */
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifdef GUCEF_MSWIN_BUILD

#ifndef DVWINSOCK_H
#include "dvwinsock.h"
#define DVWINSOCK_H
#endif /* DVWINSOCK_H ? */

#include <winsock2.h>                   /* windows networking API */

#endif /* GUCEF_MSWIN_BUILD ? */

#include "CTCPServerSocket.h"           /* this file's header */

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

/*
 *      This is a value used internally for reading into a buffer between
 *      events. If this value is set to small you will get inefficient code
 *      due to unnessesary memory allocation operations, however if you set it
 *      to large you will allocate memory that is never used.
 *      So what you need to do is find out what the average length is of your
 *      communication and adapt the value to that
 */
#define BUFFER_READ_SIZE        25

#define DEFAULT_MAX_CONNECTIONS 100UL 
#define HEAP_RESIZE_AMOUNT	5UL

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CTCPServerSocket::ClientDataRecievedEvent = "GUCEF::COMCORE::CTCPServerSocket::ClientDataRecievedEvent";
const CORE::CEvent CTCPServerSocket::ClientDataSentEvent = "GUCEF::COMCORE::CTCPServerSocket::ClientDataSentEvent";
const CORE::CEvent CTCPServerSocket::ClientConnectedEvent = "GUCEF::COMCORE::CTCPServerSocket::ClientConnectedEvent";
const CORE::CEvent CTCPServerSocket::ClientDisconnectedEvent = "GUCEF::COMCORE::CTCPServerSocket::ClientDisconnectedEvent";
const CORE::CEvent CTCPServerSocket::ClientErrorEvent = "GUCEF::COMCORE::CTCPServerSocket::ClientErrorEvent";
const CORE::CEvent CTCPServerSocket::ServerSocketOpenedEvent = "GUCEF::COMCORE::CTCPServerSocket::ServerSocketOpenedEvent";
const CORE::CEvent CTCPServerSocket::ServerSocketClosedEvent = "GUCEF::COMCORE::CTCPServerSocket::ServerSocketClosedEvent";
const CORE::CEvent CTCPServerSocket::ServerSocketErrorEvent = "GUCEF::COMCORE::CTCPServerSocket::ServerSocketErrorEvent";
const CORE::CEvent CTCPServerSocket::ServerSocketClientErrorEvent = "GUCEF::COMCORE::CTCPServerSocket::ServerSocketClientErrorEvent";
const CORE::CEvent CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent = "GUCEF::COMCORE::CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct STCPServerConData
{
        SOCKET sockid;
        CORE::CString clientip;
        struct sockaddr_in clientaddr;
};
typedef struct STCPServerConData TTCPServerConData;

/*-------------------------------------------------------------------------*/

struct STCPServerSockData
{
        #ifdef GUCEF_MSWIN_BUILD
        SOCKET sockid;           /* Winsock socket number */
        UInt32 connectcount;     /* number of clients connected to the server socket */
        UInt32 maxcon;           /* maximum number of connections for this server socket */
        bool blocking;           /* is this a blocking or non-blocking server ? */
        SOCKADDR_IN serverinfo;  /* winsock info on the listning socket */
        int threadmethod;        /* threading method used by this socket */
        struct timeval timeout;  /* timeout for blocking operations */
        #endif
                
};
typedef struct STCPServerSockData TTCPServerSockData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTCPServerSocket::CTCPServerSocket( void )
        : CSocket() ,
          m_port(0)
{TRACE;
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CTCPServerSocket::CTCPServerSocket( bool blocking ) 
        : CSocket()                               ,
          _connections( DEFAULT_MAX_CONNECTIONS ) ,
          _active( false )                        ,
          _blocking( blocking )                   ,
          m_port( 0 )
{TRACE;
        
        _data = new TTCPServerSockData;
        _data->blocking = blocking;
        _data->sockid = 0;
        _data->connectcount = 0;
//        _data->threadmethod = TM_NO_THREADING;
        _data->maxcon = DEFAULT_MAX_CONNECTIONS;
                
        _connections.SetResizeChange( HEAP_RESIZE_AMOUNT );
        for ( UInt32 i=0; i<_connections.GetArraySize(); i++ )
        {
                _connections.SetEntry( i                                , 
                                       new CTCPServerConnection( this , 
                                                                 i    ) );
        }
}

/*-------------------------------------------------------------------------*/

CTCPServerSocket::~CTCPServerSocket()
{TRACE;
        /*
         *      Clean everything up
         */
        Close();       

        /*
         *      Cleanup connection data
         */
        for ( Int32 i=0; i<=_connections.GetLast(); i++ )
        {
                delete (CTCPServerConnection*)_connections[ i ];
        }
}

/*-------------------------------------------------------------------------*/

void
CTCPServerSocket::RegisterEvents( void )
{TRACE;

    ClientDataRecievedEvent.Initialize();
    ClientDataSentEvent.Initialize();
    ClientConnectedEvent.Initialize();
    ClientDisconnectedEvent.Initialize();
    ClientErrorEvent.Initialize();
    ServerSocketOpenedEvent.Initialize();
    ServerSocketClosedEvent.Initialize();
    ServerSocketErrorEvent.Initialize();
    ServerSocketClientErrorEvent.Initialize();
    ServerSocketMaxConnectionsChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

/*
 *	Obtain pointer to connection data by means of connection index
 */
CTCPServerConnection* 
CTCPServerSocket::GetConnection( UInt32 index )
 {TRACE;

	return (CTCPServerConnection*)_connections[ index ];
}

/*-------------------------------------------------------------------------*/

void 
CTCPServerSocket::Update( void )
{TRACE;
        if ( _active )//&& ( _data->threadmethod == TM_NO_THREADING ) )
        {
                /*
                 *      Accept new incoming connections
                 */
                AcceptClients();
                
                /*
                 *      Update the client connections
                 */
                if ( !_blocking )
                {
                        for ( UInt32 i=0; i<_data->maxcon; ++i )
                        {
                                ((CTCPServerConnection*)(_connections[ i ]))->Update();
                        } 
                }
        }                                       
}

/*-------------------------------------------------------------------------*/   

void
CTCPServerSocket::AcceptClients( void )
{TRACE;
        int s;          /* s is where the data is stored from the select function */
        int nfds;       /* This is used for Compatibility */
        fd_set conn;    /* Setup the read variable for the Select function */
        
        if ( _data->connectcount < _data->maxcon )
        {
                FD_ZERO( &conn ); // Set the data in conn to nothing
                FD_SET( _data->sockid , 
                        &conn         ); // Tell it to get the data from the Listening Socket
                        
                s = 0;        
                if ( !_data->blocking )                        
                {
                        /* 
                         *      Up the nfds value by one, shouldnt be the same for each
                         *      client that connects for compatability reasons
                         *      Doing a select first will ensure that we don't block on accept()
                         */
                        nfds = (int)_data->sockid+1;
                        s = select( nfds            , 
                                    &conn           , 
                                    NULL            , 
                                    NULL            , 
                                    &_data->timeout ); // Is there any data coming in?
                                                        
                        if ( s > 0 ) /* Someone is trying to Connect */
                        {
                                CTCPServerConnection* clientcon;
                                int aint;
                                for ( UInt32 i=0; i<_data->maxcon; ++i )
                                {
                                        clientcon = (CTCPServerConnection*)(_connections[ i ]);
                                        if ( !clientcon->IsActive() )                                
                                        {
                                                aint = sizeof( struct sockaddr );
                                                clientcon->_data->sockid = accept( _data->sockid                                    ,
                                                                                   (struct sockaddr*) &clientcon->_data->clientaddr ,
                                                                                   &aint                                            );

                                                /*
                                                 *      Client Connected
                                                 *      Store the Ip of the Client that Just Connected.
                                                 */
                                                char clientip[ 20 ];
                                                WSTS_inet_ntoa( clientcon->_data->clientaddr.sin_addr ,
                                                                clientip                              );                                                        
                                                clientcon->_data->clientip = clientip;
                                                
	                                        /*
	                                         *      Set the socket into the desired mode of operation
	                                         *      ie blocking or non-blocking mode
	                                         */
                                                unsigned long blockmode = _blocking;
                                                if ( ioctlsocket( _data->sockid           , 
                                                                  FIONBIO                 , 
                                                                  (u_long FAR*)&blockmode ) == SOCKET_ERROR )
                                                {
                                                        clientcon->_data->sockid = 0;
                                                        return;
                                                }	                                         
	                                        clientcon->_blocking = _blocking;             	                                                
                                                
                                                ++_data->connectcount;
                                                clientcon->_active = true;
                                                                                    
                                                /*
                                                 *      Call the on client connect event handler      
                                                 */
                                                NotifyObservers( ClientConnectedEvent );                                                                                 

                                                return;
                                        }                                        
                                }   
                        }
                }                        
        }        
} 

/*-------------------------------------------------------------------------*/

bool
CTCPServerSocket::IsActive( void ) const 
{TRACE;
        return _active;
}

/*-------------------------------------------------------------------------*/

bool 
CTCPServerSocket::IsBlocking( void ) const
{TRACE;
        return _blocking;
}
                        
/*-------------------------------------------------------------------------*/

bool
CTCPServerSocket::ListenOnPort( UInt16 servport )
{TRACE;

        /*
         *      Close all existing connections
         */
        if ( IsActive() )
        {
                Close();
        }
        
        /*
         *      === Microsoft Windows implementation === 
         */        
        #ifdef GUCEF_MSWIN_BUILD
        
        int error = 0;
        _data->sockid = WSTS_socket( AF_INET     ,    /* Go over TCP/IP */
	                             SOCK_STREAM ,    /* This is a stream-oriented socket */
	                             IPPROTO_TCP ,    /* Use TCP rather than UDP */
	                             &error      );   
                
        if ( _data->sockid == INVALID_SOCKET ) 
        {
            GUCEF_ERROR_LOG( 1, "Socket error: " + CORE::Int32ToString( error ) );
            TServerSocketErrorEventData eData( error );
            NotifyObservers( ServerSocketErrorEvent, &eData );                                                      
            return false;			
        }
	
        /* Set the desired blocking mode */
        int mode = _blocking;
        if ( ioctlsocket( _data->sockid      , 
                          FIONBIO            , 
                          (u_long FAR*)&mode ) == SOCKET_ERROR )
        {
                NotifyObservers( ServerSocketErrorEvent );
                return false;
        }	    
	
	/*
	 *      We set the protocol family to TCP/IP
	 */
	_data->serverinfo.sin_family = AF_INET;
	
	/* 
	 *      Since this socket is listening for connections, any local address will do
	 */
	_data->serverinfo.sin_addr.s_addr = INADDR_ANY;	
	
        /* 
         *      Convert integer servport to network-byte order
         *      and insert into the port field	
         */
	_data->serverinfo.sin_port = htons( servport );		    

        /* 
         *      Bind the socket to our local server address
         */
	int retval = WSTS_bind( _data->sockid                  , 
	                        (LPSOCKADDR)&_data->serverinfo , 
	                        sizeof( _data->serverinfo )    ,
	                        &error                        );

	if ( retval == SOCKET_ERROR ) 
	{
		GUCEF_ERROR_LOG( 1, "Socket error: " + CORE::Int32ToString( error ) );
		TServerSocketErrorEventData eData( error );
		NotifyObservers( ServerSocketErrorEvent, &eData );		
		return false;
	}
	
	/*
	 *      Set the socket into the desired mode of operation
	 *      ie blocking or non-blocking mode
	 */
	unsigned long blockarg;
	_blocking ? blockarg = 0 : blockarg = 1; 
	ioctlsocket( _data->sockid ,
	             FIONBIO       ,
	             &blockarg     );	               
	
        /*
         *      Make the socket listen
         *
         *      Up to maxcon connections may wait at any
	 *      one time to be accept()'ed
         */
	retval = WSTS_listen( _data->sockid      , 
	                      _data->maxcon      ,
	                      &error             );		

	if ( retval == SOCKET_ERROR ) 
	{
		TServerSocketErrorEventData eData( error );
		NotifyObservers( ServerSocketErrorEvent, &eData );                
		return false;
	}
	_active = true;
	
    NotifyObservers( ServerSocketOpenedEvent );
	
	m_port = servport;
	
	/*
	 *      Accept new connections if there are any.
	 */
        AcceptClients();
                                 
        return true;
        
        /*
         *      === Linux implementation === 
         */         
        #elif GUCEF_LINUX_BUILD
            #error implementation not implemented
        #elif
        return false;
        #endif                
}

/*-------------------------------------------------------------------------*/

UInt32 
CTCPServerSocket::GetMaxConnections( void ) const
{TRACE; 
        return _connections.GetCount(); 
}

/*-------------------------------------------------------------------------*/

UInt32 
CTCPServerSocket::GetActiveCount( void ) const
{TRACE; 
        return _acount; 
}

/*-------------------------------------------------------------------------*/

void
CTCPServerSocket::Close( void )
{TRACE;

        /*
         *      Close the socket
         *      By setting active to false the thread will terminate
         */
        _datalock.Lock();
        
        if ( IsActive() )
        {        	
        	//StopAndWait();
        	
                closesocket( _data->sockid );                    
                                
                NotifyObservers( ServerSocketClosedEvent );
                                   
                for( UInt32 i=0; i<_connections.GetCount(); ++i )
                {
                        ((CTCPServerConnection*)_connections[ i ])->Close();
                }
        }                
        _datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

/**
 *	Member function that can be used to set the threading method for
 *	a specific client connection. Returns wheter or not successfull.
 */
void
CTCPServerSocket::SetClientThreading( bool thread )
{TRACE;       
	/*if ( ( method != TM_THREAD_PER_EVENT ) &&
             ( method != TM_CONSUMER_THREAD ) &&
             ( method != TM_NO_THREADING ) ) return false;

	if ( !datalock.Lock_Mutex() ) return false;
	if ( (Int32)connection <= connections.Last() )
        {
        	((CTCPServerConnection*)connections[ connection ])->Threading_Method( method );
        }
        datalock.Unlock_Mutex();    */
        //return false;
}

/*-------------------------------------------------------------------------*/

/**
 *	Member function that can be used to get the threading method for
 *	a specific client connection. Returns -1 if unsuccessfull.
 */
bool
CTCPServerSocket::GetClientThreading( void ) const
{TRACE;
/*	if ( !datalock.Lock_Mutex() ) return -1;
	if ( (Int32)connection <= connections.Last() )
        {
        	int method = ((CTCPServerConnection*)connections[ connection ])->Threading_Method();
                datalock.Unlock_Mutex();
                return method;
        }
        datalock.Unlock_Mutex();  */
        return false;
}

/*-------------------------------------------------------------------------*/

UInt16
CTCPServerSocket::GetPort( void ) const
{TRACE;
        return m_port;        
}

/*-------------------------------------------------------------------------*/

void 
CTCPServerSocket::OnClientRead( CTCPServerConnection* connection ,
                                const UInt32 connectionid        ,
                                const char* data                 ,
                                const UInt16 recieved            ,
                                UInt16& keepbytes                )
{TRACE;
        _datalock.Lock();
        
        NotifyObservers( ClientDataRecievedEvent );
        /*
        if ( _iface )
        {
                _iface->OnClientRead( *this        ,
                                      *connection  ,
                                      connectionid ,
                                      data         ,
                                      recieved     ,
                                      keepbytes    );
        } */
        _datalock.Unlock();        
}

/*-------------------------------------------------------------------------*/

void 
CTCPServerSocket::OnClientConnectionClosed( CTCPServerConnection* connection ,
                                            const UInt32 connectionid        ,
                                            bool closedbyclient              )
{TRACE;
        _datalock.Lock();
        
        NotifyObservers( ClientDisconnectedEvent );
        /*
        if ( _iface )
        {
                _iface->OnClientDisconnect( *this          ,
                                            *connection    ,
                                            connectionid   ,
                                            closedbyclient );
        } */
        _datalock.Unlock();        
}                                            

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
