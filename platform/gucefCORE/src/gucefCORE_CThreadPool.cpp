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

    return GUCEF_NEW CTaskQueueItem( const_cast< CTaskQueueItem& >( *this ) );
}

/*-------------------------------------------------------------------------*/

CThreadPool::CThreadPool( PulseGeneratorPtr threadPoolPulseContext ,
                          const CString& poolName                  )
    : CTSGNotifier( threadPoolPulseContext, true, false )
    , CTSharedPtrCreator< CThreadPool, MT::CMutex >( this )
    , m_poolName( poolName )
    , m_consumerFactory()
    , m_desiredNrOfThreads( 0 )
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
    
    m_acceptNewWork = false;
    if ( !NotifyObservers( DestructionEvent ) )
        return;

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
    m_taskQueue.SetAcceptsNewMail( false );
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
    UInt32 activeNrOfThreads = (UInt32) m_taskDelegators.size();
    if ( (Int32)desiredNrOfThreads > activeNrOfThreads )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Increasing the number of threads used for processing tasks to " +
            UInt32ToString( m_desiredNrOfThreads ) + " from " + ToString( activeNrOfThreads ) );

        size_t addCount = desiredNrOfThreads - activeNrOfThreads;
        for ( UInt32 i=0; i<addCount; ++i )
        {
            // Just spawn a task delegator
            CTaskDelegator* rawDelegator = GUCEF_NEW CTaskDelegator( CreateSharedPtr() );
            CTaskDelegatorPtr delegator( rawDelegator->CreateSharedPtr() );
            SubscribeTo( delegator.GetPointerAlways() );
            m_taskDelegators.insert( delegator );

            if ( delegator->Activate() )
            {
                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Created thread with ID " + ToString( delegator->GetThreadID() ) );
            }
            else
            {
                m_taskDelegators.erase( delegator );
                GUCEF_ERROR_LOG( LOGLEVEL_CRITICAL, "ThreadPool: Failed to create thread" );
            }
        }
    }
    else
    if ( (Int32)desiredNrOfThreads < activeNrOfThreads )
    {
        UInt32 deactivateCount = activeNrOfThreads - desiredNrOfThreads;

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
                                                ToString( m_desiredNrOfThreads ) + " from " + ToString( activeNrOfThreads ) +
                                                " by asking " + ToString( leftToBeDeactivated ) + " additional threads to deactivate" );

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
            taskConsumer = CCoreGlobal::Instance()->GetTaskManager().CreateTaskConsumer( taskType );
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
            SubscribeToTaskConsumerEvents( taskConsumer );

            CTaskQueueItem* queueItem = GUCEF_NEW CTaskQueueItem( taskConsumer              ,
                                                                  taskData                  ,
                                                                  assumeOwnershipOfTaskData );

            m_taskQueue.AddMail( taskType, queueItem );

            // We dont want to queue a task that will never be picked up by anyone
            if ( 0 == GetActiveNrOfThreads() && !m_allowAppThreadToWork )
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
CThreadPool::SubscribeToTaskConsumerEvents( CTaskConsumerPtr& taskConsumer )
{GUCEF_TRACE;

    CORE::CNotifier* notifier = taskConsumer.GetPointerAlways();
    SubscribeTo( notifier, CTaskConsumer::TaskKilledEvent );
    SubscribeTo( notifier, CTaskConsumer::TaskStartedEvent );
    SubscribeTo( notifier, CTaskConsumer::TaskStartupFailedEvent );
    SubscribeTo( notifier, CTaskConsumer::TaskPausedEvent );
    SubscribeTo( notifier, CTaskConsumer::TaskResumedEvent );
    SubscribeTo( notifier, CTaskConsumer::TaskFinishedEvent );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::SetNrOfThreadsToLogicalCPUs( const UInt32 factor )
{GUCEF_TRACE;

    SetDesiredNrOfThreads( GetLogicalCPUCount() * factor );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::SetDesiredNrOfThreads( const UInt32 nrOfThreads )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_desiredNrOfThreads = nrOfThreads;
    EnforceDesiredNrOfThreads( nrOfThreads, true );
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadPool::GetDesiredNrOfThreads( void ) const
{GUCEF_TRACE;

    return m_desiredNrOfThreads;
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadPool::GetActiveNrOfThreads( void ) const
{GUCEF_TRACE;

    return (UInt32) m_taskDelegators.size();
}

/*-------------------------------------------------------------------------*/

UInt32 
CThreadPool::GetNrOfQueuedTasks( void ) const
{GUCEF_TRACE;

    return m_taskQueue.AmountOfMail();
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
CThreadPool::SetupTask( CTaskConsumerPtr taskConsumer  ,
                        CICloneable* taskData          ,
                        bool assumeOwnershipOfTaskData )
{GUCEF_TRACE;

    if ( taskConsumer.IsNULL() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool: Cannot setup task because a nullptr is passed as the taskConsumer" );
        return false;
    }

    MT::CObjectScopeLock lock( this );

    if ( !m_acceptNewWork )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Refusing to setup task immediatly of type \"" + taskConsumer->GetType() + "\" because the task manager is not accepting new work" );
        return false;
    }

    if ( !taskConsumer->GetTaskDelegator().IsNULL() )
    {
        UInt32 threadId = taskConsumer->GetTaskDelegator()->GetThreadID();
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool: Cannot setup task because the taskConsumer given already has a delegator (thread: " + ToString( threadId ) + ") assigned" );
        return false;
    }

    // Leverage the special friendship to internally set up the relationship / ownership
    // Keep in mind we support both task consumers who's life cycle is managed by the pool but also
    // externalized consumers which have an independent life cycle. As such we need to keep track of which is
    // which so we don't assume life cycle ownership when in fact we have none
    taskConsumer->SetIsOwnedByThreadPool( false );    
    taskConsumer->SetThreadPool( CreateBasicSharedPtr() );

    // IMPORTANT: We set the flag to signal to the thread on startup that we wish to perform a phased setup
    //            This causes the consumer to be linked but no OnTaskStart or the like will not yet be invoked on the consumer 
    taskConsumer->SetIsInPhasedSetup( true );

    // We listen for task consumer generic events
    SubscribeToTaskConsumerEvents( taskConsumer );

    // Since clearly the task consumer wishes to utlize this particular thread pool we will now
    // make sure the task consumer is registered as a known consumer for this thread pool
    m_taskConsumerMap[ taskConsumer->m_taskId ] = taskConsumer;

    // Check to see if ownership of the task data (if any) is being transfered to us or
    // do we need a private copy
    CICloneable* taskDataToUse = taskData;
    if ( !assumeOwnershipOfTaskData && GUCEF_NULL != taskData )
    {
        taskDataToUse = taskData->Clone();
    }
    
    // Now spawn a task delegator
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Setting up task of type \"" + taskConsumer->GetType() + "\" with ID " + UInt32ToString( taskConsumer->GetTaskId() )  );
    CTaskDelegatorPtr delegator( ( GUCEF_NEW CSingleTaskDelegator( CreateSharedPtr(), taskConsumer, taskDataToUse ) )->CreateSharedPtr() );
    SubscribeTo( delegator.GetPointerAlways() );
    m_taskDelegators.insert( delegator );

    if ( delegator->Activate() )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Successfully activated delegator for task type \"" + taskConsumer->GetType() +
            "\" with task ID " + UInt32ToString( taskConsumer->GetTaskId() ) + " and thread ID " + ToString( delegator->GetThreadID() )  );
        
        TThreadStartedEventData threadIdData( delegator->GetThreadID() );
        NotifyObserversFromThread( ThreadStartedEvent, &threadIdData );

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
CThreadPool::StartTask( CTaskConsumerPtr taskConsumer  ,
                        CICloneable* taskData          ,
                        bool assumeOwnershipOfTaskData )
{GUCEF_TRACE;

    if ( taskConsumer.IsNULL() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool: Cannot start task because a nullptr is passed as the taskConsumer" );
        return false;
    }

    // Check to see if setup has been performed yet
    // If it was not explictly invoked yet we will just incorporate the setup step here
    if ( !taskConsumer->GetIsInPhasedSetup() )
    {
        if ( !SetupTask( taskConsumer, taskData, assumeOwnershipOfTaskData ) )
        {
            return false;            
        }
    }

    // IMPORTANT: We remove the flag to signal to the delegator it should commence operations
    taskConsumer->SetIsInPhasedSetup( false );
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Task of task type \"" + taskConsumer->GetType() +
            "\" with task ID " + ToString( taskConsumer->GetTaskId() ) + " instructed to commence work" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::StartTask( const CString& taskType            ,
                        CICloneable* taskData              ,
                        CTaskConsumerPtr* outTaskConsumer  ,
                        bool assumeOwnershipOfTaskData     )
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
        taskConsumer = CCoreGlobal::Instance()->GetTaskManager().CreateTaskConsumer( taskType );
    }
    if ( !taskConsumer.IsNULL() )
    {
        if ( SetupTask( taskConsumer, taskData, assumeOwnershipOfTaskData ) )
        {
            // IMPORTANT: We remove the flag to signal to the delegator it should commence operations
            taskConsumer->SetIsInPhasedSetup( false );            
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool: Task of task type \"" + taskType +
                    "\" with task ID " + ToString( taskConsumer->GetTaskId() ) + " instructed to commence work" );

            if ( GUCEF_NULL != outTaskConsumer )
            {
                *outTaskConsumer = taskConsumer;
            }
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "ThreadPool: Failed to start task of type \"" + taskType +
                "\" with task ID " + UInt32ToString( taskConsumer->GetTaskId() )  + " because task setup failed" );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( LOGLEVEL_IMPORTANT, "ThreadPool: Failed to start task of type \"" + taskType +
            " because no such task type is known" );
        return false;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::StartTask( const CString& taskType           ,
                        const CDataNode& taskData         ,
                        CTaskConsumerPtr* outTaskConsumer ,
                        bool assumeOwnershipOfTaskData    )
{GUCEF_TRACE;

    TAbstractTaskDataFactory::TProductPtr taskDataPtr = m_taskDataFactory.Create( taskType );
    if ( taskDataPtr.IsNULL() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool:StartTask: Task of type \"" + taskType + "\" cannot be created because no task data factory exists for the given type" );
        return false;
    }

    CDataNodeSerializableSettings defaultSerializerSettings;
    if ( !taskDataPtr->Deserialize( taskData, defaultSerializerSettings ) )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool:StartTask: Task of type \"" + taskType + "\" cannot be created because deserialization of the task data failed" );
        return false;
    }

    // Now start the task with the data we constructed    
    return StartTask( taskType, taskDataPtr.GetPointerAlways(), outTaskConsumer, assumeOwnershipOfTaskData );
}

/*-------------------------------------------------------------------------*/

bool 
CThreadPool::TaskOfTypeExists( const CString& taskType  , 
                               UInt32* taskIdIfExists   , 
                               UInt32* threadIdIfExists ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    TTaskConsumerMap::const_iterator i = m_taskConsumerMap.begin();
    while ( i != m_taskConsumerMap.end() )
    {
        const CTaskConsumerPtr& taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() && taskType == taskConsumer->GetType() )
        {
            if ( GUCEF_NULL != taskIdIfExists )
                *taskIdIfExists = taskConsumer->GetTaskId();    
            if ( GUCEF_NULL != threadIdIfExists )
                *threadIdIfExists = taskConsumer->GetDelegatorThreadId(); 

            return true;
        }
        ++i;
    }

    if ( GUCEF_NULL != taskIdIfExists )
        *taskIdIfExists = 0;    
    if ( GUCEF_NULL != threadIdIfExists )
        *threadIdIfExists = 0; 

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::StartTaskIfNoneExists( const CString& taskType            ,
                                    CICloneable* taskData              ,
                                    CTaskConsumerPtr* outTaskConsumer  ,
                                    bool assumeOwnershipOfTaskData     )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    // Check if a task of the type given already exists while we have the lock
    UInt32 taskIdIfExists = 0;   
    UInt32 threadIdIfExists = 0;
    if ( TaskOfTypeExists( taskType, &taskIdIfExists, &threadIdIfExists ) )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool:StartTaskIfNoneExists: Task of type \"" + taskType + "\" with ID " +
                                            UInt32ToString( taskIdIfExists ) + " already exists and its using thread with ID " + UInt32ToString( threadIdIfExists ) );
        return true;
    }


    // No such task exists, just create a new one    
    return StartTask( taskType, taskData, outTaskConsumer, assumeOwnershipOfTaskData );
}

/*-------------------------------------------------------------------------*/

bool 
CThreadPool::StartTaskIfNoneExists( const CString& taskType           ,
                                    const CDataNode& taskData         ,
                                    CTaskConsumerPtr* outTaskConsumer )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    // Check if a task of the type given already exists while we have the lock
    UInt32 taskIdIfExists = 0;   
    UInt32 threadIdIfExists = 0;
    if ( TaskOfTypeExists( taskType, &taskIdIfExists, &threadIdIfExists ) )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "ThreadPool:StartTaskIfNoneExists: Task of type \"" + taskType + "\" with ID " +
                                            UInt32ToString( taskIdIfExists ) + " already exists and its using thread with ID " + UInt32ToString( threadIdIfExists ) );
        return true;
    }

    // No such task exists, just create a new one
    // First we must construct the task data
    
    TAbstractTaskDataFactory::TProductPtr taskDataPtr = m_taskDataFactory.Create( taskType );
    if ( taskDataPtr.IsNULL() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool:StartTaskIfNoneExists: Task of type \"" + taskType + "\" cannot be created because no task data factory exists for the given type" );
        return false;
    }

    CDataNodeSerializableSettings defaultSerializerSettings;
    if ( !taskDataPtr->Deserialize( taskData, defaultSerializerSettings ) )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "ThreadPool:StartTaskIfNoneExists: Task of type \"" + taskType + "\" cannot be created because deserialization of the task data failed" );
        return false;
    }

    // Now start the task with the data we constructed    
    return StartTask( taskType, taskDataPtr.GetPointerAlways(), outTaskConsumer );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::TaskCleanup( CTaskConsumerPtr taskConsumer ,
                           CICloneable* taskData         )
{GUCEF_TRACE;

    //MT::CObjectScopeLock lock( this );
    GUCEF_DELETE taskData;
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::OnTaskStarted( CTaskConsumerPtr taskConsumer )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
    {
        TTaskStartedEventData eData( taskConsumer->GetTaskId() ); 
        NotifyObserversFromThread( TaskStartedEvent, &eData ); 
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::OnTaskStartupFailed( CTaskConsumerPtr taskConsumer )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
    {
        TTaskStartupFailedEventData eData( taskConsumer->GetTaskId() ); 
        NotifyObserversFromThread( TaskStartupFailedEvent, &eData ); 
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::OnTaskKilled( CTaskConsumerPtr taskConsumer )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
    {
        TTaskKilledEventData eData( taskConsumer->GetTaskId() ); 
        NotifyObserversFromThread( TaskKilledEvent, &eData ); 
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::OnTaskStopped( CTaskConsumerPtr taskConsumer )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
    {
        TTaskStoppedEventData eData( taskConsumer->GetTaskId() ); 
        NotifyObserversFromThread( TaskStoppedEvent, &eData ); 
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::OnTaskPaused( CTaskConsumerPtr taskConsumer )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
    {
        TTaskPausedEventData eData( taskConsumer->GetTaskId() ); 
        NotifyObserversFromThread( TaskPausedEvent, &eData ); 
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::OnTaskResumed( CTaskConsumerPtr taskConsumer )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
    {
        TTaskResumedEventData eData( taskConsumer->GetTaskId() ); 
        NotifyObserversFromThread( TaskResumedEvent, &eData ); 
    }
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::OnTaskFinished( CTaskConsumerPtr taskConsumer )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
    {
        TTaskFinishedEventData eData( taskConsumer->GetTaskId() ); 
        NotifyObserversFromThread( TaskFinishedEvent, &eData ); 
    }
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

                UInt32 workerThreadId = delegator->GetThreadID();
                UInt32 callerThreadId = MT::GetCurrentTaskID();
                
                lock.EarlyUnlock();

                if ( workerThreadId != callerThreadId )
                {
                    delegator->WaitForThreadToFinish( timeoutInMs );
                    return true;
                }
                else
                {
                    GUCEF_ERROR_LOG( LOGLEVEL_CRITICAL, "ThreadPool:WaitForTaskToFinish: Task with ID " + UInt32ToString( taskId ) + 
                        " is requested to be waited on from thread " + UInt32ToString( callerThreadId ) + 
                        " which is also the thread running the task. This is not allowed to prevent deadlocks" );                    
                    return false;
                }
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

/*-------------------------------------------------------------------------*/

void
CThreadPool::GetAllRegisteredTaskConsumerFactoryTypes( CORE::CString::StringSet& taskTypes ) const
{GUCEF_TRACE;

    m_consumerFactory.ObtainKeySet( taskTypes );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::GetAllRegisteredTaskDataFactoryTypes( CORE::CString::StringSet& taskTypes ) const
{GUCEF_TRACE;

    m_taskDataFactory.ObtainKeySet( taskTypes );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::RegisterTaskDataFactory( const CString& taskType   ,
                                      TTaskDataFactory* factory )
{GUCEF_TRACE;

    m_taskDataFactory.RegisterConcreteFactory( taskType, factory );
}

/*-------------------------------------------------------------------------*/

void
CThreadPool::UnregisterTaskDataFactory( const CString& taskType )
{GUCEF_TRACE;

    m_taskDataFactory.UnregisterConcreteFactory( taskType );
}

/*-------------------------------------------------------------------------*/

bool 
CThreadPool::IsCustomTaskDataForTaskTypeSerializable( const CString& taskType ) const
{GUCEF_TRACE;

    return m_taskDataFactory.IsConstructible( taskType );
}

/*-------------------------------------------------------------------------*/

CIDataNodeSerializableTaskDataBasicPtr
CThreadPool::CreateCustomTaskDataForTaskTypeIfAvailable( const CString& taskType ) const
{GUCEF_TRACE;

    return m_taskDataFactory.Create( taskType );
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetTaskIdForThreadId( const UInt32 threadId ,
                                   UInt32& taskId        ) const
{GUCEF_TRACE;

    taskId = 0;
    MT::CObjectScopeLock lock( this );

    TTaskDelegatorSet::const_iterator i = m_taskDelegators.begin();
    while ( i != m_taskDelegators.end() )
    {
        const TTaskDelegatorBasicPtr& delegator = (*i);
        if ( !delegator.IsNULL() )
        {
            CTaskConsumerPtr taskConsumer = delegator->GetTaskConsumer();
            if ( !taskConsumer.IsNULL() )
            {
                taskId = taskConsumer->GetTaskId();
                return true;
            }
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetThreadIdForTaskId( const UInt32 taskId ,
                                   UInt32& threadId    ) const
{GUCEF_TRACE;

    threadId = 0;
    MT::CObjectScopeLock lock( this );

    TTaskConsumerMap::const_iterator i = m_taskConsumerMap.find( taskId );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            threadId = taskConsumer->GetDelegatorThreadId();
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CThreadPool::GetInfo( CThreadPoolInfo& info ) const
{GUCEF_TRACE;

    info.Clear();
    
    // Obtain an overall lock to get a coherent snapshot of threadpool info
    MT::CObjectScopeLock lock( this );

    info.SetActiveNrOfThreads( (UInt32) m_taskDelegators.size() );
    info.SetDesiredNrOfThreads( m_desiredNrOfThreads );
    info.SetAllowMainApplicationThreadToPickUpQueuedTasks( m_allowAppThreadToWork );
    m_consumerFactory.ObtainKeySet( info.GetTaskConsumerFactoryTypes() );
    m_taskDataFactory.ObtainKeySet( info.GetTaskDataFactoryTypes() );
    info.SetQueuedTaskCount( m_taskQueue.AmountOfMail() );
    info.SetThreadPoolName( m_poolName );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetTaskInfo( UInt32 taskId                                             , 
                          CTaskInfo& info                                           , 
                          bool obtainTaskDataCopyIfPossible                         ,
                          CDataNodeSerializableSettings* taskDataSerializerSettings ) const
{GUCEF_TRACE;

    info.Clear();
    MT::CObjectScopeLock lock( this );

    TTaskConsumerMap::const_iterator i = m_taskConsumerMap.find( taskId );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            lock.EarlyUnlock();
            
            info.SetTaskId( taskId );
            info.SetThreadId( taskConsumer->GetDelegatorThreadId() );
            info.SetTaskTypeName( taskConsumer->GetType() );
            info.SetCustomTaskDataIsSerializable( IsCustomTaskDataForTaskTypeSerializable( info.GetTaskTypeName() ) );
            info.SetHasTaskData( taskConsumer->HasTaskData() );
            if ( obtainTaskDataCopyIfPossible && info.GetHasTaskData() && info.GetCustomTaskDataIsSerializable() )
            {
                if ( GUCEF_NULL != taskDataSerializerSettings )
                {
                    taskConsumer->GetSerializedTaskDataCopy( info.GetCustomTaskData(), *taskDataSerializerSettings );
                }
                else
                {
                    CDataNodeSerializableSettings defaultSerializableSettings;
                    taskConsumer->GetSerializedTaskDataCopy( info.GetCustomTaskData(), defaultSerializableSettings );
                }
            }
            return true;
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetAllTaskInfo( TTaskInfoMap& info                                        ,
                             bool obtainTaskDataCopyIfPossible                         ,
                             CDataNodeSerializableSettings* taskDataSerializerSettings ) const
{GUCEF_TRACE;

    info.clear();
    MT::CObjectScopeLock lock( this );

    bool totalSuccess = true;
    TTaskConsumerMap::const_iterator i = m_taskConsumerMap.begin();
    while ( i != m_taskConsumerMap.end() )
    {
        UInt32 taskId = (*i).first;
        CTaskInfo& taskInfo = info[ taskId ];

        totalSuccess = GetTaskInfo( taskId                       , 
                                    taskInfo                     , 
                                    obtainTaskDataCopyIfPossible , 
                                    taskDataSerializerSettings   ) && totalSuccess;
        ++i;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetThreadInfo( UInt32 threadId, CThreadInfo& info ) const
{GUCEF_TRACE;

    info.Clear();
    MT::CObjectScopeLock lock( this );

    TTaskDelegatorSet::const_iterator i = m_taskDelegators.begin();
    while ( i != m_taskDelegators.end() )
    {
        if ( threadId == (*i)->GetThreadID() )
            return (*i)->GetThreadInfo( info );
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetAllThreadInfo( TThreadInfoMap& info ) const
{GUCEF_TRACE;

    info.clear();
    MT::CObjectScopeLock lock( this );

    bool totalSuccess = true;
    TTaskDelegatorSet::const_iterator i = m_taskDelegators.begin();
    while ( i != m_taskDelegators.end() )
    {
        const TTaskDelegatorBasicPtr& delegator = (*i);
        if ( !delegator.IsNULL() )
        {
            UInt32 threadId = (*i)->GetThreadID();
            CThreadInfo& threadInfo = info[ threadId ];
            totalSuccess = delegator->GetThreadInfo( threadInfo ) && totalSuccess;
        }        
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPool::GetSerializedTaskDataCopy( UInt32 taskId                                     ,
                                        CDataNode& domNode                                ,
                                        CDataNodeSerializableSettings& serializerSettings ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    TTaskConsumerMap::const_iterator i = m_taskConsumerMap.find( taskId );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            return taskConsumer->GetSerializedTaskDataCopy( domNode, serializerSettings );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const CString& 
CThreadPool::GetThreadPoolName( void ) const
{GUCEF_TRACE;
    
    // no lock since this is only ever set in the constructor with no way to change it after the fact
    return m_poolName;  
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
