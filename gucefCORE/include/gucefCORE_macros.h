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

#ifndef GUCEF_CORE_MACROS_H
#define GUCEF_CORE_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CONFIG_H
#include "gucefCORE_config.h"      /* Module build configuration */
#define GUCEF_CORE_CONFIG_H
#endif /* GUCEF_CORE_CONFIG_H ? */

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"           /* include the GUCEF common macros */
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

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
    #define DEBUG_MODE
  #else
    #undef DEBUG_MODE
  #endif /* compiler DEBUG switches */
#endif /* MANUAL_DEBUG_MODE_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking. Use the switches in the
 *      config file to control the export type.
 */
#ifdef GUCEF_MSWIN_BUILD
  #ifdef GUCEFCORE_BUILD_MODULE
    #define GUCEFCORE_EXPORT __declspec( dllexport )
  #else
    #define GUCEFCORE_EXPORT __declspec( dllimport )
  #endif /* GUCEFCORE_BUILD_MODULE ? */
#else
  #define GUCEFCORE_EXPORT   /* Linux does not need an additional directive */
#endif /* GUCEF_MSWIN_BUILD ? */

/*
 *  Are we exporting C++ code ?
 */
#undef GUCEFCORE_EXPORT_CPP
#ifdef GUCEFCORE_EXPORT_CPP_CODE
  #define GUCEFCORE_EXPORT_CPP GUCEFCORE_EXPORT
#else
  #define GUCEFCORE_EXPORT_CPP
#endif /* GUCEFCORE_EXPORT_CPP_CODE ? */

/*
 *  Are we exporting C code ?
 */
#undef GUCEFCORE_EXPORT_C
#ifdef GUCEFCORE_EXPORT_C_CODE
  #define GUCEFCORE_EXPORT_C GUCEFCORE_EXPORT
#else
  #define GUCEFCORE_EXPORT_C
#endif /* GUCEFCORE_EXPORT_C_CODE ? */

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
    #define DEBUGOUTPUTsss( c_str1, c_str2, c_str3 ) ( CORE::tsprintf( "%s%s%s @ Line %d File %s\n", c_str1, c_str2, c_str3, __LINE__, __FILE__ ) )
    #define DEBUGOUTPUTsss( c_str1, c_str2, c_str3, c_str4 ) ( GUCEF::CORE::tsprintf( "%s%s%s%s @ Line %d File %s\n", c_str1, c_str2, c_str3, c_str4, __LINE__, __FILE__ ) )
  #else
    #define DEBUGOUTPUT( c_str ) ( GUCEF::CORE::tsprintf( "%s\n", c_str ) )
    #define DEBUGOUTPUTs DEBUGOUTPUT
    #define DEBUGOUTPUTi( intval ) ( GUCEF::CORE::tsprintf( "%i\n", intval ) )    
    #define DEBUGOUTPUTss( c_str1, c_str2 ) ( GUCEF::CORE::tsprintf( "%s%s\n", c_str1, c_str2 ) )
    #define DEBUGOUTPUTis( intval, c_str ) ( GUCEF::CORE::tsprintf( "%i%s\n", intval, c_str ) )
    #define DEBUGOUTPUTsi( c_str, intval ) ( GUCEF::CORE::tsprintf( "%s%i\n", c_str, intval ) )
    #define DEBUGOUTPUTsss( c_str1, c_str2, c_str3 ) ( GUCEF::CORE::tsprintf( "%s%s%s\n", c_str1, c_str2, c_str3 ) )
    #define DEBUGOUTPUTssss( c_str1, c_str2, c_str3, c_str4 ) ( GUCEF::CORE::tsprintf( "%s%s%s%s\n", c_str1, c_str2, c_str3, c_str4 ) )
  #endif  
#else
  #define DEBUGOUTPUT( c_str ) ( )
  #define DEBUGOUTPUTi ( intval ) ( )
  #define DEBUGOUTPUTs DEBUGOUTPUT
  #define DEBUGOUTPUTss( c_str1, c_str2 ) ( )
  #define DEBUGOUTPUTsi( c_str, intval ) ( )
  #define DEBUGOUTPUTis( intval, c_str ) ( )
  #define DEBUGOUTPUTsss( c_str1, c_str2, c_str3 ) ( )
  #define DEBUGOUTPUTssss( c_str1, c_str2, c_str3, c_str4 ) ( )
#endif /* DEBUG_MODE ? */  

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_MACROS_H ? */
