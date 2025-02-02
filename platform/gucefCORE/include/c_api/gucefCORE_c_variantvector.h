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

#ifndef GUCEF_CORE_C_VARIANTVECTOR_H
#define GUCEF_CORE_C_VARIANTVECTOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_C_VARIANTDATA_H
#include "gucefCORE_c_variantdata.h"
#define GUCEF_CORE_C_VARIANTDATA_H
#endif /* GUCEF_CORE_C_VARIANTDATA_H ? */

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

typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_push_back ) ( void* privdata, TVariantData* data, UInt8 linkIfPossible ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_clear ) ( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef size_t ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_size ) ( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_back ) ( void* privdata, TVariantData* back, UInt8 linkIfPossible ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_front ) ( void* privdata, TVariantData* front, UInt8 linkIfPossible ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_pop_back ) ( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_at ) ( void* privdata, UInt32 index, TVariantData* entryData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

typedef Int8 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_is_const ) ( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt8 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_type_id_of_elements ) ( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TVariantVectorFunc_byte_size_of_element_type ) ( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
    
/*-------------------------------------------------------------------------*/

/**
 *  Structure that hold all the function pointers to the event handlers
 *  for read events. Should be called by plugins as needed.
 */
struct SVariantVectorApi
{
    /**
     *  The following are function pointers for functions which mimic the std::vector
     */
    TVariantVectorFunc_push_back    push_back;
    TVariantVectorFunc_clear        clear;
    TVariantVectorFunc_size         size;
    TVariantVectorFunc_back         back;
    TVariantVectorFunc_front        front;
    TVariantVectorFunc_pop_back     pop_back; 
    TVariantVectorFunc_at           at;

    /**
     *  The following are function pointers for functions which are custom helpers
     *  keeping in mind that not information is as easily available across the C API boundary
     */
    TVariantVectorFunc_is_const                     is_const;
    TVariantVectorFunc_type_id_of_elements          type_id_of_elements;
    TVariantVectorFunc_byte_size_of_element_type    byte_size_of_element_type;
    
    void* privateData;
};
typedef struct SVariantVectorApi TVariantVectorApi;

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

#endif /* GUCEF_CORE_C_VARIANTVECTOR_H ? */
