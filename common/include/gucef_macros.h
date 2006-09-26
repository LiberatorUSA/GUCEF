/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#ifndef GUCEF_MACROS_H
#define GUCEF_MACROS_H

/*
 *      Build configuration specific macros for the GUCEF platform
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"      /* GUCEF platform build configuration */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

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
#ifndef GUCEF_MANUAL_OS_DEFINE
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
 *      Target platform selection.
 *      Defines:
 *      GUCEF_LINUX_BUILD
 *      GUCEF_MSWIN_BUILD
 */
#if !(defined(GUCEF_LINUX_BUILD) | defined(GUCEF_MSWIN_BUILD))
    #error You need to define a build/target OS
#elif defined(GUCEF_LINUX_BUILD) & defined(GUCEF_MSWIN_BUILD)
    #error You have defined more than 1 build/target OS's
#endif

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
    #define WIN32_LEAN_AND_MEAN     /* trim fat from windoze by default */
    #define WIN32_EXTRA_LEAN        /* trim additional tub of lard from windoze by default */
#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------*/

/*
 *      Change calling convention notation depending on the compiler used.
 */
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define GUCEF_CALLSPEC_C __cdecl
#else  
  #define GUCEF_CALLSPEC_C cdecl
#endif
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define GUCEF_CALLSPEC_STD __stdcall
#else  
  #define GUCEF_CALLSPEC_STD stdcall
#endif   
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define GUCEF_CALLSPEC_PASCAL __pascal
#else  
  #define GUCEF_CALLSPEC_PASCAL pascal
#endif 
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define GUCEF_CALLSPEC_FAST __fastcall
#else  
  #define GUCEF_CALLSPEC_FAST fastcall
#endif

/*
 *      Macro that switches to the desired calling convention
 *      for GUCEF modules
 */ 
#undef GUCEF_CALLSPEC_TYPE
#ifdef GUCEF_USE_CALLSPEC_C 
  #define GUCEF_CALLSPEC_TYPE GUCEF_CALLSPEC_C
#else 
  #ifdef GUCEF_USE_CALLSPEC_STD  
    #define GUCEF_CALLSPEC_TYPE GUCEF_CALLSPEC_STD
  #else  
    #ifdef GUCEF_USE_CALLSPEC_PASCAL    
      #define GUCEF_CALLSPEC_TYPE GUCEF_CALLSPEC_PASCAL
    #else 
       #ifdef GUCEF_USE_CALLSPEC_FAST  
         #define GUCEF_CALLSPEC_TYPE GUCEF_CALLSPEC_FAST
       #else  
         #error No calling convention configuration was specified
       #endif    
     #endif
  #endif
#endif  

/*
 *      Macro that switches to the desired calling convention
 *      for plugins
 */
#undef GUCEF_PLUGIN_CALLSPEC_TYPE
#ifdef GUCEF_USE_PLUGIN_CALLSPEC_C 
  #define GUCEF_PLUGIN_CALLSPEC_TYPE GUCEF_CALLSPEC_C
#else 
  #ifdef GUCEF_USE_PLUGIN_CALLSPEC_STD  
    #define GUCEF_PLUGIN_CALLSPEC_TYPE GUCEF_CALLSPEC_STD
  #else  
    #ifdef GUCEF_USE_PLUGIN_CALLSPEC_PASCAL    
      #define GUCEF_PLUGIN_CALLSPEC_TYPE GUCEF_CALLSPEC_PASCAL
    #else 
       #ifdef GUCEF_USE_PLUGIN_CALLSPEC_FAST  
         #define GUCEF_PLUGIN_CALLSPEC_TYPE GUCEF_CALLSPEC_FAST
       #else  
         #error No calling convention configuration was specified for plugins 
       #endif    
     #endif
  #endif
#endif

/*
 *      Macro that switches between a calling convention prefix and
 *      postfix notation for the calling convention macro.
 */
#undef GUCEF_CALLSPEC_PREFIX
#undef GUCEF_CALLSPEC_SUFFIX
#undef GUCEF_PLUGIN_CALLSPEC_PREFIX
#undef GUCEF_PLUGIN_CALLSPEC_SUFFIX
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define GUCEF_CALLSPEC_PREFIX GUCEF_CALLSPEC_TYPE
  #define GUCEF_CALLSPEC_SUFFIX
  #define GUCEF_PLUGIN_CALLSPEC_PREFIX PLUGIN_CALLSPEC_TYPE  
  #define GUCEF_PLUGIN_CALLSPEC_SUFFIX
#else
  #define GUCEF_CALLSPEC_PREFIX
  #define GUCEF_CALLSPEC_SUFFIX  __attribute__((GUCEF_CALLSPEC_TYPE))
  #define GUCEF_PLUGIN_CALLSPEC_PREFIX   
  #define GUCEF_PLUGIN_CALLSPEC_SUFFIX __attribute__((GUCEF_PLUGIN_CALLSPEC_TYPE)) 
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-09-2006 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/