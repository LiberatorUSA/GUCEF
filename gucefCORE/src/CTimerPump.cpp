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

#include "CTracer.h"
#include "CTimer.h"
#include "CTimerPump.h"

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

CTimerPump* CTimerPump::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTimerPump::CTimerPump( void )
    : CGUCEFAppSubSystem()                       ,
      m_isATimerActive( false )                  ,
      m_timerList()                              ,
      m_minimalResolution( GUCEFCORE_UINT32MAX )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTimerPump::~CTimerPump()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTimerPump*
CTimerPump::Instance( void )
{GUCEF_TRACE;

    if ( !m_instance )
    {
        m_instance = new CTimerPump();
    }
    return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CTimerPump::Deinstance( void )
{GUCEF_TRACE;

    delete m_instance;
    m_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void 
CTimerPump::RegisterTimer( CTimer* timer )
{GUCEF_TRACE;

    m_timerList.insert( std::pair< CTimer*, bool >( timer, false ) );
    RegisterSubSystem();
}

/*-------------------------------------------------------------------------*/
    
void 
CTimerPump::UnregisterTimer( CTimer* timer )
{GUCEF_TRACE;

    m_timerList.erase( timer );
    if ( m_timerList.empty() )
    {
        UnregisterSubSystem();
    }
}

/*-------------------------------------------------------------------------*/

void 
CTimerPump::OnUpdate( const UInt64 tickCount               ,
                      const Float64 updateDeltaInMilliSecs )
{GUCEF_TRACE;

    TTimerList::iterator i( m_timerList.begin() );
    while ( i != m_timerList.end() )
    {
        (*i).first->OnUpdate();
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CTimerPump::TimerSetRequiresUpdates( CTimer* timer              ,
                                     const bool requiresUpdates )
{GUCEF_TRACE;

    // First we determine if we still have a periodic update requirement
    if ( requiresUpdates )
    {
        m_isATimerActive = true;
    }
    else
    {
        // Check if any other timers are active
        bool isATimerActive = false;
        TTimerList::iterator i( m_timerList.begin() );
        while ( i != m_timerList.end() )
        {
            if ( ( (*i).first != timer ) &&
                 (*i).second              )
            {
                isATimerActive = true;
                break;
            }
            ++i;
        }
        
        m_isATimerActive = isATimerActive;
    }
    
    if ( m_isATimerActive )
    {
        // Determine the minimal resolution of all the timer combined
        // @todo we should be using the smallest divider here
        UInt32 interval = 0;
        TTimerList::iterator i( m_timerList.begin() );
        while ( i != m_timerList.end() )
        {
            interval = (*i).first->GetInterval();
            if ( interval < m_minimalResolution )
            {
                m_minimalResolution = interval;
                break;
            }
            ++i;
        }
        
        SetPeriodicUpdateRequirement( true );
        RequestUpdateInterval( m_minimalResolution );
    }
    else
    {
        SetPeriodicUpdateRequirement( false );
        RequestUpdateInterval( GUCEFCORE_UINT32MAX );
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
