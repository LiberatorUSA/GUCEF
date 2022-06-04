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

#ifndef GUCEF_CORE_CENUMERABLE_H
#define GUCEF_CORE_CENUMERABLE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CENUMERATOR_H
#include "gucefCORE_CEnumerator.h"
#define GUCEF_CORE_CENUMERATOR_H
#endif /* GUCEF_CORE_CENUMERATOR_H ? */

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

/**
 *  Interface class for runtime polymorphic access for enumeration
 * 
 *  While slower than STL iterators and the like, carrying a runtime penalty, this interface allows
 *  for iteration across different data sets where regular C++ iterators struggle due to their
 *  template based basis. That basis is problematic across different runtime environments / ABIs.
 *  Especially when using plugin concepts with varying product life cycles and tool chains we 
 *  cannot always assume the same level of compatibility.
 */
class GUCEF_CORE_PUBLIC_CPP CIEnumerable
{
    public:

    CIEnumerable( void );

    CIEnumerable( const CIEnumerable& src );

    virtual ~CIEnumerable();

    CIEnumerable& operator=( const CIEnumerable& src );

    virtual bool GetEnumerator( CEnumerator& enumerator ) = 0;

    virtual bool GetEnumerator( CConstEnumerator& enumerator ) const = 0;

    bool GetConstEnumerator( CConstEnumerator& enumerator )
        { return const_cast< const CIEnumerable* >( this )->GetEnumerator( enumerator ); }

    
    protected:
    friend class CConstEnumerator;

    virtual UInt8 GetTypeOfCurrent( CVariant& enumeratorData ) const = 0;
    
    virtual bool GetCurrent( CVariant& enumeratorData   , 
                             CVariant& value            , 
                             bool linkIfPossible = true ) = 0;

    virtual bool GetCurrent( CVariant& enumeratorData        ,
                             const CIEnumerable** enumerable ) = 0;

    virtual bool GetIdOfCurrent( CVariant& enumeratorData   , 
                                 CVariant& value            , 
                                 bool linkIfPossible = true ) = 0;

    virtual bool GetNameOfCurrent( CVariant& enumeratorData ,
                                   CVariant& value          , 
                                   bool linkIfPossible = true ) = 0;

    virtual bool CanEnumerateForward( CVariant& enumeratorData ) const = 0;

    virtual bool CanEnumerateBackward( CVariant& enumeratorData ) const = 0;

    virtual bool MoveNext( CVariant& enumeratorData ) = 0;
    
    virtual bool MovePrev( CVariant& enumeratorData ) = 0;

    virtual bool IsAtEnd( CVariant& enumeratorData ) const = 0;

    virtual Int32 Compare( CVariant& enumeratorData            ,
                           const CVariant& otherEnumeratorData ) const = 0;

    protected:
    friend class CEnumerator;

    virtual bool GetCurrent( CVariant& enumeratorData  ,
                             CIEnumerable** enumerable ) = 0;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CENUMERABLE_H ? */
