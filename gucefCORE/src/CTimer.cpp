/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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
{TRACE;

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
{TRACE;

    RegisterEvents();
    
    assert( m_timerFreq != 0 );
    m_timerPump->RegisterTimer( this );
}

/*-------------------------------------------------------------------------*/

CTimer::~CTimer()
{TRACE;

    m_timerPump->UnregisterTimer( this );
}

/*-------------------------------------------------------------------------*/

CTimer& 
CTimer::operator=( const CTimer& src )
{TRACE;

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
{TRACE;

    TimerStartedEvent.Initialize();
    TimerUpdateEvent.Initialize();
    TimerStoppedEvent.Initialize();
    TimerIntervalChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/
    
void 
CTimer::SetInterval( const UInt32 updateDeltaInMilliSecs )
{TRACE;

    m_updateDeltaInMilliSecs = updateDeltaInMilliSecs;
    
    TimerIntervalChangedEventData eData( updateDeltaInMilliSecs );
    NotifyObservers( TimerIntervalChangedEvent, &eData );
}

/*-------------------------------------------------------------------------*/
    
UInt32 
CTimer::GetInterval( void ) const
{TRACE;

    return m_updateDeltaInMilliSecs;
}

/*-------------------------------------------------------------------------*/

void
CTimer::SetEnabled( const bool enabled )
{TRACE;

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
{TRACE;

    return m_enabled;
} 

/*-------------------------------------------------------------------------*/

void 
CTimer::OnUpdate( void )
{TRACE;

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
{TRACE;

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
{TRACE;

    return GetRunTicks() / m_timerFreq;
}

/*-------------------------------------------------------------------------*/

UInt64
CTimer::GetTickCount( void ) const
{TRACE;

    return m_tickCount;
}

/*-------------------------------------------------------------------------*/

Float64
CTimer::GetTimeInMilliSecs( void ) const
{TRACE;

    return m_tickCount / m_timerFreq;
}

/*-------------------------------------------------------------------------*/

void
CTimer::Reset( void )
{TRACE;

    m_lastTimerCycle = MT::PrecisionTickCount();
    m_tickCount = 0;
}

/*-------------------------------------------------------------------------*/

Float64
CTimer::GetApproxMaxTimerResolutionInMilliSecs( void )
{TRACE;

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