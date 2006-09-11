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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "CTimerPump.h"
#include "CGUCEFApplication.h"
#include "CITimerClient.h"

#define GUCEF_CORE_CTIMER_CPP
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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTimer::CTimer( const UInt32 interval /* = 10 */ )
    : m_lastTimerCycle( 0UL )      ,
      m_enabled( false )           ,
      m_interval( interval )       ,
      m_activationTickCount( 0UL ) ,
      m_tickCount( 0UL )
{
    CTimerPump::Instance()->RegisterTimer( this );
}          

/*-------------------------------------------------------------------------*/

CTimer::CTimer( const CTimer& src )
     : m_interval( src.m_interval )             ,
       m_lastTimerCycle( src.m_lastTimerCycle ) ,
       m_enabled( false )                       ,
       m_activationTickCount( 0UL )             ,
       m_tickCount( 0UL )
{
    CTimerPump::Instance()->RegisterTimer( this );
}

/*-------------------------------------------------------------------------*/

CTimer::~CTimer()
{
    CTimerPump::Instance()->UnregisterTimer( this );
}

/*-------------------------------------------------------------------------*/

CTimer& 
CTimer::operator=( const CTimer& src )
{
    if ( this != &src )
    {
        m_interval = src.m_interval;
        m_lastTimerCycle = src.m_lastTimerCycle;
        m_enabled = src.m_enabled;
        m_tickCount = src.m_tickCount;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
void 
CTimer::SetInterval( const UInt32 interval )
{
    m_interval = interval;
}

/*-------------------------------------------------------------------------*/
    
UInt32 
CTimer::GetInterval( void ) const
{
    return m_interval;
}

/*-------------------------------------------------------------------------*/

void
CTimer::SetEnabled( const bool enabled )
{
    if ( m_enabled != enabled )
    {
        m_enabled = enabled;                
        
        if ( enabled )
        {        
            m_lastTimerCycle = m_activationTickCount = ::GetTickCount();
            m_tickCount = 0UL;           
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CTimer::GetEnabled( void ) const
{
    return m_enabled;
} 

/*-------------------------------------------------------------------------*/

void 
CTimer::Subscribe( CITimerClient* client )
{
    m_clients.insert( client );
}

/*-------------------------------------------------------------------------*/
    
void 
CTimer::Unsubscribe( CITimerClient* client )
{
    m_clients.erase( client );
}

/*-------------------------------------------------------------------------*/

void 
CTimer::OnUpdate( const UInt32 tickCount  ,
                  const UInt32 deltaTicks )
{
    if ( m_enabled )
    {
        Int32 timerDelta( (Int32)tickCount - (Int32)m_lastTimerCycle );
        if ( timerDelta >= (Int32)m_interval )
        {        
            m_tickCount += deltaTicks;
            
            TTimerClientSet::iterator i = m_clients.begin();
            while ( i != m_clients.end() )
            {
                (*i)->OnTimer( *this      ,
                               tickCount  ,
                               timerDelta ); 
                ++i;
            }            
            m_lastTimerCycle = tickCount;   
        }
    }         
}

/*-------------------------------------------------------------------------*/

UInt32 
CTimer::GetRunTicks( void ) const
{
    return ::GetTickCount() - m_activationTickCount;
}

/*-------------------------------------------------------------------------*/

UInt32
CTimer::GetTickCount( void ) const
{
    return m_tickCount;
}

/*-------------------------------------------------------------------------*/

void
CTimer::Reset( void )
{
    m_lastTimerCycle = CGUCEFApplication::Instance()->GetLastUpdateTickCount();
    m_tickCount = 0UL;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/