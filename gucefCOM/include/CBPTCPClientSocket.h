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

#ifndef CBPTCPCLIENTSOCKET_H
#define CBPTCPCLIENTSOCKET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define CDYNAMICBUFFER_H
#endif /* CDYNAMICBUFFER_H ? */

#ifndef CTCPCLIENTSOCKETINTERFACE_H
#include "CTCPClientSocketInterface.h"
#define CTCPCLIENTSOCKETINTERFACE_H
#endif /* CTCPCLIENTSOCKETINTERFACE_H */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"  /* GUCEF COM specific macros and build defs */
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

/*
 *      forward declaration of interface base class
 */
class CBPTCPClientSocketInterface;

/*-------------------------------------------------------------------------*/

/**
 *      Basic Protocol TCP Client socket class
 *      Just like the standard TCP client socket class but adds a simple protocol
 *      layer. This layer will seperate recieved transmissions and will validate the
 *      length of each transmission. 
 *      Appart from that it's just the normal raw data stream.
 */
class EXPORT_CPP CBPTCPClientSocket : public COMCORE::CTCPClientSocketInterface
{
        public:
        
        CBPTCPClientSocket( bool isblocking );
        
        virtual ~CBPTCPClientSocket();

        bool ConnectTo( const CORE::CString& address , 
                        UInt16 port                  ); 

        /**
         *      Attempts to reconnect to the server provided with
         *      Connect_To(). If Connect_To() has not yet been called then this
         *      member function has no effect.
         */
        bool Reconnect( void );
        
        void Close( void );

        /**
         *      Attempt to send the data and returns imediatly.
         *      The return value indicates the success/failure.
         *      You can call this from multiple threads, however if you do then
         *      the order in which the data will be send should be conssiddered
         *      to be random.        
         */
        bool Send( const void* data   , 
                   UInt32 length      ,
                   UInt32 timeout = 0 );
                       
        bool Send( const CORE::CString& data );                       

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
        
        UInt32 GetMaxRead( void ) const;

        /** 
         *
         */
        bool IsActive( void ) const; 

        /**
         *      
         */       
        bool IsBlocking( void ) const;  
        
        void SetInterface( CBPTCPClientSocketInterface* iface );
        
        CBPTCPClientSocketInterface* GetInterface( void ) const;
        
        protected:
        
        /**
         *      Event handler called when data has been recieved.
         *      data is of type char but should be considdered to be a
         *      consequtive block of data of size length.
         */
        virtual void OnRead( COMCORE::CTCPClientSocket &socket ,
                             const char *data                  ,
                             UInt32 size                       ,
                             UInt32 &keepbytes                 );

        /**
         *      Event handler called when we are about to write to the socket.
         */
        virtual void OnWrite( COMCORE::CTCPClientSocket &socket ,
                              const void* data                  ,
                              UInt32 length                     );

        /**
         *      Event handler called when we are about to connect to a server
         */
        virtual void OnConnecting( COMCORE::CTCPClientSocket &socket );

        /**
         *      Event handler called when we have successfully connected to
         *      a server socket.
         */
        virtual void OnConnected( COMCORE::CTCPClientSocket &socket );

        /**
         *      Event handler called when have lost the connection.
         *      The socket is now closed.
         */
        virtual void OnDisconnect( COMCORE::CTCPClientSocket &socket );

        /**
         *      Event handler called when a socket error occured.
         */
        virtual void OnError( COMCORE::CTCPClientSocket &socket ,
                              TSocketError errorcode            );
                
        
        private:
        
        bool ParseTCPTransmission( const char *data        ,
                                   UInt32 size             ,
                                   const char*& transstart ,
                                   UInt32& transsize       );

        CBPTCPClientSocket( const CBPTCPClientSocket& src );            /**< making a copy of a socket doesnt make sense */
        CBPTCPClientSocket& operator=( const CBPTCPClientSocket& src ); /**< making a copy of a socket doesnt make sense */
        
        private:
        
        CORE::CDynamicBuffer m_sendbuffer;
        COMCORE::CTCPClientSocket m_socket;
        CBPTCPClientSocketInterface* m_iface;       
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CBPTCPCLIENTSOCKET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-09-2005 :
        - Initial version
          
-----------------------------------------------------------------------------*/
