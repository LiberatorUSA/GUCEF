/*
 *  gucefINPUT: GUCEF module providing input device interaction
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_INPUT_MACROS_H
#define GUCEF_INPUT_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef MACROS_H
#include "macros.h"                /* generic often used macros */
#define MACROS_H
#endif /* MACROS_H ? */

#ifndef GUCEF_INPUT_ETYPES_H
#include "gucefINPUT_ETypes.h"      /* simple types */
#define GUCEF_INPUT_ETYPES_H
#endif /* GUCEF_INPUT_ETYPES_H ? */

#ifndef GUCEF_INPUT_CONFIG_H
#include "gucefINPUT_config.h"      /* Module build configuration */
#define GUCEF_INPUT_CONFIG_H
#endif /* GUCEF_INPUT_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      O/S Detection macro.
 *      If you don't want to use the auto detection macro then set
 *      the MANUAL_OS_DEFINE define in the config file.
 */
#ifndef MANUAL_OS_DEFINE
  #if defined( WIN32 ) || defined( _WIN32 )
    #define GUCEF_MSWIN_BUILD
  #elif defined( linux )
    #define GUCEF_LINUX_BUILD
  #else
    #error Cannot automaticly detect your operating system
  #endif  
#endif /* MANUAL_OS_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Auto detection of debug mode
 */
#ifndef MANUAL_DEBUG_MODE_DEFINE
  #if defined( _DEBUG )
    #define GUCEF_INPUT_DEBUG_MODE
  #else
    #undef GUCEF_INPUT_DEBUG_MODE
  #endif /* compiler DEBUG switches */
#endif /* MANUAL_DEBUG_MODE_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      In debug mode we will include the core module's tracer
 */
#ifdef __cplusplus
  #ifdef GUCEF_INPUT_DEBUG_MODE
    #ifndef GUCEF_CORE_CTRACER_H
      #include "CTracer.h"
      #define GUCEF_CORE_CTRACER_H
    #endif /* GUCEF_CORE_CTRACER_H ? */
  #else
    #define TRACE
  #endif
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_INPUT_CONFIG_H
#include "gucefINPUT_config.h"      /* Module build configuration */
#define GUCEF_INPUT_CONFIG_H
#endif /* GUCEF_INPUT_CONFIG_H ? */

/*-------------------------------------------------------------------------*/

/*
 *      When compiling the gucefINPUT library you should define BUILD_GUCEFINPUT_DLL
 *      before including this macro file.
 */
#ifndef GUCEF_INPUT_MANUAL_EXPORT_DEFINE
  #undef GUCEF_INPUT_USE_DLL
  #undef GUCEF_INPUT_BUILD_DLL
  #ifdef BUILD_GUCEF_INPUT_DLL
    #define GUCEF_INPUT_BUILD_DLL
  #else
    #define GUCEF_INPUT_USE_DLL 
  #endif
#endif /* GUCEF_INPUT_MANUAL_EXPORT_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef GUCEF_INPUT_EXPORT
#ifdef GUCEF_MSWIN_BUILD
  #ifdef GUCEF_INPUT_BUILD_DLL
    #define GUCEF_INPUT_EXPORT __declspec( dllexport )
  #else
    #ifdef GUCEF_INPUT_USE_DLL
      #define GUCEF_INPUT_EXPORT __declspec( dllimport )
    #else
      #define GUCEF_INPUT_EXPORT
    #endif /* GUCEF_INPUT_USE_DLL */
  #endif /* GUCEF_INPUT_BUILD_DLL */
#else
  #define GUCEF_INPUT_EXPORT   /* Linux does not need an additional directive */
#endif /* GUCEF_MSWIN_BUILD ? */

#undef GUCEF_INPUT_EXPORT_CPP
#ifdef GUCEF_INPUT_EXPORT_CPP_CODE
  #define GUCEF_INPUT_EXPORT_CPP GUCEF_EXPORT
#else
  #define GUCEF_INPUT_EXPORT_CPP
#endif /* GUCEF_INPUT_EXPORT_CPP_CODE */

#undef GUCEF_INPUT_EXPORT_C 
#ifdef GUCEF_INPUT_EXPORT_C_CODE
  #define GUCEF_INPUT_EXPORT_C GUCEF_INPUT_EXPORT
#else
  #define GUCEF_INPUT_EXPORT_C
#endif /* GUCEF_INPUT_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_INPUT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
