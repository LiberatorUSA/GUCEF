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
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CSINGLETASKDELEGATOR_H
#include "gucefCORE_CSingleTaskDelegator.h"
#define GUCEF_CORE_CSINGLETASKDELEGATOR_H
#endif /* GUCEF_CORE_CSINGLETASKDELEGATOR_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefCORE_CThreadPool.h"

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

const CEvent CThreadPool::ThreadKilledEvent = "GUCEF::CORE::CThreadPool::ThreadKilledEvent";
const CEvent CThreadPool::ThreadStartedEvent = "GUCEF::CORE::CThreadPool::ThreadStartedEvent";
const CEvent CThreadPool::ThreadPausedEvent = "GUCEF::CORE::CThreadPool::ThreadPausedEvent";
const CEvent CThreadPool::ThreadResumedEvent = "GUCEF::CORE::CThreadPool::ThreadResumedEvent";
const CEvent CThreadPool::ThreadFinishedEvent = "GUCEF::CORE::CThreadPool::ThreadFinishedEvent";

const CEvent CThreadPool::TaskQueuedEvent = "GUCEF::CORE::CThreadPool::TaskQueuedEvent";
const CEvent CThreadPool::QueuedTaskStartedEvent = "GUCEF::CORE::CThreadPool::QueuedTaskStartedEvent";
const CEvent CThreadPool::TaskStartedEvent = "GUCEF::CORE::CThreadPool::TaskStartedEvent";
const CEvent CThreadPool::TaskStartupFailedEvent = "GUCEF::CORE::CThreadPool::TaskStartupFailedEvent";
const CEvent CThreadPool::TaskKilledEvent = "GUCEF::CORE::CThreadPool::TaskKilledEvent";
const CEvent CThreadPool::TaskStoppedEvent = "GUCEF::CORE::CThreadPool::TaskStoppedEvent";
const CEvent CThreadPool::TaskPausedEvent = "GUCEF::CORE::CThreadPool::TaskPausedEvent";
const CEvent CThreadPool::TaskResumedEvent = "GUCEF::CORE::CThreadPool::TaskResumedEvent";
const CEvent CThreadPool::TaskFinishedEvent = "GUCEF::CORE::CThreadPool::TaskFinishedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CThreadPool::RegisterEvents( void )
{GUCEF_TRACE;

    ThreadKilledEvent.Initialize();
    ThreadStartedEvent.Initialize();
    ThreadPausedEvent.Initialize();
    ThreadResumedEvent.Initialize();
    ThreadFinishedEvent.Initialize();

    TaskQueuedEvent.Initialize();
    QueuedTaskStartedEvent.Initialize();
    TaskStartedEvent.Initialize();
    TaskStartupFailedEvent.Initialize();
    TaskKilledEvent.Initialize();
    TaskStoppedEvent.Initialize();
    TaskPausedEvent.Initialize();
    TaskResumedEvent.Initialize();
    TaskFinishedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CThreadPool::CTaskQueueItem::CTaskQueueItem( CTaskConsumerPtr consumer      ,
                                              CICloneable* taskData          ,
                                              bool assumeOwnershipOfTaskData )
    : CICloneable()
    , m_taskData( taskData )
    , m_taskConsumer( consumer )
    , m_assumeOwnershipOfTaskData( assumeOwnershipOfTaskData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CThreadPool::CTaskQueueItem::CTaskQueueItem( CTaskQueueItem& src )
    : CICloneable()
    , m_taskData( src.m_taskData )
    , m_taskConsumer( src.m_taskConsumer )
    , m_assumeOwnershipOfTaskData( src.m_assumeOwnershipOfTaskData )
{GUCEF_TRACE;

    if ( !m_assumeOwnershipOfTaskData && GUCEF_NULL != m_taskData )
    {
        m_taskData = m_taskData->Clone();
    }
}

/*-------------------------------------------------------------------------*/

CThreadPool::CTaskQueueItem::~CTaskQueueItem()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTaskConsumerPtr&
CThreadPool::CTaskQueueItem::GetTaskConsumer( void )
{GUCEF_TRACE;

    return m_taskConsumer;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CThreadPool::CTaskQueueItem::GetTaskData( void )
{GUCEF_TRACE;

    return m_taskData;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CThreadPool::CTaskQueueItem::Clone( void ) const
{GUCEF_TRACE;

    return new CTaskQueueItem( const_cast< CTaskQueueItem& >( *this ) );
}

/*-------------------------------------------------------------------------*/

CThreadPool::CThreadPool( void )
    : CTSGNotifier()
    , CTSharedPtrCreator< CThreadPool, MT::CMutex >( this )
    , m_consumerFactory()
    , m_desiredNrOfThreads( 0 )
    , m_activeNrOfThreads( 0 )
    , m_taskQueue()
    , m_taskConsumerMap()
    , m_taskDelegators()
    , m_acceptNewWork( true )
    , m_allowAppThreadToWork( false )
{GUCEF_TRACE;

    RegisterEvents();

    CCoreGlobal::Instance()->GetApplication().Subscribe( &AsObserver(), CGUCEFApplication::AppShutdownEvent );
}

/*-------------------------------------------------------------------------*/

CThreadPool::~CThreadPool( void )
{GUCEF_TRACE;

    NotifyObservers( DestructionEvent );

    MT::CObjectScopeLock lock( this );

    // Cleanup tasks
    TTaskDelegatorSet::iterator i = m_taskDelegators.begin();
    while ( i != m_taskDelegators.end() )
    {
        // Kill the task
        TTaskDelegatorBasicPtr delegator = (*i);
        delegator->Deactivate( true, true );
        ++i;
    }
    m_taskDelegators.clear();
}

/*-------------------------------------------------------------------------*/

const CString&
CThreadPool::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CThreadPool";
    return typeName;
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::SetAllowMainApplicationThreadToPickUpQueuedTasks( bool allowAppThreadToWork )
{GUCEF_TRACE;

    m_allowAppThreadToWork = allowAppThreadToWork;
    SetPropagatePulseEvent( m_allowAppThreadToWork );
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetAllowMainApplicationThreadToPickUpQueuedTasks( void ) const
{GUCEF_TRACE;

    return m_allowAppThreadToWork;
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadPool::CarryOutQueuedTasksIfAny( UInt32 maxTasks )
{GUCEF_TRACE;

    for ( UInt32 i=0; i<maxTasks; ++i )
    {
        CTaskConsumerPtr taskConsumer;
        CICloneable* taskData = GUCEF_NULL;
        if ( GetQueuedTask( taskConsumer    ,
                            &taskData       ) )
        {
            CSingleTaskDelegator singleTaskExecutor( CreateSharedPtr(), taskConsumer, taskData );
            singleTaskExecutor.ExecuteTaskFromCallingThread();
        }
        else
        {
            return i;
        }
    }
    return maxTasks;
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::RemoveDelegator( CNotifier* notifier )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    TTaskDelegatorSet::iterator i = m_taskDelegators.begin();
    while ( i != m_taskDelegators.end() )
    {
        if ( static_cast< const CNotifier* >( (*i).GetPointerAlways() ) == notifier )
        {
            TTaskDelegatorBasicPtr delegator = (*i);
            m_taskDelegators.erase( i );
            return;
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::OnPumpedNotify( CNotifier* notifier    ,
                             const CEvent& eventid  ,
                             CICloneable* eventdata )
{GUCEF_TRACE;

    if ( CPulseGenerator::PulseEvent == eventid )
    {
        if ( m_allowAppThreadToWork )
        {
            UInt32 tasksExecuted = CarryOutQueuedTasksIfAny( 1 );
            GUCEF_DEBUG_LOG( LOGLEVEL_BELOW_NORMAL, "ThreadPool: Executed " + ToString( tasksExecuted ) + " tasks using the main application thread" );
        }
        return;
    }
    else
    if ( CGUCEFApplication::AppShutdownEvent == eventid )
    {
        MT::CObjectScopeLock lock( this );

        // Make sure we shut down all tasks
        EnforceDesiredNrOfThreads( 0, true );
    }
    else
    if ( CTaskDelegator::ThreadKilledEvent == eventid )
    {
        NotifyObservers( ThreadKilledEvent, eventdata );
        RemoveDelegator( notifier );
    }
    else
    if ( CTaskDelegator::ThreadStartedEvent == eventid )
    {
        NotifyObservers( ThreadStartedEvent, eventdata );
    }
    else
    if ( CTaskDelegator::ThreadPausedEvent == eventid )
    {
        NotifyObservers( ThreadPausedEvent, eventdata );
    }
    else
    if ( CTaskDelegator::ThreadResumedEvent == eventid )
    {
        NotifyObservers( ThreadResumedEvent, eventdata );
    }
    else
    if ( CTaskDelegator::ThreadFinishedEvent == eventid )
    {
        NotifyObservers( ThreadFinishedEvent, eventdata );
        RemoveDelegator( notifier );
    }
    else
    if ( CTaskConsumer::TaskKilledEvent == eventid )
    {
        NotifyObservers( TaskKilledEvent, eventdata );
    }
    else
    if ( CTaskConsumer::TaskStartedEvent == eventid )
    {
        NotifyObservers( TaskKilledEvent );
    }
    else
    if ( CTaskConsumer::TaskStartupFailedEvent == eventid )
    {
        NotifyObservers( TaskStartupFailedEvent );
    }
    else
    if ( CTaskConsumer::TaskPausedEvent == eventid )
    {
        NotifyObservers( TaskPausedEvent );
    }
    else
    if ( CTaskConsumer::TaskResumedEvent == eventid )
    {
        NotifyObservers( TaskResumedEvent );
    }
    else
    if ( CTaskConsumer::TaskFinishedEvent == eventid )
    {
        NotifyObservers( TaskFinishedEvent );
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::RemoveConsumer( const UInt32 taskID )
{GUCEF_TRACE;

    UInt32 queueItemsRemoved = 0;
    bool taskConsumerUnlinked = false;

    Lock();
    m_taskQueue.DoLock();

    TTaskMailbox::iterator i = m_taskQueue.begin();
    while ( i != m_taskQueue.end() )
    {
        TTaskMailbox::TMailElement& mailElement = (*i);
        CTaskQueueItem* queueItem = static_cast< CTaskQueueItem* >( mailElement.data );

        CTaskConsumerPtr& consumer = queueItem->GetTaskConsumer();
        if ( !consumer.IsNULL() && taskID == consumer->GetTaskId() )
        {
            m_taskQueue.erase( i );
            i = m_taskQueue.begin();
            continue;
        }
        ++i;
    }

    m_taskQueue.DoUnlock();

    TTaskConsumerMap::iterator n = m_taskConsumerMap.find( taskID );
    if ( n != m_taskConsumerMap.end() )
    {
        m_taskConsumerMap.erase( n );
        taskConsumerUnlinked = true;
    }

    Unlock();

    GUCEF_SYSTEM_LOG( LOGLEVEL_BELOW_NORMAL, "ThreadPool: Removing refrences to task ID " + ToString( taskID ) +
        ". This task had " + ToString( queueItemsRemoved ) + " queued work items. Unlinked consumer: " + ToString( taskConsumerUnlinked ) );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::EnforceDesiredNrOfThreads( UInt32 desiredNrOfThreads ,
                                        bool gracefullEnforcement )
{GUCEF_TRACE;

    // This is an internal function but still make sure that the TaskManager is locked
    // when this function is called. There are no locks here for efficiency!!!
    m_desiredNrOfThreads = desiredNrOfThreads;

    // Check if we need to do anything
    if ( (Int32)desiredNrOfThreads > m_activeNrOfThreads )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Increasing the number of threads used for processing tasks to " +
            UInt32ToString( m_desiredNrOfThreads ) + " from " + UInt32ToString( m_activeNrOfThreads ) );

        UInt32 addCount = desiredNrOfThreads - m_activeNrOfThreads;
        for ( UInt32 i=0; i<addCount; ++i )
        {
            // Just spawn a task delegator
            CTaskDelegator* rawDelegator = new CTaskDelegator( CreateSharedPtr() );
            CTaskDelegatorPtr delegator( rawDelegator->CreateSharedPtr() );
            SubscribeTo( delegator.GetPointerAlways() );
            m_taskDelegators.insert( delegator );

            if ( delegator->Activate() )
            {
                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Created thread with ID " + UInt32ToString( delegator->GetThreadID() ) );
            }
            else
            {
                m_taskDelegators.erase( delegator );
                GUCEF_ERROR_LOG( LOGLEVEL_CRITICAL, "ThreadPool: Failed to create thread" );
            }
        }
    }
    else
    if ( (Int32)desiredNrOfThreads < m_activeNrOfThreads )
    {
        UInt32 deactivateCount = m_activeNrOfThreads - desiredNrOfThreads;

        // Check the number of threads that are already asked to deactivate
        UInt32 deactivatingCount = 0;
        TTaskDelegatorSet::iterator i = m_taskDelegators.begin();
        while ( i != m_taskDelegators.end() )
        {
            if ( (*i)->IsActive() )
            {
                if ( (*i)->IsDeactivationRequested() )
                {
                    ++deactivatingCount;
                }
            }
            ++i;
        }

        // Check if we need to do anything
        Int32 leftToBeDeactivated = (Int32) deactivateCount - deactivatingCount;
        if ( leftToBeDeactivated > 0 )
        {
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Decreasing the number of threads used for processing tasks to " +
                                                UInt32ToString( m_desiredNrOfThreads ) + " from " + UInt32ToString( m_activeNrOfThreads ) +
                                                " by asking " + Int32ToString( leftToBeDeactivated ) + " additional threads to deactivate" );

            TTaskDelegatorSet::iterator i = m_taskDelegators.begin();
            while ( i != m_taskDelegators.end() )
            {
                if ( (*i)->IsActive() )
                {
                    // If the thread is not yet asked to deactivate we will do so now up
                    // to the number of thread we wish to deactivate
                    if ( !(*i)->IsDeactivationRequested() )
                    {
                        // Ask thread to deactivate
                        TTaskDelegatorBasicPtr delegator = (*i);
                        delegator->Deactivate( gracefullEnforcement, false );
                    }
                }
                ++i;
            }
        }
    }
    // else: we don't have to do anything
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::QueueTask( const CString& taskType           ,
                        CICloneable* taskData             ,
                        CTaskConsumerPtr* outTaskConsumer ,
                        CObserver* taskObserver           ,
                        bool assumeOwnershipOfTaskData    )
{GUCEF_TRACE;

    {
        MT::CObjectScopeLock lock( this );

        if ( !m_acceptNewWork )
            return false;

        // Create a consumer for the given task type
        CTaskConsumerPtr taskConsumer( m_consumerFactory.Create( taskType ) );
        if ( taskConsumer.IsNULL() )
        {
            // No pool local factory registered capable of handling the task type
            // We now check the global factory for the same
            CTaskConsumerPtr taskConsumer = CCoreGlobal::Instance()->GetTaskManager().CreateTaskConsumer( taskType );
        }
        if ( !taskConsumer.IsNULL() )
        {
            if ( GUCEF_NULL != outTaskConsumer )
            {
                *outTaskConsumer = taskConsumer;
            }
            taskConsumer->SetIsOwnedByThreadPool( true );
            taskConsumer->SetThreadPool( CreateBasicSharedPtr() );

            if ( GUCEF_NULL != taskObserver )
            {
                taskConsumer->Subscribe( taskObserver );
            }
            SubscribeTo( taskConsumer.GetPointerAlways() );

            CTaskQueueItem* queueItem = new CTaskQueueItem( taskConsumer              ,
                                                            taskData                  ,
                                                            assumeOwnershipOfTaskData );

            m_taskQueue.AddMail( taskType, queueItem );

            // We dont want to queue a task that will never be picked up by anyone
            if ( 0 == GetNrOfThreads() && !m_allowAppThreadToWork )
            {
                EnforceDesiredNrOfThreads( 1, true );
            }
        }
    }
    NotifyObservers( TaskQueuedEvent );
    return true;
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::SetNrOfThreadsToLogicalCPUs( const UInt32 factor )
{GUCEF_TRACE;

    SetNrOfThreads( GetLogicalCPUCount() * factor );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::SetNrOfThreads( const UInt32 nrOfThreads )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    EnforceDesiredNrOfThreads( nrOfThreads, true );
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadPool::GetNrOfThreads( void ) const
{GUCEF_TRACE;

    return m_activeNrOfThreads;
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::RegisterTaskConsumerFactory( const CString& taskType       ,
                                          TTaskConsumerFactory* factory )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_consumerFactory.RegisterConcreteFactory( taskType, factory );
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: new consumer factory registerd of type " + taskType );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::UnregisterTaskConsumerFactory( const CString& taskType )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_consumerFactory.UnregisterConcreteFactory( taskType );
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: consumer factory unregisterd of type " + taskType );
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetQueuedTask( CTaskConsumerPtr& taskConsumer ,
                            CICloneable** taskData         )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    CString taskConsumerType;
    CICloneable* queueItemPtr;
    if ( m_taskQueue.GetMail( taskConsumerType ,
                              &queueItemPtr       ) )
    {
        CTaskQueueItem* queueItem = static_cast< CTaskQueueItem* >( queueItemPtr );
        taskConsumer = queueItem->GetTaskConsumer();
        *taskData = queueItem->GetTaskData();

        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::StartTask( CTaskConsumerPtr taskConsumer ,
                        CICloneable* taskData         )
{GUCEF_TRACE;

    if ( taskConsumer.IsNULL() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool: Cannot start task because a nullptr is passed as the taskConsumer" );
        return false;
    }

    MT::CObjectScopeLock lock( this );

    if ( !taskConsumer->GetTaskDelegator().IsNULL() )
    {
        UInt32 threadId = taskConsumer->GetTaskDelegator()->GetThreadID();
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool: Cannot start task because the taskConsumer given already has a delegator (thread: " + ToString( threadId ) + ") assigned" );
        return false;
    }

    if ( !m_acceptNewWork )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Refusing to start task immediatly of type \"" + taskConsumer->GetType() + "\" because the task manager is not accepting new work" );
        return false;
    }

    // Leverage the special friendship to internally set up the relationship / ownership
    // Keep in mind we support both task consumers who's life cycle is managed by the pool but also
    // externalized consumers which have an independent life cycle. As such we need to keep track of which is
    // which so we don't assume life cycle ownership when in fact we have none
    taskConsumer->SetIsOwnedByThreadPool( false );
    taskConsumer->SetThreadPool( CreateBasicSharedPtr() );

    // We proxy event messages
    SubscribeTo( taskConsumer.GetPointerAlways() );

    // Since clearly the task consumer wishes to utlize this particular thread pool we will now
    // make sure the task consumer is registered as a known consumer for this thread pool
    m_taskConsumerMap[ taskConsumer->m_taskId ] = taskConsumer;

    // Just spawn a task delegator, it will auto register as an active task
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Starting task immediatly of type \"" + taskConsumer->GetType() + "\" with ID " + UInt32ToString( taskConsumer->GetTaskId() )  );
    CTaskDelegatorPtr delegator( ( new CSingleTaskDelegator( CreateSharedPtr(), taskConsumer, 0 != taskData ? taskData->Clone() : 0 ) )->CreateSharedPtr() );
    SubscribeTo( delegator.GetPointerAlways() );
    m_taskDelegators.insert( delegator );

    if ( delegator->Activate() )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Successfully activated delegator for task type \"" + taskConsumer->GetType() +
            "\" with task ID " + UInt32ToString( taskConsumer->GetTaskId() ) + " and thread ID " + ToString( delegator->GetThreadID() )  );
        return true;
    }
    else
    {
        m_taskDelegators.erase( delegator );

        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool: Failed to activate delegator for task type \"" + taskConsumer->GetType() +
            "\" with task ID " + UInt32ToString( taskConsumer->GetTaskId() )  + " and thread ID " + ToString( delegator->GetThreadID() )  );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::StartTask( const CString& taskType ,
                        CICloneable* taskData   ,
                        CTaskConsumerPtr* task  )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( !m_acceptNewWork )
        return false;

    // Create a consumer for the given task type
    CTaskConsumerPtr taskConsumer( m_consumerFactory.Create( taskType ) );
    if ( taskConsumer.IsNULL() )
    {
        // No pool local factory registered capable of handling the task type
        // We now check the global factory for the same
        CTaskConsumerPtr taskConsumer = CCoreGlobal::Instance()->GetTaskManager().CreateTaskConsumer( taskType );
    }
    if ( !taskConsumer.IsNULL() )
    {
        // Just spawn a task delegator, it will auto register as an active task
        taskConsumer->SetIsOwnedByThreadPool( true );
        taskConsumer->SetThreadPool( CreateBasicSharedPtr() );
        if ( GUCEF_NULL != task )
        {
            *task = taskConsumer;
        }
        SubscribeTo( taskConsumer.GetPointerAlways() );

        CTaskDelegatorPtr delegator( ( new CSingleTaskDelegator( CreateSharedPtr(), taskConsumer, 0 != taskData ? taskData->Clone() : 0 ) )->CreateSharedPtr() );
        SubscribeTo( delegator.GetPointerAlways() );
        m_taskDelegators.insert( delegator );

        if ( delegator->Activate() )
        {
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Started task immediatly of type \"" + taskType + "\" with ID " +
                                                UInt32ToString( taskConsumer->GetTaskId() ) + " using thread with ID " + UInt32ToString( delegator->GetThreadID() ) );
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool: Failed to task immediatly of type \"" + taskType + "\" with ID " +
                                                UInt32ToString( taskConsumer->GetTaskId() ) + " using thread with ID " + UInt32ToString( delegator->GetThreadID() ) );

            m_taskDelegators.erase( delegator );
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::TaskCleanup( CTaskConsumerPtr taskConsumer ,
                           CICloneable* taskData         )
{GUCEF_TRACE;

    //MT::CObjectScopeLock lock( this );
    delete taskData;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::PauseTask( const UInt32 taskID          ,
                        const bool force             ,
                        const bool okIfTaskIsUnknown )
{GUCEF_TRACE;

    Lock();
    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskID );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            TTaskDelegatorBasicPtr delegator = taskConsumer->GetTaskDelegator();
            if ( !delegator.IsNULL() )
            {
                delegator->Pause( force );
                Unlock();

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Paused task with ID " + UInt32ToString( taskID ) );
                NotifyObserversFromThread( TaskPausedEvent );
                return true;
            }
            else
            {
                // If a consumer does not have a delegator then it hasnt started yet
                Unlock();
                return true;
            }
        }
    }
    else
    {
        Unlock();
        return okIfTaskIsUnknown;
    }
    Unlock();
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::ResumeTask( const UInt32 taskID          ,
                         const bool okIfTaskIsUnknown )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( !m_acceptNewWork )
        return false;

    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskID );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            TTaskDelegatorBasicPtr delegator = taskConsumer->GetTaskDelegator();
            if ( !delegator.IsNULL() )
            {
                delegator->Resume();

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Resumed task with ID " + UInt32ToString( taskID ) );
                NotifyObserversFromThread( TaskResumedEvent );
                return true;
            }
        }
    }
    else
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Cannot resume task with ID " + UInt32ToString( taskID ) + ". It is not known to this pool" );
        return okIfTaskIsUnknown;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::RequestTaskToStop( const UInt32 taskID    ,
                                bool callerShouldWait  ,
                                bool okIfTaskIsUnknown )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskID );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            TTaskDelegatorBasicPtr delegator = taskConsumer->GetTaskDelegator();
            if ( !delegator.IsNULL() )
            {
                lock.EarlyUnlock();
                delegator->Deactivate( false, callerShouldWait );

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Requested task with ID " + UInt32ToString( taskID ) + " to stop" );
            }
            else
            {
                // If a consumer does not have a delegator then it hasnt started yet or it already finished its work
                // either way its already stopped
                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: task with ID " + UInt32ToString( taskID ) + " is known but not active" );
            }
        }
    }
    else
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: task with ID " + UInt32ToString( taskID ) + " is not known to this pool" );
        return okIfTaskIsUnknown;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::RequestTaskToStop( CTaskConsumerPtr taskConsumer ,
                                bool callerShouldWait         )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
        return RequestTaskToStop( taskConsumer->GetTaskId(), callerShouldWait );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::WaitForTaskToFinish( const UInt32 taskId ,
                                   Int32 timeoutInMs   )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskId );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            TTaskDelegatorBasicPtr delegator = taskConsumer->GetTaskDelegator();
            if ( !delegator.IsNULL() )
            {
                GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "ThreadPool:WaitForTaskToFinish: Waiting for task with ID " + UInt32ToString( taskId ) + " to finish" );

                lock.EarlyUnlock();
                delegator->WaitForThreadToFinish( timeoutInMs );
                return true;
            }
            else
            {
                // If a consumer does not have a delegator then it hasnt started yet
                GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "ThreadPool:WaitForTaskToFinish: Task with ID " + UInt32ToString( taskId ) + " is not associated with any thread and as such no wait occured" );
                return true;
            }
        }
    }

    GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "ThreadPool:WaitForTaskToFinish: Task with ID " + UInt32ToString( taskId ) + " is not associated with any thread and as such no wait occured" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::WaitForTaskToFinish( CTaskConsumerPtr taskConsumer ,
                                   Int32 timeoutInMs             )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
        return WaitForTaskToFinish( taskConsumer->GetTaskId(), timeoutInMs );
    return true;
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::RequestAllTasksToStop( bool waitOnStop, bool acceptNewWork )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Requesting all tasks to stop" );

    MT::CObjectScopeLock lock( this );

    m_acceptNewWork = acceptNewWork;

    TTaskConsumerMap::iterator i = m_taskConsumerMap.begin();
    while ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            TTaskDelegatorBasicPtr delegator = taskConsumer->GetTaskDelegator();
            if ( !delegator.IsNULL() )
            {
                UInt32 taskId = taskConsumer->GetTaskId();
                delegator->Deactivate( false, waitOnStop );

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Requested task with ID " + UInt32ToString( taskId ) + " to stop" );
            }
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::RequestAllThreadsToStop( bool waitOnStop, bool acceptNewWork )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Requesting all tasks to stop" );

    MT::CObjectScopeLock lock( this );
    m_acceptNewWork = acceptNewWork;
    EnforceDesiredNrOfThreads( 0, true );

}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::KillTask( const UInt32 taskID )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskID );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            TTaskDelegatorBasicPtr delegator = taskConsumer->GetTaskDelegator();
            if ( !delegator.IsNULL() )
            {
                delegator->Deactivate( true, true );

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Killed task with ID " + UInt32ToString( taskID ) );
                return true;
            }
            else
            {
                // If a consumer does not have a delegator then it hasnt started yet
                GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "ThreadPool: Request to kill task with ID " + UInt32ToString( taskID ) + ". It does not have a thread assigned" );
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
