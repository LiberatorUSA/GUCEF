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

CDefaultHTTPServerRouterController::CDefaultHTTPServerRouterController( CHTTPServer& server )
    : CIHTTPServerRouterController() ,
      m_routerMap()                  ,
      m_httpServer( &server )
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
    CString remnant = uri.SubstrToSubstr( "://", false );
    if ( remnant.Length() < uri.Length() )
    {
        // Strip the hostname and port section
        return '/' + remnant.SubstrToChar( '/', true );
    }
    
    // the uri is already a uri without schema and hostname
    return uri;
}

/*-------------------------------------------------------------------------*/

CIHTTPServerRouter*
CDefaultHTTPServerRouterController::GetHandler( const CString& uri )
{GUCEF_TRACE;

    CString searchStr = GetUriPathFromFullUri( uri );
    TRouterMap::iterator i = m_routerMap.begin();
    while ( i != m_routerMap.end() )
    {
        if ( (*i).first.HasSubstr( searchStr, true ) )
        {
            return (*i).second;
        }
        ++i;
    }
    return NULL;
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
CDefaultHTTPServerRouterController::MakeUriAbsolute( CIHTTPServerRouter& router ,
                                                     const CString& uriSuffix   )
{GUCEF_TRACE;

    return MakeUriAbsolute( router                            , 
                            m_httpServer->GetLastRequestUri() ,
                            uriSuffix                         );
}

/*-------------------------------------------------------------------------*/
    
CString
CDefaultHTTPServerRouterController::MakeUriAbsolute( CIHTTPServerRouter& router      ,
                                                     const CString& uriWithAuthority ,
                                                     const CString& uriSuffix        )
{GUCEF_TRACE;
    
    // Check if the full Uri is actually already a absolute Uri
    CString remnant = GetUriPathFromFullUri( uriSuffix );
    if ( remnant == uriSuffix )
    {
        // Use the 'uriWithAuthority' to build a new trunk for the suffix Uri
        CString absoluteUri = uriWithAuthority.CutChars( uriWithAuthority.Length() - GetUriPathFromFullUri( uriWithAuthority ).Length(), false );

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
