/*
 *  MsmqMetrics: Service for obtaining MSMQ stats
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

#include <queue>

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
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

class MsmqMetrics;

class RestApiProcessMetricsInfoResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiProcessMetricsInfoResource( MsmqMetrics* app );

    virtual ~RestApiProcessMetricsInfoResource();

    virtual bool Serialize( const CORE::CString& resourcePath   ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    MsmqMetrics* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiProcessMetricsConfigResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiProcessMetricsConfigResource( MsmqMetrics* app, bool appConfig );

    virtual ~RestApiProcessMetricsConfigResource();

    virtual bool Serialize( const CORE::CString& resourcePath   , 
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& params         ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    MsmqMetrics* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class MsmqMetrics : public CORE::CObservingNotifier
{
    public:

    MsmqMetrics( void );
    virtual ~MsmqMetrics();

    bool Start( void );

    bool SetStandbyMode( bool newMode );

    bool IsGlobalStandbyEnabled( void ) const;

    bool LoadConfig( const CORE::CValueList& appConfig   ,
                     const CORE::CDataNode& globalConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    private:

    void RegisterEventHandlers( void );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );
    
    class MsmqQueue
    {
        public:
        
        CORE::CString queueName;
        std::wstring msmqQueueFormatName;
        bool queueNameIsMsmqFormatName;
        CORE::CString metricFriendlyQueueName;

        MsmqQueue( const CORE::CString& qName                   , 
                   const CORE::CString& metricFriendlyQueueName ,
                   bool queueNamesAreMsmqFormatNames            );
    };
    typedef std::vector< MsmqQueue >    MsmqQueueVector;
    
    CORE::Int64 GetCurrentNrOfMessagesInQueue( MsmqQueue& q ) const;

    const std::wstring& GetMsmqQueueFormatName( MsmqQueue& q ) const;

    static bool MsmqQueueGUIDToMsmqQueueFormatName( const CORE::CString& queueGuid ,
                                                    std::wstring& queueFormatName  );
    
    static bool MsmqQueueGUIDToMsmqQueueFormatName( const GUID& queueGuid          ,
                                                    std::wstring& queueFormatName  );

    static bool MsmqPathNameToMsmqQueueFormatName( const std::wstring& pathName   ,
                                                    std::wstring& queueFormatName  );
    
    static bool StringToMsmqGUID( const CORE::CAsciiString& guidString, GUID& guid );
    
    static bool MsmqGUIDToString( const GUID& guid, CORE::CAsciiString& guidString );

    static CORE::CString GenerateMetricsFriendlyQueueName( const CORE::CString& queueName );
    
    private:

    typedef CORE::CTEventHandlerFunctor< MsmqMetrics > TEventCallback;

    WEB::CHTTPServer m_httpServer;
    WEB::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;
    CORE::CTimer m_metricsTimer;
    bool m_enableRestApi;
    MsmqQueueVector m_queues;
    bool m_queueNamesAreMsmqFormatNames;
    CORE::CString m_metricsPrefix;
};

/*-------------------------------------------------------------------------*/
