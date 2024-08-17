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

#ifndef GUCEF_CPP_H
#define GUCEF_CPP_H

/*
 *  Macros to allow the GUCEF platform to use some newer C++ features while
 *  still supporting as old a compiler as possible for as wide a reach as possible
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
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

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
        #if __cplusplus > 201103L  /* >= C++ 11 : This is not fullproof since not every compiler truly supports the spec */
            #define GUCEF_RVALUE_REFERENCES_SUPPORTED 1
        #endif
    #endif
#endif

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_RVALUE_REFERENCES_SUPPORTED
    #define GUCEF_MOVE( m ) std::move( m )
#else
    #define GUCEF_MOVE( m ) m
#endif

/*-------------------------------------------------------------------------*/

#if defined(__cplusplus)
    #undef GUCEF_NOEXCEPT_IS_SUPPORTED
    #if __cplusplus > 201103L /* >= C++ 11 : This is not fullproof since not every compiler truly supports the spec */
        #if defined( __GNUG__ )  /* GNU C++ compiler */
            #if ( GCC_VERSION > 40600 )
                #define GUCEF_NOEXCEPT_IS_SUPPORTED 1
            #endif
        #else
            /* Rely on only cpp version check */
            #define GUCEF_NOEXCEPT_IS_SUPPORTED 1
        #endif
    #else /* some compilers still support it before official inclusion in C++11 */
        #if defined(__clang__)
            #if __has_feature(cxx_noexcept)
                #define GUCEF_NOEXCEPT_IS_SUPPORTED 1
            #endif
        #else
            #if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || \
                defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
                #define GUCEF_NOEXCEPT_IS_SUPPORTED 1
            #endif
        #endif
    #endif
#endif

#if defined(__cplusplus)
  #ifdef GUCEF_NOEXCEPT_IS_SUPPORTED
    #define GUCEF_NOEXCEPT noexcept( true )
  #else
    #define GUCEF_NOEXCEPT
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

#if defined(__cplusplus)
    
    #if __cplusplus >= 199711L // type_traits header was added in C++11
      #include <type_traits>
    #else

    // backfill for C++11 type_traits
    namespace std {

    // Base template for is_signed trait
    template <typename T>
    struct is_signed {
        static const bool value = false;
    };

    // Specializations for signed types
    template <>
    struct is_signed<signed char> {
        static const bool value = true;
    };

    template <>
    struct is_signed<short> {
        static const bool value = true;
    };

    template <>
    struct is_signed<int> {
        static const bool value = true;
    };

    template <>
    struct is_signed<long> {
        static const bool value = true;
    };

    // Base template for is_unsigned trait
    template <typename T>
    struct is_unsigned {
        static const bool value = false;
    };

    // Specializations for unsigned types
    template <>
    struct is_unsigned<unsigned char> {
        static const bool value = true;
    };

    template <>
    struct is_unsigned<unsigned short> {
        static const bool value = true;
    };

    template <>
    struct is_unsigned<unsigned int> {
        static const bool value = true;
    };

    template <>
    struct is_unsigned<unsigned long> {
        static const bool value = true;
    };

    }; // namespace std
    
    #endif

#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CPP_H ? */

