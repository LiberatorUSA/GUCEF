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

#include <Objbase.h>
#include <Mq.h>

#undef SetPort

#include <string>

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

#include "MsmqMetrics.h"

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

std::wstring 
RetrieveWin32APIErrorMessage( DWORD dwErr )
{
    // from: https://docs.microsoft.com/en-us/windows/win32/seccrypto/retrieving-error-messages

    WCHAR   wszMsgBuff[512];  // Buffer for text.

    DWORD   dwChars;  // Number of chars returned.

    // Try to get the message from the system errors.
    dwChars = ::FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM |
                                FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL,
                                dwErr,
                                0,
                                wszMsgBuff,
                                512,
                                NULL );

    if (0 == dwChars)
    {
        // The error code did not exist in the system errors.
        // Try Ntdsbmsg.dll for the error code.

        HINSTANCE hInstNtdsbmsg = NULL;

        // Load the library.
        hInstNtdsbmsg = ::LoadLibraryA( "Ntdsbmsg.dll" );
        if ( NULL == hInstNtdsbmsg )
        {
            // cannot load Ntdsbmsg.dll for refinement
            return std::wstring();
        }

        // Try getting message text from ntdsbmsg.
        dwChars = FormatMessageW( FORMAT_MESSAGE_FROM_HMODULE |
                                 FORMAT_MESSAGE_IGNORE_INSERTS,
                                 hInstNtdsbmsg,
                                 dwErr,
                                 0,
                                 wszMsgBuff,
                                 512,
                                 NULL );

        // Free the library.
        ::FreeLibrary( hInstNtdsbmsg );

    }

    return wszMsgBuff;
}

/*-------------------------------------------------------------------------*/

RestApiProcessMetricsInfoResource::RestApiProcessMetricsInfoResource( MsmqMetrics* app )
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
    output.SetAttribute( "application", "MsmqMetrics" );
    output.SetAttribute( "buildDateTime", compileDt.ToIso8601DateTimeString( true, true ) );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", CORE::CVariant( false ) );
    #else
    output.SetAttribute( "isReleaseBuild", CORE::CVariant( true ) );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiProcessMetricsConfigResource::RestApiProcessMetricsConfigResource( MsmqMetrics* app, bool appConfig )
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

MsmqMetrics::MsmqMetrics( void )
    : CORE::CObservingNotifier()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_enableRestApi( true )
    , m_queues()
    , m_queueNamesAreMsmqFormatNames( false )
    , m_metricsPrefix()
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

MsmqMetrics::~MsmqMetrics()
{GUCEF_TRACE;

    SetStandbyMode( true );
}

/*-------------------------------------------------------------------------*/

MsmqMetrics::MsmqQueue::MsmqQueue( const CORE::CString& qName        ,
                                   bool queueNamesAreMsmqFormatNames )
    : queueName( qName )
    , msmqQueueFormatName()
    , queueNameIsMsmqFormatName( queueNamesAreMsmqFormatNames )
    , metricFriendlyQueueName()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

MsmqMetrics::MsmqQueueProperties::MsmqQueueProperties( void )
    : queueLabel()
    , quota( 0 )
    , pathName()
    , pathNameDNS()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CString 
MsmqMetrics::MsmqQueueProperties::ToString( void ) const
{GUCEF_TRACE;

    return "queueLabel=" + queueLabel + ", quota=" + CORE::ToString( quota ) + ", pathName=" + pathName + ", pathNameDNS=" + pathNameDNS;
}

/*-------------------------------------------------------------------------*/

void
MsmqMetrics::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &MsmqMetrics::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );

}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::StringToMsmqGUID( const CORE::CAsciiString& guidString ,
                               GUID& guid                           )
{GUCEF_TRACE;

    if ( !guidString.IsNULLOrEmpty() && guidString.IsFormattingValid() )
    {
        int scanResult = sscanf( guidString.C_String(), "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
                &guid.Data1, &guid.Data2, &guid.Data3, 
                &guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3] ,
                &guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7] );

        return 11 == scanResult;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::MsmqGUIDToString( const GUID& guid               ,
                               CORE::CAsciiString& guidString )
{GUCEF_TRACE;

    guidString.Reserve( 128 );
    int printResult = sprintf( guidString.C_String(), "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
                guid.Data1, guid.Data2, guid.Data3, 
                guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3] ,
                guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7] );
    guidString.DetermineLength();
                
    return printResult > 0;
}

/*-------------------------------------------------------------------------*/

bool 
MsmqMetrics::GetMsmqQueueQuota( const std::wstring& queueFormatName ,
                                UInt64& queueQuota                  )
{GUCEF_TRACE;

    // Validate the input string.  
    if ( queueFormatName.empty() ) 
        return false;  

    // Define the maximum number of queue properties and a property counter.  
    const int NUMBEROFPROPERTIES = 1;  
    DWORD cPropId=0;  
  
    // Define a queue property structure.  
    MQQUEUEPROPS   QueueProps;  
    QUEUEPROPID    aQueuePropId[ NUMBEROFPROPERTIES ];  
    MQPROPVARIANT  aQueuePropVar[ NUMBEROFPROPERTIES ];  
    HRESULT        aQueuePropStatus[ NUMBEROFPROPERTIES ];  
    HRESULT queryResultCode = MQ_OK;  
  
    // Specify the PROPID_Q_LABEL property.  
    aQueuePropId[ cPropId ] = PROPID_Q_LABEL;       // Property ID  
    aQueuePropVar[ cPropId ].vt = VT_NULL;          // Type indicator  
    cPropId++;  
  
    // Initialize the MQQUEUEPROPS structure.  
    QueueProps.cProp = cPropId;  
    QueueProps.aPropID = aQueuePropId;  
    QueueProps.aPropVar = aQueuePropVar;  
    QueueProps.aStatus = aQueuePropStatus;  
 
    // Get the queue properties.  
    queryResultCode = ::MQGetQueueProperties( queueFormatName.c_str(), &QueueProps );  
    if ( FAILED( queryResultCode ) )  
    {  
        CORE::UInt32 errorCode =  HRESULT_CODE( queryResultCode );
        std::wstring errMsg = RetrieveWin32APIErrorMessage( errorCode );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqQueueLabel: Failed to obtain queue quota for queue format name \"" + 
            CORE::ToString( queueFormatName ) + "\". HRESULT=" + CORE::ToString( queryResultCode ) + " Code Segment= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) ) ; 
        return false; 
    } 
    
    queueQuota = (UInt64) aQueuePropVar[ 0 ].ulVal;  
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
MsmqMetrics::GetMsmqQueueLabel( const std::wstring& queueFormatName  ,
                                CORE::CString& queueLabel            )
{GUCEF_TRACE;

    // Validate the input string.  
    if ( queueFormatName.empty() ) 
        return false;  

    // Define the maximum number of queue properties and a property counter.  
    const int NUMBEROFPROPERTIES = 1;  
    DWORD cPropId=0;  
  
    // Define a queue property structure.  
    MQQUEUEPROPS   QueueProps;  
    QUEUEPROPID    aQueuePropId[ NUMBEROFPROPERTIES ];  
    MQPROPVARIANT  aQueuePropVar[ NUMBEROFPROPERTIES ];  
    HRESULT        aQueuePropStatus[ NUMBEROFPROPERTIES ];  
    HRESULT queryResultCode = MQ_OK;  
  
    // Specify the PROPID_Q_QUOTA property.  
    aQueuePropId[ cPropId ] = PROPID_Q_LABEL;         // Property ID  
    aQueuePropVar[ cPropId ].vt = VT_NULL;            // Type indicator  
    cPropId++;  
  
    // Initialize the MQQUEUEPROPS structure.  
    QueueProps.cProp = cPropId;  
    QueueProps.aPropID = aQueuePropId;  
    QueueProps.aPropVar = aQueuePropVar;  
    QueueProps.aStatus = aQueuePropStatus;  
 
    // Get the queue properties.  
    queryResultCode = ::MQGetQueueProperties( queueFormatName.c_str(), &QueueProps );  
    if ( FAILED( queryResultCode ) )  
    {  
        CORE::UInt32 errorCode =  HRESULT_CODE( queryResultCode );
        std::wstring errMsg = RetrieveWin32APIErrorMessage( errorCode );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqQueueLabel: Failed to obtain queue label for queue format name \"" + 
            CORE::ToString( queueFormatName ) + "\". HRESULT=" + CORE::ToString( queryResultCode ) + " Code Segment= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) ) ; 
        return false; 
    }  
  
    if ( NULL != aQueuePropVar[ 0 ].pwszVal )
    {
        std::wstring queueLabelWs = aQueuePropVar[ 0 ].pwszVal;
        queueLabel = CORE::ToString( queueLabelWs );
        ::MQFreeMemory( aQueuePropVar[ 0 ].pwszVal );
        aQueuePropVar[ 0 ].pwszVal = NULL;
        return true;
    }
    else
    {
        queueLabel.Clear();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
MsmqMetrics::GetMsmqQueuePathName( const std::wstring& queueFormatName  ,
                                   CORE::CString& queuePathName         )
{GUCEF_TRACE;

    // Validate the input string.  
    if ( queueFormatName.empty() ) 
        return false;  

    // Define the maximum number of queue properties and a property counter.  
    const int NUMBEROFPROPERTIES = 1;  
    DWORD cPropId=0;  
  
    // Define a queue property structure.  
    MQQUEUEPROPS   QueueProps;  
    QUEUEPROPID    aQueuePropId[ NUMBEROFPROPERTIES ];  
    MQPROPVARIANT  aQueuePropVar[ NUMBEROFPROPERTIES ];  
    HRESULT        aQueuePropStatus[ NUMBEROFPROPERTIES ];  
    HRESULT queryResultCode = MQ_OK;  
  
    // Specify the PROPID_Q_PATHNAME_DNS property.  
    aQueuePropId[ cPropId ] = PROPID_Q_PATHNAME_DNS;   // Property ID  
    aQueuePropVar[ cPropId ].vt = VT_NULL;             // Type indicator  
    cPropId++;  
  
    // Initialize the MQQUEUEPROPS structure.  
    QueueProps.cProp = cPropId;  
    QueueProps.aPropID = aQueuePropId;  
    QueueProps.aPropVar = aQueuePropVar;  
    QueueProps.aStatus = aQueuePropStatus;  
 
    // Get the queue properties.  
    queryResultCode = ::MQGetQueueProperties( queueFormatName.c_str(), &QueueProps );  
    if ( FAILED( queryResultCode ) )  
    {  
        CORE::UInt32 errorCode =  HRESULT_CODE( queryResultCode );
        std::wstring errMsg = RetrieveWin32APIErrorMessage( errorCode );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqQueuePathName: Failed to obtain queue path name for queue format name \"" + 
            CORE::ToString( queueFormatName ) + "\". HRESULT=" + CORE::ToString( queryResultCode ) + " Code Segment= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) ) ; 
        return false; 
    }  
  
    if ( NULL != aQueuePropVar[ 0 ].pwszVal )
    {
        std::wstring queuePathNameWs = aQueuePropVar[ 0 ].pwszVal;
        queuePathName = CORE::ToString( queuePathNameWs );
        ::MQFreeMemory( aQueuePropVar[ 0 ].pwszVal );
        aQueuePropVar[ 0 ].pwszVal = NULL;
        return true;
    }
    else
    {
        queuePathName.Clear();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
MsmqMetrics::GetMsmqQueuePathNameDNS( const std::wstring& queueFormatName  ,
                                      CORE::CString& queuePathNameDNS      )
{GUCEF_TRACE;

    // Validate the input string.  
    if ( queueFormatName.empty() ) 
        return false;  

    // Define the maximum number of queue properties and a property counter.  
    const int NUMBEROFPROPERTIES = 1;  
    DWORD cPropId=0;  
  
    // Define a queue property structure.  
    MQQUEUEPROPS   QueueProps;  
    QUEUEPROPID    aQueuePropId[ NUMBEROFPROPERTIES ];  
    MQPROPVARIANT  aQueuePropVar[ NUMBEROFPROPERTIES ];  
    HRESULT        aQueuePropStatus[ NUMBEROFPROPERTIES ];  
    HRESULT queryResultCode = MQ_OK;  
  
    // Specify the PROPID_Q_PATHNAME property.  
    aQueuePropId[ cPropId ] = PROPID_Q_PATHNAME_DNS;       // Property ID  
    aQueuePropVar[ cPropId ].vt = VT_NULL;             // Type indicator  
    cPropId++;   
  
    // Initialize the MQQUEUEPROPS structure.  
    QueueProps.cProp = cPropId;  
    QueueProps.aPropID = aQueuePropId;  
    QueueProps.aPropVar = aQueuePropVar;  
    QueueProps.aStatus = aQueuePropStatus;  
 
    // Get the queue properties.  
    queryResultCode = ::MQGetQueueProperties( queueFormatName.c_str(), &QueueProps );  
    if ( FAILED( queryResultCode ) )  
    {  
        CORE::UInt32 errorCode =  HRESULT_CODE( queryResultCode );
        std::wstring errMsg = RetrieveWin32APIErrorMessage( errorCode );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqQueuePathNameDNS: Failed to obtain queue path name DNS for queue format name \"" + 
            CORE::ToString( queueFormatName ) + "\". HRESULT=" + CORE::ToString( queryResultCode ) + " Code Segment= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) ) ; 
        return false; 
    }  
  
    if ( NULL != aQueuePropVar[ 0 ].pwszVal )
    {
        std::wstring queuePathNameDNSWs = aQueuePropVar[ 0 ].pwszVal;
        queuePathNameDNS = CORE::ToString( queuePathNameDNSWs );
        ::MQFreeMemory( aQueuePropVar[ 0 ].pwszVal );
        aQueuePropVar[ 0 ].pwszVal = NULL;
        return true;
    }
    else
    {
        queuePathNameDNS.Clear();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
MsmqMetrics::GetMsmqQueueProperties( const std::wstring& queueFormatName  ,
                                     MsmqQueueProperties& queueProperties )
{GUCEF_TRACE;

    // Validate the input string.  
    if ( queueFormatName.empty() ) 
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqQueueProperties: No queue format name was provided" ); 
        return false; 
    }

    // We query for each of the properties independently since we have no clue what type of queue we are 
    // actually dealing with. As such we best effort try to obtain properties, getting whatever we can get
    bool totalSuccess = true;
    totalSuccess = GetMsmqQueueLabel( queueFormatName, queueProperties.queueLabel ) && totalSuccess;
    totalSuccess = GetMsmqQueueQuota( queueFormatName, queueProperties.quota ) && totalSuccess;
    totalSuccess = GetMsmqQueuePathName( queueFormatName, queueProperties.pathName ) && totalSuccess;
    totalSuccess = GetMsmqQueuePathNameDNS( queueFormatName, queueProperties.pathNameDNS ) && totalSuccess;

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::MsmqPathNameToMsmqQueueFormatName( const std::wstring& pathName   ,
                                                std::wstring& queueFormatName  )
{GUCEF_TRACE;

    DWORD maxFormatNameSize = 512;
    WCHAR queueFormatNameTmp[ 512 ]; 
    HRESULT formatConversionResult = ::MQPathNameToFormatName( pathName.c_str()    ,    
                                                               queueFormatNameTmp  ,    
                                                               &maxFormatNameSize  );
    if ( !FAILED( formatConversionResult ) )
    {
        queueFormatName = queueFormatNameTmp;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:MsmqPathNameToMsmqQueueFormatName: Converted pathName \"" + 
            CORE::ToString( pathName ) + "\" to \"" + CORE::ToString( queueFormatName ) + "\"" ) ; 
        return true;
    }
    else
    {
        CORE::UInt32 errorCode =  HRESULT_CODE( formatConversionResult );
        std::wstring errMsg = RetrieveWin32APIErrorMessage( errorCode );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:MsmqPathNameToMsmqQueueFormatName: Failed to convert path \"" + 
            CORE::ToString( pathName ) + "\" to MSMQ format name. HRESULT=" + CORE::ToString( formatConversionResult ) + " Code Segment= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) ) ; 
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::MsmqQueueGUIDToMsmqQueueFormatName( const GUID& queueGuid          ,
                                                 std::wstring& queueFormatName  )
{GUCEF_TRACE;

    DWORD maxFormatNameSize = 512;
    WCHAR queueFormatNameTmp[ 512 ]; 
    HRESULT formatConversionResult = ::MQInstanceToFormatName( const_cast< GUID* >( &queueGuid ) ,    
                                                               queueFormatNameTmp                ,    
                                                               &maxFormatNameSize                );
    if ( !FAILED( formatConversionResult ) )
    {
        queueFormatName = queueFormatNameTmp;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:MsmqQueueGUIDToMsmqQueueFormatName: Converted GUID structure to \"" + 
            CORE::ToString( queueFormatName ) + "\"" ) ; 
        return true;
    }
    else
    {
        CORE::UInt32 errorCode =  HRESULT_CODE( formatConversionResult );
        std::wstring errMsg = RetrieveWin32APIErrorMessage( errorCode );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:MsmqQueueGUIDToMsmqQueueFormatName: Failed to convert GUID to MSMQ format name. HRESULT=" + 
            CORE::ToString( formatConversionResult ) + " Code Segment= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) ) ; 
        return false;
    }
    
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::MsmqQueueGUIDToMsmqQueueFormatName( const CORE::CString& queueGuid ,
                                                 std::wstring& queueFormatName  )
{GUCEF_TRACE;

    CORE::CAsciiString asciiGuidStr = queueGuid.ForceToAscii( 0 );
    if ( asciiGuidStr.DetermineLength() != queueGuid.Length() )
    {
        CORE::CAsciiString asciiGuidStrLogExample = queueGuid.ForceToAscii( '*' );
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:MsmqQueueGUIDToMsmqQueueFormatName: Code point count difference while converting queueGuid \"" + 
            queueGuid + "\" to ASCII representation. Replacing non-ASCII with '*' yields: " + asciiGuidStrLogExample ) ; 
    }

    GUID queueGuidStruct;
    if ( StringToMsmqGUID( asciiGuidStr, queueGuidStruct ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:MsmqQueueGUIDToMsmqQueueFormatName: Converted queueGuid string \"" + 
            queueGuid + "\" to a GUID structure successfully" ); 

        return MsmqQueueGUIDToMsmqQueueFormatName( queueGuidStruct, queueFormatName );
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:MsmqQueueGUIDToMsmqQueueFormatName: Failed to convert queueGuid string \"" + 
        queueGuid + "\" to a GUID structure" ); 
    return false;
}

/*-------------------------------------------------------------------------*/

const std::wstring&
MsmqMetrics::GetMsmqQueueFormatName( MsmqQueue& q )
{
    if ( !q.msmqQueueFormatName.empty() )
        return q.msmqQueueFormatName;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqQueueFormatName: MSMQ queue 'format name' is not yet set, attempting to obtain it. Queue=" + q.queueName ) ; 

    std::wstring wQueueName;
    CORE::Utf8toUtf16( q.queueName, wQueueName );

    // Should we try and take the string as given and not do anything fancy?
    if ( !q.queueNameIsMsmqFormatName )  
    {
        bool isSuspectedQueueGuid = ( 0 == q.queueName.HasChar( '{', true ) && 0 < q.queueName.HasChar( '}', true ) );
        if ( isSuspectedQueueGuid )
        {
            std::wstring wQueueFormatName;
            if ( MsmqQueueGUIDToMsmqQueueFormatName( q.queueName, wQueueFormatName ) )
            {
                q.msmqQueueFormatName = wQueueFormatName;
            }
        }
        else
        {
            std::wstring wQueueFormatName;
            if ( MsmqPathNameToMsmqQueueFormatName( wQueueName, wQueueFormatName ) )
            {
                q.msmqQueueFormatName = wQueueFormatName;
            }
        }
    }
    else
    {
        q.msmqQueueFormatName = wQueueName;
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqQueueFormatName: Since the topic name is flagged as a format name we will use it as-is. format name aka Queue=" + q.queueName );
    }

    return q.msmqQueueFormatName; 
}

/*-------------------------------------------------------------------------*/

CORE::Int64
MsmqMetrics::GetQueueMetric( MsmqQueue& q                           , 
                             const CORE::CString& metricDescription ,
                             UInt32 propId                          ,
                             UInt32 propType                        ) const
{GUCEF_TRACE;

    // MQMgmtGetInfo only supports a format name
    // As such depending on how the setting is presented we need to translate it
    const std::wstring& queueFormatName = GetMsmqQueueFormatName( q );
    if ( queueFormatName.empty() )
        return -1;

    // Define the required constants and variables.  
    const int NUMBEROFPROPERTIES = 1;  
    DWORD cPropId = 0;  

    // Define an MQMGMTROPS structure.  
    ::MQMGMTPROPS mgmtprops;  
    ::MGMTPROPID aMgmtPropId[ NUMBEROFPROPERTIES ];  
    ::MQPROPVARIANT aMgmtPropVar[ NUMBEROFPROPERTIES ];  

    aMgmtPropId[ cPropId ] = (PROPID) propId;              // Property identifier  
    aMgmtPropVar[ cPropId ].vt = (VARTYPE) propType;       // Type indicator  
    ++cPropId;

    // Initialize the MQMGMTPROPS structure.  
    mgmtprops.cProp = cPropId;   // number of management properties  
    mgmtprops.aPropID = aMgmtPropId;// management property IDs  
    mgmtprops.aPropVar = aMgmtPropVar;// management property values  
    mgmtprops.aStatus  = NULL;// no storage for error codes   

    // Now that we formulated the request
    // actually ask for the info
    std::wstring queueInfoFormatName = L"QUEUE=" + queueFormatName;
    HRESULT queueInfoFetchResult = ::MQMgmtGetInfo( NULL, queueInfoFormatName.c_str(), &mgmtprops );
    if ( MQ_OK == queueInfoFetchResult )
    {
        switch ( propType )
        {
            case VT_UI4: return (CORE::Int64) aMgmtPropVar[ 0 ].ulVal;
            case VT_UI8: return (CORE::Int64) aMgmtPropVar[ 0 ].uhVal.QuadPart;

            default:
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetQueueMetric: PropType is unsupported. Change the code! Type=" + CORE::ToString( propType ) );
                return 0;
            }
        }
        
    }

    CORE::UInt32 errorCode =  HRESULT_CODE( queueInfoFetchResult );
    std::wstring errMsg = RetrieveWin32APIErrorMessage( HRESULT_CODE( queueInfoFetchResult ) );
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetQueueMetric: Failed to obtain " + metricDescription + ". Queue Name: " + q.queueName +". errorCode= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) );
    return -1;
}

/*-------------------------------------------------------------------------*/

CORE::UInt64
MsmqMetrics::GetMsmqComputerMetric( const CORE::CString& metricDescription ,
                                    UInt32 propId                          ,
                                    UInt32 propType                        ) const
{GUCEF_TRACE;

    // Define the required constants and variables.  
    const int NUMBEROFPROPERTIES = 1;  
    DWORD cPropId = 0;  

    // Define an MQMGMTROPS structure.  
    ::MQMGMTPROPS mgmtprops;  
    ::MGMTPROPID aMgmtPropId[ NUMBEROFPROPERTIES ];  
    ::MQPROPVARIANT aMgmtPropVar[ NUMBEROFPROPERTIES ];  

    aMgmtPropId[ cPropId ] = (PROPID) propId;              // Property identifier  
    aMgmtPropVar[ cPropId ].vt = (VARTYPE) propType;       // Type indicator  
    ++cPropId;

    // Initialize the MQMGMTPROPS structure.  
    mgmtprops.cProp = cPropId;   // number of management properties  
    mgmtprops.aPropID = aMgmtPropId;// management property IDs  
    mgmtprops.aPropVar = aMgmtPropVar;// management property values  
    mgmtprops.aStatus  = NULL;// no storage for error codes   

    // Now that we formulated the request
    // actually ask for the info
    HRESULT computerInfoFetchResult = ::MQMgmtGetInfo( NULL, L"MACHINE", &mgmtprops );
    if ( MQ_OK == computerInfoFetchResult )
    {
        switch ( propType )
        {
            case VT_UI4: return (CORE::Int64) aMgmtPropVar[ 0 ].ulVal;
            case VT_UI8: return (CORE::Int64) aMgmtPropVar[ 0 ].uhVal.QuadPart;

            default:
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqComputerMetric: PropType is unsupported. Change the code! Type=" + CORE::ToString( propType ) );
                return 0;
            }
        }
        
    }

    CORE::UInt32 errorCode =  HRESULT_CODE( computerInfoFetchResult );
    std::wstring errMsg = RetrieveWin32APIErrorMessage( HRESULT_CODE( computerInfoFetchResult ) );
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:GetMsmqComputerMetric: Failed to obtain " + metricDescription + ". errorCode= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) );
    return -1;
}

/*-------------------------------------------------------------------------*/

CORE::Int64
MsmqMetrics::GetCurrentNrOfMessagesInQueue( MsmqQueue& q ) const
{GUCEF_TRACE;

    return GetQueueMetric( q, "count of queued messages", PROPID_MGMT_QUEUE_MESSAGE_COUNT, VT_UI4 );
}

/*-------------------------------------------------------------------------*/

CORE::Int64
MsmqMetrics::GetCurrentByteCountOfMesssagesInQueue( MsmqQueue& q ) const
{GUCEF_TRACE;

    // For MSMQ 3.0 and above:
    #if ( _WIN32_WINNT >= 0x0501 )
    return GetQueueMetric( q, "byte count of messages", PROPID_MGMT_QUEUE_BYTES_IN_QUEUE, VT_UI4 );
    #else
    return -1;
    #endif
}

/*-------------------------------------------------------------------------*/

CORE::Int64
MsmqMetrics::GetCurrentNrOfMessagesInQueueJournal( MsmqQueue& q ) const
{GUCEF_TRACE;

    // For MSMQ 3.0 and above:
    #if ( _WIN32_WINNT >= 0x0501 )
    return GetQueueMetric( q, "count of messages in journal", PROPID_MGMT_QUEUE_JOURNAL_MESSAGE_COUNT, VT_UI4 );
    #else
    return -1;
    #endif
}

/*-------------------------------------------------------------------------*/

CORE::Int64
MsmqMetrics::GetCurrentByteCountOfMesssagesInQueueJournal( MsmqQueue& q ) const
{GUCEF_TRACE;

    // For MSMQ 3.0 and above:
    #if ( _WIN32_WINNT >= 0x0501 )
    return GetQueueMetric( q, "byte count of messages in journal", PROPID_MGMT_QUEUE_BYTES_IN_JOURNAL, VT_UI4 );
    #else
    return -1;
    #endif
}

/*-------------------------------------------------------------------------*/

CORE::UInt64
MsmqMetrics::GetComputerGlobalTotalBytesOfAllMessagesOfAllQueues( void ) const
{GUCEF_TRACE;

    // For MSMQ 3.0 and above:
    #if ( _WIN32_WINNT >= 0x0501 )
    return (CORE::UInt64) GetMsmqComputerMetric( "global byte count of messages", PROPID_MGMT_MSMQ_BYTES_IN_ALL_QUEUES, VT_UI8 );
    #else
    return 0;
    #endif
}

/*-------------------------------------------------------------------------*/

CORE::CString
MsmqMetrics::GenerateMetricsFriendlyQueueName( const CORE::CString& queueName )
{GUCEF_TRACE;

    // Let's avoid non-ASCII stumbling blocks and force the down to ASCII
    CORE::CAsciiString asciiMetricsFriendlyQueueName = queueName.ForceToAscii( '_' );
    
    // Replace special chars
    static const char disallowedChars[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '=', '+', ',', '.', '<', '>', '/', '?', '`', '~', '\\', '|', '{', '}', '[', ']', ';', ':', '\'', '\"' };
    asciiMetricsFriendlyQueueName = asciiMetricsFriendlyQueueName.ReplaceChars( disallowedChars, sizeof(disallowedChars)/sizeof(char), '_' );

    // Back to the platform wide string convention format
    CORE::CString metricsFriendlyQueueName = CORE::ToString( asciiMetricsFriendlyQueueName );
    return metricsFriendlyQueueName;
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::InitQueueInfo( MsmqQueue& q )
{GUCEF_TRACE;

    q.msmqQueueFormatName = GetMsmqQueueFormatName( q );
    q.metricFriendlyQueueName = GenerateMetricsFriendlyQueueName( q.queueName );    
    if ( !q.msmqQueueFormatName.empty() )
    {
        GetMsmqQueueProperties( q.msmqQueueFormatName, q.queueProperties );
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "MsmqMetrics:InitQueueInfo: Obtained properties of MSMQ queue with name \"" + q.queueName + 
            "\" as follows: " + q.queueProperties.ToString() );

        if ( !q.queueProperties.queueLabel.IsNULLOrEmpty() )
            q.metricFriendlyQueueName = GenerateMetricsFriendlyQueueName( q.queueProperties.queueLabel );
    }

    return true; // best effort is fine
}

/*-------------------------------------------------------------------------*/

void
MsmqMetrics::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventId  ,
                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // Now publish all the metrics to the metrics system
    MsmqQueueVector::iterator i = m_queues.begin();
    while ( i != m_queues.end() )
    {
        MsmqQueue& q = (*i);
        CORE::CString metricsPrefix = m_metricsPrefix + q.metricFriendlyQueueName;

        // in order to get this metric you need elevated access to the queue
        CORE::Int64 msmqMsgsInQueue = GetCurrentNrOfMessagesInQueue( q );
        if ( msmqMsgsInQueue >= 0 )
            GUCEF_METRIC_GAUGE( metricsPrefix + ".msmqMsgsInQueue", msmqMsgsInQueue, 1.0f );

        // in order to get this metric you need elevated access to the queue
        // You also need MSMQ version 3.0 or later
        CORE::Int64 msmqMsgsTotalByteCount = GetCurrentByteCountOfMesssagesInQueue( q );
        if ( msmqMsgsTotalByteCount >= 0 )
            GUCEF_METRIC_GAUGE( metricsPrefix + ".msmqMsgsTotalByteCount", msmqMsgsTotalByteCount, 1.0f );

        // in order to get this metric you need elevated access to the queue
        // You also need MSMQ version 3.0 or later
        CORE::Int64 msmqMsgsInQueueJournal = GetCurrentNrOfMessagesInQueueJournal( q );
        if ( msmqMsgsInQueueJournal >= 0 )
            GUCEF_METRIC_GAUGE( metricsPrefix + ".msmqMsgsInQueueJournal", msmqMsgsInQueueJournal, 1.0f );

        // in order to get this metric you need elevated access to the queue
        // You also need MSMQ version 3.0 or later
        CORE::Int64 msmqMsgsInJournalTotalByteCount = GetCurrentByteCountOfMesssagesInQueueJournal( q );
        if ( msmqMsgsInJournalTotalByteCount >= 0 )
            GUCEF_METRIC_GAUGE( metricsPrefix + ".msmqMsgsInJournalTotalByteCount", msmqMsgsInJournalTotalByteCount, 1.0f );

        ++i;
    }

    // in order to get this metric you need elevated access to the queue
    // You also need MSMQ version 3.0 or later
    CORE::UInt64 msmqTotalMsgBytes = GetComputerGlobalTotalBytesOfAllMessagesOfAllQueues();
    GUCEF_METRIC_GAUGE( m_metricsPrefix + ".msmqTotalMsgBytes", msmqTotalMsgBytes, 1.0f );
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::Start( void )
{GUCEF_TRACE;

    if ( m_enableRestApi )
    {
        if ( m_httpServer.Listen() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "MsmqMetrics: Opened REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "MsmqMetrics: Failed to open REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        }
    }

    if ( SetStandbyMode( false ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "MsmqMetrics: Startup completed successfully" );
        return true;
    }

    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "MsmqMetrics: Startup failed" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::SetStandbyMode( bool newModeIsStandby )
{GUCEF_TRACE;

    m_metricsTimer.SetEnabled( !newModeIsStandby );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::IsGlobalStandbyEnabled( void ) const
{GUCEF_TRACE;

    return m_metricsTimer.GetEnabled();
}

/*-------------------------------------------------------------------------*/

bool
MsmqMetrics::LoadConfig( const CORE::CValueList& appConfig   ,
                         const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    m_enableRestApi = CORE::StringToBool( appConfig.GetValueAlways( "enableRestApi" ), m_enableRestApi );
    m_metricsPrefix = appConfig.GetValueAlways( "metricsPrefix", m_metricsPrefix );
    m_queueNamesAreMsmqFormatNames = CORE::StringToBool( appConfig.GetValueAlways( "queueNamesAreMsmqFormatNames" ), m_queueNamesAreMsmqFormatNames );

    CORE::CString::StringVector queuesToWatch = appConfig.GetValueAlways( "queuesToWatch" ).AsString().ParseElements( ';', false );
    CORE::CString::StringVector::iterator i = queuesToWatch.begin();
    while ( i != queuesToWatch.end() )
    {
        m_queues.push_back( MsmqQueue( (*i), m_queueNamesAreMsmqFormatNames ) );
        ++i;                                                          
    }

    // Perform an initial init of the queue information
    MsmqQueueVector::iterator n = m_queues.begin();
    while ( n != m_queues.end() )
    {
        MsmqQueue& q = (*n);
        InitQueueInfo( q );
        ++n;
    }

    if ( m_enableRestApi )
    {
        m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "restApiPort" ) ), 10000 ) );

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
MsmqMetrics::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode&
MsmqMetrics::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/
