/*
 *  pubsubpluginAWSSQS: Generic GUCEF COMCORE plugin for providing pubsub via AWS's SQS
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

#include <string.h>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENT_H
#include "pubsubpluginAWSSQS_CAwsSqsPubSubClient.h"
#define PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENT_H ? */

#include "pubsubpluginAWSSQS_CAwsSqsPubSubClientTopic.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace AWSSQS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopic::CAwsSqsPubSubClientTopic( CAwsSqsPubSubClient* client )
    : COMCORE::CPubSubClientTopic()
    , m_client( client )
    , m_redisShardHost()
    , m_config()
    , m_lock()
{GUCEF_TRACE;
        
    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopic::~CAwsSqsPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();
}


/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CAwsSqsPubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::Publish( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;

    return true;
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClientTopic::PrepStorageForReadMsgs( CORE::UInt32 msgCount )
{GUCEF_TRACE;

    // reset size, note this does not dealloc the underlying memory
    m_pubsubMsgs.clear();
    TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs.GetData();
    msgRefs.clear();

    if ( msgCount > m_pubsubMsgs.capacity() )
    {
        // Grow the actual storage, this is allowed to become larger than msgCount to limit
        // the nr of dynamic allocations
        m_pubsubMsgs.reserve( msgCount );        
        msgRefs.reserve( msgCount );
    }
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );


    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    Disconnect();
    
    MT::CScopeMutex lock( m_lock );

    return false;
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CAwsSqsPubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CAwsSqsPubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSQS */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
