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

CPubSubChannelSettings::CPubSubChannelSettings( void )
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

CPubSubChannelSettings::CPubSubChannelSettings( const CPubSubChannelSettings& src )
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

CPubSubChannelSettings&
CPubSubChannelSettings::operator=( const CPubSubChannelSettings& src )
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

CPubSubSideChannelSettings*
CPubSubChannelSettings::GetPubSubSideSettings( char side )
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
CPubSubChannelSettings::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    CORE::CDataNode* pubSubSidesCollection = tree.Structure( "PubSubSides", '/' );
    pubSubSidesCollection->SetNodeType( GUCEF_DATATYPE_ARRAY );

    // We don't want to merge from a potential previous save so we wipe what could be
    // a pre-existing collection
    pubSubSidesCollection->DelSubTree();

    TCharToPubSubSideChannelSettingsMap::const_iterator i = pubSubSideChannelSettingsMap.begin();
    while ( i != pubSubSideChannelSettingsMap.end() )
    {
        const CPubSubSideChannelSettings& sideSettings = (*i).second;
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
CPubSubChannelSettings::LoadConfig( const CORE::CDataNode& cfg )
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
            CPubSubSideChannelSettings sideSettings;
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
CPubSubChannelSettings::UpdateDerivedSettings( void )
{GUCEF_TRACE;

    metricsPrefix = "channel." + CORE::ToString( channelId ) + ".";

    TCharToPubSubSideChannelSettingsMap::iterator i = pubSubSideChannelSettingsMap.begin();
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


