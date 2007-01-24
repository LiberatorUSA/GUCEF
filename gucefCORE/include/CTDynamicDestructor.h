/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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
 
#ifndef GUCEF_CORE_CTDYNAMICDESTRUCTOR_H
#define GUCEF_CORE_CTDYNAMICDESTRUCTOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CTDynamicDestructorBase.h"

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
 *  Class meant for delegation of destruction duties.
 */
template< typename T >
class CTDynamicDestructor : public CTDynamicDestructorBase< T >
{
    public:

    typedef T TDestructorType;
        
    /**
     *  
     */
    CTDynamicDestructor( const bool destroySelfOnDestroyObject = false );
  
    virtual void DestroyObject( T* objectToBeDestroyed );

    virtual ~CTDynamicDestructor();    

    private:

    CTDynamicDestructor( const CTDynamicDestructor& src );            /**< not implemented */
    CTDynamicDestructor& operator=( const CTDynamicDestructor& src ); /**< not implemented */

    private:

    bool m_destroySelfOnDestroyObject;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTDynamicDestructor< T >::CTDynamicDestructor( const bool destroySelfOnDestroyObject /* = false */ )
    : CTDynamicDestructorBase< T >()                             ,
      m_destroySelfOnDestroyObject( destroySelfOnDestroyObject ) 
{

}

/*-------------------------------------------------------------------------*/
   
template< typename T >
CTDynamicDestructor< T >::~CTDynamicDestructor()
{

}

/*-------------------------------------------------------------------------*/

template< typename T > 
void
CTDynamicDestructor< T >::DestroyObject( T* objectToBeDestroyed )
{
    delete objectToBeDestroyed;

    if ( m_destroySelfOnDestroyObject )
    {
        delete this;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTDYNAMICDESTRUCTOR_H ? */
