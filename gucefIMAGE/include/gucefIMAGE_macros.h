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

#ifndef GUCEFIMAGE_MACROS_H
#define GUCEFIMAGE_MACROS_H

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
    #define MSWIN_BUILD
  #else
    #define LINUX_BUILD
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

#ifdef MSWIN_BUILD
#define WIN32_LEAN_AND_MEAN     /* don't change this line: trim fat from windoze */
#define WIN32_EXTRA_LEAN        /* don't change this line: trim additional tub of lard from windoze */
#endif

/*-------------------------------------------------------------------------*/

/*
 *      When compiling the gucefIMAGE library you should define BUILD_GUCEFIMAGE_DLL
 *      before including this macro file.
 */
#ifndef MANUAL_EXPORT_DEFINE
  #undef USE_DLL
  #undef BUILD_DLL
  #ifdef BUILD_GUCEFIMAGE_DLL
    #define BUILD_DLL
  #else
    #define USE_DLL
  #endif  
#endif /* MANUAL_EXPORT_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef EXPORT
#ifdef MSWIN_BUILD
  #ifdef BUILD_DLL
    #define EXPORT __declspec( dllexport )
  #else
    #ifdef USE_DLL
      #define EXPORT __declspec( dllimport )
    #else
      #define EXPORT
    #endif /* USE_DLL */
  #endif /* BUILD_DLL */
#else
  #define EXPORT   /* Linux does not need an additional directive */
#endif /* MSWIN_BUILD ? */

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

#endif /* GUCEFIMAGE_MACROS_H ? */




