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

#ifndef GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H
#define GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COM_CHTTPREQUESTDATA_H
#include "gucefCOM_CHttpRequestData.h"
#define GUCEF_COM_CHTTPREQUESTDATA_H
#endif /* GUCEF_COM_CHTTPREQUESTDATA_H ? */

#ifndef GUCEF_COM_CHTTPRESPONSEDATA_H
#include "gucefCOM_CHttpResponseData.h"
#define GUCEF_COM_CHTTPRESPONSEDATA_H
#endif /* GUCEF_COM_CHTTPRESPONSEDATA_H ? */

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

class GUCEF_COM_EXPORT_CPP CIHttpServerRequestHandler
{
    public:

    virtual CHttpResponseData* OnRead( const CHttpRequestData& request ) = 0;

    virtual CHttpResponseData* OnReadMetaData( const CHttpRequestData& request ) = 0;
    
    virtual CHttpResponseData* OnUpdate( const CHttpRequestData& request ,
                                       bool isDeltaUpdateOnly          ) = 0;
    
    virtual CHttpResponseData* OnCreate( const CHttpRequestData& request ) = 0;
    
    virtual CHttpResponseData* OnDelete( const CHttpRequestData& request ) = 0;

    virtual bool SetRouterController( CIHTTPServerRouterController* routerController ) = 0;
    
    virtual CIHTTPServerRouterController* GetRouterController( void ) const = 0;

    CIHttpServerRequestHandler( void );
    CIHttpServerRequestHandler( const CIHttpServerRequestHandler& src );
    virtual ~CIHttpServerRequestHandler();
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H ? */
