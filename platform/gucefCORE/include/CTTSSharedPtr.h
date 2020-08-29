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

#ifndef GUCEF_CORE_CTTSSHAREDPTR_H
#define GUCEF_CORE_CTTSSHAREDPTR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

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
 *  Templated implementation of a thread safe shared pointer.
 */
template< typename T >
class CTTSSharedPtr : public CTSharedPtr< T >
{
    public:

    /**
     *  Constructor that allows you to create an 'uninitialized' pointer.
     *  You will have to assign a valid shared pointer at a later time
     *  to initialize the shared pointer.
     *
     *  Note that if you attempt to use the shared pointer before it
     *  is initialized an exception will be thrown
     *
     */
    CTTSSharedPtr( void );

    explicit CTTSSharedPtr( const int NULLvalue );

    CTTSSharedPtr( T* ptr                                                      ,
                   CTDynamicDestructorBase< T >* objectDestructor = GUCEF_NULL );


    // inlined copy constructor, has to be inlined in class definition for now due to VC6 limitations
    template< class Derived >
    CTTSSharedPtr( const CTTSSharedPtr< Derived >& src )
        : CTSharedPtr< T >( src )
        , m_mutex()
    {GUCEF_TRACE;

    }

    CTTSSharedPtr( const CTTSSharedPtr& src );

    virtual ~CTTSSharedPtr();

    CTTSSharedPtr& operator=( const CTTSSharedPtr& src );

    /**
     *  Mutex based implementation.
     *  Called during operations on base class
     */
    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Mutex based implementation.
     *  Called during operations on base class
     */
    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    MT::CMutex m_mutex;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTTSSharedPtr< T >::CTTSSharedPtr( void )
    : CTSharedPtr< T >()
    , m_mutex()
{GUCEF_TRACE;
    // Note that if this constructor is used an assignment is required at
    // a later time to initialize the shared pointer
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTTSSharedPtr< T >::CTTSSharedPtr( const int NULLvalue )
    : CTSharedPtr< T >()
    , m_mutex()
{GUCEF_TRACE;

    // Note that if this constructor is used an assignment is required at
    // a later time to initialize the shared pointer
    assert( NULLvalue == NULL );
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTTSSharedPtr< T >::CTTSSharedPtr( T* ptr                                                      ,
                                   CTDynamicDestructorBase< T >* objectDestructor /* = NULL */ )
    : CTSharedPtr< T >( ptr, objectDestructor )
    , m_mutex()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTTSSharedPtr< T >::CTTSSharedPtr( const CTTSSharedPtr< T >& src )
    : CTSharedPtr< T >( src )
    , m_mutex()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTTSSharedPtr< T >::~CTTSSharedPtr()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTTSSharedPtr< T >&
CTTSSharedPtr< T >::operator=( const CTTSSharedPtr< T >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CTSharedPtr< T >::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T >
bool
CTTSSharedPtr< T >::Lock( void ) const
{GUCEF_TRACE;

    return m_mutex.Lock();
}

/*-------------------------------------------------------------------------*/

template< typename T >
bool
CTTSSharedPtr< T >::Unlock( void ) const
{GUCEF_TRACE;

    return m_mutex.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTTSSHAREDPTR_H ? */
