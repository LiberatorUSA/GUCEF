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

#ifndef GUCEF_CORE_BASICMACROS_H
#include "macros.h"             /* generic often used macros */
#define GUCEF_CORE_BASICMACROS_H
#endif /* GUCEF_CORE_BASICMACROS_H ? */

#ifndef GUCEF_IMAGE_ETYPES_H
#include "gucefIMAGE_ETypes.h"      /* simple types */
#define GUCEF_IMAGE_ETYPES_H
#endif /* GUCEF_IMAGE_ETYPES_H ? */

#ifndef GUCEF_IMAGE_CONFIG_H
#include "gucefIMAGE_config.h"      /* Module build configuration */
#define GUCEF_IMAGE_CONFIG_H
#endif /* GUCEF_IMAGE_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef GUCEF_EXPORTSPEC
#ifdef GUCEFIMAGE_BUILD_MODULE
  #define GUCEF_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUCEF_EXPORTSPEC GUCEF_IMPORT
#endif /* BUILD_DLL */

#undef GUCEF_IMAGE_EXPORT_CPP
#ifdef GUCEF_IMAGE_EXPORT_CPP_CODE
  #define GUCEF_IMAGE_EXPORT_CPP GUCEF_EXPORTSPEC
#else
  #define GUCEF_IMAGE_EXPORT_CPP
#endif /* GUCEF_IMAGE_EXPORT_CPP_CODE */

#undef GUCEF_IMAGE_EXPORT_C
#ifdef GUCEF_IMAGE_EXPORT_C_CODE
  #define GUCEF_IMAGE_EXPORT_C GUCEF_EXPORTSPEC
#else
  #define GUCEF_IMAGE_EXPORT_C
#endif /* GUCEF_IMAGE_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_MACROS_H ? */




