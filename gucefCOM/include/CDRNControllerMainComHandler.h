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

#ifndef CDRNCONTROLLERMAINCOMHANDLER_H
#define CDRNCONTROLLERMAINCOMHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CBPTCPSERVERSOCKETINTERFACE_H
#include "CBPTCPServerSocketInterface.h"
#define CBPTCPSERVERSOCKETINTERFACE_H
#endif /* CBPTCPSERVERSOCKETINTERFACE_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"
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

class CDRNController;

/*-------------------------------------------------------------------------*/

/**
 *      Internally used class
 *      Acts as a proxy to the DRN Controller
 */
class CDRNControllerMainComHandler : public COM::CBPTCPServerSocketInterface
{
        public:
        
        CDRNControllerMainComHandler( CDRNController* controller );
        
        ~CDRNControllerMainComHandler();
        
        /**
         *      we recieved data on a client connection.
         *      Note that although data is of type char* it can be considdered
         *      to be almost any type as long as the type is 1 byte in size.
         *      It is typed as char to make it easy to use with text based
         *      protocols, but feel free to cast to whatever you need. 
         */
        virtual void OnClientRead( COM::CBPTCPServerSocket& socket     ,
                                   COM::CBPTCPServerConnection& client ,
                                   UInt32 connection                   ,
                                   const char *data                    ,
                                   UInt32 size                         ,
                                   UInt32 &keepbytes                   );

        /**
         *      Data is about to be sent to a client
         */
        virtual void OnClientWrite( COM::CBPTCPServerSocket& socket     ,
                                    COM::CBPTCPServerConnection& client ,
                                    UInt32 connection                   ,
                                    const char *data                    ,
                                    UInt32 size                         );

        /**
         *      a connection with a client has been established
         */
        virtual void OnClientConnected( COM::CBPTCPServerSocket& socket     ,
                                        COM::CBPTCPServerConnection& client ,
                                        UInt32 connection                   );

        /**
         *      A connection with a client has been closed
         */
        virtual void OnClientDisconnect( COM::CBPTCPServerSocket& socket     ,
                                         COM::CBPTCPServerConnection& client ,
                                         UInt32 connection                   ,
                                         bool clientclose                    );

        /**
         *      an error occured on a client connection
         */
        virtual void OnClientError( COM::CBPTCPServerSocket& socket     ,
                                    COM::CBPTCPServerConnection& client ,
                                    UInt32 connection                   ,
                                    TSocketError *errorcode             );

        /**
         *      The server socket has begun listning on the port given
         */
        virtual void OnOpen( COM::CBPTCPServerSocket& socket );

        /**
         *      the server socket has stopped listning and the client
         *      connections have been closed
         */
        virtual void OnClose( COM::CBPTCPServerSocket& socket );

        /**
         *      The maximum storage room for connection data has been changed.
         *      You can use this event to resize your own data storage to match
         *      the new size. This will allow you to store data per connection
         *      and access it using the event connection index.
         */
        virtual void OnMaxConnectionsChanged( COM::CBPTCPServerSocket& socket ,
                                              UInt32 old_max                  ,
                                              UInt32 new_max                  );

        /**
         *      an error occured on the server socket
         */
        virtual void OnError( COM::CBPTCPServerSocket& socket ,
                              TSocketError errorcode          );
        
        private:
        CDRNControllerMainComHandler( void );
        CDRNControllerMainComHandler( const CDRNControllerMainComHandler& src );
        CDRNControllerMainComHandler& operator=( const CDRNControllerMainComHandler& src );
        
        CDRNController* m_controller;                              
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CDRNCONTROLLERMAINCOMHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-07-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
