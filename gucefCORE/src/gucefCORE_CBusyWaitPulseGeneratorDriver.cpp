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

#include "gucefCORE_CBusyWaitPulseGeneratorDriver.h"

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

CBusyWaitPulseGeneratorDriver::CBusyWaitPulseGeneratorDriver( void )
    : CIPulseGeneratorDriver()  ,
      m_loop( false )           ,
      m_desiredPulseDelta( 10 )
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/

CBusyWaitPulseGeneratorDriver::CBusyWaitPulseGeneratorDriver( const CBusyWaitPulseGeneratorDriver& src )
    : CIPulseGeneratorDriver( src ) ,
      m_loop( false )               ,
      m_desiredPulseDelta( 10 )
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/

CBusyWaitPulseGeneratorDriver::~CBusyWaitPulseGeneratorDriver()
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/

void
CBusyWaitPulseGeneratorDriver::RequestPulse( CPulseGenerator& pulseGenerator )
{GUCEF_TRACE;

    SendDriverPulse( pulseGenerator );
}

/*--------------------------------------------------------------------------*/

void
CBusyWaitPulseGeneratorDriver::Run( CPulseGenerator& pulseGenerator )
{GUCEF_TRACE;
    
    m_loop = true;
    Float64 remainingDelta;
    while ( m_loop )
    {
        remainingDelta = m_desiredPulseDelta - pulseGenerator.GetActualPulseDeltaInMilliSecs();
        if ( remainingDelta <= 0.0 )
        {
            SendDriverPulse( pulseGenerator );
        }
        else
        {
            if ( remainingDelta > 1.0 )
            {
                //MT::PrecisionDelay( (UInt32)remainingDelta ); 
            }
        }
    }
}

/*--------------------------------------------------------------------------*/
    
void
CBusyWaitPulseGeneratorDriver::RequestPeriodicPulses( CPulseGenerator& pulseGenerator    ,
                                                      const UInt32 pulseDeltaInMilliSecs )
{GUCEF_TRACE;

    m_desiredPulseDelta = pulseDeltaInMilliSecs;                                      
}

/*--------------------------------------------------------------------------*/

void
CBusyWaitPulseGeneratorDriver::RequestPulseInterval( CPulseGenerator& pulseGenerator    ,
                                                     const UInt32 pulseDeltaInMilliSecs )
{GUCEF_TRACE;

    m_desiredPulseDelta = pulseDeltaInMilliSecs;
}

/*--------------------------------------------------------------------------*/
    
void
CBusyWaitPulseGeneratorDriver::RequestStopOfPeriodicUpdates( CPulseGenerator& pulseGenerator )
{GUCEF_TRACE;

    m_loop = false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
