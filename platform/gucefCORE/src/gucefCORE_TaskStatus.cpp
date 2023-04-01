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

#include "gucefCORE_TaskStatus.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

 TTaskStatus 
 TaskStatusStringToTaskStatus( const char* taskStatusStr )
{GUCEF_TRACE;

    if ( GUCEF_NULL == taskStatusStr )
        return ETaskStatus::TASKSTATUS_UNDEFINED;

    if ( 0 == strcmp( taskStatusStr, "QUEUED" ) )
        return ETaskStatus::TASKSTATUS_QUEUED;

    if ( 0 == strcmp( taskStatusStr, "STARTUP" ) )
        return ETaskStatus::TASKSTATUS_STARTUP;

    if ( 0 == strcmp( taskStatusStr, "STARTUP_FAILED" ) )
        return ETaskStatus::TASKSTATUS_STARTUP_FAILED;

    if ( 0 == strcmp( taskStatusStr, "RUNNING" ) )
        return ETaskStatus::TASKSTATUS_RUNNING;

    if ( 0 == strcmp( taskStatusStr, "PAUSED" ) )
        return ETaskStatus::TASKSTATUS_PAUSED;

    if ( 0 == strcmp( taskStatusStr, "RESUMED" ) )
        return ETaskStatus::TASKSTATUS_RESUMED;

    if ( 0 == strcmp( taskStatusStr, "STOPPED" ) )
        return ETaskStatus::TASKSTATUS_STOPPED;        

    if ( 0 == strcmp( taskStatusStr, "FINISHED" ) )
        return ETaskStatus::TASKSTATUS_FINISHED;        

    return ETaskStatus::TASKSTATUS_UNDEFINED;
}

/*-------------------------------------------------------------------------*/

 TTaskStatus 
 TaskStatusStringToTaskStatus( const CString& taskStatusStr )
{GUCEF_TRACE;

    return TaskStatusStringToTaskStatus( taskStatusStr.C_String() );
}

/*-------------------------------------------------------------------------*/

const char*
TaskStatusToTaskStatusString( TTaskStatus taskStatus )
{GUCEF_TRACE;

    switch ( taskStatus )
    {
        case ETaskStatus::TASKSTATUS_QUEUED: return "QUEUED";
        case ETaskStatus::TASKSTATUS_STARTUP: return "STARTUP";
        case ETaskStatus::TASKSTATUS_STARTUP_FAILED: return "STARTUP_FAILED";
        case ETaskStatus::TASKSTATUS_RUNNING: return "RUNNING";
        case ETaskStatus::TASKSTATUS_PAUSED: return "PAUSED";
        case ETaskStatus::TASKSTATUS_RESUMED: return "RESUMED";
        case ETaskStatus::TASKSTATUS_STOPPED: return "STOPPED";
        case ETaskStatus::TASKSTATUS_FINISHED: return "FINISHED";

        default:
        case ETaskStatus::TASKSTATUS_UNDEFINED:
        {
            return "UNDEFINED";
        }
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
