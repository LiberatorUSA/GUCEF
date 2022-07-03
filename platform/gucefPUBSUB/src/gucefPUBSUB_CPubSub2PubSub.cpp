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

#include "gucefPUBSUB_CPubSub2PubSub.h"

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
                const CPubSubChannelSettings& channelSettings = (*n).second;
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
            CPubSubChannelSettings& channelSettings = m_channelSettings[ channelIndex ];

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

        CPubSubChannelSettings* channelSettings = GUCEF_NULL;
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
        CPubSubChannelSettings::TCharToPubSubSideChannelSettingsMap::iterator n = channelSettings->pubSubSideChannelSettingsMap.begin();
        while ( n != channelSettings->pubSubSideChannelSettingsMap.end() )
        {
            CPubSubSideChannelSettings& sideSettings = (*n).second;
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

    m_httpRouter.SetResourceMapping( "/info", ( new RestApiPubSub2PubSubInfoResource( this ) )->CreateSharedPtr() );
    m_httpRouter.SetResourceMapping( "/config/appargs", ( new RestApiPubSub2PubSubConfigResource( this, true ) )->CreateSharedPtr() );
    m_httpRouter.SetResourceMapping( "/config", ( new RestApiPubSub2PubSubConfigResource( this, false ) )->CreateSharedPtr()  );
    m_httpRouter.SetResourceMapping( "/config/channels/*", ( new RestApiPubSubClientChannelConfigResource( this ) )->CreateSharedPtr() );
    m_httpRouter.SetResourceMapping( appConfig->GetAttributeValueOrChildValueByName( "restBasicHealthUri" ).AsString( "/health/basic", true ), ( new WEB::CDummyHTTPServerResource() )->CreateSharedPtr() );
    
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

    static const CORE::CString classTypeName = "GUCEF::PUBSUB::PubSub2PubSub";
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


