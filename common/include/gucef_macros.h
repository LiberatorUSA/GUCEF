/*
 *  Generic header for GUCEF platform wide macros
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
#include "gucef_config.h"        /* GUCEF configuration */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

#ifndef GUCEF_PLATFORM_H
#include "gucef_platform.h"      /* GUCEF platform build configuration */
#define GUCEF_PLATFORM_H
#endif /* GUCEF_PLATFORM_H ? */

#ifndef GUCEF_DLLINIT_H
#include "gucef_DLLInit.h"       /* Platform dependent shared library entry point macros */
#define GUCEF_DLLINIT_H
#endif /* GUCEF_DLLINIT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  Dummy define that can be used as a dummy param in macros
 */
#undef GUCEF_VOIDDEF
#define GUCEF_VOIDDEF

/*-------------------------------------------------------------------------*/

/*
 *  GUCEF wide debug flag
 */
#if !defined( NDEBUG ) && !( defined( _DEBUG ) || defined( __DEBUG ) || defined( __DEBUG__ ) )
  #define _DEBUG
#endif
#if defined( _DEBUG ) || defined( __DEBUG ) || defined( __DEBUG__ )
  #define GUCEF_DEBUG_MODE
#else
  #undef GUCEF_DEBUG_MODE
#endif

/*-------------------------------------------------------------------------*/

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

/*
 *      Bit target
 */
#if !( defined(GUCEF_32BIT) || defined(GUCEF_64BIT) )
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
        #ifdef _WIN64
            #define GUCEF_64BIT
        #endif
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
        #ifdef __LP64__
            #define GUCEF_64BIT
        #endif
    #endif
#endif

#if !( defined(GUCEF_32BIT) || defined(GUCEF_64BIT) )
    #define GUCEF_32BIT
#endif

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
 *      for GUCEF modules. Default GUCEF_CALLSPEC_C
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
         #define GUCEF_CALLSPEC_TYPE GUCEF_CALLSPEC_C
       #endif
     #endif
  #endif
#endif

/*
 *      Macro that switches to the desired calling convention
 *      for plugins. Default GUCEF_CALLSPEC_C
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
         #define GUCEF_PLUGIN_CALLSPEC_TYPE GUCEF_CALLSPEC_C
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
  #define GUCEF_PLUGIN_CALLSPEC_PREFIX GUCEF_PLUGIN_CALLSPEC_TYPE
  #define GUCEF_PLUGIN_CALLSPEC_SUFFIX
#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
  #define GUCEF_CALLSPEC_PREFIX
  #define GUCEF_CALLSPEC_SUFFIX
  #define GUCEF_PLUGIN_CALLSPEC_PREFIX
  #define GUCEF_PLUGIN_CALLSPEC_SUFFIX
#else
  #define GUCEF_CALLSPEC_PREFIX __attribute__((GUCEF_CALLSPEC_TYPE))
  #define GUCEF_CALLSPEC_SUFFIX
  #define GUCEF_PLUGIN_CALLSPEC_PREFIX __attribute__((GUCEF_PLUGIN_CALLSPEC_TYPE))
  #define GUCEF_PLUGIN_CALLSPEC_SUFFIX
#endif


/*
 *      Macro that switches between a calling convention prefix and
 *      postfix notation for the calling convention macro for the std
 *      calling convention
 */
#undef GUCEF_CALLSPEC_STD_PREFIX
#undef GUCEF_CALLSPEC_STD_SUFFIX
#undef GUCEF_PLUGIN_CALLSPEC_STD_PREFIX
#undef GUCEF_PLUGIN_CALLSPEC_STD_SUFFIX
#if defined ( __BORLANDC__ ) || defined ( _MSC_VER )
  #define GUCEF_CALLSPEC_STD_PREFIX GUCEF_CALLSPEC_STD
  #define GUCEF_CALLSPEC_STD_SUFFIX
  #define GUCEF_PLUGIN_CALLSPEC_STD_PREFIX GUCEF_CALLSPEC_STD
  #define GUCEF_PLUGIN_CALLSPEC_STD_SUFFIX
#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )
  #define GUCEF_CALLSPEC_STD_PREFIX
  #define GUCEF_CALLSPEC_STD_SUFFIX
  #define GUCEF_PLUGIN_CALLSPEC_STD_PREFIX
  #define GUCEF_PLUGIN_CALLSPEC_STD_SUFFIX
#else
  #define GUCEF_CALLSPEC_STD_PREFIX __attribute__((GUCEF_CALLSPEC_STD))
  #define GUCEF_CALLSPEC_STD_SUFFIX
  #define GUCEF_PLUGIN_CALLSPEC_STD_PREFIX __attribute__((GUCEF_CALLSPEC_STD))
  #define GUCEF_PLUGIN_CALLSPEC_STD_SUFFIX
#endif

/*-------------------------------------------------------------------------*/

/*
 *  Macros that defines the correct character to be used for directory
 *  seperation in a path depending on the platform we are building for.
 */
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #define GUCEF_DIRSEPCHAROPPOSITE '/'
  #define GUCEF_DIRSEPCHAR '\\'
#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
  #define GUCEF_DIRSEPCHAROPPOSITE '\\'
  #define GUCEF_DIRSEPCHAR '/'
#endif

/*-------------------------------------------------------------------------*/

/*
 *  Macros that defines the correct end of line string to be used
 *  depending on the platform we are building for.
 */
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #define GUCEF_EOL "\r\n"
  #define GUCEF_EOL_LENGTH 2
#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
  #define GUCEF_EOL "\n"
  #define GUCEF_EOL_LENGTH 1
#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_MACOS ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_IPHONEOS ) )
  #define GUCEF_EOL "\r"
  #define GUCEF_EOL_LENGTH 1
#endif

/*-------------------------------------------------------------------------*/

/*
 *      Bit operations.
 *      Use the following macro's to set/get a bit in an integer.
 */
#undef GUCEF_GETBITX
#define GUCEF_GETBITX( var, x ) ( var & ( 1<<x ) )
#undef GUCEF_SETBITXON
#define GUCEF_SETBITXON( var, x ) ( var |= 1<<x )
#undef GUCEF_SETBITXOFF
#define GUCEF_SETBITXOFF( var, x ) ( var &= ~( 1<<x ) )

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus
#undef GUCEF_DECLARE_SINGLETON_INTERFACE
#define GUCEF_DECLARE_SINGLETON_INTERFACE( identifier )          \
(                                                                \
        private:                                                 \
                identifier( void );                              \
                identifier( const identifier& src );             \
                identifier& operator=( const identifier& src );  \
                static identifier *_instance;                    \
        public:                                                  \
                identifier* Instance( void );                    \
)
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus
#undef GUCEF_DECLARE_SINGLETON
#define GUCEF_DECLARE_SINGLETON( apiCall, identifier, baseClass ) \
(                                                                 \
        class apiCall identifier baseClass                        \
        {                                                         \
            GUCEF_DECLARE_SINGLETON_INTERFACE( identifier )       \
        };                                                        \
)
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus
#undef GUCEF_DEFINE_SINGLETON_INTERFACE
#define GUCEF_DEFINE_SINGLETON_INTERFACE( identifier, baseClass )  \
(                                                                  \
                                                                   \
        identifier::identifier( void )                             \
            baseClass                                              \
        {                                                          \
        }                                                          \
                                                                   \
        identifier*                                                \
        identifier::Instance( void )                               \
        {                                                          \
                if ( !m_instance )                                 \
                {                                                  \
                        m_instance = new identifier();             \
                }                                                  \
                return m_instance;                                 \
        }                                                          \
                                                                   \
        identifier::identifier( const identifier& src )            \
            baseClass                                              \
        {                                                          \
        }                                                          \
                                                                   \
        identifier&                                                \
        identifier::operator=( const identifier& src )             \
        {                                                          \
                return *this;                                      \
        }                                                          \
)

#undef GUCEF_DEFINE_SINGLETON
#define GUCEF_DEFINE_SINGLETON GUCEF_DEFINE_SINGLETON_INTERFACE

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#define GUCEF_ASSERT_ALWAYS    assert( 0 );
#define GUCEF_UNREACHABLE      GUCEF_ASSERT_ALWAYS;

/*-------------------------------------------------------------------------*/

/*
 *  now we include some dynamicly linked module macro's
 */
#ifndef GUCEF_DLLINIT_H
#include "gucef_DLLInit.h"
#define GUCEF_DLLINIT_H
#endif /* GUCEF_DLLINIT_H ? */

/*-------------------------------------------------------------------------*/

/*
 *  now we include a macro for unifying the main entry point into applications
 *  Usage of this macro is of course optional. It is for convenience only.
 */
#ifndef GUCEF_OSMAIN_H
#include "gucef_osmain.h"
#define GUCEF_OSMAIN_H
#endif /* GUCEF_OSMAIN_H ? */

/*-------------------------------------------------------------------------*/

/* GCC/G++ does not define __TIMESTAMP__ */

#ifndef __TIMESTAMP__
#define __TIMESTAMP__ __DATE__ " " __TIME__
#endif

/*-------------------------------------------------------------------------*/

#undef NULL
#if defined(__cplusplus)
  #ifndef __CPP09NULLPTRSUPPORTED
    #if GUCEF_COMPILER == GUCEF_COMPILER_GNUC
      #define NULL __null
    #else
      #define NULL (0)
    #endif
  #else
    #define NULL (nullptr)
  #endif
#else
  #define NULL ((void*)0)
#endif

#define GUCEF_NULL NULL

/*-------------------------------------------------------------------------*/

#if defined(__cplusplus)
    #undef GUCEF_RVALUE_REFERENCES_SUPPORTED
    #if defined( _MSC_VER )  /* MS visual studio */
        #if _MSC_VER >= 1600  /* Visual Studio 2010 */
            #define GUCEF_RVALUE_REFERENCES_SUPPORTED 1
        #endif
    #elif defined( __GNUG__ )  /* GNU C++ compiler */
        #if __cpp_rvalue_references >= 200610
            #define GUCEF_RVALUE_REFERENCES_SUPPORTED 1
        #endif
    #else
        /* Unknown compiler, fall back to cpp version check */
        #if __cplusplus > 201103L  /* This is not fullproof since not every compiler truly supports the spec */
            #define GUCEF_RVALUE_REFERENCES_SUPPORTED 1
        #endif        
    #endif
#endif

/*-------------------------------------------------------------------------*/

#undef GUCEF_VIRTUAL_OVERRIDE_IS_SUPPORTED
#if defined(__cplusplus)
  #if ( __cplusplus >= 201103L ) /* Are we using C++11 or newer */
    #define GUCEF_VIRTUAL_OVERRIDE_IS_SUPPORTED  1
  #else
    #if defined( _MSC_VER )   /* Visual Studio has support even if the language version specified is older */
      #if ( _MSC_VER > 1600 ) /* Visual Studio 2010 (VC10) */
        #define GUCEF_VIRTUAL_OVERRIDE_IS_SUPPORTED  1
      #endif
    #endif
  #endif
#endif

#if defined(__cplusplus)
  #ifdef GUCEF_VIRTUAL_OVERRIDE_IS_SUPPORTED
    #define GUCEF_VIRTUAL_OVERRIDE override
  #else
    #define GUCEF_VIRTUAL_OVERRIDE
  #endif
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-02-2007 :
       - Logan:  Added __TIMESTAMP__ macro
- 30-09-2006 :
       - Dinand: Moved the singleton macro's here from another macro file.
- 29-09-2006 :
       - Logan:  Trimmed tub of lard
- 29-09-2006 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/
