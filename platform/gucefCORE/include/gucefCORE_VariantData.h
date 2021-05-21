/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_VARIANTDATA_H
#define GUCEF_CORE_VARIANTDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

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
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SHeapData
{
    void* heap_data;                /**< pointer to the block of heap memory */
    UInt32 heap_data_size;          /**< size of the referenced memory block in bytes */
    UInt8 heap_data_is_linked;      /**< 1 or 0 flag to denote whether the heap data is owned by the variant and thus would need to be deleted or whether its merely linked as a reference */
};
typedef struct SHeapData THeapData;

struct SVariantData
{
    UInt8 containedType;
    union UnionType
    {
        Int8        int8_data;
        UInt8       uint8_data; 
        Int16       int16_data;
        UInt16      uint16_data;
        Int32       int32_data;
        UInt32      uint32_data;
        Int64       int64_data;
        UInt64      uint64_data;
        Float32     float32_data;
        Float64     float64_data;
        THeapData   heap_data;
    } union_data;
};
typedef struct SVariantData TVariantData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_VARIANTDATA_H ? */
