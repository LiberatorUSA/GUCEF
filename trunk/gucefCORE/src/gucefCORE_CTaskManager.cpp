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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

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

CTaskManager::CTaskQueueItem::CTaskQueueItem( CTaskConsumer::TTaskID& taskId ,
                                              CICloneable* taskData          ,
                                              CObserver* taskObserver        )
    : m_taskId( taskId )             ,
      m_taskData( taskData )         ,
      m_taskObserver( taskObserver )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/
    
CTaskManager::CTaskQueueItem::CTaskQueueItem( CTaskQueueItem& src )
    : m_taskId( src.m_taskId )              ,
      m_taskData( src.m_taskData->Clone() ) ,
      m_taskObserver( src.m_taskObserver )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CTaskManager::CTaskQueueItem::~CTaskQueueItem()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CObserver*
CTaskManager::CTaskQueueItem::GetTaskObserver( void )
{GUCEF_TRACE;

    return m_taskObserver;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CTaskManager::CTaskQueueItem::GetTaskData( void )
{GUCEF_TRACE;

    return m_taskData; 
}

/*-------------------------------------------------------------------------*/

const CTaskConsumer::TTaskID& 
CTaskManager::CTaskQueueItem::GetTaskId( void ) const
{GUCEF_TRACE;

    return m_taskId;
}

/*-------------------------------------------------------------------------*/

CTaskConsumer::TTaskID&
CTaskManager::CTaskQueueItem::GetMutableTaskId( void )
{
    return m_taskId;
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
      m_activeTasks()           ,
      m_nonactiveTasks()        ,
      m_desiredNrOfThreads( 0 ) ,
      m_taskQueue()             ,
      m_taskIdGenerator()
{GUCEF_TRACE;

    RegisterEvents();
    
    CGUCEFApplication::Instance()->Subscribe( &AsObserver(), CGUCEFApplication::AppShutdownEvent );
}

/*-------------------------------------------------------------------------*/

CTaskManager::~CTaskManager( void )
{GUCEF_TRACE;

    g_mutex.Lock();
    // Cleanup tasks
    TTaskMap::iterator i = m_activeTasks.begin();
    while ( i != m_activeTasks.end() )
    {
        // Kill the task
        (*i).second->Deactivate( true );
        
        // Delete the task object
        delete (*i).second;
        ++i;
    }
    m_activeTasks.clear();
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

    g_mutex.Lock();
    if ( NULL == g_instance )
    {
        g_instance = new CTaskManager();
        g_instance->EnforceDesiredNrOfThreads();
    }
    g_mutex.Unlock();
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::Deinstance( void )
{GUCEF_TRACE;

    g_mutex.Lock();
    delete g_instance;
    g_instance = NULL;
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
        // Make sure we shut down all tasks
        TTaskMap::iterator i = m_activeTasks.begin();
        while ( i != m_activeTasks.end() )
        {
            // Ask them to shut down gracefully
            (*i).second->Deactivate( false );
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::EnforceDesiredNrOfThreads( void )
{GUCEF_TRACE;
    
    // This is an internal function but still make sure that the TaskManager is locked
    // when this function is called. There are no locks here for efficiency!!!
    
    // Check if we need to do anything
    if ( m_desiredNrOfThreads > m_activeTasks.size() )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Increasing the number of threads used for processing tasks to " + UInt32ToString( m_desiredNrOfThreads ) + " from " + UInt32ToString( (UInt32)m_activeTasks.size() ) );
        
        UInt32 addCount = m_desiredNrOfThreads - m_activeTasks.size();
        for ( UInt32 i=0; i<addCount; ++i )
        {
            // Just spawn a task delegator, it will auto register as an active task
            CTaskDelegator* delegator = new CTaskDelegator();
            delegator->Activate();
        }
    }
    else
    if ( m_desiredNrOfThreads < m_activeTasks.size() )
    {        
        UInt32 deactivateCount = ((UInt32)m_activeTasks.size()) - m_desiredNrOfThreads;
        
        // Check the number of threads that are already asked to deactivate
        UInt32 deactivatingCount = 0;
        TTaskMap::iterator i = m_activeTasks.begin();
        while ( i != m_activeTasks.end() )
        {
            if ( (*i).second->IsDeactivationRequested() )
            {
                ++deactivatingCount;
            }
            ++i;
        }
        
        // Check if we need to do anything
        Int32 leftToBeDeactivated = (Int32) deactivateCount - deactivatingCount;
        if ( leftToBeDeactivated > 0 )
        {
            GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Decreasing the number of threads used for processing tasks to " + 
                                                UInt32ToString( m_desiredNrOfThreads ) + " from " + UInt32ToString( (UInt32)m_activeTasks.size() ) + 
                                                " by asking " + Int32ToString( leftToBeDeactivated ) + " additional threads to deactivate" );
            
            TTaskMap::iterator i = m_activeTasks.begin();
            while ( i != m_activeTasks.end() )
            {
                // If the thread is not yet asked to deactivate we will do so now up
                // to the number of thread we wish to deactivate
                if ( !(*i).second->IsDeactivationRequested() )
                {
                    // Ask thread to gracefully deactivate
                    (*i).second->Deactivate( false );
                }
                ++i;
            }
        }
    }
    // else: we don't have to do anything
    
    if ( m_nonactiveTasks.size() > 0 )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Cleaning up non-active tasks " );
        
        // Cleanup tasks that have been discarded
        TTaskMap::iterator i = m_nonactiveTasks.begin();
        while ( i != m_nonactiveTasks.end() )
        {
            delete (*i).second;
            ++i;
        }
        m_nonactiveTasks.clear();
    }
}

/*-------------------------------------------------------------------------*/
    
void
CTaskManager::QueueTask( const CString& taskType ,
                         CICloneable* taskData   ,
                         CObserver* taskObserver )
{GUCEF_TRACE;
                         
    CTaskConsumer::TTaskID uniqueTaskId = m_taskIdGenerator.GenerateID( false );
    CTaskQueueItem* queueItem = new CTaskQueueItem( uniqueTaskId ,
                                                    taskData     ,
                                                    taskObserver );
    
    NotifyObservers( TaskQueuedEvent );
    
    g_mutex.Lock();
    m_taskQueue.AddMail( taskType, queueItem );
    g_mutex.Unlock();
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
                             CTaskConsumer::TTaskID* taskId ,
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
        CTaskConsumer::TTaskID uniqueTaskId = m_taskIdGenerator.GenerateID( false );
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
    CTaskConsumer::TTaskID uniqueTaskId = taskConsumer->GetMutableTaskId();
    m_consumerFactory.Destroy( taskConsumer );
    m_taskIdGenerator.ReleaseID( &uniqueTaskId );
    delete taskData;
    EnforceDesiredNrOfThreads();
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::FlagTaskAsActive( CTaskDelegator& task )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_activeTasks[ task.GetThreadID() ] = &task;
    
    CTaskConsumer* taskConsumer = task.GetTaskConsumer();
    if ( NULL != taskConsumer )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Task delegator activated for task of type \"" + taskConsumer->GetType() + 
                                           "\" with ID " + UInt32ToString( taskConsumer->GetTaskID() ) + " using thread with ID " + UInt32ToString( task.GetThreadID() ) );
    }
    else
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Task delegator activated for tasks using thread with ID " + UInt32ToString( task.GetThreadID() ) );
    }
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/
    
void
CTaskManager::FlagTaskAsNonActive( CTaskDelegator& task )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_activeTasks.erase( task.GetThreadID() );
    m_nonactiveTasks[ task.GetThreadID() ] = &task;
    
    CTaskConsumer* taskConsumer = task.GetTaskConsumer();
    if ( NULL != taskConsumer )
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Task delegator deactivated for task of type \"" + taskConsumer->GetType() + 
                                           "\" with ID " + UInt32ToString( taskConsumer->GetTaskID() ) + " using thread with ID " + UInt32ToString( task.GetThreadID() ) );
    }
    else
    {
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "TaskManager: Task delegator deactivated for tasks using thread with ID " + UInt32ToString( task.GetThreadID() ) );
    }
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::PauseTask( const UInt32 taskID ,
                         const bool force    )
{GUCEF_TRACE;

    g_mutex.Lock();
    TTaskMap::iterator i = m_activeTasks.find( taskID );
    if ( i != m_activeTasks.end() )
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
