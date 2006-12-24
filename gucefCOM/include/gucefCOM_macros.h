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

#ifndef GUCEFCOM_MACROS_H
#define GUCEFCOM_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COM_ETYPES_H
#include "gucefCOM_ETypes.h"
#define GUCEF_COM_ETYPES_H
#endif /* GUCEF_COM_ETYPES_H ? */

#ifndef GUCEF_COM_CONFIG_H
#include "gucefCOM_config.h"      /* Module build configuration */
#define GUCEF_COM_CONFIG_H
#endif /* GUCEF_COM_CONFIG_H ? */

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
    #define DEBUG_MODE
  #else
    #undef DEBUG_MODE
  #endif /* compiler DEBUG switches */
#endif /* MANUAL_DEBUG_MODE_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      In debug mode we will include the core module's tracer
 */
#ifdef __cplusplus
  #ifdef DEBUG_MODE
    #ifndef CTRACER_H
      #include "CTracer.h"
      #define CTRACER_H
    #endif /* CTRACER_H ? */
  #else
    #define TRACE
  #endif
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macro for enabling/disabling the GUCEF namespace
 */
#undef GUCEF_NAMESPACE_BEGIN
#undef GUCEF_NAMESPACE_END   
#ifdef USE_GUCEF_NAMESPACE
  #define GUCEF_NAMESPACE_BEGIN   namespace GUCEF {
  #define GUCEF_NAMESPACE_END     }
#else
  #define GUCEF_NAMESPACE_BEGIN
  #define GUCEF_NAMESPACE_END
#endif /* USE_GUCEF_NAMESPACE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macro for enabling/disabling the CORE namespace
 */
#undef COM_NAMESPACE_BEGIN
#undef COM_NAMESPACE_END   
#ifdef USE_COM_NAMESPACE
  #define COM_NAMESPACE_BEGIN   namespace COM {
  #define COM_NAMESPACE_END     }
#else
  #define COM_NAMESPACE_BEGIN
  #define COM_NAMESPACE_END
#endif /* USE_COM_NAMESPACE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking. Use the switches in the
 *      config file to control the export type.
 */
#ifdef GUCEF_MSWIN_BUILD
  #ifdef GUCEFCOM_BUILD_MODULE
    #define GUCEFCOM_EXPORT __declspec( dllexport )
  #else
    #define GUCEFCOM_EXPORT __declspec( dllimport )
  #endif /* GUCEFCOM_BUILD_MODULE ? */
#else
  #define GUCEFCOM_EXPORT   /* Linux does not need an additional directive */
#endif /* GUCEF_MSWIN_BUILD ? */

/*
 *  Are we exporting C++ code ?
 */
#undef GUCEFCOM_EXPORT_CPP
#ifdef GUCEFCOM_EXPORT_CPP_CODE
  #define GUCEFCOM_EXPORT_CPP GUCEFCOM_EXPORT
#else
  #define GUCEFCOM_EXPORT_CPP
#endif /* GUCEFCOM_EXPORT_CPP_CODE ? */

/*
 *  Are we exporting C code ?
 */
#undef GUCEFCOM_EXPORT_C
#ifdef GUCEFCOM_EXPORT_C_CODE
  #define GUCEFCOM_EXPORT_C GUCEFCOM_EXPORT
#else
  #define GUCEFCOM_EXPORT_C
#endif /* GUCEFCOM_EXPORT_C_CODE ? */

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

#endif /* GUCEFCOM_MACROS_H ? */






