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

#ifndef CGUUDPSOCKET_H
#define CGUUDPSOCKET_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define CDYNAMICBUFFER_H
#endif /* CDYNAMICBUFFER_H ? */

#ifndef CUDPSOCKET_H
#include "CUDPSocket.h"
#define CUDPSOCKET_H
#endif /* CUDPSOCKET_H ? */

#ifndef CIUDPSOCKETEVENTHANDLER_H
#include "CIUDPSocketEventHandler.h"
#define CIUDPSOCKETEVENTHANDLER_H
#endif /* CIUDPSOCKETEVENTHANDLER_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"      /* gucefCOM build config and macros */
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
 *      Forward declarations of classes used
 */
class CIPHUDPSocketEventHandler;

/*-------------------------------------------------------------------------*/

/**
 *      Wraps a standard UDP socket and adds handling for wrangled packets and
 *      packet numbering. These basic things almost always need to be performed 
 *      when using UDP so this class should save you some work. 
 *
 *      Note that you can only communicate with other CPHUDPSocket sockets and not
 *      with standard UDP sockets.
 *
 *      Packet loss is not compensated by the PH UDP socket. If you cannot handle
 *      data loss then you should use a TCP based socket instead of UDP.
 *
 *      Most mangled packets will be filtered out and dropped. This occures if bytes
 *      seem to be lost or added to the packet. The packet checksum must also be 
 *      correct in relation to the packet data payload.
 *      It is possible, in theory, that a packet has the correct size and just happens
 *      to have a correct checksum/data combo while being in fact a mangled packet.
 *      This will only occur in verry rare cases but keep it in mind.
 */
class EXPORT_CPP CPHUDPSocket : public COMCORE::CIUDPSocketEventHandler
{
        public:
        
        /*
         *      Typedef some shortcut types
         */
        typedef COMCORE::CUDPSocket::TIPAddress TIPAddress;         
        typedef COMCORE::CUDPSocket::TIPAddress TIPAddress;
        typedef COMCORE::CUDPSocket::TUDPSocketEvent TUDPSocketEvent;
        
        /**
         *      Constructs a GU protocol UDP socket
         *      It adds some additional functionality ontop of normal UDP
         *      packet handing.
         *
         *      @param minpayloadsize minimal data payload size you will send, used to initialze the buffer
         *      @param sendpackettypes number of different packet types you will send, you cannot send packets with type id's higher then this number
         *      @param rcvpackettypes number of different packet types you will recieve, you cannot recieve packets with type id's higher then this number
         */        
        CPHUDPSocket( UInt32 minpayloadsize = 100 ,
                      UInt16 sendpackettypes = 16 ,
                      UInt16 rcvpackettypes = 16  );
        
        /**
         *      Cleans up the allocated data and closes the socket
         */
        virtual ~CPHUDPSocket();
        
        /**
         *      Returns wheter the socket is currently active
         *
         *      @return activity status of the socket
         */
        bool IsActive( void ) const;
        
        /**
         *      Attempts to open the UDP socket for use.
         *      The port used is left up to the operating system.
         *      This will typicly be used for clients in a client-server
         *      architecture.
         */
        bool Open( void );
        
        /**
         *      Attempts to open the UDP socket for use on the given port.
         *      This will typicly be used for a server in a client-server
         *      architecture or for an peer-to-peer architecture.
         *      Any local address can be used.
         *
         *      @param port port used for sending and/or recieving data
         */        
        bool Open( UInt16 port );
        
        /**
         *      Attempts to open the UDP socket for use on the given local
         *      address and port. Basicly the same as Open( UInt16 port )
         *      but here you binding to a specific local address manditory.
         *
         *      @param localaddr local address to be used for this socket
         *      @param port port used for sending and/or recieving data
         */  
        bool Open( const GUCEF::CORE::CString& localaddr ,
                   UInt16 port                           );
                   
        UInt16 GetPort( void ) const;                   
        
        /**
         *      Closes the socket connection.
         */
        void Close( bool force = false );
        
        /**
         *      Attempts to send a UDP packet to the given destination.
         *      Note that the data you send must fit within the payload
         *      space of a UDP packet.
         *      Note that this method is inefficient for regular
         *      transmissions to the same destination, use the other version
         *      for such a purpose.
         *
         *      @param dnsname dns name of IP string of the remote host.
         *      @param port remote port that the data is to be sent to.
         *      @param data pointer to the data you wish to send
         *      @param datasize size in bytes of the data block pointed to by data
         *      @param packettype ID of the packet's type. This should not exceed the sendpackettypes value given uppon socket construction.
         *      @param deliveralways Wheter the reciever should drop the packet if it already recieved a newer packet. false == drop.
         *      @return the actual number of bytes that where sent. -1 indicates an error.
         */
        Int32 SendPacketTo( const GUCEF::CORE::CString& dnsname ,
                            UInt16 port                         ,
                            const void* data                    , 
                            UInt16 datasize                     ,
                            UInt16 packettype                   ,
                            bool deliveralways                  );

        /**
         *      Attempts to send a UDP packet to the given destination.
         *      Note that the data you send must fit within the payload
         *      space of a UDP packet.
         *
         *      @param dest Information about the packet destination.
         *      @param data pointer to the data you wish to send
         *      @param datasize size in bytes of the data block pointed to by data
         *      @param packettype ID of the packet's type. This should not exceed the sendpackettypes value given uppon socket construction.
         *      @param deliveralways Wheter the reciever should drop the packet if it already recieved a newer packet. false == drop.         
         *      @return the actual number of bytes that where sent. -1 indicates an error.
         */
        Int32 SendPacketTo( const TIPAddress& dest ,
                            const void* data       , 
                            UInt16 datasize        ,
                            UInt16 packettype      ,
                            bool deliveralways     );
        
        /**
         *      Attempts to resolve the given destination address and port
         *      information and stores the result in the dest struct.
         *      You should store the result for furture packet sends so
         *      that the dns ect. no longer has to be resolved with each
         *      packet send.
         *
         *      @param destaddrstr the destination address in string form that you wish to resolve
         *      @param destport the port on the remote host you wish to address
         *      @param dest structure that will hold the resolved version of the given data
         *      @return returns wheter the given data could be resolved.      
         */
        static bool ResolveDest( const GUCEF::CORE::CString& destaddrstr ,
                                 UInt16 destport                         ,  
                                 TIPAddress& dest                        );                    

        /**
         *      Attempts to resolve the source information into a more human-friendly
         *      version of the information. 
         *
         *      @param src the information about the data source in network format
         *      @param srcaddrstr output variable for the source IP/DNS
         *      @param srcport output variable for the source port                     
         */
        static bool ResolveSrc( const TIPAddress& src            ,
                                GUCEF::CORE::CString& srcaddrstr ,
                                UInt16& srcport                  );
        
        /**
         *      Registers the given event handlign interface for use
         *      with this socket. Events generated by this socket will
         *      be sent to the handler.
         *
         *      @param handler event handler interface
         */
        void RegisterEventHandler( CIPHUDPSocketEventHandler* handler );        

        protected:
        
        virtual void OnUDPSocketEvent( COMCORE::CUDPSocket& socket ,
                                       TUDPSocketEvent event       );
                        
        private:

        void OnPacketRecieved( void );

        void BufferPacketSendInfo( const void* data         ,
                                   const UInt16 datasize    ,
                                   const UInt16 packettype  ,
                                   const bool deliveralways );
        
        UInt32* _sendpcounters;
        UInt16 _sendpackettypes;
        UInt32* _rcvpcounters;
        UInt16 _rcvpackettypes;        
        CORE::CDynamicBuffer _buffer;
        COMCORE::CUDPSocket _sock;
        CIPHUDPSocketEventHandler* _handler;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_END
COM_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CPHUDPSOCKET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-07-2005 :
        - Moved to GUCEF::COM
        - Stripped 'deliver always' functionality
        - Stripped 'packet type' handling functionality
        - Renamed to CPHUDPSocket
- 28-05-2005 :
        - Designed and implemented this class.
          This class will take care of some basic packet management ontop of
          the standard UDP functionality. It can be used to send player movements
          ect. with ease.

-----------------------------------------------------------------------------*/
