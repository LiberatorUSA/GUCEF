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
 *      Most mangled packets will be filtered out and dropped. This occurs if bytes
 *      seem to be lost or added to the packet. The packet checksum must also be 
 *      correct in relation to the packet data payload.
 *      It is possible, in theory, that a packet has the correct size and just happens
 *      to have a correct checksum/data combo while being in fact a mangled packet.
 *      This will only occur in very rare cases but keep it in mind.
 */
class GUCEFCOM_EXPORT_CPP CPHUDPSocket : public CORE::CObservingNotifier
{
        public:
        
    /*
     *      Typedef some shortcut types
     */
    typedef COMCORE::CUDPSocket::TIPAddress TIPAddress;         
    typedef COMCORE::CUDPSocket::TIPAddress TIPAddress;
        
    static const CORE::CEvent PHUDPSocketErrorEvent;
    static const CORE::CEvent PHUDPSocketClosedEvent;
    static const CORE::CEvent PHUDPSocketOpenedEvent;
    static const CORE::CEvent PHUDPPacketRecievedEvent;
    
    struct SPHUDPPacketRecievedEventData
    {
        COMCORE::CSocket::TIPAddress sourceAddress; /**< the source address of the data */
        CORE::TLinkedCloneableBuffer dataBuffer;    /**< the received packet data's payload */
        UInt32 packetType;                          /**< type of the packet */
        UInt32 packetNumber;                        /**< ordering number of this packet for it's type */
    };
    typedef CORE::CTCloneableObj< struct SPHUDPPacketRecievedEventData > PHUDPPacketRecievedEventData;
        
    /**
     *      Constructs a GU protocol UDP socket
     *      It adds some additional functionality on top of normal UDP
     *      packet handing.
     *
     *      @param minpayloadsize minimal data payload size you will send, used to initialize the buffer
     *      @param sendpackettypes number of different packet types you will send, you cannot send packets with type id's higher then this number
     *      @param rcvpackettypes number of different packet types you will receive, you cannot receive packets with type id's higher then this number
     */        
    CPHUDPSocket( UInt32 minpayloadsize = 100 ,
                  UInt16 sendpackettypes = 16 ,
                  UInt16 rcvpackettypes = 16  );
    
    /**
     *      Cleans up the allocated data and closes the socket
     */
    virtual ~CPHUDPSocket();
    
    /**
     *      Returns whether the socket is currently active
     *
     *      @return activity status of the socket
     */
    bool IsActive( void ) const;
    
    /**
     *      Attempts to open the UDP socket for use.
     *      The port used is left up to the operating system.
     *      This will typically be used for clients in a client-server
     *      architecture.
     */
    bool Open( void );
    
    /**
     *      Attempts to open the UDP socket for use on the given port.
     *      This will typically be used for a server in a client-server
     *      architecture or for an peer-to-peer architecture.
     *      Any local address can be used.
     *
     *      @param port port used for sending and/or receiving data
     */        
    bool Open( UInt16 port );
    
    /**
     *      Attempts to open the UDP socket for use on the given local
     *      address and port. Basically the same as Open( UInt16 port )
     *      but here you binding to a specific local address mandatory.
     *
     *      @param localaddr local address to be used for this socket
     *      @param port port used for sending and/or receiving data
     */  
    bool Open( const CORE::CString& localaddr ,
               UInt16 port                    );
               
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
     *      @param dnsname DNS name of IP string of the remote host.
     *      @param port remote port that the data is to be sent to.
     *      @param data pointer to the data you wish to send
     *      @param datasize size in bytes of the data block pointed to by data
     *      @param packettype ID of the packet's type. This should not exceed the sendpackettypes value given upon socket construction.
     *      @param deliveralways Whether the receiver should drop the packet if it already received a newer packet. false == drop.
     *      @return the actual number of bytes that where sent. -1 indicates an error.
     */
    Int32 SendPacketTo( const CORE::CString& dnsname ,
                        UInt16 port                  ,
                        const void* data             , 
                        UInt16 datasize              ,
                        UInt16 packettype            ,
                        bool deliveralways           );

    /**
     *      Attempts to send a UDP packet to the given destination.
     *      Note that the data you send must fit within the payload
     *      space of a UDP packet.
     *
     *      @param dest Information about the packet destination.
     *      @param data pointer to the data you wish to send
     *      @param datasize size in bytes of the data block pointed to by data
     *      @param packettype ID of the packet's type. This should not exceed the sendpackettypes value given upon socket construction.
     *      @param deliveralways Whether the receiver should drop the packet if it already received a newer packet. false == drop.         
     *      @return the actual number of bytes that where sent. -1 indicates an error.
     */
    Int32 SendPacketTo( const TIPAddress& dest ,
                        const void* data       , 
                        UInt16 datasize        ,
                        UInt16 packettype      ,
                        bool deliveralways     );
   
    static void RegisterEvents( void );

    protected:
    
    /**
     *  Event callback member function.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );
                    
    private:

    void OnPacketRecieved( const TIPAddress& sourceAddress                ,
                           const CORE::TLinkedCloneableBuffer& dataBuffer );

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

- 07-12-2006 :
        - Dinand: converted class to use the latest revision of the CORE module notification system
- 23-07-2005 :
        - Dinand: Moved to GUCEF::COM
        - Dinand: Stripped 'deliver always' functionality as a property of the packet type
        - Dinand: Stripped manditory 'packet type' handling functionality
        - Dinand: Renamed to CPHUDPSocket
- 28-05-2005 :
        - Dinand: Designed and implemented this class.
          This class will take care of some basic packet management ontop of
          the standard UDP functionality. It can be used to send player movements
          ect. with ease.

-----------------------------------------------------------------------------*/
