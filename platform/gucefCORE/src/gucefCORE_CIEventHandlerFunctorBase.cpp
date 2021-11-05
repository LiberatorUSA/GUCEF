/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include "gucefCORE_CIEventHandlerFunctorBase.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/


CIEventHandlerFunctorBase::CIEventHandlerFunctorBase( void )
    : CIObserver()  ,
      CICloneable()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CIEventHandlerFunctorBase::CIEventHandlerFunctorBase( const CIEventHandlerFunctorBase& src )
    : CIObserver( src )  ,
      CICloneable( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CIEventHandlerFunctorBase::~CIEventHandlerFunctorBase()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CIEventHandlerFunctorBase&
CIEventHandlerFunctorBase::operator=( const CIEventHandlerFunctorBase& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CIObserver::operator=( src );
        CICloneable::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
