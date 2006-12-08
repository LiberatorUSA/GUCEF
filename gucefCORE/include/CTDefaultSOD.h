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

#ifndef GUCEF_CORE_CTDEFAULTSOD_H
#define GUCEF_CORE_CTDEFAULTSOD_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* module macro's */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H
#include "CTSharedObjectDestructor.h"
#define GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H
#endif /* GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H ? */

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
 *  Templated shared object destructor that simply deletes the given pointer.
 */
template< typename T >
class CTDefaultSOD : public CTSharedObjectDestructor< T >
{
    public:

    CTDefaultSOD( void );

    CTDefaultSOD( const CTDefaultSOD& src );

    virtual ~CTDefaultSOD();

    CTDefaultSOD& operator=( const CTDefaultSOD& src );

    virtual void DestroySharedObject( T* sharedPointer );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTDefaultSOD< T >::CTDefaultSOD( void )
{

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTDefaultSOD< T >::CTDefaultSOD( const CTDefaultSOD< T >& src )
{

}

/*-------------------------------------------------------------------------*/
  
template< typename T >
CTDefaultSOD< T >::~CTDefaultSOD()
{

}

/*-------------------------------------------------------------------------*/

template< typename T >
CTDefaultSOD< T >&
CTDefaultSOD< T >::operator=( const CTDefaultSOD& src )
{
    if ( &src != this )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T >
void
CTDefaultSOD< T >::DestroySharedObject( T* sharedPointer )
{
    delete sharedPointer;
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
        - file layout update to latest standard

-----------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTDEFAULTSOD_H ? */

