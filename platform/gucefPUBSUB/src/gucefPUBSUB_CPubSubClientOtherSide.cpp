/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
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

#include <string.h>

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#include "gucefPUBSUB_CPubSubGlobal.h"
#define GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#endif /* GUCEF_PUBSUB_CPUBSUBGLOBAL_H ? */

#ifndef GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#include "gucefPUBSUB_CBasicPubSubMsg.h"
#define GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#endif /* GUCEF_PUBSUB_CBASICPUBSUBMSG_H ? */

#ifndef GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefWEB_CDummyHTTPServerResource.h"
#define GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTCHANNEL_H
#include "gucefPUBSUB_CPubSubClientChannel.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTCHANNEL_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTCHANNEL_H ? */

#include "gucefPUBSUB_CPubSubClientOtherSide.h"

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE                  10000
#define GUCEF_DEFAULT_PUBSUB_RECONNECT_DELAY_IN_MS                  100
#define GUCEF_DEFAULT_PUBSUB_MAX_PUBLISHED_MSG_INFLIGHT_TIME_IN_MS  ( 30 * 1000 )
#define GUCEF_DEFAULT_PUBSUB_SIDE_MAX_IN_FLIGHT                     1000

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubClientOtherSide::CPubSubClientOtherSide( CPubSubClientChannel* parentChannel, char side )
    : CPubSubClientSide( side )
    , m_parentChannel( parentChannel )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientOtherSide::~CPubSubClientOtherSide()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPubSubClientOtherSide::OnTaskEnding( CORE::CICloneable* taskdata ,
                                      bool willBeForced           )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskEnding( taskdata, willBeForced );
    m_parentChannel = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientOtherSide::OnTaskEnded( CORE::CICloneable* taskData ,
                                     bool wasForced              )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskEnded( taskData, wasForced );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientOtherSide::GetAllSides( TPubSubClientSideVector*& sides )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_parentChannel )
        return m_parentChannel->GetAllSides( sides );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientOtherSide::IsTrackingInFlightPublishedMsgsForAcksNeeded( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_parentChannel )
        return m_parentChannel->IsTrackingInFlightPublishedMsgsForAcksNeeded();
    
    // we should be able to access to parent channel always, but if not, better safe then sorry
    return true;    
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientOtherSide::AcknowledgeReceiptForSide( CIPubSubMsg::TNoLockSharedPtr& msg ,
                                                   CPubSubClientSide* msgReceiverSide )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_parentChannel )
    {
        return m_parentChannel->AcknowledgeReceiptForSideImpl( GetDelegatorThreadId() ,
                                                               msg                    ,
                                                               msgReceiverSide        );
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


