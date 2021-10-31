/*
 *  gucefMT: GUCEF module providing multithreading solutions
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

#ifndef GUCEF_MT_MACROS_H
#define GUCEF_MT_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"           /* GUCEF platform wide config */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"          /* GUCEF platform wide macros */
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_MT_CONFIG_H
#include "gucefMT_config.h"        /* Module build configuration */
#define GUCEF_MT_CONFIG_H
#endif /* GUCEF_MT_CONFIG_H ? */

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"        /* type definitions of simple types */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

#ifndef GUCEF_MT_BASICMACROS_H
#include "gucefMT_basicMacros.h"   /* generic often used macros */
#define GUCEF_MT_BASICMACROS_H
#endif /* GUCEF_MT_BASICMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines:
 *      GUCEF_EXPORT_C_CODE
 *      GUCEF_EXPORT_CPP_CODE
 */
#if defined( GUCEF_MT_BUILD_MODULE )
  #define GUCEF_MT_EXPORTSPEC GUCEF_EXPORT
#elif defined( GUCEF_MT_EMBED_CODE )
  #define GUCEF_MT_EXPORTSPEC
#else
  #define GUCEF_MT_EXPORTSPEC GUCEF_IMPORT
#endif /* GUCEF_MT_BUILD_MODULE */

/*-------------------------------------------------------------------------*/

/*
 *  Seperate switch for C++ exports
 */
#undef GUCEF_MT_PUBLIC_CPP
#ifdef GUCEF_MT_PUBLIC_CPP_CODE
  #define GUCEF_MT_PUBLIC_CPP GUCEF_MT_EXPORTSPEC
#else
  #define GUCEF_MT_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_MT_PUBLIC_CPP_CODE */

/*-------------------------------------------------------------------------*/

/*
 *  Seperate switch for C exports
 */
#undef GUCEF_MT_PUBLIC_C
#ifdef GUCEF_MT_PUBLIC_C_CODE
  #define GUCEF_MT_PUBLIC_C GUCEF_MT_EXPORTSPEC
#else
  #define GUCEF_MT_PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEFMT_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#define GUCEF_MT_INFINITE_LOCK_TIMEOUT          GUCEF_MT_UINT32MAX
#define GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS     GUCEF_MT_INFINITE_LOCK_TIMEOUT

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-09-2006 :
       - Dinand: reduced macros in this file.

-----------------------------------------------------------------------------*/
