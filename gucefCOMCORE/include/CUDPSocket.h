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

#ifndef CUDPSOCKET_H
#define CUDPSOCKET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"       /* gucefMT mutex class */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef CDVSTRING_H
#include "CDVString.h"    /* gucefCORE platform string implementation */
#define CDVSTRING_H
#endif /* CDVSTRING_H ? */

#ifndef CSOCKET_H 
#include "CSocket.h"      /* base class for all TCP and UDP related socket classes */
#define CSOCKET_H
#endif /* CSOCKET_H ? */

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

/*
 *     Forward declaration of the event handling interface class
 */
class CIUDPSocketEventHandler;

/*-------------------------------------------------------------------------*/

/**
 *      UDP Socket class that wraps the O/S UDP socket functionality.
 *      This class can act as a blocking or non-blocking UDP socket.
 *      In both cases a PACKET_RECIEVED event will be sent when a packet
 *      is queued.
 */
class EXPORT_CPP CUDPSocket : public CSocket
{
        public:
        struct SUDPSockData;            /**< forward declaration of platform data storage structure */

        typedef enum UDPSocketEvent 
        {
                UDP_SOCKET_ERROR = 0 ,
                UDP_SOCKET_CLOSED    ,
                UDP_SOCKET_OPENED    ,
                UDP_PACKET_RECIEVED   
        } TUDPSocketEvent;                           

        /**
         *      Creates a UDP socket object initialized to be either blocking
         *      or non-blocking. Choosing either type is a design decision.
         *
         *      @param blocking wheter to construct the socket as a blocking socket
         */        
        CUDPSocket( bool blocking );
        
        /**
         *      Cleans up the allocated data and closes the socket
         */
        ~CUDPSocket();
        
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
         *      @param dnsname dns name of IP string of the remote host.
         *      @param port remote port that the data is to be sent to.
         *      @param data pointer to the data you wish to send
         *      @param datasize size in bytes of the data block pointed to by data
         *      @return the actual number of bytes that where sent. -1 indicates an error.
         */
        Int32 SendPacketTo( const CORE::CString& dnsname ,
                            UInt16 port                  ,
                            const void* data             , 
                            UInt16 datasize              );
                                    
        /**
         *      Attempts to send a UDP packet to the given destination.
         *      Note that the data you send must fit within the payload
         *      space of a UDP packet.
         *
         *      @param dest Information about the packet destination.
         *      @param data pointer to the data you wish to send
         *      @param datasize size in bytes of the data block pointed to by data
         *      @return the actual number of bytes that where sent. -1 indicates an error.
         */
        Int32 SendPacketTo( const TIPAddress& dest ,
                            const void* data       , 
                            UInt16 datasize        );
        
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
        static bool ResolveDest( const CORE::CString& destaddrstr ,
                                 UInt16 destport                  ,  
                                 TIPAddress& dest                 );

        /**
         *      For blocking sockets this call will not return untill data is
         *      recieved. For non-blocking sockets this call returns imediatly
         *      if there is no data and returns an error value.
         *
         *      Recieved data is written into destbuf up to bufsize bytes.
         *      If the destination buffer is not large enough to hold the entire
         *      packet data then the remaining data will be lost !!!
         *
         *      If there are more packets queued after a successfull read then
         *      a new PACKET_RECIEVED event will be sent. This cycle repeats untill 
         *      there are no more packets in the queue.
         *
         *      @param src structure that will hold the information about the source of the data
         *      @param destbuf buffer the recieved data will be written to.
         *      @param bufsize size of the destination buffer in bytes.
         *      @return the number of bytes written to the given buffer, returns -1 on error.
         */
        Int32 Recieve( TIPAddress& src ,
                       void* destbuf   , 
                       UInt16 bufsize  );
                       
        /**
         *      Same as the other version of Recieve() except here
         *      you do not have to pass in a structure for source info.
         *      Only use this version if you don't care where the packet 
         *      came from
         *
         *      @param destbuf buffer the recieved data will be written to.
         *      @param bufsize size of the destination buffer in bytes.         
         */                       
        Int32 Recieve( void* destbuf      , 
                       UInt16 bufsize     );                       

        /**
         *      Attempts to resolve the source information into a more human-friendly
         *      version of the information. 
         *
         *      @param src the information about the data source in network format
         *      @param srcaddrstr output variable for the source IP/DNS
         *      @param srcport output variable for the source port                     
         */
        static bool ResolveSrc( const TIPAddress& src     ,
                                CORE::CString& srcaddrstr ,
                                UInt16& srcport           );
                                 
        /**
         *      returns wheter this socket is a blocking socket
         *
         *      @return wheter this socket is a blocking socket
         */                                 
        bool IsBlocking( void ) const;
        
        /**
         *      Registers the given event handlign interface for use
         *      with this socket. Events generated by this socket will
         *      be sent to the handler.
         *
         *      @param handler event handler interface
         */
        void RegisterEventHandler( CIUDPSocketEventHandler* handler );
        
        /**
         *      Unregisters the given event handling interface from
         *      usage with the socket instance. It will no longer receive 
         *      any events form the socket.
         *
         *      @param handler event handler interface
         */
        void UnregisterEventHandler( CIUDPSocketEventHandler* handler );
        
        protected:
        
        /**
         *      Polls the socket if the polling flag is set this update cycle
         *      If any data is known to be queued after a poll then the
         *      PACKET_RECIEVED event will be sent. 
         */
        virtual void Update( UInt32 tickcount  ,
                             UInt32 deltaticks );
        
        private:
        
        void CheckForData( void );                     /**< checks the socket for queued incoming data */
        void SendEvent( TUDPSocketEvent event );       /**< simply dispatch the event to all handlers */
        
        CORE::CDynamicArray _ifaces;    /**< event handler interfaces */
        bool _blocking;                 /**< is this socket blocking ? */
        bool _checkfordata;             /**< check for data and dispatch event next update cycle ? */
        struct SUDPSockData* _data;     /**< container for platform specific data */
        UInt16 m_port;
        MT::CMutex _datalock;           /**< mutex for thread-safety when manipulating the socket */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CUDPSOCKET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-05-2005 :
        - Initial version of the new implementation using winsock directly,
          got rid of all the SDL crap.

-----------------------------------------------------------------------------*/
