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

#ifndef GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H
#include "gucefMT_CObjectScopeReadOnlyLock.h"
#define GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H
#endif /* GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H ? */ 

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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

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

const CEvent CTaskManager::ThreadPoolCreatedEvent = "GUCEF::CORE::CTaskManager::ThreadPoolCreatedEvent";
const CEvent CTaskManager::ThreadPoolDestructionEvent = "GUCEF::CORE::CTaskManager::ThreadPoolCreatedEvent";

const CString CTaskManager::ClassTypeName = "GUCEF::CORE::CTaskManager";
const CString CTaskManager::DefaultThreadPoolName = "default";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void 
CTaskManager::RegisterEvents( void )
{GUCEF_TRACE;

    ThreadPoolCreatedEvent.Initialize();
    ThreadPoolDestructionEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CTaskManager::CTaskManager( void )
    : CTSGNotifier() 
    , m_taskIdGenerator()
    , m_consumerFactory()     
    , m_taskDataFactory()
    , m_threadPools() 
    , m_desiredGlobalNrOfThreads( 0 )
    , m_activeGlobalNrOfThreads( 0 )
{GUCEF_TRACE;

    m_threadPools[ DefaultThreadPoolName ] = ( GUCEF_NEW CThreadPool( CORE::CCoreGlobal::Instance()->GetPulseGenerator(), DefaultThreadPoolName ) )->CreateSharedPtr();
    
    ThreadPoolCreatedEventData eData( DefaultThreadPoolName );
    NotifyObserversFromThread( ThreadPoolCreatedEvent, &eData );
}

/*-------------------------------------------------------------------------*/

CTaskManager::~CTaskManager( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    SignalUpcomingDestruction();
    m_threadPools.clear();
    lock.EarlyUnlock();
}

/*-------------------------------------------------------------------------*/

const CString&
CTaskManager::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::OnPumpedNotify( CNotifier* notifier    ,
                              const CEvent& eventid  ,
                              CICloneable* eventdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ThreadPoolPtr 
CTaskManager::GetThreadPool( const CString& threadPoolName ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.find( threadPoolName );
    if ( i != m_threadPools.end() )
    {
        return (*i).second;
    }
    return ThreadPoolPtr();
}

/*-------------------------------------------------------------------------*/

ThreadPoolPtr 
CTaskManager::GetOrCreateThreadPool( const CString& threadPoolName            , 
                                     PulseGeneratorPtr threadPoolPulseContext ,
                                     bool createIfNotExists                   )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::iterator i = m_threadPools.find( threadPoolName );
    if ( i != m_threadPools.end() )
    {
        return (*i).second;
    }
    
    if ( createIfNotExists )
    {
        ThreadPoolPtr newPool = ( GUCEF_NEW CThreadPool( threadPoolPulseContext, threadPoolName ) )->CreateSharedPtr();
        m_threadPools[ threadPoolName ] = newPool;

        ThreadPoolCreatedEventData eData( threadPoolName );
        NotifyObserversFromThread( ThreadPoolCreatedEvent, &eData ); 
        return newPool; 
    }

    return ThreadPoolPtr();
}

/*-------------------------------------------------------------------------*/

ThreadPoolPtr 
CTaskManager::GetOrCreateThreadPool( const CString& threadPoolName , 
                                     bool createIfNotExists        )
{GUCEF_TRACE;

    return GetOrCreateThreadPool( threadPoolName, CORE::CCoreGlobal::Instance()->GetPulseGenerator(), createIfNotExists );        
}

/*-------------------------------------------------------------------------*/

UInt32 
CTaskManager::GetGlobalNrOfActiveThreads( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );

    UInt32 threadCount = 0;
    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        threadCount += (*i).second->GetActiveNrOfThreads();
        ++i;
    }
    return threadCount;
}

/*-------------------------------------------------------------------------*/

UInt32
CTaskManager::GetGlobalNrOfDesiredThreads( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );

    UInt32 threadCount = 0;
    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        threadCount += (*i).second->GetDesiredNrOfThreads();
        ++i;
    }
    return threadCount;
}

/*-------------------------------------------------------------------------*/

UInt32 
CTaskManager::GetGlobalNrOfQueuedTasks( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );

    UInt32 taskCount = 0;
    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        taskCount += (*i).second->GetNrOfQueuedTasks();
        ++i;
    }
    return taskCount;
}

/*-------------------------------------------------------------------------*/

void 
CTaskManager::RequestAllThreadsToStop( bool waitOnStop, bool acceptNewWork )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        (*i).second->RequestAllThreadsToStop( waitOnStop, acceptNewWork );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::RemoveConsumer( UInt32 taskID )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        (*i).second->RemoveConsumer( taskID );
        ++i;
    }
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

void
CTaskManager::RegisterTaskConsumerId( CTaskConsumer::TTaskId& taskId )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    //@TODO: gcc does not allow direct assignment, check this
    TTaskIdGenerator::TNumericID tmp = m_taskIdGenerator.GenerateID( false );
    taskId = tmp;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::UnregisterTaskConsumerId( CTaskConsumer::TTaskId& taskId )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    RemoveConsumer( taskId );
    m_taskIdGenerator.ReleaseID( &taskId );
}

/*-------------------------------------------------------------------------*/

CTaskConsumerPtr
CTaskManager::CreateTaskConsumer( const CString& taskType )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    CTaskConsumerPtr taskConsumer( m_consumerFactory.Create( taskType ) );
    return taskConsumer;
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::GetAllRegisteredTaskConsumerFactoryTypes( CORE::CString::StringSet& taskTypes )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        (*i).second->GetAllRegisteredTaskConsumerFactoryTypes( taskTypes );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskManager::GetRegisteredTaskConsumerFactoryTypes( const CString& threadPoolName       ,
                                                     CORE::CString::StringSet& taskTypes )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::iterator i = m_threadPools.find( threadPoolName );
    if ( i != m_threadPools.end() )
    {
        (*i).second->GetAllRegisteredTaskConsumerFactoryTypes( taskTypes );
    }
}

/*-------------------------------------------------------------------------*/

void 
CTaskManager::GetAllThreadPoolNames( CORE::CString::StringSet& poolNames ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        poolNames.insert( (*i).first );
        ++i;
    }
}
/*-------------------------------------------------------------------------*/

bool 
CTaskManager::GetInfo( CTaskManagerInfo& info ) const
{GUCEF_TRACE;

    // Obtain an overall lock to get a coherent snapshot of threadpool info
    MT::CObjectScopeLock lock( this );

    info.SetActiveGlobalNrOfThreads( m_activeGlobalNrOfThreads );
    info.SetDesiredGlobalNrOfThreads( m_desiredGlobalNrOfThreads );
    m_consumerFactory.ObtainKeySet( info.GetTaskConsumerFactoryTypes() );
    m_taskDataFactory.ObtainKeySet( info.GetTaskDataFactoryTypes() );
    GetAllThreadPoolNames( info.GetThreadPoolNames() );
    info.SetGlobalNrOfQueuedTasks( GetGlobalNrOfQueuedTasks() );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetTaskIdForThreadId( const UInt32 threadId ,
                                    UInt32& taskId        ) const
{GUCEF_TRACE;

    taskId = 0;
    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        if ( (*i).second->GetTaskIdForThreadId( threadId, taskId ) )
            return true;
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetThreadIdForTaskId( const UInt32 taskId ,
                                    UInt32& threadId    ) const
{GUCEF_TRACE;

    threadId = 0;
    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        if ( (*i).second->GetThreadIdForTaskId( taskId, threadId ) )
            return true;
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetTaskInfo( UInt32 taskId                                             , 
                           CTaskInfo& info                                           ,
                           bool obtainTaskDataCopyIfPossible                         ,
                           CDataNodeSerializableSettings* taskDataSerializerSettings ) const
{GUCEF_TRACE;

    info.Clear();
    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        if ( (*i).second->GetTaskInfo( taskId, info, obtainTaskDataCopyIfPossible, taskDataSerializerSettings ) )
            return true;
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetAllTaskInfo( TTaskInfoMap& info                                        ,
                              bool obtainTaskDataCopyIfPossible                         ,
                              CDataNodeSerializableSettings* taskDataSerializerSettings ) const
{GUCEF_TRACE;

    info.clear();
    MT::CObjectScopeLock lock( this );

    bool totalSuccess = true;
    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        totalSuccess = (*i).second->GetAllTaskInfo( info                         , 
                                                    obtainTaskDataCopyIfPossible , 
                                                    taskDataSerializerSettings   ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CTaskManager::GetThreadPoolInfo( const CString& poolName, CThreadPoolInfo& info ) const
{GUCEF_TRACE;

    info.Clear();
    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.find( poolName );
    if ( i != m_threadPools.end() )
    {
        if ( (*i).second->GetInfo( info ) )
            return true;
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetThreadInfo( UInt32 threadId, CThreadInfo& info ) const
{GUCEF_TRACE;

    info.Clear();
    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        if ( (*i).second->GetThreadInfo( threadId, info ) )
            return true;
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetAllThreadInfo( TThreadInfoMap& info ) const
{GUCEF_TRACE;

    info.clear();
    MT::CObjectScopeLock lock( this );

    bool totalSuccess = true;
    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        totalSuccess = (*i).second->GetAllThreadInfo( info ) && totalSuccess;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CTaskManager::IsTaskDataForTaskTypeSerializable( const CString& taskType ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        if ( (*i).second->IsTaskDataForTaskTypeSerializable( taskType ) )
            return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskManager::GetSerializedTaskDataCopy( const UInt32 taskId                               ,
                                         CDataNode& domNode                                ,
                                         CDataNodeSerializableSettings& serializerSettings ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    ThreadPoolMap::const_iterator i = m_threadPools.begin();
    while ( i != m_threadPools.end() )
    {
        if ( (*i).second->GetSerializedTaskDataCopy( taskId, domNode, serializerSettings ) )
            return true;
        ++i;
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
