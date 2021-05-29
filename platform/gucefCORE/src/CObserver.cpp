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
#include "CNotificationIDRegistry.h"
#include "CNotifier.h"

#include "CObserver.h"

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

CObserver::CObserver( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CObserver::CObserver( const CObserver& src )
{GUCEF_TRACE;

    // We intentionally do not copy subscriptions
}

/*-------------------------------------------------------------------------*/

CObserver::~CObserver()
{GUCEF_TRACE;

    Lock();

    /*
     *  Neatly un-subscribe from all notifiers
     */
    TNotifierList listcopy( m_notifiers );
    TNotifierList::iterator i( listcopy.begin() );
    while ( i != listcopy.end() )
    {
        (*i)->OnObserverDestroy( this );
        ++i;
    }
    m_notifiers.clear();

    Unlock();
}

/*-------------------------------------------------------------------------*/

CObserver&
CObserver::operator=( const CObserver& src )
{GUCEF_TRACE;

    // We intentionally do not copy subscriptions
    return *this;
}

/*-------------------------------------------------------------------------*/

const CString&
CObserver::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CObserver";
    return typeName;
}

/*-------------------------------------------------------------------------*/

void
CObserver::UnsubscribeAllFromObserver( void )
{GUCEF_TRACE;

    Lock();

    /*
     *  Neatly un-subscribe from all notifiers
     */
    TNotifierList listcopy( m_notifiers );
    TNotifierList::iterator i( listcopy.begin() );
    while ( i != listcopy.end() )
    {
        /*
         *  It might not be evident here but it is the notifier
         *  who updates our administration which is why we had to make a copy of
         *  our notifier list.
         */
        (*i)->Unsubscribe( this );
        ++i;
    }

    Unlock();
}

/*-------------------------------------------------------------------------*/

void
CObserver::SubscribeTo( CNotifier* notifier )
{GUCEF_TRACE;

    notifier->Subscribe( this );
}

/*-------------------------------------------------------------------------*/

void
CObserver::SubscribeTo( CNotifier* notifier   ,
                        const CEvent& eventid )
{
    SubscribeToImp( notifier, eventid, 0 );
}

/*-------------------------------------------------------------------------*/

void
CObserver::SubscribeTo( CNotifier* notifier                 ,
                        const CEvent& eventid               ,
                        CIEventHandlerFunctorBase& callback )
{
    SubscribeToImp( notifier, eventid, &callback );
}

/*-------------------------------------------------------------------------*/

void
CObserver::SubscribeToImp( CNotifier* notifier                 ,
                           const CEvent& eventid               ,
                           CIEventHandlerFunctorBase* callback )
{GUCEF_TRACE;

    notifier->Subscribe( this     ,
                         eventid  ,
                         callback );
}

/*-------------------------------------------------------------------------*/

void
CObserver::LinkTo( CNotifier* notifier )
{GUCEF_TRACE;

    if ( GUCEF_NULL != notifier )
    {
        Lock();
        m_notifiers.insert( notifier );
        Unlock();
    }
}

/*-------------------------------------------------------------------------*/

void
CObserver::UnlinkFrom( CNotifier* notifier )
{GUCEF_TRACE;

    Lock();
    m_notifiers.erase( notifier );
    Unlock();
}

/*-------------------------------------------------------------------------*/

UInt32
CObserver::GetSubscriptionCount( void )
{GUCEF_TRACE;

    UInt32 subscriptionCount( 0 );
    Lock();
    TNotifierList::const_iterator i( m_notifiers.begin() );
    while ( i != m_notifiers.end() )
    {
        subscriptionCount += (*i)->GetSubscriptionCountForObserver( this );
        ++i;
    }
    Unlock();
    return subscriptionCount;
}

/*-------------------------------------------------------------------------*/

UInt32
CObserver::GetNotifierCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_notifiers.size();
}

/*-------------------------------------------------------------------------*/

void
CObserver::UnsubscribeFrom( CNotifier& notifier )
{GUCEF_TRACE;

    assert( &notifier );
    notifier.Unsubscribe( this );
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CObserver::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

bool
CObserver::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    // dummy to avoid mandatory implementation by descending classes
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CObserver::Unlock( void ) const
{GUCEF_TRACE;

    // dummy to avoid mandatory implementation by descending classes
    return false;
}

/*-------------------------------------------------------------------------*/

void
CObserver::OnNotify( CNotifier* notifier                 ,
                     const CEvent& eventid               ,
                     CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    // dummy to avoid mandatory implementation by descending classes
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
