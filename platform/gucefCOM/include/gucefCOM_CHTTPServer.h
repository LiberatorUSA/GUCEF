/*
 *  gucefCOM: GUCEF module providing communication implementations 
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

#ifndef GUCEF_COM_CHTTPSERVER_H
#define GUCEF_COM_CHTTPSERVER_H

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

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COM_CIHTTPSERVERROUTERCONTROLLER_H
#include "gucefCOM_CIHTTPServerRouterController.h"
#define GUCEF_COM_CIHTTPSERVERROUTERCONTROLLER_H
#endif /* GUCEF_COM_CIHTTPSERVERROUTERCONTROLLER_H ? */

#ifndef GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H
#include "gucefCOM_CIHttpServerRequestHandler.h"
#define GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H
#endif /* GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H ? */

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

class GUCEF_COM_EXPORT_CPP CHTTPServer : CORE::CObserver
{
    public:
    
    typedef std::vector< CString > TStringVector;
    
    CHTTPServer( CIHTTPServerRouterController* routerController = GUCEF_NULL ,
                 CIHttpServerRequestHandler* requestHandler = GUCEF_NULL     );

    CHTTPServer( CORE::CPulseGenerator& pulsGenerator                        ,
                 CIHTTPServerRouterController* routerController = GUCEF_NULL ,
                 CIHttpServerRequestHandler* requestHandler = GUCEF_NULL     );
    
    virtual ~CHTTPServer();
    
    /** 
     *  listen on default NIC with port given 
     */
    bool ListenOnPort( const UInt16 port );
    
    bool Listen( void );
    
    /**
     *  stop listening for clients, close server socket 
     */
    void Close( void ); 
    
    bool SetPort( UInt16 port );
    
    UInt16 GetPort( void ) const;

    CIHTTPServerRouterController* GetRouterController( void ) const;

    CIHttpServerRequestHandler* GetRequestHandler( void ) const;
    
    private:

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

    void ProcessReceivedData( const CORE::CDynamicBuffer& inputBuffer ,
                              CORE::CDynamicBuffer& outputBuffer      );

    CHttpRequestData* ParseRequest( const CORE::CDynamicBuffer& inputBuffer );

    void ParseResponse( const CHttpResponseData& returnData  ,
                        CORE::CDynamicBuffer& outputBuffer );
                        
    UInt32 ParseHeaderFields( const char* bufferPtr       ,
                              const UInt32 bufferSize     ,
                              TStringVector& headerFields ) const;

    CHTTPServer( const CHTTPServer& src );             /**< not implemented */
    CHTTPServer& operator=( const CHTTPServer& src );  /**< not implemented */
    
    private:
    
    COMCORE::CTCPServerSocket m_tcpServerSocket;
    CIHTTPServerRouterController* m_routerController;
    CIHttpServerRequestHandler* m_requestHandler;
    bool m_keepAliveConnections;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CHTTPSERVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
