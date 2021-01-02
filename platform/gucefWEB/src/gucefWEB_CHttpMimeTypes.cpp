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

#include "gucefWEB_CHttpMimeTypes.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CHttpMimeTypes::MimeTypeAny = "*/*";
const CORE::CString CHttpMimeTypes::MimeTypeJson = "application/json";
const CORE::CString CHttpMimeTypes::MimeTypeJsonOld = "text/x-json";
const CORE::CString CHttpMimeTypes::MimeTypeXml = "application/xml";
const CORE::CString CHttpMimeTypes::MimeTypeXmlAlt = "text/xml";
const CORE::CString CHttpMimeTypes::MimeTypeIni = "application/ini";
const CORE::CString CHttpMimeTypes::MimeTypeIniProper = "zz-application/zz-winassoc-ini";
const CORE::CString CHttpMimeTypes::MimeTypeText = "text/plain";
const CORE::CString CHttpMimeTypes::MimeTypeOctet = "application/octet-stream";

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
