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

#include "gucefCORE_CEventAggregateTrigger.h"          

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CEvent CEventAggregateTrigger::AggregateTriggerEvent = "GUCEF::CORE::CEventAggregateTrigger::AggregateTriggerEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventAggregateTrigger::CEventAggregateTrigger( void )
    : CTSGNotifier()
    , m_eventCritereaMap()
    , m_isTriggered( false )
    , m_onlyTriggerOnce( true )
{GUCEF_TRACE

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CEventAggregateTrigger::CEventAggregateTrigger( PulseGeneratorPtr pulseGenerator )
    : CTSGNotifier( pulseGenerator )
    , m_eventCritereaMap()
    , m_isTriggered( false )
    , m_onlyTriggerOnce( true )
{GUCEF_TRACE

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CEventAggregateTrigger::CEventAggregateTrigger( const CEventAggregateTrigger& src )
    : CTSGNotifier( src )
    , m_eventCritereaMap( src.m_eventCritereaMap )
    , m_isTriggered( src.m_isTriggered )
    , m_onlyTriggerOnce( src.m_onlyTriggerOnce )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEventAggregateTrigger::~CEventAggregateTrigger()
{GUCEF_TRACE; 

    SignalUpcomingDestruction();
}

/*-------------------------------------------------------------------------*/

void 
CEventAggregateTrigger::RegisterEvents( void )
{GUCEF_TRACE; 

    AggregateTriggerEvent.Initialize();    
}

/*-------------------------------------------------------------------------*/

void
CEventAggregateTrigger::OnTriggerCritereaEvent( CNotifier* notifier    ,
                                                const CEvent& eventId  ,
                                                CICloneable* eventData )
{GUCEF_TRACE; 

    MT::CObjectScopeLock lock( this );
    
    TEventCritereaMap::iterator critereaIter = m_eventCritereaMap.find( eventId );
    if ( m_eventCritereaMap.end() != critereaIter )
    {
        critereaIter->second = true;
        lock.EarlyUnlock();

        CheckForTriggerCriterea();
    }
}

/*-------------------------------------------------------------------------*/

void 
CEventAggregateTrigger::CheckForTriggerCriterea( void )
{GUCEF_TRACE; 

    MT::CObjectScopeLock lock( this );

    if ( !m_eventCritereaMap.empty() )
    {
        TEventCritereaMap::iterator critereaIter = m_eventCritereaMap.begin();
        while ( m_eventCritereaMap.end() != critereaIter )
        {
            if ( !critereaIter->second )
                return;
            ++critereaIter;
        } 

        bool performTrigger = true;
        if ( m_onlyTriggerOnce && m_isTriggered )
            performTrigger = false;
        
        m_isTriggered = true;

        lock.EarlyUnlock();        
        if ( performTrigger )
            NotifyObservers( AggregateTriggerEvent );
    }
}

/*-------------------------------------------------------------------------*/

bool 
CEventAggregateTrigger::AddEventToTriggerCriterea( const CEvent& eventId )
{GUCEF_TRACE; 

    MT::CObjectScopeLock lock( this );
    
    TEventCritereaMap::iterator critereaIter = m_eventCritereaMap.find( eventId );
    if ( m_eventCritereaMap.end() == critereaIter )
    {
        m_eventCritereaMap[ eventId ] = false;
        return true;    
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CEventAggregateTrigger::SubscribeAndAddEventToTriggerCriterea( CINotifier* notifier, const CEvent& eventId )
{GUCEF_TRACE; 

    if ( GUCEF_NULL == notifier )
        return false;

    MT::CObjectScopeLock lock( this );

    TEventCallback callback( this, &CEventAggregateTrigger::OnTriggerCritereaEvent );
    if ( notifier->Subscribe( &AsObserver(), eventId, &callback ) )
    {
        return AddEventToTriggerCriterea( eventId );
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CEventAggregateTrigger::RemoveEventFromTriggerCriterea( const CEvent& eventId )
{GUCEF_TRACE; 

    MT::CObjectScopeLock lock( this );
    
    TEventCritereaMap::iterator critereaIter = m_eventCritereaMap.find( eventId );
    if ( m_eventCritereaMap.end() != critereaIter )
    {
        m_eventCritereaMap.erase( critereaIter );
        return true;    
    }
    return false;  
}

/*-------------------------------------------------------------------------*/

void
CEventAggregateTrigger::SetOnlyTriggerOnce( bool onlyTriggerOnce )
{GUCEF_TRACE; 

    MT::CObjectScopeLock lock( this );
    m_onlyTriggerOnce = onlyTriggerOnce;
}

/*-------------------------------------------------------------------------*/

bool
CEventAggregateTrigger::GetOnlyTriggerOnce( void ) const
{GUCEF_TRACE; 

    MT::CObjectScopeLock lock( this );
    return m_onlyTriggerOnce;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
