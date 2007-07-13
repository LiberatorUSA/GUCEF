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
#include "gucefMT_dvmtoswrap.h"  /* needed for the precision timer */
#include "CTimerPump.h"
#include "CTracer.h"
#include "CTimer.h"

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

const CEvent CTimer::TimerStartedEvent = "GUCEF::CORE::CTimer::TimerStartedEvent";
const CEvent CTimer::TimerUpdateEvent = "GUCEF::CORE::CTimer::TimerUpdateEvent";
const CEvent CTimer::TimerStoppedEvent = "GUCEF::CORE::CTimer::TimerStoppedEvent";
const CEvent CTimer::TimerIntervalChangedEvent = "GUCEF::CORE::CTimer::TimerIntervalChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTimer::CTimer( const UInt32 updateDeltaInMilliSecs /* = 10 */ )
    : m_lastTimerCycle( 0 )                                    ,
      m_enabled( false )                                       ,
      m_updateDeltaInMilliSecs( updateDeltaInMilliSecs )       ,
      m_activationTickCount( 0 )                               ,
      m_tickCount( 0 )                                         ,
      m_timerPump( CTimerPump::Instance() )                    ,
      m_timerFreq( MT::PrecisionTimerResolution() / 1000.0 )
{GUCEF_TRACE;

    RegisterEvents();
    
    assert( m_timerFreq != 0 );
    m_timerPump->RegisterTimer( this );
}          

/*-------------------------------------------------------------------------*/

CTimer::CTimer( const CTimer& src )
     : m_updateDeltaInMilliSecs( src.m_updateDeltaInMilliSecs ) ,
       m_lastTimerCycle( src.m_lastTimerCycle )                 ,
       m_enabled( false )                                       ,
       m_activationTickCount( 0 )                               ,
       m_tickCount( 0 )                                         ,
       m_timerPump( CTimerPump::Instance() )                    ,
       m_timerFreq( MT::PrecisionTimerResolution() / 1000.0 )
{GUCEF_TRACE;

    RegisterEvents();
    
    assert( m_timerFreq != 0 );
    m_timerPump->RegisterTimer( this );
}

/*-------------------------------------------------------------------------*/

CTimer::~CTimer()
{GUCEF_TRACE;

    m_timerPump->UnregisterTimer( this );
}

/*-------------------------------------------------------------------------*/

CTimer& 
CTimer::operator=( const CTimer& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_updateDeltaInMilliSecs = src.m_updateDeltaInMilliSecs;
        m_lastTimerCycle = src.m_lastTimerCycle;
        m_enabled = src.m_enabled;
        m_tickCount = src.m_tickCount;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CTimer::RegisterEvents( void )
{GUCEF_TRACE;

    TimerStartedEvent.Initialize();
    TimerUpdateEvent.Initialize();
    TimerStoppedEvent.Initialize();
    TimerIntervalChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/
    
void 
CTimer::SetInterval( const UInt32 updateDeltaInMilliSecs )
{GUCEF_TRACE;

    m_updateDeltaInMilliSecs = updateDeltaInMilliSecs;
    
    TimerIntervalChangedEventData eData( updateDeltaInMilliSecs );
    NotifyObservers( TimerIntervalChangedEvent, &eData );
}

/*-------------------------------------------------------------------------*/
    
UInt32 
CTimer::GetInterval( void ) const
{GUCEF_TRACE;

    return m_updateDeltaInMilliSecs;
}

/*-------------------------------------------------------------------------*/

void
CTimer::SetEnabled( const bool enabled )
{GUCEF_TRACE;

    if ( m_enabled != enabled )
    {
        m_enabled = enabled;                
        
        if ( enabled )
        {                    
            m_lastTimerCycle = m_activationTickCount = MT::PrecisionTickCount();
            m_timerFreq = MT::PrecisionTimerResolution() / 1000.0;
            assert( m_timerFreq != 0 );
            m_tickCount = 0;
            
            // Make sure we get an update and tell the pump about our update requirement
            m_timerPump->TimerSetRequiresUpdates( this, true );
            
            NotifyObservers( TimerStartedEvent );
        }
        else
        {
            // we don't need periodic updates anymore 
            m_timerPump->TimerSetRequiresUpdates( this, false );
            
            NotifyObservers( TimerStoppedEvent );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CTimer::GetEnabled( void ) const
{GUCEF_TRACE;

    return m_enabled;
} 

/*-------------------------------------------------------------------------*/

void 
CTimer::OnUpdate( void )
{GUCEF_TRACE;

    if ( m_enabled )
    {
        UInt64 newTickCount = MT::PrecisionTickCount();    
        UInt64 deltaTicks = m_lastTimerCycle - newTickCount;
        Float64 deltaMilliSecs = ( deltaTicks / m_timerFreq ) ;
        m_lastTimerCycle = newTickCount;
        m_tickCount += deltaTicks;

        if ( deltaMilliSecs >= m_updateDeltaInMilliSecs )
        {                    
            TTimerUpdateData updateData;
            updateData.tickCount = m_tickCount;
            updateData.updateDeltaInMilliSecs = deltaMilliSecs;
            TimerUpdateEventData cuData( updateData );
            NotifyObservers( TimerUpdateEvent, &cuData );
        }
    }         
}

/*-------------------------------------------------------------------------*/

UInt64 
CTimer::GetRunTicks( void ) const
{GUCEF_TRACE;

    if ( m_enabled )
    {
        return MT::PrecisionTickCount() - m_activationTickCount;
    }
    else
    {
        return m_tickCount;
    }
}

/*-------------------------------------------------------------------------*/

Float64
CTimer::GetRunTimeInMilliSecs( void ) const
{GUCEF_TRACE;

    return GetRunTicks() / m_timerFreq;
}

/*-------------------------------------------------------------------------*/

UInt64
CTimer::GetTickCount( void ) const
{GUCEF_TRACE;

    return m_tickCount;
}

/*-------------------------------------------------------------------------*/

Float64
CTimer::GetTimeInMilliSecs( void ) const
{GUCEF_TRACE;

    return m_tickCount / m_timerFreq;
}

/*-------------------------------------------------------------------------*/

void
CTimer::Reset( void )
{GUCEF_TRACE;

    m_lastTimerCycle = MT::PrecisionTickCount();
    m_tickCount = 0;
}

/*-------------------------------------------------------------------------*/

Float64
CTimer::GetApproxMaxTimerResolutionInMilliSecs( void )
{GUCEF_TRACE;

    return MT::PrecisionTimerResolution() / 1000.0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/