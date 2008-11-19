/*
 *  gucefPATCHER: GUCEF RAD module providing a patch delivery system
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_PATCHER_CPATCHTASKDATA_H
#include "gucefPATCHER_CPatchTaskData.h"
#define GUCEF_PATCHER_CPATCHTASKDATA_H
#endif /* GUCEF_PATCHER_CPATCHTASKDATA_H ? */

#ifndef GUCEF_PATCHER_CPATCHTASKCONSUMER_H
#include "gucefPATCHER_CPatchTaskConsumer.h"
#define GUCEF_PATCHER_CPATCHTASKCONSUMER_H
#endif /* GUCEF_PATCHER_CPATCHTASKCONSUMER_H ? */

#include "gucefPATCHER_CPatchManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CPatchManager::PatchTaskStartedEvent = "GUCEF::PATCHER::CPatchManager::PatchTaskStartedEvent";
const CORE::CEvent CPatchManager::PatchTaskStoppedEvent = "GUCEF::PATCHER::CPatchManager::PatchTaskStoppedEvent";
const CORE::CEvent CPatchManager::PatchTaskStopRequestedEvent = "GUCEF::PATCHER::CPatchManager::PatchTaskStopRequestedEvent";
const CORE::CEvent CPatchManager::PatchTaskFinishedEvent = "GUCEF::PATCHER::CPatchManager::PatchTaskFinishedEvent";
const CORE::CEvent CPatchManager::PatchTaskPausedEvent = "GUCEF::PATCHER::CPatchManager::PatchTaskPausedEvent";
const CORE::CEvent CPatchManager::PatchTaskResumedEvent = "GUCEF::PATCHER::CPatchManager::PatchTaskResumedEvent";    
const CORE::CEvent CPatchManager::PatchTaskEventReceivedEvent = "GUCEF::PATCHER::CPatchManager::PatchTaskEventReceivedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CPatchManager::RegisterEvents( void )
{GUCEF_TRACE;

    PatchTaskStartedEvent.Initialize();
    PatchTaskStoppedEvent.Initialize();
    PatchTaskStopRequestedEvent.Initialize();
    PatchTaskFinishedEvent.Initialize();
    PatchTaskPausedEvent.Initialize();
    PatchTaskResumedEvent.Initialize();
    PatchTaskEventReceivedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPatchManager::CPatchManager( void )
    : CTSGNotifier( CORE::CGUCEFApplication::Instance()->GetPulseGenerator() ) ,
      m_taskManager( CORE::CTaskManager::Instance() )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPatchManager::CPatchManager( CORE::CPulseGenerator& pulseGenerator )
    : CTSGNotifier( pulseGenerator )                  ,
      m_taskManager( CORE::CTaskManager::Instance() )
{GUCEF_TRACE;
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/
    
CPatchManager::CPatchManager( const CPatchManager& src  )
    : CTSGNotifier( src )  
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPatchManager::~CPatchManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CPatchManager::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::PATCHER::CPatchManager";
    return typeName;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchManager::StartTask( const CString& taskName                  ,
                          const CORE::CDataNode& patchEngineConfig )

{GUCEF_TRACE;
                          
    LockData();
    if ( m_taskMap.find( taskName ) == m_taskMap.end() )
    {
        UnlockData();
        CPatchTaskData taskData( *this, patchEngineConfig, taskName );
        return m_taskManager->StartTask( CPatchTaskConsumer::GetTypeString() ,
                                         &taskData                           );
    }
    UnlockData();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchManager::PauseTask( const CString& taskName )
{GUCEF_TRACE;
    
    LockData();
    TTaskMap::iterator i = m_taskMap.find( taskName );
    if ( i != m_taskMap.end() )
    {
        UInt32 taskID = (*i).second->GetTaskID();
        UnlockData();
        
        if ( m_taskManager->PauseTask( taskID ) )
        {
            TPatchTaskPausedEventData eData( taskName );
            NotifyObserversFromThread( PatchTaskPausedEvent, &eData );
            return true;
        }
        return false;
    }
    UnlockData();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatchManager::ResumeTask( const CString& taskName )
{GUCEF_TRACE;
    
    LockData();
    TTaskMap::iterator i = m_taskMap.find( taskName );
    if ( i != m_taskMap.end() )
    {
        UInt32 taskID = (*i).second->GetTaskID();
        UnlockData();
        
        if ( m_taskManager->ResumeTask( taskID ) )
        {
            TPatchTaskResumedEventData eData( taskName );
            NotifyObserversFromThread( PatchTaskResumedEvent, &eData );
            return true;            
        }
        return false;
    }
    UnlockData();
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchManager::RequestTaskToStop( const CString& taskName )
{GUCEF_TRACE;

    LockData();
    TTaskMap::iterator i = m_taskMap.find( taskName );
    if ( i != m_taskMap.end() )
    {
        UInt32 taskID = (*i).second->GetTaskID();        
        UnlockData();
        
        if ( m_taskManager->RequestTaskToStop( taskID ) )
        {
            TPatchTaskStopRequestedEventData eData( taskName );
            NotifyObserversFromThread( PatchTaskStopRequestedEvent, &eData );
            return true;
        }
        return false;
    }
    UnlockData();
    return false;
}

/*-------------------------------------------------------------------------*/
        
void
CPatchManager::GetTaskList( TStringVector& list ) const
{GUCEF_TRACE;

    LockData();
    TTaskMap::const_iterator i = m_taskMap.begin();
    while ( i != m_taskMap.end() )
    {
        list.push_back( (*i).first );
        ++i;
    }
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CPatchManager::RegisterTask( CPatchTaskConsumer* task )
{GUCEF_TRACE;

    LockData();
    m_taskMap[ task->GetTaskName() ] = task;
    UnlockData();
    
    SubscribeTo( &task->GetPatchEngine() );
    
    TPatchTaskStartedEventData eData( task->GetTaskName() );
    NotifyObserversFromThread( PatchTaskStartedEvent, &eData );    
}

/*-------------------------------------------------------------------------*/
    
void
CPatchManager::UnregisterTask( CPatchTaskConsumer* task )
{GUCEF_TRACE;

    LockData();
    m_taskMap.erase( task->GetTaskName() );
    UnlockData();
    
    UnsubscribeFrom( &task->GetPatchEngine() );
    
    TPatchTaskFinishedEventData eData( task->GetTaskName() );
    NotifyObserversFromThread( PatchTaskFinishedEvent, &eData );
}

/*-------------------------------------------------------------------------*/

void
CPatchManager::OnPumpedNotify( CORE::CNotifier* notifier    ,
                               const CORE::CEvent& eventid  ,
                               CORE::CICloneable* eventdata )

{GUCEF_TRACE;
                               
    // @todo: finish
    //PatchTaskEventReceivedEvent
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
