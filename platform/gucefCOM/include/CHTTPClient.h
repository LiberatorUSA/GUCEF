/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

#ifndef GUCEF_COM_CHTTPCLIENT_H
#define GUCEF_COM_CHTTPCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"	     /* TCP Client socket */
#define CTCPCLIENTSOCKET_H
#endif /* CTCPCLIENTSOCKET_H ? */

#ifndef CDVSTRING_H
#include "CDVString.h"
#define CDVSTRING_H
#endif /* CDVSTRING_H ? */

#ifndef CVALUELIST_H
#include "CValueList.h"
#define CVALUELIST_H
#endif /* CVALUELIST_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

#ifndef GUCEF_COM_HTTPCODE_H
#include "gucefCOM_HTTPCode.h"
#define GUCEF_COM_HTTPCODE_H
#endif /* GUCEF_COM_HTTPCODE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Simplistic HTTP protocol client for basic tasks
 */
class GUCEF_COM_EXPORT_CPP CHTTPClient : public CORE::CObservingNotifier
{
	public:

    static const CORE::CEvent ConnectingEvent;
    static const CORE::CEvent ConnectedEvent;
    static const CORE::CEvent DisconnectedEvent;
    static const CORE::CEvent ConnectionErrorEvent;        
    static const CORE::CEvent HTTPErrorEvent;
    static const CORE::CEvent HTTPRedirectEvent;
    static const CORE::CEvent HTTPContentEvent;                
    static const CORE::CEvent HTTPDataRecievedEvent;
    static const CORE::CEvent HTTPDataSentEvent;        
    static const CORE::CEvent HTTPTransferFinishedEvent;

	static void RegisterEvents( void );
	
	public:

    struct SHTTPContentEventData   /**< stores data about the HTTP transmission payload */
    {
        UInt32 contentSize;        /**< the size of the content in the HTTP payload in bytes */
        bool resumeSupported;      /**< does the remote host support resume ? */
        THTTPCODE HTTPcode;        /**< HTTP code for the given content */
    };
    typedef CORE::CTCloneableObj< struct SHTTPContentEventData >  THTTPContentEventData;      /**< see struct SHTTPContentEventData for details */
    typedef CORE::TLinkedCloneableBuffer                          THTTPDataRecievedEventData; /**< contains buffer with the recieved data */
    typedef CORE::TLinkedCloneableBuffer                          THTTPDataSendEventData;     /**< contains buffer with the dispatched data */    
    typedef CORE::TCloneableString                                THTTPRedirectEventData;     /**< contains the URL to which the client is beeing redirected */
    typedef CORE::CTCloneableObj< THTTPCODE >                     THTTPErrorEventData;        /**< contains the relevant HTTP error code */
    typedef COMCORE::CTCPClientSocket::TSocketErrorEventData      TConnectionErrorEventData;  /**< socket error code */

	public:

    CHTTPClient( void );                
    
    CHTTPClient( CORE::CPulseGenerator& pulseGenerator );
    
    virtual ~CHTTPClient();
    
    /**
     *  Starts a POST operation.
     *  @param valuelistAsContent Key-Value storage to use for generating the payload
     */
    bool Post( const CORE::CString& host                               ,
               UInt16 port                                             ,
               const CORE::CString& path                               , 
               const CORE::CValueList* valuelistAsContent = GUCEF_NULL ,
               const CORE::CString& contentType = "text/plain"         );
               
    /**
     *  Starts a POST operation.
     *  @param valuelistAsContent Key-Value storage to use for generating the payload
     */
    bool Post( const CORE::CString& urlstring                          ,
               const CORE::CValueList* valuelistAsContent = GUCEF_NULL ,
               const CORE::CString& contentType = "text/plain"         );                   
      
    /**
     *  Starts a POST operation.
     *  @param payload binary storage representing the content
     */
    bool Post( const CORE::CString& urlstring      ,
               const CORE::CString& contentType    ,
               const CORE::CDynamicBuffer& payload ); 

    /**
     *  Starts a POST operation.
     *  @param payload binary storage representing the content
     */
    bool Post( const CORE::CString& host           ,
               UInt16 port                         ,
               const CORE::CString& path           ,
               const CORE::CString& contentType    ,
               const CORE::CDynamicBuffer& payload );

    bool Get( const CORE::CString& host                ,
              UInt16 port                              ,
              const CORE::CString& path                ,
              const UInt32 byteoffset = 0              ,
              const CORE::CValueList* valuelist = NULL );

    bool Get( const CORE::CString& urlstring           ,
              const UInt32 byteoffset = 0              ,
              const CORE::CValueList* valuelist = NULL );
              
    void Close( void );                  
              
    bool IsConnected( void ) const;

    UInt32 GetBytesReceived( bool resetCounter );

    UInt32 GetBytesTransmitted( bool resetCounter );

    bool SetHTTPProxy( const CORE::CString& proxyHost ,
                       const UInt16 port = 80         );
    
    void GetHTTPProxy( CORE::CString& proxyHost ,
                       UInt16& port             );

    virtual const CString& GetClassTypeName( void ) const;
    
    protected:	

    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  Note: Do NOT  forget to call this class's implementation
     *  from your descending class.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );
	private:
	
    /**
     *      Event handler that is called when we recieve data from the
     *      server. Initial processing will be done by this server.
     */
    void OnRead( COMCORE::CTCPClientSocket &socket    ,
                 const char* data                     ,
                 const UInt32 length                  );

    void OnDisconnect( COMCORE::CTCPClientSocket& socket );
    
    void OnWrite( COMCORE::CTCPClientSocket& socket                   ,
                  COMCORE::CTCPClientSocket::TDataSentEventData& data );

    private:
    
    bool ParseURL( const CORE::CString& urlstring ,
                   CORE::CString& host            ,
                   UInt16& port                   ,
                   CORE::CString& path            );
    
    CHTTPClient( const CHTTPClient& src );
    CHTTPClient& operator=( const CHTTPClient& src );
    
    private:
    
    CORE::CDynamicBuffer m_sendBuffer;
    COMCORE::CTCPClientSocket m_socket;
    bool m_downloading;
    UInt32 m_recieved;
    UInt32 m_filesize;
    CORE::CString m_proxyHost;
    UInt16 m_proxyPort;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CHTTPCLIENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: changed the code a bit so it also works for multi-part
          transmissions with the Apache server
- 07-02-2007 :
        - Dinand: Added basic HTTP proxy support
- 08-11-2006 :
        - Dinand: Updated notification system usage to the latest revision
- 07-10-2006 :
        - Dinand: Converted class to a notification based event handling system
          instead of the customized event handler interface classes.
- 18-09-2005 :
        - Dinand: Completed switch to a Post/Get based system
- 09-08-2005 :
        - Dinand: Class renamed to CHTTPClient
        - Dinand: Interface and code modified, only loosely based on the previous
          version called CHTTPDownload.
- 31-05-2005 :
        - Dinand: Commenced integration into new GUCEF codebase
- 19-04-2004 :
        - Dinand: If a file is not found (404) error there was no way to detect it.
          Thus i changed the OnError() event handler to include an error code.
          The error code given is one of the new enum values.
        - Dinand: Got rid of the use of atoi() because it's not always available.
        - Dinand: Added external include guards.
        - Dinand: Added HTTP_Code() member function.
- 19-12-2003 :
        - Dinand: Changed handling of OnDisconnect event for the socket.
          This event generated by the socket will now always cause the
          OnDisconnect to be called of the interface. This may or may not be
          preceded with an OnFinish event.
        - Dinand: Fixed a bug in error code debug output.
        - Dinand: Replaced printf() debugging output with the tsprintf() version.
- 05-11-2003 :
        - Dinand: Created overloaded versions of the Download() member function.
          In order to accomplish this some functions call other overloaded
          versions. Only accepting a complete URL is a bit of a drag if all you
          want to change is the file to download in an entire batch of downloads
        - Dinand: Made several inlined get functions const
        - Dinand: Removed some unneeded return values.
        - Dinand: Removed several data members which I replaced with a local variable
          in the relevant member functions.
- 02-11-2003 :
        - Dinand: Received initial version of this class from Logan Benjamin. Many
          thanks to him for contributing this class to the DVNETCOM codebase.
        - Dinand: Modified the file layout to conform with my latest standard.

---------------------------------------------------------------------------*/
