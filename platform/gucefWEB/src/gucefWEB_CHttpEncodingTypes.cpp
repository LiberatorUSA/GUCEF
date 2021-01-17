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

#include "gucefWEB_CHttpEncodingTypes.h"

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

const CORE::CString CHttpEncodingTypes::EncodingTypeAny = "*";
const CORE::CString CHttpEncodingTypes::EncodingTypeIdentity = "identify";
const CORE::CString CHttpEncodingTypes::EncodingTypeGZip = "gzip";
const CORE::CString CHttpEncodingTypes::EncodingTypeGZipAlt = "x-gzip";
const CORE::CString CHttpEncodingTypes::EncodingTypeCompress = "compress";
const CORE::CString CHttpEncodingTypes::EncodingTypeDeflate = "deflate";
const CORE::CString CHttpEncodingTypes::EncodingTypeBrotli = "br";

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
