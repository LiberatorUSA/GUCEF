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

#ifndef GUCEF_WEB_HTTPCODE_H
#define GUCEF_WEB_HTTPCODE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCOM_MACROS_H
#include "gucefWEB_macros.h"
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum EHttpVerb
{
    HTTP_VERB_UNKNOWN = 0,

    HTTP_VERB_GET,
    HTTP_VERB_PUT,
    HTTP_VERB_POST,
    HTTP_VERB_PATCH,
    HTTP_VERB_DELETE
};
typedef enum EHttpVerb THttpVerb;

/*-------------------------------------------------------------------------*/

/**
 *      Enumeration type with HTTP client and server error codes.
 */
enum EHTTPCODE
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
};
typedef enum EHTTPCODE THTTPCODE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_HTTPCODE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Moved codes into seperate file

---------------------------------------------------------------------------*/
