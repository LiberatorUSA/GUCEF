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

#ifndef GUCEF_CORE_VARIANTDATA_H
#include "gucefCORE_VariantData.h"
#define GUCEF_CORE_VARIANTDATA_H
#endif /* GUCEF_CORE_VARIANTDATA_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CDynamicBuffer;

/**
 *  Imlementation of a Variant class mainly used for interaction between 
 *  compile-time and run-time typed environments and/or between weakly and
 *  strongly typed environments.
 *
 *  Performance wise your gain or loss will depend on data used to store in the variant
 *  and whether you otherwise would have used a string to represent the same.
 *  Strings always use dynamic memory allocations whereas this is not always the case
 *  with a variant. Various operations such as comparison can potentially also be faster 
 *  depending on the type interactions involved.
 *
 *  TODO: Later support will be added for ExactFloat in which case a fidelity gain is an additional benefit
 */
class GUCEF_CORE_PUBLIC_CPP CVariant
{
    public:

    typedef std::vector< CVariant >     VariantVector;
    typedef std::set< CVariant >        VariantSet;

    static const CVariant   Empty;
    
    CVariant( void );
    
    /**
     *  Note that if the variant uses linked heap data said heap data will be copied
     *  to a privately owned copy to avoid hard to trace invalid pointer issues
     */
    CVariant( const CVariant& src );

    #ifdef GUCEF_RVALUE_REFERENCES_SUPPORTED
    CVariant( CVariant&& src ) GUCEF_NOEXCEPT;
    #endif
    
    ~CVariant(); 
    
    explicit CVariant( bool    data );
    explicit CVariant( Int8    data );
    explicit CVariant( UInt8   data ); 
    explicit CVariant( Int16   data );
    explicit CVariant( UInt16  data );
    explicit CVariant( Int32   data );
    explicit CVariant( UInt32  data );
    explicit CVariant( Int64   data );
    explicit CVariant( UInt64  data );
    explicit CVariant( Float32 data );
    explicit CVariant( Float64 data );
    
    /**
     *  Creates a private copy of the ASCII string data and sets the type as such
     */
    CVariant( const CAsciiString& data );

    /**
     *  Creates a private copy of the UTF8 string data and sets the type as such
     */
    CVariant( const CUtf8String& data );

    /**
     *  Creates a private copy of the assumed to be null terminated string data and sets the type as UTF8
     */
    CVariant( const char* utf8Str );

    /**
     *  Creates a private copy of the string data and sets the type as UTF8
     */
    CVariant( const std::string& utf8Str );

    CVariant( const void* data, UInt32 dataSize, UInt8 varType = GUCEF_DATATYPE_BINARY );
    
    bool IsInteger( void ) const;
    bool IsFloat( void ) const;
    bool IsString( void ) const;
    bool IsBoolean( void ) const;
    bool IsBinary( void ) const;
    
    static bool UsesDynamicMemory( UInt8 typeId );
    bool UsesDynamicMemory( void ) const;
    bool IsDynamicMemoryLinked( void ) const;
    bool OwnsDynamicMemory( void ) const;
    bool IsInitialized( void ) const;
    bool IsNULLOrEmpty( void ) const; 
    
    UInt8 GetTypeId( void ) const;  

    bool            AsBool( bool defaultIfNeeded = false, bool resolveVarsIfApplicable = false ) const;
    Int8            AsInt8( Int8 defaultIfNeeded = 0, bool resolveVarsIfApplicable = false ) const;
    UInt8           AsUInt8( UInt8 defaultIfNeeded = 0, bool resolveVarsIfApplicable = false ) const;
    Int16           AsInt16( Int16 defaultIfNeeded = 0, bool resolveVarsIfApplicable = false ) const;
    UInt16          AsUInt16( UInt16 defaultIfNeeded = 0, bool resolveVarsIfApplicable = false ) const;
    Int32           AsInt32( Int32 defaultIfNeeded = 0, bool resolveVarsIfApplicable = false ) const;
    UInt32          AsUInt32( UInt32 defaultIfNeeded = 0, bool resolveVarsIfApplicable = false ) const;
    Int64           AsInt64( Int64 defaultIfNeeded = 0, bool resolveVarsIfApplicable = false ) const;
    UInt64          AsUInt64( UInt64 defaultIfNeeded = 0, bool resolveVarsIfApplicable = false ) const;
    Float32         AsFloat32( Float32 defaultIfNeeded = 0.0f, bool resolveVarsIfApplicable = false ) const;
    Float64         AsFloat64( Float64 defaultIfNeeded = 0.0, bool resolveVarsIfApplicable = false ) const;
    CString         AsString( const CString& defaultIfNeeded = CString::Empty, bool resolveVarsIfApplicable = false ) const;
    CAsciiString    AsAsciiString( const CAsciiString& defaultIfNeeded = CAsciiString::Empty, bool resolveVarsIfApplicable = false ) const;
    CUtf8String     AsUtf8String( const CUtf8String& defaultIfNeeded = CUtf8String::Empty, bool resolveVarsIfApplicable = false ) const;
    const void*     AsVoidPtr( const void* defaultIfNeeded = GUCEF_NULL ) const;
    const char*     AsCharPtr( const char* defaultIfNeeded = GUCEF_NULL ) const;
    CDynamicBuffer  AsBuffer( void ) const;

    /**
     *  Returns the size of the storage used in bytes by the stored type
     */
    UInt32 ByteSize( bool includeNullTerm = true ) const;

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
    CVariant& operator=( Float32 data );
    CVariant& operator=( Float64 data );
    CVariant& operator=( const CAsciiString& data );
    CVariant& operator=( const CUtf8String& data );
    CVariant& operator=( const std::string& data );
    CVariant& operator=( const std::wstring& data );
    CVariant& operator=( const CVariant& src );

    operator CAsciiString() const;
    operator CUtf8String() const;

    void Clear( void );

    /**
     *  Sets the variant data based on a raw pointer to the data type of type
     *  indicated by dataSize. Please take care that the buffer pointed to by data
     *  has enough bytes to hold the varType indicated or the operation will fail.
     */
    bool Set( const void* data, UInt32 dataSize, UInt8 varType = GUCEF_DATATYPE_BINARY, bool linkOnlyForDynMem = false );

    bool SetString( UInt8 varType, const CString& data, const CVariant& defaultValue = CVariant::Empty );

    /**
     *  If you are setting a binary type using a string said string is assumed to be Base64 encoded
     */
    bool SetFromString( UInt8 varType, const CString& data, const CVariant& defaultValue = CVariant::Empty );

    const TVariantData* CStyleAccess( void ) const;

    CVariant& LinkTo( const CDynamicBuffer& src, UInt32 bufferOffset = 0, UInt8 varType = GUCEF_DATATYPE_BINARY, UInt32 bytesToLink = 0 );
    CVariant& LinkTo( const void* externalBuffer, UInt32 bufferSize );
    CVariant& LinkTo( const CVariant& src );
    CVariant& LinkTo( const CAsciiString& src );
    CVariant& LinkTo( const CUtf8String& src );
    CVariant& LinkTo( const std::string& src );
    CVariant& LinkTo( const std::wstring& src );
        
    CVariant& TransferOwnershipTo( CVariant& newOwner );

    protected:

    void* HeapReserve( UInt32 byteSize, bool allowReduction = false );

    TVariantData m_variantData;
};

/*-------------------------------------------------------------------------*/

inline CString ToString( const CVariant& var ) { return var.AsString(); }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CVARIANT_H ? */
