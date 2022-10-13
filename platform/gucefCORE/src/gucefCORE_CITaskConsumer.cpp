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

#ifndef GUCEF_CORE_CTASKDELEGATOR_H
#include "gucefCORE_CTaskDelegator.h"
#define GUCEF_CORE_CTASKDELEGATOR_H
#endif /* GUCEF_CORE_CTASKDELEGATOR_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

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
    : CTSGNotifier( PulseGeneratorPtr(), true, false )
    , m_taskId()
    , m_threadPool()
    , m_delegator()
    , m_ownedByThreadPool( false )
    , m_inPhasedSetup( false )
{GUCEF_TRACE;

    RegisterEvents();
    CCoreGlobal::Instance()->GetTaskManager().RegisterTaskConsumerId( m_taskId );
}

/*-------------------------------------------------------------------------*/

CTaskConsumer::~CTaskConsumer()
{GUCEF_TRACE;

    SignalUpcomingDestruction();    
    CCoreGlobal::Instance()->GetTaskManager().UnregisterTaskConsumerId( m_taskId );    
}

/*-------------------------------------------------------------------------*/

void 
CTaskConsumer::SetIsInPhasedSetup( bool inPhasedSetup )
{GUCEF_TRACE;

    m_inPhasedSetup = inPhasedSetup;
}

/*-------------------------------------------------------------------------*/

bool 
CTaskConsumer::GetIsInPhasedSetup( void ) const
{GUCEF_TRACE;

    return m_inPhasedSetup;
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::SetIsOwnedByThreadPool( bool ownedByThreadPool )
{GUCEF_TRACE;

    m_ownedByThreadPool = ownedByThreadPool;
}

/*-------------------------------------------------------------------------*/

bool
CTaskConsumer::IsOwnedByThreadPool( void ) const
{GUCEF_TRACE;

    return m_ownedByThreadPool;
}

/*-------------------------------------------------------------------------*/

CTaskConsumer::TTaskDelegatorBasicPtr
CTaskConsumer::GetTaskDelegator( void )
{GUCEF_TRACE;

    return m_delegator;
}

/*-------------------------------------------------------------------------*/

UInt32
CTaskConsumer::GetDelegatorThreadId( void ) const
{GUCEF_TRACE;

    TTaskDelegatorBasicPtr delegator = m_delegator;
    return !delegator.IsNULL() ? delegator->GetThreadID() : 0;
}

/*-------------------------------------------------------------------------*/

void 
CTaskConsumer::RequestTaskCycleDelayInMs( UInt32 requestedDelayInMs )
{GUCEF_TRACE;

    TTaskDelegatorBasicPtr delegator = m_delegator;
    if ( !delegator.IsNULL() )
        delegator->RequestTaskCycleDelayInMs( requestedDelayInMs );
}

/*-------------------------------------------------------------------------*/

UInt32
CTaskConsumer::GetTaskId( void ) const
{GUCEF_TRACE;

    return m_taskId;
}

/*-------------------------------------------------------------------------*/

bool
CTaskConsumer::WaitForTaskToFinish( Int32 timeoutInMs )
{GUCEF_TRACE;

    TBasicThreadPoolPtr threadPool = m_threadPool;
    return !threadPool.IsNULL() ? threadPool->WaitForTaskToFinish( m_taskId, timeoutInMs ) : false;
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
CTaskConsumer::SetTaskDelegator( const TTaskDelegatorBasicPtr& delegator )
{GUCEF_TRACE;

    m_delegator = delegator;
    if ( !m_delegator.IsNULL() )
    {
        SetPulseGenerator( m_delegator->GetPulseGenerator() );
    }
    else
    {
        SetPulseGenerator( PulseGeneratorPtr() );
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::SetThreadPool( const TThreadPoolBasicPtr& threadPool )
{GUCEF_TRACE;

    m_threadPool = threadPool;
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

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::OnTaskEnding( CICloneable* taskdata ,
                             bool willBeForced     )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CTaskConsumer::SetCpuAffinityMask( UInt32 affinityMaskSize ,
                                   void* affinityMask      )
{GUCEF_TRACE;

    TTaskDelegatorBasicPtr delegator = m_delegator;
    if ( !delegator.IsNULL() )
    {
        return delegator->SetCpuAffinityMask( affinityMaskSize, affinityMask );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskConsumer::SetCpuAffinityByCpuId( UInt32 cpuId )
{GUCEF_TRACE;

    TTaskDelegatorBasicPtr delegator = m_delegator;
    if ( !delegator.IsNULL() )
    {
        return delegator->SetCpuAffinityByCpuId( cpuId );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskConsumer::IsDeactivationRequested( void ) const
{GUCEF_TRACE;

    TTaskDelegatorBasicPtr delegator = m_delegator;
    if ( !delegator.IsNULL() )
    {
        return delegator->IsDeactivationRequested();
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CTaskConsumer::RequestTaskToStop( bool waitOnStop )
{GUCEF_TRACE;

    TTaskDelegatorBasicPtr delegator = m_delegator;
    if ( !delegator.IsNULL() )
    {
        TBasicThreadPoolPtr threadPool = delegator->GetThreadPool();
        if ( !threadPool.IsNULL() )
        {
            return threadPool->RequestTaskToStop( m_taskId, waitOnStop );
        }
    }
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
