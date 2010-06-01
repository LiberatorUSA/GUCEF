/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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

#ifndef GUCEF_PLATFORM_H
#define GUCEF_PLATFORM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/* Initial platform/compiler-related stuff to set.
*/
#define GUCEF_PLATFORM_WIN32 1
#define GUCEF_PLATFORM_LINUX 2
#define GUCEF_PLATFORM_APPLE 3

#define GUCEF_COMPILER_MSVC 1
#define GUCEF_COMPILER_GNUC 2
#define GUCEF_COMPILER_BORL 3

#define GUCEF_ENDIAN_LITTLE 1
#define GUCEF_ENDIAN_BIG 2

#define GUCEF_ARCHITECTURE_32 1
#define GUCEF_ARCHITECTURE_64 2

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* Finds the compiler type and version.
*/
#if defined( _MSC_VER )
#   define GUCEF_COMPILER GUCEF_COMPILER_MSVC
#   define GUCEF_COMP_VER _MSC_VER

#elif defined( __GNUC__ )
#   define GUCEF_COMPILER GUCEF_COMPILER_GNUC
#   define GUCEF_COMP_VER (((__GNUC__)*100) + \
        (__GNUC_MINOR__*10) + \
        __GNUC_PATCHLEVEL__)

#elif defined( __BORLANDC__ )
#   define GUCEF_COMPILER GUCEF_COMPILER_BORL
#   define GUCEF_COMP_VER __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif


/* Finds the current platform */

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define GUCEF_PLATFORM GUCEF_PLATFORM_WIN32
#   define GUCEF_MSWIN_BUILD
#elif defined( __APPLE_CC__)
#   define GUCEF_PLATFORM GUCEF_PLATFORM_APPLE
#else
#   define GUCEF_PLATFORM GUCEF_PLATFORM_LINUX
#   define GUCEF_LINUX_BUILD
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-01-2007 :
       - Initial version of this file.

---------------------------------------------------------------------------*/
