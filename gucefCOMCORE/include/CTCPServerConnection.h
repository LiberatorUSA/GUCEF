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

#ifndef CTCPSERVERCONNECTION_H
#define CTCPSERVERCONNECTION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCORE_H
#include "gucefCORE.h"           /* gucefCORE library API */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"       /* build defines */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

#ifndef GUCEF_COMCORE_ETYPES_H
#include "gucefCOMCORE_ETypes.h"             /* simple types used */
#define GUCEF_COMCORE_ETYPES_H
#endif /* GUCEF_COMCORE_ETYPES_H ? */

#ifndef CSOCKET_H
#include "CSocket.h"            /* socket base class */
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
class GUCEF_COMCORE_EXPORT_CPP CTCPServerConnection : public CSocket
{
        public:

        /**
         *      Closes the connection to the client.
         *      This can cause this object to be deleted by the parent server.
         *      After a call to Close() the object should no longer be accessed
         *      by the user.
         */
        void Close( void );

        /**
         *      Allows you to read data from the socket when using a blocking
         *      socket approach. This member function always returns false for
         *      non-blocking sockets.         
         *      
         */
        bool Read( char *dest     , 
                   UInt32 size    , 
                   UInt32 &wbytes , 
                   Int32 timeout  );

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

        /**
         *	Constructor, init vars
         */
        CTCPServerConnection( CTCPServerSocket *tcp_serversock ,
                              UInt32 connection_idx            );

        /**
         *	Decontructor, close connection and cleanup.
         */
        virtual ~CTCPServerConnection();
        
        protected:
        friend class CTCPServerSocket;

        /**                 
         *      polls the socket ect. as needed and update stats.
         *
         *      @param tickcount the tick count when the Update process commenced.
         *      @param deltaticks ticks since the last Update process commenced.          
         */
        virtual void Update( void ); 
                             
        private:
        
        void CheckRecieveBuffer( void );
        
        private:
        friend class CTCPServerSocket;
        
        struct STCPServerConData* _data;
        bool _blocking;  
        bool _active;
        CORE::CDynamicBuffer m_readbuffer;
        CORE::CDynamicBuffer _sendbuffer;
        MT::CMutex _datalock;
        UInt32 m_maxreadbytes;
        UInt16 m_keepbytes;
        UInt32 m_connectionidx;      
        CTCPServerSocket* m_parentsock;

        CTCPServerConnection( void ); /* private default constructor because we need data */                             
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CTCPSERVERCONNECTION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-06-2004 :
        - Fixed a bug in the set Max_Read(). An old check that made sure the
          mr value was > 0 was still present causing a problem with the new
          max read protection and could potentionally cause data to 'drop'. 
- 20-01-2004 :
        - Added the set and get for Max_Read(). This addition is meant to
          increase the resistance of the server to data spamming.
- 13-01-2004 :
        - This class now overrides the CSocket's mutex lock and unlock member
          functions so we can also lock the server socket. This prevents a
          deadlock from occurring between the two.
- 11-01-2004 :
        - Modified code so that this object also closes the NET2 socket itself.
          All code for manipulating the lower level socket functions for the
          connection is now located in this class. The server socket should
          delete this object in a connection closing member function or a loop
          will occur causing access of a deleted object.
- 05-01-2004 :
        - Modified the class code so that it will now process events for this
          connection itself instead of letting the parent server socket do it.
          This means setting the threading method for this connection now has
          an effect as intended.
        - The socket will now close after an error occured.
- 23-09-2003 :
        - Added this section.
        - Added Wait_Untill_Read() in order to provide a method of operation
          similar to a blocking socket. Also added Read_Data() which will allow
          a blocking socket impementation to be used. You should only call these
          member functions from a thread other then the main application thread.

-----------------------------------------------------------------------------*/

