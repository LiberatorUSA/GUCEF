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
#include "gucefMT_dvmtoswrap.h"         /* OS wrappers for threading functionality */
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#include "gucefCORE_CTaskDelegator.h"

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

const CEvent CTaskDelegator::ThreadKilledEvent = "GUCEF::CORE::CTaskDelegator::ThreadKilledEvent";
const CEvent CTaskDelegator::ThreadStartedEvent = "GUCEF::CORE::CTaskDelegator::ThreadStartedEvent";
const CEvent CTaskDelegator::ThreadPausedEvent = "GUCEF::CORE::CTaskDelegator::ThreadPausedEvent";
const CEvent CTaskDelegator::ThreadResumedEvent = "GUCEF::CORE::CTaskDelegator::ThreadResumedEvent";
const CEvent CTaskDelegator::ThreadFinishedEvent = "GUCEF::CORE::CTaskDelegator::ThreadFinishedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CTaskDelegator::RegisterEvents( void )
{GUCEF_TRACE;

    ThreadKilledEvent.Initialize();
    ThreadStartedEvent.Initialize();
    ThreadPausedEvent.Initialize();
    ThreadResumedEvent.Initialize();
    ThreadFinishedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CTaskDelegator::CTaskDelegator( const TBasicThreadPoolPtr& threadPool )
    : MT::CActiveObject()
    , CNotifier()
    , CTSharedPtrCreator< CTaskDelegator, MT::CMutex >( this )
    , CIPulseGeneratorDriver()
    , m_pulseGenerator()
    , m_taskConsumer()
    , m_taskData( GUCEF_NULL )
    , m_threadPool( threadPool )
    , m_consumerBusy( false )
    , m_sendRegularPulses( false )
    , m_immediatePulseTickets( 0 )
    , m_immediatePulseTicketMax( 1 )
    , m_taskRequestedCycleDelayInMs( 0 )
{GUCEF_TRACE;

    assert( !m_threadPool.IsNULL() );

    RegisterEvents();

    m_pulseGenerator = CPulseGenerator::CreateSharedObj();
    m_pulseGenerator->SetPulseGeneratorDriver( this );
}

/*-------------------------------------------------------------------------*/

CTaskDelegator::CTaskDelegator( const TBasicThreadPoolPtr& threadPool ,
                                const CTaskConsumerPtr& taskConsumer  ,
                                CICloneable* taskData                 )
    : MT::CActiveObject()
    , CNotifier()
    , CTSharedPtrCreator< CTaskDelegator, MT::CMutex >( this )
    , CIPulseGeneratorDriver()
    , m_pulseGenerator()
    , m_taskConsumer( taskConsumer )
    , m_taskData( taskData )
    , m_threadPool( threadPool )
    , m_consumerBusy( false )
    , m_sendRegularPulses( false )
    , m_immediatePulseTickets( 0 )
    , m_immediatePulseTicketMax( 1 )
    , m_taskRequestedCycleDelayInMs( 0 )
{GUCEF_TRACE;

    RegisterEvents();

    m_pulseGenerator = CPulseGenerator::CreateSharedObj();
    m_pulseGenerator->SetPulseGeneratorDriver( this );

    // If we are being handed the task consumer already we can already establish the bi-directional link
    // this delegator is going to be the one to execute this task
    // This means the task is now assigned to the thread which is represented by this delegator
    m_taskConsumer->SetTaskDelegator( CreateBasicSharedPtr() );
}

/*-------------------------------------------------------------------------*/

CTaskDelegator::~CTaskDelegator()
{GUCEF_TRACE;

    if ( !NotifyObservers( DestructionEvent ) ) 
        return;

    UnsubscribeAllFromNotifier();
    m_pulseGenerator->SetPulseGeneratorDriver( GUCEF_NULL );
    m_pulseGenerator.Unlink();
    m_threadPool.Unlink();
    
    if ( !m_taskConsumer.IsNULL() )
        m_taskConsumer->SetPulseGenerator( PulseGeneratorPtr() );
    m_taskConsumer.Unlink();
}

/*-------------------------------------------------------------------------*/

TBasicThreadPoolPtr
CTaskDelegator::GetThreadPool( void )
{GUCEF_TRACE;

    return m_threadPool;
}

/*-------------------------------------------------------------------------*/

PulseGeneratorPtr
CTaskDelegator::GetPulseGenerator( void )
{GUCEF_TRACE;

    return m_pulseGenerator;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestImmediatePulse( CPulseGenerator& pulseGenerator )
{GUCEF_TRACE;

    if ( &pulseGenerator == m_pulseGenerator.GetPointerAlways() )
    {
        m_immediatePulseTickets = m_immediatePulseTicketMax;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestPulsesPerImmediatePulseRequest( CPulseGenerator& pulseGenerator                     ,
                                                       const Int32 requestedPulsesPerImmediatePulseRequest )
{GUCEF_TRACE;

    if ( &pulseGenerator == m_pulseGenerator.GetPointerAlways() )
    {
        if ( requestedPulsesPerImmediatePulseRequest > 1 )
            m_immediatePulseTicketMax = requestedPulsesPerImmediatePulseRequest;
        else
            m_immediatePulseTicketMax = 1;
        m_sendRegularPulses = true;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestPeriodicPulses( CPulseGenerator& pulseGenerator    ,
                                       const UInt32 pulseDeltaInMilliSecs )

{GUCEF_TRACE;

    if ( &pulseGenerator == m_pulseGenerator.GetPointerAlways() )
    {
        m_delayInMilliSecs = pulseDeltaInMilliSecs;
        m_sendRegularPulses = true;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestPulseInterval( CPulseGenerator& pulseGenerator    ,
                                      const UInt32 pulseDeltaInMilliSecs )
{GUCEF_TRACE;

    if ( &pulseGenerator == m_pulseGenerator.GetPointerAlways() )
    {
        m_delayInMilliSecs = pulseDeltaInMilliSecs;
        m_sendRegularPulses = true;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestStopOfPeriodicUpdates( CPulseGenerator& pulseGenerator )
{GUCEF_TRACE;

    if ( &pulseGenerator == m_pulseGenerator.GetPointerAlways() )
    {
        m_sendRegularPulses = false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::OnThreadStart( void* taskdata )
{GUCEF_TRACE;

    ThreadStartedEventData id( GetThreadID() );
    NotifyObservers( ThreadStartedEvent, &id );
    return true;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnThreadStarted( void* taskdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::TaskCleanup( CTaskConsumerPtr taskConsumer ,
                             CICloneable* taskData         )
{GUCEF_TRACE;

    if ( !m_threadPool.IsNULL() )
    {
        m_threadPool->TaskCleanup( taskConsumer ,
                                   taskData     );
    }

    m_taskConsumer.Unlink();
    taskConsumer->SetTaskDelegator( CTaskDelegatorPtr() );
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::OnThreadCycle( void* taskdata )
{GUCEF_TRACE;

    CTaskConsumerPtr taskConsumer;
    if ( ( !m_threadPool.IsNULL() ) &&
         ( m_threadPool->GetQueuedTask( taskConsumer    ,
                                        &m_taskData     ) ) )
    {
        m_taskConsumer = taskConsumer;

        ProcessTask( taskConsumer ,
                     m_taskData   );

        TaskCleanup( taskConsumer ,
                     m_taskData   );

        taskConsumer.Unlink();
        m_taskData = GUCEF_NULL;
    }

    // Return false, this is an infinite task worker thread
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::ProcessTask( CTaskConsumerPtr taskConsumer ,
                             CICloneable* taskData         )
{GUCEF_TRACE;

    bool returnStatus = false;
    bool taskWasFinished = false;

    // Create local refs for the invocation duration to avoid external interference
    PulseGeneratorPtr pulseGenerator = m_pulseGenerator;
    TBasicThreadPoolPtr threadPool = m_threadPool;
    if ( pulseGenerator.IsNULL() || threadPool.IsNULL() )
        return false;
    
    // first establish the bi-directional link
    // this delegator is going to be the one to execute this task
    // This means the task is now assigned to the thread which is represented by this delegator
    taskConsumer->SetTaskDelegator( CreateBasicSharedPtr() );

    // Now that the consumer is linked to the delegator wait until we are
    // given the signal that we are ready to begin operations
    while ( taskConsumer->GetIsInPhasedSetup() )
    {
        if ( !IsDeactivationRequested() )
        {
            pulseGenerator->WaitTillNextPulseWindow( 25 );
        }
        else
        {
            break;
        }
    }
    
    if ( !IsDeactivationRequested() )
    {
        // Now we go through the execution sequence within a cycle as if this
        // where a thread sequence
        m_consumerBusy = true;

        // Notify the threadpool directy
        taskConsumer->SetTaskStatus( TASKSTATUS_STARTUP );
        m_threadPool->OnTaskStartup( taskConsumer );
        if ( taskConsumer->OnTaskStart( taskData ) )
        {
            taskConsumer->SetTaskStatus( TASKSTATUS_RUNNING );
            taskConsumer->OnTaskStarted( taskData );

            // cycle the task as long as it is not "done"
            while ( !IsDeactivationRequested() )
            {
                // Perform a cycle directly and ask the task if we are done
                if ( taskConsumer->OnTaskCycle( taskData ) )
                {
                    // Task says we are done
                    taskWasFinished = true;
                    break;
                }

                if ( m_sendRegularPulses || m_immediatePulseTickets > 0 )
                    SendDriverPulse( *pulseGenerator.GetPointerAlways() );

                if ( m_immediatePulseTickets > 0 )
                {
                    --m_immediatePulseTickets;
                }
                else
                {
                    if ( m_taskRequestedCycleDelayInMs > 0 && m_taskRequestedCycleDelayInMs > m_minimalCycleDeltaInMilliSecs )
                    {
                        UInt32 timeLeftToWait = m_taskRequestedCycleDelayInMs; 
                        while ( !IsDeactivationRequested() && timeLeftToWait >= m_minimalCycleDeltaInMilliSecs )
                        {
                            UInt32 elapsedWaitTimeInMilliSecs = 0;
                            pulseGenerator->WaitTillNextPulseWindow( m_minimalCycleDeltaInMilliSecs, timeLeftToWait, &elapsedWaitTimeInMilliSecs  );

                            Int64 timeLeftToWaitResult = ((Int64)timeLeftToWait) - elapsedWaitTimeInMilliSecs;
                            if ( timeLeftToWaitResult < m_minimalCycleDeltaInMilliSecs )
                                timeLeftToWait = 0;
                            else
                                timeLeftToWait = (UInt32) timeLeftToWaitResult;

                            // While we dont trigger OnTaskCycle for an extended period, we do still send out out driver pulses
                            if ( m_sendRegularPulses )
                                SendDriverPulse( *pulseGenerator.GetPointerAlways() );

                            // Any immediate pulse requests cause us to bust out of the extended cycle delay as it counts as an override
                            if ( m_immediatePulseTickets > 0 )
                                break;
                        }
                        m_taskRequestedCycleDelayInMs = 0;
                    }
                    else
                    {
                        pulseGenerator->WaitTillNextPulseWindow( m_minimalCycleDeltaInMilliSecs );
                    }
                }
            }

            taskConsumer->OnTaskEnding( taskData, false );
            taskConsumer->SetTaskStatus( taskWasFinished ? TASKSTATUS_FINISHED : TASKSTATUS_STOPPED );
            taskConsumer->OnTaskEnded( taskData, false );            

            if ( !threadPool.IsNULL() )
            {
                // Notify the threadpool directy
                threadPool->OnTaskFinished( taskConsumer );
                threadPool->RemoveConsumer( taskConsumer->GetTaskId() );
            }
            returnStatus = true;
        }
        else
        {
            taskConsumer->SetTaskStatus( TASKSTATUS_STARTUP_FAILED );
            taskConsumer->OnTaskStartupFailed( taskData );

            if ( !threadPool.IsNULL() )
            {
                // Notify the threadpool directy
                threadPool->OnTaskStartupFailed( taskConsumer );
                threadPool->RemoveConsumer( taskConsumer->GetTaskId() );
            }
        }
        m_consumerBusy = false;    
    }
    else
    {
        taskConsumer->SetTaskStatus( TASKSTATUS_STOPPED );
        
        // We never even got to start the consumer's work
        // not an error, just an efficiency thing so we still return success
        if ( !threadPool.IsNULL() )
        {
            // Notify the threadpool directy
            threadPool->OnTaskStopped( taskConsumer );
            threadPool->RemoveConsumer( taskConsumer->GetTaskId() );
        }
        returnStatus = true;
    }
    return returnStatus;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestTaskCycleDelayInMs( UInt32 requestedDelayInMs )
{GUCEF_TRACE;

    m_taskRequestedCycleDelayInMs = requestedDelayInMs;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnThreadPausedForcibly( void* taskdata )
{GUCEF_TRACE;

    ThreadPausedEventData id( GetThreadID() );
    if ( !NotifyObservers( ThreadPausedEvent, &id ) ) return;

    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {
        taskConsumer->SetTaskStatus( TASKSTATUS_PAUSED );
        taskConsumer->OnTaskPaused( static_cast< CICloneable* >( taskdata ), true );

        TBasicThreadPoolPtr threadPool = m_threadPool;
        if ( !threadPool.IsNULL() )
        {
            // Notify the threadpool directy
            threadPool->OnTaskPaused( taskConsumer );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnThreadResumed( void* taskdata )
{GUCEF_TRACE;

    ThreadResumedEventData id( GetThreadID() );
    if ( !NotifyObservers( ThreadResumedEvent, &id ) ) return;

    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {        
        TBasicThreadPoolPtr threadPool = m_threadPool;
        if ( !threadPool.IsNULL() )
        {
            // Notify the threadpool directy
            threadPool->OnTaskResumed( taskConsumer );
        }

        taskConsumer->SetTaskStatus( TASKSTATUS_RESUMED );
        taskConsumer->OnTaskResumed( static_cast< CICloneable* >( taskdata ) );
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnThreadEnding( void* taskdata    ,
                                bool willBeForced )
{GUCEF_TRACE;

    // This is invoked from a different thread than the thread represented by the TaskDelegator
    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {
        taskConsumer->OnTaskEnding( static_cast< CICloneable* >( taskdata ), willBeForced );
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnThreadEnded( void* taskdata ,
                               bool forced    )
{GUCEF_TRACE;

    // if we get here and the m_consumerBusy flag is set then the task was killed
    // and we did not finish whatever the consumer was doing gracefully
    // We should give the consumer a chance to cleanup
    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {
        TTaskStatus taskStatusBeforeEnd = taskConsumer->GetTaskStatus();
        if ( !( TASKSTATUS_STOPPED == taskStatusBeforeEnd || 
                TASKSTATUS_FINISHED == taskStatusBeforeEnd ) )
        {
            taskConsumer->SetTaskStatus( TASKSTATUS_STOPPED );
        }
        
        taskConsumer->OnTaskEnded( static_cast< CICloneable* >( taskdata ), m_consumerBusy );

        if ( !m_threadPool.IsNULL() )
            m_threadPool->RemoveConsumer( taskConsumer->GetTaskId() );
        taskConsumer->SetTaskDelegator( CTaskDelegatorPtr() );
    }

    if ( forced )
    {
        ThreadFinishedEventData id( GetThreadID() );
        NotifyObservers( ThreadKilledEvent, &id );
    }
    else
    {
        ThreadFinishedEventData id( GetThreadID() );
        NotifyObservers( ThreadFinishedEvent, &id );
    }
}

/*-------------------------------------------------------------------------*/

CTaskConsumerPtr
CTaskDelegator::GetTaskConsumer( void ) const
{GUCEF_TRACE;

    return m_taskConsumer;
}

/*-------------------------------------------------------------------------*/

bool 
CTaskDelegator::HasTaskData( UInt32 taskId ) const
{GUCEF_TRACE;

    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {    
        if ( taskId == taskConsumer->GetTaskId() )
            return GUCEF_NULL != m_taskData;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CTaskDelegator::GetSerializedTaskDataCopy( UInt32 taskId                                           , 
                                           CDataNode& domNode                                      ,  
                                           const CDataNodeSerializableSettings& serializerSettings ) const
{GUCEF_TRACE;

    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {    
        if ( taskId == taskConsumer->GetTaskId() )
        {
            if ( GUCEF_NULL != m_taskData )
            {
                if ( CCoreGlobal::Instance()->GetTaskManager().IsCustomTaskDataForTaskTypeSerializable( taskConsumer->GetType() ) )
                {
                    const CIDataNodeSerializable* serializableTaskData = static_cast< const CIDataNodeSerializable* >( m_taskData );
                    if ( serializableTaskData->Serialize( domNode, serializerSettings ) )
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::GetThreadInfo( CThreadInfo& info ) const
{GUCEF_TRACE;

    info.SetThreadId( GetThreadID() );
    info.SetThreadStatus( GetThreadStatus() );
        
    UInt64 affinityMask = 0;
    UInt32 affinityMaskSize = sizeof(affinityMask);
    if ( GetCpuAffinityMask( affinityMaskSize, &affinityMask, affinityMaskSize ) )
    {
        info.SetThreadAffinityMask( affinityMask );
    }
    else
    {
        info.SetThreadAffinityMask( 0 );
    }

    UInt32 threadCpuId = 0;
    if ( GetCpuAffinityByCpuId( threadCpuId ) )
    {
        info.SetThreadAffinityByCpuId( threadCpuId );
    }
    else
    {
        info.SetThreadAffinityByCpuId( 0 );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

const MT::CILockable*
CTaskDelegator::AsLockable( void ) const
{GUCEF_TRACE;

    return static_cast< const MT::CActiveObject* >( this );
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return MT::CActiveObject::Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::Unlock( void ) const
{GUCEF_TRACE;

    return MT::CActiveObject::Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
