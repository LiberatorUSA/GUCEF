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

#ifndef GUCEF_CORE_SFINAE_UTILS_H
#define GUCEF_CORE_SFINAE_UTILS_H

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

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TEMPLATES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  C++98 backfill for C++11's enable_if<>
 */
template<bool B, class T = void>
struct EnableIf {};

template<class T>
struct EnableIf< true, T > { typedef T type; };

template<bool B, class T = void>
struct EnableIfNot {};

template<class T>
struct EnableIfNot< false, T > { typedef T type; };

/*-------------------------------------------------------------------------*/

/**
 *  C++98 compatible SFINAE template helper
 *  Allows for checking for the existance of the typedef TContainedType
 */
template < class T >
struct TypeHasContainedType
{
    // For the compile time comparison.
    typedef char    yes[1];
    typedef yes     no[2];

    template < typename TestClass > static yes& test( typename TestClass::TContainedType* /*unused*/ ) { static yes result; return result; }
    template < typename TestClass > static no&  test( ... ) { static no result; return result; }

    // The constant used as a return value for the test.
    enum { value = sizeof( test<T>(0) ) == sizeof( yes ) };
};

/*-------------------------------------------------------------------------*/

/**
 *  C++98 compatible SFINAE template helper
 *  Allows for checking for the existance of the typedef TLockType
 */
template < class T >
struct TypeHasLockType
{
    // For the compile time comparison.
    typedef char    yes[1];
    typedef yes     no[2];

    template < typename TestClass > static yes& test( typename TestClass::TLockType* /*unused*/ ) { static yes result; return result; }
    template < typename TestClass > static no&  test( ... ) { static no result; return result; }

    // The constant used as a return value for the test.
    enum { value = sizeof( test<T>(0) ) == sizeof( yes ) };
};

/*-------------------------------------------------------------------------*/

/**
 *  C++98 compatible SFINAE template helper
 *  Allows for checking for the existance of the member function TContainedType* T::GetPointerAlways
 */
template < class T >
struct TypeHasMemberFunctionGetPointerAlways
{
    // For the compile time comparison.
    typedef char    yes[1];
    typedef yes     no[2];

    template <typename U, U u> struct reallyHas;

    template < typename TestClass > static yes& test( reallyHas< typename TestClass::TContainedType* (TestClass::*)(), &TestClass::GetPointerAlways >* /*unused*/ ) { static yes result; return result; }
    template < typename TestClass > static no&  test( ... ) { static no result; return result; }

    // The constant used as a return value for the test.
    enum { value = sizeof( test<T>(0) ) == sizeof( yes ) };
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_SFINAE_UTILS_H ? */

