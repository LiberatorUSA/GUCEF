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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* make sure no structure packing is enabled, this allows us to use the structures directly */
#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif /* WIN32 */

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
    Int16 major;
    Int16 minor;
    Int16 patch;
    Int16 release;
};

typedef struct SVersion TVersion;

/*--------------------------------------------------------------------------*/

struct SVersionRange
{
    TVersion min;
    UInt8 includeMin;
    TVersion max;
    UInt8 includeMax;
};

typedef struct SVersionRange TVersionRange;

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
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/* restore structure packing */
#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(pop)
#else
#pragma pack()
#endif /* WIN32 */

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
 