/*
 *  gucefWEB: GUCEF module providing Web application functionality 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#include "gucefWEB_CTaskManagerServerResource.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CTaskManagerServerResource::CThreadPoolMetaData::CThreadPoolMetaData( void )
    : threadPoolInfo()
    , threadPoolInfoRsc()
    , threadPoolInfoLastUpdate( CORE::CDateTime::PastMax ) 
{GUCEF_TRACE;
    
    threadPoolInfoRsc = CDataNodeSerializableHttpServerResourcePtr( new CDataNodeSerializableHttpServerResource() );
    threadPoolInfoRsc->LinkTo( GUCEF_NULL, &threadPoolInfo, GUCEF_NULL ); 
}

/*-------------------------------------------------------------------------*/

CTaskManagerServerResource::CTaskManagerServerResource( void )
    : CORE::CTSGNotifier()
    , m_taskManagerInfo()
    , m_taskManagerInfoRsc()
    , m_threadPoolMetaDataMap()
    , m_router( GUCEF_NULL )
    , m_rwLock( true )
{GUCEF_TRACE;

    m_taskManagerInfoRsc.LinkTo( GUCEF_NULL, &m_taskManagerInfo, &m_rwLock );    
    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CTaskManagerServerResource::CTaskManagerServerResource( const CTaskManagerServerResource& src )
    : CORE::CTSGNotifier( src )
    , m_taskManagerInfo( src.m_taskManagerInfo )
    , m_taskManagerInfoRsc()
    , m_threadPoolMetaDataMap( src.m_threadPoolMetaDataMap )
    , m_router( src.m_router )
    , m_rwLock( src.m_rwLock )
{GUCEF_TRACE;

    m_taskManagerInfoRsc.LinkTo( GUCEF_NULL, &m_taskManagerInfo, &m_rwLock );
    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CTaskManagerServerResource::~CTaskManagerServerResource( void )
{GUCEF_TRACE;

    SignalUpcomingDestruction();
}

/*-------------------------------------------------------------------------*/

void
CTaskManagerServerResource::RegisterEventHandlers( void )
{GUCEF_TRACE;

    CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();
    
    TEventCallback callback( this, &CTaskManagerServerResource::OnThreadPoolCreation );
    SubscribeTo( &taskManager, CORE::CTaskManager::ThreadPoolCreatedEvent, callback );

    TEventCallback callback2( this, &CTaskManagerServerResource::OnThreadPoolDestruction );
    SubscribeTo( &taskManager, CORE::CTaskManager::ThreadPoolDestructionEvent, callback2 );    
}

/*-------------------------------------------------------------------------*/

bool 
CTaskManagerServerResource::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockOpBool( m_rwLock.WriterStart( lockWaitTimeoutInMs ) );
}

/*-------------------------------------------------------------------------*/

bool 
CTaskManagerServerResource::Unlock( void ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockOpBool( m_rwLock.WriterStop() );
}

/*-------------------------------------------------------------------------*/

bool 
CTaskManagerServerResource::ReadOnlyLock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockOpBool( m_rwLock.ReaderStart( lockWaitTimeoutInMs ) );
}

/*-------------------------------------------------------------------------*/

bool 
CTaskManagerServerResource::ReadOnlyUnlock( void ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockOpBool( m_rwLock.ReaderStop() );
}

/*-------------------------------------------------------------------------*/

void
CTaskManagerServerResource::OnThreadPoolCreation( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_router )
    {
        MT::CScopeWriterLock writeLock( m_rwLock );
        
        const CORE::CString& poolName = static_cast< CORE::CTaskManager::ThreadPoolCreatedEventData* >( eventData )->GetData();
        CThreadPoolMetaData& threadPoolMetaData = m_threadPoolMetaDataMap[ poolName ];
        
        CORE::CTaskManager& taskManager = CORE::CCoreGlobal::Instance()->GetTaskManager();
        CORE::ThreadPoolPtr threadPool = taskManager.GetThreadPool( poolName );

        bool totalSuccess = true;
        totalSuccess = threadPool->GetInfo( threadPoolMetaData.threadPoolInfo ) && totalSuccess; 
        totalSuccess = threadPool->GetAllTaskInfo( threadPoolMetaData.allTaskInfo, false, GUCEF_NULL ) && totalSuccess; 

        if ( totalSuccess )
            threadPoolMetaData.threadPoolInfoLastUpdate = CORE::CDateTime::NowUTCDateTime();    

        m_router->SetResourceMapping( "/v1/taskmanager/threadpools/" + poolName, threadPoolMetaData.threadPoolInfoRsc ); 
        //m_httpResources.push_back( threadPoolMetaData.threadPoolInfoRsc ); 
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskManagerServerResource::OnThreadPoolDestruction( CORE::CNotifier* notifier    ,
                                                     const CORE::CEvent& eventId  ,
                                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CTaskManagerServerResource::ConnectHttpRouting( CIHTTPServerRouter& webRouter )
{GUCEF_TRACE;

    // "/v1/taskmanager"                                            <- TaskManagerInfo obj
    // "/v1/taskmanager/tasks/"                                     <- task id index Or list of TaskInfo objects
    //                                                                 this one supports submitting new tasks in a data driven manner
    // "/v1/taskmanager/threads/"                                   <- thread id index Or list of ThreadInfo objects 
    // "/v1/taskmanager/threadpools/"                               <- thread pool name index Or list of ThreadPoolInfo obj
    // "/v1/taskmanager/threadpools/default/"                       <- ThreadPoolInfo obj
    // "/v1/taskmanager/threadpools/default/threads/"               <- thread id index Or list of ThreadInfo objects specific to the given thread pool
    // "/v1/taskmanager/threadpools/default/tasks/"                 <- task id index Or list of TaskInfo objects specific to the given thread pool

    m_router = &webRouter;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CTaskManagerServerResource::DisconnectHttpRouting( CIHTTPServerRouter& webRouter )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
