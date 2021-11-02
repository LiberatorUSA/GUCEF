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

#ifndef GUCEF_SHAREDMODULE_H
#include "gucef_sharedmodule.h"
#define GUCEF_SHAREDMODULE_H
#endif /* GUCEF_SHAREDMODULE_H ? */

#ifndef GUCEF_WEB_CMODULE_H
#include "gucefWEB_CModule.h"
#define GUCEF_WEB_CMODULE_H
#endif /* GUCEF_WEB_CMODULE_H ? */

#include "gucefWEB.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_WEB_EMBED_CODE
GUCEF_DLL_INIT_FUNC_BEG_END( GUCEF::WEB::CModule );
#endif

/*-------------------------------------------------------------------------*/

const char*
GUCEF_WEB_ModuleCompileTimeStamp( void )
{GUCEF_TRACE;

    return __TIMESTAMP__;
}

/*-------------------------------------------------------------------------*/
