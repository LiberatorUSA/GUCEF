/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#ifndef GUCEF_CORE_CTBASICSHAREDPTR_H
#define GUCEF_CORE_CTBASICSHAREDPTR_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H
#include "CTSharedObjectDestructor.h"
#define GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H
#endif /* GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* module macro's */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#include "ExceptionMacros.h"

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

/**
 *  Templated implementation of a shared pointer.
 *
 *  A shared pointer adds some safety to pointers that get passed
 *  arround where objects keep a reference to object instances independant
 *  of eachother. In such situations it can become unclear who is responsible 
 *  for destroying the refrenced object instance.
 *  The shared pointer will reference count the pointer and ensure that even 
 *  though the original owner has no knowledge of other classes referencing the 
 *  object it can safely manage object destruction as desired.
 *
 *  Note that the usage of an external destructor for the shared object by means of the
 *  CTSharedObjectDestructor template is manditory. By passing in a pointer to such a class
 *  you basicly create a callback link for cleanup purposes.
 *
 *  Note that this shared pointer implementation is by no means threadsafe and should 
 *  never be used across thread-boundries.
 */
template< typename T >
class CTBasicSharedPtr
{
    public:
    
    typedef CTSharedObjectDestructor< T >   TDestructor;
    
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
    
    CTBasicSharedPtr( T* ptr                        ,
                      TDestructor* objectDestructor );
 
    CTBasicSharedPtr( T& ptr                        ,
                      TDestructor* objectDestructor );
 
    // inlined copy constructor, has to be inlined in class definition for now due to VC6 limitations
    template< class Derived >
    CTBasicSharedPtr( const CTBasicSharedPtr< Derived >& src )
        : m_ptr(
              static_cast< Derived* >( reinterpret_cast< const CTBasicSharedPtr& >( src ).m_ptr ) ),
          m_refCounter(
              reinterpret_cast< const CTBasicSharedPtr& >( src ).m_refCounter ),
          m_objectDestructor(
              reinterpret_cast< const CTBasicSharedPtr& >( src ).m_objectDestructor )
    {   
        // regarding the initializer list:
        //   We use reinterpret_cast to make use of the automatic same-class-friend-relationship
        //   which allows us to access the data members of the given src.
        //   We perform an additional static_cast on the pointer to ensure that the template argument
        //   is actually part of the same inheritance chain.
    
        if ( m_refCounter )
        {        
            ++(*m_refCounter);
        }
    }
    
    CTBasicSharedPtr( const CTBasicSharedPtr& src );
    
    virtual ~CTBasicSharedPtr();
    
    // implemented inline as a workaround for VC6 issues
    template< class Derived >
    CTBasicSharedPtr& operator=( const CTBasicSharedPtr< Derived >& src )
    {
        //TICS !GOLC-004: is protected
        if ( &reinterpret_cast< const CTBasicSharedPtr& >( src ) != this )
        {
            Unlink();
        
            // We use reinterpret_cast to make use of the automatic same-class-friend-relationship
            // which allows us to access the data members of the given src.
            // We perform an additional static_cast on the pointer to ensure that the template argument
            // is actually part of the same inheritance chain.
            m_ptr = static_cast< Derived* >( reinterpret_cast< const CTBasicSharedPtr& >( src ).m_ptr );
            m_refCounter = reinterpret_cast< const CTBasicSharedPtr& >( src ).m_refCounter;
            m_objectDestructor = reinterpret_cast< const CTBasicSharedPtr& >( src ).m_objectDestructor;
            
            if ( m_refCounter )
            {
                ++(*m_refCounter);
            }
        }
        return *this;
    }

    // implemented inline as a workaround for VC6 issues
    // The dummy param is a VC6 hack for templated member functions
    template< class Derived >
    CTBasicSharedPtr< Derived > StaticCast( bool dummy = true )
    {
            ++(*m_refCounter);
            
            CTBasicSharedPtr< Derived > retVal;
            retVal.m_ptr = static_cast< Derived* >( m_ptr );
            retVal.m_refCounter = m_refCounter;
            retVal.m_objectDestructor = reinterpret_cast< CTBasicSharedPtr< Derived >::TDestructor* >( m_objectDestructor );

            return retVal;
    }
 
    CTBasicSharedPtr& operator=( const CTBasicSharedPtr& src );
 
    inline bool operator==( const T* other ) const;
    
    inline bool operator==( const CTBasicSharedPtr& other ) const;
 
    inline bool operator!=( const T* other ) const;
    
    inline bool operator!=( const CTBasicSharedPtr& other ) const;
 
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
    
    inline T* GetPointer( void );
    
    inline const T* GetPointer( void ) const;
    
    inline bool IsNULL( void ) const;

    UInt32 GetReferenceCount( void ) const;

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( ENotInitialized );

    protected:

    /**
     *  Can be used by descending classes to implement in-scope memory management
     *  utilizing method 1 as described above.
     */
    void SetToNULL( void );

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

    TDestructor* GetDestructor( void ) const;
    
    void Unlink( void );
    
    private:
    
    T* m_ptr;                        /**< the pointer that we wish to reference count */
    UInt32* m_refCounter;            /**< shared reference counter */
    TDestructor* m_objectDestructor; /**< manditory external object destructor */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTBasicSharedPtr< T >::CTBasicSharedPtr( void )
        : m_ptr( NULL )                          ,
          m_refCounter( NULL )                   ,
          m_objectDestructor( NULL )
{TRACE;
    // Note that if this constructor is used an assignment is required at
    // a later time to initialize the shared pointer
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTBasicSharedPtr< T >::CTBasicSharedPtr( T* ptr                        ,
                                         TDestructor* objectDestructor )
        : m_ptr( ptr )                           ,
          m_refCounter( NULL )                   ,
          m_objectDestructor( objectDestructor )
{TRACE;

    if ( ptr )
    {
        m_refCounter = new UInt32( 1UL );
    }
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTBasicSharedPtr< T >::CTBasicSharedPtr( T& ptr                        ,
                                         TDestructor* objectDestructor )
        : m_ptr( &ptr )                         ,
          m_refCounter( new UInt32( 1UL ) )     ,
          m_objectDestructor( objectDestructor )          
{TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTBasicSharedPtr< T >::CTBasicSharedPtr( const CTBasicSharedPtr< T >& src )
        : m_ptr( src.m_ptr )                           ,
          m_refCounter( src.m_refCounter )             ,
          m_objectDestructor( src.m_objectDestructor )
{TRACE;

    if ( m_refCounter )
    {        
        ++(*m_refCounter);
    }
}

/*-------------------------------------------------------------------------*/
   
template< typename T >
CTBasicSharedPtr< T >::~CTBasicSharedPtr()
{TRACE;

    Unlink();
}

/*-------------------------------------------------------------------------*/

template< typename T >
void
CTBasicSharedPtr< T >::Initialize( T* ptr                        ,
                                   TDestructor* objectDestructor )
{TRACE;

    // If you get an assert here:
    //    You have an error in your decending class: you cannot initialize twice
    assert( m_ptr == NULL );
    assert( m_objectDestructor == NULL );
    assert( m_refCounter == NULL );

    objectDestructor->DestroySharedObject( NULL );

    *this = CTBasicSharedPtr< T >( ptr, objectDestructor );
}

/*-------------------------------------------------------------------------*/

template< typename T >
void 
CTBasicSharedPtr< T >::OverrideDestructor( TDestructor* newObjectDestructor )
{TRACE;

    m_objectDestructor = newObjectDestructor;
}

/*-------------------------------------------------------------------------*/

template< typename T >
typename CTBasicSharedPtr< T >::TDestructor*
CTBasicSharedPtr< T >::GetDestructor( void ) const
{TRACE;

    return m_objectDestructor;
}

/*-------------------------------------------------------------------------*/

template< typename T >
bool
CTBasicSharedPtr< T >::IsNULL( void ) const
{TRACE;

    return NULL == m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
UInt32 
CTBasicSharedPtr< T >::GetReferenceCount( void ) const
{TRACE;

    if ( m_refCounter )
    {
        return *m_refCounter;
    }
    return 0UL;
}

/*-------------------------------------------------------------------------*/
  
template< typename T >
CTBasicSharedPtr< T >& 
CTBasicSharedPtr< T >::operator=( const CTBasicSharedPtr< T >& src )
{TRACE;

    if ( this != &src )
    {
        Unlink();
        
        m_ptr = src.m_ptr;
        m_refCounter = src.m_refCounter;
        m_objectDestructor = src.m_objectDestructor;
        
        if ( m_refCounter )
        {
            ++(*m_refCounter);
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTBasicSharedPtr< T >::operator==( const CTBasicSharedPtr< T >& other ) const
{TRACE;
    
    return m_ptr == other.m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTBasicSharedPtr< T >::operator==( const T* other ) const
{TRACE;

    return other == m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
operator==( const T* ptr, const CTBasicSharedPtr< T >& other )
{TRACE;

    return other == ptr;
}

/*-------------------------------------------------------------------------*/

/*
 *  workaround for comparison to NULL, since NULL is a integer by default 
 */
template< typename T >
inline bool 
operator==( const int ptr, const CTBasicSharedPtr< T >& other )
{TRACE;

    assert( NULL == ptr );
    return static_cast< const T* >( NULL ) == other;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline bool 
CTBasicSharedPtr< T >::operator!=( const CTBasicSharedPtr< T >& other ) const
{TRACE;

    return m_ptr != other.m_ptr;    
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTBasicSharedPtr< T >::operator!=( const T* other ) const
{TRACE;

    return other != m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
operator!=( const T* ptr, const CTBasicSharedPtr< T >& other )
{TRACE;

    return other != ptr;
}

/*-------------------------------------------------------------------------*/

/*
 *  workaround for comparison to NULL, since NULL is a integer by default 
 */
template< typename T >
inline bool 
operator!=( const int ptr, const CTBasicSharedPtr< T >& other )
{TRACE;

    assert( NULL == ptr );
    return static_cast< const T* >( NULL ) != other;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline T& 
CTBasicSharedPtr< T >::operator*( void )
{TRACE;

    if ( m_ptr )
    {
        return *m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T >::operator*( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline const T& 
CTBasicSharedPtr< T >::operator*( void ) const
{TRACE;

    if ( m_ptr )
    {
        return *m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T >::operator*( void ) const: uninitialized pointer usage" );

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline T*
CTBasicSharedPtr< T >::operator->( void )
{TRACE;

    if ( m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T >::operator->( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline const T* 
CTBasicSharedPtr< T >::operator->( void ) const
{TRACE;

    if ( m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T >::operator->( void ) const: uninitialized pointer usage" );    
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline T* 
CTBasicSharedPtr< T >::GetPointer( void )
{TRACE;

    if ( m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T >::operator->( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline const T* 
CTBasicSharedPtr< T >::GetPointer( void ) const
{TRACE;

    if ( m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTBasicSharedPtr< T >::operator->( void ) const: uninitialized pointer usage" ); 
}

/*-------------------------------------------------------------------------*/

template< typename T >
void 
CTBasicSharedPtr< T >::Unlink( void )
{TRACE;

    if ( m_ptr )
    {
        --(*m_refCounter);
        if ( 0UL == *m_refCounter )
        {
            // We should check if the destructor pointer is not-NULL
            // A descending class may NULL it for its own purposes.
            if ( NULL != m_objectDestructor )
            {
                m_objectDestructor->DestroySharedObject( m_ptr );
                m_objectDestructor = NULL;
            }
            
            delete m_refCounter;
            m_refCounter = NULL;

            m_ptr = NULL;
        }
    }

    // this object may not have been the last refrence but we still have to NULL
    // the attributes to allow this onject to be re-used
    m_objectDestructor = NULL;
    m_refCounter = NULL;
    m_ptr = NULL;
}

/*-------------------------------------------------------------------------*/

template< typename T >
void 
CTBasicSharedPtr< T >::SetToNULL( void )
{
    m_objectDestructor = NULL;
    delete m_refCounter;
    m_refCounter = NULL;
    m_ptr = NULL;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 14-12-2005 :
        - Dinand: Moved code into this new baseclass from CTSharedPtr to allow
          some level of shared pointer usage without actually requiring the type
          to be defined. ie you can create a shared pointer with a forward declaration.

-----------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTBASICSHAREDPTR_H ? */
