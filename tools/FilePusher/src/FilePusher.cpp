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

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::UInt32 FilePusher::DefaultNewFileRestPeriodInSecs = 60 * 10; // Rest period of 10 minutes

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

FilePusher::FilePusher( void )
    : CORE::CObservingNotifier()
    , m_dirWatcher()
    , m_httpClient()
    , m_httpServer()
    , m_httpRouter()
    , m_appConfig()
    , m_globalConfig()
    , m_metricsTimer()
    , m_transmitMetrics( true )
    , m_fileMatchPatterns()
    , m_newFileRestPeriodTimer()
    , m_newFileRestQueue()
    , m_restingTimeForNewFilesInSecs( DefaultNewFileRestPeriodInSecs )
    , m_pushQueue()
    , m_pushTimer() 
    , m_dirsToWatch()
    , m_deleteFilesAfterSuccessfullPush( true )
    , m_filePushDestinationUri()
    , m_currentFilePushBuffer()
{GUCEF_TRACE;

    RegisterEventHandlers();    
}

/*-------------------------------------------------------------------------*/

FilePusher::~FilePusher()
{GUCEF_TRACE;

    m_httpClient.Close();
    m_httpServer.Close();
    m_dirWatcher.RemoveAllWatches();
}

/*-------------------------------------------------------------------------*/

void
FilePusher::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &FilePusher::OnMetricsTimerCycle );
    SubscribeTo( &m_metricsTimer                ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );
    
    TEventCallback callback2( this, &FilePusher::OnWatchedLocalDirFileCreation );
    SubscribeTo( &m_dirWatcher                             ,
                 CORE::CDirectoryWatcher::FileCreatedEvent ,
                 callback2                                 );

    TEventCallback callback3( this, &FilePusher::OnNewFileRestPeriodTimerCycle );
    SubscribeTo( &m_newFileRestPeriodTimer      ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback3                      );

    TEventCallback callback4( this, &FilePusher::OnFilePushTimerCycle );
    SubscribeTo( &m_pushTimer                   ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback4                      );
}

/*-------------------------------------------------------------------------*/

void
FilePusher::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_transmitMetrics )
    {

    }
}

/*-------------------------------------------------------------------------*/

void
FilePusher::OnFilePushTimerCycle( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventId  ,
                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // We only do a max of 1 file per cycle since this is a single-threaded app on purpose
    // As such we need to allow for time for other actions

    TStringUInt64Map::iterator i = m_pushQueue.begin();
    while ( i != m_pushQueue.end() )
    {
        // Load the file content from disk
        const CORE::CString& filePath = (*i).first;
        if ( !m_currentFilePushBuffer.LoadContentFromFile( filePath, (CORE::UInt32) (*i).second ) )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePusher: Unable to load bytes from file \"" + filePath + "\". Is it still in use? Skipping the file for now" );
            ++i;
            continue;
        }

        CORE::CString fileExt = CORE::ExtractFileExtention( filePath ).Lowercase();
        CORE::CString contentType = "application/octet-stream";
        if ( fileExt == "txt" || fileExt == "log" )
        {
            contentType = "text/plain";
        }
        
        // Begin the push
        CORE::CString filename = CORE::ExtractFilename( filePath );
        CORE::CString pushUrlForFile = m_filePushDestinationUri.ReplaceSubstr( "{filename}", filename );
        if ( m_httpClient.Post( pushUrlForFile, contentType, m_currentFilePushBuffer ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "FilePusher: Commenced HTTP POST for content from file \"" + filePath + "\"" );
            return;    
        }

        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FilePusher: Failed to HTTP POST bytes from file \"" + filePath + "\". Skipping the file for now" );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
FilePusher::QueueFileForPushing( const CORE::CString& filePath )
{GUCEF_TRACE;

    // We might get triggered multiple times for the same file.
    // Ignore redundant queueing requests
    if ( m_pushQueue.find( filePath ) != m_pushQueue.end() )
        return;

    m_pushQueue[ filePath ] = 0;
}

/*-------------------------------------------------------------------------*/

void
FilePusher::OnNewFileRestPeriodTimerCycle( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventId  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    TStringSet restedFiles;
    time_t nowTime = time( NULL );
    TStringTimeMap::iterator i = m_newFileRestQueue.begin();
    while ( i != m_newFileRestQueue.end() )
    {
        const CORE::CString& newFilePath = (*i).first;
        time_t& lastModified = (*i).second;

        lastModified = CORE::Get_Modification_Time( newFilePath.C_String() );
        if ( nowTime - lastModified > m_restingTimeForNewFilesInSecs )
        {
            // This file has not been modified for at least the required resting period.
            // As such tis file can now be considered a candidate for pushing.
            QueueFileForPushing( newFilePath );
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

void
FilePusher::QueueNewFileForPushingAfterUnmodifiedRestPeriod( const CORE::CString& newFilePath )
{GUCEF_TRACE;

    // Add the file to the list of files to be checked periodically to see if there is no more changes
    // being made to the file aka a resting period.
    time_t lastModified = CORE::Get_Modification_Time( newFilePath.C_String() );
    m_newFileRestQueue[ newFilePath ] = lastModified;
}

/*-------------------------------------------------------------------------*/

void
FilePusher::TriggerRolledOverFileCheck( const CORE::CString& dirWithFiles   ,
                                        const CORE::CString& patternToMatch )
{GUCEF_TRACE;

    // Check to see if we have multiple, thus rolled over, files in the target dir matching the pattern
    TUInt32StringMap matchedFiles;
    struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( dirWithFiles.C_String() );
    if ( GUCEF_NULL != did )
    {
        do
        {
            if ( CORE::DI_Is_It_A_File( did ) == 1 )
            {
                CORE::CString filename = DI_Name( did );
                if ( patternToMatch.WildcardEquals( filename, '*', false ) )
                {
                    matchedFiles[ CORE::DI_Timestamp( did ) ] = CORE::CombinePath( dirWithFiles, filename );    
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
            size_t index = 0; 
            size_t max = matchedFiles.size() - 1;
            TUInt32StringMap::iterator i = matchedFiles.begin();
            while ( index < max )
            {
                QueueFileForPushing( (*i).second );
                ++index; ++i;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
FilePusher::OnWatchedLocalDirFileCreation( CORE::CNotifier* notifier    ,
                                           const CORE::CEvent& eventId  ,
                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // Which file is new?
    CORE::CDirectoryWatcher::TFileCreatedEventData* fileCreatedEventData = static_cast< CORE::CDirectoryWatcher::TFileCreatedEventData* >( eventData );
    const CORE::CString& newFilePath = fileCreatedEventData->GetData();

    // Filter potentially unrelated files
    TPushStyle pushStyle = EPushStyle::PUSHSTYLE_UNKNOWN;
    CORE::CString patternMatched;
    CORE::CString newFilename = CORE::ExtractFilename( newFilePath );
    TStringPushStyleMap::iterator i = m_fileMatchPatterns.begin();
    while ( i != m_fileMatchPatterns.end() )
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
        case EPushStyle::PUSHSTYLE_ROLLED_OVER_FILES:
        {
            CORE::CString dirWithFiles = CORE::StripFilename( newFilePath );
            TriggerRolledOverFileCheck( dirWithFiles, patternMatched );
            return;
        }
        case EPushStyle::PUSHSTYLE_MATCHING_NEW_FILES_WITH_REST_PERIOD:
        {
            QueueNewFileForPushingAfterUnmodifiedRestPeriod( newFilePath );
            return;
        }
        case EPushStyle::PUSHSTYLE_UNKNOWN:
        default:
        {
            return;
        }
    };
}

/*-------------------------------------------------------------------------*/

bool
FilePusher::Start( void )
{GUCEF_TRACE;


    if ( m_transmitMetrics )
    {
        m_metricsTimer.SetInterval( 1000 );
        m_metricsTimer.SetEnabled( true );
    }
    
    m_newFileRestPeriodTimer.SetInterval( 1000 );
    m_newFileRestPeriodTimer.SetEnabled( true );

    m_pushTimer.SetInterval( 1000 );
    m_pushTimer.SetEnabled( true );
    
    if ( m_httpServer.Listen() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: Opened REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
        return true;
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: Failed to open REST API on port " + CORE::UInt16ToString( m_httpServer.GetPort() ) );
    return false;

}

/*-------------------------------------------------------------------------*/

bool 
FilePusher::LoadConfig( const CORE::CValueList& appConfig   ,
                        const CORE::CDataNode& globalConfig )
{GUCEF_TRACE;

    m_transmitMetrics = CORE::StringToBool( appConfig.GetValueAlways( "TransmitMetrics", "true" ) );

    m_restingTimeForNewFilesInSecs = CORE::StringToUInt32( CORE::ResolveVars( appConfig.GetValueAlways( "RestingTimeForNewFilesInSecs", CORE::UInt32ToString( DefaultNewFileRestPeriodInSecs ) ) ) );
    m_deleteFilesAfterSuccessfullPush = CORE::StringToBool( CORE::ResolveVars( appConfig.GetValueAlways( "DeleteFilesAfterSuccessfullPush", "true" ) ) );
    
    m_filePushDestinationUri = CORE::ResolveVars( appConfig.GetValueAlways( "FilePushDestinationUri" ) );
    if ( m_filePushDestinationUri.IsNULLOrEmpty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: You must specify setting \"FilePushDestinationUri\". It currently has no value" );
        return false;    
    }
    CORE::CString protocolSanityCheckUri = m_filePushDestinationUri.Lowercase();
    if ( 0 != protocolSanityCheckUri.HasSubstr( "http://", true ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: The specified push Uri \"" + m_filePushDestinationUri + "\" does not specify a valid supported protocol" );
        return false; 
    }

    CORE::CValueList::TStringVector settingValues = appConfig.GetValueVectorAlways( "DirToWatch" );
    CORE::CValueList::TStringVector::iterator n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        m_dirsToWatch.insert( settingValue );
        ++n;
    }
    if ( m_dirsToWatch.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: You must specify at least one instance of setting \"DirToWatch\". Currently there are none with a value" );
        return false;
    }

    settingValues = appConfig.GetValueVectorAlways( "FilePatternForNewFilesWithRestPeriod" );
    n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        m_fileMatchPatterns[ settingValue ] = PUSHSTYLE_MATCHING_NEW_FILES_WITH_REST_PERIOD;
        ++n;
    }
    settingValues = appConfig.GetValueVectorAlways( "FilePatternForRolledOverFiles" );
    n = settingValues.begin();
    while ( n != settingValues.end() )
    {
        CORE::CString settingValue = CORE::ResolveVars( (*n) );
        m_fileMatchPatterns[ settingValue ] = PUSHSTYLE_ROLLED_OVER_FILES;
        ++n;
    }
    if ( m_fileMatchPatterns.empty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "FilePusher: You must specify at least one instance of setting \"FilePatternForNewFilesWithRestPeriod\" or \"FilePatternForRolledOverFiles\". Currently there are none with a value" );
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