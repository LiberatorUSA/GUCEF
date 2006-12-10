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

#ifndef GUCEFCORE_H
#include "gucefCORE.h"          /* GUCEF CORE library API */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefCOMCORE.h"       /* GUCEF COMCORE library API */
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;
using namespace GUCEF::MT;
using namespace GUCEF::CORE;
using namespace GUCEF::COMCORE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#if 0 // Needs conversion to new event system
class ServerIFace : public CTCPServerSocketInterface
{
        public:
        
        virtual ~ServerIFace()
        {
        }
        
        protected: 
        
        /**
         *      we recieved data on a client connection.
         *      Note that although data is of type char* it can be considdered
         *      to be almost any type alsong as the type is 1 byte in size.
         *      It is typed as char to make it easy to use with text based
         *      protocols, but feel free to cast to whatever you need. 
         */
        virtual void OnClientRead( CTCPServerSocket &socket       ,
                                   CORE::UInt32 connection        ,
                                   const char *data               ,
                                   CORE::UInt32 size              ,
                                   CORE::UInt32 &keepbytes        ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnClientRead( CTCPServerSocket &socket, UInt32 connection, const char *data, UInt32 size, UInt32 &keepbytes )" );
        }

        /**
         *      Data is about to be sent to a client
         */
        virtual void OnClientWrite( CTCPServerSocket &socket ,
                                    CORE::UInt32 connection        ,
                                    const char *data         ,
                                    CORE::UInt32 size              ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnClientWrite( CTCPServerSocket &socket, UInt32 connection, const char *data, UInt32 size )" );
                DEBUGOUTPUTis( size, " bytes will be written" );
        }

        /**
         *      a connection with a client has been established
         */
        virtual void OnClientConnected( CTCPServerSocket &socket        ,
                                        CTCPServerConnection& clientcon ,
                                        CORE::UInt32 connection               ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnClientConnected( CTCPServerSocket &socket, CTCPServerConnection& clientcon, UInt32 connection )" );
                DEBUGOUTPUTsi( "New client at index ", connection );
        }

        /**
         *      A connection with a client has been closed
         */
        virtual void OnClientDisconnect( CTCPServerSocket &socket ,
                                         CORE::UInt32 connection        ,
                                         bool clientclose         ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnClientDisconnect( CTCPServerSocket &socket, UInt32 connection, bool clientclose )" );
                DEBUGOUTPUTsi( "Client disconnect at index ", connection );
        }

        /**
         *      an error occured on a client connection
         */
        virtual void OnClientError( CTCPServerSocket &socket ,
                                    CORE::UInt32 connection        ,
                                    TSocketError errorcode   ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnClientError( CTCPServerSocket &socket, UInt32 connection, TSocketError errorcode )" );
                DEBUGOUTPUTsi( "Error on client connection ", connection );
        }

        /**
         *      The server socket has begun listning on the port given
         */
        virtual void OnOpen( CTCPServerSocket &socket ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnOpen( CTCPServerSocket &socket )" );
        }

        /**
         *      the server socket has stopped listning and the client
         *      connections have been closed
         */
        virtual void OnClose( CTCPServerSocket &socket ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnClose( CTCPServerSocket &socket )" );
        }

        /**
         *      The maximum storage room for connection data has been changed.
         *      You can use this event to resize your own data storage to match
         *      the new size. This will allow you to store data per connection
         *      and access it using the event connection index.
         */
        virtual void OnMaxConnectionsChanged( CTCPServerSocket &socket ,
                                              CORE::UInt32 old_max           ,
                                              CORE::UInt32 new_max           ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnMaxConnectionsChanged( CTCPServerSocket &socket, UInt32 old_max, UInt32 new_max )" );
        }

        /**
         *      an error occured on the server socket
         */
        virtual void OnError( CTCPServerSocket &socket ,
                              TSocketError errorcode   ) 
        {
                DEBUGOUTPUT( "ServerIFace::OnError( CTCPServerSocket &socket, TSocketError errorcode )" );
        }                       
};

/*-------------------------------------------------------------------------*/

class ClientIFace : public CTCPClientSocketInterface
{
        protected:
        
        /**
         *      Event handler called when data has been recieved.
         *      data is of type char but should be considdered to be a
         *      consequtive block of data of size length.
         */
        virtual void OnRead( CTCPClientSocket &socket ,
                             const char *data         ,
                             CORE::UInt32 size              ,
                             CORE::UInt32 &keepbytes        ) 
        {
                DEBUGOUTPUT( "ClientIFace::OnRead( CTCPClientSocket &socket, const char *data, UInt32 size, UInt32 &keepbytes )" );
        }

        /**
         *      Event handler called when we are about to write to the socket.
         */
        virtual void OnWrite( CTCPClientSocket &socket ,
                              const char *data         ,
                              CORE::UInt32 length            ) 
        {
                DEBUGOUTPUT( "ClientIFace::OnWrite( CTCPClientSocket &socket, const char *data, UInt32 length )" );
        }

        /**
         *      Event handler called when we are about to connect to a server
         */
        virtual void OnConnecting( CTCPClientSocket &socket ) 
        {
                DEBUGOUTPUT( "ClientIFace::OnConnecting( CTCPClientSocket &socket )" );
        }

        /**
         *      Event handler called when we have successfully connected to
         *      a server socket.
         */
        virtual void OnConnected( CTCPClientSocket &socket ) 
        {
                DEBUGOUTPUT( "ClientIFace::OnConnected( CTCPClientSocket &socket )" );
        }

        /**
         *      Event handler called when have lost the connection.
         *      The socket is now closed.
         */
        virtual void OnDisconnect( CTCPClientSocket &socket ) 
        {
                DEBUGOUTPUT( "ClientIFace::OnDisconnect( CTCPClientSocket &socket )" );
        }

        /**
         *      Event handler called when a socket error occured.
         */
        virtual void OnError( CTCPClientSocket &socket ,
                              TSocketError errorcode   ) 
        {
                DEBUGOUTPUT( "ClientIFace::OnError( CTCPClientSocket &socket, TSocketError errorcode )" );
        }       
};

/*-------------------------------------------------------------------------*/

class CUDPTestobj : public CIUDPSocketEventHandler
{

        public:
        CUDPSocket socket;
        
        CUDPTestobj( bool blocking )
                : socket( blocking )
        {
                socket.RegisterEventHandler( this );        
        }
        
        ~CUDPTestobj()
        {
                socket.UnregisterEventHandler( this );       
        }
        
        protected:
        
        virtual void OnUDPSocketEvent( CUDPSocket& socket                ,
                                       CUDPSocket::TUDPSocketEvent event )
        {
                switch ( event )
                {
                        case CUDPSocket::UDP_PACKET_RECIEVED :
                        {
                                DEBUGOUTPUTsi( "Recieved packet on UDP socket ", socket.GetSocketID() );
                                
                                char buf[ 10 ];
                                CUDPSocket::TIPAddress src;
                                socket.Recieve( src, buf, 10 );
                                if ( memcmp( buf, "TESTPACKET", 10 ) == 0 )
                                {
                                        CORE::Int32 sendbytes = socket.SendPacketTo( src, "TESTPACKET", 10 );        
                                        if ( sendbytes != 10 )
                                        {
                                                DEBUGOUTPUTsi( "Incorrect send bytes ", sendbytes );        
                                        }
                                }
                                break;
                        }                
                        case CUDPSocket::UDP_SOCKET_CLOSED :
                        {
                                DEBUGOUTPUTsi( "Closed UDP socket ", socket.GetSocketID() );
                                break;
                        }
                        case CUDPSocket::UDP_SOCKET_OPENED :
                        {
                                DEBUGOUTPUTsi( "Opened UDP socket ", socket.GetSocketID() );
                                break;
                        }
                        case CUDPSocket::UDP_SOCKET_ERROR :
                        {
                                DEBUGOUTPUTsi( "Error on UDP socket ", socket.GetSocketID() );
                                break;
                        }                                                
                        default :
                        {
                                break;
                        }                        
                }
        }                                                               
};

/*-------------------------------------------------------------------------*/

class CTestTCPClientServer : public CGUCEFAppSubSystem
{
        public:
        
        CTestTCPClientServer( bool block )
                : _serversock( block ) ,
                  _clientsock( block ) ,
                  _udpsock1( block ) ,
                  _udpsock2( block )
        {

        }
        
        protected:
                   
        virtual void OnNotify( CORE::CNotifier* notifier           ,
                               const CORE::CEvent& eventid         ,
                               CORE::CICloneable* eventdata = NULL )
        {
                if ( eventid == CGUCEFApplication::AppInitEvent )
                {
                        _clientsock.Subscribe( &AsObserver() );
                        _serversock.SetInterface( &_serveriface );
                
                        _serversock.ListenOnPort( 10000 );
                        _clientsock.ConnectTo( "127.0.0.1" ,
                                               10000       );
                                               
                        _udpsock1.socket.Open( 11000 );
                        _udpsock2.socket.Open( 11001 );
                        _udpsock2.socket.SendPacketTo( "127.0.0.1", 11000, "TESTPACKET", 10 );                                                       
                }
        }
        
        /**
         *      Called each event pump update cycle after the events for 
         *      the current cycle have been pumped to all clients.
         *
         *      @param tickcount the tick count when the Update process commenced.
         *      @param deltaticks ticks since the last Update process commenced.
         */        
        virtual void OnUpdate( const CORE::UInt32 tickcount  ,
                               const CORE::UInt32 deltaticks )
        {
                /* don't hogg the CPU */
                ThreadDelay( 10 );
        }
        
        private:
        CORE::UInt32 _appinitevent; 
        CTCPServerSocket _serversock; 
        CTCPClientSocket _clientsock; 
        ServerIFace _serveriface;
        ClientIFace _clientiface;
        CUDPTestobj _udpsock1;
        CUDPTestobj _udpsock2;                                    
};
#endif