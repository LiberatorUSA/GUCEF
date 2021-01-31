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

#ifndef GUCEF_APP_FILEPUSHER_H
#define GUCEF_APP_FILEPUSHER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

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

#ifndef GUCEF_CORE_CDATETIME_H
#include "gucefCORE_CDateTime.h"
#define GUCEF_CORE_CDATETIME_H
#endif /* GUCEF_CORE_CDATETIME_H ? */

#ifndef GUCEF_CORE_CDIRECTORYWATCHER_H
#include "gucefCORE_CDirectoryWatcher.h"
#define GUCEF_CORE_CDIRECTORYWATCHER_H
#endif /* GUCEF_CORE_CDIRECTORYWATCHER_H ? */

#ifndef GUCEF_WEB_CHTTPSERVER_H
#include "gucefWEB_CHTTPServer.h"
#define GUCEF_WEB_CHTTPSERVER_H
#endif /* GUCEF_WEB_CHTTPSERVER_H ? */

#ifndef GUCEF_WEB_CHTTPCLIENT_H
#include "gucefWEB_CHTTPClient.h"
#define GUCEF_WEB_CHTTPCLIENT_H
#endif /* GUCEF_WEB_CHTTPCLIENT_H ? */

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

class FilePusher;

class RestApiFilePusherInfoResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiFilePusherInfoResource( FilePusher* app );

    virtual ~RestApiFilePusherInfoResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    FilePusher* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiFilePusherConfigResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiFilePusherConfigResource( FilePusher* app, bool appConfig );

    virtual ~RestApiFilePusherConfigResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    FilePusher* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

class FilePushDestinationSettings
{
    public:

    static const CORE::UInt32 DefaultNewFileRestPeriodInSecs;

    enum EPushStyle
    {
        PUSHSTYLE_UNKNOWN                               = 0,
        PUSHSTYLE_ROLLED_OVER_FILES                        ,
        PUSHSTYLE_MATCHING_NEW_FILES_WITH_REST_PERIOD      ,
        PUSHSTYLE_MATCHING_ALL_FILES_WITH_REST_PERIOD
    };
    typedef enum EPushStyle TPushStyle;
    typedef std::map< CORE::CString, TPushStyle > TStringPushStyleMap;
    
    typedef std::set< CORE::CString > TStringSet;
    typedef std::map< CORE::UInt32, CORE::CString > TUInt32StringMap;
    typedef std::map< CORE::UInt64, CORE::CString::StringVector > TUInt64StringVectorMap;

    bool LoadConfig( const CORE::CValueList& config );
    bool LoadConfig( const CORE::CDataNode& rootNode );

    FilePushDestinationSettings( void );
    FilePushDestinationSettings( const FilePushDestinationSettings& src );
    ~FilePushDestinationSettings();
    FilePushDestinationSettings& operator=( const FilePushDestinationSettings& other );

    CORE::CString filePushDestinationUri;
    TStringPushStyleMap fileMatchPatterns;
    TStringSet dirsToWatch;
    bool watchSubDirsOfDirsToWatch;
    CORE::UInt32 restingTimeForNewFilesInSecs;
    bool deleteFilesAfterSuccessfullPush;
    bool transmitMetrics;
    bool compressFilesBeforePush;
    CORE::CString fileCompressionCodecToUse;
    CORE::CString fileCompressionCodecFileExt;
    CORE::CString fileCompressionTempDir;
    TStringSet fileTypesToCompress;
};

/*-------------------------------------------------------------------------*/

class FilePushDestination : public CORE::CObservingNotifier
{
    public:

    FilePushDestination( void );

    FilePushDestination( const FilePushDestination& src );

    virtual ~FilePushDestination();

    FilePushDestination& operator=( const FilePushDestination& src );

    bool Start( void );

    bool LoadConfig( const FilePushDestinationSettings& settings );
    
    private:

    class PushEntry
    {
        public:
        
        CORE::UInt64 currentOffsetInFile;
        CORE::CString encodedFilepath;
        CORE::CString filePath;
    };

    typedef CORE::CTEventHandlerFunctor< FilePushDestination > TEventCallback;
    typedef enum FilePushDestinationSettings::EPushStyle TPushStyle;
    typedef FilePushDestinationSettings::TStringSet TStringSet;
    typedef FilePushDestinationSettings::TStringPushStyleMap TStringPushStyleMap;
    
    void RegisterEventHandlers( void );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnWatchedLocalDirFileCreation( CORE::CNotifier* notifier    ,
                                   const CORE::CEvent& eventId  ,
                                   CORE::CICloneable* eventData );

    void
    OnNewFileRestPeriodTimerCycle( CORE::CNotifier* notifier    ,
                                   const CORE::CEvent& eventId  ,
                                   CORE::CICloneable* eventData );
    
    void
    OnAllFilesDirScanTimerCycle( CORE::CNotifier* notifier    ,
                                 const CORE::CEvent& eventId  ,
                                 CORE::CICloneable* eventData );

    void
    OnFilePushTimerCycle( CORE::CNotifier* notifier    ,
                          const CORE::CEvent& eventId  ,
                          CORE::CICloneable* eventData );

    void
    OnHttpClientConnected( CORE::CNotifier* notifier    ,
                           const CORE::CEvent& eventId  ,
                           CORE::CICloneable* eventData );

    void
    OnHttpClientDisconnected( CORE::CNotifier* notifier    ,
                              const CORE::CEvent& eventId  ,
                              CORE::CICloneable* eventData );
                           
    void
    OnHttpClientConnectionError( CORE::CNotifier* notifier    ,
                                 const CORE::CEvent& eventId  ,
                                 CORE::CICloneable* eventData );

    void
    OnHttpClientHttpError( CORE::CNotifier* notifier    ,
                           const CORE::CEvent& eventId  ,
                           CORE::CICloneable* eventData );

    void
    OnHttpClientHttpRedirect( CORE::CNotifier* notifier    ,
                              const CORE::CEvent& eventId  ,
                              CORE::CICloneable* eventData );

    void
    OnHttpClientHttpContent( CORE::CNotifier* notifier    ,
                             const CORE::CEvent& eventId  ,
                             CORE::CICloneable* eventData );

    void
    OnHttpClientHttpDataSent( CORE::CNotifier* notifier    ,
                              const CORE::CEvent& eventId  ,
                              CORE::CICloneable* eventData );

    void
    OnHttpClientHttpDataRecieved( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventId  ,
                                  CORE::CICloneable* eventData );

    void
    OnHttpClientHttpTransferFinished( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventId  ,
                                      CORE::CICloneable* eventData );
    
    void
    OnAsyncVfsOperationCompleted( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventId  ,
                                  CORE::CICloneable* eventData );

    void
    OnAsyncVfsFileEncodeCompleted( CORE::CNotifier* notifier    ,
                                   const CORE::CEvent& eventId  ,
                                   CORE::CICloneable* eventData );

    void
    QueueNewFileForPushingAfterUnmodifiedRestPeriod( const CORE::CString& newFilePath );

    bool
    QueueAllPreExistingFilesForDir( const CORE::CString& dir );

    bool
    DoesFilenameMatchPushAllFilesPattern( const CORE::CString& filename ) const;

    void
    TriggerRolledOverFileCheck( const CORE::CString& dirWithFiles   ,
                                const CORE::CString& patternToMatch );

    void
    TriggerRolledOverFileCheck( const CORE::CString& dirWithFiles                  ,
                                const CORE::CString::StringVector& patternsToMatch );
    
    void
    QueueFileForPushing( const PushEntry& entry );

    void
    QueueFileForPushOrEncode( const CORE::CString& filePath );

    bool 
    PushFileUsingHttp( const PushEntry& entry );

    bool
    PushFileUsingVfs( const PushEntry& entry );

    void
    OnFilePushFinished( CORE::CNotifier* notifier    ,
                        const CORE::CEvent& eventId  ,
                        CORE::CICloneable* eventData );

    static CORE::CDateTime GetLatestTimestampForFile( const CORE::CString& filePath );

    CORE::CString::StringVector
    GetFilePatternsForPushType( TPushStyle pushStyle ) const;

    CORE::CString
    DetermineWatchedDirSubPath( const CORE::CString& filePath  ) const;

    bool 
    IsFileATempEncodingFile( const CORE::CString& filePath ) const;
    
    private:

    typedef std::map< CORE::CString, CORE::CDateTime > TStringTimeMap;
    typedef std::map< CORE::CString, PushEntry > TStringPushEntryMap;
    typedef std::map< CORE::CDateTime, CORE::CString::StringVector > TDateTimeStringVectorMap;

    WEB::CHTTPClient m_httpClient;
    CORE::CDirectoryWatcher m_dirWatcher;
    CORE::CTimer m_allFilesDirScanTimer;        
    CORE::CTimer m_newFileRestPeriodTimer;
    TStringTimeMap m_newFileRestQueue;
    TStringPushEntryMap m_encodeQueue;
    TStringPushEntryMap m_pushQueue;
    CORE::CTimer m_pushTimer;
    CORE::CDynamicBuffer m_currentFilePushBuffer;
    const PushEntry* m_currentFileBeingPushed;
    CORE::UInt32 m_lastPushDurationInSecs;
    CORE::UInt32 m_lastEncodeDurationInSecs;
    CORE::CTimer m_metricsTimer;
    FilePushDestinationSettings m_settings;
};

/*-------------------------------------------------------------------------*/

class FilePusher : public CORE::CObservingNotifier
{
    public:

    FilePusher( void );
    virtual ~FilePusher();

    bool Start( void );

    bool LoadConfig( const CORE::CValueList& appConfig   ,
                     const CORE::CDataNode& globalConfig );

    const CORE::CValueList& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;
    
    private:

    typedef std::vector< FilePushDestination > FilePushDestinationVector;
    
    WEB::CHTTPServer m_httpServer;
    WEB::CDefaultHTTPServerRouter m_httpRouter;
    CORE::CValueList m_appConfig;
    CORE::CDataNode m_globalConfig;    
    FilePushDestinationVector m_filePushDestinations;
};

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_APP_FILEPUSHER_H ? */
