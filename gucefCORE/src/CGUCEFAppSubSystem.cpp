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

CGUCEFAppSubSystem::CGUCEFAppSubSystem( const bool registerSubSystem /* = false */ )
    : m_isRegistered( false )             ,
      m_updateInterval( 0 )               ,
      m_inNeedOfAnUpdate( false )         ,
      m_requiresPeriodicUpdates( false )
{TRACE;
    
    if ( registerSubSystem )
    {
        RegisterSubSystem();
    }
}

/*--------------------------------------------------------------------------*/

CGUCEFAppSubSystem::~CGUCEFAppSubSystem()
{TRACE;
    
    UnregisterSubSystem();
}

/*--------------------------------------------------------------------------*/

void
CGUCEFAppSubSystem::RegisterSubSystem( void )
{TRACE;
    
    if ( !m_isRegistered )
    {
        CGUCEFApplication::Instance()->RegisterSubSystem( this );
        m_isRegistered = true;
        
        if ( m_inNeedOfAnUpdate )
        {
            CGUCEFApplication::Instance()->DoRequestSubSysUpdate();
        }        
    }
}

/*--------------------------------------------------------------------------*/
    
void
CGUCEFAppSubSystem::UnregisterSubSystem( void )
{TRACE;
    
    if ( m_isRegistered )
    {
        CGUCEFApplication::Instance()->UnregisterSubSystem( this );
        m_isRegistered = false;
    }
}

/*--------------------------------------------------------------------------*/

bool
CGUCEFAppSubSystem::IsSubSystemRegistered( void ) const
{TRACE;
    
    return m_isRegistered;
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
CGUCEFAppSubSystem::SetPeriodicUpdateRequirement( const bool requiresPeriodicUpdates )
{TRACE;
    
    m_requiresPeriodicUpdates = requiresPeriodicUpdates;
    CGUCEFApplication::Instance()->RefreshPeriodicUpdateRequirement();
}

/*--------------------------------------------------------------------------*/

void
CGUCEFAppSubSystem::RequestUpdate( void )
{TRACE;

    if ( !m_inNeedOfAnUpdate )
    {
        m_inNeedOfAnUpdate = true;
        
        if ( m_isRegistered )
        {
            CGUCEFApplication::Instance()->DoRequestSubSysUpdate();
        }
        return;
    }
    
    m_inNeedOfAnUpdate = true;
}

/*--------------------------------------------------------------------------*/
    
void
CGUCEFAppSubSystem::RequestUpdateInterval( const UInt32 updateDeltaInMilliSecs )
{TRACE;

    if ( updateDeltaInMilliSecs != m_updateInterval )
    {
        m_updateInterval = updateDeltaInMilliSecs;
    }
    RequestUpdate();
}

/*--------------------------------------------------------------------------*/

UInt32
CGUCEFAppSubSystem::GetDesiredUpdateInterval( void ) const
{TRACE;

    return m_updateInterval;
}

/*--------------------------------------------------------------------------*/

void
CGUCEFAppSubSystem::OnUpdate( const UInt64 tickCount               ,
                              const Float64 updateDeltaInMilliSecs )
{TRACE;

    /* dummy to avoid mandatory implementation by descending classes */
}

/*--------------------------------------------------------------------------*/

void
CGUCEFAppSubSystem::OnNotify( CNotifier* notifier                 ,
                              const CEvent& eventid               ,
                              CICloneable* eventdata /* = NULL */ )
{TRACE;

    /* dummy to avoid mandatory implementation by descending classes */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
