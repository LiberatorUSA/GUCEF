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
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_owner )
        m_owner->OnNotify( notifier  ,
                           eventid   ,
                           eventdata );
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CNotifierObservingComponent::AsLockable( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_owner )
        return m_owner->AsLockable();
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool 
CNotifierObservingComponent::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_owner )
        return m_owner->Lock( lockWaitTimeoutInMs );
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool 
CNotifierObservingComponent::Unlock( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_owner )
        return m_owner->Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

CNotifierObservingComponent::CNotifierObservingComponent( void )
    : CObserver()
    , m_owner( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent::CNotifierObservingComponent( const CNotifierObservingComponent& src )
    : CObserver( src )
    , m_owner( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent::~CNotifierObservingComponent()
{GUCEF_TRACE;

    SignalUpcomingObserverDestruction();
}

/*-------------------------------------------------------------------------*/
    
CNotifierObservingComponent& 
CNotifierObservingComponent::operator=( const CNotifierObservingComponent& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CNotifierObservingComponent::SetOwner( CObservingNotifier* owner )
{GUCEF_TRACE;

    Lock();
    m_owner = owner;
    Unlock();
}

/*-------------------------------------------------------------------------*/

const CString&
CNotifierObservingComponent::GetClassTypeName( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_owner )
    {
        return m_owner->GetClassTypeName();
    }
    
    static const CString typeName = "GUCEF::CORE::CNotifierObservingComponent";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
