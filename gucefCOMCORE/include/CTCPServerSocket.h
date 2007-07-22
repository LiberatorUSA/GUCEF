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

/**
 *      Server socket class for TCP 
 */
class GUCEF_COMCORE_EXPORT_CPP CTCPServerSocket : public CSocket
{
    public:

    struct SConnectionInfo
    {
        CHostAddress hostAddress;
        CTCPServerConnection* connection;
        UInt32 connectionIndex;
    };
    typedef struct SConnectionInfo TConnectionInfo;
    
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
    typedef CORE::CTCloneableObj< TConnectionInfo > TClientConnectedEventData;

    static void RegisterEvents( void );
    
    public:
    
    CTCPServerSocket( bool blocking );

    /*
     *  Obtain pointer to connection data by means of connection index
     */
    CTCPServerConnection* GetConnection( UInt32 index );

    UInt32 GetActiveCount( void ) const;

    /*
     *      Methods that activate or de-activate the socket
     */
    bool ListenOnPort( UInt16 port );   /* listen on default NIC with port given */
    
    void Close( void ); /* stop listening for clients, close server socket */
    
    UInt16 GetPort( void ) const;
    
    bool IsActive( void ) const;
    
    /**
     *      Returns whether the socket is blocking or non-blocking
     */
    bool IsBlocking( void ) const;
    
    UInt32 GetMaxConnections( void ) const;
    
    void GetListenAddress( CHostAddress& listenAddress ) const;
    
    /**                 
     *      polls the socket etc. as needed and update stats.        
     */
    virtual void Update( void );           

    /*
     *      Constructor and Destructor
     */
    virtual ~CTCPServerSocket();   
    
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
        - Fixed a potential deadlock between the server socket and a connection
          object which could happen on rare occasions.
        - Now uses the CDynamicArray class instead of managing it's own heap.  
- 05-01-2004 :
        - Modified server code so that each client connection can now have
          it's own threading method as designed. This means that several
          handler member functions where moved to the CTCPServerConnection
          class.
        - The server socket will now close if an error occurred.
        - Added member functions to control the threading of client connections
              - ClientCon_Threading_Method()   get+set
              - ClientCon_Default_Threading()  get+set
- 12-11-2003 :
        - Modified the class so that you no longer set a max number of client
          connections but instead the heap gets automatically resized. There is
          a define in the .cpp which define the resize amount. This change will
          make the sockets easier to use.
        - CTCPServerConnection objects are now created when there is a connection
          and not on init of the server socket.
- 30-10-2003 :
        - Changed constructor to use the new self registering system with the com
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


