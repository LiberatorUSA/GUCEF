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

#ifndef CDRNCONTROLLERTOCLIENTHANDLER_H
#define CDRNCONTROLLERTOCLIENTHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CTCPCLIENTSOCKETINTERFACE_H
#include "CTCPClientSocketInterface.h"
#define CTCPCLIENTSOCKETINTERFACE_H
#endif /* CTCPCLIENTSOCKETINTERFACE_H ? */

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
 *      Internally used class.
 */
class CDRNControllerToClientHandler : public COMCORE::CTCPClientSocketInterface
{
        protected:
        
        /**
         *      Event handler called when data has been recieved.
         *      data is of type char but should be considdered to be a
         *      consequtive block of data of size length.
         */
        virtual void OnRead( CTCPClientSocket &socket ,
                             const char *data         ,
                             UInt32 size              ,
                             UInt32 &keepbytes        ) {}

        /**
         *      Event handler called when we are about to write to the socket.
         */
        virtual void OnWrite( CTCPClientSocket &socket ,
                              const char *data         ,
                              UInt32 length            ) {}

        /**
         *      Event handler called when we are about to connect to a server
         */
        virtual void OnConnecting( CTCPClientSocket &socket ) {}

        /**
         *      Event handler called when we have successfully connected to
         *      a server socket.
         */
        virtual void OnConnected( CTCPClientSocket &socket ) {}

        /**
         *      Event handler called when have lost the connection.
         *      The socket is now closed.
         */
        virtual void OnDisconnect( CTCPClientSocket &socket ) {}

        /**
         *      Event handler called when a socket error occured.
         */
        virtual void OnError( CTCPClientSocket &socket ,
                              TSocketError errorcode   ) {}

        /**
         *      Event handler called after a round-trip time count (RTT)
         *      has been completed. if rtt is -1 then there was an error pinging
         *      most likely a timeout.
         */
        virtual void OnPing( CTCPClientSocket &socket ,
                             UInt32 hops              ,
                             Int32 rtt                ) {}
                             
        public:
        
        CDRNControllerToClientHandler( CDRNClient* client );
        
        ~CDRNControllerToClientHandler();                                    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CDRNCONTROLLERTOCLIENTHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-07-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
