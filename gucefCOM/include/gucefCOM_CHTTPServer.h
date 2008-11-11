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

    struct SHttpRequestData
    {
        TStringVector resourceRepresentations;
        TStringVector resourceVersions;
        CString requestType;
        CString requestHost;
        CString requestUri;
        CORE::CDynamicBuffer content;
        CString transactionID; 
    };
    typedef struct SHttpRequestData THttpRequestData;
    
    struct SHttpReturnData
    {
        TStringVector allowedMethods;
        CString cacheability;
        CString eTag;
        CORE::CDynamicBuffer content;
        int statusCode;
        CString contentType;
        CString location;
        TStringVector acceptedTypes;
    };
    typedef struct SHttpReturnData THttpReturnData;
    
    public:

    CHTTPServer( CIHTTPServerRouterController* routerController = NULL );

    CHTTPServer( CORE::CPulseGenerator& pulsGenerator                  ,
                 CIHTTPServerRouterController* routerController = NULL );
    
    virtual ~CHTTPServer();
    
    CString GetLastRequestUri( void ) const;
    
    private:

    THttpReturnData* PerformReadOperation( const THttpRequestData& request , 
                                           bool contentRequested           );

    bool MatchResourceVersion( const CString& resourceVersion  ,
                               const TStringVector& searchList );
                               
    THttpReturnData* OnRead( const THttpRequestData& request );

    THttpReturnData* OnReadMetaData( const THttpRequestData& request );
    
    THttpReturnData* OnUpdate( const THttpRequestData& request );
    
    THttpReturnData* OnCreate( const THttpRequestData& request );
    
    THttpReturnData* OnDelete( const THttpRequestData& request );

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

    void ProcessReceivedData( const CORE::CDynamicBuffer& inputBuffer );

    THttpRequestData* ParseRequest( const CORE::CDynamicBuffer& inputBuffer );

    void ParseResponse( const THttpReturnData& returnData  ,
                        CORE::CDynamicBuffer& outputBuffer );
                        
    UInt32 ParseHeaderFields( const char* bufferPtr       ,
                              const UInt32 bufferSize     ,
                              TStringVector& headerFields ) const;

    CHTTPServer( const CHTTPServer& src );             /**< not implemented */
    CHTTPServer& operator=( const CHTTPServer& src );  /**< not implemented */
    
    private:
    
    COMCORE::CTCPServerSocket m_tcpServerSocket;
    CIHTTPServerRouterController* m_routerController;
    CString m_lastRequestUri;
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
