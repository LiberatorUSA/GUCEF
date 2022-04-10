/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <iostream>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

#ifndef GUCEF_CORE_CVARIANTBINARYSERIALIZER_H
#include "gucefCORE_CVariantBinarySerializer.h"
#define GUCEF_CORE_CVARIANTBINARYSERIALIZER_H
#endif /* GUCEF_CORE_CVARIANTBINARYSERIALIZER_H ? */

#include "TestVariantBinarySerializer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#if GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX || GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID
  #define DEBUGBREAK __builtin_trap()
#elif GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN
  #define DEBUGBREAK DebugBreak()
#else
  #define DEBUGBREAK
#endif

#define ERRORHERE { std::cout << "Test failed @ " << __FILE__ << "(" << __LINE__ << ")\n"; DEBUGBREAK; }
#define ASSERT_TRUE( test ) if ( !(test) ) { ERRORHERE; } 
#define ASSERT_FALSE( test ) if ( (test) ) { ERRORHERE; }

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

void
GenerateTestBinaryPayload( CORE::CDynamicBuffer& buffer, UInt32 startNum, UInt32 endNum )
{GUCEF_TRACE;

    for ( UInt32 i=startNum; i<endNum; ++i )
    {
        buffer.AppendValue( i );
    }    
}

/*-------------------------------------------------------------------------*/

void
PerformVariantBinarySerializerTests( void )
{
    std::cout << "\n\n**** COMMENCING CVariantBinarySerializer TESTS ****\n";
    
    try
    {                              
        CORE::CDynamicBuffer buffer;
        CORE::CVariant var, var2;
        UInt32 bytesWritten = 0;
        UInt32 byteRead = 0;

        // test empty variant
        var.Clear();
        buffer.Clear();
        bytesWritten = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Serialize( var, 0, buffer, bytesWritten ) );
        ASSERT_TRUE( bytesWritten == buffer.GetDataSize() );
        ASSERT_TRUE( bytesWritten == 1 );
        
        byteRead = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Deserialize( var2, 0, buffer, true, byteRead ) );
        ASSERT_TRUE( byteRead == bytesWritten );
        ASSERT_TRUE( byteRead == buffer.GetDataSize() );
        ASSERT_TRUE( byteRead == 1 );
        ASSERT_FALSE( var2.UsesDynamicMemory() );
        ASSERT_TRUE( var == var2 );

        // test integer variant
        var.Clear();
        var = UInt64( 1 );
        buffer.Clear();
        bytesWritten = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Serialize( var, 0, buffer, bytesWritten ) );
        ASSERT_TRUE( bytesWritten == buffer.GetDataSize() );
        ASSERT_TRUE( bytesWritten == 1+sizeof(UInt64) );
        
        byteRead = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Deserialize( var2, 0, buffer, true, byteRead ) );
        ASSERT_TRUE( byteRead == bytesWritten );
        ASSERT_TRUE( byteRead == buffer.GetDataSize() );
        ASSERT_TRUE( byteRead == 1+sizeof(UInt64) );
        ASSERT_TRUE( var2.AsUInt64() == 1 );
        ASSERT_FALSE( var2.UsesDynamicMemory() );
        ASSERT_TRUE( var == var2 );

        // test boolean variant
        var.Clear();
        var = true;
        buffer.Clear();
        bytesWritten = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Serialize( var, 0, buffer, bytesWritten ) );
        ASSERT_TRUE( bytesWritten == buffer.GetDataSize() );
        ASSERT_TRUE( bytesWritten == 1+sizeof(Int32) );
        
        byteRead = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Deserialize( var2, 0, buffer, true, byteRead ) );
        ASSERT_TRUE( byteRead == bytesWritten );
        ASSERT_TRUE( byteRead == buffer.GetDataSize() );
        ASSERT_TRUE( byteRead == 1+sizeof(Int32) );
        ASSERT_TRUE( var2.AsBool() );
        ASSERT_FALSE( var2.UsesDynamicMemory() );
        ASSERT_TRUE( var == var2 );

        // test float variant
        var.Clear();
        var = Float32( 1.0f );
        buffer.Clear();
        bytesWritten = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Serialize( var, 0, buffer, bytesWritten ) );
        ASSERT_TRUE( bytesWritten == buffer.GetDataSize() );
        ASSERT_TRUE( bytesWritten == 1+sizeof(Float32) );
        
        byteRead = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Deserialize( var2, 0, buffer, true, byteRead ) );
        ASSERT_TRUE( byteRead == bytesWritten );
        ASSERT_TRUE( byteRead == buffer.GetDataSize() );
        ASSERT_TRUE( byteRead == 1+sizeof(Float32) );
        ASSERT_TRUE( var2.AsFloat32() == 1.0f );
        ASSERT_FALSE( var2.UsesDynamicMemory() );
        ASSERT_TRUE( var == var2 );

        var.Clear();
        var = Float64( 1.0 );
        buffer.Clear();
        bytesWritten = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Serialize( var, 0, buffer, bytesWritten ) );
        ASSERT_TRUE( bytesWritten == buffer.GetDataSize() );
        ASSERT_TRUE( bytesWritten == 1+sizeof(Float64) );
        
        byteRead = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Deserialize( var2, 0, buffer, true, byteRead ) );
        ASSERT_TRUE( byteRead == bytesWritten );
        ASSERT_TRUE( byteRead == buffer.GetDataSize() );
        ASSERT_TRUE( byteRead == 1+sizeof(Float64) );
        ASSERT_TRUE( var2.AsFloat64() == 1.0 );
        ASSERT_FALSE( var2.UsesDynamicMemory() );
        ASSERT_TRUE( var == var2 );

        // test string variant
        var.Clear();
        CORE::CUtf8String utf8TestString( "this is a test - this is a test - this is a test - this is a test - this is a test - this is a test - this is a test - this is a test" );
        var = utf8TestString;
        buffer.Clear();
        bytesWritten = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Serialize( var, 0, buffer, bytesWritten ) );
        ASSERT_TRUE( bytesWritten == buffer.GetDataSize() );
        ASSERT_TRUE( bytesWritten == 1+sizeof(UInt32)+utf8TestString.ByteSize() );
        
        byteRead = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Deserialize( var2, 0, buffer, true, byteRead ) );
        ASSERT_TRUE( byteRead == bytesWritten );
        ASSERT_TRUE( byteRead == buffer.GetDataSize() );
        ASSERT_TRUE( byteRead == 1+sizeof(UInt32)+utf8TestString.ByteSize() );
        ASSERT_TRUE( var2.AsUtf8String() == utf8TestString );
        ASSERT_TRUE( var2.UsesDynamicMemory() );
        ASSERT_TRUE( var == var2 );

        var.Clear();
        CORE::CAsciiString asciiTestString( "this is a test - this is a test - this is a test - this is a test - this is a test - this is a test - this is a test - this is a test" );
        var = asciiTestString;
        buffer.Clear();
        bytesWritten = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Serialize( var, 0, buffer, bytesWritten ) );
        ASSERT_TRUE( bytesWritten == buffer.GetDataSize() );
        ASSERT_TRUE( bytesWritten == 1+sizeof(UInt32)+asciiTestString.ByteSize() );
        
        byteRead = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Deserialize( var2, 0, buffer, true, byteRead ) );
        ASSERT_TRUE( byteRead == bytesWritten );
        ASSERT_TRUE( byteRead == buffer.GetDataSize() );
        ASSERT_TRUE( byteRead == 1+sizeof(UInt32)+asciiTestString.ByteSize() );
        ASSERT_TRUE( var2.AsAsciiString() == asciiTestString );
        ASSERT_TRUE( var2.UsesDynamicMemory() );
        ASSERT_TRUE( var == var2 );

        // test blob variant
        CORE::CDynamicBuffer blob;
        GenerateTestBinaryPayload( blob, 1, 100 );
        var.Clear();
        var = blob;
        buffer.Clear();
        bytesWritten = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Serialize( var, 0, buffer, bytesWritten ) );
        ASSERT_TRUE( bytesWritten == buffer.GetDataSize() );
        ASSERT_TRUE( bytesWritten == 1+sizeof(UInt32)+blob.GetDataSize() );
        
        byteRead = 0;
        ASSERT_TRUE( CORE::CVariantBinarySerializer::Deserialize( var2, 0, buffer, true, byteRead ) );
        ASSERT_TRUE( byteRead == bytesWritten );
        ASSERT_TRUE( byteRead == buffer.GetDataSize() );
        ASSERT_TRUE( byteRead == 1+sizeof(UInt32)+blob.GetDataSize() );
        ASSERT_TRUE( var2 == blob );
        ASSERT_TRUE( var2.UsesDynamicMemory() );
        ASSERT_TRUE( var == var2 );        
    }
    catch( ... )
    {
        ERRORHERE;
    }

    std::cout << "\n\n**** FINISHED CVariantBinarySerializer TESTS ****\n";
}

/*-------------------------------------------------------------------------*/
