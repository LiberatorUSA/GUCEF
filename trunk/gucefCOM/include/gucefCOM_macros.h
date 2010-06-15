/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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
 *      Macro for enabling/disabling the GUCEF namespace
 */
#undef namespace GUCEF {
#undef }; /* namespace GUCEF */   
#ifdef USE_GUCEF_NAMESPACE
  #define namespace GUCEF {   namespace GUCEF {
  #define }; /* namespace GUCEF */     }
#else
  #define namespace GUCEF {
  #define }; /* namespace GUCEF */
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

#define GUCEF_COM_EXPORT_CPP GUCEFCOM_EXPORT_CPP
#define GUCEF_COM_EXPORT_C GUCEFCOM_EXPORT_C

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






