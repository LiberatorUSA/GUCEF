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

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

#include "TestVariant.h"

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

/*-------------------------------------------------------------------------*/

void
PerformVariantTests( void )
{
    std::cout << "\n\n**** COMMENCING CVariant TESTS ****\n";
    
    try
    {
        CORE::CVariant var;
        ASSERT_TRUE( var.GetTypeId() == GUCEF_DATATYPE_UNKNOWN );
        ASSERT_TRUE( var.GetTypeName() == GUCEF_DATATYPE_NAME_UNKNOWN );
        ASSERT_FALSE( var.IsInteger() );
        ASSERT_FALSE( var.IsBlob() );
        ASSERT_FALSE( var.IsBsob() );
        ASSERT_FALSE( var.IsBoolean() );        
        ASSERT_FALSE( var.IsFloat() );
        ASSERT_TRUE( var.IsNULLOrEmpty() );
        ASSERT_FALSE( var.IsString() );
        ASSERT_FALSE( var.IsDynamicMemoryLinked() );
        ASSERT_FALSE( var.IsInitialized() );

        var = Int8( 1 );
        ASSERT_TRUE( var.GetTypeId() == GUCEF_DATATYPE_INT8 );
        ASSERT_TRUE( var.GetTypeName() == GUCEF_DATATYPE_NAME_INT8 );
        ASSERT_TRUE( var.IsInteger() );
        ASSERT_FALSE( var.IsBlob() );
        ASSERT_FALSE( var.IsBsob() );
        ASSERT_FALSE( var.IsBoolean() );        
        ASSERT_FALSE( var.IsFloat() );
        ASSERT_FALSE( var.IsNULLOrEmpty() );
        ASSERT_FALSE( var.IsString() );
        ASSERT_FALSE( var.IsDynamicMemoryLinked() );
        ASSERT_TRUE( var.IsInitialized() );

        var = Float32( 1.0f );
        ASSERT_TRUE( var.GetTypeId() == GUCEF_DATATYPE_FLOAT32 );
        ASSERT_TRUE( var.GetTypeName() == GUCEF_DATATYPE_NAME_FLOAT32 );
        ASSERT_FALSE( var.IsInteger() );
        ASSERT_FALSE( var.IsBlob() );
        ASSERT_FALSE( var.IsBsob() );
        ASSERT_FALSE( var.IsBoolean() );        
        ASSERT_TRUE( var.IsFloat() );
        ASSERT_FALSE( var.IsNULLOrEmpty() );
        ASSERT_FALSE( var.IsString() );
        ASSERT_FALSE( var.IsDynamicMemoryLinked() );
        ASSERT_TRUE( var.IsInitialized() );

        var = Float64( 1.0 );
        ASSERT_TRUE( var.GetTypeId() == GUCEF_DATATYPE_FLOAT64 );
        ASSERT_TRUE( var.GetTypeName() == GUCEF_DATATYPE_NAME_FLOAT64 );
        ASSERT_FALSE( var.IsInteger() );
        ASSERT_FALSE( var.IsBlob() );
        ASSERT_FALSE( var.IsBsob() );
        ASSERT_FALSE( var.IsBoolean() );        
        ASSERT_TRUE( var.IsFloat() );
        ASSERT_FALSE( var.IsNULLOrEmpty() );
        ASSERT_FALSE( var.IsString() );
        ASSERT_FALSE( var.IsDynamicMemoryLinked() );
        ASSERT_TRUE( var.IsInitialized() );

        var = UInt16( 1 );
        ASSERT_TRUE( var.GetTypeId() == GUCEF_DATATYPE_UINT16 );
        ASSERT_TRUE( var.GetTypeName() == GUCEF_DATATYPE_NAME_UINT16 );
        ASSERT_TRUE( var.IsInteger() );
        ASSERT_FALSE( var.IsBlob() );
        ASSERT_FALSE( var.IsBsob() );
        ASSERT_FALSE( var.IsBoolean() );        
        ASSERT_FALSE( var.IsFloat() );
        ASSERT_FALSE( var.IsNULLOrEmpty() );
        ASSERT_FALSE( var.IsString() );
        ASSERT_FALSE( var.IsDynamicMemoryLinked() );
        ASSERT_TRUE( var.IsInitialized() );

        var = Int32( 1 );
        ASSERT_TRUE( var.GetTypeId() == GUCEF_DATATYPE_INT32 );
        ASSERT_TRUE( var.GetTypeName() == GUCEF_DATATYPE_NAME_INT32 );
        ASSERT_TRUE( var.IsInteger() );
        ASSERT_FALSE( var.IsBlob() );
        ASSERT_FALSE( var.IsBsob() );
        ASSERT_FALSE( var.IsBoolean() );        
        ASSERT_FALSE( var.IsFloat() );
        ASSERT_FALSE( var.IsNULLOrEmpty() );
        ASSERT_FALSE( var.IsString() );
        ASSERT_FALSE( var.IsDynamicMemoryLinked() );
        ASSERT_TRUE( var.IsInitialized() );



    }
    catch( ... )
    {
        ERRORHERE;
    }

    std::cout << "\n\n**** FINISHED CVariant TESTS ****\n";
}

/*-------------------------------------------------------------------------*/
