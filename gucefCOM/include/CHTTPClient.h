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

#ifndef CHTTPCLIENT_H
#define CHTTPCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"	     /* TCP Client socket */
#define CTCPCLIENTSOCKET_H
#endif /* CTCPCLIENTSOCKET_H ? */

#ifndef CTCPCLIENTSOCKETINTERFACE_H
#include "CTCPClientSocketInterface.h" /* Interface for TCP Client Socket */
#define CTCPCLIENTSOCKETINTERFACE_H
#endif /* CTCPCLIENTSOCKETINTERFACE_H ? */

#ifndef CDVSTRING_H
#include "CDVString.h"
#define CDVSTRING_H
#endif /* CDVSTRING_H ? */

#ifndef CVALUELIST_H
#include "CValueList.h"
#define CVALUELIST_H
#endif /* CVALUELIST_H ? */

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
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Enumeration type with HTTP client and server error codes.
 */
typedef enum THTTPCODE
{
        /* Initialization value */
        HTTPCODE_DEFAULT = 0,

        /* HTTP Information Codes */
        HTTPCODE_100_CONTINUE = 100,
        HTTPCODE_101_SWITCHING_PROTOCOLS,

        /* HTTP Success Codes */
        HTTPCODE_200_OK = 200,
        HTTPCODE_201_CREATED,
        HTTPCODE_202_ACCEPTED,
        HTTPCODE_203_NON_AUTHORITATIVE_INFORMATION,
        HTTPCODE_204_NO_CONTENT,
        HTTPCODE_205_RESET_CONTENT,
        HTTPCODE_206_PARTIAL_CONTENT,

        /* HTTP Redirection Codes */
        HTTPCODE_300_MULTIPLE_CHOICES = 300,
        HTTPCODE_301_MOVED_PERMANENTLY,
        HTTPCODE_302_FOUND,
        HTTPCODE_303_SEE_OTHER,
        HTTPCODE_304_NOT_MODIFIED,
        HTTPCODE_305_USE_PROXY,
        HTTPCODE_307_TEMPORARY_REDIRECT,

        /* HTTP Client Error codes */
        HTTPCODE_400_BAD_REQUEST = 400,
        HTTPCODE_401_UNAUTHORIZED,
        HTTPCODE_402_PAYMENT_REQUIRED,
        HTTPCODE_403_FORBIDDEN,
        HTTPCODE_404_NOT_FOUND,
        HTTPCODE_405_METHOD_NOT_ALLOWED,
        HTTPCODE_406_NOT_ACCEPTABLE,
        HTTPCODE_407_PROXY_AUTHENTICATION_REQUIRED,
        HTTPCODE_408_REQUEST_TIMEOUT,
        HTTPCODE_409_CONFLICT,
        HTTPCODE_410_GONE,
        HTTPCODE_411_LENGTH_REQUIRED,
        HTTPCODE_412_PRECONDITION_FAILED,
        HTTPCODE_413_REQUEST_ENTITY_TOO_LARGE,
        HTTPCODE_414_REQUEST_URI_TOO_LARGE,
        HTTPCODE_415_UNSUPPORTED_MEDIA_TYPE,
        HTTPCODE_416_REQUESTED_RANGE_NOT_SATISFIABLE,
        HTTPCODE_417_EXPECTATION_FAILED,

        /* HTTP Server Error codes */
        HTTPCODE_500_INTERNAL_SERVER_ERROR = 500,
        HTTPCODE_501_NOT_IMPLEMENTED,
        HTTPCODE_502_BAD_GATEWAY,
        HTTPCODE_503_SERVICE_UNAVAILABLE,
        HTTPCODE_504_GATEWAY_TIMEOUT,
        HTTPCODE_505_HTTP_VERSION_NOT_SUPPORTED
} THTTPCODE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Forward declaration of this client's interface class
 */
class CHTTPClientInterface;

/*-------------------------------------------------------------------------*/

/**
 *      file downloading client utilising the HTTP protocol.
 */
class GUCEFCOM_EXPORT_CPP CHTTPClient : public COMCORE::CTCPClientSocketInterface
{
	public:

        CHTTPClient( void );                
        
        virtual ~CHTTPClient();
        
        bool Post( const CORE::CString& host                ,
                   UInt16 port                              ,
                   const CORE::CString& path                , 
                   const CORE::CValueList* valuelist = NULL );
                   
        bool Post( const CORE::CString& urlstring           ,
                   const CORE::CValueList* valuelist = NULL );                   
                   
        bool Get( const CORE::CString& host                ,
                  UInt16 port                              ,
                  const CORE::CString& path                ,
                  const UInt32 byteoffset = 0              ,
                  const CORE::CValueList* valuelist = NULL );

        bool Get( const CORE::CString& urlstring           ,
                  const UInt32 byteoffset = 0              ,
                  const CORE::CValueList* valuelist = NULL );
                  
        void Close( void );                  
                  
        UInt32 GetBytesRecieved( void ) const;
        
	void SetInterface( CHTTPClientInterface* iface );
	
	CHTTPClientInterface* GetInterface( void ) const;
	
	protected:
	
        /**
         *      Event handler that is called when we recieve data from the
         *      server. Initial processing will be done by this server.
         */
        virtual void OnRead( COMCORE::CTCPClientSocket &socket ,
                             const char *data              ,
                             UInt32 length                 ,
                             UInt32 &keepbytes             );

        virtual void OnConnecting( COMCORE::CTCPClientSocket& socket );

        virtual void OnConnected( COMCORE::CTCPClientSocket& socket );

        virtual void OnDisconnect( COMCORE::CTCPClientSocket& socket );
        
        virtual void OnWrite( COMCORE::CTCPClientSocket &socket ,
                              const void* data                  ,
                              UInt32 length                     );        	
	
        private:
        
        bool ParseURL( const CORE::CString& urlstring ,
                       CORE::CString& host            ,
                       UInt16& port                   ,
                       CORE::CString& path            );
        
        CHTTPClient( const CHTTPClient& src );
        
        CHTTPClient& operator=( const CHTTPClient& src );
        
        private:
        
        COMCORE::CTCPClientSocket m_socket;
        CHTTPClientInterface* m_iface;
        bool m_downloading;
        UInt32 m_recieved;
        UInt32 m_filesize;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CHTTPCLIENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-09-2005 :
        - Completed switch to a Post/Get based system
- 09-08-2005 :
        - Class renamed to CHTTPClient
        - Interface and code modified, only loosly based on the previous version
          called CHTTPDownload.
- 31-05-2005 :
        - Commenced integration into new GUCEF codebase
- 19-04-2004 :
        - If a file is not found (404) error there was no way to detect it.
          Thus i changed the OnError() event handler to include an error code.
          The error code given is one of the new enum values.
        - Got rid of the use of atoi() because it's not always available.
        - Added external include guards.
        - Added HTTP_Code() member function.
- 19-12-2003 :
        - Changed handling of OnDisconnect event for the socket.
          This event generated by the socket will now always cause the
          OnDisconnect to be called of the interface. This may or may not be
          preceded with an OnFinish event.
        - Fixed a bug in errorcode debug output.
        - Replaced printf() debugging output with the tsprintf() version.
- 05-11-2003 :
        - Created overloaded versions of the Download() member function.
          In order to accomplish this some fuctions call other overloaded
          versions. Only accepting a complete URL is a bit of a drag if all you
          want to change is the file to download in an entire batch of downloads
        - Made several inlined get functions const
        - Removed some unneeded return values.
        - Removed several data members which I replaced with a local variable
          in the relevant member functions.
- 02-11-2003 :
        - Recieved initial version of this class from Logan Benjamin. Many
          thanks to him for contributing this class to the DVNETCOM codebase.
        - Modified the file layout to conform with my latest standard.

---------------------------------------------------------------------------*/
