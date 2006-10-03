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

#ifndef CBPTCPCLIENTSOCKETINTERFACE_H
#define CBPTCPCLIENTSOCKETINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CBPTCPCLIENTSOCKET_H
#include "CBPTCPClientSocket.h" /* Basic Protocol TCP socket class for which this is the interface */
#define CBPTCPCLIENTSOCKET_H
#endif /* CBPTCPCLIENTSOCKET_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"       /* build defines */
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

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
 *      This class provides virtual methods for CClientsocket events
 *      Derive a class from this and pass the overrided member functions of this
 *      class as the event handlers for CTCPClientsocket
 */
class GUCEFCOM_EXPORT_CPP CBPTCPClientSocketInterface 
{
        friend class CBPTCPClientSocket;

        protected:
        
        typedef CBPTCPClientSocket::TSocketError TSocketError;

        /**
         *      Event handler called when data has been recieved.
         *      data is of type char but should be considdered to be a
         *      consequtive block of data of size length.
         */
        virtual void OnRead( CBPTCPClientSocket &socket ,
                             const char *data           ,
                             UInt32 size                ) {}

        /**
         *      Event handler called when we are about to write to the socket.
         */
        virtual void OnWrite( CBPTCPClientSocket &socket ,
                              const void* data           ,
                              UInt32 length              ) {}

        /**
         *      Event handler called when we are about to connect to a server
         */
        virtual void OnConnecting( CBPTCPClientSocket &socket ) {}

        /**
         *      Event handler called when we have successfully connected to
         *      a server socket.
         */
        virtual void OnConnected( CBPTCPClientSocket &socket ) {}

        /**
         *      Event handler called when have lost the connection.
         *      The socket is now closed.
         */
        virtual void OnDisconnect( CBPTCPClientSocket &socket ) {}

        /**
         *      Event handler called when a socket error occured.
         */
        virtual void OnError( CBPTCPClientSocket &socket ,
                              TSocketError errorcode     ) {}

        /**
         *      Event handler called after a round-trip time count (RTT)
         *      has been completed. if rtt is -1 then there was an error pinging
         *      most likely a timeout.
         */
        virtual void OnPing( CBPTCPClientSocket &socket ,
                             UInt32 hops                ,
                             Int32 rtt                  ) {}

        public:

        /**
         *      Virtual destructor, the descending classes destructor will get
         *      called when this object is deleted.
         */
        virtual ~CBPTCPClientSocketInterface() {}
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CBPTCPCLIENTSOCKETINTERFACE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-03-2005 :
        - Updated for the move into the COMCORE module
- 13-04-2004 :
        - Got rid of the tab character usage here.
        - Added external include guards.
- 22-01-2004 :
        - Changed pointers to the client into refrences.
        - Added keepbytes to the OnRead() handler. 
- 07-12-2003 :
        - Added this section.
        - Updated header layout to new standard.

-----------------------------------------------------------------------------*/
