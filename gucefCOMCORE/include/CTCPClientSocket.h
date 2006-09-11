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

#ifndef CTCPCLIENTSOCKET_H
#define CTCPCLIENTSOCKET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CSOCKET_H
#include "CSocket.h"                   /* socket Base class */
#define CSOCKET_H
#endif /* CSOCKET_H */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"  /* GUCEF COMCORE specific macros and build defs */
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

/*
 *      forward declaration of interface base class
 */
class CTCPClientSocketInterface;

/*-------------------------------------------------------------------------*/

/**
 *      TCP Client socket class
 */
class EXPORT_CPP CTCPClientSocket : public CSocket
{
        public:
        
        CTCPClientSocket( bool blocking );
        
        virtual ~CTCPClientSocket();

        /**
         *     
         */
        bool ConnectTo( const CORE::CString& address , 
                        UInt16 port                  ); 

        /**
         *      Attempts to reconnect to the server provided with
         *      Connect_To(). If Connect_To() has not yet been called then this
         *      member function has no effect.
         */
        bool Reconnect( void );
        
        void Close( void );                                  /* close the socket connection */

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

        #ifdef NETWORK_ICMP_SUPPORT
        /**
         *      This will allow you to ping a server.
         *      active sets wheter or not we should ping.
         *      pings is the number of ping to be performed. If this is smaller
         *      then 0 a single ping will be performed with the caller thread
         *      returning when the ping has been completed. In all other cases
         *      a pinging task will be spawned after which this member function
         *      will return. A value of 0 for ping equals infinite.
         *      maxhops is the maximum number of hops to try before we abort the
         *      ping.
         */
        void Ping( bool active    ,
                   Int32 pings    ,
                   UInt32 maxhops );
        #endif /* NETWORK_ICMP_SUPPORT ? */

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
        
        void SetInterface( CTCPClientSocketInterface* iface );
        
        CTCPClientSocketInterface* GetInterface( void ) const;

        public:
        
        struct STCPClientSockData;
        
        protected:
        
        /** 
         *      polls the socket ect. as needed and update stats.
         *
         *      @param tickcount the tick count when the Update process commenced.
         *      @param deltaticks ticks since the last Update process commenced.          
         */
        virtual void Update( UInt32 tickcount  ,
                             UInt32 deltaticks );        
        
        
        private:
        
        void CheckRecieveBuffer( void );               
        
        CORE::CString _remoteaddr;
        UInt16 _remoteport;        
        struct STCPClientSockData* _data;
        bool _blocking;
        bool _active;
        MT::CMutex datalock;
        CTCPClientSocketInterface* m_iface;
        CORE::CDynamicBuffer m_readbuffer;
        UInt32 m_maxreadbytes;
        UInt32 m_keepbytes;                      

//        UInt32 maxrbytes;                      /* max number of bytes to recieve before proccessing it */
//        UInt32 keepbytes;                      /* the number of bytes that should be kept untill next proccessing cycle */

        CTCPClientSocket( void );                /* default constructor cannot be used since we need to register the socket */
        CTCPClientSocket& operator=( const CTCPClientSocket& src ); /* making a copy of a socket doesnt make sense */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CTCPCLIENTSOCKET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-03-2005 :
        - Initial version of the new implementation using winsock directly,
          got rid of all the SDL crap.
- 19-04-2004 :
        - The Ping() member function will from now on only be available if
          NETWORK_ICMP_SUPPORT is defined. 
- 17-04-2004 :
        - Added Send_Data_And_Wait(). This member function helps get arround the
          problems that occur when using a seperate wait fuction since then a
          response could be recieved before the wait is reached.
- 05-12-2003 :
        - Changed behaviour. If a socket error occures the socket will be closed.
- 20-12-2003 :
        - Fixed a small bug when resizing buffer which caused a '\0' to replace
          some characters.
- 06-12-2003 :
        - Changed code in the Handle_Recieved() member function. Allocation of
          storage now happens less because the buffer is recycled between
          transmissions. This meant the addtion of 2 private data members. 
- 30-10-2003 :
        - Changed contructor to use the new self registering system with the com
          manager.
- 23-09-2003 :
        - Added Wait_Untill_Read() in order to provide a method of operation
          similar to a blocking socket. Also added Read_Data() which will allow
          a blocking socket impementation to be used. You should only call these
          member functions from a thread other then the main application thread.
- 05-08-2003 :
        - Changed SDL_Event_Handler into Handle_SDL_Event because of the new
          threading implementation and moved it to protected.
- 13-02-2003 :
        - Added this section.

-----------------------------------------------------------------------------*/
