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

#ifndef GUCEF_WEB_CHTTPMIMETYPES_H
#define GUCEF_WEB_CHTTPMIMETYPES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_WEB_MACROS_H
#include "gucefWEB_macros.h"      /* often used gucefCOM macros */
#define GUCEF_WEB_MACROS_H
#endif /* GUCEF_WEB_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_WEB_PUBLIC_CPP CHttpMimeTypes
{    
    public:
    
    static const CORE::CString MimeTypeAny;
    static const CORE::CString MimeTypeJson;
    static const CORE::CString MimeTypeJsonOld;
    static const CORE::CString MimeTypeYamlProposed;
    static const CORE::CString MimeTypeYamlText;
    static const CORE::CString MimeTypeYamlTextExt;
    static const CORE::CString MimeTypeYamlApp;
    static const CORE::CString MimeTypeYamlAppExt;
    static const CORE::CString MimeTypeXml;
    static const CORE::CString MimeTypeXmlAlt;
    static const CORE::CString MimeTypeIni;
    static const CORE::CString MimeTypeIniProper;
    static const CORE::CString MimeTypeText;
    static const CORE::CString MimeTypeOctet;
    static const CORE::CString MimeTypeWwwFormUrlencoded;
    static const CORE::CString MimeTypeGupDataNodeContainer;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CHTTPMIMETYPES_H ? */
   