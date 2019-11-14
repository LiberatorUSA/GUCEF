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

typedef MT::TBuildinDataType TBuildinDataType;
typedef MT::TBasicDataType TBasicDataType;
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 30-09-2006 :
       - Dinand: Added this section

-----------------------------------------------------------------------------*/