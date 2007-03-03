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
{TRACE;

}

/*-------------------------------------------------------------------------*/

CTimerPump::~CTimerPump()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CTimerPump*
CTimerPump::Instance( void )
{TRACE;

    if ( !m_instance )
    {
        m_instance = new CTimerPump();
    }
    return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CTimerPump::Deinstance( void )
{TRACE;

    delete m_instance;
    m_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void 
CTimerPump::RegisterTimer( CTimer* timer )
{TRACE;

    m_timerList.insert( std::pair< CTimer*, bool >( timer, false ) );
    RegisterSubSystem();
}

/*-------------------------------------------------------------------------*/
    
void 
CTimerPump::UnregisterTimer( CTimer* timer )
{TRACE;

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
{TRACE;

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
{TRACE;

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
