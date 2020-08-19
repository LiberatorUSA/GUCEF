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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefCOM_CHttpRequestData.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CHttpRequestData::CHttpRequestData( void )
    : CORE::CICloneable()         
    , resourceRepresentations()
    , resourceVersions()
    , requestType()
    , requestHost()
    , requestUri()
    , content()
    , transactionID()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHttpRequestData::CHttpRequestData( const CHttpRequestData& src )
    : CORE::CICloneable( src )         
    , resourceRepresentations( src.resourceRepresentations )
    , resourceVersions( src.resourceVersions )
    , requestType( src.requestType )
    , requestHost( src.requestHost )
    , requestUri( src.requestUri )
    , content( src.content )
    , transactionID( src.transactionID )
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
