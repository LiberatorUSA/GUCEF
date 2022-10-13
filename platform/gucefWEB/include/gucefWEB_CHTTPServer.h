/*
 *  gucefWEB: GUCEF module providing Web application functionality 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef GUCEF_WEB_CHTTPSERVER_H
#define GUCEF_WEB_CHTTPSERVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_WEB_CIHTTPSERVERROUTERCONTROLLER_H
#include "gucefWEB_CIHTTPServerRouterController.h"
#define GUCEF_WEB_CIHTTPSERVERROUTERCONTROLLER_H
#endif /* GUCEF_WEB_CIHTTPSERVERROUTERCONTROLLER_H ? */

#ifndef GUCEF_WEB_CIHTTPSERVERREQUESTHANDLER_H
#include "gucefWEB_CIHttpServerRequestHandler.h"
#define GUCEF_WEB_CIHTTPSERVERREQUESTHANDLER_H
#endif /* GUCEF_WEB_CIHTTPSERVERREQUESTHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_WEB_PUBLIC_CPP CHttpServerSettings : public CORE::CIConfigurable
{
    public:

    static const CORE::CString ClassTypeName;

    UInt32 maxClientConnections;
    bool keepAliveConnections;
    bool processRequestsAsync;
    bool allowWebSocketUpgrades;

    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CHttpServerSettings( void );
    CHttpServerSettings( const CHttpServerSettings& src );
    virtual ~CHttpServerSettings();
    CHttpServerSettings& operator=( const CHttpServerSettings& src );
};

/*-------------------------------------------------------------------------*/

class GUCEF_WEB_PUBLIC_CPP CHTTPServer : public CORE::CObservingNotifier
{
    public:

    static const CORE::CEvent WebSocketClientConnectedEvent;
        
    typedef CORE::CTLinkedCloneableObj< CHttpRequestData > TWebSocketClientConnectedEventData;
    
    public:

    typedef CString::StringVector TStringVector;
    
    CHTTPServer( THttpServerRequestHandlerFactory* requestHandlerFactory = GUCEF_NULL ,
                 const CHttpServerSettings* settings = GUCEF_NULL                     );

    CHTTPServer( const CORE::PulseGeneratorPtr& pulsGenerator                         ,
                 THttpServerRequestHandlerFactory* requestHandlerFactory = GUCEF_NULL ,
                 const CHttpServerSettings* settings = GUCEF_NULL                     );
    
    virtual ~CHTTPServer();

    void SetSettings( const CHttpServerSettings& settings );

    void GetSettings( CHttpServerSettings& settings );
    
    /** 
     *  listen on default NIC with port given 
     */
    bool ListenOnPort( const UInt16 port );
    
    bool Listen( void );

    bool IsActive( void ) const;
    
    /**
     *  stop listening for clients, close server socket 
     */
    void Close( void ); 
    
    bool SetPort( UInt16 port );
    
    UInt16 GetPort( void ) const;

    CIHTTPServerRouterController* GetRouterController( void ) const;

    CIHttpServerRequestHandler* GetRequestHandler( void ) const;

    THttpServerRequestHandlerFactory* GetRequestHandlerFactory( void ) const;
    
    private:

    friend class CAsyncHttpServerRequestHandler;
    friend class CAsyncHttpServerResponseHandler;

    /**
     *  Event callback member function.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventid               ,
                           CORE::CICloneable* eventdata /* = NULL */ );
                           
    void ExtractCommaSeparatedValues( const CString& stringToExtractFrom ,
                                      TStringVector& list                ) const;

    void StripItems( TStringVector& list ,
                     char stripDelim     ) const;

    void SyncProcessReceivedData( COMCORE::CTCPServerConnection* connection ,
                                  const CORE::CDynamicBuffer& inputBuffer   ,
                                  CORE::CDynamicBuffer& outputBuffer        );

    void ASyncProcessReceivedData( COMCORE::CTCPServerConnection* connection ,
                                   const CORE::CDynamicBuffer& inputBuffer   );

    bool ParseRequest( const CORE::CDynamicBuffer& inputBuffer ,
                       CHttpRequestData& requestData           );
                        
    UInt32 ParseHeaderFields( const char* bufferPtr       ,
                              const UInt32 bufferSize     ,
                              TStringVector& headerFields ) const;

    void RegisterEventHandlers( void );

    void
    OnClientConnectedEvent( CORE::CNotifier* notifier    ,
                            const CORE::CEvent& eventId  ,
                            CORE::CICloneable* eventData );

    void
    OnClientDataReceivedEvent( CORE::CNotifier* notifier    ,
                               const CORE::CEvent& eventId  ,
                               CORE::CICloneable* eventData );

    bool SendResponseASync( UInt32 connectionId                  ,
                            const CORE::CDynamicBuffer& response ,
                            const COMCORE::CIPAddress& remoteIP  );

    void OnWebSocketUpgrade( COMCORE::CTCPServerConnection* connection ,
                             const CHttpRequestData& requestData       );
    
    bool InitSha1CodecLink( void );

    CORE::CString GenerateWebSocketResponseKey( const CORE::CString& clientKey );
    
    virtual bool Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    CHTTPServer( const CHTTPServer& src );             /**< not implemented */
    CHTTPServer& operator=( const CHTTPServer& src );  /**< not implemented */
    
    private:

    typedef CORE::CTEventHandlerFunctor< CHTTPServer > TEventCallback;
    
    COMCORE::CTCPServerSocket m_tcpServerSocket;
    CIHttpServerRequestHandler* m_requestHandler;
    THttpServerRequestHandlerFactory* m_requestHandlerFactory;
    CHttpServerSettings m_settings;
    CORE::CCodecRegistry::TICodecPtr m_sha1Codec;
    MT::CMutex m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CHTTPSERVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
