/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_CIPULSEGENERATORDRIVER_H
#include "gucefCORE_CIPulseGeneratorDriver.h"
#define GUCEF_CORE_CIPULSEGENERATORDRIVER_H
#endif /* GUCEF_CORE_CIPULSEGENERATORDRIVER_H ? */

#include "gucefCORE_CPulseGenerator.h"

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

const CEvent CPulseGenerator::PulseEvent = "GUCEF::CORE::CPulseGenerator::PulseEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CPulseGenerator::RegisterEvents( void )
{GUCEF_TRACE;

    PulseEvent.Initialize();
}

/*--------------------------------------------------------------------------*/

CPulseGenerator::CPulseGenerator( void )
    : CNotifier()                                      ,
      m_lastCycleTickCount( MT::PrecisionTickCount() ) ,
      m_updateDeltaInMilliSecs( 10 )                   ,
      m_ticksPerMs( 1000 )                             ,
      m_forcedStopOfPeriodPulses( false )              ,
      m_periodicUpdateRequestors()                     ,
      m_driver( NULL )                                 ,
      m_mutex()
{GUCEF_TRACE;

    RegisterEvents();
    
    // Cache the precision timer resolution in time slices per millisecond
    m_ticksPerMs = ( MT::PrecisionTimerResolution() / 1000.0 );
}

/*--------------------------------------------------------------------------*/

CPulseGenerator::~CPulseGenerator()
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/

const CString&
CPulseGenerator::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CPulseGenerator";
    return typeName;
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::RequestPulse( void )
{GUCEF_TRACE;

    LockData();
    // Are we already doing period updates ?
    if ( m_periodicUpdateRequestors.size() == 0 )
    {
        // We are not so we must trigger an update either by delegation to a  
        // driver or by doing so directly
        if ( GUCEF_NULL != m_driver )
        {
            UnlockData();
            m_driver->RequestPulse( *this );
        }
        else
        {
            UInt64 newTickCount = MT::PrecisionTickCount();
            Int64 deltaTicks = newTickCount - m_lastCycleTickCount;
            Float64 updateDeltaTime = deltaTicks / m_ticksPerMs;
            m_lastCycleTickCount = newTickCount;
            UnlockData();
            
            CPulseData pulseData( newTickCount, deltaTicks, updateDeltaTime );
            NotifyObservers( PulseEvent, &pulseData );
        }
    }
    else
        UnlockData();
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::RequestPeriodicPulses( void* requestor )
{GUCEF_TRACE;

    LockData();
    m_periodicUpdateRequestors[ requestor ] = m_updateDeltaInMilliSecs;
    if ( m_driver != NULL && m_periodicUpdateRequestors.size() == 1 )
    {
        UnlockData();
        m_driver->RequestPeriodicPulses( *this, m_updateDeltaInMilliSecs );
        return;
    }
    UnlockData();    
}

/*--------------------------------------------------------------------------*/

UInt32
GreatestCommonDivider( UInt32 x, UInt32 y )
{GUCEF_TRACE;

    if ( x != 0 && y != 0 )
    {
        UInt32 r = x % y;
        while ( y != 0 )
        {
            x = y;
            y = r;
            r = x % y;
        }
        return y;
    }
    
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32
CPulseGenerator::DetermineRequiredPulseInterval( void ) const
{GUCEF_TRACE;
    
    if ( 0 == m_periodicUpdateRequestors.size() )
    {
        return m_updateDeltaInMilliSecs;
    }
    else
    if ( 1 > m_periodicUpdateRequestors.size() )
    {
        UInt32 a, gcd;
        TPeriodicPulseRequestorMap::const_iterator i = m_periodicUpdateRequestors.begin();
        
        gcd = (*i).second;
        ++i;
                 
        do
        {
            a = (*i).second;
            gcd = GreatestCommonDivider( a, gcd );
            ++i;
        }
        while ( i != m_periodicUpdateRequestors.end() );
        
        return gcd;
    }
    else
    {
        return (*m_periodicUpdateRequestors.begin()).second;
    }
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::RequestPeriodicPulses( void* requestor                    ,
                                        const UInt32 pulseDeltaInMilliSecs )
{GUCEF_TRACE;

    LockData();
    if ( NULL != requestor )
    {
        m_periodicUpdateRequestors[ requestor ] = pulseDeltaInMilliSecs;
    }
    
    m_updateDeltaInMilliSecs = DetermineRequiredPulseInterval();
    
    if ( NULL != m_driver )
    {
        UnlockData();
        m_driver->RequestPeriodicPulses( *this, m_updateDeltaInMilliSecs );
        return;
    }
    UnlockData(); 
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::RequestStopOfPeriodicUpdates( void* requestor )
{GUCEF_TRACE;
    
    LockData();
    if ( NULL != requestor )
    {
        m_periodicUpdateRequestors.erase( requestor );
    }
    
    // Determine the new common divider interval for all clients
    m_updateDeltaInMilliSecs = DetermineRequiredPulseInterval();
    
    // Check if we have a driver we can pass the request on to
    if ( NULL != m_driver )
    {
        // Depending on whether we have more clients wanting updates
        // we should either update the frequency or switch to manually requested pulses
        if ( !m_periodicUpdateRequestors.empty() )
        {
            m_driver->RequestPulseInterval( *this, m_updateDeltaInMilliSecs );
        }
        else
        {
            m_driver->RequestStopOfPeriodicUpdates( *this );
        }
    }    
    UnlockData();
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::RequestPulseInterval( const UInt32 minimalPulseDeltaInMilliSecs )
{GUCEF_TRACE;

    LockData();
    if ( minimalPulseDeltaInMilliSecs < m_updateDeltaInMilliSecs )
    {
        if ( NULL != m_driver )
        {
            m_updateDeltaInMilliSecs = minimalPulseDeltaInMilliSecs;
            UnlockData();
            m_driver->RequestPulseInterval( *this, m_updateDeltaInMilliSecs );
            return;
        }
    }
    UnlockData();
}

/*--------------------------------------------------------------------------*/

bool
CPulseGenerator::IsPulsingPeriodicly( void ) const
{GUCEF_TRACE;

    return m_periodicUpdateRequestors.size() > 0 && !m_forcedStopOfPeriodPulses;
}

/*--------------------------------------------------------------------------*/
    
UInt32
CPulseGenerator::GetRequiredPulseDeltaInMilliSecs( void ) const
{GUCEF_TRACE;

    return m_updateDeltaInMilliSecs;
}

/*--------------------------------------------------------------------------*/

Float64
CPulseGenerator::GetActualPulseDeltaInMilliSecs( void ) const
{GUCEF_TRACE;

    Int64 deltaTicks = (Int64) ( MT::PrecisionTickCount() - m_lastCycleTickCount );
    if ( deltaTicks > 0 )
    {
        return deltaTicks / m_ticksPerMs;
    }
    return 0.0;
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::WaitTillNextPulseWindow( UInt32 forcedMinimalDeltaInMilliSecs  ,
                                          UInt32 desiredMaximumDeltaInMilliSecs ) const
{GUCEF_TRACE;
   
    Float64 deltaInMs = GetActualPulseDeltaInMilliSecs();
    Int32 timeLeftToWaitInMs = (Int32) ( m_updateDeltaInMilliSecs - deltaInMs );

    if ( timeLeftToWaitInMs <= 0 )
        return; // Time's up already

    // forcedMinimalDeltaInMilliSecs == 0 means no forced minimum
    if ( ( forcedMinimalDeltaInMilliSecs > 0 ) && 
         ( forcedMinimalDeltaInMilliSecs >= (UInt32) timeLeftToWaitInMs ) )
    {
        timeLeftToWaitInMs = forcedMinimalDeltaInMilliSecs;
    }
    // desiredMaximumDeltaInMilliSecs == 0 means no maximum desired
    if ( ( desiredMaximumDeltaInMilliSecs > 0 ) &&
         ( desiredMaximumDeltaInMilliSecs < (UInt32) timeLeftToWaitInMs ) )
    {
        timeLeftToWaitInMs = desiredMaximumDeltaInMilliSecs;
    }

    MT::PrecisionDelay( timeLeftToWaitInMs );
}

/*--------------------------------------------------------------------------*/

UInt64
CPulseGenerator::GetTickCountAtLastPulse( void ) const
{GUCEF_TRACE;
    
    return m_lastCycleTickCount;
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::SetPulseGeneratorDriver( CIPulseGeneratorDriver* driver )
{GUCEF_TRACE;

    LockData();
    m_driver = driver;
    if ( IsPulsingPeriodicly() )
    {
        UnlockData();
        if ( GUCEF_NULL != m_driver )
            m_driver->RequestPulseInterval( *this, m_updateDeltaInMilliSecs );
    }
    else
    {
        UnlockData();        
        if ( GUCEF_NULL != m_driver )
            m_driver->RequestPulse( *this );
    }
}

/*--------------------------------------------------------------------------*/
    
CIPulseGeneratorDriver*
CPulseGenerator::GetPulseGeneratorDriver( void ) const
{GUCEF_TRACE;
    
    return m_driver;
}

/*--------------------------------------------------------------------------*/

bool
CPulseGenerator::IsForcedStopOfPeriodicPulsesRequested( void ) const
{GUCEF_TRACE;

    return m_forcedStopOfPeriodPulses;
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::ForceStopOfPeriodicPulses( void )
{GUCEF_TRACE;
    
    LockData();
    m_forcedStopOfPeriodPulses = true;
    if ( NULL != m_driver )
    {
        UnlockData();
        m_driver->RequestStopOfPeriodicUpdates( *this );
        return;
    }
    UnlockData();
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::AllowPeriodicPulses( void )
{GUCEF_TRACE;

    LockData();
    if ( m_forcedStopOfPeriodPulses )
    {
        m_forcedStopOfPeriodPulses = false;        
        if ( NULL != m_driver && m_periodicUpdateRequestors.size() > 0 )
        {
            UnlockData();
            m_driver->RequestPeriodicPulses( *this, m_updateDeltaInMilliSecs );
            return;
        }
    }
    UnlockData();
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::RequestPeriodicPulses( void )
{GUCEF_TRACE;

    LockData();
    if ( !m_forcedStopOfPeriodPulses )
    {
        if ( NULL != m_driver )
        {
            UnlockData();
            m_driver->RequestPeriodicPulses( *this, m_updateDeltaInMilliSecs );
            return;
        }
    }    
    UnlockData();
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::LockData( void ) const
{GUCEF_TRACE;

    m_mutex.Lock();
}

/*--------------------------------------------------------------------------*/
    
void
CPulseGenerator::UnlockData( void ) const
{GUCEF_TRACE;

    m_mutex.Unlock();
}

/*--------------------------------------------------------------------------*/

void
CPulseGenerator::OnDriverPulse( void )
{GUCEF_TRACE;

    LockData();
    UInt64 tickCount = MT::PrecisionTickCount();
    UInt64 deltaTicks = tickCount - m_lastCycleTickCount;
    Float64 deltaMilliSecs = deltaTicks / m_ticksPerMs;   
    m_lastCycleTickCount = tickCount; 
    UnlockData();
    
    CPulseData pulseData( tickCount, deltaTicks, deltaMilliSecs );
    NotifyObservers( PulseEvent, &pulseData );
                                   
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
