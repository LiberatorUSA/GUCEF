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

#ifndef GUCEF_WEB_MACROS_H
#define GUCEF_WEB_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
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

#ifndef GUCEF_WEB_ETYPES_H
#include "gucefWEB_ETypes.h"
#define GUCEF_WEB_ETYPES_H
#endif /* GUCEF_WEB_ETYPES_H ? */

#ifndef GUCEF_WEB_CONFIG_H
#include "gucefWEB_config.h"      /* Module build configuration */
#define GUCEF_WEB_CONFIG_H
#endif /* GUCEF_WEB_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Auto detection of debug mode
 */
#ifndef MANUAL_DEBUG_MODE_DEFINE
  #if defined( _DEBUG )
    #define DEBUG_MODE
  #else
    #undef DEBUG_MODE
  #endif /* compiler DEBUG switches */
#endif /* MANUAL_DEBUG_MODE_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */

#undef GUCEF_WEB_EXPORTSPEC
#if defined( GUCEF_WEB_BUILD_MODULE )
  #define GUCEF_WEB_EXPORTSPEC GUCEF_EXPORT
#elif defined( GUCEF_WEB_EMBED_CODE )
  #define GUCEF_WEB_EXPORTSPEC
#else
  #define GUCEF_WEB_EXPORTSPEC GUCEF_IMPORT
#endif /* GUCEF_WEB_BUILD_MODULE ? */

#undef GUCEF_WEB_PUBLIC_CPP
#ifdef GUCEF_WEB_PUBLIC_CPP_CODE
  #define GUCEF_WEB_PUBLIC_CPP GUCEF_WEB_EXPORTSPEC
#else
  #define GUCEF_WEB_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_WEB_EXPORT_CPP_CODE */

#define GUCEF_WEB_PRIVATE_CPP GUCEF_HIDDEN

#undef GUCEF_WEB_PUBLIC_C
#ifdef GUCEF_WEB_PUBLIC_C_CODE
  #define GUCEF_WEB_PUBLIC_C GUCEF_WEB_EXPORTSPEC
#else
  #define GUCEF_WEB_PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEF_WEB_PUBLIC_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_MACROS_H ? */