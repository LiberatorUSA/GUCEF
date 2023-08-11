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

#include "gucefPUBSUB_CPubSubSideChannelSettings.h"

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubSideChannelConfig::CPubSubSideChannelConfig( void )
    : CORE::CIConfigurable()
    , pubsubClientConfig()
    , pubsubBookmarkPersistenceConfig()
    , performPubSubInDedicatedThread( true )                                                            // potentially more performant overall, depends on various factors including threads used in the backend
    , useBackendTopicThreadForTopicIfAvailable( true )                                                  // some backends may have their own per-topic thread, if this is 'true' we will try to hook up to that thread for logic that needs to execute for said specific topic
    , applyThreadCpuAffinity( false )                                                                   // if we don't have a dedicated host this may cause bigger problems so safer to go with false
    , cpuAffinityForPubSubThread( 0 )
    , subscribeWithoutBookmarkIfNoneIsPersisted( true )                                                 // best effort
    , treatPublishWithoutTargetTopicAsBroadcast( true )                                                 // for simpler side to side operation where topic mappings are not provided
    , retryFailedPublishAttempts( true )                                                                // safer default, assume we don't want fire-and-forget but want the extra safegaurds
    , allowOutOfOrderPublishRetry( false )                                                              // safer default, assume we don't want out-of-order messages. this reduces performance if there are publish errors
    , maxMsgPublishRetryAttempts( -1 )                                                                  // safer default is no max nr of publish retries
    , maxMsgPublishRetryTotalTimeInMs( -1 )                                                             // safer default is no max time for publish retries
    , maxPublishedMsgInFlightTimeInMs( GUCEF_DEFAULT_PUBSUB_MAX_PUBLISHED_MSG_INFLIGHT_TIME_IN_MS )     // we want a large but not infinite max time on this as a safe default. ensures we dont run out of memory due to in flight messages
    , allowTimedOutPublishedInFlightMsgsRetryOutOfOrder( true )                                         // even though we dont want to send messages out-of-order here we will allow it to avoid losing messages as the bigger evil
    , maxMsgPublishAckRetryAttempts( -1 )                                                               // safer default is no max nr of ack retries
    , maxMsgPublishAckRetryTotalTimeInMs( -1 )                                                          // safer default is no max time for ack retries
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , collectMetrics( true )
    , metricsIntervalInMs( 1000 )
    , pubsubIdPrefix()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelConfig::CPubSubSideChannelConfig( const CPubSubSideChannelConfig& src )
    : CORE::CIConfigurable( src )
    , pubsubClientConfig( src.pubsubClientConfig )
    , pubsubBookmarkPersistenceConfig( src.pubsubBookmarkPersistenceConfig )
    , performPubSubInDedicatedThread( src.performPubSubInDedicatedThread )
    , useBackendTopicThreadForTopicIfAvailable( src.useBackendTopicThreadForTopicIfAvailable )
    , applyThreadCpuAffinity( src.applyThreadCpuAffinity )
    , cpuAffinityForPubSubThread( src.cpuAffinityForPubSubThread )
    , subscribeWithoutBookmarkIfNoneIsPersisted( src.subscribeWithoutBookmarkIfNoneIsPersisted )
    , treatPublishWithoutTargetTopicAsBroadcast( src.treatPublishWithoutTargetTopicAsBroadcast )
    , retryFailedPublishAttempts( src.retryFailedPublishAttempts )
    , allowOutOfOrderPublishRetry( src.allowOutOfOrderPublishRetry )
    , maxMsgPublishRetryAttempts( src.maxMsgPublishRetryAttempts )
    , maxMsgPublishRetryTotalTimeInMs( src.maxMsgPublishRetryTotalTimeInMs )
    , maxPublishedMsgInFlightTimeInMs( src.maxPublishedMsgInFlightTimeInMs )
    , allowTimedOutPublishedInFlightMsgsRetryOutOfOrder( src.allowTimedOutPublishedInFlightMsgsRetryOutOfOrder )
    , maxMsgPublishAckRetryAttempts( src.maxMsgPublishAckRetryAttempts )
    , maxMsgPublishAckRetryTotalTimeInMs( src.maxMsgPublishAckRetryTotalTimeInMs )
    , ticketRefillOnBusyCycle( src.ticketRefillOnBusyCycle )
    , collectMetrics( src.collectMetrics )
    , metricsIntervalInMs( src.metricsIntervalInMs )
    , pubsubIdPrefix( src.pubsubIdPrefix )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelConfig::~CPubSubSideChannelConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelConfig&
CPubSubSideChannelConfig::operator=( const CPubSubSideChannelConfig& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CORE::CIConfigurable::operator=( src );

        pubsubClientConfig = src.pubsubClientConfig;
        pubsubBookmarkPersistenceConfig = src.pubsubBookmarkPersistenceConfig;
        performPubSubInDedicatedThread = src.performPubSubInDedicatedThread;
        useBackendTopicThreadForTopicIfAvailable = src.useBackendTopicThreadForTopicIfAvailable;
        applyThreadCpuAffinity = src.applyThreadCpuAffinity;
        cpuAffinityForPubSubThread = src.cpuAffinityForPubSubThread;
        subscribeWithoutBookmarkIfNoneIsPersisted = src.subscribeWithoutBookmarkIfNoneIsPersisted;
        treatPublishWithoutTargetTopicAsBroadcast = src.treatPublishWithoutTargetTopicAsBroadcast;
        retryFailedPublishAttempts = src.retryFailedPublishAttempts;
        allowOutOfOrderPublishRetry = src.allowOutOfOrderPublishRetry;
        maxMsgPublishRetryAttempts = src.maxMsgPublishRetryAttempts;
        maxMsgPublishRetryTotalTimeInMs = src.maxMsgPublishRetryTotalTimeInMs;
        maxPublishedMsgInFlightTimeInMs = src.maxPublishedMsgInFlightTimeInMs;
        allowTimedOutPublishedInFlightMsgsRetryOutOfOrder = src.allowTimedOutPublishedInFlightMsgsRetryOutOfOrder;
        maxMsgPublishAckRetryAttempts = src.maxMsgPublishAckRetryAttempts;
        maxMsgPublishAckRetryTotalTimeInMs = src.maxMsgPublishAckRetryTotalTimeInMs;
        ticketRefillOnBusyCycle = src.ticketRefillOnBusyCycle;
        collectMetrics = src.collectMetrics;
        metricsIntervalInMs = src.metricsIntervalInMs;
        pubsubIdPrefix = src.pubsubIdPrefix;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubSideChannelConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    CORE::CDataNode* psClientConfig = cfg.Structure( "PubSubClientConfig", '/' );
    if ( !pubsubClientConfig.SaveConfig( *psClientConfig ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelConfig:SaveConfig: config is malformed, failed to save a mandatory PubSubClientConfig section" );
        return false;
    }

    CORE::CDataNode* psBookmarkPersistenceConfig = cfg.Structure( "PubSubBookmarkPersistenceConfig", '/' );
    if ( !pubsubBookmarkPersistenceConfig.SaveConfig( *psBookmarkPersistenceConfig ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelConfig:SaveConfig: config is malformed, failed to save PubSubBookmarkPersistenceConfig section" );
        return false;
    }    

    totalSuccess = cfg.SetAttribute( "performPubSubInDedicatedThread", performPubSubInDedicatedThread ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "useBackendTopicThreadForTopicIfAvailable", useBackendTopicThreadForTopicIfAvailable ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "applyThreadCpuAffinity", applyThreadCpuAffinity ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "cpuAffinityForPubSubThread", cpuAffinityForPubSubThread ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "subscribeWithoutBookmarkIfNoneIsPersisted", subscribeWithoutBookmarkIfNoneIsPersisted ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "treatPublishWithoutTargetTopicAsBroadcast", treatPublishWithoutTargetTopicAsBroadcast ) && totalSuccess;    
    totalSuccess = cfg.SetAttribute( "retryFailedPublishAttempts", retryFailedPublishAttempts ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "allowOutOfOrderPublishRetry", allowOutOfOrderPublishRetry ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "maxMsgPublishRetryAttempts", maxMsgPublishRetryAttempts ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "maxMsgPublishRetryTotalTimeInMs", maxMsgPublishRetryTotalTimeInMs ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "maxPublishedMsgInFlightTimeInMs", maxPublishedMsgInFlightTimeInMs ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "allowTimedOutPublishedInFlightMsgsRetryOutOfOrder", allowTimedOutPublishedInFlightMsgsRetryOutOfOrder ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "maxMsgPublishAckRetryAttempts", maxMsgPublishAckRetryAttempts ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "maxMsgPublishAckRetryTotalTimeInMs", maxMsgPublishAckRetryTotalTimeInMs ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "ticketRefillOnBusyCycle", ticketRefillOnBusyCycle ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "collectMetrics", collectMetrics ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "metricsIntervalInMs", metricsIntervalInMs ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "pubsubIdPrefix", pubsubIdPrefix ) && totalSuccess;    

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubSideChannelConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    const CORE::CDataNode* psClientConfig = cfg.Search( "PubSubClientConfig", '/', false );
    if ( GUCEF_NULL != psClientConfig )
    {
        if ( !pubsubClientConfig.LoadConfig( *psClientConfig ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelConfig:LoadConfig: config is unacceptable, failed to load mandatory PubSubClientConfig section" );
            return false;
        }

        // There is no sane default of pubsubClientType since it depends on the clients loaded into the application
        // as such this is a mandatory setting to provide
        if ( pubsubClientConfig.pubsubClientType.IsNULLOrEmpty() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelConfig:LoadConfig: config is malformed, \"pubsubClientType\" was not provided" );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelConfig:LoadConfig: config is malformed, a PubSubClientConfig section is mandatory" );
        return false;
    }

    const CORE::CDataNode* psBookmarkPersistenceConfig = cfg.Search( "PubSubBookmarkPersistenceConfig", '/', false );
    if ( GUCEF_NULL != psBookmarkPersistenceConfig )
    {
        if ( !pubsubBookmarkPersistenceConfig.LoadConfig( *psBookmarkPersistenceConfig ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelConfig:LoadConfig: failed to load mandatory PubSubBookmarkPersistenceConfig section" );
            return false;
        }
    }
    else
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelConfig:LoadConfig: a PubSubBookmarkPersistenceConfig section is expected, will use defaults" );
    }

    performPubSubInDedicatedThread = cfg.GetAttributeValueOrChildValueByName( "performPubSubInDedicatedThread" ).AsBool( performPubSubInDedicatedThread, true );
    useBackendTopicThreadForTopicIfAvailable = cfg.GetAttributeValueOrChildValueByName( "useBackendTopicThreadForTopicIfAvailable" ).AsBool( useBackendTopicThreadForTopicIfAvailable, true ); 
    applyThreadCpuAffinity = cfg.GetAttributeValueOrChildValueByName( "applyThreadCpuAffinity" ).AsBool( applyThreadCpuAffinity, true );
    cpuAffinityForPubSubThread = cfg.GetAttributeValueOrChildValueByName( "cpuAffinityForPubSubThread" ).AsUInt32( cpuAffinityForPubSubThread, true );
    subscribeWithoutBookmarkIfNoneIsPersisted = cfg.GetAttributeValueOrChildValueByName( "subscribeWithoutBookmarkIfNoneIsPersisted" ).AsBool( subscribeWithoutBookmarkIfNoneIsPersisted, true );
    treatPublishWithoutTargetTopicAsBroadcast = cfg.GetAttributeValueOrChildValueByName( "treatPublishWithoutTargetTopicAsBroadcast" ).AsBool( treatPublishWithoutTargetTopicAsBroadcast, true );
    retryFailedPublishAttempts = cfg.GetAttributeValueOrChildValueByName( "retryFailedPublishAttempts" ).AsBool( retryFailedPublishAttempts, true );
    allowOutOfOrderPublishRetry = cfg.GetAttributeValueOrChildValueByName( "allowOutOfOrderPublishRetry" ).AsBool( allowOutOfOrderPublishRetry, true );
    maxMsgPublishRetryAttempts = cfg.GetAttributeValueOrChildValueByName( "maxMsgPublishRetryAttempts" ).AsInt32( maxMsgPublishRetryAttempts, true );
    maxMsgPublishRetryTotalTimeInMs = cfg.GetAttributeValueOrChildValueByName( "maxMsgPublishRetryTotalTimeInMs" ).AsInt32( maxMsgPublishRetryTotalTimeInMs, true );
    maxPublishedMsgInFlightTimeInMs = cfg.GetAttributeValueOrChildValueByName( "maxPublishedMsgInFlightTimeInMs" ).AsInt32( maxPublishedMsgInFlightTimeInMs, true );
    allowTimedOutPublishedInFlightMsgsRetryOutOfOrder = cfg.GetAttributeValueOrChildValueByName( "allowTimedOutPublishedInFlightMsgsRetryOutOfOrder" ).AsBool( allowTimedOutPublishedInFlightMsgsRetryOutOfOrder, true );
    ticketRefillOnBusyCycle = cfg.GetAttributeValueOrChildValueByName( "ticketRefillOnBusyCycle" ).AsUInt32( ticketRefillOnBusyCycle, true );
    collectMetrics = cfg.GetAttributeValueOrChildValueByName( "collectMetrics" ).AsBool( collectMetrics, true );
    metricsIntervalInMs = cfg.GetAttributeValueOrChildValueByName( "metricsIntervalInMs" ).AsUInt32( metricsIntervalInMs, true );
    pubsubIdPrefix = cfg.GetAttributeValueOrChildValueByName( "pubsubIdPrefix" ).AsString( pubsubIdPrefix, true );

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CPubSubSideChannelConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::PUBSUB::CPubSubSideChannelConfig";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

CPubSubClientTopicConfigPtr
CPubSubSideChannelConfig::GetTopicConfig( const CORE::CString& topicName ) const
{GUCEF_TRACE;

    CPubSubClientConfig::TPubSubClientTopicConfigPtrVector::const_iterator i = pubsubClientConfig.topics.begin();
    while ( i != pubsubClientConfig.topics.end() )
    {
        if ( topicName == (*i)->topicName )
            return (*i);
        ++i;
    }
    return CPubSubClientTopicConfigPtr();
}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelSettings::CPubSubSideChannelSettings( void )
    : CPubSubSideChannelConfig()
    , needToTrackInFlightPublishedMsgsForAck( false )  // composite cached value: based on backend features plus desired behaviour
    , metricsPrefix()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelSettings::CPubSubSideChannelSettings( const CPubSubSideChannelSettings& src )
    : CPubSubSideChannelConfig( src )
    , needToTrackInFlightPublishedMsgsForAck( src.needToTrackInFlightPublishedMsgsForAck )
    , metricsPrefix( src.metricsPrefix )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelSettings::~CPubSubSideChannelSettings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelSettings&
CPubSubSideChannelSettings::operator=( const CPubSubSideChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CPubSubSideChannelConfig::operator=( src );

        needToTrackInFlightPublishedMsgsForAck = src.needToTrackInFlightPublishedMsgsForAck;
        metricsPrefix = src.metricsPrefix;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubSideChannelSettings::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    totalSuccess = CPubSubSideChannelConfig::SaveConfig( cfg ) && totalSuccess;

    // Derived settings are advisory outputs only meaning we will save them but we wont load them
    totalSuccess = cfg.SetAttribute( "needToTrackInFlightPublishedMsgsForAck", needToTrackInFlightPublishedMsgsForAck ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "metricsPrefix", metricsPrefix ) && totalSuccess;

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CPubSubSideChannelSettings::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::PUBSUB::CPubSubSideChannelSettings";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


