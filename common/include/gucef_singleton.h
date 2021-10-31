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

#ifndef GUCEF_SINGLETON_H
#define GUCEF_SINGLETON_H

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

#ifndef GUCEF_CALLCONV_H
#include "gucef_callconv.h"      /* GUCEF platform calling convention macros */
#define GUCEF_CALLCONV_H
#endif /* GUCEF_CALLCONV_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

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

#endif /* GUCEF_SINGLETON_H ? */

