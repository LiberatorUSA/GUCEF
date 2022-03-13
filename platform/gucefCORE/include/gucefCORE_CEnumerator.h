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

#ifndef GUCEF_CORE_CENUMERATOR_H
#define GUCEF_CORE_CENUMERATOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

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

class CIEnumerable;

/**
 *  Base class for runtime polymorphic read-only access to an enumeration
 * 
 *  While slower than STL iterators and the like, carrying a runtime penalty, this interface allows
 *  for iteration across different data sets where regular C++ iterators struggle due to their
 *  template based basis. That basis is problematic across different runtime environments / ABIs.
 *  Especially when using plugin concepts with varying product life cycles and tool chains we 
 *  cannot always assume the same level of compatibility.
 */
class GUCEF_CORE_PUBLIC_CPP CConstEnumerator
{
    public:

    CConstEnumerator( void );

    CConstEnumerator( const CConstEnumerator& src );

    virtual ~CConstEnumerator();

    CConstEnumerator& operator=( const CConstEnumerator& src );

    virtual UInt8 GetTypeOfCurrent( void ) const = 0;
    
    virtual bool GetCurrent( CVariant& value, bool linkIfPossible = true ) = 0;

    virtual bool GetCurrent( const CIEnumerable* enumerable ) = 0;

    virtual bool GetIdOfCurrent( CVariant& value, bool linkIfPossible = true ) = 0;

    virtual bool CanEnumerateForward( void ) const = 0;

    virtual bool CanEnumerateBackward( void ) const = 0;

    virtual bool MoveNext( void ) = 0;
    
    virtual bool MovePrev( void ) = 0;

    virtual bool IsAtEnd( void ) const = 0;

    virtual Int32 Compare( const CConstEnumerator& other ) const = 0;

    CConstEnumerator& operator++( void );

    CConstEnumerator& operator--( void );

    bool operator<( const CConstEnumerator& other );
    bool operator<=( const CConstEnumerator& other );
    bool operator>( const CConstEnumerator& other );
    bool operator>=( const CConstEnumerator& other );
    bool operator==( const CConstEnumerator& other );
    bool operator!=( const CConstEnumerator& other );

    //CStyleAccess( void );
};


/*-------------------------------------------------------------------------*/

/**
 *  Base class for runtime polymorphic mutable access to an enumeration
 * 
 *  While slower than STL iterators and the like, carrying a runtime penalty, this interface allows
 *  for iteration across different data sets where regular C++ iterators struggle due to their
 *  template based basis. That basis is problematic across different runtime environments / ABIs.
 *  Especially when using plugin concepts with varying product life cycles and tool chains we 
 *  cannot always assume the same level of compatibility.
 */
class GUCEF_CORE_PUBLIC_CPP CEnumerator : public CConstEnumerator
{
    public:

    CEnumerator( void );

    CEnumerator( const CEnumerator& src );

    virtual ~CEnumerator();

    CEnumerator& operator=( const CEnumerator& src );

    virtual bool GetCurrent( CIEnumerable* enumerable ) = 0;

    CEnumerator& operator++( void );

    CEnumerator& operator--( void );

    bool operator<( const CEnumerator& other );
    bool operator<=( const CEnumerator& other );
    bool operator>( const CEnumerator& other );
    bool operator>=( const CEnumerator& other );
    bool operator==( const CEnumerator& other );
    bool operator!=( const CEnumerator& other );

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CENUMERATOR_H ? */
