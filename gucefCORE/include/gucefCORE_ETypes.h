/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_ETYPES_H
#define GUCEF_CORE_ETYPES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Maximum and minimal values for the simple types which we are about to
 *      define.
 */
#define GUCEFCORE_INT8MAX     GUCEFMT_INT8MAX
#define GUCEFCORE_INT8MIN     GUCEFMT_INT8MIN
#define GUCEFCORE_UINT8MAX    GUCEFMT_UINT8MAX
#define GUCEFCORE_UINT8MIN    GUCEFMT_UINT8MIN
#define GUCEFCORE_INT16MAX    GUCEFMT_INT16MAX
#define GUCEFCORE_INT16MIN    GUCEFMT_INT16MIN
#define GUCEFCORE_UINT16MAX   GUCEFMT_UINT16MAX
#define GUCEFCORE_UINT16MIN   GUCEFMT_UINT16MIN
#define GUCEFCORE_INT32MAX    GUCEFMT_INT32MAX
#define GUCEFCORE_INT32MIN    GUCEFMT_INT32MIN
#define GUCEFCORE_UINT32MAX   GUCEFMT_UINT32MAX
#define GUCEFCORE_UINT32MIN   GUCEFMT_UINT32MIN
#define GUCEFCORE_FLOAT32MAX  GUCEFMT_FLOAT32MAX
#define GUCEFCORE_FLOAT32MIN  GUCEFMT_FLOAT32MIN

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
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef MT::UInt8 UInt8;    /* 1 byte, unsigned */
typedef MT::Int8 Int8;      /* 1 byte, signed */

typedef MT::UInt16 UInt16;   /* 2 bytes, unsigned */
typedef MT::Int16 Int16;     /* 2 bytes, signed */

typedef MT::UInt32 UInt32;   /* 4 bytes, unsigned */
typedef MT::Int32  Int32;    /* 4 bytes, signed */

typedef MT::Int64 Int64;     /* 8 bytes, signed */
typedef MT::UInt64 UInt64;   /* 8 bytes, unsigned */

typedef MT::Float32 Float32;  /* 4 bytes, signed, decimal */
typedef MT::Float64 Float64;  /* 8 bytes, signed, decimal */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* __cplusplus ? */

#endif /* GUCEF_CORE_ETYPES_H ? */





