/*
 *  FileSorter: service which sorts files
 *
 *  Copyright (C) 1998 - 2024.  Dinand Vanvelzen
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

#ifndef GUCEF_APP_FILESORTER_H
#define GUCEF_APP_FILESORTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>
#include <vector>
#include <map>

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

#ifndef GUCEF_CORE_CFILESYSTEMDIRECTORYWATCHER_H
#include "gucefCORE_CFileSystemDirectoryWatcher.h"
#define GUCEF_CORE_CFILESYSTEMDIRECTORYWATCHER_H
#endif /* GUCEF_CORE_CFILESYSTEMDIRECTORYWATCHER_H ? */

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

#ifndef GUCEF_WEB_CTASKMANAGERSERVERRESOURCE_H
#include "gucefWEB_CTaskManagerServerResource.h"
#define GUCEF_WEB_CTASKMANAGERSERVERRESOURCE_H
#endif /* GUCEF_WEB_CTASKMANAGERSERVERRESOURCE_H ? */

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

class FileSorter;

class RestApiFileSorterInfoResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiFileSorterInfoResource( FileSorter* app );

    virtual ~RestApiFileSorterInfoResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    FileSorter* m_app;
};

/*-------------------------------------------------------------------------*/

class RestApiFileSorterConfigResource : public WEB::CCodecBasedHTTPServerResource
{
    public:

    RestApiFileSorterConfigResource( FileSorter* app, bool appConfig );

    virtual ~RestApiFileSorterConfigResource();

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    private:

    FileSorter* m_app;
    bool m_appConfig;
};

/*-------------------------------------------------------------------------*/

enum EFileType
{
    FILETYPE_UNKNOWN        = 0,

    FILETYPE_TEXT           = 1,
    FILETYPE_BINARY         = 2,
    FILETYPE_IMAGE          = 3,
    FILETYPE_AUDIO          = 4,
    FILETYPE_VIDEO          = 5,
    FILETYPE_ARCHIVE        = 6,
    FILETYPE_DOCUMENT,
    FILETYPE_PRESENTATION,
    FILETYPE_SPREADSHEET,
    FILETYPE_DATABASE,
    FILETYPE_EXECUTABLE,
    FILETYPE_SCRIPT,
    FILETYPE_WEBPAGE,
    FILETYPE_SOURCECODE,
    FILETYPE_CONFIGURATION,
    FILETYPE_LOG,
    FILETYPE_TEMPORARY,
    FILETYPE_COMPRESSED,
    FILETYPE_OTHER
};
typedef enum EFileType TFileType;

typedef std::set< TFileType > TFileTypeSet;
typedef std::map< CORE::CString, TFileType > TStringToFileTypeMap;
typedef std::map< TFileType, CORE::CString > TFileTypeToStringMap;

/*-------------------------------------------------------------------------*/

class FileTypeConfig : public CORE::CIConfigurable
{
    public:

    static const CORE::CString ClassTypeName;
    
    FileTypeConfig( void );

    virtual ~FileTypeConfig();

    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    TFileType GetFileTypeForFileExtension( const CORE::CString& fileExt ) const;

    static CORE::CString GetStringForFileType( const TFileType fileType );
    static TFileType GetFileTypeForString( const CORE::CString& fileTypeStr );
    static TFileTypeSet GetFileTypesForStringSet( const CORE::CString::StringSet& fileTypeStrs );
    static CORE::CString::StringSet GetStringSetForFileTypes( const TFileTypeSet& fileTypes );

    const CORE::CString& GetSortRootFolderForFileType( TFileType fileType ) const;

    const CORE::CString& GetSortRootFolderForFileExtension( const CORE::CString& fileExt ) const;

    private:

    TStringToFileTypeMap m_fileExtToTypeMap;
    TFileTypeToStringMap m_typeToSortFolderMap;
};

/*-------------------------------------------------------------------------*/

class FileSorterConfig : public CORE::CGloballyConfigurable
{
    public:

    static const CORE::CString ClassTypeName;
    
    FileSorterConfig( void );

    virtual ~FileSorterConfig();

    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    FileTypeConfig fileTypeConfig;
    CORE::CString vfsInboxPath;
    CORE::UInt32 vfsInboxPathScanIntervalInMs;
    CORE::CString vfsSortSourceRootPath;
    CORE::CString vfsSortedTargetRootPath;
    bool useDateTimeFolderStructure;
    CORE::Int16 dirStructureUtcOffsetInMins;
    CORE::CString yearFolderDecoration;
    bool useMonthFolder;
    CORE::CString monthFolderDecoration;
    bool useDayFolder;
    CORE::CString dayFolderDecoration;
    bool sortAllFileTypes;
    TFileTypeSet fileTypesToSort;

    private:

};

/*-------------------------------------------------------------------------*/

class FileSorter : public CORE::CTSGNotifier
{
    public:

    FileSorter( void );
    virtual ~FileSorter();

    bool Start( void );

    bool LoadConfig( const CORE::CValueList& appArgs     ,
                     const CORE::CDataNode& globalConfig );

    const FileSorterConfig& GetAppConfig( void ) const;

    const CORE::CDataNode& GetGlobalConfig( void ) const;

    private:

    typedef CORE::CTEventHandlerFunctor< FileSorter > TEventCallback;

    void RegisterEventHandlers( void );

    bool SortFile( const CORE::CString& currentVfsFilePath );

    bool SortInitialRootPaths( void );

    bool SortFilesInVfsPath( const CORE::CString& vfsRootPath );

    void OnAppStarted( CORE::CNotifier* notifier    ,
                       const CORE::CEvent& eventId  ,
                       CORE::CICloneable* eventData );
    
    void OnVfsInitializationCompleted( CORE::CNotifier* notifier    ,
                                       const CORE::CEvent& eventId  ,
                                       CORE::CICloneable* eventData );

    void OnVfsWatchedInboxDirChange( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventId  ,
                                     CORE::CICloneable* eventData );

    void OnInboxWatchTimerCycle( CORE::CNotifier* notifier    ,
                                 const CORE::CEvent& eventId  ,
                                 CORE::CICloneable* eventData );
    
    private:
    
    WEB::CHTTPServer m_httpServer;
    WEB::CDefaultHTTPServerRouter m_httpRouter;
    WEB::CTaskManagerServerResource m_taskManagementRsc;    
    CORE::CTimer m_inboxWatchTimer;
    CORE::CDataNode m_globalConfig;    
    CORE::CValueList m_appArgs;
    FileSorterConfig m_appConfig;
};

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_APP_FILESORTER_H ? */
