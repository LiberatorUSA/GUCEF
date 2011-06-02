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

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#include "TestSharedPtr.h"

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
#define TESTASSERT( condition ) { if ( !( condition ) ) { ERRORHERE; } }

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class Base
{
    public:
    
    Base() : y( 1111 ) {}

    virtual ~Base() {}
    
    int y;
};

class Derived : public Base
{
    public:
    
    Derived() : x( 2222 ) {}

    virtual ~Derived() {}
    
    int x;
};

class Unrelated
{
    public:
    
    Unrelated() : z( 9999 ) {}
    
    virtual ~Unrelated();
    
    int z;
};

typedef GUCEF::CORE::CTSharedPtr< Base > TBasePtr;
typedef GUCEF::CORE::CTSharedPtr< Derived > TDerivedPtr;
typedef GUCEF::CORE::CTSharedPtr< Unrelated > TUnrelatedPtr;

void
PerformSharedPtrTests( void )
{
    std::cout << "\n\n**** COMMENCING SHARED PTR TESTS ****\n";
    
    try
    {
        //--- compile time tests, uncomment to test
        
        TUnrelatedPtr a;
        TDerivedPtr b;
        TBasePtr c;
        
        //a = b;                        //<- should not compile
        //b = a;                        //<- should not compile
        //a.StaticCast< Derived >();    //<- should not compile
        
        //c = a;                        //<- should not compile
        //a = c;                        //<- should not compile
        //a.StaticCast< Base >();       //<- should not compile
        
        
        //---- runtime tests
        
        Derived* d = new Derived();
        Derived* d2 = NULL;
        
        TBasePtr bp;
        TDerivedPtr dp( d );
        
        bp = dp;
        
        TESTASSERT( bp.GetReferenceCount() == 2 );
        TESTASSERT( dp.GetReferenceCount() == 2 );
        TESTASSERT( d == bp.GetPointerAlways() );
        TESTASSERT( d == dp.GetPointerAlways() );
        
        d2 = new Derived();
        bp = d2;
        
        TESTASSERT( (bp.GetReferenceCount() == 1) );
        TESTASSERT( (dp.GetReferenceCount() == 1) );
        TESTASSERT( d2 == bp.GetPointerAlways() );
        TESTASSERT( d == dp.GetPointerAlways() );
        
        dp = bp;
        
        TESTASSERT( bp.GetReferenceCount() == 2 );
        TESTASSERT( dp.GetReferenceCount() == 2 );
        TESTASSERT( d2 == bp.GetPointerAlways() );
        TESTASSERT( d2 == dp.GetPointerAlways() );
    }
    catch( ... )
    {
        ERRORHERE;
    }

    std::cout << "\n\n**** FINISHED SHAREDPTR TESTS ****\n";
}

/*-------------------------------------------------------------------------*/
