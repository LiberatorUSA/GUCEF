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

/*
 *      @file
 *      Storage structure now defined in a seperate file so it can be used for
 *      both C and C++ modules.
 */

#ifndef GUCEF_CORE_ESTRUCTS_H
#define GUCEF_CORE_ESTRUCTS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"                     /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace CORE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SArea2D
{
        Float32 min[ 2 ];  /* X, Y */
        Float32 max[ 2 ];  /* X, Y */
};

typedef struct SArea2D TArea2D;

/*--------------------------------------------------------------------------*/

struct SArea3D
{
        Float32 min[ 3 ];  /* X, Y, Z */
        Float32 max[ 3 ];  /* X, Y, Z */
};

typedef struct SArea3D TArea3D;

/*--------------------------------------------------------------------------*/

struct SSize3D
{
        Float32 width;
        Float32 height;
        Float32 depth;
};

typedef struct SSize3D TSize3D;

/*--------------------------------------------------------------------------*/

struct SVersion
{
        UInt16 mayor;
        UInt16 minor;
        UInt16 patch;
        UInt16 release;
};

typedef struct SVersion TVersion;

/*--------------------------------------------------------------------------*/

struct SPoint2DI
{
        Int32 x;
        Int32 y;
};

typedef struct SPoint2DI TPoint2DI;

/*--------------------------------------------------------------------------*/

struct SPoint2DF
{
        Float32 x;
        Float32 y;
};

typedef struct SPoint2DF TPoint2DF;

/*--------------------------------------------------------------------------*/

struct SPoint3DI
{
        Int32 x;
        Int32 y;
        Int32 z;
};

typedef struct SPoint3DI TPoint3DI;

/*--------------------------------------------------------------------------*/

struct SPoint3DF
{
        Float32 x;
        Float32 y;
        Float32 z;
};

typedef struct SPoint3DF TPoint3DF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_ESTRUCTS_H ? */
 