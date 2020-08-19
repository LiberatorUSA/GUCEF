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

#include "gucefCOM_CHttpResponseData.h"

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

CHttpResponseData::CHttpResponseData( void )
    : CORE::CICloneable()         
    , allowedMethods()
    , cacheability()
    , eTag()
    , lastModified()
    , content()
    , statusCode()
    , contentType()
    , location()
    , acceptedTypes()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHttpResponseData::CHttpResponseData( const CHttpResponseData& src )
    : CORE::CICloneable( src )         
    , allowedMethods( src.allowedMethods )
    , cacheability( src.cacheability )
    , eTag( src.eTag )
    , lastModified( src.lastModified )
    , content( src.content )
    , statusCode( src.statusCode )
    , contentType( src.contentType )
    , location( src.location )
    , acceptedTypes( src.acceptedTypes )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHttpResponseData::~CHttpResponseData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CHttpResponseData::Clone( void ) const 
{GUCEF_TRACE;

    return new CHttpResponseData( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
