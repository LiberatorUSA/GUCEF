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

#ifndef GUCEF_MT_CICLONEABLE_H
#include "gucefMT_CICloneable.h"
#define GUCEF_MT_CICLONEABLE_H
#endif /* GUCEF_MT_CICLONEABLE_H ? */

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

template <typename T>
struct true_or_false_type
{
    // For the compile time comparison.
    typedef char    yes[1];
    typedef yes     no[2];
};

template <typename T>
struct true_type : true_or_false_type< T >
{
    enum { value = true };
};

template <typename T>
struct false_type : true_or_false_type< T >
{
    enum { value = false };
};

template <typename T>
struct remove_const
{
    typedef T type;
};

template <typename T>
struct remove_const<const T>
{
    typedef T type;
};

template <typename T>
struct remove_volatile
{
    typedef T type;
};

template <typename T>
struct remove_volatile< volatile T >
{
    typedef T type;
};

template <typename T>
struct remove_cv : remove_const< typename remove_volatile< T >::type > {};

template <typename T>
struct is_unqualified_pointer
{
    enum { value = false_type< T >::value };
};

template <typename T>
struct is_unqualified_pointer<T*>
{
    enum { value = true_type< T >::value };
};

//template <typename T>
//struct is_pointer_type : is_unqualified_pointer<typename remove_cv<T>::type> {};
//
//template <typename T>
//bool is_pointer(const T&)
//{
//    return is_pointer_type<T>::value;
//}


template< class T > struct remove_pointer                    {typedef T type;};
template< class T > struct remove_pointer<T*>                {typedef T type;};
template< class T > struct remove_pointer<T* const>          {typedef T type;};
template< class T > struct remove_pointer<T* volatile>       {typedef T type;};
template< class T > struct remove_pointer<T* const volatile> {typedef T type;};

/*-------------------------------------------------------------------------*/


/**
 *  C++98 backfill for C++11's enable_if<>
 */
template<bool B, class T = void>
struct EnableIf {};

template<class T>
struct EnableIf< true, T > { typedef T type; };

template<bool B, bool B2, class T = void, class T2 = void>
struct EnableIf2 {};

template<class T, class T2>
struct EnableIf2< true, true, T, T2 > { typedef T type; typedef T2 type2; };

template<bool B, bool B2, class T = void, class T2 = void>
struct EnableIfFirstOf2 {};

template<class T, class T2>
struct EnableIfFirstOf2< true, false, T, T2 > { typedef T type; typedef T2 type2; };

template<bool B, class T = void>
struct EnableIfNot {};

template<class T>
struct EnableIfNot< false, T > { typedef T type; };

template<bool B, bool B2, class T = void, class T2 = void>
struct EnableIfNot2 {};

template<class T, class T2>
struct EnableIfNot2< false, false, T, T2 > { typedef T type; typedef T2 type2; };

/*-------------------------------------------------------------------------*/

/**
 *  C++98 compatible SFINAE template helper
 *  Allows checking if T is itself a pointer type
 *  In >= C++11 the analog would be std::is_pointer_type< T >
 */
template < class T >
struct TypeIsPointerType
{
    // For the compile time comparison.
    typedef char    yes[1];
    typedef yes     no[2];

    template < typename TestPtrType > static yes& test( typename remove_cv< TestPtrType >::type* /*unused*/ ) { static yes result; return result; }
    template < typename TestPtrType > static yes& test( void* /*unused*/ ) { static yes result; return result; }
    template < typename TestPtrType > static no&  test( ... ) { static no result; return result; }

    // The constant used as a return value for the test.
    enum { value = sizeof( test< T >( T() ) ) == sizeof( yes ) };
};

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

    template < typename TestClass > static yes& test( const typename TestClass::TContainedType* /*unused*/ ) { static yes result; return result; }
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

    template < typename TestClass > static yes& test( const typename TestClass::TLockType* /*unused*/ ) { static yes result; return result; }
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

/*-------------------------------------------------------------------------*/

/**
 *  C++98 compatible SFINAE template helper
 *  Allows for checking for the existance of the member function MT::CICloneable* T::Clone()
 */
template < class T >
struct TypeHasMemberFunctionClone
{
    // For the compile time comparison.
    typedef char    yes[1];
    typedef yes     no[2];

    template <typename U, U u> struct reallyHas;

    template < typename TestClass > static yes& test( reallyHas< typename MT::CICloneable* (TestClass::*)(), &TestClass::Clone >* /*unused*/ ) { static yes result; return result; }
    template < typename TestClass > static no&  test( ... ) { static no result; return result; }

    // The constant used as a return value for the test.
    enum { value = sizeof( test<T>(0) ) == sizeof( yes ) };
};

/*-------------------------------------------------------------------------*/

/**
 *  C++98 compatible SFINAE template helper
 *  Allows for checking for the existance of the member function bool T::LoadConfig( ... )
 */
template < class T >
struct TypeHasMemberFunctionLoadConfig
{
    // For the compile time comparison.
    typedef char    yes[1];
    typedef yes     no[2];

    template <typename U, U u> struct reallyHas;

    template < typename TestClass > static yes& test( reallyHas< bool (TestClass::*)(), &TestClass::LoadConfig >* /*unused*/ ) { static yes result; return result; }
    template < typename TestClass > static no&  test( ... ) { static no result; return result; }

    // The constant used as a return value for the test.
    enum { value = sizeof( test<T>(0) ) == sizeof( yes ) };
};

/*-------------------------------------------------------------------------*/

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

/**
 *  C++98 compatible SFINAE template helper
 *  Allows for checking for the existance of the accessible assignement operator
 */
template < class T >
struct TypeHasAssignmentOperator
{
    // For the compile time comparison.
    typedef char    yes[1];
    typedef yes     no[2];

    template <typename U, U u> struct reallyHas;

    template < typename TestClass > static yes& test( reallyHas< typename TestClass& (TestClass::*)(), (&TestClass::operator=) >* /*unused*/ ) { static yes result; return result; }
    template < typename TestClass > static no&  test( ... ) { static no result; return result; }

    // The constant used as a return value for the test.
    enum { value = sizeof( test<T>(0) ) == sizeof( yes ) };
};

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_SFINAE_UTILS_H ? */

