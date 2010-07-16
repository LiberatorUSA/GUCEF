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

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

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

MT::CMutex CTaskManager::g_mutex;
CTaskManager* CTaskManager::g_instance = NULL;

const CEvent CTaskManager::ThreadKilledEvent = "GUCEF::CORE::CTaskManager::ThreadKilledEvent";
const CEvent CTaskManager::ThreadStartedEvent = "GUCEF::CORE::CTaskManager::ThreadStartedEvent";
const CEvent CTaskManager::ThreadPausedEvent = "GUCEF::CORE::CTaskManager::ThreadPausedEvent";
const CEvent CTaskManager::ThreadResumedEvent = "GUCEF::CORE::CTaskManager::ThreadResumedEvent";
const CEvent CTaskManager::ThreadFinishedEvent = "GUCEF::CORE::CTaskManager::ThreadFinishedEvent";

const CEvent CTaskManager::TaskQueuedEvent = "GUCEF::CORE::CTaskManager::TaskQueuedEvent";
const CEvent CTaskManager::QueuedTaskStartedEvent = "GUCEF::CORE::CTaskManager::QueuedTaskStartedEvent";
const CEvent CTaskManager::TaskStartedImmediatelyEvent = "GUCEF::CORE::CTaskManager::TaskStartedImmediatelyEvent";
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
    TaskStartedImmediatelyEvent.Initialize();
    TaskKilledEvent.Initialize();
    TaskStoppedEvent.Initialize();
    TaskPausedEvent.Initialize();
    TaskResumedEvent.Initialize();
    TaskFinishedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CTaskManager::CTaskQueueItem::CTaskQueueItem( CTaskConsumer* consumer ,
                                              CICloneable* taskData   )
    : CICloneable()              ,
      m_taskData( taskData )     ,
      m_taskConsumer( consumer )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTaskManager::CTaskQueueItem::CTaskQueueItem( CTaskQueueItem& src )
    : CICloneable()                         ,
      m_taskData( src.m_taskData->Clone() ) ,
      m_taskConsumer( src.m_taskConsumer )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTaskManager::CTaskQueueItem::~CTaskQueueItem()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTaskConsumer*
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
    : CObservingNotifier()      ,
      m_consumerFactory()       ,
      m_desiredNrOfThreads( 0 ) ,
      m_activeNrOfThreads( 0 )  ,
      m_taskQueue()             ,
      m_taskIdGenerator()       ,
      m_taskConsumerMap()       ,
      m_taskDelegators()      
{GUCEF_TRACE;

    RegisterEvents();

    CGUCEFApplication::Instance()->Subscribe( &AsObserver(), CGUCEFApplication::AppShutdownEvent );
}

/*-------------------------------------------------------------------------*/

CTaskManager::~CTaskManager( void )
{GUCEF_TRACE;

    g_mutex.Lock();
    
    // Cleanup tasks
    TTaskDelegatorSet::iterator i = m_taskDelegators.begin();
    while ( i != m_taskDelegators.end() )
    {
        // Kill the task
        (*i)->Deactivate( true );
        ++i;
    }
    m_taskDelegators.clear();
    
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

const CString&
CTaskManager::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::CORE::CTaskManager";
    return typeName;
}

/*-------------------------------------------------------------------------*/

CTaskManager*
CTaskManager::Instance( void )
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        g_mutex.Lock();
        if ( NULL == g_instance )
        {
            g_instance = new CTaskManager();
            
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager Singleton created" );
        }
        g_mutex.Unlock();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::Deinstance( void )
{GUCEF_TRACE;

    g_mutex.Lock();
    delete g_instance;
    g_instance = NULL;
    
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager Singleton destroyed" );
    
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::OnNotify( CNotifier* notifier    ,
                        const CEvent& eventid  ,
                        CICloneable* eventdata )
{GUCEF_TRACE;

    if ( CGUCEFApplication::AppShutdownEvent == eventid )
    {
        g_mutex.Lock();
        
        // Make sure we shut down all tasks
        EnforceDesiredNrOfThreads( 0, true );
        
        g_mutex.Unlock();
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RemoveConsumerFromQueue( CTaskConsumer* consumer )
{GUCEF_TRACE;

    g_mutex.Lock();    
    m_taskQueue.LockData();
    
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
    
    m_taskQueue.UnlockData();
    g_mutex.Unlock();
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
    if ( desiredNrOfThreads > m_activeNrOfThreads )
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
    if ( desiredNrOfThreads < m_activeNrOfThreads )
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
                        (*i)->Deactivate( gracefullEnforcement );
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

void
CTaskManager::LockData( void ) const
{GUCEF_TRACE;

    g_mutex.Lock();
}

/*-------------------------------------------------------------------------*/
    
void
CTaskManager::UnlockData( void ) const
{GUCEF_TRACE;

    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::QueueTask( const CString& taskType ,
                         CICloneable* taskData   ,
                         CTaskConsumer** task    )
{GUCEF_TRACE;

    CTaskQueueItem* queueItem = new CTaskQueueItem( ,
                                                    taskData     ,
                                                    taskObserver );
    g_mutex.Lock();
    m_taskQueue.AddMail( taskType, queueItem );
    g_mutex.Unlock();
    NotifyObservers( TaskQueuedEvent );
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

    g_mutex.Lock();
    m_desiredNrOfThreads = nrOfThreads;
    EnforceDesiredNrOfThreads();
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

UInt32
CTaskManager::GetNrOfThreads( void ) const
{GUCEF_TRACE;

    return (UInt32) m_activeTasks.size();
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RegisterTaskConsumerFactory( const CString& taskType       ,
                                           TTaskConsumerFactory* factory )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_consumerFactory.RegisterConcreteFactory( taskType, factory );
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: new consumer factory registerd of type " + taskType );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::UnregisterTaskConsumerFactory( const CString& taskType )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_consumerFactory.UnregisterConcreteFactory( taskType );
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: consumer factory unregisterd of type " + taskType );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetQueuedTask( CTaskConsumer** taskConsumer   ,
                             CTaskConsumer::TTaskId* taskId ,
                             CICloneable** taskData         ,
                             CObserver** taskObserver       )
{GUCEF_TRACE;

    g_mutex.Lock();
    EnforceDesiredNrOfThreads();
    CString taskConsumerType;
    CICloneable* queueItemPtr;
    if ( m_taskQueue.GetMail( taskConsumerType ,
                              &queueItemPtr       ) )
    {
        CTaskQueueItem* queueItem = static_cast< CTaskQueueItem* >( queueItemPtr );

        if ( m_consumerFactory.IsConstructible( taskConsumerType ) )
        {
            *taskConsumer = m_consumerFactory.Create( taskConsumerType );
            *taskId = queueItem->GetMutableTaskId();
            *taskData = queueItem->GetTaskData();
            *taskObserver = queueItem->GetTaskObserver();
        }

        g_mutex.Unlock();
        return *taskConsumer != NULL;
    }
    g_mutex.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::StartTask( const CString& taskType     ,
                         CICloneable* taskData       ,
                         CObserver* taskObserver     ,
                         UInt32* taskID /* = NULL */ )

{GUCEF_TRACE;

    g_mutex.Lock();

    // Create a consumer for the given task type
    CTaskConsumer* taskConsumer = m_consumerFactory.Create( taskType );
    if ( NULL != taskConsumer )
    {
        // Just spawn a task delegator, it will auto register as an active task
        CTaskConsumer::TTaskId uniqueTaskId = m_taskIdGenerator.GenerateID( false );
        *taskID = uniqueTaskId;
        CTaskDelegator* delegator = new CSingleTaskDelegator( taskConsumer, uniqueTaskId, taskData->Clone(), taskObserver );
        delegator->Activate();

        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Started task immediatly of type \"" + taskType + "\" with ID " +
                                            UInt32ToString( *taskID ) + " using thread with ID " + UInt32ToString( delegator->GetThreadID() ) );

        g_mutex.Unlock();
        return true;
    }

    g_mutex.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::TaskCleanup( CTaskConsumer* taskConsumer ,
                           CICloneable* taskData       )
{GUCEF_TRACE;

    g_mutex.Lock();
    CTaskConsumer::TTaskId uniqueTaskId = taskConsumer->GetMutableTaskId();
    m_consumerFactory.Destroy( taskConsumer );
    m_taskIdGenerator.ReleaseID( &uniqueTaskId );
    delete taskData;
    EnforceDesiredNrOfThreads();
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::PauseTask( const UInt32 taskID ,
                         const bool force    )
{GUCEF_TRACE;

    g_mutex.Lock();
    TTaskMap::iterator i = m_taskConsumerMap.find( taskID );
    if ( i != m_taskConsumerMap.end() )
    {
        (*i).second->Pause( force );
        g_mutex.Unlock();

        NotifyObservers( TaskPausedEvent );
        return true;
    }
    g_mutex.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::ResumeTask( const UInt32 taskID )
{GUCEF_TRACE;

    g_mutex.Lock();
    TTaskMap::iterator i = m_activeTasks.find( taskID );
    if ( i != m_activeTasks.end() )
    {
        (*i).second->Resume();
        g_mutex.Unlock();

        NotifyObservers( TaskResumedEvent );
        return true;
    }
    g_mutex.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::RequestTaskToStop( const UInt32 taskID )
{GUCEF_TRACE;

    g_mutex.Lock();
    TTaskMap::iterator i = m_activeTasks.find( taskID );
    if ( i != m_activeTasks.end() )
    {
        (*i).second->Deactivate( false );
        g_mutex.Unlock();
        return true;
    }
    g_mutex.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::KillTask( const UInt32 taskID )
{GUCEF_TRACE;

    g_mutex.Lock();
    TTaskMap::iterator i = m_activeTasks.find( taskID );
    if ( i != m_activeTasks.end() )
    {
        (*i).second->Deactivate( true );
        g_mutex.Unlock();
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Killed task with ID " + UInt32ToString( taskID ) );
        return true;
    }
    g_mutex.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RegisterTaskConsumer( CTaskConsumer& consumer        ,
                                    CTaskConsumer::TTaskId& taskId )
{GUCEF_TRACE;

    g_mutex.Lock();
    taskId = m_taskIdGenerator.GenerateID( false );
    m_taskConsumerMap[ taskId ] = &consumer;
    SubscribeTo( consumer );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/
    
void
CTaskManager::UnregisterTaskConsumer( CTaskConsumer& consumer        ,
                                      CTaskConsumer::TTaskId& taskId )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_taskConsumerMap.erase( consumer.GetTaskId() );
    m_taskIdGenerator.ReleaseID( taskId );
    RemoveObjectFromQueue( &consumer );
    UnsubscribeFrom( consumer );
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RegisterTaskDelegator( CTaskDelegator& delegator )
{GUCEF_TRACE;

    g_mutex.Lock();        
    m_taskDelegators.insert( &delegator );    
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/
    
void
CTaskManager::UnregisterTaskDelegator( CTaskDelegator& delegator )
{GUCEF_TRACE;

    g_mutex.Lock();        
    m_taskDelegators.erase( &delegator );    
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
