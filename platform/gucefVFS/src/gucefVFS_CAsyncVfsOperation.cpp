/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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

#include <string.h>
#include <time.h> 

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#include "gucefVFS_CAsyncVfsOperation.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CAsyncVfsOperation::TaskType = "AsyncVfsOperation";
const CORE::CEvent CAsyncVfsOperation::AsyncVfsOperationCompletedEvent = "GUCEF::VFS::CAsyncVfsOperation::AsyncVfsOperationCompletedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void 
CAsyncVfsOperation::RegisterEvents( void )
{GUCEF_TRACE;

    AsyncVfsOperationCompletedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CAsyncVfsOperation::CAsyncVfsOperation()
    : CORE::CTaskConsumer()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CAsyncVfsOperation::CAsyncVfsOperation( const CAsyncVfsOperation& src )
    : CORE::CTaskConsumer()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncVfsOperation::~CAsyncVfsOperation()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CString
CAsyncVfsOperation::GetType( void ) const
{GUCEF_TRACE;

    return TaskType;
}

/*-------------------------------------------------------------------------*/

bool
CAsyncVfsOperation::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    // This is a simplistic task, no bootstrap is required.
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation(" + CORE::PointerToString( this ) + "):OnTaskStart" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAsyncVfsOperation::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    time_t startTime = time( NULL );
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation(" + CORE::PointerToString( this ) + "):OnTaskCycle" );

    CAsyncVfsTaskData* syncCallData = static_cast< CAsyncVfsTaskData* >( taskData );
    if ( GUCEF_NULL == syncCallData )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation:OnTaskCycle: No task data to operate upon" );
        return true;
    }

    switch ( syncCallData->operationType )
    {
        case ASYNCVFSOPERATIONTYPE_MOUNTARCHIVE:
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation(" + CORE::PointerToString( this ) + "):OnTaskCycle: Async invocation of MountArchive" );
            
            CMountArchiveTaskData* specificSyncCallData = static_cast< CMountArchiveTaskData* >( syncCallData );
            bool success = CVfsGlobal::Instance()->GetVfs().MountArchive( specificSyncCallData->settings );

            CAsyncVfsTaskResultData syncCallResult;
            syncCallResult.successState = success;
            syncCallResult.SetTaskData( syncCallData );
            syncCallResult.durationInSecs = (UInt32) ( time( NULL ) - startTime );

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation(" + CORE::PointerToString( this ) + "):OnTaskCycle: Completed Async invocation of MountArchive" );

            if ( !NotifyObservers( AsyncVfsOperationCompletedEvent, &syncCallResult ) ) return true;
            break;
        }
        case ASYNCVFSOPERATIONTYPE_STOREDATAASFILE:
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation(" + CORE::PointerToString( this ) + "):OnTaskCycle: Async invocation of StoreAsFile" );
            
            CStoreAsFileTaskData* specificSyncCallData = static_cast< CStoreAsFileTaskData* >( syncCallData );
            bool success = CVfsGlobal::Instance()->GetVfs().StoreAsFile( specificSyncCallData->filepath       , 
                                                                         specificSyncCallData->data.GetData() ,
                                                                         specificSyncCallData->offset         ,
                                                                         specificSyncCallData->overwrite      );
        
            CAsyncVfsTaskResultData syncCallResult;
            syncCallResult.successState = success;
            syncCallResult.SetTaskData( syncCallData );
            syncCallResult.durationInSecs = (UInt32) ( time( NULL ) - startTime );

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation(" + CORE::PointerToString( this ) + "):OnTaskCycle: Completed Async invocation of StoreAsFile" );
            
            if ( !NotifyObservers( AsyncVfsOperationCompletedEvent, &syncCallResult ) ) return true;
            break;
        }
        case ASYNCVFSOPERATIONTYPE_UNKNOWN:
        default:
        {
            CAsyncVfsTaskResultData syncCallResult;
            syncCallResult.successState = false;
            syncCallResult.SetTaskData( syncCallData );
            syncCallResult.durationInSecs = (UInt32) ( time( NULL ) - startTime );
            
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation:OnTaskCycle: Invalid or Unsupported operation type specified:" + 
                    CORE::Int32ToString( (Int32) syncCallData->operationType ) );
            break;
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
CAsyncVfsOperation::OnTaskEnd( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    // This is a simplistic task, no shutdown or cleanup is required.
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AsyncVfsOperation(" + CORE::PointerToString( this ) + "):OnTaskEnd" );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
