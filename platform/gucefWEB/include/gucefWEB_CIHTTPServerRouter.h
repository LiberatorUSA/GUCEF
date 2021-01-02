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

#ifndef GUCEF_COM_CIHTTPSERVERROUTER_H
#define GUCEF_COM_CIHTTPSERVERROUTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_COM_CIHTTPSERVERRESOURCE_H
#include "gucefCOM_CIHTTPServerResource.h"
#define GUCEF_COM_CIHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CIHTTPSERVERRESOURCE_H ? */

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

class CIHTTPServerRouterController;

class GUCEF_COM_PUBLIC_CPP CIHTTPServerRouter
{
    public:
    
    typedef CIHTTPServerResource::THTTPServerResourcePtr THTTPServerResourcePtr;
    
    CIHTTPServerRouter( void );
    
    CIHTTPServerRouter( const CIHTTPServerRouter& src );
    
    virtual ~CIHTTPServerRouter();
    
    CIHTTPServerRouter& operator=( const CIHTTPServerRouter& src );
    
    virtual THTTPServerResourcePtr ResolveUriToResource( const CString& uri ) = 0;
    
    virtual void SetServiceRoot( const CString& serviceRoot ) = 0;
    
    virtual const CString& GetServiceRoot( void ) const = 0;
    
    virtual void SetMountPath( const CString& mountPath ) = 0;
    
    virtual const CString& GetMountPath( void ) const = 0;
    
    virtual void SetRouterController( CIHTTPServerRouterController& controller ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CIHTTPSERVERROUTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
