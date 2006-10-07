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
{TRACE;

    m_observer.SetOwner( this );
}

/*-------------------------------------------------------------------------*/
    
CObservingNotifier::CObservingNotifier( const CObservingNotifier& src )
    : m_observer( src.m_observer )
{TRACE;

    m_observer.SetOwner( this );
}

/*-------------------------------------------------------------------------*/
    
CObservingNotifier::~CObservingNotifier()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CObservingNotifier& 
CObservingNotifier::operator=( const CObservingNotifier& src )
{TRACE;

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
void 
CObservingNotifier::UnsubscribeAllFromObserver( void )
{TRACE;

    LockData();
    m_observer.UnsubscribeFromAll();
    UnlockData();
}
    
/*-------------------------------------------------------------------------*/

UInt32 
CObservingNotifier::GetObserverSubscriptionCount( void )
{TRACE;

    LockData();
    UInt32 retval( m_observer.GetSubscriptionCount() );
    UnlockData();
    return retval;
}

/*-------------------------------------------------------------------------*/

UInt32 
CObservingNotifier::GetObserverNotifierCount( void )
{TRACE;

    LockData();
    UInt32 retval( m_observer.GetNotifierCount() );
    UnlockData();
    return retval;
}

/*-------------------------------------------------------------------------*/

void 
CObservingNotifier::SubscribeTo( CNotifier* notifier )
{TRACE;

    LockData();
    notifier->Subscribe( &m_observer );
    UnlockData();
}

/*-------------------------------------------------------------------------*/
    
void 
CObservingNotifier::SubscribeTo( CNotifier* notifier   ,
                                 const CEvent& eventid )
{TRACE;

    LockData();
    notifier->Subscribe( &m_observer ,
                         eventid     );
    UnlockData();
}                                   

/*-------------------------------------------------------------------------*/

void 
CObservingNotifier::UnsubscribeFrom( CNotifier* notifier )
{TRACE;

    LockData();
    notifier->Unsubscribe( &m_observer );
    UnlockData();
}

/*-------------------------------------------------------------------------*/
                 
void 
CObservingNotifier::UnsubscribeFrom( CNotifier* notifier   ,
                                     const CEvent& eventid )
{TRACE;

    LockData();
    notifier->Unsubscribe( &m_observer ,
                           eventid     );
    UnlockData();
}

/*-------------------------------------------------------------------------*/

CObserver& 
CObservingNotifier::AsObserver( void )
{TRACE;

    return m_observer;
}

/*-------------------------------------------------------------------------*/

const CObserver& 
CObservingNotifier::AsObserver( void ) const
{TRACE;

    return m_observer;
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::AddEventForwarding( const CEvent& eventid            , 
                                        CNotifier* notifier /* = NULL */ )
{TRACE;

    if ( NULL != notifier )
    {
        LockData();
        TNotifierList& notifierList = m_eventNotifierMap[ eventid ];
        notifierList.insert( notifier );
        UnlockData();
    }
    else
    {
        LockData();
        m_eventList.insert( eventid );
        UnlockData();        
    }
}

/*-------------------------------------------------------------------------*/
    
void
CObservingNotifier::RemoveEventForwarding( const CEvent& eventid            ,
                                           CNotifier* notifier /* = NULL */ )
{TRACE;

    if ( NULL != notifier )
    {
        LockData();
        TEventNotifierMap::iterator i( m_eventNotifierMap.find( eventid ) );
        if ( i != m_eventNotifierMap.end() )
        {
            TNotifierList& notifierList = m_eventNotifierMap[ eventid ];
            notifierList.erase( notifier );
        }
        UnlockData();
    }
    else
    {
        LockData();
        m_eventList.erase( eventid );
        UnlockData();        
    }
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::RemoveEventForwarding( CNotifier& notifier )
{TRACE;

    assert( &notifier );
    
    LockData();
    
    TEventNotifierMap::iterator i( m_eventNotifierMap.begin() );
    while ( i != m_eventNotifierMap.end() )
    {
        TNotifierList& notifierList = (*i).second;
        TNotifierList::iterator n( notifierList.find( &notifier ) );
        if ( n != notifierList.end() )
        {
            notifierList.erase( n );
        }
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CObservingNotifier::OnNotify( CNotifier* notifier                 ,
                              const CEvent& eventid               ,
                              CICloneable* eventdata /* = NULL */ )
{TRACE;

    assert( NULL != notifier );
    
    // Check for administration updates
    if ( eventid == notifier->GetDestructionEventID() )
    {
        // make sure that our forwarding list does not contain pointers 
        // to already deleted notifier instances
        RemoveEventForwarding( *notifier );
    }
    else
    {
        LockData();
        
        // Check for events we should forward
        if ( m_eventList.end() != m_eventList.find( eventid ) )
        {
            NotifyObservers( eventid, eventdata );
        }
        else
        {
            // Check for events we should forward for this notifier
            TEventNotifierMap::iterator i( m_eventNotifierMap.find( eventid ) );
            if ( i != m_eventNotifierMap.end() )
            {
                TNotifierList& notifierList = (*i).second;
                TNotifierList::iterator n( notifierList.find( notifier ) );
                if ( n != notifierList.end() )
                {
                    NotifyObservers( eventid, eventdata );
                }
            }
        }
        
        UnlockData();   
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
