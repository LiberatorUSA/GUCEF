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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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
//      UTILITIES                                                          //
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

CTaskDelegator::CTaskDelegator( CThreadPool* threadPool )
    : MT::CActiveObject()
    , CNotifier()
    , CIPulseGeneratorDriver()
    , m_threadPool( threadPool )
    , m_pulseGenerator()
    , m_taskConsumer()
    , m_taskData( GUCEF_NULL )
    , m_immediatePulseTickets( 0 )
    , m_immediatePulseTicketMax( 1 )
{GUCEF_TRACE;

    assert( GUCEF_NULL != m_threadPool );

    RegisterEvents();

    m_pulseGenerator.SetPulseGeneratorDriver( this );
}

/*-------------------------------------------------------------------------*/

CTaskDelegator::CTaskDelegator( CThreadPool* threadPool       ,
                                CTaskConsumerPtr taskConsumer ,
                                CICloneable* taskData         )
    : MT::CActiveObject()
    , CNotifier()
    , CIPulseGeneratorDriver()
    , m_threadPool( threadPool )
    , m_pulseGenerator()
    , m_taskConsumer( taskConsumer )
    , m_taskData( taskData )
    , m_immediatePulseTickets( 0 )
    , m_immediatePulseTicketMax( 1 )
{GUCEF_TRACE;

    RegisterEvents();

    m_pulseGenerator.SetPulseGeneratorDriver( this );
}

/*-------------------------------------------------------------------------*/

CTaskDelegator::~CTaskDelegator()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_threadPool )
        m_threadPool->UnregisterTaskDelegator( *this );
}

/*-------------------------------------------------------------------------*/

CPulseGenerator&
CTaskDelegator::GetPulseGenerator( void )
{GUCEF_TRACE;

    return m_pulseGenerator;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestImmediatePulse( CPulseGenerator& pulseGenerator )
{GUCEF_TRACE;

    if ( &pulseGenerator == &m_pulseGenerator )
    {
        m_immediatePulseTickets = m_immediatePulseTicketMax;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestPulsesPerImmediatePulseRequest( CPulseGenerator& pulseGenerator                     ,
                                                       const Int32 requestedPulsesPerImmediatePulseRequest )
{GUCEF_TRACE;

    if ( &pulseGenerator == &m_pulseGenerator )
    {
        if ( requestedPulsesPerImmediatePulseRequest > 1 )
            m_immediatePulseTicketMax = requestedPulsesPerImmediatePulseRequest;
        else
            m_immediatePulseTicketMax = 1;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestPeriodicPulses( CPulseGenerator& pulseGenerator    ,
                                       const UInt32 pulseDeltaInMilliSecs )

{GUCEF_TRACE;

    if ( &pulseGenerator == &m_pulseGenerator )
    {
        m_delayInMilliSecs = pulseDeltaInMilliSecs;
        Resume();
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestPulseInterval( CPulseGenerator& pulseGenerator    ,
                                      const UInt32 pulseDeltaInMilliSecs )
{GUCEF_TRACE;

    if ( &pulseGenerator == &m_pulseGenerator )
    {
        m_delayInMilliSecs = pulseDeltaInMilliSecs;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::RequestStopOfPeriodicUpdates( CPulseGenerator& pulseGenerator )
{GUCEF_TRACE;

    if ( &pulseGenerator == &m_pulseGenerator )
    {
        Pause( false );
    }
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::OnThreadStart( void* taskdata )
{GUCEF_TRACE;

    NotifyObservers( ThreadStartedEvent );
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

    if ( GUCEF_NULL != m_threadPool )
    {
        m_threadPool->TaskCleanup( taskConsumer ,
                                   taskData     );
    }

    m_taskConsumer = GUCEF_NULL;
    taskConsumer->SetTaskDelegator( GUCEF_NULL );
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::OnThreadCycle( void* taskdata )
{GUCEF_TRACE;

    CTaskConsumerPtr taskConsumer;
    if ( ( GUCEF_NULL != m_threadPool ) &&
         ( m_threadPool->GetQueuedTask( taskConsumer    ,
                                        &m_taskData     ) ) )
    {
        m_taskConsumer = taskConsumer;

        ProcessTask( taskConsumer ,
                     m_taskData   );

        TaskCleanup( taskConsumer ,
                     m_taskData   );

        taskConsumer = GUCEF_NULL;
        m_taskData = GUCEF_NULL;
    }

    // Return false, this is an infinate task worker thread
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::ProcessTask( CTaskConsumerPtr taskConsumer ,
                             CICloneable* taskData         )
{GUCEF_TRACE;

    bool returnStatus = false;

    // first establish the bi-directional link
    // this delegator is going to be the one to execute this task
    // This means the task is now assigned to the thread which is represented by this delegator
    taskConsumer->SetTaskDelegator( this );

    // Now we go through the execution sequence within a cycle as if this
    // where a thread sequence
    m_consumerBusy = true;
    if ( taskConsumer->OnTaskStart( taskData ) )
    {
        taskConsumer->OnTaskStarted( taskData );

        // cycle the task as long as it is not "done"
        while ( !IsDeactivationRequested() )
        {
            // Perform a cycle directly and ask the task if we are done
            if ( taskConsumer->OnTaskCycle( taskData ) )
            {
                // Task says we are done
                break;
            }

            SendDriverPulse( m_pulseGenerator );
            if ( m_immediatePulseTickets > 0 )
            {
                --m_immediatePulseTickets;
            }
            else
            {
                m_pulseGenerator.WaitTillNextPulseWindow( m_minimalCycleDeltaInMilliSecs );
            }
        }

        taskConsumer->OnTaskEnding( taskData, false );
        taskConsumer->OnTaskEnded( taskData, false );
        returnStatus = true;
    }
    else
    {
        taskConsumer->OnTaskStartupFailed( taskData );
    }
    m_consumerBusy = false;
    return returnStatus;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnThreadPausedForcibly( void* taskdata )
{GUCEF_TRACE;

    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {
        taskConsumer->OnTaskPaused( static_cast< CICloneable* >( taskdata ), true );
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnThreadResumed( void* taskdata )
{GUCEF_TRACE;

    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {
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
    // We should give the consume a chance to cleanup
    CTaskConsumerPtr taskConsumer = m_taskConsumer;
    if ( !taskConsumer.IsNULL() )
    {
        taskConsumer->OnTaskEnded( static_cast< CICloneable* >( taskdata ), m_consumerBusy );
        taskConsumer->SetTaskDelegator( GUCEF_NULL );
    }
}

/*-------------------------------------------------------------------------*/

CTaskConsumerPtr
CTaskDelegator::GetTaskConsumer( void )
{GUCEF_TRACE;

    return m_taskConsumer;
}

/*-------------------------------------------------------------------------*/

const MT::CILockable*
CTaskDelegator::AsLockable( void ) const
{GUCEF_TRACE;

    return static_cast< const MT::CActiveObject* >( this );
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::Lock( void ) const
{GUCEF_TRACE;

    return MT::CActiveObject::Lock();
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
