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

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H
#include "pubsubpluginTEST_CTestPubSubClientConfig.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTCONFIG_H ? */

#include "pubsubpluginTEST_CTestPubSubClient.h"

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CTestPubSubClient::TypeName = "TEST";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CTestPubSubClient::CTestPubSubClient( const PUBSUB::CPubSubClientConfig& config )
    : STORAGE::CStoragePubSubClient( config )
    , m_testConfig()
{GUCEF_TRACE;

    m_testConfig = m_config;
}

/*-------------------------------------------------------------------------*/

CTestPubSubClient::~CTestPubSubClient()
{GUCEF_TRACE;

    SignalUpcomingDestruction();
}

/*-------------------------------------------------------------------------*/

CTestPubSubClientConfig&
CTestPubSubClient::GetConfig( void )
{GUCEF_TRACE;

    return m_testConfig;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClientTopicPtr
CTestPubSubClient::CreateTopicAccess( PUBSUB::CPubSubClientTopicConfigPtr topicConfig ,
                                      CORE::PulseGeneratorPtr pulseGenerator          )
{GUCEF_TRACE;

    CTestPubSubClientTopicPtr topicAccess;
    {
        MT::CScopeMutex lock( m_lock );

        if ( pulseGenerator.IsNULL() )
            pulseGenerator = m_config.topicPulseGenerator; // none given, use default for topics
        if ( pulseGenerator.IsNULL() )
            pulseGenerator = GetPulseGenerator();          // none present, use client level pulse generator

        topicAccess = static_cast< CORE::CTSharedObjCreator< CTestPubSubClientTopic, MT::CMutex >* >( GUCEF_NEW CTestPubSubClientTopic( this, pulseGenerator ) )->CreateSharedPtr();
        if ( topicAccess->LoadConfig( *topicConfig ) )
        {
            m_topicMap[ topicConfig->topicName ] = topicAccess;
            STORAGE::CStoragePubSubClientTopicPtr storageTopicAccess = topicAccess.StaticCast< STORAGE::CStoragePubSubClientTopic >();
            RegisterTopicEventHandlers( storageTopicAccess );
        }
        else
        {
            topicAccess->Shutdown();
            topicAccess.Unlink();
        }
    }

    if ( !topicAccess.IsNULL() )
    {
        TopicAccessCreatedEventData eData( topicConfig->topicName );
        NotifyObservers( TopicAccessCreatedEvent, &eData );
    }

    return topicAccess;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CTestPubSubClient::GetType( void ) const
{GUCEF_TRACE;

    return TypeName;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.SaveConfig( cfg );
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    CTestPubSubClientConfig parsedCfg;
    if ( parsedCfg.LoadConfig( cfg ) )
    {
        MT::CScopeMutex lock( m_lock );
        m_config = parsedCfg;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClient::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg  )
{GUCEF_TRACE;

    // Try to see if we can properly load the entire config before
    // applying it. If not stick with old config vs corrupt config
    CTestPubSubClientConfig parsedCfg;
    if ( parsedCfg.LoadConfig( cfg ) )
    {
        MT::CScopeMutex lock( m_lock );
        m_config = parsedCfg;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CTestPubSubClient::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::PUBSUBPLUGIN::TEST::CTestPubSubClient";
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

/*--------------------------------------------------------------------------*/
