/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"           /* include the GUCEF common config */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"           /* include the GUCEF common macros */
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CONFIG_H
#include "gucefCORE_config.h"      /* Module build configuration */
#define GUCEF_CORE_CONFIG_H
#endif /* GUCEF_CORE_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Auto detection of debug mode
 */
#ifndef MANUAL_DEBUG_MODE_DEFINE
  #if !defined( NDEBUG ) && !( defined( _DEBUG ) || defined( __DEBUG ) || defined( __DEBUG__ ) )
    #define _DEBUG
  #endif
  #if defined( _DEBUG ) || defined( __DEBUG ) || defined( __DEBUG__ )
    #define GUCEF_CORE_DEBUG_MODE
  #else
    #undef GUCEF_CORE_DEBUG_MODE
  #endif /* compiler DEBUG switches */
#endif /* MANUAL_DEBUG_MODE_DEFINE ? */

/*-------------------------------------------------------------------------*/

/* support for legacy define */
#ifdef GUCEFCORE_BUILD_MODULE
  #define GUCEF_CORE_BUILD_MODULE
#endif

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */

#undef GUCEF_CORE_EXPORTSPEC
#if defined( GUCEF_CORE_BUILD_MODULE )
  #define GUCEF_CORE_EXPORTSPEC GUCEF_EXPORT
#elif defined( GUCEF_CORE_EMBED_CODE )
  #define GUCEF_CORE_EXPORTSPEC GUCEF_HIDDEN
#else
  #define GUCEF_CORE_EXPORTSPEC GUCEF_IMPORT
#endif /* GUCEF_CORE_BUILD_MODULE ? */

#undef GUCEF_CORE_PUBLIC_CPP
#ifdef GUCEF_CORE_PUBLIC_CPP_CODE
  #define GUCEF_CORE_PUBLIC_CPP GUCEF_CORE_EXPORTSPEC
#else
  #define GUCEF_CORE_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_CORE_EXPORT_CPP_CODE */

#define GUCEF_CORE_PRIVATE_CPP GUCEF_HIDDEN

#undef GUCEF_CORE_PUBLIC_C
#ifdef GUCEF_CORE_PUBLIC_C_CODE
  #define GUCEF_CORE_PUBLIC_C GUCEF_CORE_EXPORTSPEC
#else
  #define GUCEF_CORE_PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEF_CORE_PUBLIC_C_CODE */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_DEFAULT_STRING_FORMAT
    #define GUCEF_DEFAULT_STRING_FORMAT     GUCEF_DATATYPE_UTF8_STRING
#endif

/*-------------------------------------------------------------------------*/

#if ( GUCEF_DEFAULT_STRING_FORMAT == GUCEF_DATATYPE_ASCII_STRING )

#define GUCEF_DATATYPE_STRING               GUCEF_DATATYPE_ASCII_STRING
#define GUCEF_DATATYPE_BOOLEAN_STRING       GUCEF_DATATYPE_BOOLEAN_ASCII_STRING

#else

#define GUCEF_DATATYPE_STRING               GUCEF_DATATYPE_UTF8_STRING
#define GUCEF_DATATYPE_BOOLEAN_STRING       GUCEF_DATATYPE_BOOLEAN_UTF8_STRING

#endif

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_DEFAULT_UTF_STRING_FORMAT
    #define GUCEF_DEFAULT_UTF_STRING_FORMAT     GUCEF_DATATYPE_UTF8_STRING
#endif

/*-------------------------------------------------------------------------*/

#if ( GUCEF_DEFAULT_UTF_STRING_FORMAT == GUCEF_DATATYPE_UTF8_STRING )

#define GUCEF_DATATYPE_UTF_STRING               GUCEF_DATATYPE_UTF8_STRING
#define GUCEF_DATATYPE_BOOLEAN_UTF_STRING       GUCEF_DATATYPE_BOOLEAN_UTF8_STRING

#endif

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_CORE_DEBUG_MODE
  #ifndef TSPRINTING_H
    #include "tsprinting.h"
    #define TSPRINTING_H
  #endif /* TSPRINTING_H ? */
#endif /* GUCEF_CORE_DEBUG_MODE ? */

/*-------------------------------------------------------------------------*/

#undef DEBUGOUTPUT
#undef DEBUGOUTPUTs
#undef DEBUGOUTPUTi
#undef DEBUGOUTPUTss
#undef DEBUGOUTPUTsi
#undef DEBUGOUTPUTis
#undef DEBUGOUTPUTsss
#ifdef GUCEF_CORE_DEBUG_MODE
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
#endif /* GUCEF_CORE_DEBUG_MODE ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_MACROS_H ? */
