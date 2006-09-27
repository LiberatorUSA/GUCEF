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

#ifndef GUCEF_MT_CONFIG_H
#include "gucefMT_config.h"        /* Module build configuration */
#define GUCEF_MT_CONFIG_H
#endif /* GUCEF_MT_CONFIG_H ? */

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"          /* GUCEF platform wide macros */
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_MT_BASICMACROS_H
#include "gucefMT_basicMacros.h"   /* generic often used macros */
#define GUCEF_MT_BASICMACROS_H
#endif /* GUCEF_MT_BASICMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines:
 *      GUCEF_EXPORT_C_CODE
 *      GUCEF_EXPORT_CPP_CODE
 */
#ifdef GUCEFMT_BUILD_MODULE
    #if !(defined(GUCEFMT_EXPORT_C_CODE) | defined(GUCEFMT_EXPORT_CPP_CODE))
        #define GUCEFMT_EXPORT_C_CODE
        #define GUCEFMT_EXPORT_CPP_CODE
    #endif

    #define GUCEFMT_EXPORT GUCEF_EXPORT
#else
    #define GUCEFMT_EXPORT GUCEF_IMPORT
#endif /* GUCEF_BUILD_DLL */

/*-------------------------------------------------------------------------*/

/*
 *      config file to control the export type.
 */
#ifdef GUCEFMT_BUILD_MODULE

#else
#endif /* GUCEFMT_BUILD_DLL */

/*-------------------------------------------------------------------------*/

/*
 *  Seperate switch for C++ exports
 */
#undef GUCEFMT_EXPORT_CPP
#ifdef GUCEFMT_EXPORT_CPP_CODE
  #define GUCEFMT_EXPORT_CPP GUCEFMT_EXPORT
#else
  #define GUCEFMT_EXPORT_CPP
#endif /* GUCEFMT_EXPORT_CPP_CODE */

/*-------------------------------------------------------------------------*/

/*
 *  Seperate switch for C exports
 */
#undef GUCEFMT_EXPORT_C
#ifdef GUCEFMT_EXPORT_C_CODE
  #define GUCEFMT_EXPORT_C GUCEFMT_EXPORT
#else
  #define GUCEFMT_EXPORT_C
#endif /* GUCEFMT_EXPORT_C_CODE */

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
