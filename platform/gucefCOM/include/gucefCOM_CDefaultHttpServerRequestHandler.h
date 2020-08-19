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

class GUCEF_COM_EXPORT_CPP CDefaultHttpServerRequestHandler : public CIHttpServerRequestHandler
{
    public:

    typedef std::vector< CString > TStringVector;

    virtual CHttpResponseData* OnRead( const CHttpRequestData& request ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CHttpResponseData* OnReadMetaData( const CHttpRequestData& request ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual CHttpResponseData* OnUpdate( const CHttpRequestData& request ,
                                       bool isDeltaUpdateOnly          ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual CHttpResponseData* OnCreate( const CHttpRequestData& request ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual CHttpResponseData* OnDelete( const CHttpRequestData& request ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SetRouterController( CIHTTPServerRouterController* routerController ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual CIHTTPServerRouterController* GetRouterController( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CDefaultHttpServerRequestHandler( CIHTTPServerRouterController* routerController = GUCEF_NULL );
    CDefaultHttpServerRequestHandler( const CDefaultHttpServerRequestHandler& src );
    virtual ~CDefaultHttpServerRequestHandler();

    private:

    CHttpResponseData* PerformReadOperation( const CHttpRequestData& request , 
                                           bool contentRequested           );

    bool MatchResourceVersion( const CString& resourceVersion  ,
                               const TStringVector& searchList );

    private:

    CIHTTPServerRouterController* m_routerController;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CDEFAULTHTTPSERVERREQUESTHANDLER_H ? */
