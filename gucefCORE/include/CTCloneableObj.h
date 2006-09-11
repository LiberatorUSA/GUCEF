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

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#define GUCEF_CORE_CTCLONEABLEOBJ_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CICloneable.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_CPP
    #pragma warning( push )
#endif

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

template< typename T > 
class EXPORT_CPP CTCloneableObj : public CICloneable
{
    public:

    explicit CTCloneableObj( const T& data );

    CTCloneableObj( const CTCloneableObj& src );
        
    virtual ~CTCloneableObj();
    
    CTCloneableObj& operator=( const CTCloneableObj& src );
    
    virtual CICloneable* Clone( void ) const;    
    
    const T& GetData( void ) const;

    private:

    CTCloneableObj( void );

    private:
    T m_data;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTCloneableObj< T >::CTCloneableObj( const T& data )
        : m_data( data )
{
}


/*-------------------------------------------------------------------------*/

template< typename T >
CTCloneableObj< T >::CTCloneableObj( const CTCloneableObj< T >& src )
        : m_data( src.m_data )
{
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableObj< T >::~CTCloneableObj()
{
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableObj< T >& 
CTCloneableObj< T >::operator=( const CTCloneableObj< T >& src )
{
    if ( this != &src )
    {
        m_data = src.m_data;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CICloneable* 
CTCloneableObj< T >::Clone( void ) const
{
    return new CTCloneableObj< T >( *this );
}

/*-------------------------------------------------------------------------*/

template< typename T >
const T& 
CTCloneableObj< T >::GetData( void ) const
{
    return m_data;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */