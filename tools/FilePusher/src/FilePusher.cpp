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

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CORECODECTYPES_H
#include "gucefCORE_CoreCodecTypes.h"
#define GUCEF_CORE_CORECODECTYPES_H
#endif /* GUCEF_CORE_CORECODECTYPES_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#include "gucefWEB_CDummyHTTPServerResource.h"
#define GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CDUMMYHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_WEB_CHTTPENCODINGTYPES_H
#include "gucefWEB_CHttpEncodingTypes.h"
#define GUCEF_WEB_CHTTPENCODINGTYPES_H
#endif /* GUCEF_WEB_CHTTPENCODINGTYPES_H ? */

#include "FilePusher.h"

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
    : WEB::CCodecBasedHTTPServerResource()
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
    : WEB::CCodecBasedHTTPServerResource()
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
    , watchSubDirsOfDirsToWatch( true )
    , restingTimeForNewFilesInSecs( DefaultNewFileRestPeriodInSecs )
    , deleteFilesAfterSuccessfullPush( false )
    , transmitMetrics( false )
    , compressFilesBeforePush( false )
    , fileCompressionCodecToUse( "gzip" )
    , fileCompressionCodecFileExt( "gz" )
    , fileCompressionTempDir()
    , fileTypesToCompress()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

FilePushDestinationSettings::FilePushDestinationSettings( const FilePushDestinationSettings& src )
    : filePushDestinationUri( src.filePushDestinationUri )
    , fileMatchPatterns( src.fileMatchPatterns )
    , dirsToWatch( src.dirsToWatch )
    , watchSubDirsOfDirsToWatch( src.watchSubDirsOfDirsToWatch )
    , restingTimeForNewFilesInSecs( src.restingTimeForNewFilesInSecs )
    , deleteFilesAfterSuccessfullPush( src.deleteFilesAfterSuccessfullPush )
    , transmitMetrics( src.transmitMetrics )
    , compressFilesBeforePush( src.compressFilesBeforePush )
    , fileCompressionCodecToUse( src.fileCompressionCodecToUse )
    , fileCompressionCodecFileExt( src.fileCompressionCodecFileExt )
    , fileCompressionTempDir( src.fileCompressionTempDir )
    , fileTypesToCompress( src.fileTypesToCompress )
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
        watchSubDirsOfDirsToWatch = src.watchSubDirsOfDirsToWatch;
        restingTimeForNewFilesInSecs = src.restingTimeForNewFilesInSecs;
        deleteFilesAfterSuccessfullPush = src.deleteFilesAfterSuccessfullPush;
        transmitMetrics = src.transmitMetrics;
        compressFilesBeforePush = src.compressFilesBeforePush;
        fileCompressionCodecToUse = src.fileCompressionCodecToUse;
        fileCompressionCodecFileExt = src.fileCompressionCodecFileExt;
        fileCompressionTempDir = src.fileCompressionTempDir;
        fileTypesToCompress = src.fileTypesToCompress;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestinationSettings::LoadConfig( const CORE::CValueList& appConfig )
{GUCEF_TRACE;

    transmitMetrics = CORE::StringToBool( appConfig.GetValueAlways( "TransmitMetrics" ), transmitMetrics );

    restingTimeForNewFilesInSecs = CORE::StringToUInt32( CORE::ResolveVars( appConfig.GetValueAlways( "RestingTimeForNewFilesInSecs" ) ), restingTimeForNewFilesInSecs );
    deleteFilesAfterSuccessfullPush = CORE::StringToBool( CORE::ResolveVars( appConfig.GetValueAlways( "DeleteFilesAfterSuccessfullPush" ) ), deleteFilesAfterSuccessfullPush );
    compressFilesBeforePush = CORE::StringToBool( CORE::ResolveVars( appConfig.GetValueAlways( "CompressFilesBeforePush" ) ), compressFilesBeforePush );
    fileCompressionCodecToUse = CORE::ResolveVars( appConfig.GetValueAlways( "FileCompressionCodecToUse", fileCompressionCodecToUse ) );
    fileCompressionCodecFileExt = CORE::ResolveVars( appConfig.GetValueAlways( "FileCompressionCodecFileExt", fileCompressionCodecFileExt ) );
    fileCompressionTempDir = CORE::ResolveVars( appConfig.GetValueAlways( "FileCompressionTempDir", fileCompressionTempDir ) );
    fileTypesToCompress = CORE::ResolveVars( appConfig.GetValueAlways( "FileTypesToCompress" ) ).ParseUniqueElements( ',', false );

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

    CORE::CValueList::TVariantVector settingValues = appConfig.GetValueVectorAlways( "DirToWatch" );
    CORE::CValueList::TVariantVector::iterator n = settingValues.begin();
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
    watchSubDirsOfDirsToWatch = CORE::StringToBool( CORE::ResolveVars( appConfig.GetValueAlways( "WatchSubDirsOfDirsToWatch" ) ), watchSubDirsOfDirsToWatch );

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

    transmitMetrics = CORE::StringToBool( rootNode.GetAttributeValueOrChildValueByName( "TransmitMetrics" ), transmitMetrics );

    restingTimeForNewFilesInSecs = CORE::StringToUInt32( CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "RestingTimeForNewFilesInSecs" ) ), restingTimeForNewFilesInSecs );
    deleteFilesAfterSuccessfullPush = CORE::StringToBool( CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "DeleteFilesAfterSuccessfullPush" ) ), deleteFilesAfterSuccessfullPush );
    compressFilesBeforePush = CORE::StringToBool( CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "CompressFilesBeforePush" ) ), compressFilesBeforePush );
    fileCompressionCodecToUse = CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "FileCompressionCodecToUse", fileCompressionCodecToUse ) );
    fileCompressionCodecFileExt = CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "FileCompressionCodecFileExt", fileCompressionCodecFileExt ) );
    fileCompressionTempDir = CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "FileCompressionTempDir", fileCompressionCodecFileExt ) );
    fileTypesToCompress = CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "FileTypesToCompress" ) ).ParseUniqueElements( ',', false );

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

    CORE::CDataNode::TVariantVector settingValues = rootNode.GetAttributeValueOrChildValuesByName( "DirToWatch" );
    CORE::CDataNode::TVariantVector::iterator n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n).AsString() );
        dirsToWatch.insert( settingValue );
        ++n;
    }
    if ( dirsToWatch.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestinationSettings: You must specify at least one instance of setting \"DirToWatch\". Currently there are none with a value" );
        return false;
    }
    watchSubDirsOfDirsToWatch = CORE::StringToBool( CORE::ResolveVars( rootNode.GetAttributeValueOrChildValueByName( "WatchSubDirsOfDirsToWatch" ) ), watchSubDirsOfDirsToWatch );

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
    , m_encodeTimer()
    , m_currentFilePushBuffer()
    , m_currentFileBeingPushed( GUCEF_NULL )
    , m_lastPushDurationInSecs( 0 )
    , m_lastEncodeDurationInSecs( 0 )
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
    , m_encodeTimer( src.m_encodeTimer )
    , m_currentFilePushBuffer( src.m_currentFilePushBuffer )
    , m_currentFileBeingPushed( GUCEF_NULL )
    , m_lastPushDurationInSecs( src.m_lastPushDurationInSecs )
    , m_lastEncodeDurationInSecs( src.m_lastEncodeDurationInSecs )
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
        m_encodeTimer = src.m_encodeTimer;
        m_currentFilePushBuffer = src.m_currentFilePushBuffer;
        //m_currentFileBeingPushed = src.m_currentFileBeingPushed;
        m_lastPushDurationInSecs = src.m_lastPushDurationInSecs;
        m_lastEncodeDurationInSecs = src.m_lastEncodeDurationInSecs;
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
    SubscribeTo( &m_dirWatcher                                       ,
                 CORE::CFileSystemDirectoryWatcher::FileCreatedEvent ,
                 callback2                                           );

    TEventCallback callback3( this, &FilePushDestination::OnNewFileRestPeriodTimerCycle );
    SubscribeTo( &m_newFileRestPeriodTimer      ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback3                      );

    TEventCallback callback4( this, &FilePushDestination::OnFilePushTimerCycle );
    SubscribeTo( &m_pushTimer                   ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback4                      );

    TEventCallback callback5( this, &FilePushDestination::OnFileEncodeTimerCycle );
    SubscribeTo( &m_encodeTimer                 ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback5                      );

    TEventCallback callback6( this, &FilePushDestination::OnAsyncVfsOperationCompleted );
    SubscribeTo( &VFS::CVfsGlobal::Instance()->GetVfs()     ,
                 VFS::CVFS::AsyncVfsOperationCompletedEvent ,
                 callback6                                  );

    TEventCallback callback7( this, &FilePushDestination::OnAllFilesDirScanTimerCycle );
    SubscribeTo( &m_allFilesDirScanTimer        ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback7                      );

    RegisterHttpEventHandlers();
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::RegisterHttpEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback5( this, &FilePushDestination::OnHttpClientConnected );
    SubscribeTo( &m_httpClient                    ,
                 WEB::CHTTPClient::ConnectedEvent ,
                 callback5                        );
    TEventCallback callback6( this, &FilePushDestination::OnHttpClientDisconnected );
    SubscribeTo( &m_httpClient                       ,
                 WEB::CHTTPClient::DisconnectedEvent ,
                 callback6                           );
    TEventCallback callback7( this, &FilePushDestination::OnHttpClientConnectionError );
    SubscribeTo( &m_httpClient                          ,
                 WEB::CHTTPClient::ConnectionErrorEvent ,
                 callback7                              );
    TEventCallback callback8( this, &FilePushDestination::OnHttpClientHttpError );
    SubscribeTo( &m_httpClient                    ,
                 WEB::CHTTPClient::HTTPErrorEvent ,
                 callback8                        );
    TEventCallback callback9( this, &FilePushDestination::OnHttpClientHttpRedirect );
    SubscribeTo( &m_httpClient                       ,
                 WEB::CHTTPClient::HTTPRedirectEvent ,
                 callback9                           );
    TEventCallback callback10( this, &FilePushDestination::OnHttpClientHttpContent );
    SubscribeTo( &m_httpClient                      ,
                 WEB::CHTTPClient::HTTPContentEvent ,
                 callback10                         );
    TEventCallback callback11( this, &FilePushDestination::OnHttpClientHttpDataRecieved );
    SubscribeTo( &m_httpClient                           ,
                 WEB::CHTTPClient::HTTPDataRecievedEvent ,
                 callback11                              );
    TEventCallback callback12( this, &FilePushDestination::OnHttpClientHttpDataSent );
    SubscribeTo( &m_httpClient                       ,
                 WEB::CHTTPClient::HTTPDataSentEvent ,
                 callback12                          );
    TEventCallback callback13( this, &FilePushDestination::OnHttpClientHttpTransferFinished );
    SubscribeTo( &m_httpClient                               ,
                 WEB::CHTTPClient::HTTPTransferFinishedEvent ,
                 callback13                                  );
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

    if ( m_settings.compressFilesBeforePush )
    {
        if ( m_settings.fileCompressionCodecToUse.IsNULLOrEmpty() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestination: Configured to compress files before push but no codec to use was specified" );
            return false;
        }

        m_encodeTimer.SetInterval( 1000 );
        m_encodeTimer.SetEnabled( true );

        if ( !m_settings.fileCompressionTempDir.IsNULLOrEmpty() )
        {
            if ( !CORE::CreateDirs( m_settings.fileCompressionTempDir ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePushDestination: Configured to compress files before push but the path to write temp files is not accessable or writable: " + m_settings.fileCompressionTempDir );
                return false;
            }
        }
    }

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

    CORE::CFileSystemDirectoryWatcher::CDirWatchOptions watchOptions;
    watchOptions.watchSubTree = m_settings.watchSubDirsOfDirsToWatch;
    watchOptions.watchForFileCreation = true;
    watchOptions.watchForFileDeletion = false;
    watchOptions.watchForFileModifications = false;
    watchOptions.watchForFileRenames = false;
    watchOptions.watchForDirCreation = false;
    watchOptions.watchForDirDeletion = false;
    watchOptions.watchForDirModifications = false;
    watchOptions.watchForDirRenames = false;
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
        if ( m_settings.compressFilesBeforePush )
        {
            GUCEF_METRIC_GAUGE( "FilePusher.FilesQueuedToEncode", (CORE::UInt32) m_encodeQueue.size(), 1.0f );
            GUCEF_METRIC_GAUGE( "FilePusher.LastEncodeDurationInSecs", m_lastEncodeDurationInSecs, 1.0f );
        }
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
    m_currentFileBeingPushed = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnHttpClientHttpError( CORE::CNotifier* notifier    ,
                                            const CORE::CEvent& eventId  ,
                                            CORE::CICloneable* eventData )
{GUCEF_TRACE;

    WEB::CHTTPClient::THTTPErrorEventData* httpErrorEventData = static_cast< WEB::CHTTPClient::THTTPErrorEventData* >( eventData );
    CORE::UInt32 httpErrorCode = (CORE::UInt32) httpErrorEventData->GetData();
    m_currentFileBeingPushed = GUCEF_NULL;

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: HTTP Client has experienced a HTTP error: " + CORE::UInt32ToString( httpErrorCode ) );
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
    OnFilePushFinished( notifier  ,
                        eventId   ,
                        eventData );
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
        if ( asyncOpResult->successState )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: VFS Async operation of type " +  CORE::ToString( asyncOpResult->operationType ) +  " finished successfully in " +
                    CORE::UInt32ToString( m_lastPushDurationInSecs ) + " secs" );

            switch ( asyncOpResult->operationType )
            {
                case VFS::ASYNCVFSOPERATIONTYPE_ENCODEFILE:
                case VFS::ASYNCVFSOPERATIONTYPE_ENCODEDATAASFILE:
                {
                    OnAsyncVfsFileEncodeCompleted( notifier  ,
                                                   eventId   ,
                                                   eventData );
                    break;
                }
                case VFS::ASYNCVFSOPERATIONTYPE_STOREDATAASFILE:
                {
                    OnFilePushFinished( notifier  ,
                                        eventId   ,
                                        eventData );
                    break;
                }
                case VFS::ASYNCVFSOPERATIONTYPE_MOUNTARCHIVE:
                case VFS::ASYNCVFSOPERATIONTYPE_COPYFILE:
                case VFS::ASYNCVFSOPERATIONTYPE_DECODEFILE:
                default:
                {
                    break;
                }
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: VFS Async operation type " +  CORE::ToString( asyncOpResult->operationType ) +  " failed in " +
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
FilePushDestination::OnAsyncVfsFileEncodeCompleted( CORE::CNotifier* notifier    ,
                                                    const CORE::CEvent& eventId  ,
                                                    CORE::CICloneable* eventData )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "FilePushDestination:OnAsyncVfsFileEncodeCompleted" );

    VFS::CAsyncVfsTaskResultData* asyncOpResult = static_cast< VFS::CAsyncVfsTaskResultData* >( eventData );
    if ( GUCEF_NULL != asyncOpResult )
    {
        m_lastEncodeDurationInSecs = asyncOpResult->durationInSecs;
        if ( asyncOpResult->successState )
        {
            switch ( asyncOpResult->operationType )
            {
                case VFS::ASYNCVFSOPERATIONTYPE_ENCODEDATAASFILE:
                case VFS::ASYNCVFSOPERATIONTYPE_ENCODEFILE:
                {
                    const CORE::CString& originalFilePath = asyncOpResult->asyncRequestId;

                    TStringPushEntryMap::iterator i = m_encodeQueue.find( originalFilePath );
                    if ( i != m_encodeQueue.end() )
                    {
                        QueueFileForPushing( (*i).second );
                        m_encodeQueue.erase( i );
                        m_currentFileBeingPushed = GUCEF_NULL;
                    }
                    else
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnAsyncVfsFileEncodeCompleted: Encoded file not found in encode queue" );
                    }
                    break;
                }
                default:
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnAsyncVfsFileEncodeCompleted: Unsupported operation type in handler" );
                    break;
                }
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnAsyncVfsFileEncodeCompleted: Async operation failed" );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnFilePushFinished( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventId  ,
                                         CORE::CICloneable* eventData )
{GUCEF_TRACE;

    VFS::CAsyncVfsTaskResultData* asyncOpResult = static_cast< VFS::CAsyncVfsTaskResultData* >( eventData );
    if ( GUCEF_NULL != asyncOpResult )
    {
        VFS::CStoreAsFileTaskData* taskData = static_cast< VFS::CStoreAsFileTaskData* >( asyncOpResult->GetTaskData() );
        m_lastPushDurationInSecs = asyncOpResult->durationInSecs;

        if ( asyncOpResult->successState && GUCEF_NULL != taskData )
        {
            const CORE::CString& originalFilePath = taskData->asyncRequestId;

            switch ( asyncOpResult->operationType )
            {
                case VFS::ASYNCVFSOPERATIONTYPE_STOREDATAASFILE:
                {
                    TStringPushEntryMap::iterator i = m_pushQueue.find( originalFilePath );
                    if ( i != m_pushQueue.end() )
                    {
                        PushEntry& entry = (*i).second;

                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushFinished: Successfully pushed file \"" + originalFilePath + "\" to VFS path \"" + taskData->filepath + "\"" );

                        if ( m_settings.deleteFilesAfterSuccessfullPush )
                        {
                            if ( !entry.encodedFilepath.IsNULLOrEmpty() )
                            {
                                if ( VFS::CVfsGlobal::Instance()->GetVfs().DeleteFile( entry.encodedFilepath, true ) )
                                {
                                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushFinished: Successfully deleted temp encoding file \"" + entry.encodedFilepath + "\"" );
                                }
                                else
                                {
                                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushFinished: Failed to deleted temp encoding file \"" + entry.encodedFilepath + "\"" );
                                }
                            }

                            if ( CORE::DeleteFile( entry.filePath ) )
                            {
                                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushFinished: Successfully deleted pushed file \"" + entry.filePath + "\"" );
                            }
                            else
                            {
                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushFinished: Failed to delete pushed file \"" + entry.filePath + "\"" );
                            }
                        }

                        m_pushQueue.erase( i );

                        m_currentFilePushBuffer.Clear();
                        m_currentFileBeingPushed = GUCEF_NULL;
                    }
                    else
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushFinished: Encoded file not found in encode queue" );
                    }
                    break;
                }
                default:
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushFinished: Unsupported operation type in handler" );
                    break;
                }
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushFinished: Async operation failed" );
        }
    }


}

/*-------------------------------------------------------------------------*/

CORE::CString
FilePushDestination::DetermineWatchedDirSubPath( const CORE::CString& filePath ) const
{GUCEF_TRACE;

    // First find the deepest root dir that matches
    CORE::UInt32 bestMatchLength = 0;
    CORE::CString bestMatchRoot;
    TStringSet::const_iterator i = m_settings.dirsToWatch.begin();
    while ( i != m_settings.dirsToWatch.end() )
    {
        CORE::UInt32 matchLength = filePath.FindMaxSubstrEquality( (*i), 0, true, true );
        if ( matchLength > bestMatchLength )
        {
            bestMatchRoot = (*i);
        }
        ++i;
    }

    // Now extract the part of the path that relative to the found root is a sub-dir
    CORE::CString pathRemainder = filePath.CutChars( bestMatchRoot.Length(), true, 0 );
    pathRemainder = CORE::StripFilename( pathRemainder );
    if ( !pathRemainder.IsNULLOrEmpty() )
    {
        // Ensure consistent path termination, always have a seperator at the end
        char lastChar = pathRemainder[ pathRemainder.Length()-1 ];
        if ( GUCEF_DIRSEPCHAR != lastChar && GUCEF_DIRSEPCHAROPPOSITE != lastChar )
        {
            pathRemainder += GUCEF_DIRSEPCHAR;
        }
    }
    return pathRemainder;
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestination::PushFileUsingHttp( const PushEntry& entry )
{GUCEF_TRACE;

    CORE::CString filename;
    if ( entry.encodedFilepath.IsNULLOrEmpty() )
    {
        // Load the file content from disk
        if ( !m_currentFilePushBuffer.LoadContentFromFile( entry.filePath, (CORE::UInt32) entry.currentOffsetInFile ) )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:PushFileUsingVfs: Unable to load bytes from file \"" + entry.filePath + "\". Is it still in use? Skipping the file for now" );
            return false;
        }
        filename = CORE::ExtractFilename( entry.filePath );
    }
    else
    {
        // Load the file content from disk, keeping in mind the encoded file is in the VFS
        if ( !VFS::CVfsGlobal::Instance()->GetVfs().LoadFile( m_currentFilePushBuffer, entry.encodedFilepath, "rb" ) )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:PushFileUsingVfs: Unable to load bytes from encoded vfs file \"" + entry.encodedFilepath + "\". Is it still in use? Skipping the file for now" );
            return false;
        }
        filename = CORE::ExtractFilename( entry.encodedFilepath );
    }
    m_currentFileBeingPushed = &entry;

    // Begin the push

    CORE::CString fileExt = CORE::ExtractFileExtention( filename ).Lowercase();
    CORE::CString contentType = "application/octet-stream";
    if ( fileExt == "txt" || fileExt == "log" )
    {
        contentType = "text/plain";
    }

    CORE::CString watchedDirSubDirPath;
    if ( m_settings.filePushDestinationUri.HasSubstr( "{watchedDirSubDirPath}", true ) > 0 )
    {
        watchedDirSubDirPath = DetermineWatchedDirSubPath( entry.filePath );
        watchedDirSubDirPath = watchedDirSubDirPath.ReplaceChar( '\\', '/' );
    }

    CORE::CString pushUrlForFile = m_settings.filePushDestinationUri.ReplaceSubstr( "{filename}", filename );
    pushUrlForFile = pushUrlForFile.ReplaceSubstr( "{watchedDirSubDirPath}", watchedDirSubDirPath );
    pushUrlForFile = pushUrlForFile.CompactRepeatingChar( '/' );

    if ( m_httpClient.Post( pushUrlForFile, contentType, m_currentFilePushBuffer ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Commenced HTTP POST for content from file \"" + filename + "\"" );
        return true;
    }

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Failed to HTTP POST bytes from file \"" + filename + "\". Skipping the file for now" );
    m_currentFilePushBuffer.Clear();
    m_currentFileBeingPushed = GUCEF_NULL;
    return false;
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestination::PushFileUsingVfs( const PushEntry& entry )
{GUCEF_TRACE;

    CORE::CString filename;
    if ( entry.encodedFilepath.IsNULLOrEmpty() )
    {
        // Load the file content from disk
        if ( !m_currentFilePushBuffer.LoadContentFromFile( entry.filePath, (CORE::UInt32) entry.currentOffsetInFile ) )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:PushFileUsingVfs: Unable to load bytes from file \"" + entry.filePath + "\". Is it still in use? Skipping the file for now" );
            return false;
        }
        filename = CORE::ExtractFilename( entry.filePath );
    }
    else
    {
        // Load the file content from disk, keeping in mind the encoded file is in the VFS
        if ( !VFS::CVfsGlobal::Instance()->GetVfs().LoadFile( m_currentFilePushBuffer, entry.encodedFilepath, "rb" ) )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:PushFileUsingVfs: Unable to load bytes from encoded vfs file \"" + entry.encodedFilepath + "\". Is it still in use? Skipping the file for now" );
            return false;
        }
        filename = CORE::ExtractFilename( entry.encodedFilepath );
    }

    m_currentFileBeingPushed = &entry;

    // Begin the push
    CORE::CString watchedDirSubDirPath;
    if ( m_settings.filePushDestinationUri.HasSubstr( "{watchedDirSubDirPath}", true ) > 0 )
    {
        watchedDirSubDirPath = DetermineWatchedDirSubPath( entry.filePath );
        watchedDirSubDirPath = watchedDirSubDirPath.ReplaceChar( '\\', '/' );
    }

    CORE::CString pushUrlForFile = m_settings.filePushDestinationUri.ReplaceSubstr( "{filename}", filename );
    pushUrlForFile = pushUrlForFile.ReplaceSubstr( "{watchedDirSubDirPath}", watchedDirSubDirPath );
    pushUrlForFile = pushUrlForFile.CutChars( 6, true, 0 ); // Cut vfs://
    pushUrlForFile = pushUrlForFile.CompactRepeatingChar( '/' );

    // Store the file as an async operation
    if ( VFS::CVfsGlobal::Instance()->GetVfs().StoreAsFileAsync( pushUrlForFile, m_currentFilePushBuffer, 0, true, GUCEF_NULL, entry.filePath ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:PushFileUsingVfs: Commenced async push of content from file \"" + filename + "\" to VFS path \"" + pushUrlForFile + "\"" );
        return true;
    }

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:PushFileUsingVfs: Failed to request async push of content from file \"" + filename + "\" to VFS path \"" + pushUrlForFile + "\". Skipping the file for now" );
    m_currentFilePushBuffer.Clear();
    m_currentFileBeingPushed = GUCEF_NULL;
    return false;
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnFileEncodeTimerCycle( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // We only do a max of 1 file per cycle since this is a single-threaded app on purpose
    // As such we need to allow for time for other actions

    if ( GUCEF_NULL != m_currentFileBeingPushed )
        return;

    TStringPushEntryMap::iterator i = m_encodeQueue.begin();
    while ( i != m_encodeQueue.end() )
    {
        const CORE::CString& filePath = (*i).first;
        const PushEntry& entry = (*i).second;

        if ( CORE::FileExists( filePath ) )
        {
            CORE::CFileAccess fileAccess;
            if ( fileAccess.Open( filePath, "rb" ) )
            {
                m_currentFileBeingPushed = &entry;

                // Encode the file as an async operation
                if ( VFS::CVfsGlobal::Instance()->GetVfs().EncodeAsFileAsync( fileAccess                              ,
                                                                              entry.encodedFilepath                   ,
                                                                              true                                    ,
                                                                              CORE::CoreCodecTypes::CompressionCodec  ,
                                                                              m_settings.fileCompressionCodecToUse    ,
                                                                              entry.filePath                          ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFileEncodeTimerCycle: Commenced async encode of content from file \"" + filePath + "\" to VFS path \"" + entry.encodedFilepath + "\"" );
                    return;
                }

                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFileEncodeTimerCycle: Failed to request async push of content from file \"" + filePath + "\" to VFS path \"" + entry.encodedFilepath + "\". Skipping the file for now" );
                m_currentFilePushBuffer.Clear();
                m_currentFileBeingPushed = GUCEF_NULL;
                return;
            }
            else
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFileEncodeTimerCycle: Failed to open file for when we were about to request async encoding it: \"" + filePath + "\", removing it from the queue" );
                m_encodeQueue.erase( i );
                return;
            }
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFileEncodeTimerCycle: File no longer exists when we were about to request async encoding it: \"" + filePath + "\", removing it from the queue" );
            m_encodeQueue.erase( i );
            return;
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::OnFilePushTimerCycle( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventId  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // We only do a max of 1 file per cycle since this is a single-threaded app on purpose
    // As such we need to allow for time for other actions

    if ( GUCEF_NULL != m_currentFileBeingPushed )
        return;

    TStringPushEntryMap::iterator i = m_pushQueue.begin();
    while ( i != m_pushQueue.end() )
    {
        const CORE::CString& filePath = (*i).first;
        const PushEntry& entry = (*i).second;

        if ( CORE::FileExists( filePath ) )
        {
            // Files in the push queue should already have an encoded version waiting if we are indeed applying an encoding
            if ( !entry.encodedFilepath.IsNULLOrEmpty() )
            {
                if ( !VFS::CVfsGlobal::Instance()->GetVfs().FileExists( entry.encodedFilepath ) )
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushTimerCycle: Encoded file does not exists when we were about to commence pushing it: \"" + entry.encodedFilepath + "\", queuing file again for encoding instead" );
                    m_encodeQueue[ filePath ] = entry;
                    m_pushQueue.erase( i );
                    return;
                }
            }

            if ( 0 == m_settings.filePushDestinationUri.HasSubstr( "http://", true ) )
            {
                if ( PushFileUsingHttp( entry ) )
                    return;
            }
            if ( 0 == m_settings.filePushDestinationUri.HasSubstr( "vfs://", true ) )
            {
                if ( PushFileUsingVfs( entry ) )
                    return;
            }
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination:OnFilePushTimerCycle: File no longer exists when we were about to commence pushing it: \"" + filePath + "\", removing it from the queue" );
            m_pushQueue.erase( i );
            return;
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
FilePushDestination::IsFileATempEncodingFile( const CORE::CString& filePath ) const
{GUCEF_TRACE;

    TStringPushEntryMap::const_iterator i = m_encodeQueue.begin();
    while ( i != m_encodeQueue.end() )
    {
        if ( filePath == (*i).second.encodedFilepath )
            return true;
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::QueueFileForPushOrEncode( const CORE::CString& filePath )
{GUCEF_TRACE;

    // We might get triggered multiple times for the same file.
    // Ignore redundant queueing requests
    if ( m_pushQueue.find( filePath ) != m_pushQueue.end() )
        return;
    if ( m_encodeQueue.find( filePath ) != m_encodeQueue.end() )
        return;
    if ( IsFileATempEncodingFile( filePath ) )
        return;

    if ( m_settings.compressFilesBeforePush )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Queueing file \"" + filePath + "\" for encoding" );
        const CORE::CString& encodedFileExt = m_settings.fileCompressionCodecFileExt.IsNULLOrEmpty() ? m_settings.fileCompressionCodecToUse : m_settings.fileCompressionCodecFileExt;
        PushEntry& pushEntry = m_encodeQueue[ filePath ];
        pushEntry.currentOffsetInFile = 0;
        if ( m_settings.fileCompressionTempDir.IsNULLOrEmpty() )
        {
            // If no specific temp dir was given assume a temp vfs path is mounted
            pushEntry.encodedFilepath += "/temp/filepusher/encoding/" + CORE::ExtractFilename( filePath ) + '.' + encodedFileExt;
        }
        else
        {
            CORE::CString filename = CORE::ExtractFilename( filePath ) + '.' + encodedFileExt;
            pushEntry.encodedFilepath = CORE::CombinePath( m_settings.fileCompressionTempDir, filename );

        }
        pushEntry.filePath = filePath;
    }
    else
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Queueing file \"" + filePath + "\" for pushing" );
        PushEntry& pushEntry = m_pushQueue[ filePath ];
        pushEntry.currentOffsetInFile = 0;
        pushEntry.filePath = filePath;
    }
}

/*-------------------------------------------------------------------------*/

void
FilePushDestination::QueueFileForPushing( const PushEntry& entry )
{GUCEF_TRACE;

    // We might get triggered multiple times for the same file.
    // Ignore redundant queueing requests
    if ( m_pushQueue.find( entry.filePath ) != m_pushQueue.end() )
        return;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePushDestination: Queueing file \"" + entry.filePath + "\" for pushing" );
    PushEntry& pushEntry = m_pushQueue[ entry.filePath ];
    pushEntry = entry;
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
            if ( lastChange.GetTimeDifferenceInMillisecondsTowards( nowTime ) > (CORE::Int64) ( m_settings.restingTimeForNewFilesInSecs * 1000 ) )
            {
                // This file has not been modified for at least the required resting period.
                // As such tis file can now be considered a candidate for pushing.
                QueueFileForPushOrEncode( newFilePath );
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
    TStringPushEntryMap::iterator i = m_pushQueue.find( newFilePath );
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
                    QueueFileForPushOrEncode( (*n) );
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
    CORE::CFileSystemDirectoryWatcher::TFileCreatedEventData* fileCreatedEventData = static_cast< CORE::CFileSystemDirectoryWatcher::TFileCreatedEventData* >( eventData );
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

    bool totalSuccess = true;
    struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( dir.C_String() );
    if ( GUCEF_NULL != did )
    {
        do
        {
            CORE::CString name = CORE::DI_Name( did );
            if ( name != '.' && name != ".." )
            {
                if ( 0 != CORE::DI_Is_It_A_File( did ) )
                {
                    if ( DoesFilenameMatchPushAllFilesPattern( name ) )
                    {
                        // In this mode treat the pre-existing file as a new file to push
                        CORE::CString preexistingFilePath = CORE::CombinePath( dir, name );
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePusher: Matched file \"" + preexistingFilePath + "\" to 'push all files' pattern" );
                        QueueNewFileForPushingAfterUnmodifiedRestPeriod( preexistingFilePath );
                    }
                }
                else
                {
                    if ( m_settings.watchSubDirsOfDirsToWatch )
                    {
                        CORE::CString subDir = CORE::CombinePath( dir, name );
                        totalSuccess = totalSuccess && QueueAllPreExistingFilesForDir( subDir );
                    }
                }
            }
        }
        while ( CORE::DI_Next_Dir_Entry( did ) );

        CORE::DI_Cleanup( did );
        return totalSuccess;
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
    m_httpRouter.SetResourceMapping(  appConfig.GetValueAlways( "RestBasicHealthUri", "/health/basic" ), RestApiFilePusherInfoResource::THTTPServerResourcePtr( new WEB::CDummyHTTPServerResource() )  );

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
