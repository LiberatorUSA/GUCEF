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
 
#ifndef GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H
#define GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/


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
 *  Base class for destructor delegator specializations
 */
template< typename T >
class CTDynamicDestructorBase
{
    public:

    typedef T TDestructorType;
        
    CTDynamicDestructorBase( void );
  
    virtual void DestroyObject( T* objectToBeDestroyed ) = 0;

    virtual ~CTDynamicDestructorBase();    

    private:

    CTDynamicDestructorBase( const CTDynamicDestructorBase& src );            /**< not implemented */
    CTDynamicDestructorBase& operator=( const CTDynamicDestructorBase& src ); /**< not implemented */

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTDynamicDestructorBase< T >::CTDynamicDestructorBase( void )
{

}

/*-------------------------------------------------------------------------*/
   
template< typename T >
CTDynamicDestructorBase< T >::~CTDynamicDestructorBase()
{

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF *

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTDYNAMICDESTRUCTORBASE_H ? */
