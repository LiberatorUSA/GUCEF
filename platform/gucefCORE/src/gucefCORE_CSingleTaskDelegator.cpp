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

#include <assert.h>

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

CSingleTaskDelegator::CSingleTaskDelegator( CThreadPool* threadPool       ,
                                            CTaskConsumerPtr taskConsumer ,
                                            CICloneable* taskData         )
    : CTaskDelegator( threadPool, taskConsumer, taskData )
{GUCEF_TRACE;

    assert( !taskConsumer.IsNULL() );
}

/*-------------------------------------------------------------------------*/

CSingleTaskDelegator::~CSingleTaskDelegator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CSingleTaskDelegator::OnThreadCycle( void* taskdata )
{GUCEF_TRACE;

    if ( !m_taskConsumer.IsNULL() )
    {
        bool result = CTaskDelegator::ProcessTask( m_taskConsumer, m_taskData );
        TaskCleanup( m_taskConsumer, m_taskData );
        m_taskConsumer = GUCEF_NULL;
        m_taskData = GUCEF_NULL;
        return result;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CSingleTaskDelegator::ExecuteTaskFromCallingThread( void )
{GUCEF_TRACE;

    return OnThreadCycle( GUCEF_NULL );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
