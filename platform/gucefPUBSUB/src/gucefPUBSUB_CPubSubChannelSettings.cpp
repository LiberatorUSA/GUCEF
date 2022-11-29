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

#ifndef GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#include "gucefPUBSUB_CPubSubGlobal.h"
#define GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#endif /* GUCEF_PUBSUB_CPUBSUBGLOBAL_H ? */

#ifndef GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#include "gucefPUBSUB_CBasicPubSubMsg.h"
#define GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#endif /* GUCEF_PUBSUB_CBASICPUBSUBMSG_H ? */

#include "gucefPUBSUB_CPubSubChannelSettings.h"

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

CPubSubChannelConfig::CPubSubChannelConfig( void )
    : CORE::CIConfigurable()
    , pubSubSideChannelSettingsMap()
    , flowRouterConfig()
    , channelId( -1 )
    , channelName()
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , collectMetrics( true )
    , metricsIntervalInMs( 1000 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubChannelConfig::CPubSubChannelConfig( const CPubSubChannelConfig& src )
    : CORE::CIConfigurable( src )
    , pubSubSideChannelSettingsMap( src.pubSubSideChannelSettingsMap )
    , flowRouterConfig( src.flowRouterConfig )
    , channelId( src.channelId )
    , channelName( src.channelName )
    , ticketRefillOnBusyCycle( src.ticketRefillOnBusyCycle )
    , collectMetrics( src.collectMetrics )
    , metricsIntervalInMs( src.metricsIntervalInMs )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubChannelConfig::~CPubSubChannelConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubChannelConfig&
CPubSubChannelConfig::operator=( const CPubSubChannelConfig& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        pubSubSideChannelSettingsMap = src.pubSubSideChannelSettingsMap;
        flowRouterConfig = src.flowRouterConfig;
        channelId = src.channelId;
        channelName = src.channelName;
        ticketRefillOnBusyCycle = src.ticketRefillOnBusyCycle;
        collectMetrics = src.collectMetrics;
        metricsIntervalInMs = src.metricsIntervalInMs;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CPubSubSideChannelSettings*
CPubSubChannelConfig::GetPubSubSideSettings( const CORE::CString& sideId )
{GUCEF_TRACE;

    TStringToPubSubSideChannelSettingsMap::iterator i = pubSubSideChannelSettingsMap.find( sideId );
    if ( i != pubSubSideChannelSettingsMap.end() )
    {
        return &(*i).second;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubChannelConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    CORE::CDataNode* pubSubSidesCollection = cfg.Structure( "PubSubSides", '/' );
    pubSubSidesCollection->SetNodeType( GUCEF_DATATYPE_ARRAY );

    // We don't want to merge from a potential previous save so we wipe what could be
    // a pre-existing collection
    pubSubSidesCollection->DelSubTree();

    TStringToPubSubSideChannelSettingsMap::const_iterator i = pubSubSideChannelSettingsMap.begin();
    while ( i != pubSubSideChannelSettingsMap.end() )
    {
        const CPubSubSideChannelSettings& sideSettings = (*i).second;
        CORE::CDataNode* sideSettingsNode = pubSubSidesCollection->FindOrAddChild( "PubSubSideChannelSettings" );
        if ( !sideSettings.SaveConfig( *sideSettingsNode ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:SaveConfig: config is malformed, failed to save a mandatory PubSubSideChannelSettings section" );
            return false;
        }
        ++i;
    }

    totalSuccess = cfg.SetAttribute( "channelId", channelId ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "channelName", channelName ) && totalSuccess;    
    totalSuccess = cfg.SetAttribute( "ticketRefillOnBusyCycle", ticketRefillOnBusyCycle ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "collectMetrics", collectMetrics ) && totalSuccess;

    CORE::CDataNode* pubSubFlowRouterConfigNode = cfg.FindOrAddChild( "PubSubFlowRouterConfig" );
    if ( GUCEF_NULL != pubSubFlowRouterConfigNode )
    {
        if ( !flowRouterConfig.SaveConfig( *pubSubFlowRouterConfigNode ) )
            return false;
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:SaveConfig: config is malformed, failed to save a mandatory PubSubFlowRouterConfig section" );
        return false;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubChannelConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    channelId = cfg.GetAttributeValueOrChildValueByName( "channelId" ).AsInt32( channelId, true );
    channelName = cfg.GetAttributeValueOrChildValueByName( "channelName" ).AsString( channelName, true );
    ticketRefillOnBusyCycle = cfg.GetAttributeValueOrChildValueByName( "ticketRefillOnBusyCycle" ).AsUInt32( ticketRefillOnBusyCycle, true );
    collectMetrics = cfg.GetAttributeValueOrChildValueByName( "collectMetrics" ).AsBool( collectMetrics, true );
    metricsIntervalInMs = cfg.GetAttributeValueOrChildValueByName( "metricsIntervalInMs" ).AsUInt32( metricsIntervalInMs, true );

    const CORE::CDataNode* pubSubSidesCollection = cfg.Find( "PubSubSides" );
    if ( GUCEF_NULL != pubSubSidesCollection )
    {
        CORE::CDataNode::const_iterator n = pubSubSidesCollection->ConstBegin();
        while ( n != pubSubSidesCollection->ConstEnd() )
        {
            CPubSubSideChannelSettings sideSettings;
            if ( !sideSettings.LoadConfig( *(*n) ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:LoadConfig: Side config entry failed to load from collection entry" );
                return false;
            }

            // There is no sane default of pubsubClientType since it depends on the clients loaded into the application
            // as such this is a mandatory setting to provide
            if ( sideSettings.pubsubClientConfig.pubsubClientType.IsNULLOrEmpty() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:LoadConfig: Side config is malformed, \"pubsubClientType\" was not provided" );
                return false;
            }

            // We are fully config driven with no programatically defined topics
            // As such the config must have yielded at least 1 topic
            if ( sideSettings.pubsubClientConfig.topics.empty() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:LoadConfig: Side config is malformed, having at least one topic configured for the client section is mandatory" );
                return false;
            }

            if ( (*n)->GetName().IsNULLOrEmpty() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:LoadConfig: Side config is malformed, we need a valid name for the side" );
                return false;
            }

            const CORE::CString& sideId = (*n)->GetName();
            pubSubSideChannelSettingsMap[ sideId ] = sideSettings;
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:LoadConfig: Side \'" + CORE::CString( sideId ) + "\' config successfully loaded" );

            ++n;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:LoadConfig: PubSubSides collection section is mandatory" );
        return false;
    }

    const CORE::CDataNode* pubSubFlowRouterConfig = cfg.Find( "PubSubFlowRouterConfig" );
    if ( GUCEF_NULL != pubSubFlowRouterConfig )
    {
        // If we have a flow router config it should be well formed and load properly
        if ( !flowRouterConfig.LoadConfig( *pubSubFlowRouterConfig ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: Failed to load flow router config" );
            return false;
        }
    }
    else
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubChannelConfig:LoadConfig: No PubSubFlowRouterConfig was found, will use default implicit config" );
        
        // If we do not have a flow router config we take it as allowing * -> * implicitly
        // this setting matches the historical behaviour, this route wont have any failover or spillover or dead letter
        flowRouterConfig.Clear();        
        CPubSubFlowRouteConfig implicitRoute;
        implicitRoute.fromSide = "*";
        implicitRoute.toSide = "*";
        flowRouterConfig.routes.push_back( implicitRoute );

        // We also use the safest setting for the ack style
        // this setting matches the historical behaviour
        flowRouterConfig.ackStyle = CPubSubFlowRouterConfig::AckStyle::AllOrNothing;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CPubSubChannelConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::PUBSUB::CPubSubChannelConfig";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

CPubSubChannelSettings::CPubSubChannelSettings( void )
    : CPubSubChannelConfig()
    , metricsPrefix()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubChannelSettings::CPubSubChannelSettings( const CPubSubChannelSettings& src )
    : CPubSubChannelConfig( src )
    , metricsPrefix( src.metricsPrefix )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubChannelSettings::~CPubSubChannelSettings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubChannelSettings&
CPubSubChannelSettings::operator=( const CPubSubChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CPubSubChannelConfig::operator=( src );
        metricsPrefix = src.metricsPrefix;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubChannelSettings::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = CPubSubChannelConfig::SaveConfig( cfg );

    // Derived settings are advisory outputs only meaning we will save them but we wont load them
    totalSuccess = cfg.SetAttribute( "metricsPrefix", metricsPrefix ) && totalSuccess;

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubChannelSettings::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    bool totalSuccess = CPubSubChannelConfig::LoadConfig( cfg );
    UpdateDerivedSettings();
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

void
CPubSubChannelSettings::UpdateDerivedSettings( void )
{GUCEF_TRACE;

    metricsPrefix = "channel." + CORE::ToString( channelId ) + ".";

    TStringToPubSubSideChannelSettingsMap::iterator i = pubSubSideChannelSettingsMap.begin();
    while ( i != pubSubSideChannelSettingsMap.end() )
    {
        CPubSubSideChannelSettings& sideSettings = (*i).second;

        sideSettings.metricsPrefix = metricsPrefix + "side." + (*i).first + ".";
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CPubSubChannelSettings::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::PUBSUB::CPubSubChannelSettings";
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


