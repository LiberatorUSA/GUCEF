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

#define GUCEF_CORE_COBSERVERPUMP_CPP
#include "CObserverPump.h"

#include "CPumpedObserver.h"

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

CObserverPump* CObserverPump::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CObserverPump::CObserverPump( void )
{
}

/*-------------------------------------------------------------------------*/

CObserverPump::CObserverPump( const CObserverPump& src )
{
}

/*-------------------------------------------------------------------------*/

CObserverPump::~CObserverPump()
{
}

/*-------------------------------------------------------------------------*/

CObserverPump&
CObserverPump::operator=( const CObserverPump& src )
{
    /* dummy, do not use */

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CObserverPump*
CObserverPump::Instance( void )
{
    if ( !m_instance )
    {
        m_instance = new CObserverPump();
    }
    return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CObserverPump::Deinstance( void )
{
    delete m_instance;
    m_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void 
CObserverPump::RegisterObserver( CPumpedObserver* observer )
{
    m_observerList.insert( observer );
}

/*-------------------------------------------------------------------------*/
    
void 
CObserverPump::UnregisterObserver( CPumpedObserver* observer )
{
    m_observerList.erase( observer );
}

/*-------------------------------------------------------------------------*/

void 
CObserverPump::OnUpdate( const UInt64 tickCount               ,
                         const Float64 updateDeltaInMilliSecs )
{
    TObserverList::iterator i( m_observerList.begin() );
    while ( i != m_observerList.end() )
    {
        (*i)->OnUpdate();
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
