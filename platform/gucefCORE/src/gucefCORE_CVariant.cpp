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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#include "gucefCORE_CVariant.h"

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CVariant CVariant::Empty;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CVariant::CVariant( void )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_UNKNOWN;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( bool data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_BOOLEAN_INT32;
    m_variantData.union_data.int32_data = data ? 1 : 0;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( Int8 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_INT8;
    m_variantData.union_data.int8_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( UInt8 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_UINT8;
    m_variantData.union_data.uint8_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( Int16 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_INT16;
    m_variantData.union_data.int16_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( UInt16 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_UINT16;
    m_variantData.union_data.uint16_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( Int32 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_INT32;
    m_variantData.union_data.int32_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( UInt32 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_UINT32;
    m_variantData.union_data.uint32_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( Int64 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_INT64;
    m_variantData.union_data.int64_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( UInt64 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_UINT64;
    m_variantData.union_data.uint64_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( Float32 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_FLOAT32;
    m_variantData.union_data.float32_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( Float64 data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    m_variantData.containedType = GUCEF_DATATYPE_FLOAT64;
    m_variantData.union_data.float64_data = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( const CAsciiString& data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    *this = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( const CUtf8String& data )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    *this = data;
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( const char* utf8Str )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    *this = CUtf8String( utf8Str );
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( const std::string& utf8Str )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    *this = CUtf8String( utf8Str );
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( const void* data, UInt32 dataSize, UInt8 varType )
    : m_variantData()
{GUCEF_TRACE;

    memset( &m_variantData, 0, sizeof( m_variantData ) );
    Set( data, dataSize, varType );
}

/*-------------------------------------------------------------------------*/

CVariant::CVariant( const CVariant& data )
    : m_variantData( data.m_variantData )
{GUCEF_TRACE;

    // If dynamic memory is used we need to actually copy said memory
    // into a private copy
    if ( data.UsesDynamicMemory() )
    {
        if ( 0 < data.m_variantData.union_data.heap_data.heap_data_size && GUCEF_NULL != data.m_variantData.union_data.heap_data.union_data.void_heap_data )
        {
            m_variantData.union_data.heap_data.union_data.void_heap_data = malloc( (size_t) data.m_variantData.union_data.heap_data.heap_data_size );
            assert( GUCEF_NULL != m_variantData.union_data.heap_data.union_data.void_heap_data );
            memcpy( m_variantData.union_data.heap_data.union_data.void_heap_data, data.m_variantData.union_data.heap_data.union_data.void_heap_data, (size_t) data.m_variantData.union_data.heap_data.heap_data_size );
        }
        else
        {
            m_variantData.union_data.heap_data.union_data.void_heap_data = GUCEF_NULL;
            m_variantData.union_data.heap_data.heap_data_size = 0;
        }
        m_variantData.union_data.heap_data.heap_data_is_linked = 0;
    }
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_RVALUE_REFERENCES_SUPPORTED
CVariant::CVariant( CVariant&& src ) GUCEF_NOEXCEPT
    : m_variantData( src.m_variantData )
{GUCEF_TRACE;

    memset( &src.m_variantData, 0, sizeof( src.m_variantData ) );
}
#endif

/*-------------------------------------------------------------------------*/

bool
CVariant::IsInteger( void ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8:
        case GUCEF_DATATYPE_UINT8:
        case GUCEF_DATATYPE_INT16:
        case GUCEF_DATATYPE_UINT16:
        case GUCEF_DATATYPE_INT32:
        case GUCEF_DATATYPE_UINT32:
        case GUCEF_DATATYPE_INT64:
        case GUCEF_DATATYPE_UINT64:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::IsFloat( void ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_FLOAT32:
        case GUCEF_DATATYPE_FLOAT64:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::IsString( void ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        case GUCEF_DATATYPE_UTF16_STRING:
        case GUCEF_DATATYPE_UTF32_STRING:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::IsBoolean( void ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_BOOLEAN_INT32:
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::IsBinary( void ) const
{GUCEF_TRACE;

    return m_variantData.containedType == GUCEF_DATATYPE_BINARY;
}

/*-------------------------------------------------------------------------*/

bool
CVariant::UsesDynamicMemory( UInt8 typeId )
{GUCEF_TRACE;

    switch ( typeId )
    {
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        case GUCEF_DATATYPE_UTF16_STRING:
        case GUCEF_DATATYPE_UTF32_STRING:
        case GUCEF_DATATYPE_BINARY:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::UsesDynamicMemory( void ) const
{GUCEF_TRACE;

    return UsesDynamicMemory( m_variantData.containedType );
}

/*-------------------------------------------------------------------------*/

bool
CVariant::IsDynamicMemoryLinked( void ) const
{GUCEF_TRACE;

    if ( UsesDynamicMemory() )
    {
        return 0 != m_variantData.union_data.heap_data.heap_data_is_linked;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVariant::OwnsDynamicMemory( void ) const
{GUCEF_TRACE;

    if ( UsesDynamicMemory() )
    {
        return 0 == m_variantData.union_data.heap_data.heap_data_is_linked;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVariant::IsInitialized( void ) const
{GUCEF_TRACE;

    return GUCEF_DATATYPE_UNKNOWN != m_variantData.containedType;
}

/*-------------------------------------------------------------------------*/

bool
CVariant::IsNULLOrEmpty( void ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        // Specifically for strings we check for 0 length or null terminator as well
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        {
            return 0 == m_variantData.union_data.heap_data.heap_data_size ||
                   ( 1 == m_variantData.union_data.heap_data.heap_data_size && '\0' == *( (const char*) m_variantData.union_data.heap_data.union_data.char_heap_data ) );
        }
        case GUCEF_DATATYPE_UTF16_STRING:
        {
            return 0 == m_variantData.union_data.heap_data.heap_data_size ||
                   ( 2 == m_variantData.union_data.heap_data.heap_data_size && 0 == *( (const UInt16*) m_variantData.union_data.heap_data.union_data.char_heap_data ) );
        }
        case GUCEF_DATATYPE_UTF32_STRING:
        {
            return 0 == m_variantData.union_data.heap_data.heap_data_size ||
                   ( 4 == m_variantData.union_data.heap_data.heap_data_size && 0 == *( (const UInt32*) m_variantData.union_data.heap_data.union_data.char_heap_data ) );
        }
        case GUCEF_DATATYPE_UNKNOWN: return true;
        default: return false;
    }
}

/*-------------------------------------------------------------------------*/

UInt8
CVariant::GetTypeId( void ) const
{GUCEF_TRACE;

    return m_variantData.containedType;
}

/*-------------------------------------------------------------------------*/

bool
CVariant::AsBool( bool defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_BOOLEAN_INT32:
            return 0 != m_variantData.union_data.int32_data;

        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        {
            if ( resolveVarsIfApplicable )
                return StringToBool( AsString( CString::Empty, resolveVarsIfApplicable ), defaultIfNeeded );
            else
                return StringToBool( AsString(), defaultIfNeeded );
        }

        default:
        {
            if ( IsInteger() )
                return 0 != AsUInt64();
            if ( IsFloat() )
                return 0 != AsFloat64();

            return defaultIfNeeded;
        }
    }
}

/*-------------------------------------------------------------------------*/

Int8
CVariant::AsInt8( Int8 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (Int8) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (Int8) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (Int8) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (Int8) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (Int8) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return (Int8) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (Int8) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (Int8) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (Int8) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (Int8) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (Int8) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (Int8) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_ASCII_STRING: return (Int8) ( resolveVarsIfApplicable ? StringToInt8( AsString(), defaultIfNeeded ) : StringToInt8( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (Int8) ( resolveVarsIfApplicable ? StringToInt8( AsString(), defaultIfNeeded ) : StringToInt8( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

UInt8
CVariant::AsUInt8( UInt8 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (UInt8) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (UInt8) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (UInt8) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (UInt8) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (UInt8) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return (UInt8) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (UInt8) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (UInt8) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (UInt8) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (UInt8) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (UInt8) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (UInt8) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_ASCII_STRING: return (UInt8) ( resolveVarsIfApplicable ? StringToUInt8( AsString(), defaultIfNeeded ) : StringToUInt8( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (UInt8) ( resolveVarsIfApplicable ? StringToUInt8( AsString(), defaultIfNeeded ) : StringToUInt8( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

Int16
CVariant::AsInt16( Int16 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (Int16) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (Int16) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (Int16) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (Int16) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (Int16) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return (Int16) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (Int16) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (Int16) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (Int16) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (Int16) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (Int16) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (Int16) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_ASCII_STRING: return (Int16) ( resolveVarsIfApplicable ? StringToInt16( AsString(), defaultIfNeeded ) : StringToInt16( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (Int16) ( resolveVarsIfApplicable ? StringToInt16( AsString(), defaultIfNeeded ) : StringToInt16( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

UInt16
CVariant::AsUInt16( UInt16 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (UInt16) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (UInt16) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (UInt16) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (UInt16) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (UInt16) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return (UInt16) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (UInt16) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (UInt16) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (UInt16) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (UInt16) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (UInt16) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (UInt16) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_ASCII_STRING: return (UInt16) ( resolveVarsIfApplicable ? StringToUInt16( AsString(), defaultIfNeeded ) : StringToUInt16( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (UInt16) ( resolveVarsIfApplicable ? StringToUInt16( AsString(), defaultIfNeeded ) : StringToUInt16( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

Int32
CVariant::AsInt32( Int32 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (Int32) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (Int32) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (Int32) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (Int32) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (Int32) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return (Int32) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (Int32) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (Int32) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (Int32) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (Int32) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (Int32) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (Int32) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_ASCII_STRING: return (Int32) ( resolveVarsIfApplicable ? StringToInt32( AsString(), defaultIfNeeded ) : StringToInt32( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (Int32) ( resolveVarsIfApplicable ? StringToInt32( AsString(), defaultIfNeeded ) : StringToInt32( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CVariant::AsUInt32( UInt32 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (UInt32) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (UInt32) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (UInt32) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (UInt32) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (UInt32) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return (UInt32) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (UInt32) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (UInt32) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (UInt32) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (UInt32) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (UInt32) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (UInt32) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_ASCII_STRING: return (UInt32) ( resolveVarsIfApplicable ? StringToUInt32( AsString(), defaultIfNeeded ) : StringToUInt32( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (UInt32) ( resolveVarsIfApplicable ? StringToUInt32( AsString(), defaultIfNeeded ) : StringToUInt32( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

Int64
CVariant::AsInt64( Int64 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (Int64) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (Int64) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (Int64) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (Int64) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (Int64) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (Int64) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (Int64) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (Int64) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (Int64) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (Int64) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (Int64) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (Int64) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_ASCII_STRING: return (Int64) ( resolveVarsIfApplicable ? StringToInt64( AsString(), defaultIfNeeded ) : StringToInt64( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (Int64) ( resolveVarsIfApplicable ? StringToInt64( AsString(), defaultIfNeeded ) : StringToInt64( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

UInt64
CVariant::AsUInt64( UInt64 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (UInt64) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (UInt64) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (UInt64) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (UInt64) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (UInt64) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (UInt64) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return (UInt64) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (UInt64) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (UInt64) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (UInt64) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (UInt64) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (UInt64) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1 : 0 ) : AsBool() ? 1 : 0 );
        case GUCEF_DATATYPE_ASCII_STRING: return (UInt64) ( resolveVarsIfApplicable ? StringToUInt64( AsString(), defaultIfNeeded ) : StringToUInt64( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (UInt64) ( resolveVarsIfApplicable ? StringToUInt64( AsString(), defaultIfNeeded ) : StringToUInt64( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

Float32
CVariant::AsFloat32( Float32 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (Float32) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (Float32) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (Float32) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (Float32) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (Float32) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (Float32) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return  (Float32) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (Float32) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return (Float32) m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (Float32) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (Float32) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1.0f : 0.0f ) : AsBool() ? 1.0f : 0.0f );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (Float32) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1.0f : 0.0f ) : AsBool() ? 1.0f : 0.0f );
        case GUCEF_DATATYPE_ASCII_STRING: return (Float32) ( resolveVarsIfApplicable ? StringToFloat( AsString(), defaultIfNeeded ) : StringToFloat( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (Float32) ( resolveVarsIfApplicable ? StringToFloat( AsString(), defaultIfNeeded ) : StringToFloat( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

Float64
CVariant::AsFloat64( Float64 defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return (Float64) m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return (Float64) m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return (Float64) m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return (Float64) m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return (Float64) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return (Float64) m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return  (Float64) m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return (Float64) m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return (Float64) m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return (Float64) m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return (Float32) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1.0l : 0.0l ) : AsBool() ? 1.0l : 0.0l );
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return (Float32) ( resolveVarsIfApplicable ? ( StringToBool( ResolveVars( AsString() ) ) ? 1.0l : 0.0l ) : AsBool() ? 1.0l : 0.0l );
        case GUCEF_DATATYPE_ASCII_STRING: return (Float32) ( resolveVarsIfApplicable ? StringToDouble( AsString(), defaultIfNeeded ) : StringToDouble( AsString(), defaultIfNeeded ) );
        case GUCEF_DATATYPE_UTF8_STRING: return (Float32) ( resolveVarsIfApplicable ? StringToDouble( AsString(), defaultIfNeeded ) : StringToDouble( AsString(), defaultIfNeeded ) );
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

const char*
CVariant::AsCharPtr( const char* defaultIfNeeded ) const
{GUCEF_TRACE;

    return static_cast< const char* >( AsVoidPtr(  defaultIfNeeded ) );
}

/*-------------------------------------------------------------------------*/

char            
CVariant::AsChar( char defaultIfNeeded ) const
{GUCEF_TRACE;

    return *AsCharPtr( &defaultIfNeeded );
}

/*-------------------------------------------------------------------------*/

const void*
CVariant::AsVoidPtr( const void* defaultIfNeeded ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return &m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return &m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return &m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return &m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return &m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return &m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return &m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return &m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return &m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return &m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return &m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING: return m_variantData.union_data.heap_data.union_data.char_heap_data;
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING: return m_variantData.union_data.heap_data.union_data.char_heap_data;
        case GUCEF_DATATYPE_ASCII_STRING: return m_variantData.union_data.heap_data.union_data.char_heap_data;
        case GUCEF_DATATYPE_UTF8_STRING: return m_variantData.union_data.heap_data.union_data.char_heap_data;
        case GUCEF_DATATYPE_BINARY: return m_variantData.union_data.heap_data.union_data.void_heap_data;
        default: return defaultIfNeeded;
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CVariant::ByteSize( bool includeNullTerm ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_INT8: return sizeof m_variantData.union_data.int8_data;
        case GUCEF_DATATYPE_UINT8: return sizeof m_variantData.union_data.uint8_data;
        case GUCEF_DATATYPE_INT16: return sizeof m_variantData.union_data.int16_data;
        case GUCEF_DATATYPE_UINT16: return sizeof m_variantData.union_data.uint16_data;
        case GUCEF_DATATYPE_INT32: return sizeof m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_UINT32: return sizeof m_variantData.union_data.uint32_data;
        case GUCEF_DATATYPE_INT64: return sizeof m_variantData.union_data.int64_data;
        case GUCEF_DATATYPE_UINT64: return sizeof m_variantData.union_data.uint64_data;
        case GUCEF_DATATYPE_FLOAT32: return sizeof m_variantData.union_data.float32_data;
        case GUCEF_DATATYPE_FLOAT64: return sizeof m_variantData.union_data.float64_data;
        case GUCEF_DATATYPE_BOOLEAN_INT32: return sizeof m_variantData.union_data.int32_data;
        case GUCEF_DATATYPE_BINARY: return m_variantData.union_data.heap_data.heap_data_size;

        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        {
            if ( !includeNullTerm || 0 == m_variantData.union_data.heap_data.heap_data_size )
                return m_variantData.union_data.heap_data.heap_data_size;

            const char* heapPtr = static_cast< const char* >( m_variantData.union_data.heap_data.union_data.char_heap_data );
            if ( '\0' == heapPtr[ m_variantData.union_data.heap_data.heap_data_size-1 ] )
                return m_variantData.union_data.heap_data.heap_data_size-1;
            return m_variantData.union_data.heap_data.heap_data_size;
        }
        case GUCEF_DATATYPE_UTF16_STRING:
        {
            if ( !includeNullTerm || 1 > m_variantData.union_data.heap_data.heap_data_size )
                return m_variantData.union_data.heap_data.heap_data_size;

            const UInt16 lastCodePoint = *reinterpret_cast< const UInt16* >( m_variantData.union_data.heap_data.union_data.char_heap_data + m_variantData.union_data.heap_data.heap_data_size - 2 );
            if ( 0 == lastCodePoint )
                return m_variantData.union_data.heap_data.heap_data_size-2;
            return m_variantData.union_data.heap_data.heap_data_size;
        }
        case GUCEF_DATATYPE_UTF32_STRING:
        {
            if ( !includeNullTerm || 3 > m_variantData.union_data.heap_data.heap_data_size )
                return m_variantData.union_data.heap_data.heap_data_size;

            const UInt32 lastCodePoint = *reinterpret_cast< const UInt32* >( m_variantData.union_data.heap_data.union_data.char_heap_data + m_variantData.union_data.heap_data.heap_data_size - 3 );
            if ( 0 == lastCodePoint )
                return m_variantData.union_data.heap_data.heap_data_size-4;
            return m_variantData.union_data.heap_data.heap_data_size;
        }

        default: return 0;
    }
}

/*-------------------------------------------------------------------------*/

CVariant::~CVariant()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( bool data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.int32_data = data ? 1 : 0;
    m_variantData.containedType = GUCEF_DATATYPE_BOOLEAN_INT32;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( Int8 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.int8_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_INT8;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( UInt8 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.uint8_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_UINT8;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( Int16 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.int16_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_INT16;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( UInt16 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.uint16_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_UINT16;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( Int32 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.int32_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_INT32;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( UInt32 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.uint32_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_UINT32;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( Int64 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.int64_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_INT64;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( UInt64 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.uint64_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_UINT64;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( Float32 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.float32_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_FLOAT32;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( Float64 data )
{GUCEF_TRACE;

    Clear();
    m_variantData.union_data.float64_data = data;
    m_variantData.containedType = GUCEF_DATATYPE_FLOAT64;
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant& 
CVariant::operator=( const CDynamicBuffer& data )
{GUCEF_TRACE;

    Set( data.GetConstBufferPtr(), data.GetDataSize(), GUCEF_DATATYPE_BINARY, false );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( const CAsciiString& data )
{GUCEF_TRACE;

    Set( data.C_String(), data.ByteSize(), GUCEF_DATATYPE_ASCII_STRING, false );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( const CUtf8String& data )
{GUCEF_TRACE;

    Set( data.C_String(), data.ByteSize(), GUCEF_DATATYPE_UTF8_STRING, false );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( const std::string& data )
{GUCEF_TRACE;

    Set( data.c_str(), (UInt32) data.size(), GUCEF_DATATYPE_UTF8_STRING, false );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( const std::wstring& data )
{GUCEF_TRACE;

    Set( data.c_str(), (UInt32) data.size(), GUCEF_DATATYPE_UTF16_STRING, false );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::operator=( const CVariant& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        // If dynamic memory is used we need to actually copy said memory
        // into a private copy
        if ( src.UsesDynamicMemory() )
        {
            if ( 0 < src.m_variantData.union_data.heap_data.heap_data_size && GUCEF_NULL != src.m_variantData.union_data.heap_data.union_data.void_heap_data )
            {
                m_variantData.union_data.heap_data.union_data.void_heap_data = malloc( (size_t) src.m_variantData.union_data.heap_data.heap_data_size );
                assert( GUCEF_NULL != m_variantData.union_data.heap_data.union_data.void_heap_data );
                memcpy( m_variantData.union_data.heap_data.union_data.void_heap_data, src.m_variantData.union_data.heap_data.union_data.void_heap_data, (size_t) src.m_variantData.union_data.heap_data.heap_data_size );
                m_variantData.union_data.heap_data.heap_data_size = src.m_variantData.union_data.heap_data.heap_data_size;
            }
            else
            {
                m_variantData.union_data.heap_data.union_data.void_heap_data = GUCEF_NULL;
                m_variantData.union_data.heap_data.heap_data_size = 0;
            }
            m_variantData.containedType = src.m_variantData.containedType;
            m_variantData.union_data.heap_data.heap_data_is_linked = 0;
        }
        else
        {
            memcpy( &m_variantData, &src.m_variantData, sizeof m_variantData );
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CVariant::Set( const void* data, UInt32 dataSize, UInt8 varType, bool linkOnlyForDynMem )
{GUCEF_TRACE;

    Clear();

    switch ( varType )
    {
        case GUCEF_DATATYPE_INT8:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(Int8) )
                return false;
            const Int8* typedData = reinterpret_cast< const Int8* >( data );
            m_variantData.union_data.int8_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_INT8;
            return true;
        }
        case GUCEF_DATATYPE_UINT8:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(UInt8) )
                return false;
            const UInt8* typedData = reinterpret_cast< const UInt8* >( data );
            m_variantData.union_data.uint8_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_UINT8;
            return true;
        }
        case GUCEF_DATATYPE_INT16:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(Int16) )
                return false;
            const Int16* typedData = reinterpret_cast< const Int16* >( data );
            m_variantData.union_data.int16_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_INT16;
            return true;
        }
        case GUCEF_DATATYPE_UINT16:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(UInt16) )
                return false;
            const UInt16* typedData = reinterpret_cast< const UInt16* >( data );
            m_variantData.union_data.uint16_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_UINT16;
            return true;
        }
        case GUCEF_DATATYPE_INT32:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(Int32) )
                return false;
            const Int32* typedData = reinterpret_cast< const Int32* >( data );
            m_variantData.union_data.int32_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_INT32;
            return true;
        }
        case GUCEF_DATATYPE_UINT32:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(UInt32) )
                return false;
            const UInt32* typedData = reinterpret_cast< const UInt32* >( data );
            m_variantData.union_data.uint32_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_UINT32;
            return true;
        }
        case GUCEF_DATATYPE_INT64:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(Int64) )
                return false;
            const Int64* typedData = reinterpret_cast< const Int64* >( data );
            m_variantData.union_data.int64_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_INT64;
            return true;
        }
        case GUCEF_DATATYPE_UINT64:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(UInt64) )
                return false;
            const UInt64* typedData = reinterpret_cast< const UInt64* >( data );
            m_variantData.union_data.uint64_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_UINT64;
            return true;
        }
        case GUCEF_DATATYPE_FLOAT32:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(Float32) )
                return false;
            const Float32* typedData = reinterpret_cast< const Float32* >( data );
            m_variantData.union_data.float32_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_FLOAT32;
            return true;
        }
        case GUCEF_DATATYPE_FLOAT64:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(Float64) )
                return false;
            const Float64* typedData = reinterpret_cast< const Float64* >( data );
            m_variantData.union_data.float64_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_FLOAT64;
            return true;
        }
        case GUCEF_DATATYPE_BOOLEAN_INT32:
        {
            if ( GUCEF_NULL == data || dataSize < sizeof(Int32) )
                return false;
            const Int32* typedData = reinterpret_cast< const Int32* >( data );
            m_variantData.union_data.int32_data = (*typedData);
            m_variantData.containedType = GUCEF_DATATYPE_BOOLEAN_INT32;
            return true;
        }
        case GUCEF_DATATYPE_BINARY:
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        case GUCEF_DATATYPE_UTF16_STRING:
        case GUCEF_DATATYPE_UTF32_STRING:
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        {
            if ( linkOnlyForDynMem )
            {
                m_variantData.union_data.heap_data.heap_data_is_linked = 1;
                m_variantData.union_data.heap_data.union_data.void_heap_data = const_cast< void* >( data );
                m_variantData.union_data.heap_data.heap_data_size = dataSize;
                m_variantData.containedType = varType;
                return true;
            }
            else
            {
                if ( GUCEF_NULL != HeapReserve( dataSize ) )
                {
                    memcpy( m_variantData.union_data.heap_data.union_data.void_heap_data, data, dataSize );
                    m_variantData.containedType = varType;
                    return true;
                }
                return false;
            }
        }
        default:
        {
            m_variantData.containedType = GUCEF_DATATYPE_UNKNOWN;
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::LinkTo( const CDynamicBuffer& src, UInt32 bufferOffset, UInt8 varType, UInt32 bytesToLink )
{GUCEF_TRACE;

    if ( bufferOffset + bytesToLink > src.GetDataSize() )
    {
        Clear();
        return *this;
    }

    UInt32 bufferSize = src.GetDataSize() - bufferOffset;
    if ( 0 == bytesToLink || bytesToLink > bufferSize )
        bytesToLink = bufferSize;

    Set( src.GetConstBufferPtr( bufferOffset ), bytesToLink, varType, true );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::LinkTo( const void* externalBuffer, UInt32 bufferSize )
{GUCEF_TRACE;

    Set( externalBuffer, bufferSize, GUCEF_DATATYPE_BINARY, true );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::LinkTo( const CVariant& src )
{GUCEF_TRACE;

    Set( src.AsVoidPtr(), src.ByteSize(), src.GetTypeId(), true );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::LinkTo( const CAsciiString& src )
{GUCEF_TRACE;

    Set( src.C_String(), src.ByteSize(), GUCEF_DATATYPE_ASCII_STRING, true );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::LinkTo( const CUtf8String& src )
{GUCEF_TRACE;

    Set( src.C_String(), src.ByteSize(), GUCEF_DATATYPE_UTF8_STRING, true );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::LinkTo( const std::string& src )
{GUCEF_TRACE;

    Set( src.c_str(), (UInt32) src.size(), GUCEF_DATATYPE_UTF8_STRING, true );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::LinkTo( const std::wstring& src )
{GUCEF_TRACE;

    Set( src.c_str(), (UInt32) src.size(), GUCEF_DATATYPE_UTF16_STRING, true );
    return *this;
}

/*-------------------------------------------------------------------------*/

CVariant&
CVariant::TransferOwnershipTo( CVariant& newOwner )
{GUCEF_TRACE;

    newOwner.Clear();
    newOwner.m_variantData = m_variantData;
    memset( &m_variantData, 0, sizeof( m_variantData ) );
    return newOwner;
}

/*-------------------------------------------------------------------------*/

void*
CVariant::HeapReserve( UInt32 byteSize, bool allowReduction )
{GUCEF_TRACE;

    if ( !UsesDynamicMemory() || !OwnsDynamicMemory() )
    {
        memset( &m_variantData, 0, sizeof( m_variantData ) );
        m_variantData.containedType = GUCEF_DATATYPE_BINARY;
    }

    if ( allowReduction || m_variantData.union_data.heap_data.heap_data_size < byteSize )
    {
        if ( 0 == byteSize && GUCEF_NULL != m_variantData.union_data.heap_data.union_data.void_heap_data )
        {
            free( m_variantData.union_data.heap_data.union_data.void_heap_data );
            m_variantData.union_data.heap_data.union_data.void_heap_data = GUCEF_NULL;
            m_variantData.union_data.heap_data.heap_data_size = 0;
            m_variantData.union_data.heap_data.heap_data_is_linked = 0;
            return GUCEF_NULL;
        }

        void* newBuffer = realloc( m_variantData.union_data.heap_data.union_data.void_heap_data, byteSize );
        if ( GUCEF_NULL != newBuffer )
        {
            m_variantData.union_data.heap_data.union_data.void_heap_data = newBuffer;
            m_variantData.union_data.heap_data.heap_data_size = byteSize;
            return newBuffer;
        }
        else
        {
            return GUCEF_NULL;
        }
    }

    return m_variantData.union_data.heap_data.union_data.void_heap_data;
}

/*-------------------------------------------------------------------------*/

bool
CVariant::SetString( UInt8 varType, const CString& data, const CVariant& defaultValue )
{GUCEF_TRACE;

    switch ( varType )
    {
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:  { *this = ToAsciiString( data ); if ( IsNULLOrEmpty() ) *this = defaultValue.AsAsciiString(); m_variantData.containedType = varType; return true; }
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:   { *this = ToUtf8String( data ); if ( IsNULLOrEmpty() ) *this = defaultValue.AsUtf8String(); m_variantData.containedType = varType; return true; }
        case GUCEF_DATATYPE_ASCII_STRING:          { *this = ToAsciiString( data ); if ( IsNULLOrEmpty() ) *this = defaultValue.AsAsciiString(); return true; }
        case GUCEF_DATATYPE_UTF8_STRING:           { *this = ToUtf8String( data ); if ( IsNULLOrEmpty() ) *this = defaultValue.AsUtf8String(); return true; }
        default:                                   { return false; }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::SetFromString( UInt8 varType, const CString& data, const CVariant& defaultValue )
{GUCEF_TRACE;

    switch ( varType )
    {
        case GUCEF_DATATYPE_UINT8:                 { *this = StringToUInt8( data, defaultValue.AsUInt8() ); return true; }
        case GUCEF_DATATYPE_UINT16:                { *this = StringToUInt16( data, defaultValue.AsUInt16() ); return true; }
        case GUCEF_DATATYPE_UINT32:                { *this = StringToUInt32( data, defaultValue.AsUInt32() ); return true; }
        case GUCEF_DATATYPE_UINT64:                { *this = StringToUInt64( data, defaultValue.AsUInt64() ); return true; }
        case GUCEF_DATATYPE_INT8:                  { *this = StringToInt8( data, defaultValue.AsInt8() ); return true; }
        case GUCEF_DATATYPE_INT16:                 { *this = StringToInt16( data, defaultValue.AsInt16() ); return true; }
        case GUCEF_DATATYPE_INT32:                 { *this = StringToInt32( data, defaultValue.AsInt32() ); return true; }
        case GUCEF_DATATYPE_INT64:                 { *this = StringToInt64( data, defaultValue.AsInt64() ); return true; }
        case GUCEF_DATATYPE_FLOAT32:               { *this = StringToFloat( data, defaultValue.AsFloat32() ); return true; }
        case GUCEF_DATATYPE_FLOAT64:               { *this = StringToDouble( data, defaultValue.AsFloat64() ); return true; }
        case GUCEF_DATATYPE_BOOLEAN_INT32:         { *this = StringToBool( data, defaultValue.AsBool() ); return true; }
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:  { return SetString( varType, data, defaultValue ); }
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:   { return SetString( varType, data, defaultValue ); }
        case GUCEF_DATATYPE_ASCII_STRING:          { return SetString( varType, data, defaultValue ); }
        case GUCEF_DATATYPE_UTF8_STRING:           { return SetString( varType, data, defaultValue ); }
        case GUCEF_DATATYPE_BINARY:                { return Base64Decode( data, HeapReserve( data.ByteSize()-1 ), data.ByteSize()-1, m_variantData.union_data.heap_data.heap_data_size ); }
        case GUCEF_DATATYPE_UNKNOWN:               { return false; }
        default:                                   { return false; }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::operator==( const CVariant& other ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_UINT8:
        case GUCEF_DATATYPE_UINT16:
        case GUCEF_DATATYPE_UINT32:
        case GUCEF_DATATYPE_UINT64:
        {
            return AsUInt64() == other.AsUInt64();
        }
        case GUCEF_DATATYPE_INT8:
        case GUCEF_DATATYPE_INT16:
        case GUCEF_DATATYPE_INT32:
        case GUCEF_DATATYPE_INT64:
        {
            return AsInt64() == other.AsInt64();
        }
        case GUCEF_DATATYPE_FLOAT32:
        case GUCEF_DATATYPE_FLOAT64:
        {
            return AsFloat64() == other.AsFloat64();   // @todo: epsilon compare
        }
        case GUCEF_DATATYPE_BOOLEAN_INT32:
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        {
            return AsBool() == other.AsBool();
        }
        case GUCEF_DATATYPE_BINARY:
        {
            if ( ByteSize() == other.ByteSize() )
            {
                return 0 == memcmp( AsVoidPtr(), other.AsVoidPtr(), ByteSize() );
            }
            return false;
        }
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        {
            // @todo: optimize
            return AsString() == other.AsString();
        }
        case GUCEF_DATATYPE_UNKNOWN:
        {
            // Essentially allowing nill == nill for a variant
            return GUCEF_DATATYPE_UNKNOWN == other.m_variantData.containedType;
        }
        default:
        {
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::operator!=( const CVariant& other ) const
{GUCEF_TRACE;

    return !( *this == other );
}

/*-------------------------------------------------------------------------*/

bool
CVariant::operator<( const CVariant& other ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_UINT8:
        case GUCEF_DATATYPE_UINT16:
        case GUCEF_DATATYPE_UINT32:
        case GUCEF_DATATYPE_UINT64:
        {
            return AsUInt64() < other.AsUInt64();
        }
        case GUCEF_DATATYPE_INT8:
        case GUCEF_DATATYPE_INT16:
        case GUCEF_DATATYPE_INT32:
        case GUCEF_DATATYPE_INT64:
        {
            return AsInt64() < other.AsInt64();
        }
        case GUCEF_DATATYPE_FLOAT32:
        case GUCEF_DATATYPE_FLOAT64:
        {
            return AsFloat64() < other.AsFloat64();
        }
        case GUCEF_DATATYPE_BOOLEAN_INT32:
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        {
            // Mostly nonsensical, but can be used for sorting a set
            return AsBool() < other.AsBool();
        }
        case GUCEF_DATATYPE_BINARY:
        {
            // Mostly nonsensical, but can be used for sorting a set
            UInt32 byteSize = SMALLEST( ByteSize(), other.ByteSize() );
            return 0 > memcmp( AsVoidPtr(), other.AsVoidPtr(), byteSize );
        }
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        {
            // @todo: optimize
            return AsString() < other.AsString();
        }
        case GUCEF_DATATYPE_UNKNOWN:
        {
            // Mostly nonsensical, but can be used for sorting a set
            return GUCEF_DATATYPE_UNKNOWN < other.m_variantData.containedType;
        }
        default:
        {
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::operator>( const CVariant& other ) const
{GUCEF_TRACE;

    return !( *this <= other );
}

/*-------------------------------------------------------------------------*/

bool
CVariant::operator<=( const CVariant& other ) const
{GUCEF_TRACE;

    switch ( m_variantData.containedType )
    {
        case GUCEF_DATATYPE_UINT8:
        case GUCEF_DATATYPE_UINT16:
        case GUCEF_DATATYPE_UINT32:
        case GUCEF_DATATYPE_UINT64:
        {
            return AsUInt64() <= other.AsUInt64();
        }
        case GUCEF_DATATYPE_INT8:
        case GUCEF_DATATYPE_INT16:
        case GUCEF_DATATYPE_INT32:
        case GUCEF_DATATYPE_INT64:
        {
            return AsInt64() <= other.AsInt64();
        }
        case GUCEF_DATATYPE_FLOAT32:
        case GUCEF_DATATYPE_FLOAT64:
        {
            return AsFloat64() <= other.AsFloat64();
        }
        case GUCEF_DATATYPE_BOOLEAN_INT32:
        case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:
        case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:
        {
            // Mostly nonsensical, but can be used for sorting a set
            return AsBool() <= other.AsBool();
        }
        case GUCEF_DATATYPE_BINARY:
        {
            // Mostly nonsensical, but can be used for sorting a set
            UInt32 byteSize = SMALLEST( ByteSize(), other.ByteSize() );
            return 0 >= memcmp( AsVoidPtr(), other.AsVoidPtr(), byteSize );
        }
        case GUCEF_DATATYPE_ASCII_STRING:
        case GUCEF_DATATYPE_UTF8_STRING:
        {
            // @todo: fix
            return AsString() < other.AsString() || AsString() == other.AsString();
        }
        case GUCEF_DATATYPE_UNKNOWN:
        {
            // Mostly nonsensical, but can be used for sorting a set
            return GUCEF_DATATYPE_UNKNOWN <= other.m_variantData.containedType;
        }
        default:
        {
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CVariant::operator>=( const CVariant& other ) const
{GUCEF_TRACE;

    return !( *this < other );
}

/*-------------------------------------------------------------------------*/

void
CVariant::Clear( void )
{GUCEF_TRACE;

    if ( OwnsDynamicMemory() )
    {
        if ( GUCEF_NULL != m_variantData.union_data.heap_data.union_data.void_heap_data )
            free( m_variantData.union_data.heap_data.union_data.void_heap_data );
    }
    // else: No dynamic memory management needed

    memset( &m_variantData, 0, sizeof( m_variantData ) );
}

/*-------------------------------------------------------------------------*/

CAsciiString
CVariant::AsAsciiString( const CAsciiString& defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    if ( GUCEF_DATATYPE_ASCII_STRING == m_variantData.containedType        ||
         GUCEF_DATATYPE_BOOLEAN_ASCII_STRING == m_variantData.containedType )
    {
        if ( 0 == m_variantData.union_data.heap_data.heap_data_size ||
             ( 1 == m_variantData.union_data.heap_data.heap_data_size && '\0' == *( (const char*) m_variantData.union_data.heap_data.union_data.char_heap_data ) ) )
            return defaultIfNeeded;

        return CAsciiString( (const char*) m_variantData.union_data.heap_data.union_data.char_heap_data, m_variantData.union_data.heap_data.heap_data_size );
    }
    return ToAsciiString( AsString( defaultIfNeeded ) );
}

/*-------------------------------------------------------------------------*/

CUtf8String
CVariant::AsUtf8String( const CUtf8String& defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    if ( GUCEF_DATATYPE_UTF8_STRING == m_variantData.containedType        ||
         GUCEF_DATATYPE_BOOLEAN_UTF8_STRING == m_variantData.containedType )
    {
        if ( 0 == m_variantData.union_data.heap_data.heap_data_size ||
             ( 1 == m_variantData.union_data.heap_data.heap_data_size && '\0' == *( (const char*) m_variantData.union_data.heap_data.union_data.char_heap_data ) ) )
            return defaultIfNeeded;

        return CUtf8String( (const char*) m_variantData.union_data.heap_data.union_data.char_heap_data, m_variantData.union_data.heap_data.heap_data_size );
    }
    return ToUtf8String( AsString( defaultIfNeeded ) );
}

/*-------------------------------------------------------------------------*/

CString
CVariant::AsString( const CString& defaultIfNeeded, bool resolveVarsIfApplicable ) const
{GUCEF_TRACE;

    if ( resolveVarsIfApplicable )
    {
        switch ( m_variantData.containedType )
        {
            case GUCEF_DATATYPE_UINT8:                 { return ToString( AsUInt8( StringToUInt8( ResolveVars( defaultIfNeeded ), 0 ) ) ); }
            case GUCEF_DATATYPE_UINT16:                { return ToString( AsUInt16( StringToUInt16( ResolveVars( defaultIfNeeded ), 0 ) ) ); }
            case GUCEF_DATATYPE_UINT32:                { return ToString( AsUInt32( StringToUInt32( ResolveVars( defaultIfNeeded ), 0 ) ) ); }
            case GUCEF_DATATYPE_UINT64:                { return ToString( AsUInt64( StringToUInt64( ResolveVars( defaultIfNeeded ), 0 ) ) ); }
            case GUCEF_DATATYPE_INT8:                  { return ToString( AsInt8( StringToInt8( ResolveVars( defaultIfNeeded ), 0 ) ) ); }
            case GUCEF_DATATYPE_INT16:                 { return ToString( AsInt16( StringToInt16( ResolveVars( defaultIfNeeded ), 0 ) ) ); }
            case GUCEF_DATATYPE_INT32:                 { return ToString( AsInt32( StringToInt32( ResolveVars( defaultIfNeeded ), 0 ) ) ); }
            case GUCEF_DATATYPE_INT64:                 { return ToString( AsInt64( StringToInt64( ResolveVars( defaultIfNeeded ), 0 ) ) ); }
            case GUCEF_DATATYPE_FLOAT32:               { return ToString( AsFloat32( StringToFloat( ResolveVars( defaultIfNeeded ), 0.0f ) ) ); }
            case GUCEF_DATATYPE_FLOAT64:               { return ToString( AsFloat64( StringToDouble( ResolveVars( defaultIfNeeded ), 0.0 ) ) ); }
            case GUCEF_DATATYPE_BOOLEAN_INT32:         { return ToString( AsBool( StringToBool( ResolveVars( defaultIfNeeded ), false ) ) ); }
            case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:  { return ResolveVars( ToString( AsAsciiString( ToAsciiString( ResolveVars( defaultIfNeeded ) ) ) ) ); }
            case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:   { return ResolveVars( ToString( AsUtf8String( ToUtf8String( ResolveVars( defaultIfNeeded ) ) ) ) ); }
            case GUCEF_DATATYPE_ASCII_STRING:          { return ResolveVars( ToString( AsAsciiString( ToAsciiString( ResolveVars( defaultIfNeeded ) ) ) ) ); }
            case GUCEF_DATATYPE_UTF8_STRING:           { return ResolveVars( ToString( AsUtf8String( ToUtf8String( ResolveVars( defaultIfNeeded ) ) ) ) ); }
            case GUCEF_DATATYPE_BINARY:                { CString result = Base64Encode( m_variantData.union_data.heap_data.union_data.void_heap_data, m_variantData.union_data.heap_data.heap_data_size ); return result.IsNULLOrEmpty() ? ResolveVars( defaultIfNeeded ) : result; }
            case GUCEF_DATATYPE_UNKNOWN:               { return ResolveVars( defaultIfNeeded ); }
            default:                                   { return ResolveVars( defaultIfNeeded ); }
        }
    }
    else
    {
        switch ( m_variantData.containedType )
        {
            case GUCEF_DATATYPE_UINT8:                 { return ToString( AsUInt8( StringToUInt8( defaultIfNeeded, 0 ) ) ); }
            case GUCEF_DATATYPE_UINT16:                { return ToString( AsUInt16( StringToUInt16( defaultIfNeeded, 0 ) ) ); }
            case GUCEF_DATATYPE_UINT32:                { return ToString( AsUInt32( StringToUInt32( defaultIfNeeded, 0 ) ) ); }
            case GUCEF_DATATYPE_UINT64:                { return ToString( AsUInt64( StringToUInt64( defaultIfNeeded, 0 ) ) ); }
            case GUCEF_DATATYPE_INT8:                  { return ToString( AsInt8( StringToInt8( defaultIfNeeded, 0 ) ) ); }
            case GUCEF_DATATYPE_INT16:                 { return ToString( AsInt16( StringToInt16( defaultIfNeeded, 0 ) ) ); }
            case GUCEF_DATATYPE_INT32:                 { return ToString( AsInt32( StringToInt32( defaultIfNeeded, 0 ) ) ); }
            case GUCEF_DATATYPE_INT64:                 { return ToString( AsInt64( StringToInt64( defaultIfNeeded, 0 ) ) ); }
            case GUCEF_DATATYPE_FLOAT32:               { return ToString( AsFloat32( StringToFloat( defaultIfNeeded, 0.0f ) ) ); }
            case GUCEF_DATATYPE_FLOAT64:               { return ToString( AsFloat64( StringToDouble( defaultIfNeeded, 0.0 ) ) ); }
            case GUCEF_DATATYPE_BOOLEAN_INT32:         { return ToString( AsBool( StringToBool( defaultIfNeeded, false ) ) ); }
            case GUCEF_DATATYPE_BOOLEAN_ASCII_STRING:  { return ToString( AsAsciiString( ToAsciiString( defaultIfNeeded ) ) ); }
            case GUCEF_DATATYPE_BOOLEAN_UTF8_STRING:   { return ToString( AsUtf8String( ToUtf8String( defaultIfNeeded ) ) ); }
            case GUCEF_DATATYPE_ASCII_STRING:          { return ToString( AsAsciiString( ToAsciiString( defaultIfNeeded ) ) ); }
            case GUCEF_DATATYPE_UTF8_STRING:           { return ToString( AsUtf8String( ToUtf8String( defaultIfNeeded ) ) ); }
            case GUCEF_DATATYPE_BINARY:                { CString result = Base64Encode( m_variantData.union_data.heap_data.union_data.void_heap_data, m_variantData.union_data.heap_data.heap_data_size ); return result.IsNULLOrEmpty() ? defaultIfNeeded : result; }
            case GUCEF_DATATYPE_UNKNOWN:               { return defaultIfNeeded; }
            default:                                   { return defaultIfNeeded; }
        }
    }
}

/*-------------------------------------------------------------------------*/

const TVariantData*
CVariant::CStyleAccess( void ) const
{GUCEF_TRACE;

    return &m_variantData;
}

/*-------------------------------------------------------------------------*/

CDynamicBuffer
CVariant::AsBuffer( void ) const
{GUCEF_TRACE;

    return CDynamicBuffer( static_cast< const char* >( AsVoidPtr() ), ByteSize() );
}

/*-------------------------------------------------------------------------*/

CVariant::operator CAsciiString() const
{GUCEF_TRACE;

    return AsAsciiString();
}

/*-------------------------------------------------------------------------*/

CVariant::operator CUtf8String() const
{GUCEF_TRACE;

    return AsUtf8String();
}

/*-------------------------------------------------------------------------*/

CString::StringVector 
ToStringVector( const CVariant::VariantVector& var )
{GUCEF_TRACE;

    CString::StringVector result;
    CVariant::VariantVector::const_iterator i = var.begin();
    while ( i != var.end() )
    {
        result.push_back( (*i).AsString() );
        ++i;
    }
    return result;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
