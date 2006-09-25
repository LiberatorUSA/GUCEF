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

#ifndef GUCEF_MT_BASICMACROS_H
#include "gucefMT_basicMacros.h"             /* generic often used macros */
#define GUCEF_MT_BASICMACROS_H
#endif /* GUCEF_MT_BASICMACROS_H ? */

#ifndef GUCEF_MT_CONFIG_H
#include "gucefMT_config.h"      /* Module build configuration */
#define GUCEF_MT_CONFIG_H
#endif /* GUCEF_MT_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
    #define WIN32_LEAN_AND_MEAN     /* trim fat from windoze */
    #define WIN32_EXTRA_LEAN        /* trim additional tub of lard from windoze */
#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#ifdef GUCEF_MSWIN_BUILD
  #ifdef GUCEFMT_BUILD_DLL
    #define GUCEFMT_EXPORT __declspec( dllexport )
  #else
    #define GUCEFMT_EXPORT __declspec( dllimport )
  #endif /* GUCEFMT_BUILD_DLL */
#else
  #define GUCEFMT_EXPORT   /* Linux does not need an additional directive */
#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------*/

/*
 *  Seperate switch for C++ exports
 */
#undef GUCEF_EXPORT_CPP
#ifdef GUCEF_EXPORT_CPP_CODE
  #define GUCEFMT_EXPORT_CPP GUCEFMT_EXPORT
#else
  #define GUCEFMT_EXPORT_CPP
#endif /* GUCEF_EXPORT_CPP_CODE */

/*-------------------------------------------------------------------------*/

/*
 *  Seperate switch for C exports
 */
#undef GUCEF_EXPORT_C
#ifdef GUCEF_EXPORT_C_CODE
  #define GUCEFMT_EXPORT_C GUCEFMT_EXPORT
#else
  #define GUCEFMT_EXPORT_C
#endif /* GUCEFMT_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

/*
 *      Calling convention macro's
 */
#undef GUCEF_CALLSPEC_PREFIX
#undef GUCEF_CALLSPEC_SUFFIX
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define GUCEF_CALLSPEC_PREFIX __cdecl
  #define GUCEF_CALLSPEC_SUFFIX
#else
  #define GUCEF_CALLSPEC_PREFIX
  #define GUCEF_CALLSPEC_SUFFIX  __attribute__((cdecl))
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_MACROS_H ? */



