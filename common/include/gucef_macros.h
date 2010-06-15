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
 *      O/S Specific Switches
 */
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
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
#else
  #define GUCEF_CALLSPEC_STD_PREFIX __attribute__((GUCEF_CALLSPEC_STD))
  #define GUCEF_CALLSPEC_STD_SUFFIX
  #define GUCEF_PLUGIN_CALLSPEC_STD_PREFIX __attribute__((GUCEF_CALLSPEC_STD))
  #define GUCEF_PLUGIN_CALLSPEC_STD_SUFFIX
#endif

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

/* now we include some dynamicly linked module macro's */
#ifndef GUCEF_DLLINIT_H
#include "gucef_DLLInit.h"
#define GUCEF_DLLINIT_H
#endif /* GUCEF_DLLINIT_H ? */

/*-------------------------------------------------------------------------*/

/* GCC/G++ does not define __TIMESTAMP__ */

#ifndef __TIMESTAMP__
#define __TIMESTAMP__ __DATE__ " " __TIME__
#endif

/*-------------------------------------------------------------------------*/

#undef NULL
#ifndef __CPP09NULLPTRSUPPORTED
  #define NULL (0)
#else
  #define NULL (nullptr)
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
