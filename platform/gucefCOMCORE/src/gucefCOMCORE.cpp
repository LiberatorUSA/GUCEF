/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_DLLINIT_H
#include "gucef_DLLInit.h"
#define GUCEF_DLLINIT_H
#endif /* GUCEF_DLLINIT_H ? */

#ifndef GUCEF_COMCORE_CGUCEFCOMCOREMODULE_H
#include "CGUCEFCOMCOREModule.h"
#define GUCEF_COMCORE_CGUCEFCOMCOREMODULE_H
#endif /* GUCEF_COMCORE_CGUCEFCOMCOREMODULE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefCOMCORE.h"
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_EMBED_CODE
GUCEF_DLL_INIT_FUNC_BEG_END( GUCEF::COMCORE::CModule );
#endif

/*-------------------------------------------------------------------------*/

const char*
GUCEF_COMCORE_ModuleCompileTimeStamp( void )
{
    return __TIMESTAMP__;
}

/*-------------------------------------------------------------------------*/
