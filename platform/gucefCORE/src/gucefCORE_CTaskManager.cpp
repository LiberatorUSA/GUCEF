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

#include "gucefCORE_CTaskManager.h"

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

const CEvent CTaskManager::ThreadKilledEvent = "GUCEF::CORE::CTaskManager::ThreadKilledEvent";
const CEvent CTaskManager::ThreadStartedEvent = "GUCEF::CORE::CTaskManager::ThreadStartedEvent";
const CEvent CTaskManager::ThreadPausedEvent = "GUCEF::CORE::CTaskManager::ThreadPausedEvent";
const CEvent CTaskManager::ThreadResumedEvent = "GUCEF::CORE::CTaskManager::ThreadResumedEvent";
const CEvent CTaskManager::ThreadFinishedEvent = "GUCEF::CORE::CTaskManager::ThreadFinishedEvent";

const CEvent CTaskManager::TaskQueuedEvent = "GUCEF::CORE::CTaskManager::TaskQueuedEvent";
const CEvent CTaskManager::QueuedTaskStartedEvent = "GUCEF::CORE::CTaskManager::QueuedTaskStartedEvent";
const CEvent CTaskManager::TaskStartedEvent = "GUCEF::CORE::CTaskManager::TaskStartedEvent";
const CEvent CTaskManager::TaskStartupFailedEvent = "GUCEF::CORE::CTaskManager::TaskStartupFailedEvent";
const CEvent CTaskManager::TaskKilledEvent = "GUCEF::CORE::CTaskManager::TaskKilledEvent";
const CEvent CTaskManager::TaskStoppedEvent = "GUCEF::CORE::CTaskManager::TaskStoppedEvent";
const CEvent CTaskManager::TaskPausedEvent = "GUCEF::CORE::CTaskManager::TaskPausedEvent";
const CEvent CTaskManager::TaskResumedEvent = "GUCEF::CORE::CTaskManager::TaskResumedEvent";
const CEvent CTaskManager::TaskFinishedEvent = "GUCEF::CORE::CTaskManager::TaskFinishedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CTaskManager::RegisterEvents( void )
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

CTaskManager::CTaskQueueItem::CTaskQueueItem( CTaskConsumerPtr consumer      ,
                                              CICloneable* taskData          ,
                                              bool assumeOwnershipOfTaskData )
    : CICloneable()              
    , m_taskData( taskData )     
    , m_taskConsumer( consumer ) 
    , m_assumeOwnershipOfTaskData( assumeOwnershipOfTaskData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTaskManager::CTaskQueueItem::CTaskQueueItem( CTaskQueueItem& src )
    : CICloneable()                         
    ,  m_taskData( src.m_taskData )
    ,  m_taskConsumer( src.m_taskConsumer )
    , m_assumeOwnershipOfTaskData( src.m_assumeOwnershipOfTaskData )
{GUCEF_TRACE;

    if ( !m_assumeOwnershipOfTaskData && GUCEF_NULL != m_taskData )
    {
        m_taskData = m_taskData->Clone();    
    }
}

/*-------------------------------------------------------------------------*/

CTaskManager::CTaskQueueItem::~CTaskQueueItem()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTaskConsumerPtr
CTaskManager::CTaskQueueItem::GetTaskConsumer( void )
{GUCEF_TRACE;

    return m_taskConsumer;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CTaskManager::CTaskQueueItem::GetTaskData( void )
{GUCEF_TRACE;

    return m_taskData;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CTaskManager::CTaskQueueItem::Clone( void ) const
{GUCEF_TRACE;

    return new CTaskQueueItem( const_cast< CTaskQueueItem& >( *this ) );
}

/*-------------------------------------------------------------------------*/

CTaskManager::CTaskManager( void )
    : CTSGNotifier()      
    , m_consumerFactory()       
    , m_desiredNrOfThreads( 0 ) 
    , m_activeNrOfThreads( 0 )  
    , m_taskQueue()             
    , m_taskIdGenerator()       
    , m_taskConsumerMap()       
    , m_taskDelegators()
    , m_acceptNewWork( true )
{GUCEF_TRACE;

    RegisterEvents();

    CCoreGlobal::Instance()->GetApplication().Subscribe( &AsObserver(), CGUCEFApplication::AppShutdownEvent );
}

/*-------------------------------------------------------------------------*/

CTaskManager::~CTaskManager( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    // Cleanup tasks
    TTaskDelegatorSet::iterator i = m_taskDelegators.begin();
    while ( i != m_taskDelegators.end() )
    {
        // Kill the task
        (*i)->Deactivate( true, true );
        ++i;
    }
    m_taskDelegators.clear();
}

/*-------------------------------------------------------------------------*/

const CString&
CTaskManager::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CTaskManager";
    return typeName;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::OnPumpedNotify( CNotifier* notifier    ,
                              const CEvent& eventid  ,
                              CICloneable* eventdata )
{GUCEF_TRACE;

    if ( CGUCEFApplication::AppShutdownEvent == eventid )
    {
        MT::CObjectScopeLock lock( this );

        // Make sure we shut down all tasks
        EnforceDesiredNrOfThreads( 0, true );
    }
    else
    if ( CTaskDelegator::ThreadKilledEvent == eventid )
    {
        NotifyObservers( ThreadKilledEvent );
    }
    else
    if ( CTaskDelegator::ThreadStartedEvent == eventid )
    {
        NotifyObservers( ThreadStartedEvent );
    }
    else
    if ( CTaskDelegator::ThreadPausedEvent == eventid )
    {
        NotifyObservers( ThreadPausedEvent );
    }
    else
    if ( CTaskDelegator::ThreadResumedEvent == eventid )
    {
        NotifyObservers( ThreadResumedEvent );
    }
    else
    if ( CTaskDelegator::ThreadFinishedEvent == eventid )
    {
        NotifyObservers( ThreadFinishedEvent );
    }
    else
    if ( CTaskConsumer::TaskKilledEvent == eventid )
    {
        NotifyObservers( TaskKilledEvent );
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
CTaskManager::RemoveConsumerFromQueue( CTaskConsumer* consumer )
{GUCEF_TRACE;

    Lock();
    m_taskQueue.DoLock();

    TTaskMailbox::iterator i = m_taskQueue.begin();
    while ( i != m_taskQueue.end() )
    {
        TTaskMailbox::TMailElement& mailElement = (*i);
        CTaskQueueItem* queueItem = static_cast< CTaskQueueItem* >( mailElement.data );

        if ( queueItem->GetTaskConsumer() == consumer )
        {
            m_taskQueue.erase( i );
            i = m_taskQueue.begin();
            continue;
        }
        ++i;
    }

    m_taskQueue.DoUnlock();
    Unlock();
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::EnforceDesiredNrOfThreads( UInt32 desiredNrOfThreads ,
                                         bool gracefullEnforcement )
{GUCEF_TRACE;

    // This is an internal function but still make sure that the TaskManager is locked
    // when this function is called. There are no locks here for efficiency!!!
    m_desiredNrOfThreads = desiredNrOfThreads;

    // Check if we need to do anything
    if ( (Int32)desiredNrOfThreads > m_activeNrOfThreads )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Increasing the number of threads used for processing tasks to " + UInt32ToString( m_desiredNrOfThreads ) + " from " + UInt32ToString( m_activeNrOfThreads ) );

        UInt32 addCount = desiredNrOfThreads - m_activeNrOfThreads;
        for ( UInt32 i=0; i<addCount; ++i )
        {
            // Just spawn a task delegator, it will auto register as an active task
            CTaskDelegator* delegator = new CTaskDelegator();
            delegator->Activate();
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
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Decreasing the number of threads used for processing tasks to " +
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
                        (*i)->Deactivate( gracefullEnforcement, false );
                    }
                }
                ++i;
            }
        }
    }
    // else: we don't have to do anything

    //if ( m_nonactiveTasks.size() > 0 )
    //{
    //    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Cleaning up non-active tasks " );

    //    // Cleanup tasks that have been discarded
    //    TTaskDelegatorSet::iterator i = m_nonactiveTasks.begin();
    //    while ( i != m_nonactiveTasks.end() )
    //    {
    //        delete (*i).second;
    //        ++i;
    //    }
    //    m_nonactiveTasks.clear();
    //}
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::QueueTask( const CString& taskType           ,
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
        CTaskConsumer* taskConsumer = m_consumerFactory.Create( taskType );
        if ( GUCEF_NULL != taskConsumer )
        {
            if ( GUCEF_NULL != outTaskConsumer )
            {
                *outTaskConsumer = taskConsumer;
            }
            taskConsumer->SetIsOwnedByTaskManager( true );

            if ( GUCEF_NULL != taskObserver )
            {
                taskConsumer->Subscribe( taskObserver );
            }

            CTaskQueueItem* queueItem = new CTaskQueueItem( taskConsumer              ,
                                                            taskData                  ,
                                                            assumeOwnershipOfTaskData );

            m_taskQueue.AddMail( taskType, queueItem );

            // We dont want to queue a task that will never be picked up by anyone
            if ( 0 == GetNrOfThreads() )
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
CTaskManager::SetNrOfThreadsToLogicalCPUs( const UInt32 factor )
{GUCEF_TRACE;

    SetNrOfThreads( GetLogicalCPUCount() * factor );
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::SetNrOfThreads( const UInt32 nrOfThreads )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    EnforceDesiredNrOfThreads( nrOfThreads, true );
}

/*-------------------------------------------------------------------------*/

UInt32
CTaskManager::GetNrOfThreads( void ) const
{GUCEF_TRACE;

    return m_activeNrOfThreads;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RegisterTaskConsumerFactory( const CString& taskType       ,
                                           TTaskConsumerFactory* factory )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_consumerFactory.RegisterConcreteFactory( taskType, factory );
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: new consumer factory registerd of type " + taskType );
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::UnregisterTaskConsumerFactory( const CString& taskType )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_consumerFactory.UnregisterConcreteFactory( taskType );
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: consumer factory unregisterd of type " + taskType );
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetQueuedTask( CTaskConsumerPtr& taskConsumer ,
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
CTaskManager::StartTask( CTaskConsumerPtr taskConsumer ,
                         CICloneable* taskData         )
{GUCEF_TRACE;

    if ( taskConsumer.IsNULL() )
    {
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "TaskManager: Cannot start task because a nullptr is passed as the taskConsumer" );        
        return false;
    }    
    
    MT::CObjectScopeLock lock( this );

    if ( GUCEF_NULL != taskConsumer->GetTaskDelegator() )
    {
        UInt32 threadId = taskConsumer->GetTaskDelegator()->GetThreadID();
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "TaskManager: Cannot start task because the taskConsumer given already has a delegator (thread: " + ToString( threadId ) + ") assigned" );
        return false;
    }

    if ( !m_acceptNewWork )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Refusing to start task immediatly of type \"" + taskConsumer->GetType() + "\" because the task manager is not accepting new work" );
        return false;
    }
        
    // Just spawn a task delegator, it will auto register as an active task
    taskConsumer->SetIsOwnedByTaskManager( false );
    CTaskDelegator* delegator = new CSingleTaskDelegator( taskConsumer, 0 != taskData ? taskData->Clone() : 0 );

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Starting task immediatly of type \"" + taskConsumer->GetType() + "\" with ID " + UInt32ToString( taskConsumer->GetTaskId() )  );

    delegator->Activate();

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::StartTask( const CString& taskType ,
                         CICloneable* taskData   ,
                         CTaskConsumerPtr* task  )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( !m_acceptNewWork )
        return false;

    // Create a consumer for the given task type
    CTaskConsumerPtr taskConsumer( m_consumerFactory.Create( taskType ) );
    if ( !taskConsumer.IsNULL() )
    {
        // Just spawn a task delegator, it will auto register as an active task
        taskConsumer->SetIsOwnedByTaskManager( true );
        if ( GUCEF_NULL != task )
        {
            *task = taskConsumer;
        }
        CTaskDelegator* delegator = new CSingleTaskDelegator( taskConsumer, 0 != taskData ? taskData->Clone() : 0 );

        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Started task immediatly of type \"" + taskType + "\" with ID " +
                                            UInt32ToString( taskConsumer->GetTaskId() ) + " using thread with ID " + UInt32ToString( delegator->GetThreadID() ) );

        delegator->Activate();
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::TaskCleanup( CTaskConsumerPtr taskConsumer ,
                           CICloneable* taskData         )
{GUCEF_TRACE;

    //MT::CObjectScopeLock lock( this );    
    delete taskData;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::PauseTask( const UInt32 taskID ,
                         const bool force    )
{GUCEF_TRACE;

    Lock();
    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskID );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            CTaskDelegator* delegator = taskConsumer->GetTaskDelegator();
            if ( NULL != delegator )
            {
                delegator->Pause( force );
                Unlock();

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Paused task with ID " + UInt32ToString( taskID ) );                
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
    Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::ResumeTask( const UInt32 taskID )
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
            CTaskDelegator* delegator = taskConsumer->GetTaskDelegator();
            if ( NULL != delegator )
            {
                delegator->Resume();

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Resumed task with ID " + UInt32ToString( taskID ) );
                NotifyObserversFromThread( TaskResumedEvent );
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::RequestTaskToStop( const UInt32 taskID   , 
                                 bool callerShouldWait )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskID );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            CTaskDelegator* delegator = taskConsumer->GetTaskDelegator();
            if ( NULL != delegator )
            {
                delegator->Deactivate( false, callerShouldWait );

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Requested task with ID " + UInt32ToString( taskID ) + " to stop" );
                return true;
            }
            else
            {
                // If a consumer does not have a delegator then it hasnt started yet
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::RequestTaskToStop( CTaskConsumerPtr taskConsumer ,
                                 bool callerShouldWait         )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
        return RequestTaskToStop( taskConsumer->GetTaskId(), callerShouldWait );    
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CTaskManager::WaitForTaskToFinish( const UInt32 taskId ,
                                   Int32 timeoutInMs   )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskId );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            CTaskDelegator* delegator = taskConsumer->GetTaskDelegator();
            if ( GUCEF_NULL != delegator )
            {
                GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "TaskManager:WaitForTaskToFinish: Waiting for task with ID " + UInt32ToString( taskId ) + " to finish" );

                lock.EarlyUnlock();          // @TODO: Delegator should be a threadsafe shared ptr
                delegator->WaitForThreadToFinish( timeoutInMs ); 
                return true;
            }
            else
            {
                // If a consumer does not have a delegator then it hasnt started yet
                GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "TaskManager:WaitForTaskToFinish: Task with ID " + UInt32ToString( taskId ) + " is not associated with any thread and as such no wait occured" );
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::WaitForTaskToFinish( CTaskConsumerPtr taskConsumer ,
                                   Int32 timeoutInMs             )
{GUCEF_TRACE;

    if ( !taskConsumer.IsNULL() )
        return WaitForTaskToFinish( taskConsumer->GetTaskId(), timeoutInMs ); 
    return true;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RequestAllTasksToStop( bool waitOnStop, bool acceptNewWork )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Requesting all tasks to stop" );
    
    MT::CObjectScopeLock lock( this );

    m_acceptNewWork = acceptNewWork;

    TTaskConsumerMap::iterator i = m_taskConsumerMap.begin();
    while ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            CTaskDelegator* delegator = taskConsumer->GetTaskDelegator();
            if ( NULL != delegator )
            {
                UInt32 taskId = taskConsumer->GetTaskId();
                delegator->Deactivate( false, waitOnStop );                                

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Requested task with ID " + UInt32ToString( taskId ) + " to stop" );
            }
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RequestAllThreadsToStop( bool waitOnStop, bool acceptNewWork )
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Requesting all tasks to stop" );
    
    MT::CObjectScopeLock lock( this );
    m_acceptNewWork = acceptNewWork;
    EnforceDesiredNrOfThreads( 0, true );
    
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::KillTask( const UInt32 taskID )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    TTaskConsumerMap::iterator i = m_taskConsumerMap.find( taskID );
    if ( i != m_taskConsumerMap.end() )
    {
        CTaskConsumerPtr taskConsumer = (*i).second;
        if ( !taskConsumer.IsNULL() )
        {
            CTaskDelegator* delegator = taskConsumer->GetTaskDelegator();
            if ( NULL != delegator )
            {
                delegator->Deactivate( true, true );

                GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Killed task with ID " + UInt32ToString( taskID ) );
                return true;
            }
            else
            {
                // If a consumer does not have a delegator then it hasnt started yet
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RegisterTaskConsumer( CTaskConsumer& consumer        ,
                                    CTaskConsumer::TTaskId& taskId )
{GUCEF_TRACE;

    {
        MT::CObjectScopeLock lock( this );
        //@TODO: gcc does not allow direct assignment, check this
        TTaskIdGenerator::TNumericID tmp = m_taskIdGenerator.GenerateID( false );
        taskId = tmp;
        m_taskConsumerMap[ taskId ] = &consumer;
    }
    SubscribeTo( &consumer );
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::UnregisterTaskConsumer( CTaskConsumer& consumer        ,
                                      CTaskConsumer::TTaskId& taskId )
{GUCEF_TRACE;

    {
        MT::CObjectScopeLock lock( this );
        m_taskConsumerMap.erase( consumer.GetTaskId() );
        m_taskIdGenerator.ReleaseID( &taskId );
        RemoveConsumerFromQueue( &consumer );
    }
    UnsubscribeFrom( &consumer );
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RegisterTaskDelegator( CTaskDelegator& delegator )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_taskDelegators.insert( &delegator );
    ++m_activeNrOfThreads;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::UnregisterTaskDelegator( CTaskDelegator& delegator )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_taskDelegators.erase( &delegator );
    -- m_activeNrOfThreads;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
