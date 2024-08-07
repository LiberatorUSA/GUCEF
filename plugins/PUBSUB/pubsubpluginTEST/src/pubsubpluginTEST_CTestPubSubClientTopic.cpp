/*
 *  pubsubpluginTEST: Generic GUCEF PUBSUB plugin for adding integration/system tests
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

#include <string>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#include "gucefPUBSUB_CBasicPubSubMsg.h"
#define GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#endif /* GUCEF_PUBSUB_CBASICPUBSUBMSG_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENT_H
#include "pubsubpluginTEST_CTestPubSubClient.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENT_H ? */

#include "pubsubpluginTEST_CTestPubSubClientTopic.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace TEST {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CTestPubSubClientTopic::CTestPubSubClientTopic( CTestPubSubClient* client              ,
                                                CORE::PulseGeneratorPtr pulseGenerator )
    : STORAGE::CStoragePubSubClientTopic( client, pulseGenerator )
    , CORE::CTSharedObjCreator< CTestPubSubClientTopic, MT::CMutex >( this )
    , m_hasFakeHealthIssue( false )
    , m_healthyStatusToggleTimer( pulseGenerator )
{GUCEF_TRACE;                     

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CTestPubSubClientTopic::~CTestPubSubClientTopic()
{GUCEF_TRACE;

    Shutdown();
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClientTopic::Shutdown( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_healthyStatusToggleTimer.SetEnabled( false );
    STORAGE::CStoragePubSubClientTopic::Shutdown();
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback2( this, &CTestPubSubClientTopic::OnHealthyStatusToggleTimerCycle );
    SubscribeTo( &m_healthyStatusToggleTimer    ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback2                      );

}

/*-------------------------------------------------------------------------*/

const CTestPubSubClientTopicConfig& 
CTestPubSubClientTopic::GetTopicConfig( void ) const
{GUCEF_TRACE;

    return m_testConfig;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClientTopic::SaveConfig( PUBSUB::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    config = m_testConfig;
    return m_testConfig.SaveCustomConfig( config.customConfig );
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClientTopic::LoadConfig( const PUBSUB::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    if ( CStoragePubSubClientTopic::LoadConfig( config ) )
    {
        MT::CScopeMutex lock( m_lock );

        m_testConfig = m_config;

        m_healthyStatusToggleTimer.SetEnabled( m_testConfig.toggleHealthyStatus );
        CORE::UInt32 jitterMs = 0;
        if ( m_testConfig.healthyStatusToggleIntervalJitter > 0 )
        {
            Float64 jitterFactor = rand() / ( RAND_MAX * 1.0 );
            jitterMs = (CORE::UInt32) ( jitterFactor * (CORE::Float32) m_testConfig.healthyStatusToggleIntervalJitter );
        }

        m_healthyStatusToggleTimer.SetInterval( m_testConfig.healthyStatusToggleIntervalInMs + jitterMs );

        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClientTopic::IsHealthy( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    bool newIsHealthyState = m_testConfig.defaultIsHealthyStatus && !m_hasFakeHealthIssue;

    if ( m_isHealthy != newIsHealthyState )
    {
        m_isHealthy = newIsHealthyState;

        CORE::UInt32 jitterMs = 0;
        if ( m_testConfig.healthyStatusToggleIntervalJitter > 0 )
        {
            Float64 jitterFactor = rand() / ( RAND_MAX * 1.0 );
            jitterMs = (CORE::UInt32) ( jitterFactor * (CORE::Float32) m_testConfig.healthyStatusToggleIntervalJitter );
        }

        m_healthyStatusToggleTimer.SetInterval( m_testConfig.healthyStatusToggleIntervalInMs + jitterMs ); 

        THealthStatusChangeEventData eData( newIsHealthyState ); 
        NotifyObservers( HealthStatusChangeEvent, &eData ); 
    }
    
    return newIsHealthyState;
}

/*-------------------------------------------------------------------------*/

void
CTestPubSubClientTopic::OnHealthyStatusToggleTimerCycle( CORE::CNotifier* notifier    ,
                                                         const CORE::CEvent& eventId  ,
                                                         CORE::CICloneable* eventData )
{GUCEF_TRACE;

    m_hasFakeHealthIssue = !m_hasFakeHealthIssue;
    IsHealthy();
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CTestPubSubClientTopic::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::PUBSUBPLUGIN::TEST::CTestPubSubClientTopic";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace TEST */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
