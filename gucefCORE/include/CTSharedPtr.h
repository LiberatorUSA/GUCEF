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

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#define GUCEF_CORE_CTSHAREDPTR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>
#include "gucefCORE_macros.h"
#include "CTSharedObjectDestructor.h"
#include "ExceptionMacros.h"
#include "CTracer.h"

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
 *  arround where objects keep a refrence to object instances independant
 *  of eachother. In such situations it can become unclear who is responsible 
 *  for destroying the refrenced object instance.
 *  The shared pointer will refrence count the pointer and ensure that even 
 *  though the original owner has no knowledge of other classes refrencing the 
 *  object it can safely manage object destruction as desired.
 *
 *  Note that the default behaviour of this template is to delete the refrenced
 *  object instance when the refrence count hits zero. This raises some 
 *  "out-of-scope-memory-management" concerns. While this can be the intended 
 *  behaviour, it is advised to alter this behaviour to provide an in-scope mechanism
 *  ( ie delegating destruction responsibilities to the original creator of the object ).
 *
 *      Method 1:
 *          inherit from this template and redirect instance destruction
 *          This is achieved by overriding the destructor.
 *
 *      Method 2:
 *          Make the owner of the refrenced object inherit from CTSharedObjectDestructor
 *          for the given type. Pass the owner in when creating the pointer object.
 *          This will result in in-scope memory management.
 *
 *  Note that this shared pointer implementation is by no means threadsafe and should 
 *  never be used across thread-boundries.
 */
template< typename T >
class CTSharedPtr
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
    CTSharedPtr( void );
    
    CTSharedPtr( T* ptr                               ,
                 TDestructor* objectDestructor = NULL );

    CTSharedPtr( T& ptr                               ,
                 TDestructor* objectDestructor = NULL );
    
    CTSharedPtr( const CTSharedPtr& src );
    
    virtual ~CTSharedPtr();
    
    CTSharedPtr& operator=( const CTSharedPtr& src );

    bool operator==( const T* other ) const;
    
    bool operator==( const CTSharedPtr& other ) const;

    bool operator!=( const T* other ) const;
    
    bool operator!=( const CTSharedPtr& other ) const;

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

    UInt32 GetRefrenceCount( void ) const;
    
    bool IsNULL( void ) const;

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( ENotInitialized );

    protected:

    /**
     *  Can be used by descending classes to implement in-scope memory management
     *  utilizing method 1 as described above.
     */
    void SetToNULL( void );
    
    private:
    
    void Unlink( void );
    
    private:
    
    T* m_ptr;                        /**< the pointer that we wish to refrence count */
    UInt32* m_refCounter;            /**< shared refrence counter */
    TDestructor* m_objectDestructor; /**< optional external object destructor */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( void )
        : m_ptr( NULL )              ,
          m_refCounter( NULL )       ,
          m_objectDestructor( NULL )
{
    // Note that if this constructor is used an assignment is required at
    // a later time to initialize the shared pointer
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( T* ptr                                     ,
                               TDestructor* objectDestructor /* = NULL */ )
        : m_ptr( ptr )                           ,
          m_refCounter( new UInt32( 1UL ) )      ,
          m_objectDestructor( objectDestructor )
{
    assert( ptr );
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( T& ptr                                     ,
                               TDestructor* objectDestructor /* = NULL */ )
        : m_ptr( &ptr )                         ,
          m_refCounter( new UInt32( 1UL ) )     ,
          m_objectDestructor( objectDestructor )          
{
    assert( m_ptr );    
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( const CTSharedPtr< T >& src )
        : m_ptr( src.m_ptr )                           ,
          m_refCounter( src.m_refCounter )             ,
          m_objectDestructor( src.m_objectDestructor )
{
    if ( m_refCounter )
    {
        ++(*m_refCounter);
    }
}

/*-------------------------------------------------------------------------*/
   
template< typename T >
CTSharedPtr< T >::~CTSharedPtr()
{
    Unlink();
}

/*-------------------------------------------------------------------------*/

template< typename T >
UInt32 
CTSharedPtr< T >::GetRefrenceCount( void ) const
{
    if ( m_refCounter )
    {
        return *m_refCounter;
    }
    return 0UL;
}

/*-------------------------------------------------------------------------*/
  
template< typename T >
CTSharedPtr< T >& 
CTSharedPtr< T >::operator=( const CTSharedPtr< T >& src )
{
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
CTSharedPtr< T >::operator==( const CTSharedPtr< T >& other ) const
{
    return m_ptr == other.m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTSharedPtr< T >::operator==( const T* other ) const
{
    return other == m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
operator==( const T* ptr, const CTSharedPtr< T >& other )
{
    return other == ptr;
}

/*-------------------------------------------------------------------------*/

/*
 *  workaround for comparison to NULL, since NULL is a integer by default 
 */
template< typename T >
inline bool 
operator==( const int ptr, const CTSharedPtr< T >& other )
{
    assert( NULL == ptr );
    return static_cast< const T* >( NULL ) == other;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline bool 
CTSharedPtr< T >::operator!=( const CTSharedPtr< T >& other ) const
{
    return m_ptr != other.m_ptr;    
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTSharedPtr< T >::operator!=( const T* other ) const
{
    return other != m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
operator!=( const T* ptr, const CTSharedPtr< T >& other )
{
    return other != ptr;
}

/*-------------------------------------------------------------------------*/

/*
 *  workaround for comparison to NULL, since NULL is a integer by default 
 */
template< typename T >
inline bool 
operator!=( const int ptr, const CTSharedPtr< T >& other )
{
    assert( NULL == ptr );
    return static_cast< const T* >( NULL ) != other;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline T& 
CTSharedPtr< T >::operator*( void )
{
    if ( m_ptr )
    {
        return *m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTSharedPtr< T >::operator*( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline const T& 
CTSharedPtr< T >::operator*( void ) const
{
    if ( m_ptr )
    {
        return *m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTSharedPtr< T >::operator*( void ) const: uninitialized pointer usage" );

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline T*
CTSharedPtr< T >::operator->( void )
{
    if ( m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTSharedPtr< T >::operator->( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline const T* 
CTSharedPtr< T >::operator->( void ) const
{
    if ( m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTSharedPtr< T >::operator->( void ) const: uninitialized pointer usage" );    
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline T* 
CTSharedPtr< T >::GetPointer( void )
{
    if ( m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTSharedPtr< T >::operator->( void ): uninitialized pointer usage" );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline const T* 
CTSharedPtr< T >::GetPointer( void ) const
{
    if ( m_ptr )
    {
        return m_ptr;
    }

    // Someone forgot to initialize the shared pointer with an assignment
    GUCEF_EMSGTHROW( ENotInitialized, "CTSharedPtr< T >::operator->( void ) const: uninitialized pointer usage" ); 
}

/*-------------------------------------------------------------------------*/

template< typename T >
void 
CTSharedPtr< T >::Unlink( void )
{
    if ( m_ptr )
    {
        --(*m_refCounter);
        if ( 0UL == *m_refCounter )
        {
            if ( m_objectDestructor )
            {
                m_objectDestructor->DestroySharedObject( m_ptr );
            }
            else
            {
                delete m_ptr;
            }            
            delete m_refCounter;
        }
    }
    
    /* NULL the data members just in case */
    m_objectDestructor = NULL;
    m_refCounter = NULL;
    m_ptr = NULL;
}

/*-------------------------------------------------------------------------*/

template< typename T >
void 
CTSharedPtr< T >::SetToNULL( void )
{
    m_objectDestructor = NULL;
    delete m_refCounter;
    m_refCounter = NULL;
    m_ptr = NULL;
}

/*-------------------------------------------------------------------------*/

template< typename T >
bool 
CTSharedPtr< T >::IsNULL( void ) const
{
        return NULL == m_ptr;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */
