/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#ifndef GUCEF_IMAGE_MACROS_H
#define GUCEF_IMAGE_MACROS_H

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
#include "macros.h"             /* generic often used macros */
#define MACROS_H
#endif /* MACROS_H ? */

#ifndef GUCEF_IMAGE_ETYPES_H
#include "gucefIMAGE_ETypes.h"      /* simple types */
#define GUCEF_IMAGE_ETYPES_H
#endif /* GUCEF_IMAGE_ETYPES_H ? */

#ifndef GUCEFIMAGE_CONFIG_H
#include "gucefIMAGE_config.h"      /* Module build configuration */
#define GUCEFIMAGE_CONFIG_H
#endif /* GUCEFIMAGE_CONFIG_H ? */

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
  #else
    #define GUCEF_LINUX_BUILD
  #endif 
#endif /* MANUAL_OS_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Auto detection of debug mode
 */
#ifndef MANUAL_DEBUG_MODE_DEFINE
  #undef DEBUG_MODE
  #if defined( _DEBUG )
    #define DEBUG_MODE
  #endif /* compiler DEBUG switches */
#endif /* MANUAL_DEBUG_MODE_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef EXPORT
#ifdef GUCEF_MSWIN_BUILD
  #ifdef BUILD_GUCEFIMAGE_DLL
    #define EXPORT __declspec( dllexport )
  #else
    #define EXPORT __declspec( dllimport )
  #endif /* BUILD_DLL */
#else
  #define EXPORT   /* Linux does not need an additional directive */
#endif /* GUCEF_MSWIN_BUILD ? */

#undef EXPORT_CPP
#ifdef EXPORT_CPP_CODE
  #define EXPORT_CPP EXPORT
#else
  #define EXPORT_CPP
#endif /* EXPORT_CPP_CODE */

#undef EXPORT_C 
#ifdef EXPORT_C_CODE
  #define EXPORT_C EXPORT
#else
  #define EXPORT_C
#endif /* EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_MACROS_H ? */




