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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#define GUCEF_CORE_CNOTIFIEROBSERVINGCOMPONENT_CPP
#include "CNotifierObservingComponent.h"

#include "CObservingNotifier.h"

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

void 
CNotifierObservingComponent::OnNotify( CNotifier* notifier                 ,
                                       const CEvent& eventid               ,
                                       CICloneable* eventdata /* = NULL */ )
{TRACE;

    m_owner->OnNotify( notifier  ,
                       eventid   ,
                       eventdata );
}                                         

/*-------------------------------------------------------------------------*/

void 
CNotifierObservingComponent::LockData( void )
{TRACE;

    m_owner->LockData();
}

/*-------------------------------------------------------------------------*/
    
void 
CNotifierObservingComponent::UnlockData( void )
{TRACE;

    m_owner->UnlockData();
}

/*-------------------------------------------------------------------------*/

CNotifierObservingComponent::CNotifierObservingComponent( void )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent::CNotifierObservingComponent( const CNotifierObservingComponent& src )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent::~CNotifierObservingComponent()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent& 
CNotifierObservingComponent::operator=( const CNotifierObservingComponent& src )
{TRACE;

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CNotifierObservingComponent::SetOwner( CObservingNotifier* owner )
{TRACE;

    m_owner = owner;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
