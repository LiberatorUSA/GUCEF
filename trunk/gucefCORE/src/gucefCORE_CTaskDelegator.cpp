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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTaskDelegator::CTaskDelegator( void )
    : CActiveObject()        ,
      m_taskConsumer( NULL ) ,
      m_taskManager( NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTaskDelegator::~CTaskDelegator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::OnTaskStart( void* taskdata )
{GUCEF_TRACE;

    m_taskManager = CTaskManager::Instance();
    m_taskManager->FlagTaskAsActive( *this );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CTaskDelegator::OnTaskCycle( void* taskdata )
{GUCEF_TRACE;

    CTaskConsumer* taskConsumer = NULL;
    CICloneable* taskData = NULL;
    CObserver* taskObserver = NULL;
    
    if ( m_taskManager->GetQueuedTask( &taskConsumer ,
                                       &taskData     ,
                                       &taskObserver ) )
    {
        SetAsTaskDelegator( taskConsumer );
        if ( NULL != taskObserver )
        {
            taskConsumer->Subscribe( taskObserver );
        }
        taskConsumer->ProcessTask( taskData );
        
        m_taskManager->TaskCleanup( taskConsumer ,
                                    taskData     );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CTaskConsumer*
CTaskDelegator::GetTaskConsumer( void )
{GUCEF_TRACE;

    return m_taskConsumer;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::SetAsTaskDelegator( CTaskConsumer* taskConsumer )
{GUCEF_TRACE;

    m_taskConsumer = taskConsumer;
    taskConsumer->SetTaskDelegator( this );
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::OnTaskEnd( void* taskdata )
{GUCEF_TRACE;

    m_taskManager->FlagTaskAsNonActive( *this );
}

/*-------------------------------------------------------------------------*/

CTaskManager&
CTaskDelegator::GetTaskManager( void ) const
{GUCEF_TRACE;

    return *m_taskManager;
}

/*-------------------------------------------------------------------------*/

void
CTaskDelegator::PerformTaskCleanup( CTaskConsumer* taskConsumer ,
                                    CICloneable* taskData       ) const
{GUCEF_TRACE;

    m_taskManager->TaskCleanup( taskConsumer, taskData );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
