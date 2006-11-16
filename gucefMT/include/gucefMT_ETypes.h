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

#ifndef GUCEF_MT_ETYPES_H
#define GUCEF_MT_ETYPES_H

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
typedef enum TBuildinDataType
{
        DT_FLOAT32 = 1 ,
        DT_FLOAT64     ,
        DT_UINT8       ,
        DT_INT8        ,
        DT_UINT16      ,
        DT_INT16       ,
        DT_UINT32      ,
        DT_INT32       ,
        DT_UINT64      ,
        DT_INT64
};

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





