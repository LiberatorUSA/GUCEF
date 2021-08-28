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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

PubSubSideChannelSettings::PubSubSideChannelSettings( void )
    : CORE::CIConfigurable()
    , pubsubClientConfig()
    , performPubSubInDedicatedThread()
    , applyThreadCpuAffinity( false )
    , cpuAffinityForPubSubThread( -1 )
    , subscribeUsingDefaultBookmarkIfThereIsNoLast( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

PubSubSideChannelSettings::PubSubSideChannelSettings( const PubSubSideChannelSettings& src )
    : CORE::CIConfigurable( src )
    , pubsubClientConfig( src.pubsubClientConfig )
    , performPubSubInDedicatedThread( src.performPubSubInDedicatedThread )
    , applyThreadCpuAffinity( src.applyThreadCpuAffinity )
    , cpuAffinityForPubSubThread( src.cpuAffinityForPubSubThread )
    , subscribeUsingDefaultBookmarkIfThereIsNoLast( src.subscribeUsingDefaultBookmarkIfThereIsNoLast )
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
        subscribeUsingDefaultBookmarkIfThereIsNoLast = src.subscribeUsingDefaultBookmarkIfThereIsNoLast;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
PubSubSideChannelSettings::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;
    
    CORE::CDataNode* psClientConfig = tree.Structure( "PubSubClientConfig", '/' );
    if ( !pubsubClientConfig.SaveConfig( *psClientConfig ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubSideChannelSettings:SaveConfig: config is malformed, failed to save a mandatory PubSubClientConfig section" );
        return false;        
    }

    tree.SetAttribute( "performPubSubInDedicatedThread", performPubSubInDedicatedThread );
    tree.SetAttribute( "applyThreadCpuAffinity", applyThreadCpuAffinity );
    tree.SetAttribute( "cpuAffinityForPubSubThread", cpuAffinityForPubSubThread );
    tree.SetAttribute( "subscribeUsingDefaultBookmarkIfThereIsNoLast", subscribeUsingDefaultBookmarkIfThereIsNoLast );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
PubSubSideChannelSettings::LoadConfig( const CORE::CDataNode& tree )
{GUCEF_TRACE;

    const CORE::CDataNode* psClientConfig = tree.Search( "PubSubClientConfig", '/', true );
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
    performPubSubInDedicatedThread = tree.GetAttributeValueOrChildValueByName( "performPubSubInDedicatedThread" ).AsBool( performPubSubInDedicatedThread, true );
    applyThreadCpuAffinity = tree.GetAttributeValueOrChildValueByName( "applyThreadCpuAffinity" ).AsBool( applyThreadCpuAffinity, true );
    cpuAffinityForPubSubThread = tree.GetAttributeValueOrChildValueByName( "cpuAffinityForPubSubThread" ).AsUInt32( cpuAffinityForPubSubThread, true );
    subscribeUsingDefaultBookmarkIfThereIsNoLast = tree.GetAttributeValueOrChildValueByName( "subscribeUsingDefaultBookmarkIfThereIsNoLast" ).AsBool( subscribeUsingDefaultBookmarkIfThereIsNoLast, true );
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
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( const ChannelSettings& src )
    : CORE::CIConfigurable( src )
    , pubSubSideChannelSettingsMap( src.pubSubSideChannelSettingsMap )
    , channelId( src.channelId )
    , ticketRefillOnBusyCycle( src.ticketRefillOnBusyCycle )
    , collectMetrics( src.collectMetrics )
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

    return true;
}

/*-------------------------------------------------------------------------*/

bool
ChannelSettings::LoadConfig( const CORE::CDataNode& tree )
{GUCEF_TRACE;

    const CORE::CDataNode* pubSubSidesCollection = tree.Find( "PubSubSides" );
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
            ++n;
        }       
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: PubSubSides collection section is mandatory" );
        return false;
    }

    channelId = tree.GetAttributeValueOrChildValueByName( "channelId" ).AsInt32( channelId, true );
    ticketRefillOnBusyCycle = tree.GetAttributeValueOrChildValueByName( "ticketRefillOnBusyCycle" ).AsUInt32( ticketRefillOnBusyCycle, true );
    collectMetrics = tree.GetAttributeValueOrChildValueByName( "collectMetrics" ).AsBool( collectMetrics, true );
    return true;
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
    , m_otherSide( GUCEF_NULL )
    , m_topics()
    , m_channelSettings()
    , m_sideSettings( GUCEF_NULL )
    , m_mailbox()
    , m_metricsTimer( GUCEF_NULL )
    , m_pubsubClientReconnectTimer( GUCEF_NULL )
    , m_persistance( GUCEF_NULL )
    , m_side( side )
    , m_runsInDedicatedThread( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientSide::~CPubSubClientSide()
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CPubSubClientSide::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );

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
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientSide::GetType( void ) const
{GUCEF_TRACE;

    return "PubSubClientSide";
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientSide::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                        const CORE::CEvent& eventId  ,
                                        CORE::CICloneable* eventData )
{GUCEF_TRACE;


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

    if ( GUCEF_NULL == m_otherSide )
        return false;

    CORE::UInt32 topicsToPublishOn = 0;
    CORE::UInt32 topicsPublishedOn = 0;
    bool publishSuccess = true;
    TopicVector::iterator i = m_otherSide->m_topics.begin();
    while ( i != m_otherSide->m_topics.end() )
    {
        COMCORE::CPubSubClientTopic* topic = (*i);
        if ( GUCEF_NULL != topic )
        {
            if ( topic->IsPublishingSupported() )
            {
                ++topicsToPublishOn;
                if ( topic->Publish( msgs ) )
                {
                    ++topicsPublishedOn;
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                        "):PublishMsgsSync: Failed to publish messages to topic" );
                }
            }
        }
        ++i;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
        "):PublishMsgsSync: Successfully published messages to " + CORE::ToString( topicsPublishedOn ) + " topics, " + 
        CORE::ToString( topicsToPublishOn ) + " topics available for publishing" );
    
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
        if ( m_sideSettings->performPubSubInDedicatedThread )
        {
            return PublishMsgsASync( msgs );
        }
        else
        {
            return PublishMsgsSync( msgs );
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::PublishMailboxMsgs( void )
{GUCEF_TRACE;

    COMCORE::CPubSubClientTopic::TIPubSubMsgRawPtrVector msgs;
    if ( m_mailbox.GetPtrBulkMail< COMCORE::CPubSubClientTopic::TIPubSubMsgRawPtrVector >( msgs ) )
    {
        bool publishResult = PublishMsgsSync< COMCORE::CPubSubClientTopic::TIPubSubMsgRawPtrVector >( msgs );

        // We need to delete whatever we obtained from the mailbox
        COMCORE::CPubSubClientTopic::TIPubSubMsgRawPtrVector::iterator i = msgs.begin();
        while ( i != msgs.end() )
        {
            delete (*i);
            ++i;
        }

        return publishResult;
    }
    return true;
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
        const COMCORE::CPubSubClientTopic::TPubSubMsgsRefVector& msgs = ( *static_cast< COMCORE::CPubSubClientTopic::TMsgsRecievedEventData* >( eventData ) );
        if ( !msgs.empty() )
        {                            
            PublishMsgs( msgs );        
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) + "):OnPubSubTopicMsgsReceived: exception: " + CORE::CString( e.what() ) );
    }
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::DisconnectPubSubClient( bool destroyClient )
{GUCEF_TRACE;

    if ( m_pubsubClient.IsNULL() )
        return true;

    if ( !m_pubsubClient->Disconnect() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
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
CPubSubClientSide::ConnectPubSubClient( void )
{GUCEF_TRACE;

    if ( !DisconnectPubSubClient() )
        return false;

    PubSubSideChannelSettings* pubSubSideSettings = m_channelSettings.GetPubSubSideSettings( m_side );
    if ( GUCEF_NULL == pubSubSideSettings )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClient: Unable to obtain settings for configured side \"" + m_side + "\". Cannot proceed" );        
        return false;
    }    
    COMCORE::CPubSubClientConfig& pubSubConfig = pubSubSideSettings->pubsubClientConfig;
    
    if ( m_pubsubClient.IsNULL() )
    {
        // Create and configure the pub-sub client        
        pubSubConfig.pulseGenerator = GetPulseGenerator();
        m_pubsubClient = COMCORE::CComCoreGlobal::Instance()->GetPubSubClientFactory().Create( pubSubConfig.pubsubClientType, pubSubConfig );

        if ( m_pubsubClient.IsNULL() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                "):ConnectPubSubClient: Failed to create a pub-sub client of type \"" + pubSubConfig.pubsubClientType + "\". Cannot proceed" );        
            return false;
        }
    }

    COMCORE::CPubSubClientFeatures clientFeatures;
    m_pubsubClient->GetSupportedFeatures( clientFeatures );
        
    if ( !clientFeatures.supportsAutoReconnect )
    {
        if ( GUCEF_NULL != m_pubsubClientReconnectTimer )
            m_pubsubClientReconnectTimer = new CORE::CTimer( *GetPulseGenerator(), pubSubConfig.reconnectDelayInMs );
    }

    SubscribeTo( m_pubsubClient.GetPointerAlways() );
    if ( !m_pubsubClient->Connect() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClient: Failed to connect the pub-sub client" );
        return false;
    }

    // Create and configure the pub-sub client's topics
    m_topics.clear();
    m_topics.reserve( pubSubConfig.topics.size() );
    COMCORE::CPubSubClientConfig::TPubSubClientTopicConfigVector::iterator i = pubSubConfig.topics.begin();
    while ( i != pubSubConfig.topics.end() )
    {
        COMCORE::CPubSubClientTopic* topic = m_pubsubClient->CreateTopicAccess( (*i) );
        if ( GUCEF_NULL == topic )
        {
            if ( !(*i).isOptional )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ConnectPubSubClient: Failed to create a pub-sub client topic access for topic \"" + (*i).topicName + "\". Cannot proceed" );
                return false;            
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                    "):ConnectPubSubClient: Unable to create a pub-sub client topic access for optional topic \"" + (*i).topicName + "\". Proceeding" );
            }
        }

        RegisterTopicEventHandlers( *topic );
        m_topics.push_back( topic );
        ++i;
    }

    TopicVector::iterator t = m_topics.begin();
    while ( t != m_topics.end() )
    {
        if ( (*t)->InitializeConnectivity() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                "):ConnectPubSubClient: Successfully requested connectivity initialization for topic \"" + (*t)->GetTopicName() + "\". Proceeding" );

            // We use the 'desired' feature to also drive whether we actually subscribe at this point
            // saves us an extra setting
            const COMCORE::CPubSubClientTopicConfig* topicConfig = m_pubsubClient->GetTopicConfig( (*t)->GetTopicName() );
            if ( GUCEF_NULL != topicConfig && topicConfig->needSubscribeSupport )
            {            
                // The method of subscription depends on the supported feature set
                bool subscribeSuccess = false;
                if ( clientFeatures.supportsAutoBookmarking && clientFeatures.supportsBookmarkingConcept ) // first preference is backend managed bookmarking
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                        "):ConnectPubSubClient: Bookmarking concept is natively supported and managed by the backend independently and we will attempt to subscribe as such" );

                    subscribeSuccess = (*t)->Subscribe();
                }
                else
                if ( clientFeatures.supportsMsgIdBasedBookmark && clientFeatures.supportsBookmarkingConcept ) // seconds preference is msgId based bookmarking because of its exact addressability
                {
                    CORE::CVariant msgId;
                    CORE::CDateTime msgDt;
                    if ( !m_persistance->GetLastPersistedMsgAttributes( m_channelSettings.channelId, (*t)->GetTopicName(), msgId, msgDt ) )
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                            "):ConnectPubSubClient: Bookmarking concept is supported by the backend via a client-side message Id but we failed at obtaining the last used message id" );
                        
                        if ( pubSubSideSettings->subscribeUsingDefaultBookmarkIfThereIsNoLast )
                        {
                            subscribeSuccess = (*t)->Subscribe();
                            if ( !subscribeSuccess )
                            {
                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                                    "):ConnectPubSubClient: Also unable to subscribe using the default bookmark as a fallback" );
                                return false;
                            }
                        }
                        else
                            return false;
                    }
                    else
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                            "):ConnectPubSubClient: Bookmarking concept is supported by the backend via a client-side message Id. ID=" + msgId );

                        subscribeSuccess = (*t)->SubscribeStartingAtMsgId( msgId );
                    }
                }
                else
                if ( clientFeatures.supportsMsgIdBasedBookmark && clientFeatures.supportsBookmarkingConcept ) // third preference is DateTime based bookmarking. Not as exact but better than nothing
                {
                    CORE::CVariant msgId;
                    CORE::CDateTime msgDt;
                    if ( !m_persistance->GetLastPersistedMsgAttributes( m_channelSettings.channelId, (*t)->GetTopicName(), msgId, msgDt ) )
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                            "):ConnectPubSubClient: Bookmarking concept is supported by the backend via a last-received client-side message DateTime but we failed at obtaining it" );

                        if ( pubSubSideSettings->subscribeUsingDefaultBookmarkIfThereIsNoLast )
                        {
                            subscribeSuccess = (*t)->Subscribe();
                            if ( !subscribeSuccess )
                            {
                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                                    "):ConnectPubSubClient: Also unable to subscribe using the default bookmark as a fallback" );
                                return false;
                            }
                        }
                        else
                            return false;

                    }
                    else
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                            "):ConnectPubSubClient: Bookmarking concept is supported by the backend via a last-received client-side message DateTime which is " + msgDt.ToIso8601DateTimeString( true, true ) );
                    
                        subscribeSuccess = (*t)->SubscribeStartingAtMsgDateTime( msgDt );
                    }
                }
                else
                if ( !clientFeatures.supportsBookmarkingConcept )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                        "):ConnectPubSubClient: Bookmarking concept is not supported by the backend. We will subscribe and get whatever we get" );

                    subscribeSuccess = (*t)->Subscribe();
                }
                else
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                        "):ConnectPubSubClient: Unsupported/Unknown bookmark handling by the backend. We will subscribe and get whatever we get best effort" );

                    subscribeSuccess = (*t)->Subscribe();
                }

                if ( !subscribeSuccess )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                        "):ConnectPubSubClient: Failed to subscribe to topic: " + (*t)->GetTopicName() );
                    return false;
                }
            }
        }
        ++t;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    PubSubSideChannelSettings* pubSubSideSettings = m_channelSettings.GetPubSubSideSettings( m_side );
    if ( GUCEF_NULL == pubSubSideSettings )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
            "):OnTaskStart: Unable to obtain settings for configured side \"" + m_side + "\". Cannot proceed" );        
        return false;
    }    
    m_sideSettings = pubSubSideSettings;
    
    m_runsInDedicatedThread = m_sideSettings->performPubSubInDedicatedThread; 
    COMCORE::CPubSubClientConfig& pubSubConfig = pubSubSideSettings->pubsubClientConfig;

    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
    m_metricsTimer->SetEnabled( m_channelSettings.collectMetrics );
    
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
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Successfully set a CPU affinity for logical CPU " + CORE::UInt32ToString( pubSubSideSettings->cpuAffinityForPubSubThread ) );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Failed to set a CPU affinity for logical CPU " + CORE::UInt32ToString( pubSubSideSettings->cpuAffinityForPubSubThread ) +
                    ". Proceeding without affinity");
            }
        }
    }

    if ( !ConnectPubSubClient() )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
            "):OnTaskStart: Failed initial connection attempt on task start, will rely on auto-reconnect" );
    }

    RegisterEventHandlers();

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientSide::IsRunningInDedicatedThread( void ) const
{GUCEF_TRACE;

    return m_runsInDedicatedThread;
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
            PublishMailboxMsgs();
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
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "CPubSubClientSide(" + CORE::PointerToString( this ) +
            "):LoadConfig: Unable to obtain settings for configured side \"" + m_side + "\". Cannot proceed" );        
        return false;
    }    

    m_sideSettings = pubSubSideSettings;    
    m_runsInDedicatedThread = m_sideSettings->performPubSubInDedicatedThread; 

    return true;
}

/*-------------------------------------------------------------------------*/

const ChannelSettings&
CPubSubClientSide::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}


/*-------------------------------------------------------------------------*/

CPubSubClientChannel::CPubSubClientChannel( void ) 
    : CPubSubClientSide( 'A' )
    , m_sideBPubSub( GUCEF_NULL )
{GUCEF_TRACE;

    // for now just work with a and b, we can do more sides later 
    m_sideBPubSub = new CPubSubClientSide( 'B' );
}

/*-------------------------------------------------------------------------*/

CPubSubClientChannel::~CPubSubClientChannel()
{GUCEF_TRACE;

    delete m_sideBPubSub;
    m_sideBPubSub = GUCEF_NULL;
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
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;    

    CPubSubClientSide::OnTaskCycle( taskData );    
    
    if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
    {
        m_sideBPubSub->OnTaskCycle( taskData );
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
    
    if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
    {
        m_sideBPubSub->OnTaskEnding( taskdata, willBeForced );
    }
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnded( CORE::CICloneable* taskData ,
                                   bool wasForced              )
{GUCEF_TRACE;

    CPubSubClientSide::OnTaskEnded( taskData, wasForced );    
    
    if ( !m_sideBPubSub->IsRunningInDedicatedThread() )
    {
        m_sideBPubSub->OnTaskEnded( taskData, wasForced );
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

    output.SetName( "info" );
    output.SetAttribute( "application", "pubsub2pubsub" );
    output.SetAttribute( "appBuildDateTime", PubSub2PubSub::GetAppCompileDateTime().ToIso8601DateTimeString( true, true ) );
    output.SetAttribute( "platformBuildDateTime", CORE::CDateTime::CompileDateTime().ToIso8601DateTimeString( true, true ) );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
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
        const CORE::CValueList& loadedConfig = m_app->GetAppConfig();
        return loadedConfig.SaveConfig( output );
    }
    else
    {
        const CORE::CDataNode& globalConfig = m_app->GetGlobalConfig();
        output.Copy( globalConfig );
        return true;
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
        CORE::CValueList loadedAppConfig;

        if ( isDeltaUpdateOnly )
        {
            // Grab a copy of the current app config
            loadedAppConfig = m_app->GetAppConfig();
            loadedAppConfig.SetAllowMultipleValues( false );
            loadedAppConfig.SetAllowDuplicates( false );
        }
        else
        {
            loadedAppConfig.CopySettingsFrom( m_app->GetAppConfig() );
        }

        if ( loadedAppConfig.LoadConfig( input ) )
        {
            if ( isDeltaUpdateOnly )
            {
                loadedAppConfig.SetAllowMultipleValues( m_app->GetAppConfig().GetAllowMultipleValues() );
                loadedAppConfig.SetAllowDuplicates( m_app->GetAppConfig().GetAllowDuplicates() );
            }

            // First put the app in standby mode before we mess with the settings
            if ( !m_app->SetStandbyMode( true ) )
                return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;

            const CORE::CDataNode& globalConfig = m_app->GetGlobalConfig();
            if ( m_app->LoadConfig( loadedAppConfig ) )
            {
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
                return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
            }
        }

        return TDeserializeState::DESERIALIZESTATE_CORRUPTEDINPUT;
    }
    else
    {
        if ( isDeltaUpdateOnly )
        {
            //// Grab a copy of the current global config
            //CORE::CDataNode globalConfigCopy = m_app->GetGlobalConfig();
            //if ( globalConfigCopy.Merge( input ) )
            //{
            //    const CORE::CValueList& loadedAppConfig = m_app->GetAppConfig();
            //    if ( m_app->LoadConfig( loadedAppConfig, globalConfigCopy ) )
            //    {
            //        return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
            //    }
            //    else
            //    {
            //        return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
            //    }
            //}

            return TDeserializeState::DESERIALIZESTATE_CORRUPTEDINPUT;
        }
        else
        {
            const CORE::CValueList& loadedAppConfig = m_app->GetAppConfig();
            if ( m_app->LoadConfig( input ) )
            {
                return TDeserializeState::DESERIALIZESTATE_SUCCEEDED;
            }
            else
            {
                return TDeserializeState::DESERIALIZESTATE_UNABLETOUPDATE;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

PubSub2PubSub::PubSub2PubSub( void )
    : CORE::CObserver()
    , CORE::CIConfigurable()
    , m_isInStandby( false )
    , m_globalStandbyEnabled( false )
    , m_udpStartPort()
    , m_channelCount()
    , m_storageStartChannelID()
    , m_redisStreamName()
    , m_redisHost()
    , m_redisPort()
    , m_channels()
    , m_channelSettings()
    , m_templateChannelSettings()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_transmitMetrics( true )
{GUCEF_TRACE;

    TEventCallback callback1( this, &PubSub2PubSub::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback1                      );
}

/*-------------------------------------------------------------------------*/

PubSub2PubSub::~PubSub2PubSub()
{GUCEF_TRACE;

    m_httpServer.Close();
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
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2PubSub: Opening REST API" );
        return m_httpServer.Listen();
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
PubSub2PubSub::LoadConfig( const CORE::CValueList& appConfig )
{GUCEF_TRACE;

    m_globalStandbyEnabled = CORE::StringToBool( appConfig.GetValueAlways( "GlobalStandbyEnabled" ), false );
    m_channelCount = CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "ChannelCount", "1" ) ) );
    m_storageStartChannelID = CORE::StringToInt32( CORE::ResolveVars(  appConfig.GetValueAlways( "StorageStartChannelID", "1" ) ) );

    bool applyCpuThreadAffinityByDefault = CORE::StringToBool( CORE::ResolveVars( appConfig.GetValueAlways( "ApplyCpuThreadAffinityByDefault" )  ), false );    
    CORE::UInt32 logicalCpuCount = CORE::GetLogicalCPUCount();

    CORE::UInt32 currentCpu = 0;
    CORE::Int32 maxChannelId = m_storageStartChannelID + m_channelCount;
    for ( CORE::Int32 channelId = m_storageStartChannelID; channelId < maxChannelId; ++channelId )
    {
        ChannelSettings* channelSettings = GUCEF_NULL;
        ChannelSettingsMap::iterator s = m_channelSettings.find( channelId );
        if ( s == m_channelSettings.end() )
        {
            channelSettings = &m_channelSettings[ channelId ];
            *channelSettings = m_templateChannelSettings;

            if ( -1 == channelSettings->channelId )
                channelSettings->channelId = channelId;
        }
        else
        {
            channelSettings = &m_channelSettings[ channelId ];
        }

        // Assign CPU affinity
        ChannelSettings::TCharToPubSubSideChannelSettingsMap::iterator i = channelSettings->pubSubSideChannelSettingsMap.begin();
        while ( i != channelSettings->pubSubSideChannelSettingsMap.end() )
        {
            PubSubSideChannelSettings& sideSettings = (*i).second;
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

            ++i;
        }
    }

    m_appConfig = appConfig;

    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort" ) ), 10000 ) );
    m_httpRouter.SetResourceMapping( "/info", RestApiPubSub2PubSubInfoResource::THTTPServerResourcePtr( new RestApiPubSub2PubSubInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiPubSub2PubSubConfigResource::THTTPServerResourcePtr( new RestApiPubSub2PubSubConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiPubSub2PubSubConfigResource::THTTPServerResourcePtr( new RestApiPubSub2PubSubConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping(  CORE::ResolveVars( appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ) ), RestApiPubSub2PubSubConfigResource::THTTPServerResourcePtr( new WEB::CDummyHTTPServerResource() )  );
    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

    return true;
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
PubSub2PubSub::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    TChannelCfgMap channelMap;

    CORE::CDataNode::TConstDataNodeSet channelParentCfgs = cfg.FindChildrenOfType( "Channels", true );
    CORE::CDataNode::TConstDataNodeSet::iterator i = channelParentCfgs.begin();
    while ( i != channelParentCfgs.end() )
    {
        CORE::CDataNode::const_iterator n = (*i)->ConstBegin();
        while ( n != (*i)->ConstEnd() )
        {
            const CORE::CString& channelIndex = (*n)->GetName();
            channelMap[ channelIndex ] = (*n)->FindChildrenOfType( "StorageChannel" );            
            ++n;
        }                
        ++i;
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSub2Storage:LoadConfig: Found " + CORE::ToString( channelMap.size() ) + " configuration entries for storage channels" );
    
    // load the template if any
    TChannelCfgMap::iterator m = channelMap.find( "*" );
    if ( m != channelMap.end() )
    {
        CORE::CDataNode::TConstDataNodeSet& matches = (*m).second;
        if ( !matches.empty() )
        {            
            if ( m_templateChannelSettings.LoadConfig( *(*matches.begin()) ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSub2Storage:LoadConfig: Successfully loaded template config for storage channels" );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSub2Storage:LoadConfig: Failed to correctly load template config for storage channels" );
                return false;
            }
        }
    }

    // load the specifically configured channels if any
    m = channelMap.begin();
    while ( m != channelMap.end() )
    {
        const CORE::CString& channelIndexStr = (*m).first;
        if ( channelIndexStr != '*' )
        {
            CORE::CDataNode::TConstDataNodeSet& matches = (*m).second;
            if ( !matches.empty() )
            {            
                CORE::Int32 channelIndex = CORE::StringToInt32( channelIndexStr );
                ChannelSettings& channelSettings = m_channelSettings[ channelIndex ];

                if ( channelSettings.LoadConfig( *(*matches.begin()) ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSub2Storage:LoadConfig: Successfully loaded explicit config for storage channels " + channelIndexStr );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSub2Storage:LoadConfig: Failed to correctly load explicit config for storage channels " + channelIndexStr );
                    return false;
                }
            }
        }
        ++m;
    }

    m_globalConfig.Copy( cfg );
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
PubSub2PubSub::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                    const CORE::CEvent& eventId  ,
                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CORE::Int32 channelId = m_storageStartChannelID;
    PubSubClientChannelMap::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        //const CPubSubClientChannel::ChannelMetrics& metrics = (*i).second->GetMetrics();
        //CORE::CString metricPrefix = "pubsub2pubsub.ch" + CORE::Int32ToString( channelId ) + ".";

        //GUCEF_METRIC_TIMING( metricPrefix + "redisErrorReplies", metrics.redisErrorReplies, 1.0f );
        //GUCEF_METRIC_TIMING( metricPrefix + "redisMessagesTransmitted", metrics.redisMessagesTransmitted, 1.0f );
        //GUCEF_METRIC_TIMING( metricPrefix + "redisPacketsInMessagesTransmitted", metrics.redisPacketsInMsgsTransmitted, 1.0f );
        //GUCEF_METRIC_GAUGE( metricPrefix + "redisPacketsInMessagesRatio", metrics.redisPacketsInMsgsRatio, 1.0f );
        //GUCEF_METRIC_GAUGE( metricPrefix + "redisTransmitQueueSize", metrics.redisTransmitQueueSize, 1.0f );
        //GUCEF_METRIC_TIMING( metricPrefix + "udpBytesReceived", metrics.udpBytesReceived, 1.0f );
        //GUCEF_METRIC_TIMING( metricPrefix + "udpPacketsReceived", metrics.udpPacketsReceived, 1.0f );

        ++i; ++channelId;
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList&
PubSub2PubSub::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
PubSub2PubSub::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/

