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

#ifndef GUCEF_MEMORY_H
#define GUCEF_MEMORY_H

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  now we include macros for activating memory checking related redirects
 *  if the facility is turned on per the build config
 */
#ifdef GUCEF_USE_MEMORY_LEAK_CHECKER
    #ifdef GUCEF_USE_PLATFORM_MEMORY_LEAK_CHECKER

    /*
     *  Since we are using the GUCEF platform's build-in memory leak checking
     *  we will include the header here that will redirect all malloc()/new etc calls
     */
    #ifndef GUCEF_DYNNEWON_H 
    #include "gucef_dynnewon.h"
    #define GUCEF_DYNNEWON_H
    #endif /* GUCEF_DYNNEWON_H ? */

    #else

    /*
     *  Since we are NOT using the GUCEF platform's build-in memory leak checking
     *  we will try to use whatever facilities the current OS/compiler/IDE provides
     */

    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ) && ( GUCEF_COMPILER == GUCEF_COMPILER_MSVC ) )

        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>    

        #ifdef __cplusplus
          #define GUCEF_MSVCCRT_MEMCHECK_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
          #define new GUCEF_MSVCCRT_MEMCHECK_NEW
        #endif

    #endif 

    #ifndef GUCEF_DYNNEWOFF_H 
    #include "gucef_dynnewoff.h"
    #define GUCEF_DYNNEWOFF_H
    #endif /* GUCEF_DYNNEWOFF_H ? */

    #endif /* GUCEF_USE_PLATFORM_MEMORY_LEAK_CHECKER ? */
#else

    #ifndef GUCEF_DYNNEWOFF_H 
    #include "gucef_dynnewoff.h"
    #define GUCEF_DYNNEWOFF_H
    #endif /* GUCEF_DYNNEWOFF_H ? */

#endif /* GUCEF_USE_MEMORY_LEAK_CHECKER ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MEMORY_H ? */
