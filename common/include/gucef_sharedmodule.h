/*
 *  gucef common header: provides header based platform wide facilities
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

#ifndef GUCEF_SHAREDMODULE_H
#define GUCEF_SHAREDMODULE_H

/*
 *      Build configuration specific macros for the GUCEF platform
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"        /* GUCEF configuration */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

#ifndef GUCEF_PLATFORM_H
#include "gucef_platform.h"      /* GUCEF platform compilation targets */
#define GUCEF_PLATFORM_H
#endif /* GUCEF_PLATFORM_H ? */

#ifndef GUCEF_CALLCONV_H
#include "gucef_callconv.h"      /* GUCEF platform calling convention macros */
#define GUCEF_CALLCONV_H
#endif /* GUCEF_CALLCONV_H ? */


/* @TODO: the dll init header should be merged into this header at some point */
#ifndef GUCEF_DLLINIT_H
#include "gucef_DLLInit.h"       /* O/S dependent shared library entry point macros */
#define GUCEF_DLLINIT_H
#endif /* GUCEF_DLLINIT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      O/S Specific Switches
 */
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #define WIN32_LEAN_AND_MEAN     /* trim fat from windoze by default */
    #define WIN32_EXTRA_LEAN        /* trim additional tub of lard from windoze by default */

    #define GUCEF_EXPORT __declspec( dllexport )
    #define GUCEF_IMPORT __declspec( dllimport )
    #define GUCEF_HIDDEN
#else
    #if ( GUCEF_COMPILER == GUCEF_COMPILER_GNUC ) && ( __GNUC__ >= 4 )
        #define GUCEF_EXPORT __attribute__ ((__visibility__("default")))
        #define GUCEF_IMPORT __attribute__ ((__visibility__("default")))
        #define GUCEF_HIDDEN __attribute__ ((__visibility__("hidden")))
    #else
        #define GUCEF_EXPORT
        #define GUCEF_IMPORT
        #define GUCEF_HIDDEN
    #endif
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_SHAREDMODULE_H ? */
