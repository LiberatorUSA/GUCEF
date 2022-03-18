/*
 *  pubsub2storage: service which transfers between pubsub and storage
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

#ifndef GUCEF_COMCORE_CCOMCOREGLOBAL_H
#include "gucefCOMCORE_CComCoreGlobal.h"
#define GUCEF_COMCORE_CCOMCOREGLOBAL_H
#endif /* GUCEF_COMCORE_CCOMCOREGLOBAL_H ? */

#ifndef GUCEF_COMCORE_CBASICPUBSUBMSG_H
#include "gucefCOMCORE_CBasicPubSubMsg.h"
#define GUCEF_COMCORE_CBASICPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CBASICPUBSUBMSG_H ? */

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

#include "pubsub2pubsub.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <winsock2.h>
#endif

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

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

PubSubSideChannelSettings::PubSubSideChannelSettings( void )
    : CORE::CIConfigurable()
    , pubsubClientConfig()
    , performPubSubInDedicatedThread( true )                                                            // typically the most performant overall
    , applyThreadCpuAffinity( false )                                                                   // if we don't have a dedicated host this may cause bigger problems so safer to go with false
    , cpuAffinityForPubSubThread( 0 )
    , subscribeWithoutBookmarkIfNoneIsPersisted( true )                                                 // best effort
    , retryFailedPublishAttempts( true )                                                                // safer default, assume we don't want fire-and-forget but want the extra safegaurds
    , allowOutOfOrderPublishRetry( false )                                                              // safer default, assume we don't want out-of-order messages. this reduces performance if there are publish errors
    , maxMsgPublishRetryAttempts( -1 )                                                                  // safer default is no max nr of publish retries
    , maxMsgPublishRetryTotalTimeInMs( -1 )                                                             // safer default is no max time for publish retries
    , maxPublishedMsgInFlightTimeInMs( GUCEF_DEFAULT_PUBSUB_MAX_PUBLISHED_MSG_INFLIGHT_TIME_IN_MS )     // we want a large but not infinite max time on this as a safe default. ensures we dont run out of memory due to in flight messages
    , allowTimedOutPublishedInFlightMsgsRetryOutOfOrder( true )                                         // even though we dont want to send messages out-of-order here we will allow it to avoid losing messages as the bigger evil
    , maxMsgPublishAckRetryAttempts( -1 )                                                               // safer default is no max nr of ack retries
    , maxMsgPublishAckRetryTotalTimeInMs( -1 )                                                          // safer default is no max time for ack retries
    , maxTotalMsgsInFlight( GUCEF_DEFAULT_PUBSUB_SIDE_MAX_IN_FLIGHT )                                   // for operating in parallel we dont want this too low but also not too high as it increased admin overhead and depending on other settings (acks etc) also risk
    , needToTrackInFlightPublishedMsgsForAck( false )                                                   // composite cached value: based on backend features plus desired behaviour
    , metricsPrefix()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

PubSubSideChannelSettings::PubSubSideChannelSettings( const PubSubSideChannelSettings& src )
    : CORE::CIConfigurable( src )
    , pubsubClientConfig( src.pubsubClientConfig )
    , performPubSubInDedicatedThread( src.performPubSubInDedicatedThread )
    , applyThreadCpuAffinity( src.applyThreadCpuAffinity )
    , cpuAffinityForPubSubThread( src.cpuAffinityForPubSubThread )
    , subscribeWithoutBookmarkIfNoneIsPersisted( src.subscribeWithoutBookmarkIfNoneIsPersisted )
    , retryFailedPublishAttempts( src.retryFailedPublishAttempts )
    , allowOutOfOrderPublishRetry( src.allowOutOfOrderPublishRetry )
    , maxMsgPublishRetryAttempts( src.maxMsgPublishRetryAttempts )
    , maxMsgPublishRetryTotalTimeInMs( src.maxMsgPublishRetryTotalTimeInMs )
    , maxPublishedMsgInFlightTimeInMs( src.maxPublishedMsgInFlightTimeInMs )
    , allowTimedOutPublishedInFlightMsgsRetryOutOfOrder( src.allowTimedOutPublishedInFlightMsgsRetryOutOfOrder )
    , maxMsgPublishAckRetryAttempts( src.maxMsgPublishAckRetryAttempts )
    , maxMsgPublishAckRetryTotalTimeInMs( src.maxMsgPublishAckRetryTotalTimeInMs )
    , maxTotalMsgsInFlight( src.maxTotalMsgsInFlight )
    , needToTrackInFlightPublishedMsgsForAck( false )
    , metricsPrefix( src.metricsPrefix )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

PubSubSideChannelSettings&
PubSubSideChannelSettings::operator=( const PubSubSideChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CORE::CIConfigurable::operator=( src );

        pubsubClientConfig = src.pubsubClientConfig;
        performPubSubInDedicatedThread = src.performPubSubInDedicatedThread;
        applyThreadCpuAffinity = src.applyThreadCpuAffinity;
        cpuAffinityForPubSubThread = src.cpuAffinityForPubSubThread;
        subscribeWithoutBookmarkIfNoneIsPersisted = src.subscribeWithoutBookmarkIfNoneIsPersisted;
        retryFailedPublishAttempts = src.retryFailedPublishAttempts;
        allowOutOfOrderPublishRetry = src.allowOutOfOrderPublishRetry;
        maxMsgPublishRetryAttempts = src.maxMsgPublishRetryAttempts;
        maxMsgPublishRetryTotalTimeInMs = src.maxMsgPublishRetryTotalTimeInMs;
        maxPublishedMsgInFlightTimeInMs = src.maxPublishedMsgInFlightTimeInMs;
        allowTimedOutPublishedInFlightMsgsRetryOutOfOrder = src.allowTimedOutPublishedInFlightMsgsRetryOutOfOrder;
        maxMsgPublishAckRetryAttempts = src.maxMsgPublishAckRetryAttempts;
        maxMsgPublishAckRetryTotalTimeInMs = src.maxMsgPublishAckRetryTotalTimeInMs;
        maxTotalMsgsInFlight = src.maxTotalMsgsInFlight;

        needToTrackInFlightPublishedMsgsForAck = src.needToTrackInFlightPublishedMsgsForAck;
        metricsPrefix = src.metricsPrefix;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
PubSubSideChannelSettings::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    CORE::CDataNode* psClientConfig = cfg.Structure( "PubSubClientConfig", '/' );
    if ( !pubsubClientConfig.SaveConfig( *psClientConfig ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelSettings:SaveConfig: config is malformed, failed to save a mandatory PubSubClientConfig section" );
        return false;
    }

    cfg.SetAttribute( "performPubSubInDedicatedThread", performPubSubInDedicatedThread );
    cfg.SetAttribute( "applyThreadCpuAffinity", applyThreadCpuAffinity );
    cfg.SetAttribute( "cpuAffinityForPubSubThread", cpuAffinityForPubSubThread );
    cfg.SetAttribute( "subscribeWithoutBookmarkIfNoneIsPersisted", subscribeWithoutBookmarkIfNoneIsPersisted );
    cfg.SetAttribute( "retryFailedPublishAttempts", retryFailedPublishAttempts );
    cfg.SetAttribute( "allowOutOfOrderPublishRetry", allowOutOfOrderPublishRetry );
    cfg.SetAttribute( "maxMsgPublishRetryAttempts", maxMsgPublishRetryAttempts );
    cfg.SetAttribute( "maxMsgPublishRetryTotalTimeInMs", maxMsgPublishRetryTotalTimeInMs );
    cfg.SetAttribute( "maxPublishedMsgInFlightTimeInMs", maxPublishedMsgInFlightTimeInMs );
    cfg.SetAttribute( "allowTimedOutPublishedInFlightMsgsRetryOutOfOrder", allowTimedOutPublishedInFlightMsgsRetryOutOfOrder );
    cfg.SetAttribute( "maxMsgPublishAckRetryAttempts", maxMsgPublishAckRetryAttempts );
    cfg.SetAttribute( "maxMsgPublishAckRetryTotalTimeInMs", maxMsgPublishAckRetryTotalTimeInMs );
    cfg.SetAttribute( "maxTotalMsgsInFlight", maxTotalMsgsInFlight );

    // Derived settings are advisory outputs only meaning we will save them but we wont load them
    cfg.SetAttribute( "needToTrackInFlightPublishedMsgsForAck", needToTrackInFlightPublishedMsgsForAck );
    cfg.SetAttribute( "metricsPrefix", metricsPrefix );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
PubSubSideChannelSettings::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    const CORE::CDataNode* psClientConfig = cfg.Search( "PubSubClientConfig", '/', false );
    if ( GUCEF_NULL != psClientConfig )
    {
        if ( !pubsubClientConfig.LoadConfig( *psClientConfig ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelSettings:LoadConfig: config is unacceptable, failed to load mandatory PubSubClientConfig section" );
            return false;
        }

        // There is no sane default of pubsubClientType since it depends on the clients loaded into the application
        // as such this is a mandatory setting to provide
        if ( pubsubClientConfig.pubsubClientType.IsNULLOrEmpty() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: config is malformed, \"pubsubClientType\" was not provided" );
            return false;
        }

        // We are fully config driven with no programatically defined topics
        // As such the config must have yielded at least 1 topic
        if ( pubsubClientConfig.topics.empty() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: config is malformed, having at least one topic configured for the client section is mandatory" );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: config is malformed, a PubSubClientConfig section is mandatory" );
        return false;
    }

    performPubSubInDedicatedThread = cfg.GetAttributeValueOrChildValueByName( "performPubSubInDedicatedThread" ).AsBool( performPubSubInDedicatedThread, true );
    applyThreadCpuAffinity = cfg.GetAttributeValueOrChildValueByName( "applyThreadCpuAffinity" ).AsBool( applyThreadCpuAffinity, true );
    cpuAffinityForPubSubThread = cfg.GetAttributeValueOrChildValueByName( "cpuAffinityForPubSubThread" ).AsUInt32( cpuAffinityForPubSubThread, true );
    subscribeWithoutBookmarkIfNoneIsPersisted = cfg.GetAttributeValueOrChildValueByName( "subscribeWithoutBookmarkIfNoneIsPersisted" ).AsBool( subscribeWithoutBookmarkIfNoneIsPersisted, true );
    retryFailedPublishAttempts = cfg.GetAttributeValueOrChildValueByName( "retryFailedPublishAttempts" ).AsBool( retryFailedPublishAttempts, true );
    allowOutOfOrderPublishRetry = cfg.GetAttributeValueOrChildValueByName( "allowOutOfOrderPublishRetry" ).AsBool( allowOutOfOrderPublishRetry, true );
    maxMsgPublishRetryAttempts = cfg.GetAttributeValueOrChildValueByName( "maxMsgPublishRetryAttempts" ).AsInt32( maxMsgPublishRetryAttempts, true );
    maxMsgPublishRetryTotalTimeInMs = cfg.GetAttributeValueOrChildValueByName( "maxMsgPublishRetryTotalTimeInMs" ).AsInt32( maxMsgPublishRetryTotalTimeInMs, true );
    maxPublishedMsgInFlightTimeInMs = cfg.GetAttributeValueOrChildValueByName( "maxPublishedMsgInFlightTimeInMs" ).AsInt32( maxPublishedMsgInFlightTimeInMs, true );
    maxTotalMsgsInFlight = cfg.GetAttributeValueOrChildValueByName( "maxTotalMsgsInFlight" ).AsInt64( maxTotalMsgsInFlight, true );
    allowTimedOutPublishedInFlightMsgsRetryOutOfOrder = cfg.GetAttributeValueOrChildValueByName( "allowTimedOutPublishedInFlightMsgsRetryOutOfOrder" ).AsBool( allowTimedOutPublishedInFlightMsgsRetryOutOfOrder, true );

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
PubSubSideChannelSettings::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "pubsub2storage::PubSubSideChannelSettings";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClientTopicConfig*
PubSubSideChannelSettings::GetTopicConfig( const CORE::CString& topicName )
{GUCEF_TRACE;

    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = pubsubClientConfig.topics.begin();
    while ( i != pubsubClientConfig.topics.end() )
    {
        if ( topicName == (*i).topicName )
            return &(*i);
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( void )
    : CORE::CIConfigurable()
    , pubSubSideChannelSettingsMap()
    , channelId( -1 )
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , collectMetrics( true )
    , metricsIntervalInMs( 1000 )
    , metricsPrefix()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( const ChannelSettings& src )
    : CORE::CIConfigurable( src )
    , pubSubSideChannelSettingsMap( src.pubSubSideChannelSettingsMap )
    , channelId( src.channelId )
    , ticketRefillOnBusyCycle( src.ticketRefillOnBusyCycle )
    , collectMetrics( src.collectMetrics )
    , metricsIntervalInMs( src.metricsIntervalInMs )
    , metricsPrefix( src.metricsPrefix )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings&
ChannelSettings::operator=( const ChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        pubSubSideChannelSettingsMap = src.pubSubSideChannelSettingsMap;
        channelId = src.channelId;
        ticketRefillOnBusyCycle = src.ticketRefillOnBusyCycle;
        collectMetrics = src.collectMetrics;
        metricsIntervalInMs = src.metricsIntervalInMs;
        metricsPrefix = src.metricsPrefix;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

PubSubSideChannelSettings*
ChannelSettings::GetPubSubSideSettings( char side )
{GUCEF_TRACE;

    TCharToPubSubSideChannelSettingsMap::iterator i = pubSubSideChannelSettingsMap.find( side );
    if ( i != pubSubSideChannelSettingsMap.end() )
    {
        return &(*i).second;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
ChannelSettings::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    CORE::CDataNode* pubSubSidesCollection = tree.Structure( "PubSubSides", '/' );
    pubSubSidesCollection->SetNodeType( GUCEF_DATATYPE_ARRAY );

    // We don't want to merge from a potential previous save so we wipe what could be
    // a pre-existing collection
    pubSubSidesCollection->DelSubTree();

    TCharToPubSubSideChannelSettingsMap::const_iterator i = pubSubSideChannelSettingsMap.begin();
    while ( i != pubSubSideChannelSettingsMap.end() )
    {
        const PubSubSideChannelSettings& sideSettings = (*i).second;
        CORE::CDataNode* sideSettingsNode = pubSubSidesCollection->AddChild( "PubSubSideChannelSettings" );
        if ( !sideSettings.SaveConfig( *sideSettingsNode ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:SaveConfig: config is malformed, failed to save a mandatory PubSubSideChannelSettings section" );
            return false;
        }
        ++i;
    }

    tree.SetAttribute( "channelId", channelId );
    tree.SetAttribute( "ticketRefillOnBusyCycle", ticketRefillOnBusyCycle );
    tree.SetAttribute( "collectMetrics", collectMetrics );

    // Derived settings are advisory outputs only meaning we will save them but we wont load them
    tree.SetAttribute( "metricsPrefix", metricsPrefix );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
ChannelSettings::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    channelId = cfg.GetAttributeValueOrChildValueByName( "channelId" ).AsInt32( channelId, true );
    ticketRefillOnBusyCycle = cfg.GetAttributeValueOrChildValueByName( "ticketRefillOnBusyCycle" ).AsUInt32( ticketRefillOnBusyCycle, true );
    collectMetrics = cfg.GetAttributeValueOrChildValueByName( "collectMetrics" ).AsBool( collectMetrics, true );
    metricsIntervalInMs = cfg.GetAttributeValueOrChildValueByName( "metricsIntervalInMs" ).AsUInt32( metricsIntervalInMs, true );

    const CORE::CDataNode* pubSubSidesCollection = cfg.Find( "PubSubSides" );
    if ( GUCEF_NULL != pubSubSidesCollection )
    {
        CORE::CDataNode::const_iterator n = pubSubSidesCollection->ConstBegin();
        while ( n != pubSubSidesCollection->ConstEnd() )
        {
            PubSubSideChannelSettings sideSettings;
            if ( !sideSettings.LoadConfig( *(*n) ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: Side config entry failed to load from collection entry" );
                return false;
            }

            // There is no sane default of pubsubClientType since it depends on the clients loaded into the application
            // as such this is a mandatory setting to provide
            if ( sideSettings.pubsubClientConfig.pubsubClientType.IsNULLOrEmpty() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: Side config is malformed, \"pubsubClientType\" was not provided" );
                return false;
            }

            // We are fully config driven with no programatically defined topics
            // As such the config must have yielded at least 1 topic
            if ( sideSettings.pubsubClientConfig.topics.empty() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: Side config is malformed, having at least one topic configured for the client section is mandatory" );
                return false;
            }

            if ( (*n)->GetName().IsNULLOrEmpty() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: Side config is malformed, we need a valid name for the side" );
                return false;
            }

            char sideId = (*n)->GetName()[ 0 ];
            sideSettings.metricsPrefix = metricsPrefix + "side." + sideId + ".";
            pubSubSideChannelSettingsMap[ sideId ] = sideSettings;
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: Side \'" + CORE::CString( sideId ) + "\' config successfully loaded" );

            ++n;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: PubSubSides collection section is mandatory" );
        return false;
    }

    UpdateDerivedSettings();

    return true;
}

/*-------------------------------------------------------------------------*/

void
ChannelSettings::UpdateDerivedSettings( void )
{GUCEF_TRACE;

    metricsPrefix = "channel." + CORE::ToString( channelId ) + ".";

    TCharToPubSubSideChannelSettingsMap::iterator i = pubSubSideChannelSettingsMap.begin();
    while ( i != pubSubSideChannelSettingsMap.end() )
    {
        PubSubSideChannelSettings& sideSettings = (*i).second;

        sideSettings.metricsPrefix = metricsPrefix + "side." + (*i).first + ".";

        ++i;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
ChannelSettings::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "pubsub2storage::ChannelSettings";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::CPubSubClientSide( char side )
    : CORE::CTaskConsumer()
    , m_pubsubClient()
    , m_topics()
    , m_metricsMap()
    , m_channelSettings()
    , m_sideSettings( GUCEF_NULL )
    , m_mailbox()
    , m_metricsTimer( GUCEF_NULL )
    , m_pubsubClientReconnectTimer( GUCEF_NULL )
    , m_timedOutInFlightMessagesCheckTimer( GUCEF_NULL )
    , m_persistance( GUCEF_NULL )
    , m_side( side )
    , m_awaitingFailureReport( false )
    , m_totalMsgsInFlight( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::~CPubSubClientSide()
{GUCEF_TRACE;

    DisconnectPubSubClient( true );

    delete m_timedOutInFlightMessagesCheckTimer;
    m_timedOutInFlightMessagesCheckTimer = GUCEF_NULL;

    delete m_pubsubClientReconnectTimer;
    m_pubsubClientReconnectTimer = GUCEF_NULL;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::TopicLink( void )
    : topic( GUCEF_NULL )
    , currentPublishActionIds()
    , inFlightMsgs()
    , publishFailedMsgs()
    , publishAckdMsgsMailbox()
    , metricFriendlyTopicName()
    , metrics( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::TopicLink( COMCORE::CPubSubClientTopic* t )
    : topic( t )
    , currentPublishActionIds()
    , inFlightMsgs()
    , publishFailedMsgs()
    , publishAckdMsgsMailbox()
    , metricFriendlyTopicName()
    , metrics( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::TopicLink::AddInFlightMsg( CORE::UInt64 publishActionId                ,
                                              COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg )
{GUCEF_TRACE;

    inFlightMsgs[ publishActionId ] = MsgTrackingEntry( publishActionId, msg );
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::TopicLink::AddInFlightMsgs( const COMCORE::CPubSubClientTopic::TPublishActionIdVector& publishActionIds ,
                                               const COMCORE::CPubSubClientTopic::TIPubSubMsgSPtrVector& msgs              ,
                                               bool inFlightDefaultState                                                   )
{GUCEF_TRACE;

    // this variation gets called during async flow
    size_t max = SMALLEST( publishActionIds.size(), msgs.size() );
    if ( publishActionIds.size() != msgs.size() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):TopicLink:AddInFlightMsgs: Nr of publishActionIds (" + CORE::ToString( publishActionIds.size() ) +
            ") does not match Nr of msgs (" + CORE::ToString( msgs.size() ) + "). Will proceed best effort but this will likely cause issues" );
    }

    for ( size_t i=0; i<max; ++i )
    {
        COMCORE::CIPubSubMsg::TNoLockSharedPtr msgPtr( msgs[ i ] );
        inFlightMsgs[ publishActionIds[ i ] ] = MsgTrackingEntry( publishActionIds[ i ], msgPtr, inFlightDefaultState );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::TopicLink::AddInFlightMsgs( const COMCORE::CPubSubClientTopic::TPublishActionIdVector& publishActionIds ,
                                               const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector& msgs               ,
                                               bool inFlightDefaultState                                                   )
{GUCEF_TRACE;

    // this variation gets called during sync flow
    size_t max = SMALLEST( publishActionIds.size(), msgs.size() );
    if ( publishActionIds.size() != msgs.size() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):TopicLink:AddInFlightMsgs: Nr of publishActionIds (" + CORE::ToString( publishActionIds.size() ) +
            ") does not match Nr of msgs (" + CORE::ToString( msgs.size() ) + "). Will proceed best effort but this will likely cause issues" );
    }

    for ( size_t i=0; i<max; ++i )
    {
        // in the sync/blocking flow we have not yet made a copy, lifecycle is for the call chain which doesnt work
        // for keeping longer term references. As such we create a copy of the message here
        COMCORE::CIPubSubMsg::TNoLockSharedPtr msgPtr( static_cast< COMCORE::CIPubSubMsg* >( msgs[ i ]->Clone() ) );
        inFlightMsgs[ publishActionIds[ i ] ] = MsgTrackingEntry( publishActionIds[ i ], msgPtr, inFlightDefaultState );
    }
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::MsgTrackingEntry::MsgTrackingEntry( void )
    : retryCount( 0 )
    , firstPublishAttempt( CORE::CDateTime::Empty )
    , lastPublishAttempt( CORE::CDateTime::Empty )
    , publishActionId( 0 )
    , msg()
    , isInFlight( false )
    , waitingForInFlightConfirmation( false )
    , readyToAckPublishSuccessButAckFailed( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::MsgTrackingEntry::MsgTrackingEntry( CORE::UInt64 publishActionID               ,
                                                                 COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg ,
                                                                 bool isInFlightState                        )
    : retryCount( 0 )
    , firstPublishAttempt( CORE::CDateTime::NowUTCDateTime() )
    , lastPublishAttempt( CORE::CDateTime::Empty )
    , publishActionId( publishActionID )
    , msg( msg )
    , isInFlight( isInFlightState )
    , waitingForInFlightConfirmation( true )
    , readyToAckPublishSuccessButAckFailed( false )
{GUCEF_TRACE;

    lastPublishAttempt = firstPublishAttempt;
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::MsgTrackingEntry::MsgTrackingEntry( const MsgTrackingEntry& src )
    : retryCount( src.retryCount )
    , firstPublishAttempt( src.firstPublishAttempt )
    , lastPublishAttempt( src.lastPublishAttempt )
    , publishActionId( src.publishActionId )
    , msg( src.msg )
    , isInFlight( src.isInFlight )
    , waitingForInFlightConfirmation( src.waitingForInFlightConfirmation )
    , readyToAckPublishSuccessButAckFailed( src.readyToAckPublishSuccessButAckFailed )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::TopicLink::MsgTrackingEntry&
CPubSubClientSide::TopicLink::MsgTrackingEntry::operator=( const MsgTrackingEntry& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        retryCount = src.retryCount;
        firstPublishAttempt = src.firstPublishAttempt;
        lastPublishAttempt = src.lastPublishAttempt;
        publishActionId = src.publishActionId;
        msg = src.msg;
        isInFlight = src.isInFlight;
        waitingForInFlightConfirmation = src.waitingForInFlightConfirmation;
        readyToAckPublishSuccessButAckFailed = src.readyToAckPublishSuccessButAckFailed;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CPubSubClientSide::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );

    TEventCallback callback2( this, &CPubSubClientSide::OnCheckForTimedOutInFlightMessagesTimerCycle );
    SubscribeTo( m_timedOutInFlightMessagesCheckTimer ,
                 CORE::CTimer::TimerUpdateEvent       ,
                 callback2                            );

    TEventCallback callback3( this, &CPubSubClientSide::OnTopicsAccessAutoCreated );
    SubscribeTo( m_pubsubClient.GetPointerAlways()                    ,
                 COMCORE::CPubSubClient::TopicsAccessAutoCreatedEvent ,
                 callback3                                            );

    TEventCallback callback4( this, &CPubSubClientSide::OnTopicsAccessAutoDestroyed );
    SubscribeTo( m_pubsubClient.GetPointerAlways()                      ,
                 COMCORE::CPubSubClient::TopicsAccessAutoDestroyedEvent ,
                 callback4                                              );
    
    if ( GUCEF_NULL != m_pubsubClientReconnectTimer )
    {
        TEventCallback callback( this, &CPubSubClientSide::OnPubSubClientReconnectTimerCycle );
        SubscribeTo( m_pubsubClientReconnectTimer   ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::RegisterTopicEventHandlers( COMCORE::CPubSubClientTopic& topic )
{GUCEF_TRACE;

    TEventCallback callback( this, &CPubSubClientSide::OnPubSubTopicMsgsReceived );
    SubscribeTo( &topic                                         ,
                 COMCORE::CPubSubClientTopic::MsgsRecievedEvent ,
                 callback                                       );

    TEventCallback callback2( this, &CPubSubClientSide::OnPubSubTopicMsgsPublished );
    SubscribeTo( &topic                                          ,
                 COMCORE::CPubSubClientTopic::MsgsPublishedEvent ,
                 callback2                                       );

    TEventCallback callback3( this, &CPubSubClientSide::OnPubSubTopicMsgsPublishFailure );
    SubscribeTo( &topic                                               ,
                 COMCORE::CPubSubClientTopic::MsgsPublishFailureEvent ,
                 callback3                                            );

    TEventCallback callback4( this, &CPubSubClientSide::OnPubSubTopicLocalPublishQueueFull );
    SubscribeTo( &topic                                                  ,
                 COMCORE::CPubSubClientTopic::LocalPublishQueueFullEvent ,
                 callback4                                               );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientSide::GetType( void ) const
{GUCEF_TRACE;

    return "PubSubClientSide";
}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::CPubSubClientSideMetrics::CPubSubClientSideMetrics( void )
    : publishedMsgsInFlight( 0 )
    , publishOrAckFailedMsgs( 0 )
    , lastPublishBatchSize( 0 )
    , queuedReceiveSuccessAcks( 0 )
    , hasSupportForPublishing( false )
    , hasSupportForSubscribing( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::CPubSubClientSideMetrics::CPubSubClientSideMetrics( const CPubSubClientSideMetrics& src )
    : publishedMsgsInFlight( src.publishedMsgsInFlight )
    , publishOrAckFailedMsgs( src.publishOrAckFailedMsgs )
    , lastPublishBatchSize( src.lastPublishBatchSize )
    , queuedReceiveSuccessAcks( src.queuedReceiveSuccessAcks )
    , hasSupportForPublishing( src.hasSupportForPublishing )
    , hasSupportForSubscribing( src.hasSupportForSubscribing )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::CPubSubClientSideMetrics::~CPubSubClientSideMetrics()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const
CPubSubClientSide::StringToPubSubClientSideMetricsMap&
CPubSubClientSide::GetSideMetrics( void ) const
{
    return m_metricsMap;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::UpdateTopicMetrics( TopicLink& topicLink )
{GUCEF_TRACE;

    if ( GUCEF_NULL != topicLink.metrics )
    {
        if ( topicLink.metrics->hasSupportForPublishing )
        {
            topicLink.metrics->publishOrAckFailedMsgs = topicLink.publishFailedMsgs.size();
            topicLink.metrics->lastPublishBatchSize = topicLink.currentPublishActionIds.size();
            topicLink.metrics->publishedMsgsInFlight = topicLink.inFlightMsgs.size();
        }
        if ( topicLink.metrics->hasSupportForSubscribing )
        {
            topicLink.metrics->queuedReceiveSuccessAcks = topicLink.publishAckdMsgsMailbox.AmountOfMail();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                        const CORE::CEvent& eventId  ,
                                        CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        // We update topic metric numbers both on a timer cycle AND at various points
        // in the processing pipeline. The reasoning is that processing under certain load profiles
        // can cause the totality of thread cycle operations to exceed the timer cycle time
        // this would unintentionally reduce the number of metric updates obtained
        TopicLink& topicLink = (*i).second;
        UpdateTopicMetrics( topicLink );

        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnTopicsAccessAutoCreated( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CPubSubClient::TopicsAccessAutoCreatedEventData* eData = static_cast< COMCORE::CPubSubClient::TopicsAccessAutoCreatedEventData* >( eventData );
    if ( GUCEF_NULL != eData && GUCEF_NULL != m_sideSettings )
    {
        COMCORE::CPubSubClientFeatures clientFeatures;
        m_pubsubClient->GetSupportedFeatures( clientFeatures );

        COMCORE::CPubSubClient::PubSubClientTopicSet& topicsAccess = *eData;
        COMCORE::CPubSubClient::PubSubClientTopicSet::iterator i = topicsAccess.begin();
        while ( i != topicsAccess.end() )
        {
            COMCORE::CPubSubClientTopic* tAccess = (*i);
            if ( GUCEF_NULL != tAccess )
            {
                if ( ConfigureTopicLink( *m_sideSettings, *tAccess ) )
                {
                    ConnectPubSubClientTopic( *tAccess, clientFeatures, *m_sideSettings );
                }
            }
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnTopicsAccessAutoDestroyed( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CPubSubClient::TopicsAccessAutoDestroyedEventData* eData = static_cast< COMCORE::CPubSubClient::TopicsAccessAutoDestroyedEventData* >( eventData );
    if ( GUCEF_NULL != eData && GUCEF_NULL != m_sideSettings )
    {
        COMCORE::CPubSubClient::PubSubClientTopicSet& topicsAccess = *eData;
        COMCORE::CPubSubClient::PubSubClientTopicSet::iterator i = topicsAccess.begin();
        while ( i != topicsAccess.end() )
        {
            COMCORE::CPubSubClientTopic* tAccess = (*i);
            if ( GUCEF_NULL != tAccess )
            {
                // @TODO: What to do about in-flight messages etc? Any special action?
                m_topics.erase( tAccess );
            }
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubClientReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                                      const CORE::CEvent& eventId  ,
                                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // stop the timer, reconnect time itself should not count towards the reconnect interval
    m_pubsubClientReconnectTimer->SetEnabled( false );

    // Since the client does not support reconnects we will destructively reconnnect
    // Meaning we wipe out any previous client as we cannot rely on the client implementation
    // properly handling the state transition
    if ( DisconnectPubSubClient( true ) )
    {
        if ( ConnectPubSubClient() )
            return; // no need to resume the timer
    }

    // no joy, start the timer again
    m_pubsubClientReconnectTimer->SetEnabled( false );
}

/*-------------------------------------------------------------------------*/

template < typename TMsgCollection >
bool
CPubSubClientSide::PublishMsgsSync( const TMsgCollection& msgs )
{GUCEF_TRACE;

    CORE::UInt64 totalMsgsInFlight = 0;
    CORE::UInt32 topicsToPublishOn = 0;
    CORE::UInt32 topicsPublishedOn = 0;
    bool publishSuccess = true;
    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        COMCORE::CPubSubClientTopic* topic = topicLink.topic;

        if ( GUCEF_NULL != topic )
        {
            if ( topic->IsPublishingSupported() )
            {
                ++topicsToPublishOn;

                topicLink.currentPublishActionIds.clear();
                if ( topic->Publish( topicLink.currentPublishActionIds, msgs, m_sideSettings->needToTrackInFlightPublishedMsgsForAck ) )
                {
                    if ( m_sideSettings->needToTrackInFlightPublishedMsgsForAck )
                        topicLink.AddInFlightMsgs( topicLink.currentPublishActionIds, msgs, true );

                    ++topicsPublishedOn;
                }
                else
                {
                    publishSuccess = false;

                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):PublishMsgsSync: Failed to publish messages to topic" );

                    if ( m_sideSettings->retryFailedPublishAttempts )
                    {
                        topicLink.AddInFlightMsgs( topicLink.currentPublishActionIds, msgs, false );
                        m_awaitingFailureReport = true;
                    }
                }
            }

            totalMsgsInFlight += topicLink.inFlightMsgs.size();
        }

        UpdateTopicMetrics( topicLink );
        ++i;
    }

    if ( publishSuccess )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):PublishMsgsSync: Successfully published messages to " + CORE::ToString( topicsPublishedOn ) + " topics, " +
            CORE::ToString( topicsToPublishOn ) + " topics available for publishing" );
    }

    m_totalMsgsInFlight = totalMsgsInFlight;
    return publishSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::PublishMsgsASync( const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector& msgs )
{GUCEF_TRACE;

    // Add the messages in bulk to the mailbox. Since we use pointer semantics we are actually
    // Adding the IPubSubMsg* elements since the ref will be dereferenced
    return m_mailbox.AddPtrBulkMail< const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector >( msgs );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::PublishMsgs( const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector& msgs )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );

    if ( GUCEF_NULL != m_sideSettings )
    {
        // If we are running async from other sides we need to check the mailbox
        if ( m_sideSettings->performPubSubInDedicatedThread || m_awaitingFailureReport )
        {
            return PublishMsgsASync( msgs );
        }
        else
        {
            return PublishMsgsSync< const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector >( msgs );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientSide::GetMsgAttributesForLog( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    return "MsgId=\"" + msg.GetMsgId().AsString() + "\", MsgIndex=\"" + msg.GetMsgIndex().AsString() + "\", MsgDateTime=\"" + CORE::ToString( msg.GetMsgDateTime() ) +
            "\", MsgPrimaryPayloadSize=\"" + CORE::ToString( msg.GetPrimaryPayload().ByteSize( false ) ) + "\", MsgNrOfKeyValuePairs=\"" + CORE::ToString( msg.GetKeyValuePairs().size() ) + "\", MsgNrOfMetaDataKeyValuePairs=\"" + CORE::ToString( msg.GetMetaDataKeyValuePairs().size() ) +
            "\", ReceiveActionId=\"" + CORE::ToString( msg.GetReceiveActionId() ) + "\", OriginClientTopic=\"" + ( msg.GetOriginClientTopic() != GUCEF_NULL ?  msg.GetOriginClientTopic()->GetTopicName() : CORE::CString::Empty ) + "\"";
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnCheckForTimedOutInFlightMessagesTimerCycle( CORE::CNotifier* notifier    ,
                                                                 const CORE::CEvent& eventId  ,
                                                                 CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CORE::UInt64 totalMsgsInFlight = 0;
    CORE::CDateTime cycleNowDt = CORE::CDateTime::NowUTCDateTime();
    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        COMCORE::CPubSubClientTopic* topic = topicLink.topic;

        totalMsgsInFlight += topicLink.inFlightMsgs.size();

        if ( GUCEF_NULL != topic && topic->IsPublishingSupported() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):OnCheckForTimedOutInFlightMessagesTimerCycle: Topic " + topic->GetTopicName() + " has " + CORE::ToString( topicLink.inFlightMsgs.size() ) + " messages in flight" );

            TopicLink::TUInt64Set inFlightDiscardList;
            TopicLink::TUInt64ToMsgTrackingEntryMap::iterator m = topicLink.inFlightMsgs.begin();
            while ( m != topicLink.inFlightMsgs.end() )
            {
                TopicLink::MsgTrackingEntry& inFlightEntry = (*m).second;
                if ( !inFlightEntry.msg.IsNULL() )
                {
                    if ( inFlightEntry.isInFlight && inFlightEntry.waitingForInFlightConfirmation )
                    {
                        CORE::Int64 timeInFlightInMs = inFlightEntry.lastPublishAttempt.GetTimeDifferenceInMillisecondsTowards( cycleNowDt );
                        if ( timeInFlightInMs > m_sideSettings->maxPublishedMsgInFlightTimeInMs )
                        {
                            // Since the max time elapsed we no longer consider the message in-flight
                            inFlightEntry.isInFlight = false;

                            if ( m_sideSettings->allowTimedOutPublishedInFlightMsgsRetryOutOfOrder )
                            {
                                // We will act as if we received a notice that publishing failed and will perform the regular
                                // retry routine

                                GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                    "):OnCheckForTimedOutInFlightMessagesTimerCycle: A message on topic " + topic->GetTopicName() + " has exceeded max in-flight time in ms (" + CORE::ToString( timeInFlightInMs ) +
                                    "). Will consider publishing as failed and will retry. First publish attempt was at " + CORE::ToString( inFlightEntry.firstPublishAttempt ) +
                                    " and the last attempt occured at " + CORE::ToString( inFlightEntry.lastPublishAttempt ) +
                                    ". publishActionId=" + CORE::ToString( inFlightEntry.publishActionId ) + ". " +
                                    GetMsgAttributesForLog( *inFlightEntry.msg ) );

                                // If the message has not been retried yet we will bump up the last published time
                                // this prevents the message from being discarded due to exceeding max retry time
                                if ( 0 == inFlightEntry.retryCount )
                                    inFlightEntry.firstPublishAttempt = cycleNowDt;

                                topicLink.publishFailedMsgs.insert( inFlightEntry.publishActionId );
                            }
                            else
                            {
                                inFlightDiscardList.insert( inFlightEntry.publishActionId );

                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                    "):OnCheckForTimedOutInFlightMessagesTimerCycle: A message on topic " + topic->GetTopicName() + " has exceeded max in-flight time in ms (" + CORE::ToString( timeInFlightInMs ) +
                                    ") Since we do not allow out-of-order retry for these the mesage will be lost! First publish attempt was at " + CORE::ToString( inFlightEntry.firstPublishAttempt ) +
                                    " and the last attempt occured at " + CORE::ToString( inFlightEntry.lastPublishAttempt ) +
                                    ". publishActionId=" + CORE::ToString( inFlightEntry.publishActionId ) + ". " +
                                    GetMsgAttributesForLog( *inFlightEntry.msg ) );
                            }
                        }
                    }
                }
                else
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):OnCheckForTimedOutInFlightMessagesTimerCycle: A message tracking entry on topic " + topic->GetTopicName() + " has no message associated. We will discard the entry. publishActionId=" + CORE::ToString( inFlightEntry.publishActionId ) );

                    inFlightDiscardList.insert( inFlightEntry.publishActionId );
                }

                ++m;
            }

            size_t discardedMsgs = inFlightDiscardList.size();
            TopicLink::TUInt64Set::iterator n = inFlightDiscardList.begin();
            while ( n != inFlightDiscardList.end() )
            {
                topicLink.inFlightMsgs.erase( (*n) );
                ++n;
            }
            if ( discardedMsgs > 0 )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):OnCheckForTimedOutInFlightMessagesTimerCycle: For topic " + topic->GetTopicName() + " we discarded a total of " + CORE::ToString( discardedMsgs ) +
                    " messages due to exceeding the max in-flight time allowed which is configured as " + CORE::ToString( m_sideSettings->maxPublishedMsgInFlightTimeInMs ) );
            }
        }

        UpdateTopicMetrics( topicLink );
        ++i;
    }

    m_totalMsgsInFlight = totalMsgsInFlight;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::RetryPublishFailedMsgs( void )
{GUCEF_TRACE;

    CORE::UInt64 totalMsgsInFlight = 0;
    bool publishRetrySuccess = true;
    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        COMCORE::CPubSubClientTopic* topic = topicLink.topic;

        totalMsgsInFlight += topicLink.inFlightMsgs.size();

        if ( GUCEF_NULL != topic )
        {
            if ( !topicLink.publishFailedMsgs.empty() )
            {
                if ( !topic->IsPublishingSupported() )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has " + CORE::ToString( topicLink.publishFailedMsgs.size() ) +
                        " publish failed messages and yet the topic is set as being incapable of publishing. This should never happen! Clearing the failed message as resolving the situation is impossible" );

                    topicLink.publishFailedMsgs.clear();
                    ++i; continue;
                }

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has " + CORE::ToString( topicLink.publishFailedMsgs.size() ) + " messages queued for retry" );

                TopicLink::TUInt64Set inFlightDiscardList;
                TopicLink::TUInt64Set::iterator n = topicLink.publishFailedMsgs.begin();
                while ( n != topicLink.publishFailedMsgs.end() )
                {
                    CORE::UInt64 publishActionId = (*n);
                    TopicLink::TUInt64ToMsgTrackingEntryMap::iterator m = topicLink.inFlightMsgs.find( publishActionId );
                    if ( m != topicLink.inFlightMsgs.end() )
                    {
                        TopicLink::MsgTrackingEntry& retryEntry = (*m).second;
                        if ( !retryEntry.msg.IsNULL() )
                        {
                            if ( !retryEntry.isInFlight )
                            {
                                // Is this a publish retry or a ack retry?
                                if ( retryEntry.readyToAckPublishSuccessButAckFailed )
                                {
                                    // We are retrying a publish success ack for this message

                                    // Check if we are applying a max nr of retries and if we are in violation if so
                                    if ( m_sideSettings->maxMsgPublishAckRetryAttempts >= 0                                    &&
                                            retryEntry.retryCount >= (CORE::UInt32) m_sideSettings->maxMsgPublishAckRetryAttempts )
                                    {
                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has reached max nr of ack retries (" + CORE::ToString( retryEntry.retryCount ) +
                                            ") for acknowledging successfull publishing of a message. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                            " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                            ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                            GetMsgAttributesForLog( *retryEntry.msg ) );

                                        inFlightDiscardList.insert( retryEntry.publishActionId );
                                        n = topicLink.publishFailedMsgs.erase( n );
                                        continue;
                                    }
                                    else
                                    if ( m_sideSettings->maxMsgPublishRetryTotalTimeInMs >= 0 ) // Are we checking for max retry time?
                                    {
                                        CORE::Int64 timeDiffInMs = retryEntry.firstPublishAttempt.GetTimeDifferenceInMillisecondsTowards( retryEntry.lastPublishAttempt );
                                        if ( timeDiffInMs >= m_sideSettings->maxMsgPublishRetryTotalTimeInMs )
                                        {
                                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                                "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has reached amount of ack retry time in ms (" + CORE::ToString( timeDiffInMs ) +
                                                ") for acknowledging successfull publishing of a message. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                                " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                                ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                                GetMsgAttributesForLog( *retryEntry.msg ) );

                                            inFlightDiscardList.insert( retryEntry.publishActionId );
                                            n = topicLink.publishFailedMsgs.erase( n );
                                            continue;
                                        }
                                    }

                                    // If we make it here we are allowed to try again

                                    ++retryEntry.retryCount;
                                    retryEntry.lastPublishAttempt = CORE::CDateTime::NowUTCDateTime();

                                    if ( AcknowledgeReceiptForSide( retryEntry.msg, this ) )
                                    {
                                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Signaled receipt of message acknowledgement. publishActionId=" + CORE::ToString( retryEntry.publishActionId ) +
                                            ". receiveActionId=" + CORE::ToString( retryEntry.msg->GetReceiveActionId() ) + ". Success after " + CORE::ToString( retryEntry.retryCount ) + " ack retry attempts" );

                                        topicLink.inFlightMsgs.erase( m );
                                    }
                                    else
                                    {
                                        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Still failed to signal receipt of message acknowledgement. publishActionId=" + CORE::ToString( retryEntry.publishActionId ) +
                                            ". receiveActionId=" + CORE::ToString( retryEntry.msg->GetReceiveActionId() ) + ". Currently at " + CORE::ToString( retryEntry.retryCount ) + " ack retry attempts" );
                                    }
                                }
                                else
                                {
                                    // We are retrying a backend publish of the message

                                    // Check if we are applying a max nr of retries and if we are in violation if so
                                    if ( m_sideSettings->maxMsgPublishRetryAttempts >= 0                                    &&
                                            retryEntry.retryCount >= (CORE::UInt32) m_sideSettings->maxMsgPublishRetryAttempts )
                                    {
                                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has reached max nr of publish retries (" + CORE::ToString( retryEntry.retryCount ) +
                                            ") for publishing a message. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                            " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                            ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                            GetMsgAttributesForLog( *retryEntry.msg ) );

                                        inFlightDiscardList.insert( retryEntry.publishActionId );
                                        n = topicLink.publishFailedMsgs.erase( n );
                                        continue;
                                    }
                                    else
                                    if ( m_sideSettings->maxMsgPublishRetryTotalTimeInMs >= 0 ) // Are we checking for max retry time?
                                    {
                                        CORE::Int64 timeDiffInMs = retryEntry.firstPublishAttempt.GetTimeDifferenceInMillisecondsTowards( retryEntry.lastPublishAttempt );
                                        if ( timeDiffInMs >= m_sideSettings->maxMsgPublishRetryTotalTimeInMs )
                                        {
                                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                                "):RetryPublishFailedMsgs: Topic " + topic->GetTopicName() + " has reached amount of publish retry time in ms (" + CORE::ToString( timeDiffInMs ) +
                                                ") for publishing a message. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                                " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                                ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                                GetMsgAttributesForLog( *retryEntry.msg ) );

                                            inFlightDiscardList.insert( retryEntry.publishActionId );
                                            n = topicLink.publishFailedMsgs.erase( n );
                                            continue;
                                        }
                                    }

                                    // If we make it here we are allowed to try again

                                    ++retryEntry.retryCount;
                                    retryEntry.lastPublishAttempt = CORE::CDateTime::NowUTCDateTime();

                                    CORE::UInt64 publishActionId = retryEntry.publishActionId;
                                    retryEntry.isInFlight = true;
                                    retryEntry.waitingForInFlightConfirmation = true;
                                    if ( topic->Publish( publishActionId, *retryEntry.msg.GetPointerAlways(), m_sideSettings->needToTrackInFlightPublishedMsgsForAck ) )
                                    {
                                        // It worked this time!

                                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Successfully retried publish of message on topic " + topic->GetTopicName() + " after " + CORE::ToString( retryEntry.retryCount ) +
                                            " retries. First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                            " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                            ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input publishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                            GetMsgAttributesForLog( *retryEntry.msg ) );

                                        n = topicLink.publishFailedMsgs.erase( n );
                                    }
                                    else
                                    {
                                        // Still didnt work :(
                                        publishRetrySuccess = false;

                                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                            "):RetryPublishFailedMsgs: Retry failed of publish of message on topic " + topic->GetTopicName() + ". Retry attempt " + CORE::ToString( retryEntry.retryCount ) +
                                            ". First retry attempt was at " + CORE::ToString( retryEntry.firstPublishAttempt ) +
                                            " and this last attempt was started at " + CORE::ToString( retryEntry.lastPublishAttempt ) +
                                            ". output publishActionId=" + CORE::ToString( publishActionId ) + ". input PublishActionId=" + CORE::ToString( retryEntry.publishActionId ) + ". " +
                                            GetMsgAttributesForLog( *retryEntry.msg ) );

                                        if ( !m_sideSettings->allowOutOfOrderPublishRetry )
                                        {
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                                "):RetryPublishFailedMsgs: A message tracking entry on topic " + topic->GetTopicName() + " has no message associated. We will discard the entry. publishActionId=" + CORE::ToString( retryEntry.publishActionId ) );

                            inFlightDiscardList.insert( retryEntry.publishActionId );
                        }
                    }

                    if ( n != topicLink.publishFailedMsgs.end() )
                        ++n;
                }

                size_t discardedMsgs = inFlightDiscardList.size();
                TopicLink::TUInt64Set::iterator d = inFlightDiscardList.begin();
                while ( d != inFlightDiscardList.end() )
                {
                    topicLink.inFlightMsgs.erase( (*d) );
                    ++d;
                }
                if ( discardedMsgs > 0 )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):RetryPublishFailedMsgs: For topic " + topic->GetTopicName() + " we discarded a total of " + CORE::ToString( discardedMsgs ) +
                        " messages due to exceeding the max retry attempts and/or sanity checks" );
                }
            }
        }
        UpdateTopicMetrics( topicLink );
        ++i;
    }

     m_totalMsgsInFlight = totalMsgsInFlight;
    return publishRetrySuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::PublishMailboxMsgs( void )
{GUCEF_TRACE;

    CORE::Int32 maxMailItemsToGrab = -1;
    if ( m_sideSettings->maxTotalMsgsInFlight > 0 )
    {
        CORE::Int64 remainingForFlight = m_sideSettings->maxTotalMsgsInFlight - m_totalMsgsInFlight;
        if ( remainingForFlight > 0 && remainingForFlight < GUCEF_MT_INT32MAX )
            maxMailItemsToGrab = (CORE::Int32) remainingForFlight;
    }

    COMCORE::CPubSubClientTopic::TIPubSubMsgSPtrVector msgs;
    if ( m_mailbox.GetSPtrBulkMail( msgs, maxMailItemsToGrab ) )
    {
        bool publishResult = PublishMsgsSync< COMCORE::CPubSubClientTopic::TIPubSubMsgSPtrVector >( msgs );
        return publishResult;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubTopicLocalPublishQueueFull( CORE::CNotifier* notifier    ,
                                                       const CORE::CEvent& eventId  ,
                                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COMCORE::CPubSubClientTopic* topic = static_cast< COMCORE::CPubSubClientTopic* >( notifier );

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
        "):OnPubSubTopicLocalPublishQueueFull: Topic=" + topic->GetTopicName() );

    TopicMap::iterator i = m_topics.find( topic );
    if ( i != m_topics.end() )
        UpdateTopicMetrics( (*i).second );
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubTopicMsgsReceived( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL == eventData )
        return;

    try
    {
        const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector& msgs = *static_cast< COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector* >( eventData );
        if ( !msgs.empty() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                "):OnPubSubTopicMsgsReceived: Received " + CORE::ToString( msgs.size() ) + " message(s)" );

            // We now broadcast the received messages to all other sides which is the purpose of this service
            TPubSubClientSideVector* sides = GUCEF_NULL;
            if ( GetAllSides( sides ) && GUCEF_NULL != sides )
            {
                TPubSubClientSideVector::iterator i = sides->begin();
                while ( i != sides->end() )
                {
                    CPubSubClientSide* side = (*i);
                    if ( this != side )
                    {
                        if ( (*i)->PublishMsgs( msgs ) )
                        {
                            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                "):OnPubSubTopicMsgsReceived: Successfully published message(s) to side " + (*i)->m_side );
                        }
                        else
                        {
                            // We will rely on that side's retry logic
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                "):OnPubSubTopicMsgsReceived: Failed to publish (some?) message(s) to side " + (*i)->m_side );
                        }
                    }
                    ++i;
                }
            }
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) + "):OnPubSubTopicMsgsReceived: exception: " + CORE::CString( e.what() ) );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::ProcessAcknowledgeReceiptsMailbox( void )
{GUCEF_TRACE;

    TopicMap::iterator i = m_topics.begin();
    while ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;

        COMCORE::CIPubSubMsg::TNoLockSharedPtr msg;
        while ( topicLink.publishAckdMsgsMailbox.GetMail( msg ) )
        {
            AcknowledgeReceiptSync( msg );
        }

        UpdateTopicMetrics( topicLink );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::AcknowledgeReceiptSync( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg )
{GUCEF_TRACE;

    return msg->GetOriginClientTopic()->AcknowledgeReceipt( *msg );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::AcknowledgeReceiptASync( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg )
{GUCEF_TRACE;

    TopicMap::iterator i = m_topics.find( msg->GetOriginClientTopic() );
    if ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;
        topicLink.publishAckdMsgsMailbox.AddMail( msg );
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubTopicMsgsPublished( CORE::CNotifier* notifier    ,
                                               const CORE::CEvent& eventId  ,
                                               CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL == eventData || GUCEF_NULL == notifier || !m_sideSettings->needToTrackInFlightPublishedMsgsForAck )
        return;

    const COMCORE::CPubSubClientTopic::TMsgsPublishedEventData& eData = *static_cast< COMCORE::CPubSubClientTopic::TMsgsPublishedEventData* >( eventData );
    const COMCORE::CPubSubClientTopic::TPublishActionIdVector* publishActionIds = eData;
    COMCORE::CPubSubClientTopic* topic = static_cast< COMCORE::CPubSubClientTopic* >( notifier );

    // Here we translate the publish action IDs back into the original messages
    // Subsequently we use said original messages to ack that to the message origin that we received the message
    // This is what allows us to provide a garanteed handling garantee since its an explicit handoff all the way through

    TopicMap::iterator i = m_topics.find( topic );
    if ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;

        COMCORE::CPubSubClientTopic::TPublishActionIdVector::const_iterator n = publishActionIds->begin();
        while ( n != publishActionIds->end() )
        {
            TopicLink::TUInt64ToMsgTrackingEntryMap::iterator m = topicLink.inFlightMsgs.find( (*n) );
            if ( m != topicLink.inFlightMsgs.end() )
            {
                TopicLink::MsgTrackingEntry& msgTrackingEntry = (*m).second;
                msgTrackingEntry.isInFlight = false;
                msgTrackingEntry.waitingForInFlightConfirmation = false;
                COMCORE::CIPubSubMsg::TNoLockSharedPtr msg = msgTrackingEntry.msg;

                if ( AcknowledgeReceiptForSide( msg, this ) )
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                        "):OnPubSubTopicMsgsPublished: Signaled receipt of message acknowledgement. publishActionId=" + CORE::ToString( msgTrackingEntry.publishActionId ) +
                        ". receiveActionId=" + CORE::ToString( msg->GetReceiveActionId() ) );

                    topicLink.inFlightMsgs.erase( m );
                    if ( m_totalMsgsInFlight > 0 )
                        --m_totalMsgsInFlight;
                }
                else
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):OnPubSubTopicMsgsPublished: Failed to signal receipt of message acknowledgement of message on topic " + topic->GetTopicName() +
                        ". Will reset entry for PublishAck retries. Publish details: retryCount=" + CORE::ToString( msgTrackingEntry.retryCount ) +
                        ". First publish attempt was at " + CORE::ToString( msgTrackingEntry.firstPublishAttempt ) +
                        " and this last attempt was successfull at " + CORE::ToString( msgTrackingEntry.lastPublishAttempt ) +
                        ". publishActionId=" + CORE::ToString( msgTrackingEntry.publishActionId ) + ". " +
                        GetMsgAttributesForLog( *msgTrackingEntry.msg ) );

                    // Configure the entry for a different type of retry: Ack retry
                    // This is a different retry and as such we reset the retry count and time, the log message above will provide the original values for tracing
                    msgTrackingEntry.readyToAckPublishSuccessButAckFailed = true;
                    msgTrackingEntry.retryCount = 0;
                    msgTrackingEntry.firstPublishAttempt = CORE::CDateTime::NowUTCDateTime();
                    msgTrackingEntry.lastPublishAttempt = msgTrackingEntry.firstPublishAttempt;

                    topicLink.publishFailedMsgs.insert( msgTrackingEntry.publishActionId );
                }
            }
            else
            {
                // This should not happen
                // Only understandable reasons are bad eventing from the backend or if the in-flight time-out is configured too aggressively
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnPubSubTopicMsgsPublished: Failed to locate original in-flight message related to publishActionId " + CORE::ToString( (*n) ) );
            }
            ++n;
        }

        UpdateTopicMetrics( topicLink );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnPubSubTopicMsgsPublishFailure( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventId  ,
                                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL == eventData || GUCEF_NULL == notifier || !m_sideSettings->retryFailedPublishAttempts )
        return;

    const COMCORE::CPubSubClientTopic::TMsgsPublishedEventData& eData = *static_cast< COMCORE::CPubSubClientTopic::TMsgsPublishedEventData* >( eventData );
    const COMCORE::CPubSubClientTopic::TPublishActionIdVector* publishActionIds = eData;
    COMCORE::CPubSubClientTopic* topic = static_cast< COMCORE::CPubSubClientTopic* >( notifier );

    // Here we translate the publish action IDs back into the original messages
    // Subsequently we use said original messages to set up the retry mechanism

    TopicMap::iterator i = m_topics.find( topic );
    if ( i != m_topics.end() )
    {
        TopicLink& topicLink = (*i).second;

        COMCORE::CPubSubClientTopic::TPublishActionIdVector::const_iterator n = publishActionIds->begin();
        while ( n != publishActionIds->end() )
        {
            CORE::UInt64 publishActionId = (*n);
            TopicLink::TUInt64ToMsgTrackingEntryMap::iterator m = topicLink.inFlightMsgs.find( publishActionId );
            if ( m != topicLink.inFlightMsgs.end() )
            {
                TopicLink::MsgTrackingEntry& msgTrackingEntry = (*m).second;
                msgTrackingEntry.isInFlight = false;
                msgTrackingEntry.waitingForInFlightConfirmation = false;
                msgTrackingEntry.readyToAckPublishSuccessButAckFailed = false;
                topicLink.publishFailedMsgs.insert( publishActionId );

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnPubSubTopicMsgsPublishFailure: Message with publishActionId of " + CORE::ToString( publishActionId ) + " queued for retry, publish failed" +
                    ". receiveActionId=" + CORE::ToString( msgTrackingEntry.msg->GetReceiveActionId() ) );
            }
            else
            {
                // This should not happen
                // Only understandable reasons are bad eventing from the backend or if the in-flight time-out is configured too aggressively
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnPubSubTopicMsgsPublishFailure: Failed to locate original in-flight message related to publishActionId " + CORE::ToString( (*n) ) );
            }
            ++n;
        }

        UpdateTopicMetrics( topicLink );
    }

    m_awaitingFailureReport = false;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientSide::GenerateMetricsFriendlyTopicName( const CORE::CString& topicName )
{GUCEF_TRACE;

    // Let's avoid non-ASCII stumbling blocks and force the down to ASCII
    CORE::CAsciiString asciiMetricsFriendlyTopicName = topicName.ForceToAscii( '_' );

    // Replace special chars
    static const char disallowedChars[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '=', '+', ',', '.', '<', '>', '/', '?', '`', '~', '\\', '|', '{', '}', '[', ']', ';', ':', '\'', '\"' };
    asciiMetricsFriendlyTopicName = asciiMetricsFriendlyTopicName.ReplaceChars( disallowedChars, sizeof(disallowedChars)/sizeof(char), '_' );

    // Back to the platform wide string convention format
    CORE::CString metricsFriendlyTopicName = CORE::ToString( asciiMetricsFriendlyTopicName );
    return metricsFriendlyTopicName;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::DisconnectPubSubClient( bool destroyClient )
{GUCEF_TRACE;

    if ( m_pubsubClient.IsNULL() )
        return true;

    if ( !m_pubsubClient->Disconnect() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClient: Failed to disconnect the pub-sub client" );
        return false;
    }

    COMCORE::CPubSubClientFeatures clientFeatures;
    m_pubsubClient->GetSupportedFeatures( clientFeatures );

    if ( destroyClient || !clientFeatures.supportsAutoReconnect )
    {
        m_pubsubClient.Unlink();
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::ConfigureTopicLink( const PubSubSideChannelSettings& pubSubSideSettings ,
                                       COMCORE::CPubSubClientTopic& topic                  )
{GUCEF_TRACE;

    RegisterTopicEventHandlers( topic );

    TopicLink& topicLink = m_topics[ &topic ];
    topicLink.topic = &topic;
    topicLink.metricFriendlyTopicName = pubSubSideSettings.metricsPrefix + "topic." + GenerateMetricsFriendlyTopicName( topic.GetTopicName() ) + ".";
    topicLink.metrics = &m_metricsMap[ topicLink.metricFriendlyTopicName ];
    topicLink.metrics->hasSupportForPublishing = topic.IsPublishingSupported();
    topicLink.metrics->hasSupportForSubscribing = topic.IsSubscribingSupported();

    UpdateTopicMetrics( topicLink );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::ConnectPubSubClientTopic( COMCORE::CPubSubClientTopic& topic                   ,
                                             const COMCORE::CPubSubClientFeatures& clientFeatures ,
                                             const PubSubSideChannelSettings& pubSubSideSettings  )
{GUCEF_TRACE;
    
    if ( topic.InitializeConnectivity() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClientTopic: Successfully requested connectivity initialization for topic \"" + topic.GetTopicName() + "\". Proceeding" );

        // We use the 'desired' feature to also drive whether we actually subscribe at this point
        // saves us an extra setting
        const COMCORE::CPubSubClientTopicConfig* topicConfig = m_pubsubClient->GetTopicConfig( topic.GetTopicName() );
        if ( GUCEF_NULL != topicConfig && topicConfig->needSubscribeSupport )
        {
            // The method of subscription depends on the supported feature set
            bool subscribeSuccess = false;
            if ( !clientFeatures.supportsBookmarkingConcept ) // We have no control bookmark wise with this backend, just subscribe and hope for the best
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ConnectPubSubClientTopic: Bookmarking concept is not supported by the backend, we will attempt to subscribe as-is" );

                subscribeSuccess = topic.Subscribe();
            }
            else
            if ( clientFeatures.supportsServerSideBookmarkPersistance ) // first preference is always backend managed bookmarking if available
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ConnectPubSubClientTopic: Bookmarking concept is natively supported and managed by the backend independently and we will attempt to subscribe as such" );

                subscribeSuccess = topic.Subscribe();
            }
            else
            {
                // bookmarks are supported but they rely on client-side persistance
                // we will need to obtain said bookmark

                COMCORE::CPubSubBookmark bookmark;
                if ( GUCEF_NULL == m_persistance || !m_persistance->GetPersistedBookmark( m_channelSettings.channelId, topic.GetTopicName(), bookmark ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                        "):ConnectPubSubClientTopic: Bookmarking concept is supported by the backend via a client-side message index marker but we failed at obtaining the last used message index" );

                    if ( pubSubSideSettings.subscribeWithoutBookmarkIfNoneIsPersisted )
                    {
                        subscribeSuccess = topic.Subscribe();
                        if ( !subscribeSuccess )
                        {
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                                "):ConnectPubSubClientTopic: Also unable to subscribe using the default bookmark as a fallback" );
                            return false;
                        }
                    }
                    else
                        return false;
                }
                else
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                        "):ConnectPubSubClientTopic: Bookmarking concept is supported by the backend via a client-side bookmark. Bookmark type=" + CORE::ToString( bookmark.GetBookmarkType() ) + ". Bookmark=" + bookmark.GetBookmarkData().AsString() );

                    subscribeSuccess = topic.SubscribeStartingAtBookmark( bookmark );
                }
            }

            if ( !subscribeSuccess )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ConnectPubSubClient: Failed to subscribe to topic: " + topic.GetTopicName() );
                return false;
            }
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::ConnectPubSubClient( void )
{GUCEF_TRACE;

    if ( !DisconnectPubSubClient() )
        return false;

    if ( GUCEF_NULL == m_sideSettings )
    {
        PubSubSideChannelSettings* pubSubSideSettings = m_channelSettings.GetPubSubSideSettings( m_side );
        if ( GUCEF_NULL == pubSubSideSettings )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                "):ConnectPubSubClient: Unable to obtain settings for configured side \"" + m_side + "\". Cannot proceed" );
            return false;
        }
        m_sideSettings = pubSubSideSettings;
    }
    COMCORE::CPubSubClientConfig& pubSubConfig = m_sideSettings->pubsubClientConfig;

    if ( m_pubsubClient.IsNULL() )
    {
        // Create and configure the pub-sub client
        pubSubConfig.pulseGenerator = GetPulseGenerator();
        pubSubConfig.metricsPrefix = m_sideSettings->metricsPrefix;
        m_pubsubClient = COMCORE::CComCoreGlobal::Instance()->GetPubSubClientFactory().Create( pubSubConfig.pubsubClientType, pubSubConfig );

        if ( m_pubsubClient.IsNULL() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                "):ConnectPubSubClient: Failed to create a pub-sub client of type \"" + pubSubConfig.pubsubClientType + "\". Cannot proceed" );
            return false;
        }

        // Link the client back to this object
        // This allows getting all the way from:
        //      a message -> a topic -> a client -> a pubsub side
        m_pubsubClient->SetOpaqueUserData( this );
    }

    COMCORE::CPubSubClientFeatures clientFeatures;
    m_pubsubClient->GetSupportedFeatures( clientFeatures );

    if ( !clientFeatures.supportsAutoReconnect )
    {
        if ( GUCEF_NULL != m_pubsubClientReconnectTimer )
            m_pubsubClientReconnectTimer = new CORE::CTimer( *GetPulseGenerator(), pubSubConfig.reconnectDelayInMs );
    }

    // Whether we need to track successfull message handoff (garanteed handling) depends both on whether we want that extra reliability per the config
    // (optional since nothing is free and this likely degrades performance a bit) but also whether the backend even supports it.
    // If the backend doesnt support it all we will be able to do between the sides is fire-and-forget
    m_sideSettings->needToTrackInFlightPublishedMsgsForAck = pubSubConfig.desiredFeatures.supportsSubscriberMsgReceivedAck && clientFeatures.supportsSubscriberMsgReceivedAck;

    if ( !m_pubsubClient->Connect() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClient: Failed to connect the pub-sub client" );
        return false;
    }

    // Create and configure the pub-sub client's topics
    m_topics.clear();
    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = pubSubConfig.topics.begin();
    while ( i != pubSubConfig.topics.end() )
    {
        COMCORE::CPubSubClient::PubSubClientTopicSet topicAccess;
        if ( m_pubsubClient->CreateMultiTopicAccess( (*i), topicAccess ) )
        {
            COMCORE::CPubSubClient::PubSubClientTopicSet::iterator a = topicAccess.begin();
            while ( a != topicAccess.end() )
            {            
                COMCORE::CPubSubClientTopic* topic = (*a);
                if ( GUCEF_NULL == topic )
                {
                    if ( !(*i).isOptional )
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                            "):ConnectPubSubClient: Failed to create a pub-sub client topic access for topic \"" + (*i).topicName + "\". Cannot proceed" );
                        return false;
                    }
                    else
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                            "):ConnectPubSubClient: Unable to create a pub-sub client topic access for optional topic \"" + (*i).topicName + "\". Proceeding" );
                    }
                }
                else
                {
                    ConfigureTopicLink( *m_sideSettings, *topic );
                }
                ++a;
            }
        }
        ++i;
    }

    bool totalTopicConnectSuccess = true;
    TopicMap::iterator t = m_topics.begin();
    while ( t != m_topics.end() )
    {
        TopicLink& topicLink = (*t).second;
        COMCORE::CPubSubClientTopic* topic = topicLink.topic;
        
        totalTopicConnectSuccess = ConnectPubSubClientTopic( *topicLink.topic ,
                                                             clientFeatures   ,
                                                             *m_sideSettings  ) && totalTopicConnectSuccess;

        ++t;
    }
    return totalTopicConnectSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    PubSubSideChannelSettings* pubSubSideSettings = m_channelSettings.GetPubSubSideSettings( m_side );
    if ( GUCEF_NULL == pubSubSideSettings )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):OnTaskStart: Unable to obtain settings for configured side \"" + m_side + "\". Cannot proceed" );
        return false;
    }
    m_sideSettings = pubSubSideSettings;

    COMCORE::CPubSubClientConfig& pubSubConfig = pubSubSideSettings->pubsubClientConfig;

    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), m_channelSettings.metricsIntervalInMs );
    m_metricsTimer->SetEnabled( m_channelSettings.collectMetrics );

    m_timedOutInFlightMessagesCheckTimer = new CORE::CTimer( *GetPulseGenerator(), (CORE::UInt32) m_sideSettings->maxPublishedMsgInFlightTimeInMs );
    m_timedOutInFlightMessagesCheckTimer->SetEnabled( m_sideSettings->maxPublishedMsgInFlightTimeInMs > 0 );

    if ( pubSubSideSettings->performPubSubInDedicatedThread )
    {
        // Set the minimum number of cycles we will go full speed if a single cycle was not enough to handle
        // all the processing. This will cause a bypass of CPU yielding if/when the situation arises.
        // In such a case the thread will run at max speed for a least the below set nr of cycles.
        GetPulseGenerator()->RequestPulsesPerImmediatePulseRequest( m_channelSettings.ticketRefillOnBusyCycle );

        // Default smallest pulse delta at 25ms
        GetPulseGenerator()->RequestPeriodicPulses( this, 25 );

        if ( pubSubSideSettings->applyThreadCpuAffinity )
        {
            if ( SetCpuAffinityByCpuId( pubSubSideSettings->cpuAffinityForPubSubThread ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Successfully set a CPU affinity for logical CPU " + CORE::UInt32ToString( pubSubSideSettings->cpuAffinityForPubSubThread ) );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Failed to set a CPU affinity for logical CPU " + CORE::UInt32ToString( pubSubSideSettings->cpuAffinityForPubSubThread ) +
                    ". Proceeding without affinity");
            }
        }
    }

    if ( !ConnectPubSubClient() )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):OnTaskStart: Failed initial connection attempt on task start, will rely on auto-reconnect" );
    }

    RegisterEventHandlers();

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::IsRunningInDedicatedThread( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_sideSettings )
        return m_sideSettings->performPubSubInDedicatedThread;
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_sideSettings )
    {
        // If we are running async from other sides we need to check the mailbox
        if ( m_sideSettings->performPubSubInDedicatedThread )
        {
            ProcessAcknowledgeReceiptsMailbox();

            // Check if we have a max for messages in-flight, if so dont try to process any more messages right now
            if ( m_sideSettings->maxTotalMsgsInFlight <= 0 || m_totalMsgsInFlight < (CORE::UInt64) m_sideSettings->maxTotalMsgsInFlight )
            {
                if ( !m_awaitingFailureReport )
                {
                    if ( RetryPublishFailedMsgs() || m_sideSettings->allowOutOfOrderPublishRetry )
                        PublishMailboxMsgs();
                }
            }
        }
    }

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnTaskEnding( CORE::CICloneable* taskdata ,
                                 bool willBeForced           )
{GUCEF_TRACE;

    CORE::CTaskConsumer::OnTaskEnding( taskdata, willBeForced );

    if ( willBeForced )
    {
        // reduce memory leaks
        m_mailbox.Clear();
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnTaskEnded( CORE::CICloneable* taskData ,
                                bool wasForced              )
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    delete m_pubsubClientReconnectTimer;
    m_pubsubClientReconnectTimer = GUCEF_NULL;

    delete m_timedOutInFlightMessagesCheckTimer;
    m_timedOutInFlightMessagesCheckTimer = GUCEF_NULL;

    CORE::CTaskConsumer::OnTaskEnded( taskData, wasForced );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::LoadConfig( const ChannelSettings& channelSettings )
{GUCEF_TRACE;

    m_channelSettings = channelSettings;

    PubSubSideChannelSettings* pubSubSideSettings = m_channelSettings.GetPubSubSideSettings( m_side );
    if ( GUCEF_NULL == pubSubSideSettings )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientSide(" + CORE::PointerToString( this ) +
            "):LoadConfig: Unable to obtain settings for configured side \"" + m_side + "\". Cannot proceed" );
        return false;
    }

    m_sideSettings = pubSubSideSettings;
    return true;
}

/*-------------------------------------------------------------------------*/

const ChannelSettings&
CPubSubClientSide::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}

/*-------------------------------------------------------------------------*/

PubSubSideChannelSettings*
CPubSubClientSide::GetSideSettings( void )
{GUCEF_TRACE;

    return m_sideSettings;
}

/*-------------------------------------------------------------------------*/

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
CPubSubClientOtherSide::AcknowledgeReceiptForSide( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg ,
                                                   CPubSubClientSide* msgReceiverSide          )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_parentChannel )
    {
        return m_parentChannel->AcknowledgeReceiptForSideImpl( GetDelegatorThreadId() ,
                                                               msg                    ,
                                                               msgReceiverSide        );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CPubSubClientChannel::CPubSubClientChannel( void )
    : CPubSubClientSide( 'A' )
    , m_sideBPubSub()
    , m_sides()
{GUCEF_TRACE;

    // for now just work with a and b, we can do more sides later
    m_sideBPubSub = CPubSubClientOtherSidePtr( new CPubSubClientOtherSide( this, 'B' ) );

    m_sides.push_back( this );
    m_sides.push_back( m_sideBPubSub.GetPointerAlways() );
}

/*-------------------------------------------------------------------------*/

CPubSubClientChannel::~CPubSubClientChannel()
{GUCEF_TRACE;

    m_sideBPubSub.Unlink();
    m_sides.clear();
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::GetAllSides( TPubSubClientSideVector*& sides )
{GUCEF_TRACE;

    sides = &m_sides;
    return true;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::PublishChannelMetrics( void ) const
{GUCEF_TRACE;

    TPubSubClientSideVector::const_iterator i = m_sides.begin();
    while ( i != m_sides.end() )
    {
        const StringToPubSubClientSideMetricsMap& sideMetrics = (*i)->GetSideMetrics();
        StringToPubSubClientSideMetricsMap::const_iterator n = sideMetrics.begin();
        while ( n != sideMetrics.end() )
        {
            const CORE::CString& metricFriendlyTopicName = (*n).first;
            const CPubSubClientSideMetrics& metrics = (*n).second;

            if ( metrics.hasSupportForPublishing )
            {
                GUCEF_METRIC_GAUGE( metricFriendlyTopicName + "publishedMsgsInFlight", metrics.publishedMsgsInFlight, 1.0f );
                GUCEF_METRIC_GAUGE( metricFriendlyTopicName + "publishOrAckFailedMsgs", metrics.publishOrAckFailedMsgs, 1.0f );
                GUCEF_METRIC_GAUGE( metricFriendlyTopicName + "lastPublishBatchSize", metrics.lastPublishBatchSize, 1.0f );
            }
            if ( metrics.hasSupportForSubscribing )
            {
                GUCEF_METRIC_GAUGE( metricFriendlyTopicName + "queuedReceiveSuccessAcks", metrics.queuedReceiveSuccessAcks, 1.0f );
            }
            ++n;
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::AcknowledgeReceiptForSide( COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg ,
                                                 CPubSubClientSide* msgReceiverSide          )
{GUCEF_TRACE;

    return AcknowledgeReceiptForSideImpl( GetDelegatorThreadId(), msg, msgReceiverSide );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::AcknowledgeReceiptForSideImpl( CORE::UInt32 invokerThreadId                ,
                                                     COMCORE::CIPubSubMsg::TNoLockSharedPtr& msg ,
                                                     CPubSubClientSide* msgReceiverSide          )
{GUCEF_TRACE;

    // if we only have 2 sides, no need for anything more complicated
    if ( m_sides.size() <= 2 )
    {
        COMCORE::CPubSubClientTopic* originTopic = msg->GetOriginClientTopic();
        if ( GUCEF_NULL != originTopic )
        {
            COMCORE::CPubSubClient* originClient = originTopic->GetClient();
            if ( GUCEF_NULL != originClient )
            {
                CPubSubClientSide* originSide = static_cast< CPubSubClientSide* >( originClient->GetOpaqueUserData() );
                if ( GUCEF_NULL != originSide )
                {
                    if ( invokerThreadId != originSide->GetDelegatorThreadId() )
                        return originSide->AcknowledgeReceiptASync( msg );
                    else
                        return originSide->AcknowledgeReceiptSync( msg );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since parent client owning the origin topic provided on the message does not have a link back to the owner pubsub side. This should never happen." );
                    return false;
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since no parent client owns the origin topic provided on the message. This should never happen." );
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                "):AcknowledgeReceiptForSide: Unable to ack receipt of message to origin topic since no origin topic was provided on the message. Check your config and backend feature compatibility" );
            return false;
        }
    }
    else
    {
        // @TODO: support for more than 2 sides is not implemented yet
            // The trickyness in this case is that you have to wait to ack until all sides have
            // acked (or perhaps just 1 based on config?)
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    if ( CPubSubClientSide::OnTaskStart( taskData ) )
    {
        if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
        {
            return m_sideBPubSub->OnTaskStart( taskData );
        }
        else
        {
            CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
            if ( !threadPool->StartTask( m_sideBPubSub ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskStart: Failed to start dedicated thread for other side. Falling back to a single thread" );
                PubSubSideChannelSettings* sideSettings = m_sideBPubSub->GetSideSettings();
                if ( GUCEF_NULL != sideSettings )
                    sideSettings->performPubSubInDedicatedThread = false;
                else
                    return false;

                return m_sideBPubSub->OnTaskStart( taskData );
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel:OnTaskStart: Successfully requested the launch of a dedicated thread for other side" );
                return true;
            }
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskCycle( taskData );

    if ( !m_sideBPubSub.IsNULL() )
    {
        if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
        {
            m_sideBPubSub->OnTaskCycle( taskData );
        }
    }

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnding( CORE::CICloneable* taskdata ,
                                    bool willBeForced           )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskEnding( taskdata, willBeForced );

    if ( !m_sideBPubSub.IsNULL() )
    {
        if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
        {
            m_sideBPubSub->OnTaskEnding( taskdata, willBeForced );
        }
        else
        {
            // Since we are the ones that launched the dedicated thread for the other sides we should also ask
            // to have it cleaned up when we are shutting down this thread
            CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
            if ( !threadPool->RequestTaskToStop( m_sideBPubSub.StaticCast< CORE::CTaskConsumer >(), false ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel:OnTaskEnding: Failed to request the dedicated thread for other side to stop" );
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel:OnTaskEnding: Successfully requested the dedicated thread for the other side to stop" );
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnded( CORE::CICloneable* taskData ,
                                   bool wasForced              )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskEnded( taskData, wasForced );

    if ( !m_sideBPubSub.IsNULL() )
    {
        if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
        {
            m_sideBPubSub->OnTaskEnded( taskData, wasForced );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::LoadConfig( const ChannelSettings& channelSettings )
{GUCEF_TRACE;

    return CPubSubClientSide::LoadConfig( channelSettings ) && m_sideBPubSub->LoadConfig( channelSettings );
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2PubSubInfoResource::RestApiPubSub2PubSubInfoResource( PubSub2PubSub* app )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiPubSubClientChannelConfigResource::RestApiPubSubClientChannelConfigResource( PubSub2PubSub* app )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiPubSubClientChannelConfigResource::~RestApiPubSubClientChannelConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiPubSubClientChannelConfigResource::Serialize( const CORE::CString& resourcePath   ,
                                                     CORE::CDataNode& output             ,
                                                     const CORE::CString& representation ,
                                                     const CORE::CString& params         )
{GUCEF_TRACE;

    CORE::Int32 channelsOffset = resourcePath.HasSubstr( "channels/" );
    if ( channelsOffset < 0 )
        return false;
    channelsOffset += 9;

    CORE::Int32 channelId = CORE::StringToInt32( resourcePath.SubstrToIndex( channelsOffset, false ) );

    CPubSubClientChannelPtr channel = m_app->GetChannelByChannelId( channelId );
    if ( !channel.IsNULL() )
        return channel->GetChannelSettings().SaveConfig( output );
    else
        return false;
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2PubSubInfoResource::~RestApiPubSub2PubSubInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiPubSub2PubSubInfoResource::Serialize( const CORE::CString& resourcePath   ,
                                             CORE::CDataNode& output             ,
                                             const CORE::CString& representation ,
                                             const CORE::CString& params         )
{GUCEF_TRACE;

    output.Clear();
    output.SetName( "info" );
    output.SetNodeType( GUCEF_DATATYPE_OBJECT );
    output.SetAttribute( "application", "pubsub2pubsub" );
    output.SetAttribute( "appBuildDateTime", PubSub2PubSub::GetAppCompileDateTime().ToIso8601DateTimeString( true, true ) );
    output.SetAttribute( "platformBuildDateTime", CORE::CDateTime::CompileDateTime().ToIso8601DateTimeString( true, true ) );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", CORE::CVariant( false ) );
    #else
    output.SetAttribute( "isReleaseBuild", CORE::CVariant( true ) );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2PubSubConfigResource::RestApiPubSub2PubSubConfigResource( PubSub2PubSub* app, bool appConfig )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
    m_allowDeserialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2PubSubConfigResource::~RestApiPubSub2PubSubConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiPubSub2PubSubConfigResource::Serialize( const CORE::CString& resourcePath   ,
                                               CORE::CDataNode& output             ,
                                               const CORE::CString& representation ,
                                               const CORE::CString& params         )
{GUCEF_TRACE;

    if ( m_appConfig )
    {
        const CORE::CDataNode* appConfigData = m_app->GetAppConfig();
        if ( GUCEF_NULL != appConfigData )
        {
            output.Copy( *appConfigData );
            return true;
        }
        return false;
    }
    else
    {
        const CORE::CDataNode& globalConfigData = m_app->GetGlobalConfig();
        output.Copy( globalConfigData );
        return true;
    }
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2PubSubConfigResource::TDeserializeState
RestApiPubSub2PubSubConfigResource::UpdateGlobalConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    // First put the app in standby mode before we mess with the settings
    if ( !m_app->SetStandbyMode( true ) )
        return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;

    // Grab a copy of the current global config for rollback if needed
    CORE::CDataNode oldGlobalConfig = m_app->GetGlobalConfig();
    const CORE::CDataNode* oldAppConfigData = m_app->GetAppConfig( oldGlobalConfig );

    if ( CORE::CCoreGlobal::Instance()->GetConfigStore().ApplyConsolidatedConfig( cfg ) )
    {
        const CORE::CDataNode* appConfigData = m_app->GetAppConfig();
        if ( GUCEF_NULL == appConfigData )
        {
            // We should not lose access to the app config section, this indicates a serious issue
            // Attempt to roll back so we are not entirely broken
            if ( CORE::CCoreGlobal::Instance()->GetConfigStore().ApplyConsolidatedConfig( oldGlobalConfig ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RestApiPubSub2PubSubConfigResource: Failed to obtain access to app config after applying new consolidated global config. Successfully rolled back to previous global config" );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "RestApiPubSub2PubSubConfigResource: Failed to obtain access to app config after applying new consolidated global config and subsequently also failed to roll back to previous global config" );
            }

            return TDeserializeState::DESERIALIZESTATE_CORRUPTEDINPUT;
        }

        if ( GUCEF_NULL != oldAppConfigData )
        {
            bool allowRestApiAppGlobalConfigToPersist = oldAppConfigData->GetAttributeValueOrChildValueByName( "allowPeristanceOfNewGlobalConfigViaRestApi" ).AsBool( false, true );
            if ( allowRestApiAppGlobalConfigToPersist )
            {
                if ( CORE::CCoreGlobal::Instance()->GetConfigStore().SaveConsolidatedConfig( cfg ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RestApiPubSub2PubSubConfigResource: Successfully saved new consolidated global config" );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RestApiPubSub2PubSubConfigResource: Failed to save new consolidated global config" );    
                }
            }
        }

        if ( !m_app->IsGlobalStandbyEnabled() )
        {
            if ( m_app->SetStandbyMode( false ) )
                return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
            else
                return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RestApiPubSub2PubSubConfigResource: IsGlobalStandbyEnabled is true. We will leave the app in standby mode" );
            return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
        }
    }
    else
    {
        // We ran into an issue applying the config
        // Attempt to roll back so we are not entirely broken
        if ( CORE::CCoreGlobal::Instance()->GetConfigStore().ApplyConsolidatedConfig( oldGlobalConfig ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "RestApiPubSub2PubSubConfigResource: Failed to apply new global config. Successfully rolled back to previous global config" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "RestApiUdp2RedisConfigResource: Failed to apply new global config and subsequently also failed to roll back to previous global config" );
        }                
        return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
    }        
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2PubSubConfigResource::TDeserializeState
RestApiPubSub2PubSubConfigResource::Deserialize( const CORE::CString& resourcePath   ,
                                                 const CORE::CDataNode& input        ,
                                                 const CORE::CString& representation ,
                                                 bool isDeltaUpdateOnly              )
{GUCEF_TRACE;

    if ( m_appConfig )
    {
        // Make a copy of the entire global cfg
        CORE::CDataNode globalCfg = m_app->GetGlobalConfig();
        CORE::CDataNode* appConfigData = m_app->GetAppConfig( globalCfg );

        if ( isDeltaUpdateOnly )
        {
            // Not supported yet. @TODO
            // requires proper tree merging support

            return TDeserializeState::DESERIALIZESTATE_NOTSUPPORTED;
        }
        else
        {
            // Provided input content replaces the entire app config segment
            // We also protect against dropping the node name if only the values are provided in this subset
            CORE::CString nodeName = appConfigData->GetName();
            *appConfigData = input;
            if ( appConfigData->GetName().IsNULLOrEmpty() )
                appConfigData->SetName( nodeName ); 
        }
        return UpdateGlobalConfig( globalCfg );
    }
    else
    {
        if ( isDeltaUpdateOnly )
        {
            // Not supported yet. @TODO
            // requires proper tree merging support

            return TDeserializeState::DESERIALIZESTATE_NOTSUPPORTED;
        }
        else
        {
            return UpdateGlobalConfig( input );
        }
    }
}

/*-------------------------------------------------------------------------*/

PubSub2PubSub::PubSub2PubSub( void )
    : CORE::CObserver()
    , CORE::CGloballyConfigurable( false )
    , m_isInStandby( false )
    , m_globalStandbyEnabled( false )
    , m_udpStartPort()
    , m_channelCount()
    , m_pubSub2PubSubStartChannelID( 1 )
    , m_channels()
    , m_channelSettings()
    , m_templateChannelSettings()
    , m_httpServer()
    , m_httpRouter()
    , m_globalConfig()
    , m_metricsTimer()
    , m_transmitMetrics( true )
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

PubSub2PubSub::~PubSub2PubSub()
{GUCEF_TRACE;

    m_httpServer.Close();
}

/*-------------------------------------------------------------------------*/

void
PubSub2PubSub::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &PubSub2PubSub::OnAppShutdown );
    SubscribeTo( &CORE::CCoreGlobal::Instance()->GetApplication() ,
                 CORE::CGUCEFApplication::AppShutdownEvent        ,
                 callback                                         );

    TEventCallback callback2( this, &PubSub2PubSub::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback2                      );
}

/*-------------------------------------------------------------------------*/

CPubSubClientChannelPtr
PubSub2PubSub::GetChannelByChannelId( CORE::Int32 cid ) const
{GUCEF_TRACE;

    PubSubClientChannelMap::const_iterator i = m_channels.find( cid );
    if ( i != m_channels.end() )
    {
        return (*i).second;
        ++i;
    }
    return CPubSubClientChannelPtr();
}

/*-------------------------------------------------------------------------*/

bool
PubSub2PubSub::IsGlobalStandbyEnabled( void ) const
{GUCEF_TRACE;

    return m_globalStandbyEnabled;
}

/*-------------------------------------------------------------------------*/

bool
PubSub2PubSub::Start( void )
{GUCEF_TRACE;

    m_isInStandby = true;
    bool errorOccured = !SetStandbyMode( m_globalStandbyEnabled );

    if ( !errorOccured )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub: Opening REST API on port " + CORE::ToString( m_httpServer.GetPort() ) );
        if ( !m_httpServer.Listen() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub: Failed to open REST API on port " + CORE::ToString( m_httpServer.GetPort() ) );
            return false;
        }
    }
    return !errorOccured;
}

/*-------------------------------------------------------------------------*/

bool
PubSub2PubSub::SetStandbyMode( bool putInStandbyMode )
{GUCEF_TRACE;

    // Check if we need to do anything
    if ( m_isInStandby == putInStandbyMode )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:SetStandbyMode( " + CORE::BoolToString( putInStandbyMode ) + " ): Already in the desired mode (" + CORE::BoolToString( m_isInStandby ) + "), nothing to do" );
        return true;
    }

    if ( putInStandbyMode )
    {
        bool totalSuccess = true;
        CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();

        // Signal all channel threads to stop gracefully
        // Since this standby operation is global not per channel we signal all to stop before
        // we starting any waiting operation
        PubSubClientChannelMap::iterator i = m_channels.begin();
        while ( i != m_channels.end() )
        {
            CPubSubClientChannelPtr channel = (*i).second;
            if ( !threadPool->RequestTaskToStop( channel.StaticCast< CORE::CTaskConsumer >(), false ) )
            {
                totalSuccess = false;
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:SetStandbyMode( true ): Failed to signal task to stop for channel " + CORE::Int32ToString( channel->GetChannelSettings().channelId ) )
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:SetStandbyMode( true ): Requested channel " + CORE::Int32ToString( channel->GetChannelSettings().channelId ) + "'s task to stop" );
            }
            ++i;
        }

        // Now actually wait for the threads to be finished
        i = m_channels.begin();
        while ( i != m_channels.end() )
        {
            CPubSubClientChannelPtr channel = (*i).second;
            if ( !channel->WaitForTaskToFinish( 30000 ) )
            {
                totalSuccess = false;
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:SetStandbyMode( true ): Failed to signal task to stop for channel " + CORE::Int32ToString( channel->GetChannelSettings().channelId ) )
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:SetStandbyMode( true ): Successfully waited for channel " + CORE::Int32ToString( channel->GetChannelSettings().channelId ) + "'s task to stop" );
            }
            ++i;
        }

        m_metricsTimer.SetEnabled( false );

        m_isInStandby = totalSuccess;
        return totalSuccess;
    }
    else
    {
        bool totalSuccess = true;

        // Channel config could have changed such that we need to remove channels that should no longer exist
        PubSubClientChannelMap::iterator i = m_channels.begin();
        while ( i != m_channels.end() )
        {
            CORE::Int32 channelId = (*i).first;
            ChannelSettingsMap::iterator n = m_channelSettings.find( channelId );
            if ( n == m_channelSettings.end() )
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:SetStandbyMode( false ): Found channel which no longer has corresponding channel settings, deleting channel with ID " + CORE::Int32ToString( channelId ) );
                m_channels.erase( i );
                i = m_channels.begin();
                break;
            }
            ++i;
        }

        // Alternatively channel config could have changed such that we have new channels
        ChannelSettingsMap::iterator n = m_channelSettings.begin();
        while ( n != m_channelSettings.end() )
        {
            CORE::Int32 channelId = (*n).first;
            PubSubClientChannelMap::iterator i = m_channels.find( channelId );
            if ( i == m_channels.end() )
            {
                // This is a brand new channel. Lets add the channel object for it
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:SetStandbyMode( false ): Found channel settings whith no corresponding channel object, creating new channel with ID " + CORE::Int32ToString( channelId ) );
                m_channels[ channelId ] = CPubSubClientChannelPtr( new CPubSubClientChannel() );
            }
            ++n;
        }

        CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();

        n = m_channelSettings.begin();
        while ( n != m_channelSettings.end() )
        {
            CORE::Int32 channelId = (*n).first;
            PubSubClientChannelMap::iterator i = m_channels.find( channelId );
            if ( i != m_channels.end() )
            {
                const ChannelSettings& channelSettings = (*n).second;
                CPubSubClientChannelPtr channel = (*i).second;

                if ( !channel->LoadConfig( channelSettings ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub::SetStandbyMode( false ): Failed to set channel settings on channel " + CORE::Int32ToString( channelId ) );
                    totalSuccess = false;
                    break;
                }

                if ( !threadPool->StartTask( channel ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub::SetStandbyMode( false ): Failed to start task (dedicated thread) for channel " + CORE::Int32ToString( channelId ) );
                    totalSuccess = false;
                    break;
                }
            }
            ++n;
        }

        if ( totalSuccess && m_transmitMetrics )
        {
            m_metricsTimer.SetInterval( 1000 );
            m_metricsTimer.SetEnabled( true );
        }

        m_isInStandby = !totalSuccess;
        return totalSuccess;
    }
}

/*-------------------------------------------------------------------------*/

bool
PubSub2PubSub::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    // not fully supported right now

    tree.Copy( m_globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
PubSub2PubSub::LoadConfig( const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    // First we load the channel related info.
    // We begin with that because we want to load especially the template definitions before we load the app settings as we might need the template
    
    // First store the per channel configs in a more conveniently accessable manner
    // splitting them out from the global config document
    TChannelCfgMap channelMap;
    CORE::CDataNode::TConstDataNodeSet channelParentCfgs = globalConfig.FindChildrenOfType( "Channels", true );
    CORE::CDataNode::TConstDataNodeSet::iterator i = channelParentCfgs.begin();
    while ( i != channelParentCfgs.end() )
    {
        CORE::CDataNode::const_iterator n = (*i)->ConstBegin();
        while ( n != (*i)->ConstEnd() )
        {
            const CORE::CString& channelIndex = (*n)->GetName();
            channelMap[ channelIndex ] = *(*n);
            ++n;
        }
        ++i;
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSub2Storage:LoadConfig: Found " + CORE::ToString( channelMap.size() ) + " configuration entries for pubsub2pubsub channels" );

    // load the template config if any
    // This is especially important in conjunction with command line params that would rely on a template config
    TChannelCfgMap::iterator m = channelMap.find( "*" );
    if ( m != channelMap.end() )
    {
        if ( m_templateChannelSettings.LoadConfig( (*m).second ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSub2Storage:LoadConfig: Successfully loaded template config for pubsub2pubsub channels" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSub2Storage:LoadConfig: Failed to correctly load template config for pubsub2pubsub channels" );
            return false;
        }
    }

    // load the specifically configured channels if any
    // Such channels would not be defined via command line params but can possibly be influenced by such params with combined usage
    m = channelMap.begin();
    while ( m != channelMap.end() )
    {
        const CORE::CString& channelIndexStr = (*m).first;
        if ( channelIndexStr != '*' )
        {
            CORE::Int32 channelIndex = CORE::StringToInt32( channelIndexStr );
            ChannelSettings& channelSettings = m_channelSettings[ channelIndex ];

            if ( channelSettings.LoadConfig( (*m).second ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSub2Storage:LoadConfig: Successfully loaded explicit config for pubsub2pubsub channels " + channelIndexStr );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSub2Storage:LoadConfig: Failed to correctly load explicit config for pubsub2pubsub channels " + channelIndexStr );
                return false;
            }
        }
        ++m;
    }

    // Now on to the main application config ...

    const CORE::CDataNode* appConfig = GetAppConfig( globalConfig );
    if ( GUCEF_NULL == appConfig )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:LoadConfig: Unable to locate app config in global config" );
        return false;
    }

    m_globalStandbyEnabled = appConfig->GetAttributeValueOrChildValueByName( "globalStandbyEnabled" ).AsBool( m_globalStandbyEnabled, true );

    m_pubSub2PubSubStartChannelID = appConfig->GetAttributeValueOrChildValueByName( "pubSub2PubSubStartChannelID" ).AsInt32( m_pubSub2PubSubStartChannelID, true );
    CORE::CString::StringSet channelIDStrs = appConfig->GetAttributeValueOrChildValueByName( "channelIDs" ).AsString( CORE::CString::Empty, true ).ParseUniqueElements( ',', false );
    m_channelCount = appConfig->GetAttributeValueOrChildValueByName( "channelCount" ).AsUInt16( channelIDStrs.empty() ? 1 : (CORE::UInt16) channelIDStrs.size(), true );

    bool applyCpuThreadAffinityByDefault = appConfig->GetAttributeValueOrChildValueByName( "applyCpuThreadAffinityByDefault" ).AsBool( false, true );
    CORE::UInt32 logicalCpuCount = CORE::GetLogicalCPUCount();

    // We will assume we are always given a full not a partial config so we clear the existing channel settings
    m_channelSettings.clear();

    // Validate the channel IDs.
    // Depending on the use case these could be vital identifiers not just an index so some validation is in order
    Int32Set channelIDs;
    CORE::CString::StringSet::iterator n = channelIDStrs.begin();
    while ( n != channelIDStrs.end() )
    {
        const CORE::CString& str = (*n);
        if ( str.HasChar( '-' ) >= 0 )
        {
            CORE::Int32 startId = GUCEF_MT_INT32MIN;
            CORE::Int32 endId = GUCEF_MT_INT32MIN;

            CORE::CString::StringVector rangeStrs = str.ParseElements( '-', false );
            if ( rangeStrs.size() > 0 )
                startId = CORE::StringToInt32( rangeStrs[ 0 ], GUCEF_MT_INT32MIN );
            if ( rangeStrs.size() > 1 )
                endId = CORE::StringToInt32( rangeStrs[ 1 ], GUCEF_MT_INT32MIN );

            if ( startId != GUCEF_MT_INT32MIN && endId != GUCEF_MT_INT32MIN )
            {
                if ( endId < startId )
                {
                    CORE::Int32 tmp = startId;
                    startId = endId;
                    endId = tmp;
                }

                for ( CORE::Int32 i=startId; i<=endId; i++ )
                    channelIDs.insert( i );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub::LoadConfig: Invalid channel ID range provided. Fix the config. Str: " + str );
                return false;
            }
        }
        else
        {
            CORE::Int32 id = CORE::StringToInt32( str, GUCEF_MT_INT32MIN );
            if ( id != GUCEF_MT_INT32MIN )
                channelIDs.insert( id );
        }
        ++n;
    }
    if ( channelIDs.size() < channelIDStrs.size() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub:LoadConfig: Only " + CORE::ToString( channelIDs.size() ) + " numerical channel IDs were obtained from the channel list which contained more strings. Fix the config" );
        return false;
    }

    // Make sure we have enough channel IDs specified to cover the channel count
    if ( channelIDs.size() < m_channelCount )
    {
        // auto-generate additional channel IDs
        // This allows a configuration style where you don't have to specify all channel IDs
        CORE::Int32 lastAutoGenChannelId = m_pubSub2PubSubStartChannelID;
        CORE::Int32 missingChannels = m_channelCount - (CORE::Int32) channelIDs.size();
        for ( CORE::Int32 i=0; i<missingChannels; ++i )
        {
            while ( channelIDs.find( lastAutoGenChannelId ) != channelIDs.end() )
                ++lastAutoGenChannelId;
            channelIDs.insert( lastAutoGenChannelId );
            ++lastAutoGenChannelId;
        }
    }
    else
    if ( m_channelCount < channelIDs.size() )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub::LoadConfig: " + CORE::ToString( channelIDs.size() ) + " numerical channel IDs were provided but a total channel count of " +
            CORE::ToString( m_channelCount ) + " was configured. Channel count will be increased to match the nr of IDs" );
        m_channelCount = (CORE::UInt16) channelIDs.size();
    }

    CORE::UInt32 currentCpu = 0;
    Int32Set::iterator idListIttr = channelIDs.begin();
    for ( CORE::Int32 i=0; i<m_channelCount; ++i )
    {
        CORE::Int32 channelId = (*idListIttr);

        ChannelSettings* channelSettings = GUCEF_NULL;
        ChannelSettingsMap::iterator s = m_channelSettings.find( channelId );
        if ( s == m_channelSettings.end() )
        {
            channelSettings = &m_channelSettings[ channelId ];
            *channelSettings = m_templateChannelSettings;
        }
        else
        {
            channelSettings = &m_channelSettings[ channelId ];
        }
        channelSettings->channelId = channelId;

        // Assign CPU affinity
        ChannelSettings::TCharToPubSubSideChannelSettingsMap::iterator n = channelSettings->pubSubSideChannelSettingsMap.begin();
        while ( n != channelSettings->pubSubSideChannelSettingsMap.end() )
        {
            PubSubSideChannelSettings& sideSettings = (*n).second;
            if ( sideSettings.applyThreadCpuAffinity || applyCpuThreadAffinityByDefault )
            {
                if ( sideSettings.performPubSubInDedicatedThread )
                {
                    sideSettings.cpuAffinityForPubSubThread = currentCpu;

                    ++currentCpu;
                    if ( currentCpu >= logicalCpuCount ) // Wrap around if we run out of CPUs
                        currentCpu = 0;
                }
            }

            ++n;
        }

        channelSettings->UpdateDerivedSettings();
    }

    m_httpServer.SetPort( appConfig->GetAttributeValueOrChildValueByName( "restApiPort" ).AsUInt16( 10000, true ) );

    m_httpRouter.SetResourceMapping( "/info", RestApiPubSub2PubSubInfoResource::THTTPServerResourcePtr( new RestApiPubSub2PubSubInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiPubSub2PubSubConfigResource::THTTPServerResourcePtr( new RestApiPubSub2PubSubConfigResource( this, true ) ) );
    m_httpRouter.SetResourceMapping( "/config", RestApiPubSub2PubSubConfigResource::THTTPServerResourcePtr( new RestApiPubSub2PubSubConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping( "/config/channels/*", RestApiPubSubClientChannelConfigResource::THTTPServerResourcePtr( new RestApiPubSubClientChannelConfigResource( this ) )  );
    m_httpRouter.SetResourceMapping( appConfig->GetAttributeValueOrChildValueByName( "restBasicHealthUri" ).AsString( "/health/basic", true ), RestApiPubSub2PubSubConfigResource::THTTPServerResourcePtr( new WEB::CDummyHTTPServerResource() ) );
    
    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

    m_globalConfig.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CDateTime&
PubSub2PubSub::GetAppCompileDateTime( void )
{GUCEF_TRACE;

    static CORE::CDateTime compileDt = CORE::CDateTime::CompileDateTime( __DATE__, __TIME__ );
    return compileDt;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
PubSub2PubSub::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "PubSub2PubSub";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
PubSub2PubSub::OnAppShutdown( CORE::CNotifier* notifier    ,
                              const CORE::CEvent& eventId  ,
                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // First stop all the work gracefully
    SetStandbyMode( true );

    // Since we are shutting down the app gracefully close the C&C API now
    m_httpServer.Close();

    // Now get rid of all the channels we created based on the settings
    m_channels.clear();
}

/*-------------------------------------------------------------------------*/

void
PubSub2PubSub::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventId  ,
                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // We invoke the channel's 'publish metrics' from the thread the main app
    // runs in which is a different thread vs the channels. reason being that if
    // the channels are bogged down busy they may not get to send stats in time
    PubSubClientChannelMap::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        (*i).second->PublishChannelMetrics();
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode* 
PubSub2PubSub::GetAppConfig( const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    const CORE::CDataNode* appConfig = globalConfig.Search( "Main/AppArgs", '/', true, true );
    return appConfig;
}

/*-------------------------------------------------------------------------*/

CORE::CDataNode* 
PubSub2PubSub::GetAppConfig( CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    CORE::CDataNode* appConfig = globalConfig.Structure( "Main/AppArgs", '/' );
    return appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode* 
PubSub2PubSub::GetAppConfig( void ) const
{GUCEF_TRACE;

    return GetAppConfig( m_globalConfig );
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
PubSub2PubSub::GetGlobalConfig( void ) const
{GUCEF_TRACE;

    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/

