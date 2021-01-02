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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefWEB_CHttpRequestData.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CHttpRequestData::CHttpRequestData( void )
    : CORE::CICloneable()
    , resourceRepresentations()
    , resourceVersions()
    , requestProtocol()
    , requestProtocolVersion()
    , requestType()
    , requestHost()
    , requestUri()
    , content()
    , contentRepresentation()
    , transactionID()
    , keepConnectionsAlive( true )
    , m_requestUriWithAuthority()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHttpRequestData::CHttpRequestData( const CHttpRequestData& src )
    : CORE::CICloneable( src )
    , resourceRepresentations( src.resourceRepresentations )
    , resourceVersions( src.resourceVersions )
    , requestProtocol( src.requestProtocol )
    , requestProtocolVersion( src.requestProtocolVersion )
    , requestType( src.requestType )
    , requestHost( src.requestHost )
    , requestUri( src.requestUri )
    , content( src.content )
    , contentRepresentation( src.contentRepresentation )
    , transactionID( src.transactionID )
    , keepConnectionsAlive( src.keepConnectionsAlive )
    , m_requestUriWithAuthority( src.m_requestUriWithAuthority )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHttpRequestData::~CHttpRequestData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CHttpRequestData::Clone( void ) const 
{GUCEF_TRACE;

    return new CHttpRequestData( *this );
}

/*-------------------------------------------------------------------------*/

CString 
CHttpRequestData::ConstructUriWithAuthority( void ) const
{GUCEF_TRACE;
    
    if ( m_requestUriWithAuthority.IsNULLOrEmpty() )
    {
        m_requestUriWithAuthority = requestProtocol.Lowercase() + "://" + requestHost + requestUri;        
    }
    return m_requestUriWithAuthority;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
