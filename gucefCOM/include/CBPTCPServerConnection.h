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

#ifndef CBPTCPSERVERCONNECTION_H
#define CBPTCPSERVERCONNECTION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CTCPSERVERCONNECTION_H
#include "CTCPServerConnection.h"      /* raw server connection class */
#define CTCPSERVERCONNECTION_H
#endif /* CTCPSERVERCONNECTION_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"       /* build defines */
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      STRUCTS                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

struct STCPServerConData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Forward declarations of classes used
 */
class CTCPServerSocket;

/*-------------------------------------------------------------------------*/

/**
 *      Class that represents a single connection on a TCP server socket
 */
class GUCEFCOM_EXPORT_CPP CBPTCPServerConnection
{
        public:
        
        typedef COMCORE::CSocket::TSocketError TSocketError;
        typedef COMCORE::CSocket::TIPAddress TIPAddress;
        typedef COMCORE::CSocket::TSocketType TSocketType;
        
        CBPTCPServerConnection( COMCORE::CTCPServerConnection* socket );

        virtual ~CBPTCPServerConnection();        

        /**
         *      Closes the connection to the client.
         *      This can cause this object to be deleted by the parent server.
         *      After a call to Close() the object should no longer be accessed
         *      by the user.
         */
        void Close( void );

        /**
         *      Send data to client.
         *      Wheter or not the elements of buffer are actually char's doesnt
         *      matter as long as they are 1 byte in size and stored consecutively
         */
        bool Send( const char *buffer , 
                   UInt32 length      );

        bool IsActive( void ) const;

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
        void SetMaxRead( UInt32 mr );

        /**
         *      This returns the current max read value.
         *      This represents the current setting for the maximum number of
         *      bytes that will be read from the data buffer.
         */
        UInt32 GetMaxRead( void ) const;
        
        TSocketType GetType( void ) const;             /* enum value, type of socket object */
        
        UInt32 GetSocketID( void ) const;
                             
        private:
                
        CORE::CDynamicBuffer m_sendbuffer;
        COMCORE::CTCPServerConnection* m_socket;
        
        CBPTCPServerConnection( void ); /**< private default constructor because we need data */                             
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CBPTCPSERVERCONNECTION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-09-2005 :
        - Initial version

-----------------------------------------------------------------------------*/