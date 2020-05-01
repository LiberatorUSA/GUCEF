/*
 *  Udp2Redis: service which pushes UDP packets into Redis
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
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

#ifndef GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefCOM_CDummyHTTPServerResource.h"
#define GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#include "ProcessMetrics.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    #include <winsock2.h>
#endif

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

RestApiProcessMetricsInfoResource::RestApiProcessMetricsInfoResource( ProcessMetrics* app )
    : COM::CCodecBasedHTTPServerResource()
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
RestApiProcessMetricsInfoResource::Serialize( CORE::CDataNode& output             ,
                                         const CORE::CString& representation )
{GUCEF_TRACE;

    output.SetName( "info" );
    output.SetAttribute( "application", "ProcessMetrics" );                                                                         
    output.SetAttribute( "buildDateTime", __TIMESTAMP__ );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiProcessMetricsConfigResource::RestApiProcessMetricsConfigResource( ProcessMetrics* app, bool appConfig )
    : COM::CCodecBasedHTTPServerResource()
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
RestApiProcessMetricsConfigResource::Serialize( CORE::CDataNode& output             ,
                                                const CORE::CString& representation )
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

ProcessMetrics::ProcessMetrics( void )
    : CORE::CObservingNotifier()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_gatherMemStats( true )
    , m_gatherCpuStats( true )
    , m_enableRestApi( true )
    , m_exeProcIdMap()
    , m_exeProcsToWatch()
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
{GUCEF_TRACE;

    RegisterEventHandlers();    
}

/*-------------------------------------------------------------------------*/

ProcessMetrics::~ProcessMetrics()
{GUCEF_TRACE;

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

    if ( m_exeProcsToWatch.size() == m_exeProcIdMap.size() )
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
                        CORE::TProcessId* prevPid = (*m).second;
                        (*m).second = CORE::CopyProcessId( pid ); 
                        CORE::FreeProcessId( prevPid );
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Refreshed pre-existing PID for \"" + exeName + "\"" );
                    }
                    else
                    {
                        m_exeProcIdMap[ exeName ] = CORE::CopyProcessId( pid );
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ProcessMetrics: Found PID for \"" + exeName + "\"" );
                    }
                }
            }
        }        
    }
    CORE::FreeProcessList( procIds );
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
            if ( OSWRAP_TRUE == CORE::GetProcessMemmoryUsage( (*m).second, &memUseInfo ) )
            {
                CORE::CString metricPrefix = "ProcessMetrics." + (*m).first;

                if ( m_gatherProcPageFaultCountInBytes )
                    GUCEF_METRIC_GAUGE( metricPrefix + ".MemUse.PageFaultCountInBytes", memUseInfo.pageFaultCountInBytes, 1.0f );
                if ( m_gatherProcPageFileUsageInBytes )
                    GUCEF_METRIC_GAUGE( metricPrefix + ".MemUse.PageFileUsageInBytes", memUseInfo.pageFileUsageInBytes, 1.0f );
                if ( m_gatherProcPeakPageFileUsageInBytes )
                    GUCEF_METRIC_GAUGE( metricPrefix + ".MemUse.PeakPageFileUsageInBytes", memUseInfo.peakPageFileUsageInBytes, 1.0f );
                if ( m_gatherProcPeakWorkingSetSizeInBytes )
                    GUCEF_METRIC_GAUGE( metricPrefix + ".MemUse.PeakWorkingSetSizeInBytes", memUseInfo.peakWorkingSetSizeInBytes, 1.0f );
                if ( m_gatherProcWorkingSetSizeInBytes )
                    GUCEF_METRIC_GAUGE( metricPrefix + ".MemUse.WorkingSetSizeInBytes", memUseInfo.workingSetSizeInBytes, 1.0f );
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
            CORE::TProcessId* pid = (*m).second; 
            CORE::FreeProcessId( pid );
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
                GUCEF_METRIC_GAUGE( "ProcessMetrics.MemUse.AvailablePageFileSizeInBytes", globMemInfo.availablePageFileSizeInBytes, 1.0f );
            if ( m_gatherGlobalPageFileUsageInBytes )
                GUCEF_METRIC_GAUGE( "ProcessMetrics.MemUse.AvailablePhysicalMemoryInBytes", globMemInfo.availablePhysicalMemoryInBytes, 1.0f );
            if ( m_gatherGlobalAvailableVirtualMemoryInBytes )
                GUCEF_METRIC_GAUGE( "ProcessMetrics.MemUse.AvailableVirtualMemoryInBytes", globMemInfo.availableVirtualMemoryInBytes, 1.0f );
            if ( m_gatherGlobalAvailExtendedVirtualMemoryInBytes )
                GUCEF_METRIC_GAUGE( "ProcessMetrics.MemUse.AvailExtendedVirtualMemoryInBytes", globMemInfo.availExtendedVirtualMemoryInBytes, 1.0f );
            if ( m_gatherGlobalMemoryLoadPercentage )
                GUCEF_METRIC_GAUGE( "ProcessMetrics.MemUse.MemoryLoadPercentage", (CORE::UInt64) globMemInfo.memoryLoadPercentage, 1.0f );
            if ( m_gatherGlobalTotalPageFileSizeInBytes )
                GUCEF_METRIC_GAUGE( "ProcessMetrics.MemUse.TotalPageFileSizeInBytes", globMemInfo.totalPageFileSizeInBytes, 1.0f );
            if ( m_gatherGlobalTotalPhysicalMemoryInBytes )
                GUCEF_METRIC_GAUGE( "ProcessMetrics.MemUse.TotalPhysicalMemoryInBytes", globMemInfo.totalPhysicalMemoryInBytes, 1.0f );
            if ( m_gatherGlobalTotalVirtualMemoryInBytes )
                GUCEF_METRIC_GAUGE( "ProcessMetrics.MemUse.TotalVirtualMemoryInBytes", globMemInfo.totalVirtualMemoryInBytes, 1.0f );
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

    m_metricsTimer.SetEnabled( true );

    if ( m_enableRestApi )
    {
        if ( m_httpServer.Listen() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics: Opened REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics: Startup completed successfully" );
            return true;
        }

        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics: Failed to open REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        return false;
    }
    else
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "ProcessMetrics: Startup completed successfully" );
        return true;
    }
}

/*-------------------------------------------------------------------------*/

bool 
ProcessMetrics::LoadConfig( const CORE::CValueList& appConfig   ,
                            const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;
        
    
    m_gatherCpuStats = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcCPUStats", "true" ) );
    m_enableRestApi = CORE::StringToBool( appConfig.GetValueAlways( "EnableRestApi", "true" ) );
    m_metricsTimer.SetInterval( CORE::StringToUInt32( appConfig.GetValueAlways( "MetricsGatheringIntervalInMs", "1000" ) ) );
    
    m_gatherMemStats = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcMemStats", "true" ) );
    m_gatherProcPageFaultCountInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcPageFaultCountInBytes", "true" ) );
    m_gatherProcPageFileUsageInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcPageFileUsageInBytes", "true" ) );
    m_gatherProcPeakPageFileUsageInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcPeakPageFileUsageInBytes", "true" ) );
    m_gatherProcPeakWorkingSetSizeInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcPeakWorkingSetSizeInBytes", "true" ) );
    m_gatherProcWorkingSetSizeInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherProcWorkingSetSizeInBytes", "true" ) );

    m_gatherGlobMemStats = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalMemStats", "true" ) );
    m_gatherGlobalAvailablePageFileSizeInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalAvailablePageFileSizeInBytes", "true" ) );
    m_gatherGlobalPageFileUsageInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalPageFileUsageInBytes", "true" ) );
    m_gatherGlobalAvailableVirtualMemoryInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalAvailableVirtualMemoryInBytes", "true" ) );
    m_gatherGlobalAvailExtendedVirtualMemoryInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalAvailExtendedVirtualMemoryInBytes", "true" ) );
    m_gatherGlobalMemoryLoadPercentage = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalMemoryLoadPercentage", "true" ) );
    m_gatherGlobalTotalPageFileSizeInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalTotalPageFileSizeInBytes", "true" ) );
    m_gatherGlobalTotalPhysicalMemoryInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalTotalPhysicalMemoryInBytes", "false" ) );
    m_gatherGlobalTotalVirtualMemoryInBytes = CORE::StringToBool( appConfig.GetValueAlways( "GatherGlobalTotalVirtualMemoryInBytes", "true" ) );

    TStringVector exeProcsToWatch = appConfig.GetValueAlways( "ExeProcsToWatch" ).ParseElements( ';', false );
    TStringVector::iterator i = exeProcsToWatch.begin();
    while ( i != exeProcsToWatch.end() )
    {
        m_exeProcsToWatch.insert( (*i) );
        ++i;
    }

    if ( m_enableRestApi )
    {
        m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort", "10000" ) ) ) );

        m_httpRouter.SetResourceMapping( "/info", RestApiProcessMetricsInfoResource::THTTPServerResourcePtr( new RestApiProcessMetricsInfoResource( this ) )  );
        m_httpRouter.SetResourceMapping( "/config/appargs", RestApiProcessMetricsInfoResource::THTTPServerResourcePtr( new RestApiProcessMetricsConfigResource( this, true ) )  );
        m_httpRouter.SetResourceMapping( "/config", RestApiProcessMetricsInfoResource::THTTPServerResourcePtr( new RestApiProcessMetricsConfigResource( this, false ) )  );
        m_httpRouter.SetResourceMapping(  appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ), RestApiProcessMetricsInfoResource::THTTPServerResourcePtr( new COM::CDummyHTTPServerResource() )  );
    
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