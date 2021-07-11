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

#include <queue>

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#endif /* GUCEF_COMCORE_CTCPCLIENTSOCKET_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENT_H
#include "gucefCOMCORE_CPubSubClient.h"
#define GUCEF_COMCORE_CPUBSUBCLIENT_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#include "gucefCOMCORE_CPubSubClientFactory.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFACTORY_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_WEB_CHTTPSERVER_H
#include "gucefWEB_CHTTPServer.h"
#define GUCEF_WEB_CHTTPSERVER_H
#endif /* GUCEF_WEB_CHTTPSERVER_H ? */

#ifndef GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H
#include "gucefWEB_CDefaultHTTPServerRouter.h"
#define GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H
#endif /* GUCEF_WEB_CDEFAULTHTTPSERVERROUTER_H ? */

#ifndef GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefWEB_CCodecBasedHTTPServerResource.h"
#define GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class ProcessMetrics;

class RestApiProcessMetricsInfoResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiProcessMetricsInfoResource( ProcessMetrics* app );

    virtual ~RestApiProcessMetricsInfoResource();

    virtual bool Serialize( const CORE::CString& resourcePath   ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    ProcessMetrics* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiProcessMetricsConfigResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiProcessMetricsConfigResource( ProcessMetrics* app, bool appConfig );

    virtual ~RestApiProcessMetricsConfigResource();

    virtual bool Serialize( const CORE::CString& resourcePath   , 
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    ProcessMetrics* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class MetricThreshold : public CORE::CIConfigurable
{
    public:

    CORE::CVariant minThreshold;
    CORE::CVariant maxThreshold;
    bool applyMinThreshold;
    bool applyMaxThreshold;
    CORE::CString thresholdDescription;    
    CORE::CString metricName;
    CORE::CString::StringSet procFilter;

    MetricThreshold( void );

    bool IsValid( void );
    
    virtual bool SaveConfig( CORE::CDataNode& cfg ) const;

    virtual bool LoadConfig( const CORE::CDataNode& cfg );

    virtual const CORE::CString& GetClassTypeName( void ) const;

};

/*-------------------------------------------------------------------------*/

class ProcessMetrics : public CORE::CObservingNotifier
{
    public:

    ProcessMetrics( void );
    virtual ~ProcessMetrics();

    bool Start( void );

    bool SetStandbyMode( bool newMode );

    bool IsGlobalStandbyEnabled( void ) const;

    bool LoadConfig( const CORE::CValueList& appConfig   ,
                     const CORE::CDataNode& globalConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    private:

    void RegisterEventHandlers( void );

    void RefreshPIDs( void );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void ValidateMetricThresholds( const CORE::CVariant& metricValue ,
                                   const CORE::CString& metricName   ,
                                   const CORE::CString& procName     );
    
    void PublishMetricThresholdExceeded( const CORE::CVariant& metricValue ,
                                         const CORE::CString& metricName   ,
                                         const CORE::CString& procName     ,
                                         const MetricThreshold& threshold  );
    
    bool SetupPubSubClient( const CORE::CDataNode& cfg );
    
    private:

    struct SProcInfo
    {
        CORE::TProcessId* pid;
        CORE::TProcCpuDataPoint* previousProcCpuDataDataPoint;
    };
    typedef struct SProcInfo TProcInfo;

    typedef CORE::CTEventHandlerFunctor< ProcessMetrics > TEventCallback;
    typedef std::map< CORE::CString, TProcInfo > TProcessIdMap;
    typedef std::set< CORE::CString > TStringSet;
    typedef std::vector< CORE::CString > TStringVector;
    typedef std::map< CORE::CString, MetricThreshold >  TMetricsThresholdMap;
    typedef std::map< CORE::CString, TMetricsThresholdMap >  TMetricsThresholdMapMap;

    WEB::CHTTPServer m_httpServer;
    WEB::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;
    CORE::CTimer m_metricsTimer;
    COMCORE::CPubSubClientFactory::TProductPtr m_pubSubClient;
    COMCORE::CPubSubClientTopic* m_thresholdNotificationPublishTopic;
    COMCORE::CPubSubClientFeatures m_pubSubFeatures;
    CORE::CString m_thresholdNotificationPrimaryPayloadCodecType;
    bool m_gatherMemStats;
    bool m_gatherCpuStats;
    bool m_enableRestApi;
    bool m_enableEventMsgPublishing;
    TProcessIdMap m_exeProcIdMap;
    TStringSet m_exeProcsToWatch;
    CORE::UInt32 m_exeMatchPidMatchThreshold;
    TMetricsThresholdMap m_metricsThresholds;
    TMetricsThresholdMapMap m_procMetricsThresholds;

    bool m_gatherProcPageFaultCountInBytes;
    bool m_gatherProcPageFileUsageInBytes;
    bool m_gatherProcPeakPageFileUsageInBytes;
    bool m_gatherProcPeakWorkingSetSizeInBytes;
    bool m_gatherProcWorkingSetSizeInBytes;

    bool m_gatherGlobMemStats;
    bool m_gatherGlobalAvailablePageFileSizeInBytes;
    bool m_gatherGlobalPageFileUsageInBytes;
    bool m_gatherGlobalAvailableVirtualMemoryInBytes;
    bool m_gatherGlobalAvailExtendedVirtualMemoryInBytes;
    bool m_gatherGlobalMemoryLoadPercentage;
    bool m_gatherGlobalTotalPageFileSizeInBytes;
    bool m_gatherGlobalTotalPhysicalMemoryInBytes;
    bool m_gatherGlobalTotalVirtualMemoryInBytes;

    bool m_gatherProcCpuUptime;
    bool m_gatherProcCpuOverallPercentage;
};

/*-------------------------------------------------------------------------*/
