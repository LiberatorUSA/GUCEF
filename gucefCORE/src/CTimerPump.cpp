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

#include "CTimer.h"

#define GUCEF_CORE_CTIMERPUMP_CPP
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
{

}

/*-------------------------------------------------------------------------*/

CTimerPump::CTimerPump( const CTimerPump& src )
{
}

/*-------------------------------------------------------------------------*/

CTimerPump::~CTimerPump()
{
}

/*-------------------------------------------------------------------------*/

CTimerPump&
CTimerPump::operator=( const CTimerPump& src )
{
    /* dummy, do not use */

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CTimerPump*
CTimerPump::Instance( void )
{
    if ( !m_instance )
    {
        m_instance = new CTimerPump();
    }
    return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CTimerPump::Deinstance( void )
{
    delete m_instance;
    m_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void 
CTimerPump::RegisterTimer( CTimer* timer )
{
    m_timerList.insert( timer );
}

/*-------------------------------------------------------------------------*/
    
void 
CTimerPump::UnregisterTimer( CTimer* timer )
{
    m_timerList.erase( timer );
}

/*-------------------------------------------------------------------------*/

void 
CTimerPump::OnUpdate( const UInt32 tickCount  ,
                      const UInt32 deltaTicks )
{
    TTimerList::iterator i( m_timerList.begin() );
    while ( i != m_timerList.end() )
    {
        (*i)->OnUpdate( tickCount  ,
                        deltaTicks );
        ++i;
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
