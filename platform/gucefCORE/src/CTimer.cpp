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

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"  /* needed for the precision timer */
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CTimer::CTimer( const UInt32 updateDeltaInMilliSecs /* = 25 */ )
    : CObservingNotifier()
    , m_timerFreq( MT::PrecisionTimerResolution() / 1000.0 )   
    , m_lastTimerCycle( 0 )                                    
    , m_tickCount( 0 )
    , m_updateDeltaInMilliSecs( updateDeltaInMilliSecs )
    , m_activationTickCount( 0 )
    , m_enabled( false )
    , m_immediateTriggerRequested( false )
    , m_pulseGenerator( CCoreGlobal::Instance()->GetPulseGenerator() )
    , m_opaqueUserData( GUCEF_NULL )
{GUCEF_TRACE;

    RegisterEvents();

    if ( 0 == m_updateDeltaInMilliSecs )
    {
        m_updateDeltaInMilliSecs = 1;
    }

    RegisterPulseGeneratorEventHandlers();
}

/*-------------------------------------------------------------------------*/

CTimer::CTimer( PulseGeneratorPtr pulseGenerator               ,
                const UInt32 updateDeltaInMilliSecs /* = 25 */ )
    : CObservingNotifier()
    , m_timerFreq( MT::PrecisionTimerResolution() / 1000.0 )   
    , m_lastTimerCycle( 0 )                                    
    , m_tickCount( 0 )
    , m_updateDeltaInMilliSecs( updateDeltaInMilliSecs )
    , m_activationTickCount( 0 )
    , m_enabled( false )
    , m_immediateTriggerRequested( false )
    , m_pulseGenerator( pulseGenerator )
    , m_opaqueUserData( GUCEF_NULL )
{GUCEF_TRACE;

    RegisterEvents();

    if ( 0 == m_updateDeltaInMilliSecs )
    {
        m_updateDeltaInMilliSecs = 1;
    }

    RegisterPulseGeneratorEventHandlers();
}

/*-------------------------------------------------------------------------*/

CTimer::CTimer( const CTimer& src )
    : CObservingNotifier( src )
    , m_timerFreq( MT::PrecisionTimerResolution() / 1000.0 )   
    , m_lastTimerCycle( src.m_lastTimerCycle )                                    
    , m_tickCount( 0 )
    , m_updateDeltaInMilliSecs( src.m_updateDeltaInMilliSecs )
    , m_activationTickCount( 0 )
    , m_enabled( false )
    , m_immediateTriggerRequested( false )
    , m_pulseGenerator( src.m_pulseGenerator )
    , m_opaqueUserData( GUCEF_NULL )
{GUCEF_TRACE;

    RegisterEvents();

    if ( 0 == m_updateDeltaInMilliSecs )
    {
        m_updateDeltaInMilliSecs = 1;
    }

    RegisterPulseGeneratorEventHandlers();
}

/*-------------------------------------------------------------------------*/

CTimer::~CTimer()
{GUCEF_TRACE;

    if ( !m_pulseGenerator.IsNULL() )
    {
        // Make sure this object is not still registered as requiring periodic updates
        m_pulseGenerator->RequestStopOfPeriodicUpdates( this );
        m_pulseGenerator.Unlink();
    }
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
        m_opaqueUserData = GUCEF_NULL;
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
CTimer::RegisterPulseGeneratorEventHandlers( void )
{GUCEF_TRACE;

    if ( !m_pulseGenerator.IsNULL() )
    {
        TEventCallback callback( this, &CTimer::OnPulse );
        SubscribeTo( m_pulseGenerator.GetPointerAlways() ,
                     CPulseGenerator::PulseEvent         ,
                     callback                            );
    }
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

bool
CTimer::SetEnabled( const bool enabled )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_pulseGenerator )
        return false;
    
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
            m_pulseGenerator->RequestPeriodicPulses( this, m_updateDeltaInMilliSecs );
            NotifyObservers( TimerStartedEvent );
            
        }
        else
        {
            // we don't need periodic updates anymore
            m_pulseGenerator->RequestStopOfPeriodicUpdates( this );

            NotifyObservers( TimerStoppedEvent );
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTimer::GetEnabled( void ) const
{GUCEF_TRACE;

    return m_enabled;
}

/*-------------------------------------------------------------------------*/

void
CTimer::SetOpaqueUserData( void* opaqueUserData )
{GUCEF_TRACE;

    m_opaqueUserData = opaqueUserData;
}

/*-------------------------------------------------------------------------*/

void* 
CTimer::GetOpaqueUserData( void ) const
{GUCEF_TRACE;

    return m_opaqueUserData;
}

/*-------------------------------------------------------------------------*/

void
CTimer::OnPulse( CNotifier* notifier                 ,
                 const CEvent& eventid               ,
                 CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    if ( m_enabled )
    {
        UInt64 newTickCount = MT::PrecisionTickCount();
        UInt64 deltaTicks = ( newTickCount - m_lastTimerCycle );
		Float64 deltaMilliSecs = deltaTicks / m_timerFreq;
        m_tickCount += deltaTicks;

        if ( m_immediateTriggerRequested || deltaMilliSecs >= m_updateDeltaInMilliSecs )
        {
            m_immediateTriggerRequested = false;
            m_lastTimerCycle = newTickCount;

            TimerUpdateEventData cuData;
            TTimerUpdateData& updateData = cuData.GetData();
            updateData.tickCount = m_tickCount;
            updateData.updateDeltaInMilliSecs = deltaMilliSecs;
            updateData.opaqueUserData = m_opaqueUserData;
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

 void 
 CTimer::TriggerNow( void )
 {GUCEF_TRACE;
 
    UInt64 newTickCount = MT::PrecisionTickCount();
    UInt64 deltaTicks = ( newTickCount - m_lastTimerCycle );
	Float64 deltaMilliSecs = deltaTicks / m_timerFreq;

    TimerUpdateEventData cuData;
    TTimerUpdateData& updateData = cuData.GetData();
    updateData.tickCount = m_tickCount;
    updateData.updateDeltaInMilliSecs = deltaMilliSecs;
    if ( !NotifyObservers( TimerUpdateEvent, &cuData ) ) return;
 
    Reset();
 }

 /*-------------------------------------------------------------------------*/

 void 
 CTimer::RequestImmediateTrigger( void )
 {GUCEF_TRACE;

    if ( GUCEF_NULL != m_pulseGenerator )
    {
        m_immediateTriggerRequested = true;
        m_pulseGenerator->RequestImmediatePulse();
    }
 }

/*-------------------------------------------------------------------------*/

Float64
CTimer::GetApproxMaxTimerResolutionInMilliSecs( void )
{GUCEF_TRACE;

    return MT::PrecisionTimerResolution() / 1000.0;
}

/*-------------------------------------------------------------------------*/

const CString&
CTimer::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::CORE::CTimer";
    return typeName;
}

/*-------------------------------------------------------------------------*/
                         
bool
CTimer::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;
    
    return false;
}

/*-------------------------------------------------------------------------*/
                         
bool
CTimer::Unlock( void ) const
{GUCEF_TRACE;
    
    return false;
}

/*-------------------------------------------------------------------------*/

void 
CTimer::SetPulseGenerator( PulseGeneratorPtr newPulseGenerator )
{GUCEF_TRACE;
    
    bool wasEnabled = m_enabled;
    UnsubscribeFrom( m_pulseGenerator.GetPointerAlways() );

    SetEnabled( false );
    Reset();

    m_pulseGenerator = newPulseGenerator;
    RegisterPulseGeneratorEventHandlers();

    if ( wasEnabled )
        SetEnabled( wasEnabled );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
