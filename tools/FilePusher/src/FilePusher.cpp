/*
 *  FilePusher: service which monitors the file system and pushes files to
 *  a remote repository
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

#include "FilePusher.h"

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::UInt32 FilePushDestinationSettings::DefaultNewFileRestPeriodInSecs = 60 * 10; // Rest period of 10 minutes

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

RestApiFilePusherInfoResource::RestApiFilePusherInfoResource( FilePusher* app )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiFilePusherInfoResource::~RestApiFilePusherInfoResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiFilePusherInfoResource::Serialize( CORE::CDataNode& output             ,
                                         const CORE::CString& representation )
{GUCEF_TRACE;

    output.SetName( "info" );
    output.SetAttribute( "application", "FilePusher" );
    output.SetAttribute( "buildDateTime", __TIMESTAMP__ );
    #ifdef GUCEF_DEBUG_MODE
    output.SetAttribute( "isReleaseBuild", "false" );
    #else
    output.SetAttribute( "isReleaseBuild", "true" );
    #endif
    return true;
}

/*-------------------------------------------------------------------------*/

RestApiFilePusherConfigResource::RestApiFilePusherConfigResource( FilePusher* app, bool appConfig )
    : COM::CCodecBasedHTTPServerResource()
    , m_app( app )
    , m_appConfig( appConfig )
{GUCEF_TRACE;

    m_allowSerialize = true;
}

/*-------------------------------------------------------------------------*/

RestApiFilePusherConfigResource::~RestApiFilePusherConfigResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
RestApiFilePusherConfigResource::Serialize( CORE::CDataNode& output             ,
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

FilePushDestinationSettings::FilePushDestinationSettings( void )
    : filePushDestinationUri()
    , fileMatchPatterns()
    , dirsToWatch()
    , restingTimeForNewFilesInSecs()
    , deleteFilesAfterSuccessfullPush( false )
    , transmitMetrics( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

FilePushDestinationSettings::FilePushDestinationSettings( const FilePushDestinationSettings& src )
    : filePushDestinationUri( src.filePushDestinationUri )
    , fileMatchPatterns( src.fileMatchPatterns )
    , dirsToWatch( src.dirsToWatch )
    , restingTimeForNewFilesInSecs( src.restingTimeForNewFilesInSecs )
    , deleteFilesAfterSuccessfullPush( src.deleteFilesAfterSuccessfullPush )
    , transmitMetrics( src.transmitMetrics )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

FilePushDestinationSettings::~FilePushDestinationSettings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

FilePushDestinationSettings&
FilePushDestinationSettings::operator=( const FilePushDestinationSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        filePushDestinationUri = src.filePushDestinationUri;
        fileMatchPatterns = src.fileMatchPatterns;
        dirsToWatch = src.dirsToWatch;
        restingTimeForNewFilesInSecs = src.restingTimeForNewFilesInSecs;
        deleteFilesAfterSuccessfullPush = src.deleteFilesAfterSuccessfullPush;
        transmitMetrics = src.transmitMetrics;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
FilePushDestinationSettings::LoadConfig( const CORE::CValueList& appConfig )
{GUCEF_TRACE;

    transmitMetrics = CORE::StringToBool( appConfig.GetValueAlways( "TransmitMetrics", "true" ) );

    restingTimeForNewFilesInSecs = CORE::StringToUInt32( CORE::ResolveVars( appConfig.GetValueAlways( "RestingTimeForNewFilesInSecs", CORE::UInt32ToString( DefaultNewFileRestPeriodInSecs ) ) ) );
    deleteFilesAfterSuccessfullPush = CORE::StringToBool( CORE::ResolveVars( appConfig.GetValueAlways( "DeleteFilesAfterSuccessfullPush", "true" ) ) );
    
    filePushDestinationUri = CORE::ResolveVars( appConfig.GetValueAlways( "FilePushDestinationUri" ) );
    if ( filePushDestinationUri.IsNULLOrEmpty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: You must specify setting \"FilePushDestinationUri\". It currently has no value" );
        return false;    
    }
    CORE::CString protocolSanityCheckUri = filePushDestinationUri.Lowercase();
    if ( 0 != protocolSanityCheckUri.HasSubstr( "http://", true ) && 0 != protocolSanityCheckUri.HasSubstr( "vfs://", true ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: The specified push Uri \"" + filePushDestinationUri + "\" does not specify a valid supported protocol" );
        return false; 
    }

    CORE::CValueList::TStringVector settingValues = appConfig.GetValueVectorAlways( "DirToWatch" );
    CORE::CValueList::TStringVector::iterator n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        dirsToWatch.insert( settingValue );
        ++n;
    }
    if ( dirsToWatch.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: You must specify at least one instance of setting \"DirToWatch\". Currently there are none with a value" );
        return false;
    }

    settingValues = appConfig.GetValueVectorAlways( "FilePatternForNewFilesWithRestPeriod" );
    n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        fileMatchPatterns[ settingValue ] = PUSHSTYLE_MATCHING_NEW_FILES_WITH_REST_PERIOD;
        ++n;
    }
    settingValues = appConfig.GetValueVectorAlways( "FilePatternForAllFilesWithRestPeriod" );
    n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        fileMatchPatterns[ settingValue ] = PUSHSTYLE_MATCHING_ALL_FILES_WITH_REST_PERIOD;
        ++n;
    }
    settingValues = appConfig.GetValueVectorAlways( "FilePatternForRolledOverFiles" );
    n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        fileMatchPatterns[ settingValue ] = PUSHSTYLE_ROLLED_OVER_FILES;
        ++n;
    }
    if ( fileMatchPatterns.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: You must specify at least one instance of setting \"FilePatternForNewFilesWithRestPeriod\" or \"FilePatternForRolledOverFiles\". Currently there are none with a value" );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
FilePushDestinationSettings::LoadConfig( const CORE::CDataNode& rootNode )
{GUCEF_TRACE;

    transmitMetrics = CORE::StringToBool( rootNode.GetAttributeValueOrChildValueByName( "TransmitMetrics", "true" ) );

    restingTimeForNewFilesInSecs = CORE::StringToUInt32( CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "RestingTimeForNewFilesInSecs", CORE::UInt32ToString( DefaultNewFileRestPeriodInSecs ) ) ) );
    deleteFilesAfterSuccessfullPush = CORE::StringToBool( CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "DeleteFilesAfterSuccessfullPush", "true" ) ) );
    
    filePushDestinationUri = CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "FilePushDestinationUri" ) );
    if ( filePushDestinationUri.IsNULLOrEmpty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: You must specify setting \"FilePushDestinationUri\". It currently has no value" );
        return false;    
    }
    CORE::CString protocolSanityCheckUri = filePushDestinationUri.Lowercase();
    if ( 0 != protocolSanityCheckUri.HasSubstr( "http://", true ) && 0 != protocolSanityCheckUri.HasSubstr( "vfs://", true ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: The specified push Uri \"" + filePushDestinationUri + "\" does not specify a valid supported protocol" );
        return false; 
    }

    CORE::CValueList::TStringVector settingValues = rootNode.GetAttributeValueOrChildValuesByName( "DirToWatch" );
    CORE::CValueList::TStringVector::iterator n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        dirsToWatch.insert( settingValue );
        ++n;
    }
    if ( dirsToWatch.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: You must specify at least one instance of setting \"DirToWatch\". Currently there are none with a value" );
        return false;
    }

    settingValues = rootNode.GetAttributeValueOrChildValuesByName( "FilePatternForNewFilesWithRestPeriod" );
    n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        fileMatchPatterns[ settingValue ] = PUSHSTYLE_MATCHING_NEW_FILES_WITH_REST_PERIOD;
        ++n;
    }
    settingValues = rootNode.GetAttributeValueOrChildValuesByName( "FilePatternForAllFilesWithRestPeriod" );
    n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        fileMatchPatterns[ settingValue ] = PUSHSTYLE_MATCHING_ALL_FILES_WITH_REST_PERIOD;
        ++n;
    }
    settingValues = rootNode.GetAttributeValueOrChildValuesByName( "FilePatternForRolledOverFiles" );
    n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        fileMatchPatterns[ settingValue ] = PUSHSTYLE_ROLLED_OVER_FILES;
        ++n;
    }
    if ( fileMatchPatterns.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: You must specify at least one instance of setting \"FilePatternForNewFilesWithRestPeriod\" or \"FilePatternForRolledOverFiles\". Currently there are none with a value" );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

FilePushDestination::FilePushDestination( void )
    : CORE::CObservingNotifier()
    , m_httpClient()
    , m_dirWatcher()
    , m_allFilesDirScanTimer()
    , m_newFileRestPeriodTimer()
    , m_newFileRestQueue()
    , m_pushQueue()
    , m_pushTimer() 
    , m_currentFileBeingPushed()
    , m_currentFilePushBuffer()
    , m_lastPushDurationInSecs()
    , m_metricsTimer()
    , m_settings()
{GUCEF_TRACE;

    RegisterEventHandlers();    
}

/*-------------------------------------------------------------------------*/

FilePushDestination::FilePushDestination( const FilePushDestination& src )
    : CORE::CObservingNotifier( src )
    , m_httpClient()
    , m_dirWatcher()
    , m_allFilesDirScanTimer( src.m_allFilesDirScanTimer )
    , m_newFileRestPeriodTimer( src.m_newFileRestPeriodTimer )
    , m_newFileRestQueue( src.m_newFileRestQueue )
    , m_pushQueue( src.m_pushQueue )
    , m_pushTimer( src.m_pushTimer ) 
    , m_currentFileBeingPushed( src.m_currentFileBeingPushed )
    , m_currentFilePushBuffer( src.m_currentFilePushBuffer )
    , m_lastPushDurationInSecs( src.m_lastPushDurationInSecs )
    , m_metricsTimer( src.m_metricsTimer )
    , m_settings( src.m_settings )
{GUCEF_TRACE;

    RegisterEventHandlers();    
}

/*-------------------------------------------------------------------------*/

FilePushDestination&
FilePushDestination::operator=( const FilePushDestination& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_dirWatcher = src.m_dirWatcher;
        m_allFilesDirScanTimer = src.m_allFilesDirScanTimer;
        m_newFileRestPeriodTimer = src.m_newFileRestPeriodTimer;
        m_newFileRestQueue = src.m_newFileRestQueue;
        m_pushQueue = src.m_pushQueue;
        m_pushTimer = src.m_pushTimer;
        m_currentFileBeingPushed = src.m_currentFileBeingPushed;
        m_currentFilePushBuffer = src.m_currentFilePushBuffer;
        m_lastPushDurationInSecs = src.m_lastPushDurationInSecs;
        m_metricsTimer = src.m_metricsTimer;
        m_settings = src.m_settings;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

FilePushDestination::~FilePushDestination()
{GUCEF_TRACE;

    m_httpClient.Close();
    m_dirWatcher.RemoveAllWatches();
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &FilePushDestination::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );
    
    TEventCallback callback2( this, &FilePushDestination::OnWatchedLocalDirFileCreation );
    SubscribeTo( &m_dirWatcher                             ,
                 CORE::CDirectoryWatcher::FileCreatedEvent ,
                 callback2                                 );

    TEventCallback callback3( this, &FilePushDestination::OnNewFileRestPeriodTimerCycle );
    SubscribeTo( &m_newFileRestPeriodTimer      ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback3                      );

    TEventCallback callback4( this, &FilePushDestination::OnFilePushTimerCycle );
    SubscribeTo( &m_pushTimer                   ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback4                      );

    
    TEventCallback callback5( this, &FilePushDestination::OnHttpClientConnected );
    SubscribeTo( &m_httpClient                    ,
                 COM::CHTTPClient::ConnectedEvent ,
                 callback5                        );
    TEventCallback callback6( this, &FilePushDestination::OnHttpClientDisconnected );
    SubscribeTo( &m_httpClient                       ,
                 COM::CHTTPClient::DisconnectedEvent ,
                 callback6                           );
    TEventCallback callback7( this, &FilePushDestination::OnHttpClientConnectionError );
    SubscribeTo( &m_httpClient                          ,
                 COM::CHTTPClient::ConnectionErrorEvent ,
                 callback7                              );
    TEventCallback callback8( this, &FilePushDestination::OnHttpClientHttpError );
    SubscribeTo( &m_httpClient                    ,
                 COM::CHTTPClient::HTTPErrorEvent ,
                 callback8                        );
    TEventCallback callback9( this, &FilePushDestination::OnHttpClientHttpRedirect );
    SubscribeTo( &m_httpClient                       ,
                 COM::CHTTPClient::HTTPRedirectEvent ,
                 callback9                           );
    TEventCallback callback10( this, &FilePushDestination::OnHttpClientHttpContent );
    SubscribeTo( &m_httpClient                      ,
                 COM::CHTTPClient::HTTPContentEvent ,
                 callback10                         );
    TEventCallback callback11( this, &FilePushDestination::OnHttpClientHttpDataRecieved );
    SubscribeTo( &m_httpClient                           ,
                 COM::CHTTPClient::HTTPDataRecievedEvent ,
                 callback11                              );
    TEventCallback callback12( this, &FilePushDestination::OnHttpClientHttpDataSent );
    SubscribeTo( &m_httpClient                       ,
                 COM::CHTTPClient::HTTPDataSentEvent ,
                 callback12                          );
    TEventCallback callback13( this, &FilePushDestination::OnHttpClientHttpTransferFinished );
    SubscribeTo( &m_httpClient                               ,
                 COM::CHTTPClient::HTTPTransferFinishedEvent ,
                 callback13                                  );

    TEventCallback callback14( this, &FilePushDestination::OnAsyncVfsOperationCompleted );
    SubscribeTo( &VFS::CVfsGlobal::Instance()->GetVfs()     ,
                 VFS::CVFS::AsyncVfsOperationCompletedEvent ,
                 callback14                                 );

    TEventCallback callback15( this, &FilePushDestination::OnAllFilesDirScanTimerCycle );
    SubscribeTo( &m_allFilesDirScanTimer        ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback15                     );
}

/*-------------------------------------------------------------------------*/

bool 
FilePushDestination::LoadConfig( const FilePushDestinationSettings& settings )
{GUCEF_TRACE;

    m_settings = settings;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestination::Start( void )
{GUCEF_TRACE;

    if ( m_settings.transmitMetrics )
    {
        m_metricsTimer.SetInterval( 1000 );
        m_metricsTimer.SetEnabled( true );
    }
    
    m_newFileRestPeriodTimer.SetInterval( 1000 );
    m_newFileRestPeriodTimer.SetEnabled( true );

    m_pushTimer.SetInterval( 1000 );
    m_pushTimer.SetEnabled( true );

    CORE::CString::StringVector rolloverPatterns = GetFilePatternsForPushType( TPushStyle::PUSHSTYLE_ROLLED_OVER_FILES );
    CORE::CString::StringVector allFilesPatterns = GetFilePatternsForPushType( TPushStyle::PUSHSTYLE_MATCHING_ALL_FILES_WITH_REST_PERIOD );

    CORE::CDirectoryWatcher::CDirWatchOptions watchOptions;
    watchOptions.watchForFileCreation = true;
    watchOptions.watchForFileDeletion = false;
    watchOptions.watchForFileModifications = false;
    watchOptions.watchForFileRenames = false;
    TStringSet::iterator i = m_settings.dirsToWatch.begin();
    while ( i != m_settings.dirsToWatch.end() )
    {
        if ( !allFilesPatterns.empty() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Checking for pre-existing files to push matching all-files patterns in dir \"" + (*i) + "\"" );
            QueueAllPreExistingFilesForDir( (*i) );
        }

        if ( !rolloverPatterns.empty() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Checking for pre-existing files to push matching rolled-over-files patterns in dir \"" + (*i) + "\"" );
            TriggerRolledOverFileCheck( (*i), rolloverPatterns );
        }

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Adding watch to directory \"" + (*i) + "\"" );
        if ( !m_dirWatcher.AddDirToWatch( (*i), watchOptions ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestination: Failed to add watch for directory " + (*i) );
            return false;
        }
        ++i;
    }

    if ( !allFilesPatterns.empty() )
    {
        m_allFilesDirScanTimer.SetEnabled( true );
        m_allFilesDirScanTimer.SetInterval( 10000 );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                          const CORE::CEvent& eventId  ,
                                          CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_settings.transmitMetrics )
    {
        if ( 0 == m_settings.filePushDestinationUri.HasSubstr( "http://", true ) )
        {
            GUCEF_METRIC_COUNT( "FilePusher.HttpClientBytesReceived", m_httpClient.GetBytesReceived( true ), 1.0f );
            GUCEF_METRIC_COUNT( "FilePusher.HttpClientBytesTransmitted", m_httpClient.GetBytesTransmitted( true ), 1.0f );
        }
        GUCEF_METRIC_GAUGE( "FilePusher.FilesQueuedToPush", (CORE::UInt32) m_pushQueue.size(), 1.0f );
        GUCEF_METRIC_GAUGE( "FilePusher.NewFileRestQueueSize", (CORE::UInt32) m_newFileRestQueue.size(), 1.0f );
        GUCEF_METRIC_GAUGE( "FilePusher.LastPushDurationInSecs", m_lastPushDurationInSecs, 1.0f );
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientConnected( CORE::CNotifier* notifier    ,
                                            const CORE::CEvent& eventId  ,
                                            CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: HTTP Client has connected to the server" );
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientDisconnected( CORE::CNotifier* notifier    ,
                                               const CORE::CEvent& eventId  ,
                                               CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: HTTP Client has disconnected from the server" );
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientConnectionError( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: HTTP Client has experienced a connection error" );
    m_currentFileBeingPushed.Clear();
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientHttpError( CORE::CNotifier* notifier    ,
                                            const CORE::CEvent& eventId  ,
                                            CORE::CICloneable* eventData )
{GUCEF_TRACE;

    COM::CHTTPClient::THTTPErrorEventData* httpErrorEventData = static_cast< COM::CHTTPClient::THTTPErrorEventData* >( eventData );
    CORE::UInt32 httpErrorCode = (CORE::UInt32) httpErrorEventData->GetData();

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: HTTP Client has experienced a HTTP error: " + CORE::UInt32ToString( httpErrorCode ) );
    m_currentFileBeingPushed.Clear();
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientHttpRedirect( CORE::CNotifier* notifier    ,
                                               const CORE::CEvent& eventId  ,
                                               CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: HTTP Client connection is being redirected" );
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientHttpContent( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientHttpDataSent( CORE::CNotifier* notifier    ,
                                               const CORE::CEvent& eventId  ,
                                               CORE::CICloneable* eventData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientHttpDataRecieved( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventId  ,
                                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientHttpTransferFinished( CORE::CNotifier* notifier    ,
                                                       const CORE::CEvent& eventId  ,
                                                       CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: HTTP Transfer finished" );
    OnFilePushFinished();
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnAsyncVfsOperationCompleted( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventId  ,
                                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    VFS::CAsyncVfsTaskResultData* asyncOpResult = static_cast< VFS::CAsyncVfsTaskResultData* >( eventData );
    if ( GUCEF_NULL != asyncOpResult )
    {
        m_lastPushDurationInSecs = asyncOpResult->durationInSecs;
        if (  asyncOpResult->successState )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: VFS Async operation finished successfully in " + 
                    CORE::UInt32ToString( m_lastPushDurationInSecs ) + " secs" );
            OnFilePushFinished();
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: VFS Async operation failed in " + 
                        CORE::UInt32ToString( m_lastPushDurationInSecs ) + " secs" );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: VFS Async operation failed" );
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnFilePushFinished( void )
{GUCEF_TRACE;

    TStringUInt64Map::iterator i = m_pushQueue.find( m_currentFileBeingPushed );
    if ( i != m_pushQueue.end() )
    {
        m_pushQueue.erase( i );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Successfully pushed file \"" + m_currentFileBeingPushed + "\"" ); 
        if ( m_settings.deleteFilesAfterSuccessfullPush )
        {
            if ( CORE::DeleteFile( m_currentFileBeingPushed ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Successfully deleted pushed file \"" + m_currentFileBeingPushed + "\"" );
            }
        }
        m_currentFileBeingPushed.Clear();
        m_currentFilePushBuffer.Clear();
    }
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestination::PushFileUsingHttp( const CORE::CString& pathToFileToPush, CORE::UInt32 offsetInFile )
{GUCEF_TRACE;

    // Load the file content from disk
    if ( !m_currentFilePushBuffer.LoadContentFromFile( pathToFileToPush, offsetInFile ) )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:PushFileUsingHttp: Unable to load bytes from file \"" + pathToFileToPush + "\". Is it still in use? Skipping the file for now" );
        return false;
    }
    m_currentFileBeingPushed = pathToFileToPush;

    CORE::CString fileExt = CORE::ExtractFileExtention( pathToFileToPush ).Lowercase();
    CORE::CString contentType = "application/octet-stream";
    if ( fileExt == "txt" || fileExt == "log" )
    {
        contentType = "text/plain";
    }

    // Begin the push
    CORE::CString filename = CORE::ExtractFilename( pathToFileToPush );
    CORE::CString pushUrlForFile = m_settings.filePushDestinationUri.ReplaceSubstr( "{filename}", filename );
    if ( m_httpClient.Post( pushUrlForFile, contentType, m_currentFilePushBuffer ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Commenced HTTP POST for content from file \"" + pathToFileToPush + "\"" );
        return true;    
    }

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Failed to HTTP POST bytes from file \"" + pathToFileToPush + "\". Skipping the file for now" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestination::PushFileUsingVfs( const CORE::CString& pathToFileToPush, CORE::UInt32 offsetInFile )
{GUCEF_TRACE;

    // Load the file content from disk
    if ( !m_currentFilePushBuffer.LoadContentFromFile( pathToFileToPush, offsetInFile ) )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:PushFileUsingVfs: Unable to load bytes from file \"" + pathToFileToPush + "\". Is it still in use? Skipping the file for now" );
        return false;
    }
    m_currentFileBeingPushed = pathToFileToPush;

    // Begin the push
    CORE::CString filename = CORE::ExtractFilename( pathToFileToPush );
    CORE::CString pushUrlForFile = m_settings.filePushDestinationUri.ReplaceSubstr( "{filename}", filename );
    pushUrlForFile = pushUrlForFile.CutChars( 6, true, 0 ); // Cut vfs://

    // Store the file as a singular sync blocking call
    if ( VFS::CVfsGlobal::Instance()->GetVfs().StoreAsFileAsync( pushUrlForFile, m_currentFilePushBuffer, 0, true ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Commenced async push of content from file \"" + pathToFileToPush + "\" to VFS path \"" + pushUrlForFile + "\"" );
        return true;    
    }

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Failed to pushed content from file \"" + pathToFileToPush + "\" to VFS path \"" + pushUrlForFile + "\". Skipping the file for now" );
    m_currentFilePushBuffer.Clear();
    m_currentFileBeingPushed.Clear();
    return false;
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnFilePushTimerCycle( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventId  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // We only do a max of 1 file per cycle since this is a single-threaded app on purpose
    // As such we need to allow for time for other actions

    if ( !m_currentFileBeingPushed.IsNULLOrEmpty() )
        return;

    TStringUInt64Map::iterator i = m_pushQueue.begin();
    while ( i != m_pushQueue.end() )
    {
        const CORE::CString& filePath = (*i).first;
        if ( CORE::FileExists( filePath ) )
        {
            CORE::UInt32 offsetInFile = (CORE::UInt32) (*i).second;

            if ( 0 == m_settings.filePushDestinationUri.HasSubstr( "http://", true ) )
            {
                if ( PushFileUsingHttp( filePath, offsetInFile ) )
                    return;
            }
            if ( 0 == m_settings.filePushDestinationUri.HasSubstr( "vfs://", true ) )
            {
                if ( PushFileUsingVfs( filePath, offsetInFile ) )
                    return;
            }
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: File no longer exists when we were about to commence pushing it: \"" + filePath + "\"" );
            m_pushQueue.erase( i );
            return;
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::QueueFileForPushing( const CORE::CString& filePath )
{GUCEF_TRACE;

    // We might get triggered multiple times for the same file.
    // Ignore redundant queueing requests
    if ( m_pushQueue.find( filePath ) != m_pushQueue.end() )
        return;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Queueing file \"" + filePath + "\" for pushing" );
    m_pushQueue[ filePath ] = 0;
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnAllFilesDirScanTimerCycle( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TStringSet::const_iterator i = m_settings.dirsToWatch.begin();
    while ( i != m_settings.dirsToWatch.end() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Commencing periodic scan for 'all files' style file patterns for dir: \"" + (*i) + "\"" );
        if ( QueueAllPreExistingFilesForDir( (*i) ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Finished scan for 'all files' style file patterns for dir: \"" + (*i) + "\"" );
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Failed scan for 'all files' style file patterns for dir: \"" + (*i) + "\"" );
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnNewFileRestPeriodTimerCycle( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventId  ,
                                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TStringSet restedFiles;
    CORE::CDateTime nowTime = CORE::CDateTime::NowUTCDateTime();
    TStringTimeMap::iterator i = m_newFileRestQueue.begin();
    while ( i != m_newFileRestQueue.end() )
    {
        const CORE::CString& newFilePath = (*i).first;
        if ( CORE::FileExists( newFilePath ) )
        {
            CORE::CDateTime& lastModified = (*i).second;

            CORE::CDateTime lastChange = GetLatestTimestampForFile( newFilePath );
            if ( lastChange.GetTimeDifferenceInMillisecondsTowards( nowTime ) > m_settings.restingTimeForNewFilesInSecs * 1000 )
            {
                // This file has not been modified for at least the required resting period.
                // As such tis file can now be considered a candidate for pushing.
                QueueFileForPushing( newFilePath );
                restedFiles.insert( newFilePath );
            }
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: File no longer exists when checking rest period on file: \"" + newFilePath + "\"" );
            restedFiles.insert( newFilePath );    
        }
        ++i;
    }

    // Clean out files that have rested long enough
    // We no longer need to monitor them as they moved on to the next stage
    TStringSet::iterator n = restedFiles.begin();
    while ( n != restedFiles.end() )
    {
        m_newFileRestQueue.erase( (*n) );
        ++n;
    }
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime
FilePushDestination::GetLatestTimestampForFile( const CORE::CString& filePath )
{GUCEF_TRACE;

    CORE::CDateTime lastModified = CORE::GetFileModificationTime( filePath );
    CORE::CDateTime creationTime = CORE::GetFileCreationTime( filePath );
    CORE::CDateTime lastChange = lastModified > creationTime ? lastModified : creationTime;
    return lastChange;
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::QueueNewFileForPushingAfterUnmodifiedRestPeriod( const CORE::CString& newFilePath )
{GUCEF_TRACE;

    // We don't want to add new files to the rest queue by whatever means if the same file has already 
    // progressed to the push queue so we check that first
    TStringUInt64Map::iterator i = m_pushQueue.find( newFilePath );
    if ( i == m_pushQueue.end() )
    {
        // Add the file to the list of files to be checked periodically to see if there is no more changes
        // being made to the file aka a resting period.
        CORE::CDateTime latestChange = GetLatestTimestampForFile( newFilePath );
        m_newFileRestQueue[ newFilePath ] = latestChange;
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: File with latest timestamp \"" + latestChange.ToIso8601DateTimeString( true, true ) + "\" is queued for pushing: \"" + newFilePath + "\"" );
    }
    else
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Not queueing file for rest period as its already rested and queued for pushing: \"" + newFilePath + "\"" );
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::TriggerRolledOverFileCheck( const CORE::CString& dirWithFiles   ,
                                                 const CORE::CString& patternToMatch )
{GUCEF_TRACE;

    CORE::CString::StringVector singleEntryList;
    singleEntryList.push_back( patternToMatch );
    TriggerRolledOverFileCheck( dirWithFiles, singleEntryList );
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::TriggerRolledOverFileCheck( const CORE::CString& dirWithFiles                  ,
                                                 const CORE::CString::StringVector& patternsToMatch )
{GUCEF_TRACE;

    // Check to see if we have multiple, thus rolled over, files in the target dir matching the pattern
    TDateTimeStringVectorMap matchedFiles;
    struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( dirWithFiles.C_String() );
    if ( GUCEF_NULL != did )
    {
        do
        {
            if ( CORE::DI_Is_It_A_File( did ) == 1 )
            {
                CORE::CString filename = DI_Name( did );
                CORE::CString::StringVector::const_iterator n = patternsToMatch.begin();
                while ( n != patternsToMatch.end() )
                {
                    if ( filename.WildcardEquals( (*n), '*', false ) )
                    {
                        CORE::CString fullPath = CORE::CombinePath( dirWithFiles, filename );
                        CORE::CDateTime fileTimestamp = GetLatestTimestampForFile( fullPath );
                        matchedFiles[ fileTimestamp ].push_back( fullPath );    
                    }
                    ++n;
                }
            }
        }
        while ( CORE::DI_Next_Dir_Entry( did ) != 0 );

        CORE::DI_Cleanup( did );

        if ( !matchedFiles.empty() )
        {
            // The map will list the matches in ascending order
            // With rolled over files you want to grab all files except for the 'newest' file 
            // which is the currently active file
            //
            // Note that if the newest filestamp has multiple instances we will leave all of those because we cannot
            // determine based on the timestamps which file to actually use
            size_t index = 0; 
            size_t max = matchedFiles.size() - 1;
            TDateTimeStringVectorMap::iterator i = matchedFiles.begin();
            while ( index < max )
            {
                CORE::CString::StringVector::iterator n =  (*i).second.begin();
                while ( n != (*i).second.end() )
                {
                    QueueFileForPushing( (*n) );
                    ++n;
                }
                ++index; ++i;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnWatchedLocalDirFileCreation( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventId  ,
                                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // Which file is new?
    CORE::CDirectoryWatcher::TFileCreatedEventData* fileCreatedEventData = static_cast< CORE::CDirectoryWatcher::TFileCreatedEventData* >( eventData );
    const CORE::CString& newFilePath = fileCreatedEventData->GetData();

    // Filter potentially unrelated files
    TPushStyle pushStyle = TPushStyle::PUSHSTYLE_UNKNOWN;
    CORE::CString patternMatched;
    CORE::CString newFilename = CORE::ExtractFilename( newFilePath );
    TStringPushStyleMap::iterator i = m_settings.fileMatchPatterns.begin();
    while ( i != m_settings.fileMatchPatterns.end() )
    {
        if ( newFilename.WildcardEquals( (*i).first, '*', false ) )
        {
            pushStyle = (*i).second;
            patternMatched = (*i).first;
            break;
        }
        ++i;
    }
    if ( patternMatched.IsNULLOrEmpty() )
        return;

    // Handle based on the configured push style for the given pattern
    switch ( pushStyle )
    {
        case TPushStyle::PUSHSTYLE_ROLLED_OVER_FILES:
        {
            CORE::CString dirWithFiles = CORE::StripFilename( newFilePath );
            TriggerRolledOverFileCheck( dirWithFiles, patternMatched );
            return;
        }
        case TPushStyle::PUSHSTYLE_MATCHING_ALL_FILES_WITH_REST_PERIOD:
        case TPushStyle::PUSHSTYLE_MATCHING_NEW_FILES_WITH_REST_PERIOD:
        {
            QueueNewFileForPushingAfterUnmodifiedRestPeriod( newFilePath );
            return;
        }
        case TPushStyle::PUSHSTYLE_UNKNOWN:
        default:
        {
            return;
        }
    };
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestination::DoesFilenameMatchPushAllFilesPattern( const CORE::CString& filename ) const
{GUCEF_TRACE;

    TStringPushStyleMap::const_iterator i = m_settings.fileMatchPatterns.begin();
    while ( i != m_settings.fileMatchPatterns.end() )
    {
        // Is the pattern one that is configured for a 'all files' push method?
        if ( TPushStyle::PUSHSTYLE_MATCHING_ALL_FILES_WITH_REST_PERIOD == (*i).second )
        {
            // Does the file match a configured pattern?
            if ( filename.WildcardEquals( (*i).first, '*', false ) )
            {
                return true;
            }
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestination::QueueAllPreExistingFilesForDir( const CORE::CString& dir )
{GUCEF_TRACE;

    struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( dir.C_String() );
    if ( GUCEF_NULL != did )
    {
        do
        {
            if ( 0 != CORE::DI_Is_It_A_File( did ) )
            {
                CORE::CString filename = CORE::DI_Name( did );
                if ( filename != '.' && filename != ".." )
                {
                    if ( DoesFilenameMatchPushAllFilesPattern( filename ) )
                    {
                        // In this mode treat the pre-existing file as a new file to push
                        CORE::CString preexistingFilePath = CORE::CombinePath( dir, filename );
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePusher: Matched file \"" + preexistingFilePath + "\" to 'push all files' pattern" );
                        QueueNewFileForPushingAfterUnmodifiedRestPeriod( preexistingFilePath );
                    }
                }
            }
        }
        while ( CORE::DI_Next_Dir_Entry( did ) );

        CORE::DI_Cleanup( did );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CORE::CString::StringVector
FilePushDestination::GetFilePatternsForPushType( TPushStyle pushStyle ) const
{GUCEF_TRACE;

    CORE::CString::StringVector subset;
    TStringPushStyleMap::const_iterator i = m_settings.fileMatchPatterns.begin();
    while ( i != m_settings.fileMatchPatterns.end() )
    {
        if ( pushStyle == (*i).second )
        {
            subset.push_back( (*i).first );
        }
        ++i;
    }
    return subset;
}

/*-------------------------------------------------------------------------*/

FilePusher::FilePusher( void )
    : CORE::CObservingNotifier()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_filePushDestinations()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

FilePusher::~FilePusher()
{GUCEF_TRACE;

    m_httpServer.Close();
}

/*-------------------------------------------------------------------------*/

bool
FilePusher::Start( void )
{GUCEF_TRACE;

    bool totalSuccess = true;
    FilePushDestinationVector::iterator i = m_filePushDestinations.begin();
    while ( i != m_filePushDestinations.end() )
    {
        totalSuccess = (*i).Start() && totalSuccess;
        ++i;
    }    
    
    if ( m_httpServer.Listen() )                                                              
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: Opened REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        return totalSuccess;
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: Failed to open REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
    return false;

}

/*-------------------------------------------------------------------------*/

bool 
FilePusher::LoadConfig( const CORE::CValueList& appConfig   ,
                        const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    FilePushDestinationSettings appConfigSettings;
    if ( appConfigSettings.LoadConfig( appConfig ) )
    {
        FilePushDestination appConfigDestination;
        if ( appConfigDestination.LoadConfig( appConfigSettings ) )
            m_filePushDestinations.push_back( appConfigDestination );
    }

    CORE::CDataNode::TDataNodeVector destinationNodes = globalConfig.SearchForAll( "FilePusher\\Destination", '\\', true, true );
    CORE::CDataNode::TDataNodeVector::iterator i = destinationNodes.begin();
    while ( i != destinationNodes.end() )
    {
        FilePushDestinationSettings destSettings;
        if ( destSettings.LoadConfig( *(*i) ) )
        {
            FilePushDestination destination;
            if ( destination.LoadConfig( destSettings ) )
                m_filePushDestinations.push_back( destination );
        }
        ++i;
    }

    if ( m_filePushDestinations.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: Not a single push destination was able to be loaded from config, failing overall config load" );
        return false;
    }

    m_httpServer.SetPort( CORE::StringToUInt16( CORE::ResolveVars( appConfig.GetValueAlways( "RestApiPort", "10000" ) ) ) );
    m_httpRouter.SetResourceMapping( "/info", RestApiFilePusherInfoResource::THTTPServerResourcePtr( new RestApiFilePusherInfoResource( this ) )  );
    m_httpRouter.SetResourceMapping( "/config/appargs", RestApiFilePusherInfoResource::THTTPServerResourcePtr( new RestApiFilePusherConfigResource( this, true ) )  );
    m_httpRouter.SetResourceMapping( "/config", RestApiFilePusherInfoResource::THTTPServerResourcePtr( new RestApiFilePusherConfigResource( this, false ) )  );
    m_httpRouter.SetResourceMapping(  appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ), RestApiFilePusherInfoResource::THTTPServerResourcePtr( new COM::CDummyHTTPServerResource() )  );
    
    m_httpServer.GetRouterController()->AddRouterMapping( &m_httpRouter, "", "" );

    m_appConfig = appConfig;
    m_globalConfig.Copy( globalConfig );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList& 
FilePusher::GetAppConfig( void ) const
{
    return m_appConfig;
}

/*-------------------------------------------------------------------------*/

const CORE::CDataNode& 
FilePusher::GetGlobalConfig( void ) const
{
    return m_globalConfig;
}

/*-------------------------------------------------------------------------*/