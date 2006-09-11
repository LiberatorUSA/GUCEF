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

#ifndef CBPTCPSERVERSOCKET_H
#define CBPTCPSERVERSOCKET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CTCPSERVERSOCKETINTERFACE_H
#include "CTCPServerSocketInterface.h"
#define CTCPSERVERSOCKETINTERFACE_H
#endif /* CTCPSERVERSOCKETINTERFACE_H ? */

#ifndef CBPTCPSERVERCONNECTION_H
#include "CBPTCPServerConnection.h"
#define CBPTCPSERVERCONNECTION_H
#endif /* CBPTCPSERVERCONNECTION_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"       /* build defines */
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOMC_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Forward declaration of interface base class
 */
class CBPTCPServerSocketInterface;

/*-------------------------------------------------------------------------*/

class EXPORT_CPP CBPTCPServerSocket : public COMCORE::CTCPServerSocketInterface
{
        public:
        
        typedef COMCORE::CSocket::TSocketError TSocketError;
        typedef COMCORE::CSocket::TIPAddress TIPAddress;
        typedef COMCORE::CSocket::TSocketType TSocketType;        
        
        CBPTCPServerSocket( bool blocking );
        
        virtual ~CBPTCPServerSocket();        

        UInt32 GetActiveCount( void ) const;
        
        void SetInterface( CBPTCPServerSocketInterface *new_iface );
        
        CBPTCPServerSocketInterface* GetInterface( void ) const;

        bool ListenOnPort( UInt16 port );
        
        void Close( void );
        
        UInt16 GetPort( void ) const;
        
        bool IsActive( void ) const;
        
        /**
         *      Returns wheter the socket is blocking or non-blocking
         */
        bool IsBlocking( void ) const;
        
        UInt32 GetMaxConnections( void ) const;
                                             
        protected:
        
        /**
         *      we recieved data on a client connection.
         *      Note that although data is of type char* it can be considdered
         *      to be almost any type as long as the type is 1 byte in size.
         *      It is typed as char to make it easy to use with text based
         *      protocols, but feel free to cast to whatever you need. 
         */
        virtual void OnClientRead( COMCORE::CTCPServerSocket& socket     ,
                                   COMCORE::CTCPServerConnection& client ,
                                   UInt32 connection                     ,
                                   const char *data                      ,
                                   UInt16 size                           ,
                                   UInt16 &keepbytes                     );

        /**
         *      Data is about to be sent to a client
         */
        virtual void OnClientWrite( COMCORE::CTCPServerSocket& socket     ,
                                    COMCORE::CTCPServerConnection& client ,
                                    UInt32 connection                     ,
                                    const char *data                      ,
                                    UInt32 size                           );

        /**
         *      a connection with a client has been established
         */
        virtual void OnClientConnected( COMCORE::CTCPServerSocket& socket        ,
                                        COMCORE::CTCPServerConnection& clientcon ,
                                        UInt32 connection                        );

        /**
         *      A connection with a client has been closed
         */
        virtual void OnClientDisconnect( COMCORE::CTCPServerSocket& socket     ,
                                         COMCORE::CTCPServerConnection& client ,
                                         UInt32 connection                     ,
                                         bool clientclose                      );

        /**
         *      an error occured on a client connection
         */
        virtual void OnClientError( COMCORE::CTCPServerSocket& socket     ,
                                    COMCORE::CTCPServerConnection& client ,
                                    UInt32 connection                     ,
                                    TSocketError *errorcode               );

        /**
         *      The server socket has begun listning on the port given
         */
        virtual void OnOpen( COMCORE::CTCPServerSocket &socket );

        /**
         *      the server socket has stopped listning and the client
         *      connections have been closed
         */
        virtual void OnClose( COMCORE::CTCPServerSocket &socket );

        /**
         *      The maximum storage room for connection data has been changed.
         *      You can use this event to resize your own data storage to match
         *      the new size. This will allow you to store data per connection
         *      and access it using the event connection index.
         */
        virtual void OnMaxConnectionsChanged( COMCORE::CTCPServerSocket &socket ,
                                              UInt32 old_max                    ,
                                              UInt32 new_max                    );

        /**
         *      an error occured on the server socket
         */
        virtual void OnError( COMCORE::CTCPServerSocket &socket ,
                              TSocketError errorcode            );                                     

        private:
        
        bool ParseTCPTransmission( const char *data        ,
                                   UInt16 size             ,
                                   const char*& transstart ,
                                   UInt16& transsize       );

        CBPTCPServerSocket( void );         
        
        private:                 

        COMCORE::CTCPServerSocket m_socket;
        CBPTCPServerSocketInterface* m_iface;
        CORE::CDynamicArray m_connections;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CBPTCPSERVERSOCKET_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-09-2005 :
        - Initial version.

-----------------------------------------------------------------------------*/


