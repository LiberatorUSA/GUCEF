/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef GUCEF_CORE_CTLINKEDCLONEABLE_H
#define GUCEF_CORE_CTLINKEDCLONEABLE_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_EXCEPTIONCLASSMACROS_H
#include "ExceptionClassMacros.h"   
#define GUCEF_CORE_EXCEPTIONCLASSMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONCLASSMACROS_H ? */

#ifndef GUCEF_CORE_EXCEPTIONTHROWMACROS_H
#include "ExceptionThrowMacros.h"
#define GUCEF_CORE_EXCEPTIONTHROWMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONTHROWMACROS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

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
 *  Contrary to the CTCloneableObj ans CTLinkedCloneable templates this one
 *  does not use T as a non-cloning-capable object but instead acts as a 
 *  cloning capable pointer to the cloneable.
 *  
 *  Please note that this is distinct from a Shared pointer which is about
 *  delegating lifecycle management of the object.
 *  This template is intended to be used as light weight references to avoid 
 *  costly creation of objects needlessly, such as internal buffer, where we do NOT want
 *  to delegate lifecycle management. We'd want to reuse the buffer after say
 *  an event notification referencing the buffer contents. In this example 
 *  scenario it may be needed to store the referenced data in a queue for
 *  processing by another thread in which case the data can be auto-cloned
 *  upon queue insertion.
 */
template< typename T > 
class CTConstLinkedCloneable : public CICloneable
{
    public:

    CTConstLinkedCloneable( void );
    
    // preserves the link, does not copy the wrapped object
    explicit CTConstLinkedCloneable( const T* data );

    // creates a copy of the wrapped object
    CTConstLinkedCloneable( const CTConstLinkedCloneable& src );
        
    virtual ~CTConstLinkedCloneable() GUCEF_VIRTUAL_OVERRIDE;
    
    CTConstLinkedCloneable& operator=( const CTConstLinkedCloneable& src );
    
    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    inline operator bool() const;

    inline bool operator<( const CTConstLinkedCloneable& other ) const;

    inline const T* operator->( void ) const;

    /**
     *  Because const linkage is mandated using this will cause a private copy to be created
     */
    inline T* operator->( void );

    inline bool IsNULL( void ) const;
    
    void LinkTo( const T* data );

    const T& GetData( void ) const;

    /**
     *  Because const linkage is mandated using this will cause a private copy to be created
     */
    T& GetData( void );

    void Clear( void );

    inline operator const T*() const;

    /**
     *  Because const linkage is mandated using this will cause a private copy to be created
     */
    inline operator T*();

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( ENULLPointer );

    protected:
    const T* m_ptr;
    bool m_linked;
};

/*-------------------------------------------------------------------------*/

template< typename T > 
class CTLinkedCloneable : public CICloneable
{
    public:

    CTLinkedCloneable( void );
    
    // preserves the link, does not copy the wrapped object
    explicit CTLinkedCloneable( T* data );

    // creates a copy of the wrapped object
    CTLinkedCloneable( const CTLinkedCloneable& src );
        
    virtual ~CTLinkedCloneable() GUCEF_VIRTUAL_OVERRIDE;
    
    CTLinkedCloneable& operator=( const CTLinkedCloneable& src );
    
    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    inline operator bool() const;

    inline bool operator<( const CTLinkedCloneable& other ) const;

    inline const T* operator->( void ) const;

    inline bool IsNULL( void ) const;
    
    void LinkTo( T* data );

    const T& GetData( void ) const;

    T& GetData( void );

    void Clear( void );

    inline operator const T*() const;

    inline operator T*();

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( ENULLPointer );

    protected:
    T* m_ptr;
    bool m_linked;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTConstLinkedCloneable< T >::CTConstLinkedCloneable( void )
    : m_ptr( GUCEF_NULL ) ,
      m_linked( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTConstLinkedCloneable< T >::CTConstLinkedCloneable( const T* data )
    : m_ptr( data )                 ,
      m_linked( GUCEF_NULL != m_ptr )
{GUCEF_TRACE;

}


/*-------------------------------------------------------------------------*/

template< typename T >
CTConstLinkedCloneable< T >::CTConstLinkedCloneable( const CTConstLinkedCloneable< T >& src )
    : m_ptr( GUCEF_NULL ) ,
      m_linked( false )
{GUCEF_TRACE;

    if ( GUCEF_NULL != src.m_ptr )
    {
        m_ptr = static_cast< T* >( src.m_ptr->Clone() );
    }
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTConstLinkedCloneable< T >::~CTConstLinkedCloneable()
{GUCEF_TRACE;
    
    Clear();
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
void
CTConstLinkedCloneable< T >::Clear( void )
{GUCEF_TRACE;

    if ( !m_linked )
    {
        GUCEF_DELETE m_ptr;
    }
    m_ptr = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTConstLinkedCloneable< T >& 
CTConstLinkedCloneable< T >::operator=( const CTConstLinkedCloneable< T >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        Clear();
        if ( GUCEF_NULL != src.m_ptr )
            m_ptr = static_cast< T* >( src.m_ptr->Clone() );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T >
void
CTConstLinkedCloneable< T >::LinkTo( const T* ptr )
{GUCEF_TRACE;

    Clear();
    m_ptr = ptr;
    m_linked = GUCEF_NULL != ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline const T*
CTConstLinkedCloneable< T >::operator->( void ) const
{GUCEF_TRACE;

    return m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline T*
CTConstLinkedCloneable< T >::operator->( void )
{GUCEF_TRACE;

    // mutable access requires a private copy
    if ( GUCEF_NULL != m_ptr && m_linked )
    {
        m_ptr = static_cast< T* >( m_ptr->Clone() );
        m_linked = false;
    }
    return m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool
CTConstLinkedCloneable< T >::operator<( const CTConstLinkedCloneable< T >& other ) const
{GUCEF_TRACE;

    return m_ptr < other.m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline 
CTConstLinkedCloneable< T >::operator bool() const
{GUCEF_TRACE;

    return GUCEF_NULL != m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline 
CTConstLinkedCloneable< T >::operator const T*() const
{GUCEF_TRACE;

    return m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline 
CTConstLinkedCloneable< T >::operator T*()
{GUCEF_TRACE;

    // mutable access requires a private copy
    if ( GUCEF_NULL != m_ptr && m_linked )
    {
        m_ptr = static_cast< T* >( m_ptr->Clone() );
        m_linked = false;
    }
    return const_cast< T* >( m_ptr );
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool
CTConstLinkedCloneable< T >::IsNULL() const
{GUCEF_TRACE;

    return GUCEF_NULL == m_ptr;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CICloneable* 
CTConstLinkedCloneable< T >::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CTConstLinkedCloneable< T >( *this );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
const T&
CTConstLinkedCloneable< T >::GetData( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_ptr )
    {
        return *m_ptr;
    }
    
    GUCEF_EMSGTHROW( ENULLPointer, "CTConstLinkedCloneable::GetData(): This operation is impossible without a valid object" );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
T&
CTConstLinkedCloneable< T >::GetData( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_ptr )
    {
        // mutable access requires a private copy
        if ( m_linked )
        {
            m_ptr = static_cast< T* >( m_ptr->Clone() );
            m_linked = false;
        }
        return const_cast< T& >( *m_ptr );
    }
    
    GUCEF_EMSGTHROW( ENULLPointer, "CTConstLinkedCloneable::GetData(): This operation is impossible without a valid object" );
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTLinkedCloneable< T >::CTLinkedCloneable( void )
    : m_ptr( GUCEF_NULL ) ,
      m_linked( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTLinkedCloneable< T >::CTLinkedCloneable( T* data )
    : m_ptr( data )                 ,
      m_linked( GUCEF_NULL != m_ptr )
{GUCEF_TRACE;

}


/*-------------------------------------------------------------------------*/

template< typename T >
CTLinkedCloneable< T >::CTLinkedCloneable( const CTLinkedCloneable< T >& src )
    : m_ptr( GUCEF_NULL ) ,
      m_linked( false )
{GUCEF_TRACE;

    if ( GUCEF_NULL != src.m_ptr )
    {
        m_ptr = static_cast< T* >( src.m_ptr->Clone() );
    }
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTLinkedCloneable< T >::~CTLinkedCloneable()
{GUCEF_TRACE;
    
    Clear();
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
void
CTLinkedCloneable< T >::Clear( void )
{GUCEF_TRACE;

    if ( !m_linked )
    {
        GUCEF_DELETE m_ptr;
    }
    m_ptr = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTLinkedCloneable< T >& 
CTLinkedCloneable< T >::operator=( const CTLinkedCloneable< T >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        Clear();
        if ( GUCEF_NULL != src.m_ptr )
            m_ptr = static_cast< T* >( src.m_ptr->Clone() );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T >
void
CTLinkedCloneable< T >::LinkTo( T* ptr )
{GUCEF_TRACE;

    Clear();
    m_ptr = ptr;
    m_linked = GUCEF_NULL != ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline const T*
CTLinkedCloneable< T >::operator->( void ) const
{GUCEF_TRACE;

    return m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool
CTLinkedCloneable< T >::operator<( const CTLinkedCloneable< T >& other ) const
{GUCEF_TRACE;

    return m_ptr < other.m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline 
CTLinkedCloneable< T >::operator bool() const
{GUCEF_TRACE;

    return GUCEF_NULL != m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline 
CTLinkedCloneable< T >::operator const T*() const
{GUCEF_TRACE;

    return m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline 
CTLinkedCloneable< T >::operator T*()
{GUCEF_TRACE;

    return m_ptr;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool
CTLinkedCloneable< T >::IsNULL() const
{GUCEF_TRACE;

    return GUCEF_NULL == m_ptr;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CICloneable* 
CTLinkedCloneable< T >::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CTLinkedCloneable< T >( *this );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
const T&
CTLinkedCloneable< T >::GetData( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_ptr )
    {
        return *m_ptr;
    }
    
    GUCEF_EMSGTHROW( ENULLPointer, "CTLinkedCloneable::GetData(): This operation is impossible without a valid object" );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
T&
CTLinkedCloneable< T >::GetData( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_ptr )
    {
        return *m_ptr;
    }
    
    GUCEF_EMSGTHROW( ENULLPointer, "CTLinkedCloneable::GetData(): This operation is impossible without a valid object" );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTLINKEDCLONEABLE_H ? */
