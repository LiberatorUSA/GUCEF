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

#ifndef GUCEF_CORE_TASKSTATUS_H
#define GUCEF_CORE_TASKSTATUS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum ETaskStatus : UInt8
{
    TASKSTATUS_UNDEFINED        = 0,    /**< not a real task status but rather an initialization value */

    TASKSTATUS_QUEUED              ,    /**< the task is sitting in a queue waiting for a worker thread to pick it up, currently no thread is assigned */
    TASKSTATUS_STARTUP             ,
    TASKSTATUS_STARTUP_FAILED      ,    /**< the task was started by a thread but the startup handler for the task reported an error and no further processing occured */
    TASKSTATUS_RUNNING             ,    /**< the task has been successfully started and a thread is currently working the task */
    TASKSTATUS_PAUSED              ,    /**< the task is currently paused, startup had completed and work had started but subsequently the work was put on hold */
    TASKSTATUS_RESUMED             ,    /**< same as 'TASKSTATUS_RUNNING' except that the task had been paused during its run cycle */
    TASKSTATUS_STOPPED             ,    /**< the task has been stopped on external request instead of finishing a finite length task */
    TASKSTATUS_FINISHED                 /**< the task has stopped because it finished a finite length task */
};
typedef enum ETaskStatus TTaskStatus;

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP const char* 
TaskStatusToTaskStatusString( TTaskStatus taskStatus );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP TTaskStatus 
TaskStatusStringToTaskStatus( const CString& taskStatusStr );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP TTaskStatus 
TaskStatusStringToTaskStatus( const char* taskStatusStr );

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_TASKSTATUS_H ? */
