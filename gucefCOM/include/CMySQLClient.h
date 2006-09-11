/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef CMYSQLCLIENT_H
#define CMYSQLCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"             /* mutex used for threadsafety */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef CTCPCLIENTSOCKETINTERFACE_H
#include "CTCPClientSocketInterface.h"	/* base class for the MySQL client */
#define CTCPCLIENTSOCKETINTERFACE_H
#endif /* CTCPCLIENTSOCKETINTERFACE_H */

#ifndef CDBQUERY_H
#include "CDBQuery.h"           /* query class used to store query results */
#define CDBQUERY_H
#endif /* CDBQUERY_H */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"
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
 *      Forward declaration of an event handling interface class for the
 *      CMySQLClient class.
 */
class CMySQLClientInterface;

/*--------------------------------------------------------------------------*/

/**
 *      Easy to use MySQL client built ontop of a TCP client socket interface.
 *      The client is threadsafe, but it should be noted that the client should
 *      only be accessed from created threads OR from the main application
 *      thread. Otherwise a problem could arrise out of the fact that the
 *      main application thread does the network event proccessing.
 */
class EXPORT_CPP CMySQLClient: public COMCORE::CTCPClientSocketInterface
{
	public:
	typedef COMCORE::CSocket::TSocketError TSocketError;
	
        /**
         *	Set of MySQL client connection states
         */
        typedef enum
        {
                MYSQL_CONNECTION_FAILED ,
                MYSQL_CONNECTED         ,
                MYSQL_CONNECTING        ,
                MYSQL_DISCONNECTED
        } TMySQLCCState;	

        /**
         *      Attempt to connect to a MySQL server which is specified by ip_address
         *      and port using the username and password provided to connected to the
         *      given database on the given TCP client_socket.
         */
        bool ConnectTo( const GUCEF::CORE::CString& address                    ,
                        UInt16 port = 3306                                     ,
                        const GUCEF::CORE::CString& username = "Administrator" ,
                        const GUCEF::CORE::CString& password = ""              ,
                        const GUCEF::CORE::CString& database = NULL            );

        /**
         *      Disconnect from server.
         */
        void Disconnect( void );

        /**
         *      Returns a enum value indicating the current connection state of
         *      the MySQL client.
         */
        TMySQLCCState Connection_State( void ) const  { return mysql_connection_state; }

        /**
         *      The following member function returns the last MySQL error
         *      in numerical form.
         */
        UInt16 MySQL_Error_Number( void ) const { return mysql_error_number; }

        /**
         *      The following member function returns the last MySQL error
         *      in more human readable string form. Note that the returned
         *      value can also be NULL if no error occured.
         */
        CORE::CString GetError( void ) const;

        /**
         *      Resets the MySQL error string to NULL.
         */
        void Clear_MySQL_Error( void ) { mysql_error = NULL; }

        /**
         *      Send a MySQL query to the server. The resulting data will be
         *      written into cdbquery when it is recieved. Note that if you need
         *      the data after calling a query instead of delta T later then
         *      you should use Query_And_Wait(). This member function will return
         *      false if another query is already active or if there was a problem
         *      transmitting the query.
         */
        bool Query( CDBQuery &cdbquery                   ,
                    const GUCEF::CORE::CString& sqlquery );

        /**
         *      Send a MySQL query to the server. The resulting data will be
         *      written into cdbquery when it is recieved. Note that this member
         *      function will not return untill data is recieved from the server
         *      or the given timeout is reached.
         *      Note that the primary application thread does the
         *      initial network messaging so don't call this member function
         *      from the main application thread !!!. if timeout < 0 then no
         *      timeout will be used.         
         */
        bool QueryAndWait( CDBQuery &cdbquery          ,
                             const GUCEF::CORE::CString& ,
                             Int32 timeout               );

        /**
         *      Returns wheter there currently is an active query.
         */
        bool Query_Busy( void ) const;

        /**
         *      Returns the currently linked interface.
         */
        CMySQLClientInterface* GetInterface( void ) const;

        /**
         *      Sets the new event handling interface.
         *      Note that the interface can only be set if there is no active
         *      query. 
         */
        bool SetInterface( CMySQLClientInterface *new_iface );

        /**
         *      Default constructor, we just init some vars
         */
        CMySQLClient( bool blocking );

        /**
         *      Destructor, cleanup any allocated storage space
         */
        virtual ~CMySQLClient( void );
        
	private:
	
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
                              TSocketError *errorcode           );        

        /**
         *	Handler for data we recieve on the TCP client socket 
         */
        virtual void OnRead( COMCORE::CTCPClientSocket &clientsocket ,
                             const char *data                        ,
                             UInt32 length                           ,
                             UInt32 &keepbytes                       );	
	
        CORE::CString mysql_server_version;
        CORE::CString mysql_ssl_salt;
        CORE::CString mysql_server_ip_address;
        CORE::CString mysql_username;
        CORE::CString mysql_password;
        CORE::CString mysql_database;
        CORE::CString mysql_error;
        UInt8 mysql_protocol_version, mysql_charset;
        UInt16 mysql_server_capabilities, mysql_status, mysql_client_capabilities, mysql_error_number;
        UInt32 mysql_thread_id, incoming_length, mysql_server_port;
        TMySQLCCState mysql_connection_state;
        CDBQuery *query;
        MT::CMutex lock;
        COMCORE::CTCPClientSocket socket;
        CMySQLClientInterface *iface;
        bool query_busy;

        CMySQLClient(); /* we need to now whether to use blocking  */
        CMySQLClient& operator=( const CMySQLClient &src ); /* this operator makes no sense for this class */
        bool Login( const char *data, UInt32 length );        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*--------------------------------------------------------------------------*/

#endif /* CMYSQLCLIENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-07-2005 :
        - Made some fixes that allow the client to work with the new socket classes.
        - Got rid of the all the char* strings, replaced with string objects
- 11-06-2004 :
        - Modified the constructor and Connect_To() member functions and code
          in such a way that the client socket is now part of the client instead
          of something external that is given to the MySQL client.
        - Added support for interface classes. Now certain socket events and
          MySQL client specific events will be available trough the interface.
        - Made the client threadsafe. Particularly usefull in combination with
          threaded sockets giving each socket MySQL access but with exclusive
          access during the query.
- 17-04-2004 :
        - Fixed several strncpy() bugs that caused the use of strings without
          an \0.
        - Got rid of unnessesary if statements in Connect_To() and Login().
        - Added a NULL parameter check in Connect_To().
        - All kinds of console output will now be removed in a non-debug build.
        - changed CDBQuery parameter into a refrence.
        - Added Query_And_Wait().
- 14-10-2003 :
        - Started integrating this class into the DVNETCOM codebase.
          This class was contributed to the codebase by Logan, many thanx to him
          for this usefull MySQL client.
        - changed layout to conform with my standard.
        - Constructor now inits client state to diconnected used to be garbage
          value.
        - Added the const keyword in several places.
        - Added comments to the member functions and other code.
        - Made an enum type TMySQLCCState instead of just an enum to indicate
          connection status.
        - Socket active state now gets checked in Disconnect()

---------------------------------------------------------------------------*/
