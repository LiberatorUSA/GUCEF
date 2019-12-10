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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CTASKDELEGATOR_H
#include "gucefCORE_CTaskDelegator.h"
#define GUCEF_CORE_CTASKDELEGATOR_H
#endif /* GUCEF_CORE_CTASKDELEGATOR_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#include "gucefCORE_CITaskConsumer.h"

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

const CEvent CTaskConsumer::TaskKilledEvent = "GUCEF::CORE::CTaskConsumer::TaskKilledEvent";
const CEvent CTaskConsumer::TaskStartedEvent = "GUCEF::CORE::CTaskConsumer::TaskStartedEvent";
const CEvent CTaskConsumer::TaskStartupFailedEvent = "GUCEF::CORE::CTaskConsumer::TaskStartupFailedEvent";
const CEvent CTaskConsumer::TaskPausedEvent = "GUCEF::CORE::CTaskConsumer::TaskPausedEvent";
const CEvent CTaskConsumer::TaskResumedEvent = "GUCEF::CORE::CTaskConsumer::TaskResumedEvent";
const CEvent CTaskConsumer::TaskFinishedEvent = "GUCEF::CORE::CTaskConsumer::TaskFinishedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CTaskConsumer::RegisterEvents( void )
{GUCEF_TRACE;

    TaskKilledEvent.Initialize();
    TaskStartedEvent.Initialize();
    TaskStartupFailedEvent.Initialize();
    TaskPausedEvent.Initialize();
    TaskResumedEvent.Initialize();
    TaskFinishedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CTaskConsumer::CTaskConsumer( void )
    : CObservingNotifier()          ,
      m_delegator( NULL )           ,
      m_taskId()                    ,
      m_ownedByTaskManager( false )
{GUCEF_TRACE;

    RegisterEvents();
    CCoreGlobal::Instance()->GetTaskManager().RegisterTaskConsumer( *this, m_taskId );
}

/*-------------------------------------------------------------------------*/

CTaskConsumer::~CTaskConsumer()
{GUCEF_TRACE;

    CCoreGlobal::Instance()->GetTaskManager().UnregisterTaskConsumer( *this, m_taskId );
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::SetIsOwnedByTaskManager( bool ownedByTaskManager )
{GUCEF_TRACE;

    m_ownedByTaskManager = ownedByTaskManager;
}

/*-------------------------------------------------------------------------*/

bool
CTaskConsumer::IsOwnedByTaskManager( void ) const
{GUCEF_TRACE;

    return m_ownedByTaskManager;
}

/*-------------------------------------------------------------------------*/

CTaskDelegator*
CTaskConsumer::GetTaskDelegator( void )
{GUCEF_TRACE;

    return m_delegator;
}


/*-------------------------------------------------------------------------*/

CPulseGenerator*
CTaskConsumer::GetPulseGenerator( void )
{GUCEF_TRACE;

    return m_delegator != GUCEF_NULL ? &m_delegator->GetPulseGenerator() : GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

UInt32
CTaskConsumer::GetTaskId( void ) const
{GUCEF_TRACE;

    return m_taskId;
}

/*-------------------------------------------------------------------------*/

const CString&
CTaskConsumer::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::CORE::CTaskConsumer";
    return typeName;
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::SetTaskDelegator( CTaskDelegator* delegator )
{GUCEF_TRACE;

    m_delegator = delegator;
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::OnTaskStarted( CICloneable* taskdata )
{GUCEF_TRACE;

    NotifyObservers( TaskStartedEvent );
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::OnTaskStartupFailed( CICloneable* taskdata )
{GUCEF_TRACE;

    NotifyObservers( TaskStartupFailedEvent );
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::OnTaskPaused( CICloneable* taskdata ,
                              bool forced          )
{GUCEF_TRACE;

    NotifyObservers( TaskPausedEvent );
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::OnTaskResumed( CICloneable* taskdata )
{GUCEF_TRACE;

    NotifyObservers( TaskResumedEvent );
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::OnTaskEnded( CICloneable* taskdata ,
                            bool forced           )
{GUCEF_TRACE;

    NotifyObservers( TaskFinishedEvent );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
