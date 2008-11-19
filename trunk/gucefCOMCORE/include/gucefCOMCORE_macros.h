/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_COMCORE_ETYPES_H
#include "gucefCOMCORE_ETypes.h"      /* simple types */
#define GUCEF_COMCORE_ETYPES_H
#endif /* GUCEF_COMCORE_ETYPES_H ? */

#ifndef GUCEF_COMCORE_CONFIG_H
#include "gucefCOMCORE_config.h"      /* Module build configuration */
#define GUCEF_COMCORE_CONFIG_H
#endif /* GUCEF_COMCORE_CONFIG_H ? */

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
  #ifdef GUCEFCOMCORE_BUILD_MODULE
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






