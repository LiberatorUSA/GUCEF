/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
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

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_PUBSUB_CMODULE_H
#include "gucefPUBSUB_CModule.h"
#define GUCEF_PUBSUB_CMODULE_H
#endif /* GUCEF_PUBSUB_CMODULE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefPUBSUB.h"
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PUBSUB_EMBED_CODE
GUCEF_DLL_INIT_FUNC_BEG_END( GUCEF::PUBSUB::CModule );
#endif

/*-------------------------------------------------------------------------*/

const char*
GUCEF_PUBSUB_ModuleCompileTimeStamp( void )
{
    return __TIMESTAMP__;
}

/*-------------------------------------------------------------------------*/
