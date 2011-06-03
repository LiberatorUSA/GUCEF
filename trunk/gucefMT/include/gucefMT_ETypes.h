/*
 *  gucefMT: GUCEF module providing multithreading solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef GUCEF_MT_ETYPES_H
#define GUCEF_MT_ETYPES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PLATFORM_H
#include "gucef_platform.h"
#define GUCEF_PLATFORM_H
#endif /* GUCEF_PLATFORM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Maximum and minimal values for the simple types which we are about to
 *      define.
 */
#define GUCEFMT_INT8MAX     127
#define GUCEFMT_INT8MIN     (-128)
#define GUCEFMT_UINT8MAX    256
#define GUCEFMT_UINT8MIN    0
#define GUCEFMT_INT16MAX    32767
#define GUCEFMT_INT16MIN    (-32767-1)
#define GUCEFMT_UINT16MAX   65535
#define GUCEFMT_UINT16MIN   0
#define GUCEFMT_INT32MAX    2147483647
#define GUCEFMT_INT32MIN    (-2147483647-1)
#define GUCEFMT_UINT32MAX   4294967295
#define GUCEFMT_UINT32MIN   0
#define GUCEFMT_FLOAT32MAX  (3.40282347E+38F)
#define GUCEFMT_FLOAT32MIN  (-3.40282347E+38F-1)

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace MT {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Definitions for types used in the engine code
 */
#ifndef Byte
typedef signed char     Byte;     /* 1 byte, signed */
#endif

typedef unsigned char   Ubyte;    /* 1 byte, unsigned */

#undef UByte
#define UByte           Ubyte

typedef unsigned char   UInt8;    /* 1 byte, unsigned */
typedef signed char     Int8;     /* 1 byte, signed */

typedef unsigned short  UInt16;   /* 2 bytes, unsigned */
typedef signed short    Int16;    /* 2 bytes, signed */

#if defined(__alpha) || (defined(_MIPS_SZLONG) && _MIPS_SZLONG == 64) || defined(__LP64__)
typedef unsigned int    UInt32;   /* 4 bytes, unsigned */
typedef signed int      Int32;    /* 4 bytes, signed */
#else
typedef unsigned long   UInt32;   /* 4 bytes, unsigned */
typedef signed long     Int32;    /* 4 bytes, signed */
#endif

#ifdef __GNUC__
typedef signed long long   Int64;   /* 8 bytes, signed */
typedef unsigned long long UInt64;  /* 8 bytes, unsigned */
#else
typedef signed __int64   Int64;   /* 8 bytes, signed */
typedef unsigned __int64 UInt64;  /* 8 bytes, unsigned */
#endif

typedef float           Float32;  /* 4 bytes, signed, decimal */
typedef double          Float64;  /* 8 bytes, signed, decimal */

/*--------------------------------------------------------------------------*/

/**
 *      Build-in data types as part of an enumeration set
 */
enum EBuildinDataType
{
        DATATYPE_FLOAT32 = 1 ,
        DATATYPE_FLOAT64     ,
        DATATYPE_UINT8       ,
        DATATYPE_INT8        ,
        DATATYPE_UINT16      ,
        DATATYPE_INT16       ,
        DATATYPE_UINT32      ,
        DATATYPE_INT32       ,
        DATATYPE_UINT64      ,
        DATATYPE_INT64       ,

        DATATYPE_UNKNOWN
};
typedef enum EBuildinDataType TBuildinDataType;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace MT */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_ETYPES_H ? */





