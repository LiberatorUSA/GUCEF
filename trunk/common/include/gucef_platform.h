/*
 *  GUCEF platform wide macros/defines for OS/Compiler specifics
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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
#define GUCEF_PLATFORM_MSWIN    1
#define GUCEF_PLATFORM_LINUX    2
#define GUCEF_PLATFORM_MACOS    3
#define GUCEF_PLATFORM_ANDROID  4
#define GUCEF_PLATFORM_IPHONEOS 5

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
#   define GUCEF_PLATFORM GUCEF_PLATFORM_MSWIN
#   define GUCEF_MSWIN_BUILD
#elif defined( __APPLE_CC__)
#   define GUCEF_PLATFORM GUCEF_PLATFORM_MACOS
#elif defined( __ANDROID__ ) || defined( ANDROID )
#   define GUCEF_PLATFORM GUCEF_PLATFORM_ANDROID
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
