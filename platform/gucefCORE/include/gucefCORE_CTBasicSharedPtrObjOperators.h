/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CORE_CTBASICSHAREDPTROBJOPERATORS_H
#define GUCEF_CORE_CTBASICSHAREDPTROBJOPERATORS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

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
 *  Helper template to allow one to use operators on shared pointers as if you  
 *  were dealing with the object itself. 
 *  Mainly intended for sorting of objects in containers while using pointers to said objects
 */
template< typename T, class LockType >
class CTBasicSharedPtrObjOperators : public CTBasicSharedPtr< T, LockType >
{
    public:

    CTBasicSharedPtrObjOperators( void );

    CTBasicSharedPtrObjOperators( const CTBasicSharedPtr< T, LockType >& objPtr );

    CTBasicSharedPtrObjOperators( const CTBasicSharedPtrObjOperators& src );

    virtual ~CTBasicSharedPtrObjOperators();

    CTBasicSharedPtrObjOperators& operator=( const CTBasicSharedPtrObjOperators& src );

    CTBasicSharedPtrObjOperators& operator=( const CTBasicSharedPtr< T, LockType >& objPtr );

    CTBasicSharedPtrObjOperators& operator=( const T& other );
    
    inline bool operator<( const CTBasicSharedPtrObjOperators& other ) const;
    
    inline bool operator<( const CTBasicSharedPtr< T, LockType >& other ) const;

    inline bool operator<( const T& other ) const;

    inline bool operator==( const CTBasicSharedPtrObjOperators& other ) const;

    MT::CICloneable* Clone( void ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtrObjOperators< T, LockType >::CTBasicSharedPtrObjOperators( void )
    : CTBasicSharedPtr< T, LockType >()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtrObjOperators< T, LockType >::CTBasicSharedPtrObjOperators( const CTBasicSharedPtr< T, LockType >& src )
    : CTBasicSharedPtr< T, LockType >( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtrObjOperators< T, LockType >::CTBasicSharedPtrObjOperators( const CTBasicSharedPtrObjOperators< T, LockType >& src )
    : CTBasicSharedPtr< T, LockType >( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtrObjOperators< T, LockType >::~CTBasicSharedPtrObjOperators()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtrObjOperators< T, LockType >&
CTBasicSharedPtrObjOperators< T, LockType >::operator=( const CTBasicSharedPtrObjOperators< T, LockType >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CTBasicSharedPtr< T, LockType >::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtrObjOperators< T, LockType >&
CTBasicSharedPtrObjOperators< T, LockType >::operator=( const CTBasicSharedPtr< T, LockType >& objPtr )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CTBasicSharedPtr< T, LockType >::operator=( objPtr );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtrObjOperators< T, LockType >&
CTBasicSharedPtrObjOperators< T, LockType >::operator=( const T& obj )
{GUCEF_TRACE;

    (*GetPointer()) = obj;
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtrObjOperators< T, LockType >::operator<( const CTBasicSharedPtrObjOperators< T, LockType >& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    MT::CObjectScopeLock lockOther( &other );
    
    if ( IsNULL() )
    {
        if ( other.IsNULL() )
            return false; // null is equal to null in this context, stop moving things
        return true;
    }
    else
    if ( other.IsNULL() )
    {
        // null is equal to null in this context, stop moving things
        // Also if this obj is not null then we are > 
        // so we are either == or >  Either case its false
        return false;
    }
    else
    {
        // This is what its all about
        // perform the operator on the actual referenced object instead
        return (*GetPointerAlways()) < (*other.GetPointerAlways());
    }
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtrObjOperators< T, LockType >::operator<( const CTBasicSharedPtr< T, LockType >& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    MT::CObjectScopeLock lockOther( &other );
    
    if ( IsNULL() )
    {
        if ( other.IsNULL() )
            return false; // null is equal to null in this context, stop moving things
        return true;
    }
    else
    if ( other.IsNULL() )
    {
        // null is equal to null in this context, stop moving things
        // Also if this obj is not null then we are > 
        // so we are either == or >  Either case its false
        return false;
    }
    else
    {
        // This is what its all about
        // perform the operator on the actual referenced object instead
        return (*GetPointerAlways()) < (*other.GetPointerAlways());
    }
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtrObjOperators< T, LockType >::operator<( const T& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    if ( IsNULL() )
    {
        return true;
    }
    else
    {
        // This is what its all about
        // perform the operator on the actual referenced object instead
        return (*GetPointerAlways()) < other;
    }
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtrObjOperators< T, LockType >::operator==( const CTBasicSharedPtrObjOperators< T, LockType >& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    MT::CObjectScopeLock lockOther( &other );
    
    if ( IsNULL() )
    {
        if ( other.IsNULL() )
            return true;
        return false;
    }
    else
    if ( other.IsNULL() )
    {
        if ( IsNULL() )
            return true;
        return false;
    }
    else
    {
        // This is what its all about
        // perform the operator on the actual referenced object instead
        return (*GetPointerAlways()) == (*other.GetPointerAlways());
    }
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CICloneable*
CTBasicSharedPtrObjOperators< T, LockType >::Clone( void ) const
{GUCEF_TRACE;

    return new CTBasicSharedPtrObjOperators< T, LockType >( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTBASICSHAREDPTROBJOPERATORS_H ? */
