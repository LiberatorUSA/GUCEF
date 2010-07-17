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

CTaskDelegator::CTaskDelegator( void )
    : MT::CActiveObject()    ,
      CNotifier()            ,
      m_taskConsumer( NULL )
{GUCEF_TRACE;

    CTaskManager::Instance()->RegisterTaskDelegator( *this );

}

/*-------------------------------------------------------------------------*/

CTaskDelegator::~CTaskDelegator()
{GUCEF_TRACE;

    CTaskManager::Instance()->UnregisterTaskDelegator( *this );
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::OnTaskStart( void* taskdata )
{GUCEF_TRACE;

    NotifyObservers( ThreadStartedEvent );
    return true;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnTaskStarted( void* taskdata )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::TaskCleanup( CTaskConsumer* taskConsumer ,
                             CICloneable* taskData       )
{GUCEF_TRACE;

    CTaskManager::Instance()->TaskCleanup( taskConsumer ,
                                           taskData     );
                                
    m_taskConsumer = NULL;
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::OnTaskCycle( void* taskdata )
{GUCEF_TRACE;

    CICloneable* taskData = NULL;

    if ( CTaskManager::Instance()->GetQueuedTask( &m_taskConsumer ,
                                                  &taskData       ) )
    {
        ProcessTask( *m_taskConsumer ,
                     taskData        );
                     
        TaskCleanup( m_taskConsumer ,
                     taskData       );                                    
        m_taskConsumer = NULL;
    }
    
    // Return false, this is an infinate task processing thread
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::ProcessTask( CTaskConsumer& taskConsumer ,
                             CICloneable* taskData       )
{GUCEF_TRACE;

    bool returnStatus = false;
    
    // first establish the bi-directional link
    // this delegator is going to be the one to execute this task
    taskConsumer.SetTaskDelegator( this );
    
    // Now we go through the execution sequence within a cycle as if this 
    // where a thread sequence
    m_consumerBusy = true;
    if ( taskConsumer.OnTaskStart( taskData ) )
    {
        taskConsumer.OnTaskStarted( taskData );
        
        // cycle the task as long as it is not "done"
        while ( !taskConsumer.OnTaskCycle( taskData ) )
        {
            
        }
        
        taskConsumer.OnTaskEnd( taskData );
        taskConsumer.OnTaskEnded( taskData, false );            
        returnStatus = true;
    }
    else
    {
        taskConsumer.OnTaskStartupFailed( taskData );
    }
    m_consumerBusy = false;
    return returnStatus;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnTaskEnd( void* taskdata )
{GUCEF_TRACE;

    // if we get here and the m_consumerBusy flag is set then the task was killed
    // and we did not finish whatever the consumer was doing gracefully
    // We should give the consume a chance to cleanup
    if ( m_consumerBusy )
    {
        if ( NULL != m_taskConsumer )
        {
            m_taskConsumer->OnTaskEnd( static_cast< CICloneable* >( taskdata ) );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnTaskPausedForcibly( void* taskdata )
{GUCEF_TRACE;

    if ( m_consumerBusy )
    {
        if ( NULL != m_taskConsumer )
        {
            m_taskConsumer->OnTaskPaused( static_cast< CICloneable* >( taskdata ), true );
        }
    }
}

/*-------------------------------------------------------------------------*/
    
void
CTaskDelegator::OnTaskResumed( void* taskdata )
{GUCEF_TRACE;

    if ( m_consumerBusy )
    {
        if ( NULL != m_taskConsumer )
        {
            m_taskConsumer->OnTaskResumed( static_cast< CICloneable* >( taskdata ) );
        }
    }
}
    
/*-------------------------------------------------------------------------*/
    
void
CTaskDelegator::OnTaskEnded( void* taskdata ,
                             bool forced    )
{GUCEF_TRACE;
    
    // if we get here and the m_consumerBusy flag is set then the task was killed
    // and we did not finish whatever the consumer was doing gracefully
    // We should give the consume a chance to cleanup
    if ( m_consumerBusy )
    {
        if ( NULL != m_taskConsumer )
        {
            m_taskConsumer->OnTaskEnded( static_cast< CICloneable* >( taskdata ), true );
        }
    }
}

/*-------------------------------------------------------------------------*/

CTaskConsumer*
CTaskDelegator::GetTaskConsumer( void )
{GUCEF_TRACE;

    return m_taskConsumer;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
