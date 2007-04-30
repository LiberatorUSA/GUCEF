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

#ifndef CTCPSERVERSOCKETINTERFACE_H
#define CTCPSERVERSOCKETINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h" /* TCP socket class for which this is the interface */
#define CTCPSERVERSOCKET_H
#endif /* CTCPSERVERSOCKET_H ? */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"       /* build defines */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

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
class GUCEF_COMCORE_EXPORT_CPP CTCPServerSocketInterface
{
        public:
        
        typedef CSocket::TSocketError TSocketError;
        
        CTCPServerSocketInterface( void );
        
        CTCPServerSocketInterface( const CTCPServerSocketInterface& src );
        
        CTCPServerSocketInterface& operator=( const CTCPServerSocketInterface& src );
        
        virtual ~CTCPServerSocketInterface();
        
        protected:
        friend class CTCPServerSocket;
        friend class CTCPServerConnection;               

        /**
         *      we recieved data on a client connection.
         *      Note that although data is of type char* it can be considdered
         *      to be almost any type as long as the type is 1 byte in size.
         *      It is typed as char to make it easy to use with text based
         *      protocols, but feel free to cast to whatever you need. 
         */
        virtual void OnClientRead( CTCPServerSocket& socket     ,
                                   CTCPServerConnection& client ,
                                   UInt32 connection            ,
                                   const char *data             ,
                                   UInt16 size                  ,
                                   UInt16 &keepbytes            );

        /**
         *      Data is about to be sent to a client
         */
        virtual void OnClientWrite( CTCPServerSocket& socket     ,
                                    CTCPServerConnection& client ,
                                    UInt32 connection            ,
                                    const char *data             ,
                                    UInt32 size                  );

        /**
         *      a connection with a client has been established
         */
        virtual void OnClientConnected( CTCPServerSocket& socket        ,
                                        CTCPServerConnection& clientcon ,
                                        UInt32 connection               );

        /**
         *      A connection with a client has been closed
         */
        virtual void OnClientDisconnect( CTCPServerSocket& socket     ,
                                         CTCPServerConnection& client ,
                                         UInt32 connection            ,
                                         bool clientclose             );

        /**
         *      an error occured on a client connection
         */
        virtual void OnClientError( CTCPServerSocket& socket     ,
                                    CTCPServerConnection& client ,
                                    UInt32 connection            ,
                                    TSocketError *errorcode      );

        /**
         *      The server socket has begun listning on the port given
         */
        virtual void OnOpen( CTCPServerSocket &socket );

        /**
         *      the server socket has stopped listning and the client
         *      connections have been closed
         */
        virtual void OnClose( CTCPServerSocket &socket );

        /**
         *      The maximum storage room for connection data has been changed.
         *      You can use this event to resize your own data storage to match
         *      the new size. This will allow you to store data per connection
         *      and access it using the event connection index.
         */
        virtual void OnMaxConnectionsChanged( CTCPServerSocket &socket ,
                                              UInt32 old_max           ,
                                              UInt32 new_max           );

        /**
         *      an error occured on the server socket
         */
        virtual void OnError( CTCPServerSocket &socket ,
                              TSocketError errorcode   );       
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CTCPSERVERSOCKETINTERFACE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-07-2005 :
        - Added CTCPServerConnection parameter to all client connection related
          event handlers. This helps avoid unnesassary lookups.
- 27-07-2004 :
        - Made the virtual member functions all protected and added friend
          relations.
- 11-04-2004 :
        - Added this section.
        - Added the OnMaxConnectionsChanged() event.

-----------------------------------------------------------------------------*/


