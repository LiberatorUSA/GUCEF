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

#ifndef GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H
#define GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_macros.h"

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_CPP
    #pragma warning( push )
#endif

#pragma warning( disable: 4100 ) // unreferenced formal parameter
#pragma warning( disable: 4146 ) // unary minus operator applied to unsigned type, result still unsigned
#pragma warning( disable: 4251 ) // 'classname' needs to have dll-interface to be used by clients of class 'classname'
#pragma warning( disable: 4284 ) // return type for operator -> is 'const *' (ie; not a UDT or reference to a UDT).
#pragma warning( disable: 4786 ) // identifier was truncated to 'number' characters

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
 *  Template for an interface class for deleting objects.
 *  Meant to allow implementation of destructor classes to control object
 *  ownership. Designed to be used in combination with CTSharedPtr.
 */
template< typename PointerType >
class CTSharedObjectDestructor
{
    //TICS !GOLC-001: copy constructor and assignment operator not needed for an interface class
    
    public:
    
    virtual ~CTSharedObjectDestructor();
        
    virtual void DestroySharedObject( PointerType* sharedPointer ) = 0;

public:
    CTSharedObjectDestructor() {}
private:
    const CTSharedObjectDestructor& operator=(const CTSharedObjectDestructor&);
    CTSharedObjectDestructor(const CTSharedObjectDestructor&);
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
  
template< typename PointerType >
CTSharedObjectDestructor< PointerType >::~CTSharedObjectDestructor()
{
    /* implemented to ensure that a virtual destructor is provided */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_CPP
    #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTSHAREDOBJECTDESTRUCTOR_H ? */
