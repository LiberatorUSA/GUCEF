/*
 *  gucefMT: GUCEF module providing multithreading solutions
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

#ifndef GUCEF_MT_ETYPES_H
#define GUCEF_MT_ETYPES_H

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  Legacy remap - deprecated
 */
#define GUCEF_MT_INT8MAX     GUCEF_INT8MAX
#define GUCEF_MT_INT8MIN     GUCEF_INT8MIN
#define GUCEF_MT_UINT8MAX    GUCEF_UINT8MAX
#define GUCEF_MT_UINT8MIN    GUCEF_UINT8MIN
#define GUCEF_MT_INT16MAX    GUCEF_INT16MAX
#define GUCEF_MT_INT16MIN    GUCEF_INT16MIN
#define GUCEF_MT_UINT16MAX   GUCEF_UINT16MAX
#define GUCEF_MT_UINT16MIN   GUCEF_UINT16MIN
#define GUCEF_MT_INT32MAX    GUCEF_INT32MAX
#define GUCEF_MT_INT32MIN    GUCEF_INT32MIN
#define GUCEF_MT_UINT32MAX   GUCEF_UINT32MAX
#define GUCEF_MT_UINT32MIN   GUCEF_UINT32MIN
#define GUCEF_MT_INT64MIN    GUCEF_INT64MIN
#define GUCEF_MT_INT64MAX    GUCEF_INT64MAX
#define GUCEF_MT_UINT64MIN   GUCEF_UINT64MIN
#define GUCEF_MT_UINT64MAX   GUCEF_UINT64MAX
#define GUCEF_MT_FLOAT32MAX  GUCEF_FLOAT32MAX
#define GUCEF_MT_FLOAT32MIN  GUCEF_FLOAT32MIN

/*
 *  Legacy remap - deprecated
 */
#define GUCEFMT_INT8MAX     GUCEF_MT_INT8MAX
#define GUCEFMT_INT8MIN     GUCEF_MT_INT8MIN
#define GUCEFMT_UINT8MAX    GUCEF_MT_UINT8MAX
#define GUCEFMT_UINT8MIN    GUCEF_MT_UINT8MIN
#define GUCEFMT_INT16MAX    GUCEF_MT_INT16MAX
#define GUCEFMT_INT16MIN    GUCEF_MT_INT16MIN
#define GUCEFMT_UINT16MAX   GUCEF_MT_UINT16MAX
#define GUCEFMT_UINT16MIN   GUCEF_MT_UINT16MIN
#define GUCEFMT_INT32MAX    GUCEF_MT_INT32MAX
#define GUCEFMT_INT32MIN    GUCEF_MT_INT32MIN
#define GUCEFMT_UINT32MAX   GUCEF_MT_UINT32MAX
#define GUCEFMT_UINT32MIN   GUCEF_MT_UINT32MIN
#define GUCEFMT_FLOAT32MAX  GUCEF_MT_FLOAT32MAX
#define GUCEFMT_FLOAT32MIN  GUCEF_MT_FLOAT32MIN

/*--------------------------------------------------------------------------*/

/*
 *  We only have to define types when using C++ due to namespacing 
 *  For C the gucefMT versions are automaticly used in the global namespace
 */
#ifdef __cplusplus
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  Legacy remap - deprecated
 */

typedef ::GUCEF::UInt8 UInt8;    /* 1 byte, unsigned */
typedef ::GUCEF::Int8 Int8;      /* 1 byte, signed */

typedef ::GUCEF::UInt16 UInt16;   /* 2 bytes, unsigned */
typedef ::GUCEF::Int16 Int16;     /* 2 bytes, signed */

typedef ::GUCEF::UInt32 UInt32;   /* 4 bytes, unsigned */
typedef ::GUCEF::Int32  Int32;    /* 4 bytes, signed */

typedef ::GUCEF::Int64 Int64;     /* 8 bytes, signed */
typedef ::GUCEF::UInt64 UInt64;   /* 8 bytes, unsigned */

typedef ::GUCEF::Float32 Float32;  /* 4 bytes, signed, decimal */
typedef ::GUCEF::Float64 Float64;  /* 8 bytes, signed, decimal */

typedef ::GUCEF::TBuildinDataType TBuildinDataType;
typedef ::GUCEF::TBasicDataType TBasicDataType;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_ETYPES_H ? */





