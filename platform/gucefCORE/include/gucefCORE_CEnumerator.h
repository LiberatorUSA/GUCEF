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
 *  class for runtime polymorphic read-only access to an enumeration
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

    CConstEnumerator( const CIEnumerable* enumerable ,
                      const CVariant& enumeratorData );

    CConstEnumerator( const CConstEnumerator& src );

    virtual ~CConstEnumerator();

    CConstEnumerator& operator=( const CConstEnumerator& src );

    UInt8 GetTypeOfCurrent( void ) const;
    
    bool GetCurrent( CVariant& value, bool linkIfPossible = true );

    bool GetCurrent( const CIEnumerable** enumerable );

    bool GetIdOfCurrent( CVariant& value, bool linkIfPossible = true );

    bool GetNameOfCurrent( CVariant& value, bool linkIfPossible = true );

    bool CanEnumerateForward( void ) const;

    bool CanEnumerateBackward( void ) const;

    bool MoveNext( void );
    
    bool MovePrev( void );

    bool IsAtEnd( void ) const;

    Int32 Compare( const CConstEnumerator& other ) const;

    CConstEnumerator& operator++( void );

    CConstEnumerator& operator--( void );

    bool operator<( const CConstEnumerator& other );
    bool operator<=( const CConstEnumerator& other );
    bool operator>( const CConstEnumerator& other );
    bool operator>=( const CConstEnumerator& other );
    bool operator==( const CConstEnumerator& other );
    bool operator!=( const CConstEnumerator& other );

    //CStyleAccess( void );

    protected:

    CIEnumerable* m_enumerable;
    mutable CVariant m_enumeratorData;
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

    CEnumerator( CIEnumerable* enumerable       ,
                 const CVariant& enumeratorData );

    CEnumerator( const CEnumerator& src );

    virtual ~CEnumerator();

    CEnumerator& operator=( const CEnumerator& src );

    bool GetCurrent( CIEnumerable** enumerable );

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
