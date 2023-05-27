/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CDNSCACHE_H
#include "gucefCOMCORE_CDnsCache.h"
#define GUCEF_COMCORE_CDNSCACHE_H
#endif /* GUCEF_COMCORE_CDNSCACHE_H ? */

#include "gucefCOMCORE_CDnsCacheRefreshTaskConsumer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CDnsCacheRefreshTaskConsumer::TaskType = "DnsCacheRefreshTask";
const CORE::CString CDnsCacheRefreshTaskConsumer::ClassTypeName = "GUCEF::COMCORE::CDnsCacheRefreshTaskConsumer";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDnsCacheRefreshTaskConsumer::CDnsCacheRefreshTaskConsumer( void )
    : CTaskConsumer()        
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDnsCacheRefreshTaskConsumer::~CDnsCacheRefreshTaskConsumer()
{GUCEF_TRACE;

    SignalUpcomingDestruction();
}
    
/*-------------------------------------------------------------------------*/

CORE::CString
CDnsCacheRefreshTaskConsumer::GetType( void ) const
{GUCEF_TRACE;

    return TaskType;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CDnsCacheRefreshTaskConsumer::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheRefreshTaskConsumer::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    CDnsCachePtr* ptrPtr = static_cast< CDnsCachePtr* >( taskData );
    if ( GUCEF_NULL == ptrPtr || (*ptrPtr).IsNULL() )
        return false;
    CDnsCachePtr dnsCache = *ptrPtr;

    // Set the task cycle interval
    RequestTaskCycleDelayInMs( dnsCache->GetAsyncRefreshIntervalInMs() );
    
    return true;
}

/*-------------------------------------------------------------------------*/

void
CDnsCacheRefreshTaskConsumer::OnTaskEnding( CORE::CICloneable* taskData ,
                                            bool willBeForced           )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CDnsCacheRefreshTaskConsumer::OnTaskEnded( CORE::CICloneable* taskData ,
                                           bool wasForced              )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDnsCacheRefreshTaskConsumer: task finished" );
    CORE::CTaskConsumer::OnTaskEnded( taskData, wasForced );
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheRefreshTaskConsumer::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    CDnsCachePtr* ptrPtr = static_cast< CDnsCachePtr* >( taskData );
    if ( GUCEF_NULL == ptrPtr || (*ptrPtr).IsNULL() )
        return false;
    CDnsCachePtr dnsCache = *ptrPtr;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDnsCacheRefreshTaskConsumer: Starting refresh cycle" );
    dnsCache->Refresh();
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDnsCacheRefreshTaskConsumer: Completed refresh cycle" );

    // Update the task cycle interval
    RequestTaskCycleDelayInMs( dnsCache->GetAsyncRefreshIntervalInMs() );

    // We are never 'done' as this is an on-going background task
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
