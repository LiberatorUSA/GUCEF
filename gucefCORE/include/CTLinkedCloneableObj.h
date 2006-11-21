/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#ifndef GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#define GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
 
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

#ifndef GUCEF_CORE_EXCEPTIONMACROS_H
#include "ExceptionMacros.h"
#define GUCEF_CORE_EXCEPTIONMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONMACROS_H ? */

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
 *  Similar to the CTCloneableObj template only with a twist
 *  This template will allow you to link to an object without having 
 *  to copy it. You can then pass the wrapped linked object to an interface
 *  that uses cloning. If the need for a clone arises then a real copy will 
 *  be made, but not before. This allows you to optimize passing large objects
 *  as cloneables.
 */
template< typename T > 
class CTLinkedCloneableObj : public CICloneable
{
    public:

    CTLinkedCloneableObj( void );
    
    explicit CTLinkedCloneableObj( const T* data );

    CTLinkedCloneableObj( const CTLinkedCloneableObj& src );
        
    virtual ~CTLinkedCloneableObj();
    
    CTLinkedCloneableObj& operator=( const CTLinkedCloneableObj& src );
    
    virtual CICloneable* Clone( void ) const;    
    
    const T& GetData( void ) const;

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( ENULLPointer );

    private:
    const T* m_data;
    bool m_linked;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTLinkedCloneableObj< T >::CTLinkedCloneableObj( void )
        : m_data( NULL )    ,
          m_linked( false )
{TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTLinkedCloneableObj< T >::CTLinkedCloneableObj( const T* data )
        : m_data( data )   ,
          m_linked( true )
{TRACE;

    if ( NULL == data )
    {
        GUCEF_EMSGTHROW( ENULLPointer, "CTLinkedCloneableObj( const T* data ): This operation is impossible without a valid object" );
    }
}


/*-------------------------------------------------------------------------*/

template< typename T >
CTLinkedCloneableObj< T >::CTLinkedCloneableObj( const CTLinkedCloneableObj< T >& src )
        : m_data( new T( *src.m_data ) ) ,
          m_linked( false )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTLinkedCloneableObj< T >::~CTLinkedCloneableObj()
{TRACE;
    
    if ( !m_linked )
    {
        delete m_data;
    }
    m_data = NULL;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTLinkedCloneableObj< T >& 
CTLinkedCloneableObj< T >::operator=( const CTLinkedCloneableObj< T >& src )
{TRACE;

    if ( this != &src )
    {
        if ( !m_linked )
        {
            delete m_data;
        }
        m_data = NULL;
        
        m_data = new T( *src.m_data );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CICloneable* 
CTLinkedCloneableObj< T >::Clone( void ) const
{TRACE;

    return new CTLinkedCloneableObj< T >( *this );
}

/*-------------------------------------------------------------------------*/

template< typename T >
const T& 
CTLinkedCloneableObj< T >::GetData( void ) const
{TRACE;

    if ( NULL != m_data )
    {
        return *m_data;
    }
    
    GUCEF_EMSGTHROW( ENULLPointer, "CTLinkedCloneableObj::GetData(): This operation is impossible without a valid object" );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-10-2006 :
        - Dinand: initial implementation

-----------------------------------------------------------------------------*/