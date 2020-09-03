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

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERREQUESTHANDLER_H
#define GUCEF_COM_CDEFAULTHTTPSERVERREQUESTHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H
#include "gucefCOM_CIHttpServerRequestHandler.h"
#define GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H
#endif /* GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H ? */

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

class GUCEF_COM_PUBLIC_CPP CDefaultHttpServerRequestHandler : public CIHttpServerRequestHandler
{
    public:

    typedef std::vector< CString > TStringVector;

    virtual bool OnRequest( const CHttpRequestData& request , 
                            CHttpResponseData& response     ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnRead( const CHttpRequestData& request , 
                         CHttpResponseData& response     ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool OnReadMetaData( const CHttpRequestData& request , 
                                 CHttpResponseData& response     ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool OnUpdate( const CHttpRequestData& request ,
                           bool isDeltaUpdateOnly          ,
                           CHttpResponseData& response     ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool OnCreate( const CHttpRequestData& request ,
                           CHttpResponseData& response ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool OnDelete( const CHttpRequestData& request ,
                           CHttpResponseData& response     ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SetRouterController( CIHTTPServerRouterController* routerController ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual CIHTTPServerRouterController* GetRouterController( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CDefaultHttpServerRequestHandler( CIHTTPServerRouterController* routerController = GUCEF_NULL );
    CDefaultHttpServerRequestHandler( const CDefaultHttpServerRequestHandler& src );
    virtual ~CDefaultHttpServerRequestHandler();

    private:

    bool PerformReadOperation( const CHttpRequestData& request , 
                               bool contentRequested           ,
                               CHttpResponseData& response     );

    bool MatchResourceVersion( const CString& resourceVersion  ,
                               const TStringVector& searchList );

    private:

    CIHTTPServerRouterController* m_routerController;
};

typedef CORE::CTFactory< CIHttpServerRequestHandler, CDefaultHttpServerRequestHandler > TDefaultHttpServerRequestHandlerFactory;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CDEFAULTHTTPSERVERREQUESTHANDLER_H ? */
