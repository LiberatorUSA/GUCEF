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

#ifndef GUCEF_CORE_CTCLONEABLEEXPANSION_H
#define GUCEF_CORE_CTCLONEABLEEXPANSION_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

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
 *  Tenplate to quickly and easily add a ICloneable interface implementation
 *  to any class.
 */
template< class T > 
class CTCloneableExpansion : public T ,
                             public CICloneable
{
    public:

    CTCloneableExpansion( void );

    CTCloneableExpansion( const CTCloneableExpansion& src );

    CTCloneableExpansion( const T& src );
        
    virtual ~CTCloneableExpansion();
    
    CTCloneableExpansion& operator=( const CTCloneableExpansion& src );

    CTCloneableExpansion& operator=( const T& src );
    
    virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTCloneableExpansion< T >::CTCloneableExpansion( void )
    : T()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTCloneableExpansion< T >::CTCloneableExpansion( const CTCloneableExpansion< T >& src )
    : T( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTCloneableExpansion< T >::CTCloneableExpansion( const T& src )
    : T( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableExpansion< T >::~CTCloneableExpansion()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableExpansion< T >& 
CTCloneableExpansion< T >::operator=( const CTCloneableExpansion< T >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        T::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CTCloneableExpansion< T >& 
CTCloneableExpansion< T >::operator=( const T& src )
{GUCEF_TRACE;

    T::operator=( src );
    return *this;
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
CICloneable* 
CTCloneableExpansion< T >::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CTCloneableExpansion< T >( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTCLONEABLEEXPANSION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-10-2006 :
        - Dinand: initial implementation

-----------------------------------------------------------------------------*/