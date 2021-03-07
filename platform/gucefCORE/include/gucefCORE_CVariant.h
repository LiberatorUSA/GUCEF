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

#ifndef GUCEF_CORE_CVARIANT_H
#define GUCEF_CORE_CVARIANT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CITIME_H
#include "gucefCORE_CITime.h"
#define GUCEF_CORE_CITIME_H
#endif /* GUCEF_CORE_CITIME_H ? */

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

struct SHeapData
{
    void* heap_data;
    UInt64 heap_data_size; 
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
        Int64       int64_data;
        UInt64      uint64_data;
        Float32     float32_data;
        Float64     float64_data;
        THeapData   heap_data;
    };
    UnionType union_data;
};
typedef struct SVariantData TVariantData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CVariant
{
    public:

    CVariant( void );
    CVariant( const CVariant& src );
    ~CVariant(); 

    CVariant& operator=( const CVariant& src );

    explicit CVariant( bool    data );
    explicit CVariant( Int8    data );
    explicit CVariant( UInt8   data ); 
    explicit CVariant( Int16   data );
    explicit CVariant( UInt16  data );
    explicit CVariant( Int32   data );
    explicit CVariant( UInt32  data );
    explicit CVariant( Int64   data );
    explicit CVariant( UInt64  data );
    explicit CVariant( Int64   data );
    explicit CVariant( UInt64  data );
    explicit CVariant( Float32 data );
    explicit CVariant( Float64 data );
    
    bool IsInteger( void ) const;
    bool IsFloat( void ) const;
    bool IsString( void ) const;
    bool IsBoolean( void ) const;
    
    bool UsesDynamicMemory( void ) const;
    bool IsInitialized( void ) const; 
    
    UInt8 GetTypeId( void ) const;  

    bool     AsBool( void ) const;
    Int8     AsInt8( void ) const;
    UInt8    AsUInt8( void ) const;
    Int16    AsInt16( void ) const;
    UInt16   AsUInt16( void ) const;
    Int32    AsInt32( void ) const;
    UInt32   AsUInt32( void ) const;
    Int64    AsInt64( void ) const;
    UInt64   AsUInt64( void ) const;
    Float32  AsFloat32( void ) const;
    Float64  AsFloat64( void ) const;

    bool operator==( const CVariant& other ) const;
    bool operator!=( const CVariant& other ) const;
    bool operator<( const CVariant& other ) const;
    bool operator>( const CVariant& other ) const;
    bool operator<=( const CVariant& other ) const;
    bool operator>=( const CVariant& other ) const;

    CVariant& operator=( bool    data );
    CVariant& operator=( Int8    data );
    CVariant& operator=( UInt8   data ); 
    CVariant& operator=( Int16   data );
    CVariant& operator=( UInt16  data );
    CVariant& operator=( Int32   data );
    CVariant& operator=( UInt32  data );
    CVariant& operator=( Int64   data );
    CVariant& operator=( UInt64  data );
    CVariant& operator=( Int64   data );
    CVariant& operator=( UInt64  data );
    CVariant& operator=( Float32 data );
    CVariant& operator=( Float64 data );

    void Clear( void );

    bool Set( UInt8 varType, const void* data, UInt32 dataSize );
    
    protected:

    TVariantData m_variantData;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CVARIANT_H ? */
