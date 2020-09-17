/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */
 
#ifndef GUCEF_CORE_CTDYNAMICDESTRUCTOR_H
#define GUCEF_CORE_CTDYNAMICDESTRUCTOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H
#include "CTDynamicDestructorBase.h"
#define GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H
#endif /* #define GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H ? */ 

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
