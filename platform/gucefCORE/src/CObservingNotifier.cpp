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

#include <assert.h>

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#include "CObservingNotifier.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

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

CObservingNotifier::CObservingNotifier( void )
{GUCEF_TRACE;

    m_observer.SetOwner( this );
}

/*-------------------------------------------------------------------------*/

CObservingNotifier::CObservingNotifier( const CObservingNotifier& src )
    : m_observer( src.m_observer )
{GUCEF_TRACE;

    m_observer.SetOwner( this );
}

/*-------------------------------------------------------------------------*/

CObservingNotifier::~CObservingNotifier()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CObservingNotifier&
CObservingNotifier::operator=( const CObservingNotifier& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::UnsubscribeAllFromObserver( void )
{GUCEF_TRACE;

    LockData();
    m_observer.UnsubscribeAllFromObserver();
    UnlockData();
}

/*-------------------------------------------------------------------------*/

UInt32
CObservingNotifier::GetObserverSubscriptionCount( void )
{GUCEF_TRACE;

    LockData();
    UInt32 retval( m_observer.GetSubscriptionCount() );
    UnlockData();
    return retval;
}

/*-------------------------------------------------------------------------*/

UInt32
CObservingNotifier::GetObserverNotifierCount( void )
{GUCEF_TRACE;

    LockData();
    UInt32 retval( m_observer.GetNotifierCount() );
    UnlockData();
    return retval;
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::SubscribeTo( CNotifier* notifier )
{GUCEF_TRACE;

    if ( NULL != notifier )
    {
        LockData();
        notifier->Subscribe( &m_observer );
        UnlockData();
    }
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::SubscribeTo( CNotifier* notifier   ,
                                 const CEvent& eventid )
{GUCEF_TRACE;

    SubscribeToImp( notifier, eventid, 0 );
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::SubscribeTo( CNotifier* notifier                 ,
                                 const CEvent& eventid               ,
                                 CIEventHandlerFunctorBase& callback )
{GUCEF_TRACE;

    SubscribeToImp( notifier, eventid, &callback );
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::SubscribeToImp( CNotifier* notifier                 ,
                                    const CEvent& eventid               ,
                                    CIEventHandlerFunctorBase* callback )
{GUCEF_TRACE;

    if ( NULL != notifier )
    {
        LockData();
        notifier->Subscribe( &m_observer ,
                             eventid     ,
                             callback    );
        UnlockData();
    }
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::UnsubscribeFrom( CNotifier* notifier )
{GUCEF_TRACE;

    if ( NULL != notifier )
    {
        LockData();
        notifier->Unsubscribe( &m_observer );
        UnlockData();
    }
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::UnsubscribeFrom( CNotifier* notifier   ,
                                     const CEvent& eventid )
{GUCEF_TRACE;

    if ( NULL != notifier )
    {
        LockData();
        notifier->Unsubscribe( &m_observer ,
                               eventid     );
        UnlockData();
    }
}

/*-------------------------------------------------------------------------*/

CObserver&
CObservingNotifier::AsObserver( void )
{GUCEF_TRACE;

    return m_observer;
}

/*-------------------------------------------------------------------------*/

const CObserver&
CObservingNotifier::AsObserver( void ) const
{GUCEF_TRACE;

    return m_observer;
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::OnNotify( CNotifier* notifier                 ,
                              const CEvent& eventid               ,
                              CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    // Dummy to avoid mandatory implementation by decending class
}

/*-------------------------------------------------------------------------*/

const CString&
CObservingNotifier::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CObservingNotifier";
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
