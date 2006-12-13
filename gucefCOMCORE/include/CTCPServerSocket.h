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

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CDynamicArray.h"		/* resizable array implementation */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"       /* build defines */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#include "CSocket.h"                    /* socket base class */
#include "CTCPServerConnection.h"       /* utils class for 1 client connection */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Forward declarations of types used
 */
struct STCPServerSockData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Forward declaration of interface base class
 */
class CTCPServerSocketInterface;

/*-------------------------------------------------------------------------*/

/**
 *      Server socket class for TCP 
 */
class GUCEF_COMCORE_EXPORT_CPP CTCPServerSocket : public CSocket
{
    public:
    
    static const CORE::CEvent ClientDataRecievedEvent;
    static const CORE::CEvent ClientDataSentEvent;
    static const CORE::CEvent ClientConnectedEvent;
    static const CORE::CEvent ClientDisconnectedEvent;
    static const CORE::CEvent ClientErrorEvent;        
    static const CORE::CEvent ServerSocketOpenedEvent;
    static const CORE::CEvent ServerSocketClosedEvent;
    static const CORE::CEvent ServerSocketErrorEvent;
    static const CORE::CEvent ServerSocketClientErrorEvent;
    static const CORE::CEvent ServerSocketMaxConnectionsChangedEvent;
        
    typedef CORE::TCloneableInt32   TServerSocketErrorEventData;
    typedef CORE::TCloneableInt32   TServerSocketClientErrorEventData;
    
    public:
    
    CTCPServerSocket( bool blocking );

    /*
 *	Obtain pointer to connection data by means of connection index
     */
    CTCPServerConnection* GetConnection( UInt32 index );

    UInt32 GetActiveCount( void ) const;
    
    void SetInterface( CTCPServerSocketInterface *new_iface );
    
    CTCPServerSocketInterface* GetInterface( void ) const;

    /*
     *      Methods that activate or de-activate the socket
     */
    bool ListenOnPort( UInt16 port );   /* listen on default nic with port given */
    
    void Close( void ); /* stop listening for clients, close server socket */
    
    UInt16 GetPort( void ) const;
    
    bool IsActive( void ) const;
    
    /**
     *      Returns whether the socket is blocking or non-blocking
     */
    bool IsBlocking( void ) const;
    
    UInt32 GetMaxConnections( void ) const;
    
    void SetClientThreading( bool thread );
    
    bool GetClientThreading( void ) const;
    
    /**                 
     *      polls the socket etc. as needed and update stats.
     *
     *      @param tickcount the tick count when the Update process commenced.
     *      @param deltaticks ticks since the last Update process commenced.          
     */
    virtual void Update( UInt32 tickcount  ,
                         UInt32 deltaticks );           

    /*
     *      Constructor and Destructor
     */
    virtual ~CTCPServerSocket();

    static void RegisterEvents( void );
    
    private:
    friend class CTCPServerConnection;
    
    void OnClientRead( CTCPServerConnection* connection ,
                       const UInt32 connectionid        ,
                       const char* data                 ,
                       const UInt16 recieved            ,
                       UInt16& keepbytes                );

    void OnClientConnectionClosed( CTCPServerConnection* connection ,
                                   const UInt32 connectionid        ,
                                   bool closedbyclient              );
    
    private :
    struct STCPServerSockData* _data;
    bool _active; 
    bool _blocking;               

    /**
     *	default constructor cannot be used, we need to know 
     *      whether this socket is blocking.
     */
    CTCPServerSocket( void );     
    
    void AcceptClients( void );           
    
    CORE::CDynamicArray _connections;          /**< array of connection objects */
    UInt16 m_port;
    MT::CMutex _datalock;
    UInt32 _timeout;   
    UInt32 _acount;                            /**< the number of active connections */        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CTCPSERVERSOCKET_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-03-2005 :
        - Continuation of conversion to direct winsock instead of sdl_net
- 13-01-2004 :
        - Fixed a potention deadlock between the serversocket and a connection
          object which could happen on rare occasions.
        - Now uses the CDynamicArray class intead of managing it's own heap.  
- 05-01-2004 :
        - Modified server code so that each client connection can now have
          it's own threading method as designed. This means that several
          handler member functions where moved to the CTCPServerConnection
          class.
        - The server socket will now close if an error occured.
        - Added member functions to control the threading of client connections
              - ClientCon_Threading_Method()   get+set
              - ClientCon_Default_Threading()  get+set
- 12-11-2003 :
        - Modified the class so that you no longer set a max number of client
          connections but instead the heap gets automaticly resized. There is
          a define in the .cpp which define the resize amount. This change will
          make the sockets easyer to use.
        - CTCPServerConnection objects are now created when there is a connection
          and not on init of the serversocket.
- 30-10-2003 :
        - Changed contructor to use the new self registering system with the com
          manager.
- 23-09-2003 :
        - renamed Set_Max_Connections() to Max_Connections()
- 11-08-2003 :
        - Added mutex locks to Active_Connections().
- 05-08-2003 :
	- Added this section.
        - Changed SDL_Event_Handler into Handle_SDL_Event because of the new
          threading implementation and moved it to protected.
	- Added const keyword here and there.

-----------------------------------------------------------------------------*/


