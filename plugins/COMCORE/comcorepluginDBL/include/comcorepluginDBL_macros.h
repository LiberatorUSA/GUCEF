/*
 *  comcorepluginDBL: Generic GUCEF COMCORE plugin for providing access to the DBL network driver
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

#ifndef COMCOREPLUGIN_DBL_MACROS_H
#define COMCOREPLUGIN_DBL_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"      
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"      
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef COMCOREPLUGIN_DBL_CONFIG_H
#include "comcorepluginDBL_config.h"
#define COMCOREPLUGIN_DBL_CONFIG_H
#endif /* COMCOREPLUGIN_DBL_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      When compiling the library you should define COMCOREPLUGIN_DBL_BUILD_PLUGIN_DLL
 *      before including this macro file.
 */
#undef COMCOREPLUGIN_DBL_PLUGIN_EXPORTSPEC
#ifdef COMCOREPLUGIN_DBL_BUILD_PLUGIN_DLL
  #define COMCOREPLUGIN_DBL_PLUGIN_EXPORTSPEC GUCEF_EXPORT
#else
  #define COMCOREPLUGIN_DBL_PLUGIN_EXPORTSPEC GUCEF_IMPORT
#endif

/*-------------------------------------------------------------------------*/

#undef COMCOREPLUGIN_DBL_PUBLIC_CPP
#ifdef COMCOREPLUGIN_DBL_PUBLIC_CPP_CODE
  #define COMCOREPLUGIN_DBL_PUBLIC_CPP COMCOREPLUGIN_DBL_EXPORTSPEC
#else
  #define COMCOREPLUGIN_DBL_PUBLIC_CPP GUCEF_HIDDEN
#endif /* COMCOREPLUGIN_DBL_PUBLIC_CPP_CODE */

#undef COMCOREPLUGIN_DBL_PUBLIC_C
#ifdef COMCOREPLUGIN_DBL_PUBLIC_C_CODE
  #define COMCOREPLUGIN_DBL_PUBLIC_C COMCOREPLUGIN_DBL_EXPORTSPEC
#else
  #define COMCOREPLUGIN_DBL_PUBLIC_C GUCEF_HIDDEN
#endif /* COMCOREPLUGIN_DBL_PUBLIC_C_CODE */

#define COMCOREPLUGIN_DBL_PRIVATE_CPP GUCEF_HIDDEN
#define COMCOREPLUGIN_DBL_PRIVATE_C GUCEF_HIDDEN

/*-------------------------------------------------------------------------*/

#undef COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_CPP
#ifdef COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_CPP_CODE
  #define COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_CPP COMCOREPLUGIN_DBL_PLUGIN_EXPORTSPEC
#else
  #define COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_CPP GUCEF_HIDDEN
#endif /* COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_CPP_CODE */

#undef COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_C
#ifdef COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_C_CODE
  #define COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_C COMCOREPLUGIN_DBL_PLUGIN_EXPORTSPEC
#else
  #define COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_C GUCEF_HIDDEN
#endif /* COMCOREPLUGIN_DBL_PLUGIN_PUBLIC_C_CODE */

#define COMCOREPLUGIN_DBL_PLUGIN_PRIVATE_CPP GUCEF_HIDDEN
#define COMCOREPLUGIN_DBL_PLUGIN_PRIVATE_C GUCEF_HIDDEN

/*-------------------------------------------------------------------------*/

#endif /* COMCOREPLUGIN_DBL_MACROS_H ? */

