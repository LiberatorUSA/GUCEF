/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CGUCEFAppSubSystem.h"

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

CGUCEFAppSubSystem::CGUCEFAppSubSystem( void )
{TRACE;
    CGUCEFApplication::Instance()->RegisterSubSystem( this );
}

/*--------------------------------------------------------------------------*/

CGUCEFAppSubSystem::~CGUCEFAppSubSystem()
{TRACE;
    CGUCEFApplication::Instance()->UnregisterSubSystem( this );
}

/*--------------------------------------------------------------------------*/

bool
CGUCEFAppSubSystem::IsInNeedOfAnUpdate( void ) const
{TRACE;
    return m_inNeedOfAnUpdate;
}

/*--------------------------------------------------------------------------*/
    
bool
CGUCEFAppSubSystem::ArePeriodicUpdatesRequired( void ) const
{TRACE;
    return m_requiresPeriodicUpdates;
}

/*--------------------------------------------------------------------------*/

void
CGUCEFAppSubSystem::RequestUpdate( void )
{TRACE;
    if ( !m_inNeedOfAnUpdate )
    {
        m_inNeedOfAnUpdate = true;
        CGUCEFApplication::Instance()->DoRequestSubSysUpdate();
        return;
    }
    
    m_inNeedOfAnUpdate = true;
}

/*--------------------------------------------------------------------------*/
    
void
CGUCEFAppSubSystem::RequestUpdateInterval( const UInt32 deltaTicks )
{TRACE;
    if ( deltaTicks != m_updateInterval )
    {
        m_updateInterval = deltaTicks;
    }
}

/*--------------------------------------------------------------------------*/

UInt32
CGUCEFAppSubSystem::GetDesiredUpdateInterval( void ) const
{TRACE;
    return m_updateInterval;
}

/*--------------------------------------------------------------------------*/

void
CGUCEFAppSubSystem::OnUpdate( const UInt32 applicationTicks ,
                              const UInt32 deltaTicks       )
{TRACE;
    /* dummy to avoid manditory implementation by decending classes */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
