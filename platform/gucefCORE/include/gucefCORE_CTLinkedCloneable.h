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
 *  This template is intended to be used as light weight references to costly 
 *  to create needlessly objects, such as internal buffer, where we do NOT want
 *  to delegate lifecycle management. We'd want to reuse the buffer after say
 *  an event notification referencing the buffer contents.
 */
template< typename T > 
class CTLinkedCloneable : public CICloneable
{
    public:

    CTLinkedCloneable( void );
    
    // preserves the link, does not copy the wrapped object
    explicit CTLinkedCloneable( const T* data );

    // creates a copy of the wrapped object
    CTLinkedCloneable( const CTLinkedCloneable& src );
        
    virtual ~CTLinkedCloneable() GUCEF_VIRTUAL_OVERRIDE;
    
    CTLinkedCloneable& operator=( const CTLinkedCloneable& src );
    
    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    /**
     *  Conversion operator to bool to facilitate easy ! etc checks against the
     *  pointer being NULL as some people like to do versus an explicit NULL == check.
     */
    inline operator bool() const;

    inline bool operator<( const CTLinkedCloneable& other ) const;

    inline const T* operator->( void ) const;

    inline bool IsNULL( void ) const;
    
    void LinkTo( const T* data );

    void Clear( void );

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( ENULLPointer );

    private:
    const T* m_ptr;
    bool m_linked;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTLinkedCloneable< T >::CTLinkedCloneable( void )
    : m_ptr( GUCEF_NULL ) ,
      m_linked( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTLinkedCloneable< T >::CTLinkedCloneable( const T* data )
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
        delete m_ptr;
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
CTLinkedCloneable< T >::LinkTo( const T* ptr )
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
CICloneable* 
CTLinkedCloneable< T >::Clone( void ) const
{GUCEF_TRACE;

    return new CTLinkedCloneable< T >( *this );
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
