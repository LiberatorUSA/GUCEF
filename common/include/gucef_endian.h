/*
 *  GUCEF platform wide macros/defines for OS/Compiler specifics
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#ifndef GUCEF_ENDIAN_H
#define GUCEF_ENDIAN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

#ifndef GUCEF_PLATFORM_H
#include "gucef_platform.h"
#define GUCEF_PLATFORM_H
#endif /* GUCEF_PLATFORM_H ? */

#ifndef GUCEF_TYPES_H
#include "gucef_types.h"
#define GUCEF_TYPES_H
#endif /* GUCEF_TYPES_H ? */

#ifndef GUCEF_MEMORY_H
#include "gucef_memory.h"
#define GUCEF_MEMORY_H
#endif /* GUCEF_MEMORY_H ? */

#ifndef GUCEF_SCOPESTACKTRACER_H
#include "gucef_ScopeStackTracer.h"
#define GUCEF_SCOPESTACKTRACER_H
#endif /* GUCEF_SCOPESTACKTRACER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      DEFINES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
  #define GUCEF_ENDIAN_FUNCTION_PREFIX inline
#else
    #define GUCEF_ENDIAN_FUNCTION_PREFIX static inline
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Endianess swap function for 16-bit unsigned integer
 */
GUCEF_ENDIAN_FUNCTION_PREFIX
UInt16
SwapEndianUInt16( UInt16 value )
{GUCEF_TRACE;

    return (value >> 8) |
           (value << 8);
}

/*-------------------------------------------------------------------------*/

/**
 *  Endianess swap function for 16-bit signed integer
 */
GUCEF_ENDIAN_FUNCTION_PREFIX
Int16
SwapEndianInt16( Int16 value )
{GUCEF_TRACE;

    #ifdef __cplusplus
    UInt16 uValue = static_cast< UInt16 >( value );
    return static_cast< Int16 >( SwapEndianUInt16( uValue ) );
    #else
    UInt16 uValue = (UInt16) value;
    return (Int16) SwapEndianUInt16( uValue );
    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *  Endianess swap function for 32-bit unsigned integer
 */
GUCEF_ENDIAN_FUNCTION_PREFIX
UInt32
SwapEndianUInt32( UInt32 value )
{GUCEF_TRACE;

    return ((value >> 24) & 0x000000FF) |
           ((value >> 8)  & 0x0000FF00) |
           ((value << 8)  & 0x00FF0000) |
           ((value << 24) & 0xFF000000);
}

/*-------------------------------------------------------------------------*/

/**
 *  Endianess swap function for 32-bit signed integer
 */
GUCEF_ENDIAN_FUNCTION_PREFIX
Int32
SwapEndianInt32( Int32 value )
{GUCEF_TRACE;

    #ifdef __cplusplus
    UInt32 uValue = static_cast< UInt32 >( value );
    return static_cast< Int32 >( SwapEndianUInt32( uValue ) );
    #else
    UInt32 uValue = (UInt32) value;
    return (Int32) SwapEndianUInt32( uValue );
    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *  Endianess swap function for 64-bit unsigned integer
 */
GUCEF_ENDIAN_FUNCTION_PREFIX
UInt64
SwapEndianUInt64( UInt64 value )
{GUCEF_TRACE;

    return ((value >> 56) & 0x00000000000000FFULL) |
           ((value >> 40) & 0x000000000000FF00ULL) |
           ((value >> 24) & 0x0000000000FF0000ULL) |
           ((value >> 8)  & 0x00000000FF000000ULL) |
           ((value << 8)  & 0x000000FF00000000ULL) |
           ((value << 24) & 0x0000FF0000000000ULL) |
           ((value << 40) & 0x00FF000000000000ULL) |
           ((value << 56) & 0xFF00000000000000ULL);
}

/*-------------------------------------------------------------------------*/

/**
 *  Endianess swap function for 64-bit signed integer
 */
GUCEF_ENDIAN_FUNCTION_PREFIX
Int64
SwapEndianInt64( Int64 value )
{GUCEF_TRACE;

    #ifdef __cplusplus
    UInt64 uValue = static_cast< UInt64 >( value );
    return static_cast< Int64 >( SwapEndianUInt64( uValue ) );
    #else
    UInt64 uValue = (UInt64) value;
    return (Int64) SwapEndianUInt64( uValue );
    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *  Endianess swap function for 32-bit floating point
 */
GUCEF_ENDIAN_FUNCTION_PREFIX
Float32
SwapEndianFloat32( Float32 value )
{GUCEF_TRACE;

    #ifdef __cplusplus
    UInt32 temp = SwapEndianUInt32( *reinterpret_cast< UInt32* >( &value ) );
    return *reinterpret_cast< Float32* >( &temp );
    #else
    UInt32 temp = (UInt32) value;
    return (Float32) SwapEndianUInt32( temp );
    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *  Endianess swap function for 64-bit floating point
 */
GUCEF_ENDIAN_FUNCTION_PREFIX
Float64
SwapEndianFloat64( Float64 value )
{GUCEF_TRACE;

    #ifdef __cplusplus
    UInt64 temp = SwapEndianUInt64( *reinterpret_cast< UInt64* >( &value ) );
    return *reinterpret_cast< Float64* >( &temp );
    #else
    UInt64 temp = (UInt64) value;
    return (Float64) SwapEndianUInt64( temp );
    #endif
}

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus

GUCEF_ENDIAN_FUNCTION_PREFIX Int16 SwapEndian( Int16 value ) { return SwapEndianInt16( value ); }
GUCEF_ENDIAN_FUNCTION_PREFIX UInt16 SwapEndian( UInt16 value ) { return SwapEndianUInt16( value ); }
GUCEF_ENDIAN_FUNCTION_PREFIX Int32 SwapEndian( Int32 value ) { return SwapEndianInt32( value ); }
GUCEF_ENDIAN_FUNCTION_PREFIX UInt32 SwapEndian( UInt32 value ) { return SwapEndianUInt32( value ); }
GUCEF_ENDIAN_FUNCTION_PREFIX Int64 SwapEndian( Int64 value ) { return SwapEndianInt64( value ); }
GUCEF_ENDIAN_FUNCTION_PREFIX UInt64 SwapEndian( UInt64 value ) { return SwapEndianUInt64( value ); }
GUCEF_ENDIAN_FUNCTION_PREFIX Float32 SwapEndian( Float32 value ) { return SwapEndianFloat32( value ); }
GUCEF_ENDIAN_FUNCTION_PREFIX Float64 SwapEndian( Float64 value ) { return SwapEndianFloat64( value ); }

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_ENDIAN_H ? */
