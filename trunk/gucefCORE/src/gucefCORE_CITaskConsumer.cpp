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

#ifndef GUCEF_CORE_CTASKDELEGATOR_H
#include "gucefCORE_CTaskDelegator.h"
#define GUCEF_CORE_CTASKDELEGATOR_H
#endif /* GUCEF_CORE_CTASKDELEGATOR_H ? */

#include "gucefCORE_CITaskConsumer.h"

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

CTaskConsumer::CTaskConsumer( void )
    : CNotifier()         ,
      m_delegator( NULL ) ,
      m_taskId()
{GUCEF_TRACE;

}
    
/*-------------------------------------------------------------------------*/    
    
CTaskConsumer::~CTaskConsumer()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CTaskConsumer::SetTaskData( TTaskID& taskId           ,
                            CTaskDelegator* delegator )
{GUCEF_TRACE;

    m_delegator = delegator;
    m_taskId = taskId;
}

/*-------------------------------------------------------------------------*/

CTaskDelegator*
CTaskConsumer::GetTaskDelegator( void ) const
{GUCEF_TRACE;

    return m_delegator;
}

/*-------------------------------------------------------------------------*/

const CTaskConsumer::TTaskID&
CTaskConsumer::GetTaskID( void ) const
{GUCEF_TRACE;

    return m_taskId;
}

/*-------------------------------------------------------------------------*/

CTaskConsumer::TTaskID&
CTaskConsumer::GetMutableTaskId( void )
{GUCEF_TRACE;

    return m_taskId;
}

/*-------------------------------------------------------------------------*/

const CString&
CTaskConsumer::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::CORE::CTaskConsumer";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
