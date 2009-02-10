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

#include "gucefCORE_CSingleTaskDelegator.h"

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

CSingleTaskDelegator::CSingleTaskDelegator( CTaskConsumer* taskConsumer ,
                                            CICloneable* taskData       ,
                                            CObserver* taskObserver     )
    : CTaskDelegator()               ,
      m_taskData( taskData )         ,
      m_taskObserver( taskObserver )
{GUCEF_TRACE;

    SetAsTaskDelegator( taskConsumer );
}

/*-------------------------------------------------------------------------*/

CSingleTaskDelegator::~CSingleTaskDelegator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CSingleTaskDelegator::OnTaskCycle( void* taskdata )
{GUCEF_TRACE;

    CTaskConsumer* taskConsumer = GetTaskConsumer();
    if ( NULL != m_taskObserver )
    {
        taskConsumer->Subscribe( m_taskObserver );
    }
    bool result = taskConsumer->ProcessTask( m_taskData );
    PerformTaskCleanup( taskConsumer, m_taskData );
    return result;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
