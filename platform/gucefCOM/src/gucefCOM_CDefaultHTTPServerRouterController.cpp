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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COM_CHTTPSERVER_H
#include "gucefCOM_CHTTPServer.h"
#define GUCEF_COM_CHTTPSERVER_H
#endif /* GUCEF_COM_CHTTPSERVER_H ? */

#include "gucefCOM_CDefaultHTTPServerRouterController.h"

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

CDefaultHTTPServerRouterController::CDefaultHTTPServerRouterController( void )
    : CIHTTPServerRouterController() ,
      m_routerMap()                 
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDefaultHTTPServerRouterController::~CDefaultHTTPServerRouterController()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CString
CDefaultHTTPServerRouterController::GetUriPathFromFullUri( const CString& uri ) const
{GUCEF_TRACE;

    // strip the schema section
    Int32 protocolDelimIndex = uri.HasSubstr( "://", true );
    if ( protocolDelimIndex > 0 )
    {
        Int32 hostSectionDelimIndex = uri.HasChar( '/', protocolDelimIndex+3, true );
        if ( hostSectionDelimIndex > 0 )
        {
            // Strip the hostname and port section
            return uri.CutChars( hostSectionDelimIndex, true, 0 );
        }
    }
    
    // the uri is already a uri without schema and hostname
    return uri;
}

/*-------------------------------------------------------------------------*/

CIHTTPServerRouter*
CDefaultHTTPServerRouterController::GetHandler( const CString& uri )
{GUCEF_TRACE;

    // Find the router for which the path is the longest match we can find (if any)
    UInt32 bestMatchLength = 0;
    CIHTTPServerRouter* bestMatchRouter = GUCEF_NULL;

    CString searchStr = GetUriPathFromFullUri( uri );
    TRouterMap::iterator i = m_routerMap.begin();
    while ( i != m_routerMap.end() )
    {
        UInt32 equality = (*i).first.FindMaxSubstrEquality( searchStr, 0, true, true );
        if ( equality > bestMatchLength )
        {
            bestMatchRouter = (*i).second;
        }
        ++i;
    }
    return bestMatchRouter;
}

/*-------------------------------------------------------------------------*/
    
CString
CDefaultHTTPServerRouterController::GetUriAfterTheBaseAddress( CIHTTPServerRouter& router , 
                                                               const CString& fullUri     )
{GUCEF_TRACE;

    CString remnant = GetUriPathFromFullUri( fullUri );
    
    // Check if the full Uri is actually already a relative Uri
    if ( remnant != fullUri )
    {       
        if ( router.GetServiceRoot().Length() > 0 )
        {
            // Strip the service root off the Uri
            remnant = remnant.CutChars( router.GetServiceRoot().Length() + 1, true );
        }

        if ( router.GetMountPath().Length() > 0 )
        {
            // Strip the mount path off the Uri
            remnant = remnant.CutChars( router.GetMountPath().Length() + 1, true );
        }

        if ( remnant.Length() == 0 )
        {
            remnant = "/";
        }
        else
        if ( remnant[ 0 ] != '/' )
        {
            remnant = '/' + remnant;
        }
        
        // Return the relative Uri
        return remnant;
    }
    return fullUri;
}

/*-------------------------------------------------------------------------*/
    
CString
CDefaultHTTPServerRouterController::MakeUriAbsolute( CIHTTPServerRouter& router      ,
                                                     const CString& uriWithAuthority ,
                                                     const CString& uriSuffix        )
{GUCEF_TRACE;
    
    // Check if the full Uri is actually already an absolute Uri
    // The following strips it to just the path if its a full Uri
    CString remnant = GetUriPathFromFullUri( uriSuffix );
    if ( remnant == uriSuffix )
    {
        // Use the 'uriWithAuthority' to build a new trunk for the suffix Uri
        // The suffix may be different but the server etc should be the same
        CString remainderPath = GetUriPathFromFullUri( uriWithAuthority );
        CString absoluteUri = uriWithAuthority.CutChars( remainderPath.Length(), false );

        if ( router.GetServiceRoot().Length() > 0 )
        {
            absoluteUri += '/' + router.GetServiceRoot();
        }
        if ( router.GetMountPath().Length() > 0 ) 
        {
            absoluteUri += '/' + router.GetMountPath();
        }
        return absoluteUri + uriSuffix;
    }
    return uriSuffix;
}

/*-------------------------------------------------------------------------*/
    
void
CDefaultHTTPServerRouterController::AddRouterMapping( CIHTTPServerRouter* router    ,
                                                      const CString& uriBase        ,
                                                      const CString& serviceRootUri )
{GUCEF_TRACE;

    CString basePath;
    if ( serviceRootUri.Length() > 0 )
    {
        basePath = '/' + serviceRootUri + '/' + uriBase;
    }
    else
    {
        basePath = '/' + uriBase;
    }
    m_routerMap[ basePath ] = router;
    
    router->SetMountPath( uriBase );
    router->SetServiceRoot( serviceRootUri );
    router->SetRouterController( *this );
}

/*-------------------------------------------------------------------------*/
    
void
CDefaultHTTPServerRouterController::RemoveRouterMapping( const CString& uriBase )
{GUCEF_TRACE;

     m_routerMap.erase( uriBase );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
