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

#ifndef GUCEF_COMCORE_MACROS_H
#define GUCEF_COMCORE_MACROS_H

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
#include "macros.h"                /* generic often used macros */
#define MACROS_H
#endif /* MACROS_H ? */

#ifndef GUCEF_COMCORE_ETYPES_H
#include "gucefCOMCORE_ETypes.h"      /* simple types */
#define GUCEF_COMCORE_ETYPES_H
#endif /* GUCEF_COMCORE_ETYPES_H ? */

#ifndef GUCEFCOMCORE_CONFIG_H
#include "gucefCOMCORE_config.h"      /* Module build configuration */
#define GUCEFCOMCORE_CONFIG_H
#endif /* GUCEFCOMCORE_CONFIG_H ? */

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
  #elif defined( linux )
    #define GUCEF_LINUX_BUILD
  #else
    #error Cannot automaticly detect your operating system
  #endif  
#endif /* MANUAL_OS_DEFINE ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Auto detection of debug mode
 */
#ifndef MANUAL_DEBUG_MODE_DEFINE
  #if defined( _DEBUG )
    #define GUCEF_COMCORE_DEBUG_MODE
  #else
    #undef GUCEF_COMCORE_DEBUG_MODE
  #endif /* compiler DEBUG switches */
#endif /* MANUAL_DEBUG_MODE_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef EXPORT
#ifdef GUCEF_MSWIN_BUILD
  #ifdef BUILD_GUCEF_COMCORE_DLL
    #define EXPORT __declspec( dllexport )
  #else
    #define EXPORT __declspec( dllimport )
  #endif /* BUILD_DLL */
#else
  #define EXPORT   /* Linux does not need an additional directive */
#endif /* GUCEF_MSWIN_BUILD ? */

#undef GUCEF_COMCORE_EXPORT_CPP
#ifdef GUCEF_COMCORE_EXPORT_CPP_CODE
  #define GUCEF_COMCORE_EXPORT_CPP EXPORT
#else
  #define GUCEF_COMCORE_EXPORT_CPP
#endif /* EXPORT_CPP_CODE */

#undef GUCEF_COMCORE_EXPORT_C 
#ifdef GUCEF_COMCORE_EXPORT_C_CODE
  #define GUCEF_COMCORE_EXPORT_C EXPORT
#else
  #define GUCEF_COMCORE_EXPORT_C
#endif /* GUCEF_COMCORE_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

/*
 *      Change notation depending on the compiler used.
 */
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define CALLSPEC_C __cdecl
#else  
  #define CALLSPEC_C cdecl
#endif  

#undef CALLSPEC_TYPE
#ifdef USE_CALLSPEC_C 
  #define CALLSPEC_TYPE CALLSPEC_C
#elif USE_CALLSPEC_STD  
  #define CALLSPEC_TYPE CALLSPEC_STD
#elif USE_CALLSPEC_PASCAL    
  #define CALLSPEC_TYPE CALLSPEC_PASCAL
#elif USE_CALLSPEC_FAST  
  #define CALLSPEC_TYPE CALLSPEC_FAST
#elif
  #error No calling convention configuration was specified  
#endif    
      
/*
 *      Macro that switches between a calling convention prefix and
 *      postfix notation.
 */
#undef CALLSPEC_PREFIX
#undef CALLSPEC_SUFFIX
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define CALLSPEC_PREFIX CALLSPEC_TYPE
  #define CALLSPEC_SUFFIX
#else
  #define CALLSPEC_PREFIX
  #define CALLSPEC_SUFFIX  __attribute__((CALLSPEC_TYPE))
#endif

/*-------------------------------------------------------------------------*/

/*
 *      Detection of 32/64 bits builds. 
 */
#ifndef MANUAL_BITTARGET_DEFINE
  #undef GUCEF32BIT
  #undef GUCEF64BIT
  #if defined ( GUCEF_MSWIN_BUILD ) && ( defined( _WIN32 ) || defined( WIN32 ) )
    #define GUCEF32BIT
  #elif defined ( GUCEF_MSWIN_BUILD ) && ( defined( _WIN64 ) || defined( WIN64 ) )
    #define GUCEF64BIT
  #else  
    #error Cannot autodetect wheter this is a 32 or 64 bit build
  #endif
#endif

/*-------------------------------------------------------------------------*/

/*
 *      Small macro that changes integers used for storing memory 
 *      addresses to 32 or 64 bit depending on the build target.
 *      Usage of INTPTR allows you to safely cast or manipulate a 
 *      pointer as an integer.  
 */
#undef INTPTR 
#ifdef GUCEF32BIT
  #define INTPTR UInt32
#else /* 64 Bit target */
  #define INTPTR UInt64
#endif  

/*-------------------------------------------------------------------------*/

#ifdef DEBUG_MODE
  #ifndef TSPRINTING_H
    #include "tsprinting.h"
    #define TSPRINTING_H
  #endif /* TSPRINTING_H ? */
#endif /* DEBUG_MODE ? */  

/*-------------------------------------------------------------------------*/

#undef DEBUGOUTPUT 
#undef DEBUGOUTPUTs
#undef DEBUGOUTPUTi
#undef DEBUGOUTPUTss
#undef DEBUGOUTPUTsi
#undef DEBUGOUTPUTis
#undef DEBUGOUTPUTsss
#ifdef DEBUG_MODE
  #ifdef ADD_EXTRA_INFO_TO_DEBUGOUTPUT
    #define DEBUGOUTPUT( c_str ) ( GUCEF::CORE::tsprintf( "%s @ Line %d File %s\n", c_str, __LINE__, __FILE__ ) )
    #define DEBUGOUTPUTs DEBUGOUTPUT
    #define DEBUGOUTPUTi( intval ) ( GUCEF::CORE::tsprintf( "%i @ Line %d File %s\n", intval, __LINE__, __FILE__ ) )    
    #define DEBUGOUTPUTss( c_str1, c_str2 ) ( GUCEF::CORE::tsprintf( "%s%s @ Line %d File %s\n", c_str1, c_str2, __LINE__, __FILE__ ) )
    #define DEBUGOUTPUTis( intval, c_str ) ( GUCEF::CORE::tsprintf( "%i%s @ Line %d File %s\n", intval, c_str, __LINE__, __FILE__ ) )
    #define DEBUGOUTPUTsi( c_str, intval ) ( GUCEF::CORE::tsprintf( "%s%i @ Line %d File %s\n", c_str, intval, __LINE__, __FILE__ ) )    
    #define DEBUGOUTPUTsss( c_str1, c_str2, c_str3 ) ( GUCEF::CORE::tsprintf( "%s%s%s @ Line %d File %s\n", c_str1, c_str2, c_str3, __LINE__, __FILE__ ) )
  #else
    #define DEBUGOUTPUT( c_str ) ( GUCEF::CORE::tsprintf( "%s\n", c_str ) )
    #define DEBUGOUTPUTs DEBUGOUTPUT
    #define DEBUGOUTPUTi( intval ) ( GUCEF::CORE::tsprintf( "%i\n", intval ) )    
    #define DEBUGOUTPUTss( c_str1, c_str2 ) ( GUCEF::CORE::tsprintf( "%s%s\n", c_str1, c_str2 ) )
    #define DEBUGOUTPUTis( intval, c_str ) ( GUCEF::CORE::tsprintf( "%i%s\n", intval, c_str ) )
    #define DEBUGOUTPUTsi( c_str, intval ) ( GUCEF::CORE::tsprintf( "%s%i\n", c_str, intval ) )
    #define DEBUGOUTPUTsss( c_str1, c_str2, c_str3 ) ( GUCEF::CORE::tsprintf( "%s%s%s\n", c_str1, c_str2, c_str3 ) )
  #endif  
#else
  #define DEBUGOUTPUT( c_str ) ( )
  #define DEBUGOUTPUTi ( intval ) ( )
  #define DEBUGOUTPUTs DEBUGOUTPUT
  #define DEBUGOUTPUTss( c_str1, c_str2 ) ( )
  #define DEBUGOUTPUTsi( c_str, intval ) ( )
  #define DEBUGOUTPUTis( intval, c_str ) ( )
  #define DEBUGOUTPUTsss( c_str1, c_str2, c_str3 ) ( )
#endif /* DEBUG_MODE ? */  

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_MACROS_H ? */






