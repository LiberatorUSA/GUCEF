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

#ifndef GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefWEB_CDummyHTTPServerResource.h"
#define GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H ? */

#include "pubsub2storage.h"

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
#define GUCEF_DEFAULT_MAX_DEDICATED_REDIS_WRITER_MAIL_BULK_READ     100
#define GUCEF_DEFAULT_PUBSUB_RECONNECT_DELAY_IN_MS                  100

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( void )
    : pubsubClientConfig()
    , pubsubClientTopicConfigs()
    , channelId( -1 )
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , performPubSubInDedicatedThread( true )
    , maxSizeOfDedicatedPubSubBulkMailRead( GUCEF_DEFAULT_MAX_DEDICATED_REDIS_WRITER_MAIL_BULK_READ )
    , applyThreadCpuAffinity( false )
    , cpuAffinityForDedicatedPubSubThread( 0 )
    , cpuAffinityForMainChannelThread( 0 )
    , collectMetrics( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings::ChannelSettings( const ChannelSettings& src )
    : pubsubClientConfig( src.pubsubClientConfig )
    , pubsubClientTopicConfigs( src.pubsubClientTopicConfigs )
    , channelId( src.channelId )
    , ticketRefillOnBusyCycle( src.ticketRefillOnBusyCycle )
    , performPubSubInDedicatedThread( src.performPubSubInDedicatedThread )
    , maxSizeOfDedicatedPubSubBulkMailRead( src.maxSizeOfDedicatedPubSubBulkMailRead )
    , applyThreadCpuAffinity( src.applyThreadCpuAffinity )
    , cpuAffinityForDedicatedPubSubThread( src.cpuAffinityForDedicatedPubSubThread )
    , cpuAffinityForMainChannelThread( src.cpuAffinityForMainChannelThread )
    , collectMetrics( src.collectMetrics )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

ChannelSettings&
ChannelSettings::operator=( const ChannelSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        pubsubClientConfig = src.pubsubClientConfig;
        pubsubClientTopicConfigs = src.pubsubClientTopicConfigs;
        channelId = src.channelId;
        ticketRefillOnBusyCycle = src.ticketRefillOnBusyCycle;
        performPubSubInDedicatedThread = src.performPubSubInDedicatedThread;
        maxSizeOfDedicatedPubSubBulkMailRead = src.maxSizeOfDedicatedPubSubBulkMailRead;
        applyThreadCpuAffinity = src.applyThreadCpuAffinity;
        cpuAffinityForDedicatedPubSubThread = src.cpuAffinityForDedicatedPubSubThread;
        cpuAffinityForMainChannelThread = src.cpuAffinityForMainChannelThread;
        collectMetrics = src.collectMetrics;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
ChannelSettings::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    tree.SetAttribute( "channelId", channelId );
    tree.SetAttribute( "ticketRefillOnBusyCycle", ticketRefillOnBusyCycle );
    tree.SetAttribute( "performPubSubInDedicatedThread", performPubSubInDedicatedThread );
    tree.SetAttribute( "maxSizeOfDedicatedPubSubBulkMailRead", maxSizeOfDedicatedPubSubBulkMailRead );
    tree.SetAttribute( "applyThreadCpuAffinity", applyThreadCpuAffinity );
    tree.SetAttribute( "cpuAffinityForDedicatedPubSubThread", cpuAffinityForDedicatedPubSubThread );
    tree.SetAttribute( "cpuAffinityForMainChannelThread", cpuAffinityForMainChannelThread );
    tree.SetAttribute( "collectMetrics", collectMetrics );

    CORE::CDataNode* psClientConfig = tree.Structure( "PubSubClientConfig", '/' );
    if ( !pubsubClientConfig.SaveConfig( *psClientConfig ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:SaveConfig: config is malformed, failed to save a mandatory PubSubClientConfig section" );
        return false;        
    }
    
    CORE::CDataNode::TDataNodeSet psClientTopicConfigs = tree.StructureMultiple( "PubSubClientTopicConfig"                      ,
                                                                                 '/'                                            , 
                                                                                 true                                           , 
                                                                                 (CORE::UInt32) pubsubClientTopicConfigs.size() ,
                                                                                 (CORE::UInt32) pubsubClientTopicConfigs.size() );
    
    TTopicConfigVector::const_iterator n = pubsubClientTopicConfigs.begin();
    CORE::CDataNode::TDataNodeSet::iterator i = psClientTopicConfigs.begin();
    while ( i != psClientTopicConfigs.end() )
    {
        CORE::CDataNode* psClientTopicConfig = (*i);
        if ( !(*n).SaveConfig( *psClientTopicConfig ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:SaveConfig: config is malformed, failed to save a mandatory PubSubClientTopicConfig section" );
            return false;
        }
        ++i; ++n;            
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
ChannelSettings::LoadConfig( const CORE::CDataNode& tree )
{GUCEF_TRACE;

    const CORE::CDataNode* psClientConfig = tree.Find( "PubSubClientConfig" );
    if ( GUCEF_NULL != psClientConfig )
    {
        if ( !pubsubClientConfig.LoadConfig( *psClientConfig ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: config is unacceptable, failed to load mandatory PubSubClientConfig section" );
            return false;
        }

        // There is no sane default of pubsubClientType since it depends on the clients loaded into the application
        // as such this is a mandatory setting to provide
        if ( pubsubClientConfig.pubsubClientType.IsNULLOrEmpty() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: config is malformed, \"pubsubClientType\" was not provided" );
            return false;
        }    
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: config is malformed, a PubSubClientConfig section is mandatory" );
        return false;
    }

    CORE::CDataNode::TConstDataNodeSet psClientTopicConfigs = tree.FindChildrenOfType( "PubSubClientTopicConfig", true );
    if ( !psClientTopicConfigs.empty() )
    {
        CORE::UInt32 n=0;
        pubsubClientTopicConfigs.resize( psClientTopicConfigs.size() );
        
        CORE::CDataNode::TConstDataNodeSet::iterator i = psClientTopicConfigs.begin();
        while ( i != psClientTopicConfigs.end() )
        {
            COMCORE::CPubSubClientTopicConfig& topicConfig = pubsubClientTopicConfigs[ n ];            
            if ( !topicConfig.LoadConfig( *(*i) ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: config is unacceptable, failed to load PubSubClientTopicConfig section" );
                return false;
            }
            ++i; ++n;            
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ChannelSettings:LoadConfig: config is malformed, having at least one PubSubClientTopicConfig section is mandatory" );
        return false;
    }
    
    channelId = CORE::StringToInt32( CORE::ResolveVars( tree.GetAttributeValueOrChildValueByName( "channelId" ) ), channelId );
    ticketRefillOnBusyCycle = CORE::StringToUInt32( CORE::ResolveVars( tree.GetAttributeValueOrChildValueByName( "ticketRefillOnBusyCycle" ) ), ticketRefillOnBusyCycle );
    performPubSubInDedicatedThread = CORE::StringToBool( CORE::ResolveVars( tree.GetAttributeValueOrChildValueByName( "performPubSubInDedicatedThread" ) ), performPubSubInDedicatedThread );
    maxSizeOfDedicatedPubSubBulkMailRead = CORE::StringToUInt32( CORE::ResolveVars( tree.GetAttributeValueOrChildValueByName( "maxSizeOfDedicatedPubSubBulkMailRead" ) ), maxSizeOfDedicatedPubSubBulkMailRead );
    applyThreadCpuAffinity = CORE::StringToBool( CORE::ResolveVars( tree.GetAttributeValueOrChildValueByName( "applyThreadCpuAffinity" ) ), applyThreadCpuAffinity );
    cpuAffinityForDedicatedPubSubThread = CORE::StringToUInt32( CORE::ResolveVars( tree.GetAttributeValueOrChildValueByName( "cpuAffinityForDedicatedPubSubThread" ) ), cpuAffinityForDedicatedPubSubThread );
    cpuAffinityForMainChannelThread = CORE::StringToUInt32( CORE::ResolveVars( tree.GetAttributeValueOrChildValueByName( "cpuAffinityForMainChannelThread" ) ), cpuAffinityForMainChannelThread );
    collectMetrics = CORE::StringToBool( CORE::ResolveVars( tree.GetAttributeValueOrChildValueByName( "collectMetrics" ) ), collectMetrics );
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

CPubSubClientChannel::CPubSubClientChannel()
    : CORE::CTaskConsumer()
    , m_pubsubClient()
    , m_topics()
    , m_channelSettings()
    , m_mailbox()
    , m_bulkMail()
    , m_metricsTimer( GUCEF_NULL )
    , m_pubsubClientReconnectTimer( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientChannel::~CPubSubClientChannel()
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CPubSubClientChannel::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );

    if ( GUCEF_NULL != m_pubsubClientReconnectTimer )
    {
        TEventCallback callback( this, &CPubSubClientChannel::OnPubSubClientReconnectTimerCycle );
        SubscribeTo( m_pubsubClientReconnectTimer   ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubClientChannel::GetType( void ) const
{GUCEF_TRACE;

    return "PubSubClientChannel";
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventId  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnPubSubClientReconnectTimerCycle( CORE::CNotifier* notifier    ,
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

bool
CPubSubClientChannel::DisconnectPubSubClient( bool destroyClient )
{GUCEF_TRACE;

    if ( m_pubsubClient.IsNULL() )
        return true;

    if ( !m_pubsubClient->Disconnect() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
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
CPubSubClientChannel::ConnectPubSubClient( void )
{GUCEF_TRACE;

    if ( !DisconnectPubSubClient() )
        return false;

    if ( m_pubsubClient.IsNULL() )
    {
        // Create and configure the pub-sub client
        m_channelSettings.pubsubClientConfig.pulseGenerator = GetPulseGenerator();
        m_pubsubClient = COMCORE::CComCoreGlobal::Instance()->GetPubSubClientFactory().Create( m_channelSettings.pubsubClientConfig.pubsubClientType, m_channelSettings.pubsubClientConfig );

        if ( m_pubsubClient.IsNULL() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                "):OnTaskStart: Failed to create a pub-sub client of type \"" + m_channelSettings.pubsubClientConfig.pubsubClientType + "\". Cannot proceed" );        
            return false;
        }

        COMCORE::CPubSubClientFeatures clientFeatures;
        m_pubsubClient->GetSupportedFeatures( clientFeatures );
        
        if ( !clientFeatures.supportsAutoReconnect )
        {
            if ( GUCEF_NULL != m_pubsubClientReconnectTimer )
                m_pubsubClientReconnectTimer = new CORE::CTimer( *GetPulseGenerator(), m_channelSettings.pubsubClientConfig.reconnectDelayInMs );
        }
    }

    SubscribeTo( m_pubsubClient.GetPointerAlways() );
    if ( !m_pubsubClient->Connect() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):ConnectPubSubClient: Failed to connect the pub-sub client" );
        return false;
    }

    // Create and configure the pub-sub client's topics
    m_topics.clear();
    m_topics.reserve( m_channelSettings.pubsubClientTopicConfigs.size() );
    ChannelSettings::TTopicConfigVector::iterator i = m_channelSettings.pubsubClientTopicConfigs.begin();
    while ( i != m_channelSettings.pubsubClientTopicConfigs.end() )
    {
        COMCORE::CPubSubClientTopic* topic = m_pubsubClient->CreateTopicAccess( (*i) );
        if ( GUCEF_NULL == topic )
        {
            if ( !(*i).isOptional )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Failed to create a pub-sub client topic access for topic \"" + (*i).topicName + "\". Cannot proceed" );
                return false;            
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Unable to create a pub-sub client topic access for optional topic \"" + (*i).topicName + "\". Proceeding" );
            }
        }

        SubscribeTo( topic );
        m_topics.push_back( topic );
        ++i;
    }

    TopicVector::iterator t = m_topics.begin();
    while ( t != m_topics.end() )
    {
        (*t)->Connect();
        ++t;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
    m_metricsTimer->SetEnabled( m_channelSettings.pubsubClientConfig.desiredFeatures.supportsMetrics );
    
    if ( m_channelSettings.performPubSubInDedicatedThread )
    {
        // Set the minimum number of cycles we will go full speed if a single cycle was not enough to handle
        // all the processing. This will cause a bypass of CPU yielding if/when the situation arises.
        // In such a case the thread will run at max speed for a least the below set nr of cycles.
        GetPulseGenerator()->RequestPulsesPerImmediatePulseRequest( m_channelSettings.ticketRefillOnBusyCycle );
        
        // Default smallest pulse delta at 10ms
        GetPulseGenerator()->RequestPeriodicPulses( this, 10 ); 

        if ( m_channelSettings.applyThreadCpuAffinity )
        {
            if ( SetCpuAffinityByCpuId( m_channelSettings.cpuAffinityForDedicatedPubSubThread ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Successfully set a CPU affinity for logical CPU " + CORE::UInt32ToString( m_channelSettings.cpuAffinityForDedicatedPubSubThread ) );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
                    "):OnTaskStart: Failed to set a CPU affinity for logical CPU " + CORE::UInt32ToString( m_channelSettings.cpuAffinityForDedicatedPubSubThread ) +
                    ". Proceeding without affinity");
            }
        }
    }

    if ( !ConnectPubSubClient() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientChannel(" + CORE::PointerToString( this ) +
            "):OnTaskStart: Successfully set a CPU affinity for logical CPU " + CORE::UInt32ToString( m_channelSettings.cpuAffinityForDedicatedPubSubThread ) );
    }

    RegisterEventHandlers();

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubClientChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;
    
    if ( m_channelSettings.performPubSubInDedicatedThread )
    {
        //m_bulkMail.clear();
        //if ( m_mailbox.GetMailList( m_bulkMail, m_channelSettings.maxSizeOfDedicatedPubSubBulkMailRead ) )
        //{
        //    m_bulkPackets.clear();
        //    m_bulkPacketCounts.clear();

        //    TBufferMailbox::TMailList::iterator i = m_bulkMail.begin();
        //    while ( !IsDeactivationRequested() && i != m_bulkMail.end() )
        //    {
        //        CORE::UInt32 packetCount = (*i).eventid;
        //        TCloneablePacketEntryVector* udpPackets = static_cast< TCloneablePacketEntryVector* >( (*i).data );
        //        if ( GUCEF_NULL != udpPackets )
        //        {
        //            m_bulkPackets.push_back( udpPackets );
        //            m_bulkPacketCounts.push_back( packetCount );
        //        }
        //        ++i;
        //    }

        //    if ( !IsDeactivationRequested() )
        //        RedisSendSync( m_bulkPackets, m_bulkPacketCounts );

        //    i = m_bulkMail.begin();
        //    while ( i != m_bulkMail.end() )
        //    {
        //        delete static_cast< TCloneablePacketEntryVector* >( (*i).data );
        //        ++i;
        //    }
        //}

        //// If we have the mailbox filling up we should burst for more speed
        //if ( m_channelSettings.maxSizeOfDedicatedRedisWriterBulkMailRead > 0                        &&  
        //     m_mailbox.AmountOfMail() > m_channelSettings.maxSizeOfDedicatedRedisWriterBulkMailRead  )
        //{
        //    GetPulseGenerator()->RequestImmediatePulse();
        //}
    }

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnding( CORE::CICloneable* taskdata ,
                                    bool willBeForced           )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

void
CPubSubClientChannel::OnTaskEnded( CORE::CICloneable* taskData ,
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
CPubSubClientChannel::LoadConfig( const ChannelSettings& channelSettings )
{GUCEF_TRACE;

    m_channelSettings = channelSettings;
    return true;
}

/*-------------------------------------------------------------------------*/

const ChannelSettings&
CPubSubClientChannel::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}

/*-------------------------------------------------------------------------*/

CStorageChannel::CStorageChannel()
    : CORE::CTaskConsumer()
    , m_channelSettings()
    , m_metricsTimer( GUCEF_NULL )
    , m_metrics()
    , m_pubsubClient( new CPubSubClientChannel() )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStorageChannel::CStorageChannel( const CStorageChannel& src )
    : CORE::CTaskConsumer()
    , m_channelSettings( src.m_channelSettings )
    , m_metricsTimer( GUCEF_NULL )
    , m_metrics()
    , m_pubsubClient( new CPubSubClientChannel() )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStorageChannel::~CStorageChannel()
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CStorageChannel::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback6( this, &CStorageChannel::OnMetricsTimerCycle );
    SubscribeTo( m_metricsTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback6                      );
}

/*-------------------------------------------------------------------------*/

bool
CStorageChannel::LoadConfig( const ChannelSettings& channelSettings )
{GUCEF_TRACE;

    m_channelSettings = channelSettings;
    return m_pubsubClient->LoadConfig( channelSettings );
}

/*-------------------------------------------------------------------------*/

const ChannelSettings&
CStorageChannel::GetChannelSettings( void ) const
{GUCEF_TRACE;

    return m_channelSettings;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CStorageChannel::GetType( void ) const
{GUCEF_TRACE;

    return "StorageChannel";
}

/*-------------------------------------------------------------------------*/

bool
CStorageChannel::WaitForTaskToFinish( CORE::Int32 timeoutInMs )
{GUCEF_TRACE;

    // Overriding the base class implementation because this consumer can start its own
    // consumer based on settings transparent to the caller.
    if ( CTaskConsumer::WaitForTaskToFinish( timeoutInMs ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "StorageChannel:WaitForTaskToFinish: Successfully waited for channel " + CORE::Int32ToString( m_channelSettings.channelId ) + "'s task to stop" );
        if ( m_channelSettings.performPubSubInDedicatedThread )
        {
            if ( m_pubsubClient->WaitForTaskToFinish( timeoutInMs ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "StorageChannel:WaitForTaskToFinish: Successfully waited for channel " + CORE::Int32ToString( m_channelSettings.channelId ) + "'s dedicated pub sub task to stop" );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "StorageChannel:WaitForTaskToFinish: Failed waiting for dedicated pub sub task to stop for channel " + CORE::Int32ToString( m_channelSettings.channelId ) );
            }
        }
        return true;
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "StorageChannel:WaitForTaskToFinish: Failed waiting for task to stop for channel " + CORE::Int32ToString( m_channelSettings.channelId ) );
    return false;
}

/*-------------------------------------------------------------------------*/

CStorageChannel::ChannelMetrics::ChannelMetrics( void )
    //: udpBytesReceived( 0 )
    //, udpPacketsReceived( 0 )
    //, redisMessagesTransmitted( 0 )
    //, redisPacketsInMsgsTransmitted( 0 )
    //, redisPacketsInMsgsRatio( 0 )
    //, redisTransmitQueueSize( 0 )
    //, redisErrorReplies( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CStorageChannel::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventId  ,
                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;

    //m_metrics.udpBytesReceived = m_udpSocket->GetBytesReceived( true );
    //m_metrics.udpPacketsReceived = m_udpSocket->GetNrOfDataReceivedEvents( true );
    //m_metrics.redisTransmitQueueSize = m_redisWriter->GetRedisTransmitQueueSize();
    //m_metrics.redisMessagesTransmitted = m_redisWriter->GetRedisMsgsTransmittedCounter( true );
    //m_metrics.redisPacketsInMsgsTransmitted = m_redisWriter->GetRedisPacketsInMsgsTransmittedCounter( true );
    //m_metrics.redisPacketsInMsgsRatio = m_redisWriter->GetRedisPacketsInMsgsRatio();
    //m_metrics.redisErrorReplies = m_redisWriter->GetRedisErrorRepliesCounter( true );
}

/*-------------------------------------------------------------------------*/

const CStorageChannel::ChannelMetrics&
CStorageChannel::GetMetrics( void ) const
{GUCEF_TRACE;

    return m_metrics;
}

/*-------------------------------------------------------------------------*/

bool
CStorageChannel::OnTaskStart( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    if ( m_channelSettings.performPubSubInDedicatedThread )
    {
        CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
        if ( !threadPool->StartTask( m_pubsubClient ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StorageChannel:OnTaskStart: Failed to start dedicated task (dedicated thread) for pub-sub. Falling back to a single thread" );
            m_channelSettings.performPubSubInDedicatedThread = false;
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StorageChannel:OnTaskStart: Successfully requested the launch of a dedicated task (dedicated thread) for pub-sub" );
        }
    }

    if ( !m_channelSettings.performPubSubInDedicatedThread )
    {
        if ( !m_pubsubClient->OnTaskStart( taskData ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StorageChannel:OnTaskStart: Failed startup of pub-sub client logic" );
            return false;
        }
    }

	//m_udpSocket = new COMCORE::CUDPSocket( *GetPulseGenerator(), false );
 //   m_metricsTimer = new CORE::CTimer( *GetPulseGenerator(), 1000 );
 //   m_metricsTimer->SetEnabled( m_channelSettings.collectMetrics );

 //   // Set the minimum number of cycles we will go full speed if a single cycle was not enough to handle
 //   // all the processing. This will cause a bypass of CPU yielding if/when the situation arises.
 //   // In such a case the thread will run at max speed for a least the below set nr of cycles.
 //   GetPulseGenerator()->RequestPulsesPerImmediatePulseRequest( m_channelSettings.ticketRefillOnBusyCycle );
 //   
 //   // Default smallest pulse delta at 10ms
 //   GetPulseGenerator()->RequestPeriodicPulses( this, 10 ); 

 //   if ( m_channelSettings.applyThreadCpuAffinity )
 //   {
 //       if ( SetCpuAffinityByCpuId( m_channelSettings.cpuAffinityForMainChannelThread ) )
 //       {
 //           GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StorageChannel(" + CORE::PointerToString( this ) +
 //               "):OnTaskStart: Successfully set a CPU affinity for logical CPU " + CORE::UInt32ToString( m_channelSettings.cpuAffinityForMainChannelThread ) );
 //       }
 //       else
 //       {
 //           GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StorageChannel(" + CORE::PointerToString( this ) +
 //               "):OnTaskStart: Failed to set a CPU affinity for logical CPU " + CORE::UInt32ToString( m_channelSettings.cpuAffinityForMainChannelThread ) +
 //               ". Proceeding without affinity");
 //       }
 //   }

    //RegisterEventHandlers();

  //  // Configure and open the UDP port.
  //  // Note that if there is an error here we will just keep on trying automatically
  //  m_udpSocket->SetMaxUpdatesPerCycle( m_channelSettings.udpSocketUpdateCyclesPerPulse );
  //  m_udpSocket->SetOsLevelSocketReceiveBufferSize( m_channelSettings.udpSocketOsReceiveBufferSize );
  //  m_udpSocket->SetNrOfReceiveBuffers( m_channelSettings.nrOfUdpReceiveBuffersPerSocket );
  //  m_udpSocket->SetAutoReOpenOnError( true );
  //  if ( m_udpSocket->Open( m_channelSettings.udpInterface ) )
  //  {
		//GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StorageChannel:OnTaskStart: Successfully opened UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
  //  }
  //  else
  //  {
  //      GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "StorageChannel:OnTaskStart: Failed to open UDP socket on " + m_channelSettings.udpInterface.AddressAndPortAsString() );
  //  }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStorageChannel::OnTaskCycle( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    if ( !m_channelSettings.performPubSubInDedicatedThread )
    {
        m_pubsubClient->OnTaskCycle( taskData );
    }

    // We are never 'done' so return false
    return false;
}

/*-------------------------------------------------------------------------*/

void
CStorageChannel::OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           )
{GUCEF_TRACE;

    if ( !m_channelSettings.performPubSubInDedicatedThread )
    {
        m_pubsubClient->OnTaskEnding( taskdata, willBeForced );
    }
    else
    {
        // Since we are the ones that launched the dedicated Redis write thread we should also ask
        // to have it cleaned up when we are shutting down this thread
        CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
        if ( !threadPool->RequestTaskToStop( m_pubsubClient.StaticCast< CORE::CTaskConsumer >(), false ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StorageChannel:OnTaskEnding: Failed to request the dedicated task (dedicated thread) for pub-sub to stop" );
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StorageChannel:OnTaskEnding: Successfully requested the dedicated task (dedicated thread) for pub-sub to stop" );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CStorageChannel::OnTaskEnded( CORE::CICloneable* taskData ,
                              bool wasForced              )
{GUCEF_TRACE;

    delete m_metricsTimer;
    m_metricsTimer = GUCEF_NULL;

    if ( !m_channelSettings.performPubSubInDedicatedThread )
    {
        m_pubsubClient->OnTaskEnded( taskData, wasForced );
    }

    CORE::CTaskConsumer::OnTaskEnded( taskData, wasForced );
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2StorageInfoResource::RestApiPubSub2StorageInfoResource( PubSub2Storage* app )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2StorageInfoResource::~RestApiPubSub2StorageInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiPubSub2StorageInfoResource::Serialize( const CORE::CString& resourcePath   ,
                                              CORE::CDataNode& output             ,
                                              const CORE::CString& representation ,
                                              const CORE::CString& params         )
{GUCEF_TRACE;

    output.SetName( "info" );
    output.SetAttribute( "application", "pubsub2storage" );
    output.SetAttribute( "buildDateTime", __TIMESTAMP__ );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2StorageConfigResource::RestApiPubSub2StorageConfigResource( PubSub2Storage* app, bool appConfig )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
    m_allowDeserialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiPubSub2StorageConfigResource::~RestApiPubSub2StorageConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiPubSub2StorageConfigResource::Serialize( const CORE::CString& resourcePath   ,
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

RestApiPubSub2StorageConfigResource::TDeserializeState
RestApiPubSub2StorageConfigResource::Deserialize( const CORE::CString& resourcePath   ,
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
                    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "RestApiUdp2RedisConfigResource: IsGlobalStandbyEnabled is true. We will leave the app in standby mode" );
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

PubSub2Storage::PubSub2Storage( void )
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

    TEventCallback callback1( this, &PubSub2Storage::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback1                      );
}

/*-------------------------------------------------------------------------*/

PubSub2Storage::~PubSub2Storage()
{GUCEF_TRACE;

    m_httpServer.Close();
}

/*-------------------------------------------------------------------------*/

bool
PubSub2Storage::IsGlobalStandbyEnabled( void ) const
{GUCEF_TRACE;

    return m_globalStandbyEnabled;
}

/*-------------------------------------------------------------------------*/

bool
PubSub2Storage::Start( void )
{GUCEF_TRACE;

    m_isInStandby = true;
    bool errorOccured = !SetStandbyMode( m_globalStandbyEnabled );

    if ( !errorOccured )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage: Opening REST API" );
        return m_httpServer.Listen();
    }
    return !errorOccured;
}

/*-------------------------------------------------------------------------*/

bool
PubSub2Storage::SetStandbyMode( bool putInStandbyMode )
{GUCEF_TRACE;

    // Check if we need to do anything
    if ( m_isInStandby == putInStandbyMode )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage:SetStandbyMode( " + CORE::BoolToString( putInStandbyMode ) + " ): Already in the desired mode (" + CORE::BoolToString( m_isInStandby ) + "), nothing to do" );
        return true;
    }

    if ( putInStandbyMode )
    {
        bool totalSuccess = true;
        CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();

        // Signal all channel threads to stop gracefully
        // Since this standby operation is global not per channel we signal all to stop before
        // we starting any waiting operation
        StorageChannelMap::iterator i = m_channels.begin();
        while ( i != m_channels.end() )
        {
            CStorageChannelPtr channel = (*i).second;
            if ( !threadPool->RequestTaskToStop( channel.StaticCast< CORE::CTaskConsumer >(), false ) )
            {
                totalSuccess = false;
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage:SetStandbyMode( true ): Failed to signal task to stop for channel " + CORE::Int32ToString( channel->GetChannelSettings().channelId ) )
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage:SetStandbyMode( true ): Requested channel " + CORE::Int32ToString( channel->GetChannelSettings().channelId ) + "'s task to stop" );
            }
            ++i;
        }

        // Now actually wait for the threads to be finished
        i = m_channels.begin();
        while ( i != m_channels.end() )
        {
            CStorageChannelPtr channel = (*i).second;
            if ( !channel->WaitForTaskToFinish( 30000 ) )
            {
                totalSuccess = false;
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage:SetStandbyMode( true ): Failed to signal task to stop for channel " + CORE::Int32ToString( channel->GetChannelSettings().channelId ) )
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage:SetStandbyMode( true ): Successfully waited for channel " + CORE::Int32ToString( channel->GetChannelSettings().channelId ) + "'s task to stop" );
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
        StorageChannelMap::iterator i = m_channels.begin();
        while ( i != m_channels.end() )
        {
            CORE::Int32 channelId = (*i).first;
            ChannelSettingsMap::iterator n = m_channelSettings.find( channelId );
            if ( n == m_channelSettings.end() )
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage:SetStandbyMode( false ): Found channel which no longer has corresponding channel settings, deleting channel with ID " + CORE::Int32ToString( channelId ) );
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
            StorageChannelMap::iterator i = m_channels.find( channelId );
            if ( i == m_channels.end() )
            {
                // This is a brand new channel. Lets add the channel object for it
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage:SetStandbyMode( false ): Found channel settings whith no corresponding channel object, creating new channel with ID " + CORE::Int32ToString( channelId ) );
                m_channels[ channelId ] = CStorageChannelPtr( new CStorageChannel() );
            }
            ++n;
        }

        CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();

        n = m_channelSettings.begin();
        while ( n != m_channelSettings.end() )
        {
            CORE::Int32 channelId = (*n).first;
            StorageChannelMap::iterator i = m_channels.find( channelId );
            if ( i != m_channels.end() )
            {
                const ChannelSettings& channelSettings = (*n).second;
                CStorageChannelPtr channel = (*i).second;

                if ( !channel->LoadConfig( channelSettings ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage::SetStandbyMode( false ): Failed to set channel settings on channel " + CORE::Int32ToString( channelId ) );
                    totalSuccess = false;
                    break;
                }

                if ( !threadPool->StartTask( channel ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "PubSub2Storage::SetStandbyMode( false ): Failed to start task (dedicated thread) for channel " + CORE::Int32ToString( channelId ) );
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
PubSub2Storage::LoadConfig( const CORE::CValueList& appConfig )
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

        if ( channelSettings->applyThreadCpuAffinity || applyCpuThreadAffinityByDefault )
        {
            channelSettings->cpuAffinityForMainChannelThread = currentCpu;

            ++currentCpu;
            if ( currentCpu >= logicalCpuCount ) // Wrap around if we run out of CPUs
                currentCpu = 0;

            if ( channelSettings->performPubSubInDedicatedThread )
            {
                channelSettings->cpuAffinityForDedicatedPubSubThread = currentCpu;

                ++currentCpu;
                if ( currentCpu >= logicalCpuCount ) // Wrap around if we run out of CPUs
                    currentCpu = 0;
            }
        }
    }

    m_appConfig = appConfig;

    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort" ) ), 10000 ) );
    m_httpRouter.SetResourceMapping( "/info", RestApiPubSub2StorageInfoResource::THTTPServerResourcePtr( new RestApiPubSub2StorageInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiPubSub2StorageConfigResource::THTTPServerResourcePtr( new RestApiPubSub2StorageConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiPubSub2StorageConfigResource::THTTPServerResourcePtr( new RestApiPubSub2StorageConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping(  CORE::ResolveVars( appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ) ), RestApiPubSub2StorageConfigResource::THTTPServerResourcePtr( new WEB::CDummyHTTPServerResource() )  );
    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
PubSub2Storage::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    // not fully supported right now

    tree.Copy( m_globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
PubSub2Storage::LoadConfig( const CORE::CDataNode& cfg )
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

const CORE::CString& 
PubSub2Storage::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "PubSub2Storage";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
PubSub2Storage::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventId  ,
                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;

    CORE::Int32 channelId = m_storageStartChannelID;
    StorageChannelMap::iterator i = m_channels.begin();
    while ( i != m_channels.end() )
    {
        const CStorageChannel::ChannelMetrics& metrics = (*i).second->GetMetrics();
        CORE::CString metricPrefix = "pubsub2storage.ch" + CORE::Int32ToString( channelId ) + ".";

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
PubSub2Storage::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
PubSub2Storage::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/
