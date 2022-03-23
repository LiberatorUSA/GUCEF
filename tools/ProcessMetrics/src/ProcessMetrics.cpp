/*
 *  ProcessMetrics: Service for obtaining stats on a proc by name
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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#include "ProcessMetrics.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <winsock2.h>
#endif

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_COMCORE_CCOMCOREGLOBAL_H
#include "gucefCOMCORE_CComCoreGlobal.h"
#define GUCEF_COMCORE_CCOMCOREGLOBAL_H
#endif /* GUCEF_COMCORE_CCOMCOREGLOBAL_H ? */

#ifndef GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefWEB_CDummyHTTPServerResource.h"
#define GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

RestApiProcessMetricsInfoResource::RestApiProcessMetricsInfoResource( ProcessMetrics* app )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiProcessMetricsInfoResource::~RestApiProcessMetricsInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiProcessMetricsInfoResource::Serialize( const CORE::CString& resourcePath   ,
                                              CORE::CDataNode& output             ,
                                              const CORE::CString& representation ,
                                              const CORE::CString& params         )
{GUCEF_TRACE;

    static const CORE::CDateTime compileDt = CORE::CDateTime::CompileDateTime( __DATE__, __TIME__ );

    output.SetName( "info" );
    output.SetAttribute( "application", "ProcessMetrics" );
    output.SetAttribute( "buildDateTime", compileDt.ToIso8601DateTimeString( true, true ) );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiProcessMetricsConfigResource::RestApiProcessMetricsConfigResource( ProcessMetrics* app, bool appConfig )
    : WEB::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiProcessMetricsConfigResource::~RestApiProcessMetricsConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiProcessMetricsConfigResource::Serialize( const CORE::CString& resourcePath   ,
                                                CORE::CDataNode& output             ,
                                                const CORE::CString& representation ,
                                                const CORE::CString& params         )
{GUCEF_TRACE;

    if ( m_appConfig )
    {
        const CORE::CValueList& loadedConfig = m_app->GetAppConfig();
        return loadedConfig.SaveConfig( output );
    }

    const CORE::CDataNode& globalConfig = m_app->GetGlobalConfig();
    output.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

MetricThreshold::MetricThreshold( void )
    : CORE::CIConfigurable()
    , minThreshold()
    , maxThreshold()
    , applyMinThreshold( false )
    , applyMaxThreshold( true )
    , thresholdDescription()
    , metricName()
    , procFilter()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
MetricThreshold::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    cfg.SetAttribute( "minThreshold", minThreshold );
    cfg.SetAttribute( "maxThreshold", maxThreshold );
    cfg.SetAttribute( "applyMinThreshold", applyMinThreshold );
    cfg.SetAttribute( "applyMaxThreshold", applyMaxThreshold );
    cfg.SetAttribute( "thresholdDescription", thresholdDescription );
    cfg.SetAttribute( "metricName", metricName );

    CORE::CString procFilterStr;
    CORE::CString::StringSet::iterator i = procFilter.begin();
    while ( i != procFilter.end() )
    {
        if ( procFilterStr.IsNULLOrEmpty() )
        {
            procFilterStr = (*i);
        }
        else
        {
            procFilterStr += ';' + (*i);
        }
        ++i;
    }
    cfg.SetAttribute( "procFilter", procFilterStr );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
MetricThreshold::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    minThreshold = cfg.GetAttributeValueOrChildValueByName( "minThreshold", minThreshold );
    maxThreshold = cfg.GetAttributeValueOrChildValueByName( "maxThreshold", maxThreshold );
    applyMinThreshold = cfg.GetAttributeValueOrChildValueByName( "applyMinThreshold" ).AsBool( applyMinThreshold, true );
    applyMaxThreshold = cfg.GetAttributeValueOrChildValueByName( "applyMaxThreshold" ).AsBool( applyMaxThreshold, true );
    thresholdDescription = cfg.GetAttributeValueOrChildValueByName( "thresholdDescription" ).AsString( thresholdDescription, true );
    metricName = cfg.GetAttributeValueOrChildValueByName( "metricName" ).AsString( metricName, true );

    CORE::CString procFilterStr = cfg.GetAttributeValueOrChildValueByName( "procFilter" ).AsString( CORE::CString::Empty, true );
    procFilter = procFilterStr.ParseUniqueElements( ';', false );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
MetricThreshold::IsValid( void )
{GUCEF_TRACE;

    return ( applyMinThreshold || applyMaxThreshold ) && !metricName.IsNULLOrEmpty() && ( minThreshold.IsInitialized() || maxThreshold.IsInitialized() );
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
MetricThreshold::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "MetricThreshold";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

ProcessMetrics::ProcessMetrics( void )
    : CORE::CObservingNotifier()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_pubSubClient()
    , m_thresholdNotificationPublishTopic( GUCEF_NULL )
    , m_pubSubFeatures()
    , m_thresholdNotificationPrimaryPayloadCodecType()
    , m_gatherMemStats( true )
    , m_gatherCpuStats( true )
    , m_enableRestApi( true )
    , m_enableEventMsgPublishing( true )
    , m_exeProcIdMap()
    , m_exeProcsToWatch()
    , m_exeMatchPidMatchThreshold( 0 )
    , m_metricsThresholds()
    , m_procMetricsThresholds()
    , m_globalCpuDataPoint( GUCEF_NULL )
    , m_gatherProcPageFaultCountInBytes( true )
    , m_gatherProcPageFileUsageInBytes( true )
    , m_gatherProcPeakPageFileUsageInBytes( true )
    , m_gatherProcPeakWorkingSetSizeInBytes( true )
    , m_gatherProcWorkingSetSizeInBytes( true )
    , m_gatherGlobMemStats( true )
    , m_gatherGlobalAvailablePageFileSizeInBytes( true )
    , m_gatherGlobalPageFileUsageInBytes( true )
    , m_gatherGlobalAvailableVirtualMemoryInBytes( true )
    , m_gatherGlobalAvailExtendedVirtualMemoryInBytes( true )
    , m_gatherGlobalMemoryLoadPercentage( true )
    , m_gatherGlobalTotalPageFileSizeInBytes( true )
    , m_gatherGlobalTotalPhysicalMemoryInBytes( false )
    , m_gatherGlobalTotalVirtualMemoryInBytes( true )
    , m_gatherProcCpuUptime( true )
    , m_gatherProcCpuOverallPercentage( true )
    , m_gatherGlobalCpuStats( true )
    , m_gatherGlobalCpuCurrentFrequencyInMhz( true )
    , m_gatherGlobalCpuSpecMaxFrequencyInMhz( false )
    , m_gatherGlobalCpuMaxFrequencyInMhz( false )
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

ProcessMetrics::~ProcessMetrics()
{GUCEF_TRACE;

    SetStandbyMode( true );

    CORE::FreeCpuDataPoint( m_globalCpuDataPoint );
    m_globalCpuDataPoint = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
ProcessMetrics::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &ProcessMetrics::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );

}

/*-------------------------------------------------------------------------*/

void
ProcessMetrics::RefreshPIDs( void )
{GUCEF_TRACE;

    if ( m_exeProcsToWatch.size() == m_exeProcIdMap.size() ||
         ( m_exeMatchPidMatchThreshold > 0 && m_exeProcIdMap.size() >= (size_t) m_exeMatchPidMatchThreshold ) )
        return;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Refresing PID administration" );

    CORE::UInt32 procIdCount = 0;
    CORE::TProcessId* procIds = GUCEF_NULL;
    CORE::UInt32 retVal = CORE::GetProcessList( &procIds, &procIdCount );
    if ( OSWRAP_TRUE == retVal )
    {
        for ( CORE::UInt32 i=0; i<procIdCount; ++i )
        {
            CORE::TProcessId* pid = CORE::GetProcessIdAtIndex( procIds, i );

            CORE::CString exeName;
            if ( CORE::GetExeNameForProcessId( pid, exeName ) )
            {
                exeName = CORE::ExtractFilename( exeName );
                CORE::Int32 dotIndex = exeName.HasChar( '.', false );
                if ( dotIndex >= 0 )
                    exeName = exeName.SubstrToIndex( (CORE::UInt32) dotIndex, true );

                TStringSet::iterator n = m_exeProcsToWatch.find( exeName );
                if ( n != m_exeProcsToWatch.end() )
                {
                    TProcessIdMap::iterator m = m_exeProcIdMap.find( exeName );
                    if ( m != m_exeProcIdMap.end() )
                    {
                        TProcInfo& procInfo = (*m).second;
                        TProcInfo prevProcInfo = procInfo;
                        procInfo.pid = CORE::CopyProcessId( pid );
                        procInfo.previousProcCpuDataDataPoint = CORE::CreateProcCpuDataPoint( procInfo.pid );
                        CORE::FreeProcCpuDataPoint( prevProcInfo.previousProcCpuDataDataPoint );
                        CORE::FreeProcessId( prevProcInfo.pid );
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Refreshed pre-existing PID for \"" + exeName + "\"" );
                    }
                    else
                    {
                        TProcInfo& procInfo = m_exeProcIdMap[ exeName ];
                        procInfo.pid = CORE::CopyProcessId( pid );
                        procInfo.previousProcCpuDataDataPoint = CORE::CreateProcCpuDataPoint( procInfo.pid );
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Found PID for \"" + exeName + "\"" );
                    }
                }
            }
        }
    }
    CORE::FreeProcessList( procIds );
}

/*-------------------------------------------------------------------------*/

bool
ProcessMetrics::SetupPubSubClient( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    const CORE::CDataNode* pubSubClientCfgNode = cfg.Find( "PubSubClientConfig" );
    if ( GUCEF_NULL != pubSubClientCfgNode )
    {
        COMCORE::CPubSubClientConfig pubSubClientCfg;
        if ( pubSubClientCfg.LoadConfig( *pubSubClientCfgNode ) )
        {
            m_pubSubClient = COMCORE::CComCoreGlobal::Instance()->GetPubSubClientFactory().Create( pubSubClientCfg.pubsubClientType, pubSubClientCfg );
            if ( !m_pubSubClient.IsNULL() )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics:SetupPubSubClient: Successfully instantiated pub-sub client of type: " + pubSubClientCfg.pubsubClientType );
                m_pubSubClient->GetSupportedFeatures( m_pubSubFeatures );

                // This app really only has topics for one purpose which is sending event messages
                // As such we just grab the first config'd topic as the topic to do that on vs having yet another config setting
                CORE::CString::StringSet topicNameList;
                m_pubSubClient->GetConfiguredTopicNameList( topicNameList );
                if ( !topicNameList.empty() )
                {
                    m_thresholdNotificationPublishTopic = m_pubSubClient->GetTopicAccess( *topicNameList.begin() );
                    if ( GUCEF_NULL == m_thresholdNotificationPublishTopic )
                    {
                        m_thresholdNotificationPublishTopic = m_pubSubClient->CreateTopicAccess( *topicNameList.begin() );
                    }
                    if ( GUCEF_NULL != m_thresholdNotificationPublishTopic )
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics:SetupPubSubClient: Successfully obtained access to topic with name: " + *topicNameList.begin() );
                        return true;
                    }
                    else
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics:SetupPubSubClient: Instantiated pub-sub client has topic configured, but unable to obtain access. topicName=" + *topicNameList.begin() );
                    }
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics:SetupPubSubClient: Instantiated pub-sub client has no topics configured" );
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics:SetupPubSubClient: Failed to instantiate pub-sub client from config data. pubsubClientType=" + pubSubClientCfg.pubsubClientType );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics:SetupPubSubClient: Failed to load config data from config node \"PubSubClientConfig\"" );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics:SetupPubSubClient: No config node \"PubSubClientConfig\" could be located" );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
ProcessMetrics::PublishMetricThresholdExceeded( const CORE::CVariant& metricValue ,
                                                const CORE::CString& metricName   ,
                                                const CORE::CString& procName     ,
                                                const MetricThreshold& threshold  )
{GUCEF_TRACE;

    if ( m_pubSubClient.IsNULL() || GUCEF_NULL == m_thresholdNotificationPublishTopic )
        return;

    COMCORE::CBasicPubSubMsg msg;

    msg.AddKeyValuePair( "ProcessMetrics.EventMsgType", "MetricThresholdExceeded" );
    msg.AddKeyValuePair( "ProcessMetrics.MetricName", metricName );
    msg.AddKeyValuePair( "ProcessMetrics.MetricValue", metricValue );
    if ( threshold.applyMinThreshold )
        msg.AddKeyValuePair( "ProcessMetrics.MetricMinThreshold", threshold.minThreshold );
    if ( threshold.applyMaxThreshold )
        msg.AddKeyValuePair( "ProcessMetrics.MetricMaxThreshold", threshold.maxThreshold );
    if ( !procName.IsNULLOrEmpty() )
        msg.AddKeyValuePair( "ProcessMetrics.ProcName", procName );

    // if we have support for key-value let's use that since it doesnt require a consumer to use special parsing
    if ( !m_pubSubFeatures.supportsKeyValueSetPerMsg || !m_pubSubFeatures.supportsAbsentPrimaryPayloadPerMsg )
    {
        // @TODO: It would be better if we could source this from the codec itself
        int payloadVarType = GUCEF_DATATYPE_BINARY_BLOB;
        if ( "json" == m_thresholdNotificationPrimaryPayloadCodecType ||
             "xml" == m_thresholdNotificationPrimaryPayloadCodecType   )
        {
            payloadVarType = GUCEF_DATATYPE_UTF8_STRING;
        }

        if ( !msg.MoveKeyValuePairsToEncodedPrimaryPayload( m_thresholdNotificationPrimaryPayloadCodecType, payloadVarType ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics:PublishMetricThresholdExceeded: Failed to encode primary payload on event message. Aborting" );
            return;
        }
    }

    CORE::UInt64 publishActionId = 0;
    if ( m_thresholdNotificationPublishTopic->Publish( publishActionId, msg, true ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics:PublishMetricThresholdExceeded: Published metric threshold exceeded event message" );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics:PublishMetricThresholdExceeded: Failed to publish event message" );
    }
}

/*-------------------------------------------------------------------------*/

void
ProcessMetrics::ValidateMetricThresholds( const CORE::CVariant& metricValue ,
                                          const CORE::CString& metricName   ,
                                          const CORE::CString& procName     )
{GUCEF_TRACE;

    if ( !m_enableEventMsgPublishing )
        return;

    TMetricsThresholdMap::iterator i = m_metricsThresholds.find( metricName );
    if ( i != m_metricsThresholds.end() )
    {
        MetricThreshold& thresholds = (*i).second;

        if ( thresholds.applyMinThreshold )
        {
            if ( metricValue <= thresholds.minThreshold )
            {
                PublishMetricThresholdExceeded( metricValue, metricName, procName, thresholds );
            }
        }
        if ( thresholds.applyMaxThreshold )
        {
            if ( metricValue >= thresholds.maxThreshold )
            {
                PublishMetricThresholdExceeded( metricValue, metricName, procName, thresholds );
            }
        }
    }

    TMetricsThresholdMapMap::iterator n = m_procMetricsThresholds.find( procName );
    if ( n != m_procMetricsThresholds.end() )
    {
        TMetricsThresholdMap& thresholdMapForProc = (*n).second;
        TMetricsThresholdMap::const_iterator m = thresholdMapForProc.find( metricName );
        if ( m != thresholdMapForProc.end() )
        {
            const MetricThreshold& thresholds = (*m).second;

            if ( thresholds.applyMinThreshold )
            {
                if ( metricValue <= thresholds.minThreshold )
                {
                    PublishMetricThresholdExceeded( metricValue, metricName, procName, thresholds );
                }
            }
            if ( thresholds.applyMaxThreshold )
            {
                if ( metricValue >= thresholds.maxThreshold )
                {
                    PublishMetricThresholdExceeded( metricValue, metricName, procName, thresholds );
                }
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
ProcessMetrics::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventId  ,
                                     CORE::CICloneable* eventData )
{GUCEF_TRACE;

    RefreshPIDs();

    if ( m_gatherMemStats )
    {
        TStringSet failedProcs;
        TProcessIdMap::iterator m = m_exeProcIdMap.begin();
        while ( m != m_exeProcIdMap.end() )
        {
            CORE::TProcessMemoryUsageInfo memUseInfo;
            if ( OSWRAP_TRUE == CORE::GetProcessMemoryUsage( (*m).second.pid, &memUseInfo ) )
            {
                const CORE::CString& procName = (*m).first;
                CORE::CString metricPrefix = "ProcessMetrics." + procName + '.';

                if ( m_gatherProcPageFaultCountInBytes )
                {
                    static const CORE::CString metricName = "MemUse.PageFaultCountInBytes";
                    GUCEF_METRIC_TIMING( metricPrefix + metricName, memUseInfo.pageFaultCountInBytes, 1.0f );
                    ValidateMetricThresholds( CORE::CVariant( memUseInfo.pageFaultCountInBytes ), metricName, procName );
                }
                if ( m_gatherProcPageFileUsageInBytes )
                {
                    static const CORE::CString metricName = "MemUse.PageFileUsageInBytes";
                    GUCEF_METRIC_TIMING( metricPrefix + metricName, memUseInfo.pageFileUsageInBytes, 1.0f );
                    ValidateMetricThresholds( CORE::CVariant( memUseInfo.pageFileUsageInBytes ), metricName, procName );
                }
                if ( m_gatherProcPeakPageFileUsageInBytes )
                {
                    static const CORE::CString metricName = "MemUse.PeakPageFileUsageInBytes";
                    GUCEF_METRIC_TIMING( metricPrefix + metricName, memUseInfo.peakPageFileUsageInBytes, 1.0f );
                    ValidateMetricThresholds( CORE::CVariant( memUseInfo.peakPageFileUsageInBytes ), metricName, procName );
                }
                if ( m_gatherProcPeakWorkingSetSizeInBytes )
                {
                    static const CORE::CString metricName = "MemUse.PeakWorkingSetSizeInBytes";
                    GUCEF_METRIC_TIMING( metricPrefix + metricName, memUseInfo.peakWorkingSetSizeInBytes, 1.0f );
                    ValidateMetricThresholds( CORE::CVariant( memUseInfo.peakWorkingSetSizeInBytes ), metricName, procName );
                }
                if ( m_gatherProcWorkingSetSizeInBytes )
                {
                    static const CORE::CString metricName = "MemUse.WorkingSetSizeInBytes";
                    GUCEF_METRIC_TIMING( metricPrefix + metricName, memUseInfo.workingSetSizeInBytes, 1.0f );
                    ValidateMetricThresholds( CORE::CVariant( memUseInfo.workingSetSizeInBytes ), metricName, procName );
                }
            }
            else
            {
                failedProcs.insert( (*m).first );
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Failed to obtain memory stats for \"" + (*m).first + "\"" );
            }
            ++m;
        }

        TStringSet::iterator i = failedProcs.begin();
        while ( i != failedProcs.end() )
        {
            m = m_exeProcIdMap.find( (*i) );
            CORE::FreeProcCpuDataPoint( (*m).second.previousProcCpuDataDataPoint );
            CORE::FreeProcessId( (*m).second.pid );
            m_exeProcIdMap.erase( m );

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Erased PID for \"" + (*i) + "\"" );
            ++i;
        }
    }

    if ( m_gatherGlobalCpuStats )
    {
        CORE::TCpuStats* cpuStats = GUCEF_NULL;
        if ( OSWRAP_TRUE == CORE::GetCpuStats( m_globalCpuDataPoint, &cpuStats ) && GUCEF_NULL != cpuStats )
        {
            for ( CORE::UInt32 i=0; i<cpuStats->logicalCpuCount; ++i )
            {
                CORE::CString lCpuMetricPrefix = "ProcessMetrics.Cpu." + CORE::ToString( i ) +  ".";
                CORE::TLogicalCpuStats* lCpuStats = &cpuStats->logicalCpuStats[ i ];

                if ( m_gatherGlobalCpuCurrentFrequencyInMhz )
                {
                    // check if the O/S was able to provide this value.
                    // If not disable the stat. Unsupported stats return 0
                    if ( lCpuStats->cpuCurrentFrequencyInMhz > 0.5 )
                    {
                        static const CORE::CString metricName = "CpuCurrentFrequencyInMhz";
                        GUCEF_METRIC_GAUGE( lCpuMetricPrefix + metricName, lCpuStats->cpuCurrentFrequencyInMhz, 1.0f );
                        ValidateMetricThresholds( CORE::CVariant( lCpuStats->cpuCurrentFrequencyInMhz ), metricName, CORE::CString::Empty );
                    }
                    else
                    {
                        m_gatherGlobalCpuCurrentFrequencyInMhz = false;
                    }
                }
                if ( m_gatherGlobalCpuSpecMaxFrequencyInMhz )
                {
                    // check if the O/S was able to provide this value.
                    // If not disable the stat. Unsupported stats return 0
                    if ( lCpuStats->cpuCurrentFrequencyInMhz > 0.5 )
                    {
                        static const CORE::CString metricName = "CpuSpecMaxFrequencyInMhz";
                        GUCEF_METRIC_GAUGE( lCpuMetricPrefix + metricName, lCpuStats->cpuSpecMaxFrequencyInMhz, 1.0f );
                        ValidateMetricThresholds( CORE::CVariant( lCpuStats->cpuSpecMaxFrequencyInMhz ), metricName, CORE::CString::Empty );
                    }
                    else
                    {
                        m_gatherGlobalCpuSpecMaxFrequencyInMhz = false;
                    }
                }
                if ( m_gatherGlobalCpuMaxFrequencyInMhz )
                {
                    // check if the O/S was able to provide this value.
                    // If not disable the stat. Unsupported stats return 0
                    if ( lCpuStats->cpuCurrentFrequencyInMhz > 0.5 )
                    {
                        static const CORE::CString metricName = "CpuMaxFrequencyInMhz";
                        GUCEF_METRIC_GAUGE( lCpuMetricPrefix + metricName, lCpuStats->cpuMaxFrequencyInMhz, 1.0f );
                        ValidateMetricThresholds( CORE::CVariant( lCpuStats->cpuMaxFrequencyInMhz ), metricName, CORE::CString::Empty );
                    }
                    else
                    {
                        m_gatherGlobalCpuMaxFrequencyInMhz = false;
                    }
                }
            }
        }
    }

    if ( m_gatherCpuStats )
    {
        TStringSet failedProcs;
        TProcessIdMap::iterator m = m_exeProcIdMap.begin();
        while ( m != m_exeProcIdMap.end() )
        {
            CORE::TProcessCpuUsageInfo cpuUseInfo;
            if ( OSWRAP_TRUE == CORE::GetProcessCpuUsage( (*m).second.pid, (*m).second.previousProcCpuDataDataPoint, &cpuUseInfo ) )
            {
                const CORE::CString& procName = (*m).first;
                CORE::CString metricPrefix = "ProcessMetrics." + procName + '.';

                if ( m_gatherProcCpuUptime )
                {
                    static const CORE::CString metricName = "CpuUse.UptimeInMs";
                    GUCEF_METRIC_GAUGE( metricPrefix + metricName, cpuUseInfo.uptimeInMs, 1.0f );
                    ValidateMetricThresholds( CORE::CVariant( cpuUseInfo.uptimeInMs ), metricName, procName );
                }
                if ( m_gatherProcCpuOverallPercentage )
                {
                    static const CORE::CString metricName = "CpuUse.TotalCpuUsePercentage";
                    GUCEF_METRIC_GAUGE( metricPrefix + metricName, cpuUseInfo.overallCpuConsumptionPercentage, 1.0f );
                    ValidateMetricThresholds( CORE::CVariant( cpuUseInfo.overallCpuConsumptionPercentage ), metricName, procName );
                }
            }
            else
            {
                failedProcs.insert( (*m).first );
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Failed to obtain cpu stats for \"" + (*m).first + "\"" );
            }
            ++m;
        }

        TStringSet::iterator i = failedProcs.begin();
        while ( i != failedProcs.end() )
        {
            m = m_exeProcIdMap.find( (*i) );
            CORE::FreeProcCpuDataPoint( (*m).second.previousProcCpuDataDataPoint );
            CORE::FreeProcessId( (*m).second.pid );
            m_exeProcIdMap.erase( m );

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Erased PID for \"" + (*i) + "\"" );
            ++i;
        }
    }

    if ( m_gatherGlobMemStats )
    {
        CORE::TGlobalMemoryUsageInfo globMemInfo;
        if ( OSWRAP_TRUE == CORE::GetGlobalMemoryUsage( &globMemInfo ) )
        {
            if ( m_gatherGlobalAvailablePageFileSizeInBytes )
            {
                static const CORE::CString metricName = "ProcessMetrics.MemUse.AvailablePageFileSizeInBytes";
                GUCEF_METRIC_TIMING( metricName, globMemInfo.availablePageFileSizeInBytes, 1.0f );
                ValidateMetricThresholds( CORE::CVariant( globMemInfo.availablePageFileSizeInBytes ), metricName, CORE::CString::Empty );
            }
            if ( m_gatherGlobalPageFileUsageInBytes )
            {
                static const CORE::CString metricName = "ProcessMetrics.MemUse.AvailablePhysicalMemoryInBytes";
                GUCEF_METRIC_TIMING( metricName, globMemInfo.availablePhysicalMemoryInBytes, 1.0f );
                ValidateMetricThresholds( CORE::CVariant( globMemInfo.availablePhysicalMemoryInBytes ), metricName, CORE::CString::Empty );
            }
            if ( m_gatherGlobalAvailableVirtualMemoryInBytes )
            {
                static const CORE::CString metricName = "ProcessMetrics.MemUse.AvailableVirtualMemoryInBytes";
                GUCEF_METRIC_TIMING( metricName, globMemInfo.availableVirtualMemoryInBytes, 1.0f );
                ValidateMetricThresholds( CORE::CVariant( globMemInfo.availableVirtualMemoryInBytes ), metricName, CORE::CString::Empty );
            }
            if ( m_gatherGlobalAvailExtendedVirtualMemoryInBytes )
            {
                static const CORE::CString metricName = "ProcessMetrics.MemUse.AvailExtendedVirtualMemoryInBytes";
                GUCEF_METRIC_TIMING( metricName, globMemInfo.availExtendedVirtualMemoryInBytes, 1.0f );
                ValidateMetricThresholds( CORE::CVariant( globMemInfo.availExtendedVirtualMemoryInBytes ), metricName, CORE::CString::Empty );
            }
            if ( m_gatherGlobalMemoryLoadPercentage )
            {
                static const CORE::CString metricName = "ProcessMetrics.MemUse.MemoryLoadPercentage";
                GUCEF_METRIC_TIMING( metricName, (CORE::UInt64) globMemInfo.memoryLoadPercentage, 1.0f );
                ValidateMetricThresholds( CORE::CVariant( globMemInfo.memoryLoadPercentage ), metricName, CORE::CString::Empty );
            }
            if ( m_gatherGlobalTotalPageFileSizeInBytes )
            {
                static const CORE::CString metricName = "ProcessMetrics.MemUse.TotalPageFileSizeInBytes";
                GUCEF_METRIC_TIMING( metricName, globMemInfo.totalPageFileSizeInBytes, 1.0f );
                ValidateMetricThresholds( CORE::CVariant( globMemInfo.totalPageFileSizeInBytes ), metricName, CORE::CString::Empty );
            }
            if ( m_gatherGlobalTotalPhysicalMemoryInBytes )
            {
                static const CORE::CString metricName = "ProcessMetrics.MemUse.TotalPhysicalMemoryInBytes";
                GUCEF_METRIC_GAUGE( metricName, globMemInfo.totalPhysicalMemoryInBytes, 1.0f );
                ValidateMetricThresholds( CORE::CVariant( globMemInfo.totalPhysicalMemoryInBytes ), metricName, CORE::CString::Empty );
            }
            if ( m_gatherGlobalTotalVirtualMemoryInBytes )
            {
                static const CORE::CString metricName = "ProcessMetrics.MemUse.TotalVirtualMemoryInBytes";
                GUCEF_METRIC_TIMING( metricName, globMemInfo.totalVirtualMemoryInBytes, 1.0f );
                ValidateMetricThresholds( CORE::CVariant( globMemInfo.totalVirtualMemoryInBytes ), metricName, CORE::CString::Empty );
            }
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Failed to obtain global memory stats" );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
ProcessMetrics::Start( void )
{GUCEF_TRACE;

    if ( m_enableRestApi )
    {
        if ( m_httpServer.Listen() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics: Opened REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics: Failed to open REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        }
    }

    if ( SetStandbyMode( false ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics: Startup completed successfully" );
        return true;
    }

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics: Startup failed" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
ProcessMetrics::SetStandbyMode( bool newModeIsStandby )
{GUCEF_TRACE;

    m_metricsTimer.SetEnabled( !newModeIsStandby );

    if ( newModeIsStandby )
    {
        if ( !m_exeProcIdMap.empty() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics:SetStandbyMode: Cleaning up process information" );
            TProcessIdMap::iterator m = m_exeProcIdMap.begin();
            while ( !m_exeProcIdMap.empty() )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Erasing info for \"" + (*m).first + "\"" );

                CORE::FreeProcCpuDataPoint( (*m).second.previousProcCpuDataDataPoint );
                CORE::FreeProcessId( (*m).second.pid );
                m_exeProcIdMap.erase( m );
            }
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
ProcessMetrics::IsGlobalStandbyEnabled( void ) const
{GUCEF_TRACE;

    return m_metricsTimer.GetEnabled();
}

/*-------------------------------------------------------------------------*/

bool
ProcessMetrics::LoadConfig( const CORE::CValueList& appConfig   ,
                            const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    m_gatherCpuStats = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcCPUStats" ), m_gatherCpuStats );
    m_enableRestApi = CORE::StringToBool( appConfig.GetValueAlways( "EnableRestApi" ), m_enableRestApi );
    m_enableEventMsgPublishing = CORE::StringToBool( appConfig.GetValueAlways( "EnableEventMsgPublishing" ), m_enableEventMsgPublishing );
    m_metricsTimer.SetInterval( CORE::StringToUInt32( appConfig.GetValueAlways( "MetricsGatheringIntervalInMs" ), 1000 ) );
    m_exeMatchPidMatchThreshold = CORE::StringToUInt32( appConfig.GetValueAlways( "ExeMatchPidMatchThreshold" ), m_exeMatchPidMatchThreshold );

    m_gatherMemStats = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcMemStats" ), m_gatherMemStats );
    m_gatherProcPageFaultCountInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcPageFaultCountInBytes" ), m_gatherProcPageFaultCountInBytes );
    m_gatherProcPageFileUsageInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcPageFileUsageInBytes" ), m_gatherProcPageFileUsageInBytes );
    m_gatherProcPeakPageFileUsageInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcPeakPageFileUsageInBytes" ), m_gatherProcPeakPageFileUsageInBytes );
    m_gatherProcPeakWorkingSetSizeInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcPeakWorkingSetSizeInBytes" ), m_gatherProcPeakWorkingSetSizeInBytes );
    m_gatherProcWorkingSetSizeInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcWorkingSetSizeInBytes" ), m_gatherProcWorkingSetSizeInBytes );

    m_gatherGlobMemStats = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalMemStats" ), m_gatherGlobMemStats );
    m_gatherGlobalAvailablePageFileSizeInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalAvailablePageFileSizeInBytes" ), m_gatherGlobalAvailablePageFileSizeInBytes );
    m_gatherGlobalPageFileUsageInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalPageFileUsageInBytes" ), m_gatherGlobalPageFileUsageInBytes );
    m_gatherGlobalAvailableVirtualMemoryInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalAvailableVirtualMemoryInBytes" ), m_gatherGlobalAvailableVirtualMemoryInBytes );
    m_gatherGlobalAvailExtendedVirtualMemoryInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalAvailExtendedVirtualMemoryInBytes" ), m_gatherGlobalAvailExtendedVirtualMemoryInBytes );
    m_gatherGlobalMemoryLoadPercentage = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalMemoryLoadPercentage" ), m_gatherGlobalMemoryLoadPercentage );
    m_gatherGlobalTotalPageFileSizeInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalTotalPageFileSizeInBytes" ), m_gatherGlobalTotalPageFileSizeInBytes );
    m_gatherGlobalTotalPhysicalMemoryInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalTotalPhysicalMemoryInBytes" ), m_gatherGlobalTotalPhysicalMemoryInBytes );
    m_gatherGlobalTotalVirtualMemoryInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalTotalVirtualMemoryInBytes" ), m_gatherGlobalTotalVirtualMemoryInBytes );

    m_gatherProcCpuUptime = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcCPUUptime" ), m_gatherProcCpuUptime );
    m_gatherProcCpuOverallPercentage = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcCpuOverallPercentage" ), m_gatherProcCpuOverallPercentage );

    m_gatherGlobalCpuStats = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalCpuStats" ), m_gatherGlobalCpuStats );
    m_gatherGlobalCpuCurrentFrequencyInMhz = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalCpuCurrentFrequencyInMhz" ), m_gatherGlobalCpuCurrentFrequencyInMhz );
    m_gatherGlobalCpuSpecMaxFrequencyInMhz = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalCpuSpecMaxFrequencyInMhz" ), m_gatherGlobalCpuSpecMaxFrequencyInMhz );
    m_gatherGlobalCpuMaxFrequencyInMhz = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalCpuMaxFrequencyInMhz" ), m_gatherGlobalCpuMaxFrequencyInMhz );

    TStringVector exeProcsToWatch = appConfig.GetValueAlways( "ExeProcsToWatch" ).AsString().ParseElements( ';', false );
    TStringVector::iterator i = exeProcsToWatch.begin();
    while ( i != exeProcsToWatch.end() )
    {
        m_exeProcsToWatch.insert( (*i) );
        ++i;
    }

    if ( m_gatherGlobalCpuStats )
    {
        m_globalCpuDataPoint = CORE::CreateCpuDataPoint();
    }

    if ( m_enableEventMsgPublishing )
    {
        m_metricsThresholds.clear();
        m_procMetricsThresholds.clear();

        CORE::CDataNode::TConstDataNodeSet metricsThresholdsCfgs = globalConfig.FindChildrenOfType( "MetricThreshold", true );
        CORE::CDataNode::TConstDataNodeSet::const_iterator c = metricsThresholdsCfgs.begin();
        while ( c != metricsThresholdsCfgs.end() )
        {
            MetricThreshold threshold;
            if ( threshold.LoadConfig( *(*c) ) )
            {
                if ( threshold.IsValid() )
                {
                    if ( threshold.procFilter.empty() )
                    {
                        m_metricsThresholds[ threshold.metricName ] = threshold;
                    }
                    else
                    {
                        CORE::CString::StringSet::iterator n = threshold.procFilter.begin();
                        while ( n != threshold.procFilter.end() )
                        {
                            m_procMetricsThresholds[ (*n) ][ threshold.metricName ] = threshold;
                            ++n;
                        }
                    }
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Metric threshold has invalid entry" );
                    // continue best effort, at least we'd have metrics
                }
            }
            ++c;
        }

        m_thresholdNotificationPrimaryPayloadCodecType =  appConfig.GetValueAlways( "ThresholdNotificationPrimaryPayloadCodecType", "json" );
        if ( !m_thresholdNotificationPrimaryPayloadCodecType.IsNULLOrEmpty() )
        {
            if ( !CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().IsRegistered( m_thresholdNotificationPrimaryPayloadCodecType ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Codec for encoding the threshold exceeded notifications is not registered. CodeType=" + m_thresholdNotificationPrimaryPayloadCodecType );
                // continue best effort, at least we'd have metrics
            }
        }

        if ( !SetupPubSubClient( globalConfig ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Failed set up Pub/Sub client. Event messaging will not be possible" );
            // continue best effort, at least we'd have metrics
        }
    }

    if ( m_enableRestApi )
    {
        m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort" ) ), 10000 ) );

        m_httpRouter.SetResourceMapping( "/info", RestApiProcessMetricsInfoResource::THTTPServerResourcePtr( new RestApiProcessMetricsInfoResource( this ) )  );
        m_httpRouter.SetResourceMapping( "/config/appargs", RestApiProcessMetricsInfoResource::THTTPServerResourcePtr( new RestApiProcessMetricsConfigResource( this, true ) )  );
        m_httpRouter.SetResourceMapping( "/config", RestApiProcessMetricsInfoResource::THTTPServerResourcePtr( new RestApiProcessMetricsConfigResource( this, false ) )  );
        m_httpRouter.SetResourceMapping(  appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ), RestApiProcessMetricsInfoResource::THTTPServerResourcePtr( new WEB::CDummyHTTPServerResource() )  );

        m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );
    }

    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList&
ProcessMetrics::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
ProcessMetrics::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/
