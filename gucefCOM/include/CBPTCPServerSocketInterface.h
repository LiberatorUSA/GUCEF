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

#ifndef CBPTCPSERVERSOCKETINTERFACE_H
#define CBPTCPSERVERSOCKETINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CBPTCPSERVERSOCKET_H
#include "CBPTCPServerSocket.h" /* Basic Protocol TCP socket class for which this is the interface */
#define CBPTCPSERVERSOCKET_H
#endif /* CBPTCPSERVERSOCKET_H ? */

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

/**
 *      This class provides virtual methods for CTCPServerSocket events
 *      Derive a class from this and pass the overrided methods of this
 *      class as the event handlers for CTCPServerSocket
 */
class EXPORT_CPP CBPTCPServerSocketInterface
{
        public:
        
        typedef COMCORE::CSocket::TSocketError TSocketError;
        typedef COMCORE::CSocket::TIPAddress TIPAddress;
        typedef COMCORE::CSocket::TSocketType TSocketType;        

        protected:
        friend class CBPTCPServerSocket;
        friend class CBPTCPServerConnection;               

        /**
         *      we recieved data on a client connection.
         *      Note that although data is of type char* it can be considdered
         *      to be almost any type as long as the type is 1 byte in size.
         *      It is typed as char to make it easy to use with text based
         *      protocols, but feel free to cast to whatever you need. 
         */
        virtual void OnClientRead( CBPTCPServerSocket& socket     ,
                                   CBPTCPServerConnection& client ,
                                   UInt32 connection              ,
                                   const char *data               ,
                                   UInt16 size                    ) {}

        /**
         *      Data is about to be sent to a client
         */
        virtual void OnClientWrite( CBPTCPServerSocket& socket     ,
                                    CBPTCPServerConnection& client ,
                                    UInt32 connection              ,
                                    const char *data               ,
                                    UInt32 size                    ) {}

        /**
         *      a connection with a client has been established
         */
        virtual void OnClientConnected( CBPTCPServerSocket& socket        ,
                                        CBPTCPServerConnection& clientcon ,
                                        UInt32 connection                 ) {}

        /**
         *      A connection with a client has been closed
         */
        virtual void OnClientDisconnect( CBPTCPServerSocket& socket     ,
                                         CBPTCPServerConnection& client ,
                                         UInt32 connection              ,
                                         bool clientclose               ) {}

        /**
         *      an error occured on a client connection
         */
        virtual void OnClientError( CBPTCPServerSocket& socket     ,
                                    CBPTCPServerConnection& client ,
                                    UInt32 connection              ,
                                    TSocketError *errorcode        ) {}

        /**
         *      The server socket has begun listning on the port given
         */
        virtual void OnOpen( CBPTCPServerSocket &socket ) {}

        /**
         *      the server socket has stopped listning and the client
         *      connections have been closed
         */
        virtual void OnClose( CBPTCPServerSocket &socket ) {}

        /**
         *      The maximum storage room for connection data has been changed.
         *      You can use this event to resize your own data storage to match
         *      the new size. This will allow you to store data per connection
         *      and access it using the event connection index.
         */
        virtual void OnMaxConnectionsChanged( CBPTCPServerSocket &socket ,
                                              UInt32 old_max           ,
                                              UInt32 new_max           ) {}

        /**
         *      an error occured on the server socket
         */
        virtual void OnError( CBPTCPServerSocket &socket ,
                              TSocketError errorcode   ) {}

        public:

        virtual ~CBPTCPServerSocketInterface() {}        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CBPTCPSERVERSOCKETINTERFACE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-09-2005 :
        - Initial version

-----------------------------------------------------------------------------*/


