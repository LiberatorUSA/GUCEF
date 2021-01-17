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

#ifndef GUCEF_CORE_CTBASICSHAREDPTR_H
#define GUCEF_CORE_CTBASICSHAREDPTR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_MT_CILOCKABLE_H
#include "gucefMT_CILockable.h"
#define GUCEF_MT_CILOCKABLE_H
#endif /* GUCEF_MT_CILOCKABLE_H ? */

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* module macro's */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H
#include "CTDynamicDestructorBase.h"
#define GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H
#endif /* GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H ? */

#ifndef GUCEF_CORE_EXCEPTIONCLASSMACROS_H
#include "ExceptionClassMacros.h"
#define GUCEF_CORE_EXCEPTIONCLASSMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONCLASSMACROS_H ? */

#ifndef GUCEF_CORE_EXCEPTIONTHROWMACROS_H
#include "ExceptionThrowMacros.h"
#define GUCEF_CORE_EXCEPTIONTHROWMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONTHROWMACROS_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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

template< class LockType >
class TBasicSharedPtrSharedData
{
    public:

    Int32 m_refCounter;              /**< shared reference counter */
    LockType m_lock;                 /**< shared lock, if any */

    TBasicSharedPtrSharedData( void )
        : m_refCounter( 0 )
        , m_lock()
    {
    }
};

/*-------------------------------------------------------------------------*/

/**
 *  Templated implementation of a shared pointer.
 *
 *  A shared pointer adds some safety to pointers that get passed
 *  around where objects keep a reference to object instances independent
 *  of each other. In such situations it can become unclear who is responsible
 *  for destroying the referenced object instance.
 *  The shared pointer will reference count the pointer and ensure that even
 *  though the original owner has no knowledge of other classes referencing the
 *  object it can safely manage object destruction as desired.
 *
 *  Note that the usage of an external destructor for the shared object by means of the
 *  CTSharedObjectDestructor template is mandatory. By passing in a pointer to such a class
 *  you basically create a callback link for cleanup purposes.
 *
 *  Note that this shared pointer implementation's threadsafety is dependent on the LockType
 *  template param. A NoLock could be used as the LockType for implementations where the
 *  shared ptr is garanteed to never cross thread boundaries resulting is a minor optimization.
 */
template< typename T, class LockType >
class CTBasicSharedPtr : public MT::CILockable ,
                         public CICloneable
{
    public:

    typedef CTDynamicDestructorBase< T >   TDestructor;

    protected:

    mutable TBasicSharedPtrSharedData< LockType >* m_shared;    /**< holds data shared across multiple instances of the shared pointer */
    T* m_ptr;                                                   /**< the pointer that we wish to reference count */
    TDestructor* m_objectDestructor;                            /**< manditory external object destructor */

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
    CTBasicSharedPtr( void );

    CTBasicSharedPtr( T* ptr                                         ,
                      CTDynamicDestructorBase< T >* objectDestructor );

    // inlined copy constructor, has to be inlined in class definition for now due to VC6 limitations
    template< class Derived >
    CTBasicSharedPtr( const CTBasicSharedPtr< Derived, LockType >& src )
        : m_shared( GUCEF_NULL )
        , m_ptr( GUCEF_NULL )
        , m_objectDestructor( GUCEF_NULL )
    {GUCEF_TRACE;

        InitializeUsingInheritance( src );
    }

    CTBasicSharedPtr( const CTBasicSharedPtr& src );

    virtual ~CTBasicSharedPtr();

    const TBasicSharedPtrSharedData< LockType >* GetSharedData( void ) const
    {GUCEF_TRACE;

        return m_shared;
    }

    template< class RelatedClass >
    bool InitializeUsingInheritance( const CTBasicSharedPtr< RelatedClass, LockType >& other )
    {GUCEF_TRACE;

        Unlink();

        MT::CObjectScopeLock lockOther( other );
        MT::CObjectScopeLock lock( this );

        // The static cast below is performed as a compile time validation
        // of the type passed.
        T* relatedClass = static_cast< T* >( const_cast< RelatedClass* >( other.GetPointerAlways() ) );
        if ( GUCEF_NULL != relatedClass )
        {
            m_ptr = relatedClass;
            m_objectDestructor = reinterpret_cast< CTBasicSharedPtr< T, LockType >::TDestructor* >( other.GetDestructor() );
            m_shared = const_cast< TBasicSharedPtrSharedData< LockType >* >( other.GetSharedData() );

            ++(m_shared->m_refCounter);
            return true;
        }
        return false;
    }

    // implemented inline as a workaround for VC6 issues
    template< class Derived >
    CTBasicSharedPtr& operator=( const CTBasicSharedPtr< Derived, LockType >& src )
    {
        if ( &reinterpret_cast< const CTBasicSharedPtr& >( src ) != this )
        {
            InitializeUsingInheritance( src );
        }
        return *this;
    }

    // implemented inline as a workaround for VC6 issues
    // The dummy param is a VC6 hack for templated member functions
    template< class Derived >
    CTBasicSharedPtr< Derived, LockType > StaticCast( bool dummy = true )
    {
        // We use the initialization function of the derived type's
        // shared pointer which will have knowledge of both types.
        CTBasicSharedPtr< Derived, LockType > retVal;
        retVal.InitializeUsingInheritance( *this );
        return retVal;
    }

    CTBasicSharedPtr& operator=( const CTBasicSharedPtr& src );

    CTBasicSharedPtr& operator=( int nullValue );

    inline bool operator==( Int32 nullValue ) const;

    inline bool operator==( Int64 nullValue ) const;

    inline bool operator==( const void* other ) const;

    inline bool operator==( const CTBasicSharedPtr& other ) const;

    inline bool operator!=( Int32 other ) const;

    inline bool operator!=( Int64 other ) const;

    inline bool operator!=( const void* other ) const;

    inline bool operator!=( const CTBasicSharedPtr& other ) const;

    inline bool operator<( const CTBasicSharedPtr& other ) const;

    /**
     *  operator that implements '(*mySharedPtr)'
     *
     *  @throws ENotInitialized if the pointer is not initialized
     */
    inline T& operator*( void );

    /**
     *  operator that implements '(*mySharedPtr)'
     *
     *  @throws ENotInitialized if the pointer is not initialized
     */
    inline const T& operator*( void ) const;

    /**
     *  operator that emulates 'myPointer->'
     *
     *  @throws ENotInitialized if the pointer is not initialized
     */
    inline T* operator->( void );

    /**
     *  operator that emulates 'myPointer->'
     *
     *  @throws ENotInitialized if the pointer is not initialized
     */
    inline const T* operator->( void ) const;

    /**
     *  Conversion operator to bool to facilitate easy ! etc checks against the
     *  pointer being NULL as some people like to do versus an explicit NULL == check.
     */
    inline operator bool() const;

    inline T* GetPointer( void );

    inline const T* GetPointer( void ) const;

    inline T* GetPointerAlways( void );

    inline const T* GetPointerAlways( void ) const;

    inline bool IsNULL( void ) const;

    UInt32 GetReferenceCount( void ) const;

    TDestructor* GetDestructor( void ) const;

    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( ENotInitialized );

    protected:

    /**
     *  Can be used by descending classes to implement in-scope memory management
     *  utilizing method 1 as described above.
     */
    //void SetToNULL( void );

    /**
     *  Can be used be decending implementations for late initialization.
     */
    void Initialize( T* ptr                        ,
                     TDestructor* objectDestructor );

    /**
     *  Simply sets the object destructor pointer overwriting whatever
     *  the values used to be.
     *  The pointer has to be valid.
     */
    void OverrideDestructor( TDestructor* newObjectDestructor );

    void Unlink( void );

    /**
     *  Actual locking behaviour depends on the LockType passed to the template
     */
    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Actual locking behaviour depends on the LockType passed to the template
     */
    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Creates a clone of the basic shared pointer
     *  Note that this increases the reference count like any copy would
     */
    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtr< T, LockType >::CTBasicSharedPtr( void )
    : m_shared( GUCEF_NULL )
    , m_ptr( GUCEF_NULL )
    , m_objectDestructor( GUCEF_NULL )
{GUCEF_TRACE;

    // Note that if this constructor is used an assignment is required at
    // a later time to initialize the shared pointer
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtr< T, LockType >::CTBasicSharedPtr( T* ptr                        ,
                                                   TDestructor* objectDestructor )
    : m_shared( GUCEF_NULL )
    , m_ptr( GUCEF_NULL )
    , m_objectDestructor( GUCEF_NULL )
{GUCEF_TRACE;

    if ( GUCEF_NULL != ptr )
    {
        m_shared = new TBasicSharedPtrSharedData< LockType >();
        m_shared->m_refCounter = 1UL;
        m_objectDestructor = objectDestructor;
        m_ptr = ptr;
    }
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtr< T, LockType >::CTBasicSharedPtr( const CTBasicSharedPtr< T, LockType >& src )
    : m_shared( GUCEF_NULL )
    , m_ptr( GUCEF_NULL )
    , m_objectDestructor( GUCEF_NULL )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( &src );
    if ( GUCEF_NULL != src.m_shared )
    {
        m_shared = src.m_shared;
        ++(m_shared->m_refCounter);
    }
    m_objectDestructor = src.m_objectDestructor;
    m_ptr = src.m_ptr;    
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtr< T, LockType >::~CTBasicSharedPtr()
{GUCEF_TRACE;

    Unlink();
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
void
CTBasicSharedPtr< T, LockType >::Initialize( T* ptr                        ,
                                             TDestructor* objectDestructor )
{GUCEF_TRACE;

    #ifdef GUCEF_DEBUG_MODE
    {
        MT::CObjectScopeLock lock( this );

        // If you get an assert here:
        //    You have an error in your decending class: you cannot initialize twice
        assert( m_ptr == GUCEF_NULL );
        assert( m_objectDestructor == GUCEF_NULL );
        assert( m_shared->m_refCounter == GUCEF_NULL );
    }
    #endif

    // Just in case we did not hit the asserts above because the code was compiled in release
    // mode without asserts we will still allow the scenario by unlinking first
    Unlink();

    MT::CObjectScopeLock lock( this );
    m_ptr = ptr;
    m_shared->m_refCounter = 1UL;
    m_objectDestructor = objectDestructor;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
void
CTBasicSharedPtr< T, LockType >::OverrideDestructor( TDestructor* newObjectDestructor )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_objectDestructor = newObjectDestructor;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
typename CTBasicSharedPtr< T, LockType >::TDestructor*
CTBasicSharedPtr< T, LockType >::GetDestructor( void ) const
{GUCEF_TRACE;

    return m_objectDestructor;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
bool
CTBasicSharedPtr< T, LockType >::IsNULL( void ) const
{GUCEF_TRACE;

    return GUCEF_NULL == m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
UInt32
CTBasicSharedPtr< T, LockType >::GetReferenceCount( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    return m_shared->m_refCounter;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtr< T, LockType >&
CTBasicSharedPtr< T, LockType >::operator=( int nullValue )
{GUCEF_TRACE;

    assert( nullValue == (int) GUCEF_NULL );
    Unlink();
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CTBasicSharedPtr< T, LockType >&
CTBasicSharedPtr< T, LockType >::operator=( const CTBasicSharedPtr< T, LockType >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        Unlink();

        MT::CObjectScopeLock lockSrc( src );

        m_shared = src.m_shared;
        ++m_shared->m_refCounter;
        m_ptr = src.m_ptr;
        m_objectDestructor = src.m_objectDestructor;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator<( const CTBasicSharedPtr< T, LockType >& other ) const
{GUCEF_TRACE;

    return m_ptr < other.m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator==( const CTBasicSharedPtr< T, LockType >& other ) const
{GUCEF_TRACE;

    return m_ptr == other.m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator==( const void* other ) const
{GUCEF_TRACE;

    return other == m_ptr;
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_32BIT

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator==( Int32 other ) const
{GUCEF_TRACE;

    if ( 0 == other )
        return GUCEF_NULL == m_ptr;
    if ( sizeof( int ) != sizeof( m_ptr ) )
        return false;
    return reinterpret_cast< void* >( other ) == m_ptr;
}

#else

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator==( Int32 other ) const
{GUCEF_TRACE;

    if ( 0 == other )
        return GUCEF_NULL == m_ptr;
    return false;
}

#endif

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_64BIT

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator==( Int64 other ) const
{GUCEF_TRACE;

    if ( 0 == other )
        return GUCEF_NULL == m_ptr;
    if ( sizeof( int ) != sizeof( m_ptr ) )
        return false;
    return reinterpret_cast< void* >( other ) == m_ptr;
}

#else

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator==( Int64 other ) const
{GUCEF_TRACE;

    if ( 0 == other )
        return GUCEF_NULL == m_ptr;
    return false;
}

#endif

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator!=( const CTBasicSharedPtr< T, LockType >& other ) const
{GUCEF_TRACE;

    return m_ptr != other.m_ptr;
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_32BIT

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator!=( Int32 other ) const
{GUCEF_TRACE;

    if ( 0 == other )
        return GUCEF_NULL != m_ptr;
    if ( sizeof( int ) != sizeof( m_ptr ) )
        return true;
    return reinterpret_cast< void* >( other ) != m_ptr;
}

#else

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator!=( Int32 other ) const
{GUCEF_TRACE;

    if ( 0 == other )
        return GUCEF_NULL != m_ptr;
    return true;
}

#endif

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_64BIT

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator!=( Int64 other ) const
{GUCEF_TRACE;

    if ( 0 == other )
        return GUCEF_NULL != m_ptr;
    if ( sizeof( int ) != sizeof( m_ptr ) )
        return true;
    return reinterpret_cast< void* >( other ) != m_ptr;
}

#else

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator!=( Int64 other ) const
{GUCEF_TRACE;

    if ( 0 == other )
        return GUCEF_NULL != m_ptr;
    return true;
}

#endif

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
CTBasicSharedPtr< T, LockType >::operator!=( const void* other ) const
{GUCEF_TRACE;

    return other != m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
operator==( const T* ptr, const CTBasicSharedPtr< T, LockType >& other )
{GUCEF_TRACE;

    return other == ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
operator==( const Int64 ptr, const CTBasicSharedPtr< T, LockType >& other )
{GUCEF_TRACE;

    return other == ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
operator==( const Int32 ptr, const CTBasicSharedPtr< T, LockType >& other )
{GUCEF_TRACE;

    return other == ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
operator!=( const T* ptr, const CTBasicSharedPtr< T, LockType >& other )
{GUCEF_TRACE;

    return other != ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
operator!=( Int32 intPtr, const CTBasicSharedPtr< T, LockType >& other )
{GUCEF_TRACE;

    return other != intPtr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline bool
operator!=( Int64 intPtr, const CTBasicSharedPtr< T, LockType >& other )
{GUCEF_TRACE;

    return other != intPtr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline T&
CTBasicSharedPtr< T, LockType >::operator*( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( GUCEF_NULL != m_ptr )
    {
        return *(m_ptr);
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T, LockType >::operator*( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline const T&
CTBasicSharedPtr< T, LockType >::operator*( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( GUCEF_NULL != m_ptr )
    {
        return *m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T, LockType >::operator*( void ) const: uninitialized pointer usage" );

}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline T*
CTBasicSharedPtr< T, LockType >::operator->( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( GUCEF_NULL != m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T, LockType >::operator->( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline const T*
CTBasicSharedPtr< T, LockType >::operator->( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( GUCEF_NULL != m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T, LockType >::operator->( void ) const: uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline
CTBasicSharedPtr< T, LockType >::operator bool() const
{GUCEF_TRACE;

    return GUCEF_NULL != m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline T*
CTBasicSharedPtr< T, LockType >::GetPointer( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( GUCEF_NULL != m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T, LockType >::operator->( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline const T*
CTBasicSharedPtr< T, LockType >::GetPointer( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( GUCEF_NULL != m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T, LockType >::operator->( void ) const: uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline T*
CTBasicSharedPtr< T, LockType >::GetPointerAlways( void )
{GUCEF_TRACE;

    return m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
inline const T*
CTBasicSharedPtr< T, LockType >::GetPointerAlways( void ) const
{GUCEF_TRACE;

    return m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
void
CTBasicSharedPtr< T, LockType >::Unlink( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_shared )
    {
        MT::CObjectScopeLock lock( this );
        if ( GUCEF_NULL != m_ptr )
        {
            --m_shared->m_refCounter;
            assert( m_shared->m_refCounter >= 0 );
            if ( 0 >= m_shared->m_refCounter )
            {
                // We should check if the destructor pointer is not-NULL
                // A descending class may NULL it for its own purposes.
                if ( GUCEF_NULL != m_objectDestructor )
                {
                    m_objectDestructor->DestroyObject( m_ptr );
                    m_objectDestructor = GUCEF_NULL;
                }
                m_ptr = GUCEF_NULL;

                TBasicSharedPtrSharedData< LockType >* localSharedRef = m_shared;
                m_shared = GUCEF_NULL;
                localSharedRef->m_lock.Unlock();
                delete localSharedRef;
            }
        }
    }

    // this object may not have been the last reference but we still have to NULL
    // the attributes to allow this object to be re-used
    m_shared = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

//template< typename T, class LockType >
//void
//CTBasicSharedPtr< T, LockType >::SetToNULL( void )
//{GUCEF_TRACE;
//
//    MT::CObjectScopeLock lock( this );
//
//    m_objectDestructor = GUCEF_NULL;
//    delete m_shared;
//    m_shared = GUCEF_NULL;
//    m_ptr = GUCEF_NULL;
//}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
const MT::CILockable*
CTBasicSharedPtr< T, LockType >::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
bool
CTBasicSharedPtr< T, LockType >::Lock( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_shared )
    {
        m_shared = new TBasicSharedPtrSharedData< LockType >();
    }
    return m_shared->m_lock.Lock();
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
bool
CTBasicSharedPtr< T, LockType >::Unlock( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_shared )
        return m_shared->m_lock.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

template< typename T, class LockType >
CICloneable*
CTBasicSharedPtr< T, LockType >::Clone( void ) const
{GUCEF_TRACE;

    return new CTBasicSharedPtr< T, LockType >( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTBASICSHAREDPTR_H ? */
