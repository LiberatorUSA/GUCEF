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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefCORE_CForwardingNotifier.h"

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

CForwardingNotifier::CForwardingNotifier( void )
    : CObservingNotifier()          ,
      m_forwardAllList()            ,
      m_forwardAllForNotifierList() ,
      m_eventNotifierMap()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CForwardingNotifier::CForwardingNotifier( const CForwardingNotifier& src )
    : CObservingNotifier( src )     ,
      m_forwardAllList()            ,
      m_forwardAllForNotifierList() ,
      m_eventNotifierMap()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CForwardingNotifier::~CForwardingNotifier()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CForwardingNotifier& 
CForwardingNotifier::operator=( const CForwardingNotifier& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CForwardingNotifier::SetForwardingActiveStateForAllEvents( CNotifier& notifier ,
                                                           const bool enabled  )
{GUCEF_TRACE;

    LockData();
    TNotifierMap::iterator i = m_forwardAllForNotifierList.begin();
    if ( i != m_forwardAllForNotifierList.end() )
    {
        (*i).second.enabled = enabled;
        UnlockData();
        return true;
    }
    UnlockData();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CForwardingNotifier::SetForwardingActiveStateForEvent( const CEvent& eventid ,
                                                       CNotifier* notifier   ,
                                                       const bool enabled    )
{GUCEF_TRACE;
    
    LockData();
    
    if ( NULL == notifier )
    {
        TEventMap::iterator i = m_forwardAllList.find( eventid );
        if ( i != m_forwardAllList.end() )
        {
            (*i).second.enabled = enabled;
            UnlockData();
            return true;             
        }
        UnlockData();
        return false;         
    }
    
    TEventNotifierMap::iterator i = m_eventNotifierMap.find( eventid );
    if ( i != m_eventNotifierMap.end() )
    {
        TNotifierMap& notifierMap = (*i).second;
        TNotifierMap::iterator n = notifierMap.find( notifier );
        if ( n != notifierMap.end() )
        {
            (*n).second.enabled = enabled;
            UnlockData();
            return true;            
        }
    }
    UnlockData();
    return false;    
}

/*-------------------------------------------------------------------------*/

void
CForwardingNotifier::AddForwardingForAllEvents( CNotifier& notifier                   ,
                                                const TEventOriginFilter originFilter ,
                                                const bool enabled                    )
{GUCEF_TRACE;

    assert( NULL != &notifier );
    
    LockData();
    TForwardState& state = m_forwardAllForNotifierList[ &notifier ];
    state.enabled = enabled;
    state.filter = originFilter;
    UnlockData();
}

/*-------------------------------------------------------------------------*/

bool
CForwardingNotifier::AddForwardingForEvent( const CEvent& eventid                 ,
                                            const TEventOriginFilter originFilter ,
                                            CNotifier* notifier /* = NULL */      ,
                                            const bool enabled                    )
{GUCEF_TRACE;

    if ( notifier == NULL && originFilter != EVENTORIGINFILTER_TRANSFER )
    {
        return false;
    }
    
    LockData();
    
    if ( NULL == notifier )
    {
        TForwardState& state = m_forwardAllList[ eventid ];
        state.filter = originFilter;
        state.enabled = enabled;        
    }
    else
    {
        TForwardState& state = (m_eventNotifierMap[ eventid ])[ notifier ];
        state.filter = originFilter;
        state.enabled = enabled;
    }
    
    UnlockData();    
    return true;
}

/*-------------------------------------------------------------------------*/

void
CForwardingNotifier::RemoveForwardingForAllEvents( CNotifier& notifier )
{GUCEF_TRACE;

    assert( NULL != &notifier );
    
    LockData();
    m_forwardAllForNotifierList.erase( &notifier );
    UnlockData();
}

/*-------------------------------------------------------------------------*/
    
void
CForwardingNotifier::RemoveForwardingForEvent( const CEvent& eventid            ,
                                               CNotifier* notifier /* = NULL */ )
{GUCEF_TRACE;

    LockData();
    TEventNotifierMap::iterator i( m_eventNotifierMap.find( eventid ) );
    if ( i != m_eventNotifierMap.end() )
    {
        TNotifierMap& notifierList = m_eventNotifierMap[ eventid ];
        notifierList.erase( notifier );
    }
    m_forwardAllList.erase( eventid );
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CForwardingNotifier::RemoveAllForwardingForNotifier( CNotifier& notifier )
{GUCEF_TRACE;

    assert( &notifier );
    
    LockData();
    
    m_forwardAllForNotifierList.erase( &notifier );
    
    TEventNotifierMap::iterator i( m_eventNotifierMap.begin() );
    while ( i != m_eventNotifierMap.end() )
    {
        TNotifierMap& notifierList = (*i).second;
        TNotifierMap::iterator n( notifierList.find( &notifier ) );
        if ( n != notifierList.end() )
        {
            notifierList.erase( n );
        }
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CForwardingNotifier::OnNotify( CNotifier* notifier                 ,
                               const CEvent& eventid               ,
                               CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    assert( NULL != notifier );
    
    // Check for administration updates
    if ( eventid == DestructionEvent )
    {
        // make sure that our forwarding list does not contain pointers 
        // to already deleted notifier instances
        RemoveAllForwardingForNotifier( *notifier );
    }
    else
    {
        LockData();
                
        // Check for if this is a notifier for which we forward all events
        TNotifierMap::iterator i = m_forwardAllForNotifierList.find( notifier );
        if ( m_forwardAllForNotifierList.end() != i )
        {
            const TForwardState& state = (*i).second;
            if ( state.enabled )
            {
                switch ( state.filter )
                {
                    case EVENTORIGINFILTER_UNMODIFIED :
                    {
                        if ( !NotifyObservers( *notifier, eventid, eventdata ) ) return;
                        break;
                    }
                    case EVENTORIGINFILTER_TRANSFER :
                    {
                        if ( !NotifyObservers( *this, eventid, eventdata ) ) return;
                        break;
                    }
                    default:
                    {
                        GUCEF_UNREACHABLE;                        
                    }                                                
                }
            }
        }
        else
        {
            // Check if this is an event we always forward        
            TEventMap::iterator i = m_forwardAllList.find( eventid );
            if ( m_forwardAllList.end() != i )
            {
                const TForwardState& state = (*i).second;
                if ( state.filter == EVENTORIGINFILTER_TRANSFER )
                {
                    GUCEF_DEBUG_LOG( 0, "CForwardingNotifier: Transfering event origin from " + PointerToString( notifier ) + " to " + PointerToString( this ) + " for event: " + eventid.GetName() );
                    
                    if ( !NotifyObservers( *this, eventid, eventdata ) ) return;
                }
                else
                {
                    // We should not be able to get here since you should not be able to define such a forwarding setting
                    GUCEF_UNREACHABLE;
                }
            }                
            else
            {
                // Check for specific events we should forward
                TEventNotifierMap::iterator i( m_eventNotifierMap.find( eventid ) );
                if ( i != m_eventNotifierMap.end() )
                {
                    TNotifierMap& notifierList = (*i).second;
                    TNotifierMap::iterator n( notifierList.find( notifier ) );
                    if ( n != notifierList.end() )
                    {
                        const TForwardState& state = (*n).second;
                        if ( state.enabled )
                        {
                            switch ( state.filter )
                            {
                                case EVENTORIGINFILTER_UNMODIFIED :
                                {
                                    if ( !NotifyObservers( *notifier, eventid, eventdata ) ) return;
                                    break;
                                }
                                case EVENTORIGINFILTER_TRANSFER :
                                {
                                    if ( !NotifyObservers( *this, eventid, eventdata ) ) return;
                                    break;
                                }
                                default:
                                {
                                    GUCEF_UNREACHABLE;                        
                                }                                                            
                            }
                        }
                    }
                }
            }
        }
        
        UnlockData();   
    }
}

/*-------------------------------------------------------------------------*/

const CString&
CForwardingNotifier::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::CORE::CForwardingNotifier";
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
