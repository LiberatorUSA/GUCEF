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

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTaskManager::CTaskQueueItem::CTaskQueueItem( CICloneable* taskData   ,
                                              CObserver* taskObserver )
    : m_taskData( taskData )         ,
      m_taskObserver( taskObserver )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/
    
CTaskManager::CTaskQueueItem::CTaskQueueItem( const CTaskQueueItem& src )
    : m_taskData( src.m_taskData->Clone() ) ,
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
    
CICloneable*
CTaskManager::CTaskQueueItem::Clone( void ) const
{GUCEF_TRACE;

    return new CTaskQueueItem( *this );
}

/*-------------------------------------------------------------------------*/

CTaskManager::CTaskManager( void )
    : CObservingNotifier()      ,
      m_consumerFactory()       ,
      m_activeTasks()           ,
      m_nonactiveTasks()        ,
      m_desiredNrOfThreads( 2 ) ,
      m_taskQueue()             
{GUCEF_TRACE;

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

    // Check if we need to do anything
    if ( m_desiredNrOfThreads > m_activeTasks.size() )
    {
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
        TTaskMap::iterator i = m_activeTasks.begin();
        UInt32 deactivateCount = m_desiredNrOfThreads - m_activeTasks.size();
        for ( UInt32 n=0; n<deactivateCount; ++n )
        {
            // Ask the task to stop gracefully
            (*i).second->Deactivate( false );
        }
    }
    // else: we don't have to do anything
    
    // Cleanup tasks that have been discarded
    TTaskMap::iterator i = m_nonactiveTasks.begin();
    while ( i != m_nonactiveTasks.end() )
    {
        delete (*i).second;
        ++i;
    }
    m_nonactiveTasks.clear();
}

/*-------------------------------------------------------------------------*/
    
void
CTaskManager::QueueTask( const CString& taskType ,
                         CICloneable* taskData   ,
                         CObserver* taskObserver )
{GUCEF_TRACE;
                         
    CTaskQueueItem* queueItem = new CTaskQueueItem( taskData     ,
                                                    taskObserver );
    
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
CTaskManager::GetQueuedTask( CTaskConsumer** taskConsumer ,
                             CICloneable** taskData       ,
                             CObserver** taskObserver     )
{GUCEF_TRACE;

    g_mutex.Lock();    
    EnforceDesiredNrOfThreads();
    CString taskConsumerType;
    CICloneable* queueItemPtr;
    if ( m_taskQueue.GetMail( taskConsumerType ,
                              &queueItemPtr       ) )
    {
        CTaskQueueItem* queueItem = static_cast< CTaskQueueItem* >( queueItemPtr );
        *taskConsumer = m_consumerFactory.Create( taskConsumerType );
        *taskData = queueItem->GetTaskData();
        *taskObserver = queueItem->GetTaskObserver();
        
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
        CTaskDelegator* delegator = new CSingleTaskDelegator( taskConsumer, taskData->Clone() );
        delegator->Activate();
        
        if ( NULL != taskID )
        {
            *taskID = delegator->GetThreadID();
        }
        
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
    m_consumerFactory.Destroy( taskConsumer );
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
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/
    
void
CTaskManager::FlagTaskAsNonActive( CTaskDelegator& task )
{GUCEF_TRACE;

    g_mutex.Lock();
    m_activeTasks.erase( task.GetThreadID() );
    m_nonactiveTasks[ task.GetThreadID() ] = &task;
    g_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::PauseTask( const UInt32 taskID )
{GUCEF_TRACE;

    g_mutex.Lock();
    TTaskMap::iterator i = m_activeTasks.find( taskID );
    if ( i != m_activeTasks.end() )
    {
        (*i).second->Pause();
        g_mutex.Unlock();
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
