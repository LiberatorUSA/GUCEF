/*
 *  pubsubpluginREDISCLUSTER: Generic GUCEF COMCORE plugin for providing pubsub via Redis
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

#ifndef PUBSUBPLUGIN_REDISCLUSTER_MACROS_H
#define PUBSUBPLUGIN_REDISCLUSTER_MACROS_H

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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"      
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef PUBSUBPLUGIN_REDISCLUSTER_CONFIG_H
#include "pubsubpluginREDISCLUSTER_config.h"
#define PUBSUBPLUGIN_REDISCLUSTER_CONFIG_H
#endif /* PUBSUBPLUGIN_REDISCLUSTER_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#undef PUBSUBPLUGIN_REDISCLUSTER_EXPORT
#ifdef PUBSUBPLUGIN_REDISCLUSTER_BUILD_MODULE
  #define PUBSUBPLUGIN_REDISCLUSTER_EXPORT GUCEF_EXPORT
#else
  #define PUBSUBPLUGIN_REDISCLUSTER_EXPORT GUCEF_IMPORT
#endif /* PUBSUBPLUGIN_REDISCLUSTER_BUILD_MODULE ? */

#undef PUBSUBPLUGIN_REDISCLUSTER_EXPORT_CPP
#ifdef PUBSUBPLUGIN_REDISCLUSTER_EXPORT_CPP_CODE
  #define PUBSUBPLUGIN_REDISCLUSTER_EXPORT_CPP PUBSUBPLUGIN_REDISCLUSTER_EXPORT
#else
  #define PUBSUBPLUGIN_REDISCLUSTER_EXPORT_CPP
#endif /* PUBSUBPLUGIN_REDISCLUSTER_EXPORT_CPP_CODE */

#undef PUBSUBPLUGIN_REDISCLUSTER_EXPORT_C
#ifdef PUBSUBPLUGIN_REDISCLUSTER_EXPORT_C_CODE
  #define PUBSUBPLUGIN_REDISCLUSTER_EXPORT_C PUBSUBPLUGIN_REDISCLUSTER_EXPORT
#else
  #define PUBSUBPLUGIN_REDISCLUSTER_EXPORT_C
#endif /* PUBSUBPLUGIN_REDISCLUSTER_EXPORT_C_CODE */


/*-------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_REDISCLUSTER_MACROS_H ? */

