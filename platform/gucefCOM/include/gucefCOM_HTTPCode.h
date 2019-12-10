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

#ifndef GUCEF_COM_HTTPCODE_H
#define GUCEF_COM_HTTPCODE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Enumeration type with HTTP client and server error codes.
 */
typedef enum THTTPCODE
{
        /* Initialization value */
        HTTPCODE_DEFAULT = 0,

        /* HTTP Information Codes */
        HTTPCODE_100_CONTINUE = 100,
        HTTPCODE_101_SWITCHING_PROTOCOLS,

        /* HTTP Success Codes */
        HTTPCODE_200_OK = 200,
        HTTPCODE_201_CREATED,
        HTTPCODE_202_ACCEPTED,
        HTTPCODE_203_NON_AUTHORITATIVE_INFORMATION,
        HTTPCODE_204_NO_CONTENT,
        HTTPCODE_205_RESET_CONTENT,
        HTTPCODE_206_PARTIAL_CONTENT,

        /* HTTP Redirection Codes */
        HTTPCODE_300_MULTIPLE_CHOICES = 300,
        HTTPCODE_301_MOVED_PERMANENTLY,
        HTTPCODE_302_FOUND,
        HTTPCODE_303_SEE_OTHER,
        HTTPCODE_304_NOT_MODIFIED,
        HTTPCODE_305_USE_PROXY,
        HTTPCODE_307_TEMPORARY_REDIRECT,

        /* HTTP Client Error codes */
        HTTPCODE_400_BAD_REQUEST = 400,
        HTTPCODE_401_UNAUTHORIZED,
        HTTPCODE_402_PAYMENT_REQUIRED,
        HTTPCODE_403_FORBIDDEN,
        HTTPCODE_404_NOT_FOUND,
        HTTPCODE_405_METHOD_NOT_ALLOWED,
        HTTPCODE_406_NOT_ACCEPTABLE,
        HTTPCODE_407_PROXY_AUTHENTICATION_REQUIRED,
        HTTPCODE_408_REQUEST_TIMEOUT,
        HTTPCODE_409_CONFLICT,
        HTTPCODE_410_GONE,
        HTTPCODE_411_LENGTH_REQUIRED,
        HTTPCODE_412_PRECONDITION_FAILED,
        HTTPCODE_413_REQUEST_ENTITY_TOO_LARGE,
        HTTPCODE_414_REQUEST_URI_TOO_LARGE,
        HTTPCODE_415_UNSUPPORTED_MEDIA_TYPE,
        HTTPCODE_416_REQUESTED_RANGE_NOT_SATISFIABLE,
        HTTPCODE_417_EXPECTATION_FAILED,

        /* HTTP Server Error codes */
        HTTPCODE_500_INTERNAL_SERVER_ERROR = 500,
        HTTPCODE_501_NOT_IMPLEMENTED,
        HTTPCODE_502_BAD_GATEWAY,
        HTTPCODE_503_SERVICE_UNAVAILABLE,
        HTTPCODE_504_GATEWAY_TIMEOUT,
        HTTPCODE_505_HTTP_VERSION_NOT_SUPPORTED
} THTTPCODE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_HTTPCODE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Moved codes into seperate file

---------------------------------------------------------------------------*/
