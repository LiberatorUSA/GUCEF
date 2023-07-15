/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

#ifndef DVFILEUTILS_H
#include "dvfileutils.h"                /* Needed for dir itteration */
#define DVFILEUTILS_H
#endif /* DVFILEUTILS_H ? */

#ifndef DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define DVCPPSTRINGUTILS_H
#endif /* DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef CMFILEACCESS_H
#include "CMFileAccess.h"
#define CMFILEACCESS_H
#endif /* CMFILEACCESS_H ? */

#ifndef CFILEACCESS_H
#include "CFileAccess.h"
#define CFILEACCESS_H
#endif /* CFILEACCESS_H ? */

#ifndef CDATANODE_H
#include "CDataNode.h"          /* node for data storage */
#define CDATANODE_H
#endif /* CDATANODE_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

#ifndef DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */
#define DVCPPSTRINGUTILS_H
#endif /* DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_VFS_CASYNCVFSOPERATION_H
#include "gucefVFS_CAsyncVfsOperation.h"
#define GUCEF_VFS_CASYNCVFSOPERATION_H
#endif /* GUCEF_VFS_CASYNCVFSOPERATION_H ? */

#include "gucefVFS_CVFS.h"           /* definition of the file implemented here */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CVFS::AsyncVfsOperationCompletedEvent = "GUCEF::VFS::CVFS::AsyncVfsOperationCompletedEvent";
const CORE::CEvent CVFS::ArchiveMountedEvent = "GUCEF::VFS::CVFS::ArchiveMountedEvent";
const CORE::CEvent CVFS::ArchiveUnmountedEvent = "GUCEF::VFS::CVFS::ArchiveUnmountedEvent";
const CORE::CEvent CVFS::DelayedArchiveMountingCompletedEvent = "GUCEF::VFS::CVFS::DelayedArchiveMountingCompletedEvent";
const CORE::CEvent CVFS::VfsInitializationCompletedEvent = "GUCEF::VFS::CVFS::VfsInitializationCompletedEvent";

const CORE::CString CVFS::FileSystemArchiveTypeName = "FileSystem";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CVFS::RegisterEvents( void )
{GUCEF_TRACE;

    AsyncVfsOperationCompletedEvent.Initialize();
    ArchiveMountedEvent.Initialize();
    ArchiveUnmountedEvent.Initialize();
    DelayedArchiveMountingCompletedEvent.Initialize();
    VfsInitializationCompletedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CVFS::CVFS( void )
    : CORE::CTSGNotifier()
    , CORE::CGloballyConfigurable( false )
    , CORE::CIDirectoryWatcher()
    , m_mountList()
    , _maxmemloadsize( 1024 )
    , m_abstractArchiveFactory()
    , m_fileSystemArchiveFactory()
    , m_delayMountedArchiveSettings()
    , m_delayedArchiveMountingIsComplete( false )
    , m_rwdataLock( true )
{GUCEF_TRACE;

    RegisterEvents();
    RegisterEventHandlers();
    RegisterArchiveFactory( FileSystemArchiveTypeName, m_fileSystemArchiveFactory );
}

/*-------------------------------------------------------------------------*/

CVFS::~CVFS()
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_rwdataLock );
    
    UnmountAllArchives();
    UnregisterAllArchiveFactories();
    SignalUpcomingDestruction();
}

/*-------------------------------------------------------------------------*/

void
CVFS::RegisterEventHandlers( void )
{GUCEF_TRACE;

    // Success or fail we use the config load finish event messages to trigger delayed mounting
    TEventCallback callback( this, &CVFS::OnGlobalConfigLoadFinished );
    SubscribeTo( &CORE::CCoreGlobal::Instance()->GetConfigStore()   ,
                 CORE::CConfigStore::GlobalConfigLoadCompletedEvent ,
                 callback                                           );
    TEventCallback callback2( this, &CVFS::OnGlobalConfigLoadFinished );
    SubscribeTo( &CORE::CCoreGlobal::Instance()->GetConfigStore() ,
                 CORE::CConfigStore::GlobalConfigLoadFailedEvent  ,
                 callback2                                        );
}

/*-------------------------------------------------------------------------*/

void
CVFS::UnmountAllArchives( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_rwdataLock );

    while ( !m_mountList.empty() )
    {
        TMountEntry& mountEntry = *m_mountList.begin();
        TArchiveUnmountedEventData eData( mountEntry.mountPath ); 
        if ( mountEntry.archive->UnloadArchive() )
        {
            m_archivePtrToMountEntryLookup.erase( mountEntry.archive.GetPointerAlways() );
            m_mountList.erase( m_mountList.begin() );
            NotifyObservers( ArchiveUnmountedEvent, &eData );  
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CVFS::UnregisterAllArchiveFactories( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_rwdataLock );
    m_abstractArchiveFactory.UnregisterAllConcreteFactories();
}

/*-------------------------------------------------------------------------*/

bool
CVFS::StoreAsFileAsync( const CORE::CString& filepath       ,
                        const CORE::CDynamicBuffer& data    ,
                        const CORE::UInt64 offset           ,
                        const bool overwrite                ,
                        CORE::CICloneable* requestorData    ,
                        const CORE::CString& asyncRequestId )
{GUCEF_TRACE;

    CStoreAsFileTaskData operationData;
    operationData.operationType = ASYNCVFSOPERATIONTYPE_STOREDATAASFILE;
    operationData.asyncRequestId = asyncRequestId;
    operationData.filepath = filepath;
    operationData.data.LinkTo( data );
    operationData.offset = offset;
    operationData.overwrite = overwrite;
    operationData.SetRequestorData( requestorData );

    CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
    return !CORE::TaskStatusIsAnError( threadPool->QueueTask( CAsyncVfsOperation::TaskType, &operationData, GUCEF_NULL, &AsObserver() ) );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MountArchiveAsync( const CArchiveSettings& settings    ,
                         CORE::CICloneable* requestorData    ,
                         const CORE::CString& asyncRequestId )
{GUCEF_TRACE;

    CMountArchiveTaskData operationData;
    operationData.operationType = ASYNCVFSOPERATIONTYPE_MOUNTARCHIVE;
    operationData.asyncRequestId = asyncRequestId;
    operationData.settings = settings;
    operationData.SetRequestorData( requestorData );

    CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
    return !CORE::TaskStatusIsAnError( threadPool->QueueTask( CAsyncVfsOperation::TaskType, &operationData, GUCEF_NULL, &AsObserver() ) );
}

/*-------------------------------------------------------------------------*/

void
CVFS::OnArchiveDirectoryWatcherEvent( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventid  ,
                                      CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    VFS::CArchive* archive = static_cast< VFS::CArchive* >( notifier );
        
    if ( IsDirectoryWatcherDirEvent( eventid ) )
    {
        MT::CScopeReaderLock lock( m_rwdataLock );

        TArchivePtrToMountEntryMap::iterator i = m_archivePtrToMountEntryLookup.find( archive );
        if ( i != m_archivePtrToMountEntryLookup.end() )
        {
            TMountEntry* mountEntry = (*i).second;

            if ( eventid == CORE::CDirectoryWatcherEvents::DirRenamedEvent )
            {
                CORE::CDirectoryWatcherEvents::TDirRenamedEventData* dirRenameInfo = static_cast< CORE::CDirectoryWatcherEvents::TDirRenamedEventData* >( eventdata );
                if ( GUCEF_NULL != dirRenameInfo )
                {
                    CORE::CDirectoryWatcherEvents::TDirRenamedEventData vfsAdjusted;
                    vfsAdjusted.GetData().newDirName = mountEntry->mountPath + '/' + dirRenameInfo->GetData().newDirName;
                    vfsAdjusted.GetData().oldDirName = mountEntry->mountPath + '/' + dirRenameInfo->GetData().oldDirName;
                    vfsAdjusted.GetData().newDirName = ConformVfsDirPath( vfsAdjusted.GetData().newDirName );
                    vfsAdjusted.GetData().oldDirName = ConformVfsDirPath( vfsAdjusted.GetData().oldDirName );
                    NotifyObservers( eventid, &vfsAdjusted );
                }              
            }
            else
            {
                CORE::TCloneableString* dirPath = static_cast< CORE::TCloneableString* >( eventdata );
                if ( GUCEF_NULL != dirPath )
                {
                    CORE::TCloneableString vfsPath( mountEntry->mountPath + '/' + *dirPath );
                    vfsPath = ConformVfsDirPath( vfsPath );
                    NotifyObservers( eventid, &vfsPath );
                }
            }
        }
    }
    else
    if ( IsDirectoryWatcherFileEvent( eventid ) )
    {
        MT::CScopeReaderLock lock( m_rwdataLock );
        
        TArchivePtrToMountEntryMap::iterator i = m_archivePtrToMountEntryLookup.find( archive );
        if ( i != m_archivePtrToMountEntryLookup.end() )
        {            
            TMountEntry* mountEntry = (*i).second;
                
            if ( eventid == CORE::CDirectoryWatcherEvents::FileRenamedEvent )
            {
                CORE::CDirectoryWatcherEvents::TFileRenamedEventData* fileRenameInfo = static_cast< CORE::CDirectoryWatcherEvents::TFileRenamedEventData* >( eventdata );
                if ( GUCEF_NULL != fileRenameInfo )
                {
                    CORE::CDirectoryWatcherEvents::TFileRenamedEventData vfsAdjusted;
                    vfsAdjusted.GetData().newFilename = mountEntry->mountPath + '/' + fileRenameInfo->GetData().newFilename;
                    vfsAdjusted.GetData().oldFilename = mountEntry->mountPath + '/' + fileRenameInfo->GetData().oldFilename;
                    vfsAdjusted.GetData().newFilename = ConformVfsFilePath( vfsAdjusted.GetData().newFilename );
                    vfsAdjusted.GetData().oldFilename = ConformVfsFilePath( vfsAdjusted.GetData().oldFilename );
                    NotifyObservers( eventid, &vfsAdjusted );
                }
            }
            else
            {
                CORE::TCloneableString* filePath = static_cast< CORE::TCloneableString* >( eventdata );
                if ( GUCEF_NULL != filePath )
                {
                    CORE::TCloneableString vfsPath( mountEntry->mountPath + '/' + *filePath );
                    vfsPath = ConformVfsFilePath( vfsPath );
                    NotifyObservers( eventid, &vfsPath );
                }                
            }
        }
    }
    else
    {
        // adminstrative event
        NotifyObservers( eventid );
    }
}

/*-------------------------------------------------------------------------*/

void
CVFS::OnPumpedNotify( CORE::CNotifier* notifier    ,
                      const CORE::CEvent& eventid  ,
                      CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    if ( CAsyncVfsOperation::AsyncVfsOperationCompletedEvent == eventid )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "VFS:OnPumpedNotify: Async operation completed, passing on event notification" );
        NotifyObservers( AsyncVfsOperationCompletedEvent, eventdata );
    }

    if ( IsDirectoryWatcherEvent( eventid ) )
    {
        OnArchiveDirectoryWatcherEvent( notifier  ,
                                        eventid   ,
                                        eventdata );
    }
}

/*-------------------------------------------------------------------------*/

bool
CVFS::IsInitialized( void ) const
{GUCEF_TRACE;

    return m_delayedArchiveMountingIsComplete;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::IsConnected( void ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_rwdataLock );
    
    bool isConnectedOverall = true;
    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& mountEntry = (*i);
        if ( !mountEntry.archive->IsConnected() )
        {
            isConnectedOverall = false;
            break;
        }
        ++i;
    }
    
    return isConnectedOverall;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::IsHealthy( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    bool isHealthyOverall = true;
    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& mountEntry = (*i);
        if ( !mountEntry.archive->IsHealthy() )
        {
            isHealthyOverall = false;
            break;
        }
        ++i;
    }
    
    return isHealthyOverall;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::IsDelayedArchiveMountingCompleted( void ) const
{GUCEF_TRACE;

    return m_delayedArchiveMountingIsComplete;
}

/*-------------------------------------------------------------------------*/

void
CVFS::MountAllDelayMountedArchives( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_rwdataLock );

    while ( !m_delayMountedArchiveSettings.empty() )
    {
        TArchiveSettingsVector::iterator i = m_delayMountedArchiveSettings.begin();
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CVFS:MountAllDelayMountedArchives: Commencing delayed mounting of archive with name: " + (*i).GetArchiveName() );
        if ( MountArchive( (*i) ) )
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CVFS:MountAllDelayMountedArchives: Succeeded in delayed mounting of archive with name: " + (*i).GetArchiveName() );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CVFS:MountAllDelayMountedArchives: Failed delayed mounting of archive with name: " + (*i).GetArchiveName() );
        }
        m_delayMountedArchiveSettings.erase( i );
    }
    m_delayedArchiveMountingIsComplete = true;
    
    lock.EarlyUnlock();

    NotifyObservers( DelayedArchiveMountingCompletedEvent );
}

/*-------------------------------------------------------------------------*/

void
CVFS::OnGlobalConfigLoadFinished( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventid  ,
                                  CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    MountAllDelayMountedArchives();
    
    NotifyObservers( VfsInitializationCompletedEvent );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::DeleteFile( const CString& filePath, bool okIfItDoesNotExist )
{GUCEF_TRACE;

    // @TODO: This functionality could be improved by taking into account outstanding references to the resource
    //        Not all archive types might be able to handle denying a delete themselves during ongoing I/O especially with remote storage

    CString path = ConformVfsFilePath( filePath );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Search for a file match and attempt to delete if a match exists
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        TArchivePtr archive = mountLink.mountEntry->archive;
        if ( archive->FileExists( mountLink.remainder ) )
        {
            return archive->DeleteFile( mountLink.remainder );
        }
        ++i;
    }

    // No such file found
    return okIfItDoesNotExist;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MoveFile( const CString& oldFilePath ,
                const CString& newFilePath ,
                const bool overwrite       )
{GUCEF_TRACE;

    CString oldPath = ConformVfsFilePath( oldFilePath );
    CString newPath = ConformVfsFilePath( newFilePath );
    
    if ( oldPath == newPath )
    {
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS:MoveFile: old and new path are the same. No action taken. Path= " + oldPath );
        return true;
    }    
    
    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get lists of all eligable mounts
    TConstMountLinkVector oldPathMountLinks;
    GetEligableMounts( oldPath, false, oldPathMountLinks );    
    TConstMountLinkVector newPathMountLinks;
    GetEligableMounts( newPath, false, newPathMountLinks );

    TConstMountLinkVector::iterator i = oldPathMountLinks.begin();
    while ( i != oldPathMountLinks.end() )
    {
        TConstMountLink& oldMountLink = (*i);
        TConstMountLinkVector::iterator n = newPathMountLinks.begin();
        while ( n != newPathMountLinks.end() )
        {            
            TConstMountLink& newMountLink = (*n);
            if ( oldMountLink.mountEntry == newMountLink.mountEntry )
            {
                // Found a match where both old and new path are available via the same archive
                // this takes priority over any cross-archive logical 'moves'
                TArchivePtr archive = newMountLink.mountEntry->archive;
                return archive->MoveFile( oldMountLink.remainder , 
                                          newMountLink.remainder , 
                                          overwrite              );
            }
            ++n;
        }
        ++i;
    }

    // Since we could not find an archive that can hold both the old and new location we will have to
    // perform a logical move which is a copy followed by a delete

    if( CopyFile( oldFilePath ,
                  newFilePath ,
                  overwrite   ) )
    {
        return DeleteFile( oldFilePath, false );
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MoveFileAsync( const CORE::CString& oldFilePath    ,
                     const CORE::CString& newFilePath    ,
                     const bool overwrite                ,
                     const CORE::CString& asyncRequestId )
{GUCEF_TRACE;

    CMoveFileTaskData operationData;
    operationData.operationType = ASYNCVFSOPERATIONTYPE_MOVEFILE;
    operationData.asyncRequestId = asyncRequestId;
    operationData.originalFilepath = oldFilePath;
    operationData.newFilepath = newFilePath;
    operationData.overwrite = overwrite;

    CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
    return !CORE::TaskStatusIsAnError( threadPool->QueueTask( CAsyncVfsOperation::TaskType, &operationData, GUCEF_NULL, &AsObserver() ) );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::CopyFile( const CORE::CString& originalFilepath ,
                const CORE::CString& copyFilepath     ,
                const bool overwrite                  )
{GUCEF_TRACE;

    if ( originalFilepath == copyFilepath )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:CopyFile: original and target path cannot be the same: " + originalFilepath );
        return false;
    }

    CVFSHandlePtr originalFile = GetFile( originalFilepath, "rb", overwrite );
    if ( !originalFile || GUCEF_NULL == originalFile->GetAccess() || !originalFile->GetAccess()->IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:CopyFile: Cannot obtain original file: " + originalFilepath );
        return false;
    }

    CVFSHandlePtr targetFile = GetFile( copyFilepath, "wb", overwrite );
    if ( !targetFile || GUCEF_NULL == targetFile->GetAccess() || !targetFile->GetAccess()->IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:CopyFile: Cannot obtain access to output file: " + copyFilepath );
        return false;
    }

    UInt64 bytesWritten = targetFile->GetAccess()->Write( *originalFile->GetAccess() );

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VFS:CopyFile: Successfully copied file \"" + originalFilepath +
            "\" to \"" + copyFilepath + "\"" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::CopyFileAsync( const CORE::CString& originalFilepath ,
                     const CORE::CString& copyFilepath     ,
                     const bool overwrite                  ,
                     const CORE::CString& asyncRequestId   )
{GUCEF_TRACE;

    CCopyFileTaskData operationData;
    operationData.operationType = ASYNCVFSOPERATIONTYPE_COPYFILE;
    operationData.asyncRequestId = asyncRequestId;
    operationData.originalFilepath = originalFilepath;
    operationData.copyFilepath = copyFilepath;
    operationData.overwrite = overwrite;

    CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
    return !CORE::TaskStatusIsAnError( threadPool->QueueTask( CAsyncVfsOperation::TaskType, &operationData, GUCEF_NULL, &AsObserver() ) );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::EncodeFile( const CORE::CString& originalFilepath ,
                  const CORE::CString& encodedFilepath  ,
                  const bool overwrite                  ,
                  const CORE::CString& codecFamily      ,
                  const CORE::CString& encodeCodec      )
{GUCEF_TRACE;

    if ( originalFilepath == encodedFilepath )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeFile: in-place replacement not suppported at present" );
        return false;
    }

    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();
    CORE::CCodecRegistry::TICodecPtr codec = codecRegistry.TryGetCodec( codecFamily, encodeCodec );
    if ( codec.IsNULL() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeFile: Cannot encode file due to inability to obtain codec " +
            encodeCodec + " for codec family " + codecFamily );
        return false;
    }

    CVFSHandlePtr originalFile = GetFile( originalFilepath, "rb", overwrite );
    if ( !originalFile || GUCEF_NULL == originalFile->GetAccess() || !originalFile->GetAccess()->IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeFile: Cannot obtain original file: " + originalFilepath );
        return false;
    }

    CVFSHandlePtr targetFile = GetFile( encodedFilepath, "wb", overwrite );
    if ( !targetFile || GUCEF_NULL == targetFile->GetAccess() || !targetFile->GetAccess()->IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeFile: Cannot obtain access to output file: " + encodedFilepath );
        return false;
    }

    if ( !codec->Encode( *originalFile->GetAccess(), *targetFile->GetAccess() ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeFile: Failed to encode file \"" + originalFilepath +
            "\" using codec \"" + encodeCodec + "\" from codec family \"" + codecFamily + "\"" );
        return false;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeFile: Successfully encoded file \"" + originalFilepath +
            "\" using codec \"" + encodeCodec + "\" from codec family \"" + codecFamily + "\"" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::EncodeFileAsync( const CORE::CString& originalFilepath ,
                       const CORE::CString& encodedFilepath  ,
                       const bool overwrite                  ,
                       const CORE::CString& codecFamily      ,
                       const CORE::CString& encodeCodec      ,
                       const CORE::CString& asyncRequestId   )
{GUCEF_TRACE;

    CEncodeFileTaskData operationData;
    operationData.operationType = ASYNCVFSOPERATIONTYPE_ENCODEFILE;
    operationData.asyncRequestId = asyncRequestId;
    operationData.originalFilepath = originalFilepath;
    operationData.encodedFilepath = encodedFilepath;
    operationData.overwrite = overwrite;
    operationData.codecFamily = codecFamily;
    operationData.encodeCodec = encodeCodec;

    CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
    return !CORE::TaskStatusIsAnError( threadPool->QueueTask( CAsyncVfsOperation::TaskType, &operationData, GUCEF_NULL, &AsObserver() ) );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::DecodeAsFile( CORE::CDynamicBuffer& decodedOutput  ,
                    const CORE::UInt64 bufferOffset      ,
                    const CORE::CString& encodedFilePath ,
                    const CORE::CString& codecFamily     ,
                    const CORE::CString& decodeCodec     )
{GUCEF_TRACE;

    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();
    CORE::CCodecRegistry::TICodecPtr codec = codecRegistry.TryGetCodec( codecFamily, decodeCodec );
    if ( codec.IsNULL() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeAsFile: Cannot decode file due to inability to obtain codec " +
            decodeCodec + " for codec family " + codecFamily );
        return false;
    }

    CVFSHandlePtr sourceFile = GetFile( encodedFilePath, "rb", false );
    if ( !sourceFile || GUCEF_NULL == sourceFile->GetAccess() || !sourceFile->GetAccess()->IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeAsFile: Cannot obtain access to output file: " + encodedFilePath );
        return false;
    }

    CORE::CDynamicBufferAccess bufferAccess( decodedOutput );
    bufferAccess.Setpos( (UInt32) bufferOffset );

    if ( !codec->Decode( *sourceFile->GetAccess(), bufferAccess  ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeAsFile: Failed to decode buffer using codec \"" +
            decodeCodec + "\" from codec family \"" + codecFamily + "\"" );
        return false;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeAsFile: Successfully decoded buffer using codec \"" +
        decodeCodec + "\" from codec family \"" + codecFamily + "\" loaded from \"" + encodedFilePath + "\"" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::EncodeAsFile( const CORE::CDynamicBuffer& data     ,
                    const CORE::UInt64 bufferOffset      ,
                    const CORE::CString& encodedFilepath ,
                    const bool overwrite                 ,
                    const CORE::CString& codecFamily     ,
                    const CORE::CString& encodeCodec     )
{GUCEF_TRACE;

    if ( data.GetDataSize() < bufferOffset )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeAsFile: Invalid buffer offset given which is larger than the available data" );
        return false;
    }

    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();
    CORE::CCodecRegistry::TICodecPtr codec = codecRegistry.TryGetCodec( codecFamily, encodeCodec );
    if ( codec.IsNULL() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeAsFile: Cannot encode file due to inability to obtain codec " +
            encodeCodec + " for codec family " + codecFamily );
        return false;
    }

    CVFSHandlePtr targetFile = GetFile( encodedFilepath, "wb", overwrite );
    if ( !targetFile || GUCEF_NULL == targetFile->GetAccess() || !targetFile->GetAccess()->IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeAsFile: Cannot obtain access to output file: " + encodedFilepath );
        return false;
    }

    CORE::CDynamicBufferAccess bufferAccess( data );
    bufferAccess.Setpos( (UInt32) bufferOffset );

    if ( !codec->Encode( bufferAccess, *targetFile->GetAccess() ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:EncodeAsFile: Failed to encode buffer using codec \"" +
            encodeCodec + "\" from codec family \"" + codecFamily + "\"" );
        return false;
    }

    targetFile->GetAccess()->Flush();

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeFile: Successfully encoded buffer using codec \"" +
        encodeCodec + "\" from codec family \"" + codecFamily + "\"" );
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::EncodeAsFileAsync( const CORE::CDynamicBuffer& data     ,
                         const CORE::UInt64 bufferOffset      ,
                         const CORE::CString& encodedFilepath ,
                         const bool overwrite                 ,
                         const CORE::CString& codecFamily     ,
                         const CORE::CString& encodeCodec     ,
                         const CORE::CString& asyncRequestId  )
{GUCEF_TRACE;

    CEncodeBufferAsFileTaskData operationData;
    operationData.operationType = ASYNCVFSOPERATIONTYPE_ENCODEDATAASFILE;
    operationData.asyncRequestId = asyncRequestId;
    operationData.data = data;
    operationData.bufferOffset = bufferOffset;
    operationData.encodedFilepath = encodedFilepath;
    operationData.overwrite = overwrite;
    operationData.codecFamily = codecFamily;
    operationData.encodeCodec = encodeCodec;

    CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
    return !CORE::TaskStatusIsAnError( threadPool->QueueTask( CAsyncVfsOperation::TaskType, &operationData, GUCEF_NULL, &AsObserver() ) );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::EncodeAsFile( CORE::CIOAccess& externalData        ,
                    const CORE::CString& encodedFilepath ,
                    const bool overwrite                 ,
                    const CORE::CString& codecFamily     ,
                    const CORE::CString& encodeCodec     )
{GUCEF_TRACE;

    // For now we just route to a memory buffer based implementation
    // @TODO: improve

    CORE::CDynamicBuffer buffer;
    externalData.Read( buffer, 1 );

    return EncodeAsFile( buffer, 0, encodedFilepath, overwrite, codecFamily, encodeCodec );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::EncodeAsFileAsync( CORE::CIOAccess& externalData        ,
                         const CORE::CString& encodedFilepath ,
                         const bool overwrite                 ,
                         const CORE::CString& codecFamily     ,
                         const CORE::CString& encodeCodec     ,
                         const CORE::CString& asyncRequestId  )
{GUCEF_TRACE;

    // For now we just route to a memory buffer based implementation
    // @TODO: improve

    CORE::CDynamicBuffer buffer;
    externalData.Read( buffer, 1 );

    return EncodeAsFileAsync( buffer, 0, encodedFilepath, overwrite, codecFamily, encodeCodec, asyncRequestId );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::DecodeFile( const CORE::CString& originalFilepath ,
                  const CORE::CString& decodedFilepath  ,
                  const bool overwrite                  ,
                  const CORE::CString& codecFamily      ,
                  const CORE::CString& decodeCodec      )
{GUCEF_TRACE;

    if ( originalFilepath == decodedFilepath )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeFile: in-place replacement not suppported at present" );
        return false;
    }

    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();
    CORE::CCodecRegistry::TICodecPtr codec = codecRegistry.TryGetCodec( codecFamily, decodeCodec );
    if ( codec.IsNULL() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeFile: Cannot encode file due to inability to obtain codec " +
            decodeCodec + " for codec family " + codecFamily );
        return false;
    }

    CVFSHandlePtr originalFile = GetFile( originalFilepath, "rb", overwrite );
    if ( !originalFile || GUCEF_NULL == originalFile->GetAccess() || !originalFile->GetAccess()->IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeFile: Cannot obtain original file: " + originalFilepath );
        return false;
    }

    CVFSHandlePtr targetFile = GetFile( decodedFilepath, "wb", overwrite );
    if ( !targetFile || GUCEF_NULL == targetFile->GetAccess() || !targetFile->GetAccess()->IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeFile: Cannot obtain access to output file: " + decodedFilepath );
        return false;
    }

    if ( !codec->Encode( *originalFile->GetAccess(), *targetFile->GetAccess() ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeFile: Failed to encode file \"" + originalFilepath +
            "\" using codec \"" + decodeCodec + "\" from codec family \"" + codecFamily + "\"" );
        return false;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VFS:DecodeFile: Successfully decoded file \"" + originalFilepath +
            "\" using codec \"" + decodeCodec + "\" from codec family \"" + codecFamily + "\"" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::DecodeFileAsync( const CORE::CString& originalFilepath ,
                       const CORE::CString& decodedFilepath  ,
                       const bool overwrite                  ,
                       const CORE::CString& codecFamily      ,
                       const CORE::CString& decodeCodec      ,
                       const CORE::CString& asyncRequestId   )
{GUCEF_TRACE;

    CDecodeFileTaskData operationData;
    operationData.operationType = ASYNCVFSOPERATIONTYPE_DECODEFILE;
    operationData.asyncRequestId = asyncRequestId;
    operationData.originalFilepath = originalFilepath;
    operationData.decodedFilepath = decodedFilepath;
    operationData.overwrite = overwrite;
    operationData.codecFamily = codecFamily;
    operationData.decodeCodec = decodeCodec;

    CORE::ThreadPoolPtr threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
    return !CORE::TaskStatusIsAnError( threadPool->QueueTask( CAsyncVfsOperation::TaskType, &operationData, GUCEF_NULL, &AsObserver() ) );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::StoreAsFile( const CORE::CString& file        ,
                   const CORE::CDynamicBuffer& data ,
                   const CORE::UInt64 offset        ,
                   const bool overwrite             )
{GUCEF_TRACE;

    CString filepath = ConformVfsFilePath( file );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( filepath, true, mountLinks );

    // Try to store using the available archives
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        TArchivePtr archive = mountLink.mountEntry->archive;

        if ( archive->StoreAsFile( mountLink.remainder  ,
                                   data                 ,
                                   offset               ,
                                   overwrite            ) )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Vfs: Stored " + CORE::UInt32ToString( data.GetDataSize() )  +
                    " bytes as file content at offset " + CORE::UInt64ToString( offset ) + " using archive mounted at: " + mountLink.remainder );
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Vfs: Failed to store " + CORE::UInt32ToString( data.GetDataSize() )  +
                    " bytes as file content at offset " + CORE::UInt64ToString( offset ) + " using archive mounted at: " + mountLink.remainder );
        }

        ++i;
    }

    if ( mountLinks.empty() )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Vfs: Unable to store " + CORE::UInt32ToString( data.GetDataSize() )  +
                " bytes as file content at offset " + CORE::UInt64ToString( offset ) + ". No eligible archives mounted" );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CVFS::CVFSHandlePtr
CVFS::GetFile( const CORE::CString& file          ,
               const char* mode /* = "rb" */      ,
               const bool overwrite /* = false */ )
{GUCEF_TRACE;

    CString filepath = ConformVfsFilePath( file );
    bool fileMustExist = *mode == 'r';

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( filepath, overwrite, mountLinks );

    // Find the file in the available archives
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);

        bool tryToGetFile = true;
        if ( fileMustExist )
        {
            if ( !mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
            {
                tryToGetFile = false;
            }
        }

        if ( tryToGetFile )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "Vfs: Found requested file using mount link remainder: " + mountLink.remainder );
            TArchivePtr archive = mountLink.mountEntry->archive;
            CVFSHandlePtr filePtr = archive->GetFile( mountLink.remainder ,
                                                      mode                ,
                                                      _maxmemloadsize     ,
                                                      overwrite           );

            if ( !filePtr )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Vfs: Requested file pre-conditions passed but could not be loaded from mounted archive using mount link remainder: " + mountLink.remainder );
            }
            return filePtr;
        }

        ++i;
    }

    // Unable to load file
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Vfs: Unable to locate a mount which can provide the file: " + file );
    return CVFSHandlePtr();
}

/*-------------------------------------------------------------------------*/

bool
CVFS::LoadFile( CORE::CDynamicBuffer& destinationBuffer ,
                const CORE::CString& filePath           ,
                const char* mode                        )
{GUCEF_TRACE;

    // This operation logically only supports read modes
    if ( 0 != strcmp( "rb", mode ) && 0 != strcmp( "r", mode ) )
        return false;

    // First load the file as a VFS reference as usual
    CVFSHandlePtr fileReference = GetFile( filePath, mode, false );
    if ( !fileReference || GUCEF_NULL == fileReference->GetAccess() )
        return false;

    // load the data from whatever abstracted vfs medium into memory
    destinationBuffer.Append( *fileReference->GetAccess() );

    // When we leave this scope the VFS reference is released
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::LoadFile( CORE::CDataNode& destination    ,
                const CORE::CString& filePath   ,
                const CORE::CString& codecToUse )
{GUCEF_TRACE;

    
    CORE::CString actualCodecToUse = codecToUse;
    if ( actualCodecToUse.IsNULLOrEmpty() )
        actualCodecToUse = CORE::ExtractFileExtention( filePath ); 
    if ( actualCodecToUse.IsNULLOrEmpty() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Vfs:LoadFile: No codec type name or file extension provided for file: " + filePath );
        return false;
    }

    // First load the file as a VFS reference as usual
    CVFSHandlePtr fileReference = GetFile( filePath, "rb", false );
    if ( !fileReference || GUCEF_NULL == fileReference->GetAccess() )
        return false;
    
    // Now obtain the codec
    CORE::CDStoreCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry();
    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
    if ( codecRegistry.TryLookup( actualCodecToUse, codec, false ) && !codec.IsNULL() )
    {
        // Now pass the I/O access to the codec
        if ( codec->BuildDataTree( &destination, fileReference->GetAccess() ) ) 
        {
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Vfs:LoadFile: Could not obtain a DataNode codec for type \"" + actualCodecToUse + "\"" );
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Vfs:LoadFile: Could not obtain a DataNode codec for type \"" + actualCodecToUse + "\"" );
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MountArchive( const CString& archiveName  ,
                    const CString& archivePath  ,
                    const bool writeableRequest )
{GUCEF_TRACE;

    // Mount the archive using the archive path
    return MountArchive( archiveName      ,
                         archivePath      ,
                         archivePath      ,
                         writeableRequest );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MountArchive( const CString& archiveName  ,
                    const CString& archivePath  ,
                    const CString& mountPoint   ,
                    const bool writeableRequest )
{GUCEF_TRACE;

    // Use the archive file extension as the archive type
    CString archiveType( archivePath.SubstrToChar( '.', false ) );
    return MountArchive( archiveName      ,
                         archivePath      ,
                         archiveType      ,
                         mountPoint       ,
                         writeableRequest ,
                         false            );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MountArchive( const CString& archiveName              ,
                    const CString& archivePath              ,
                    const CString& archiveType              ,
                    const CString& mountPoint               ,
                    const bool writeableRequest             ,
                    const bool autoMountSubArchives         ,
                    const CString& actualNonVfsPathOverride )
{GUCEF_TRACE;

    CArchiveSettings settings;
    settings.SetActualArchivePath( actualNonVfsPathOverride );
    settings.SetArchiveName( archiveName );
    settings.SetArchivePath( archivePath );
    settings.SetArchiveType( archiveType );
    settings.SetAutoMountSubArchives( autoMountSubArchives );
    settings.SetMountPath( mountPoint );
    settings.SetWriteableRequested( writeableRequest );
    return MountArchive( settings );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::DelayMountArchive( const CArchiveSettings& settings )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    m_delayMountedArchiveSettings.push_back( settings );
    return true;
}

/*-------------------------------------------------------------------------*/

VFS::CString
CVFS::ConformVfsDirPath( const VFS::CString& originalDirPath ) const
{GUCEF_TRACE;

    // Regardless of how the settings are presented we want to ensure a consistent vfs path scheme
    // As such we enforce having a start '/' and a trailing '/' for dir paths

    if ( originalDirPath.IsNULLOrEmpty() )
        return "/";

    VFS::CString resultStr = originalDirPath.ReplaceChar( '\\', '/' );
    resultStr = resultStr.CompactRepeatingChar( '/' );

    if ( resultStr[ 0 ] != '/' )
        resultStr = '/' + resultStr;

    if ( resultStr[ resultStr.Length()-1 ] != '/' )
        resultStr += '/';

    return resultStr;
}

/*-------------------------------------------------------------------------*/

VFS::CString
CVFS::ConformVfsFilePath( const VFS::CString& originalFilePath ) const
{GUCEF_TRACE;

    // Regardless of how the settings are presented we want to ensure a consistent vfs path scheme
    // As such we enforce having a start '/' and NO trailing '/' for file paths

    if ( originalFilePath.IsNULLOrEmpty() )
        return CString::Empty;

    VFS::CString resultStr = originalFilePath.ReplaceChar( '\\', '/' );
    resultStr = resultStr.CompactRepeatingChar( '/' );

    if ( resultStr[ 0 ] != '/' )
        resultStr = '/' + resultStr;

    if ( resultStr[ resultStr.Length()-1 ] == '/' )
        resultStr = resultStr.CutChars( 1, false, 0 );

    return resultStr;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MountArchive( const CArchiveSettings& settings )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_rwdataLock );

    // Either determine the actual non-vfs path based on previously loaded/mounted archives or use the given explicit path if any
    CString actualArchivePath = CORE::ResolveVars( settings.GetActualArchivePath() );
    if ( !actualArchivePath.IsNULLOrEmpty() || GetActualFilePath( settings.GetArchivePath(), actualArchivePath ) )
    {
        // Found a compatible type,.. create an archive for the type
        TArchivePtr archive = m_abstractArchiveFactory.Create( settings.GetArchiveType() );
        if ( !archive.IsNULL() )
        {
            // Try to load from the resource
            CArchiveSettings updatedSettings( settings );
            if ( actualArchivePath != settings.GetActualArchivePath() )
                updatedSettings.SetActualArchivePath( actualArchivePath );

            // Regardless of how the settings are presented we want to ensure a conistent mounting scheme
            // As such we enforce having a start '/' and no trailing '/' for mount paths, with the start '/' signaling 'root'
            updatedSettings.SetMountPath( ConformVfsDirPath( updatedSettings.GetMountPath() ) );

            if ( archive->LoadArchive( updatedSettings ) )
            {
                // Successfully loaded/linked the archive
                // We will add it to our mount list
                TMountEntry archiveEntry;
                archiveEntry.abspath = actualArchivePath;

                //// Get the path where the archive is located relative to the root mount
                //archiveEntry.path = archiveEntry.abspath.CutChars( mountEntry.abspath.Length(), true );

                //// Git rid of any directory seperator prefix
                //if ( archiveEntry.path.Length() > 0 )
                //{
                //    if ( ( archiveEntry.path[ 0 ] == '\\' ) ||
                //            ( archiveEntry.path[ 0 ] == '/' )   )
                //    {
                //        archiveEntry.path = archiveEntry.path.CutChars( 1, true );
                //    }
                //}

                archiveEntry.path = actualArchivePath;
                archiveEntry.writeable = updatedSettings.GetWriteableRequested();
                archiveEntry.archive = archive;
                archiveEntry.mountPath = updatedSettings.GetMountPath();

                m_mountList.push_back( archiveEntry );
                TMountEntry* pushedMountEntry = &m_mountList.back();
                m_archivePtrToMountEntryLookup[ archive.GetPointerAlways() ] = pushedMountEntry;

                SubscribeTo( archive.GetPointerAlways() );

                TArchiveMountedEventData eData( archiveEntry.mountPath );
                NotifyObservers( ArchiveMountedEvent, &eData );
                return true;
            }
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::MountArchive( const CString& archiveName    ,
                    CVFSHandlePtr archiveResource ,
                    const CString& archiveType    ,
                    const CString& mountPoint     ,
                    const bool writeableRequest   )
{GUCEF_TRACE;

    if ( archiveResource.IsNULL() )
        return false;

    MT::CScopeWriterLock lock( m_rwdataLock );

    // create an archive for the type
    TArchivePtr archive = m_abstractArchiveFactory.Create( archiveType );
    if ( !archive.IsNULL() )
    {
        // Try to load from the resource
        if ( archive->LoadArchive( archiveName      ,
                                   archiveResource  ,
                                   writeableRequest ) )
        {
            // Successfully loaded/linked the archive
            // We will add it to our mount list
            TMountEntry archiveEntry;
            archiveEntry.path = archiveResource->GetFilename();
            archiveEntry.abspath = archiveResource->GetFilePath();
            archiveEntry.writeable = writeableRequest;
            archiveEntry.archive = archive;

            // Regardless of how the settings are presented we want to ensure a conistent mounting scheme
            // As such we enforce having a start '/' and no trailing '/' for mount paths, with the start '/' signaling 'root'
            archiveEntry.mountPath = ConformVfsDirPath( mountPoint );

            m_mountList.push_back( archiveEntry );
            TMountEntry* pushedMountEntry = &m_mountList.back();
            m_archivePtrToMountEntryLookup[ archive.GetPointerAlways() ] = pushedMountEntry;

            SubscribeTo( archive.GetPointerAlways() );

            TArchiveMountedEventData eData( archiveEntry.mountPath );
            NotifyObservers( ArchiveMountedEvent, &eData );
            return true;
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::IsMountedArchive( const CString& location ) const
{GUCEF_TRACE;

    // Keep in mind that mounted archives count as directories
    CString testLocation( ConformVfsDirPath( location ) );

    MT::CScopeReaderLock lock( m_rwdataLock );

    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& archiveEntry = (*i);
        if ( archiveEntry.mountPath == testLocation )
        {
            return true;
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CVFS::GetMountedArchiveList( const CString& location ,
                             const CString& filter   ,
                             const bool recursive    ,
                             TStringSet& outputList  ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_rwdataLock );

    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& archiveEntry = (*i);

        if ( archiveEntry.mountPath.Length() > 0 )
        {
            // Check if the location is a root for the mount path
            Int32 subSection = archiveEntry.mountPath.HasSubstr( location, true );
            if ( 0 == subSection )
            {
                // We now know the archive is mounted in either the root of "location"
                // or a sub directory
                if ( !recursive )
                {
                    // Check if we have multiple subdirs beyond the "location" to get to
                    // the archive. If so then we cannot add this archive because recursive
                    // searching is not allowed.
                    CString remainder = archiveEntry.mountPath.CutChars( subSection, true );
                    if ( remainder.Length() > 0 )
                    {
                        // make sure we are using a single directory seperator scheme
                        remainder = remainder.ReplaceChar( '\\', '/' );

                        // Dont count a starting directory seperator
                        if ( remainder[ 0 ] == '/' )
                        {
                            remainder = remainder.CutChars( 1, true );
                        }

                        Int32 dirDelimter = remainder.HasChar( '/', true );
                        if ( dirDelimter > -1 )
                        {
                            // We found a directory seperator so now me must check if it happens
                            // to be the last character
                            if ( remainder.Length() != dirDelimter+1 )
                            {
                                // The directory seperator was not the last character so we have multiple
                                // sub-dirs which is not allowed, we cannot add this item
                                ++i;
                                continue;
                            }
                        }
                    }
                }

                // Now we must validate the filename with the given filter
                CString filename = GUCEF::CORE::ExtractFilename( archiveEntry.mountPath );
                if ( FilterValidation( filename ,
                                       filter   ) )
                {
                    // Everything checks out, we have found a mounted archive that meets
                    // the criterea so we add it to the list
                    outputList.insert( archiveEntry.mountPath );
                }
            }
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CVFS::GetActualFilePath( const CString& file ,
                         CString& path       ) const
{GUCEF_TRACE;

    CString filePath = ConformVfsFilePath( file );
    
    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( filePath, false, mountLinks );

    // Find the file in the available archives
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
        {
            path = mountLink.mountEntry->abspath;
            CORE::AppendToPath( path, mountLink.remainder );
            return true;
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::GetVfsPathForAbsolutePath( const CString& absolutePath ,
                                 CString& relativePath       ) const
{GUCEF_TRACE;

    // Make sure there are no more variables in the given absolute path
    CString realAbsPath = CORE::RelativePath( absolutePath );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Try to match a mount entry
    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& mountEntry = (*i);
        Int32 subStrIndex = realAbsPath.HasSubstr( mountEntry.abspath, true );
        if ( subStrIndex == 0 )
        {
            // This mount entry has at least a partially matching path
            CString remainder = realAbsPath.CutChars( mountEntry.abspath.Length(), true );

            // Now we will try to locate a resource given that path so we can validate
            // that this is not a coincidence
            if ( mountEntry.archive->FileExists( remainder ) )
            {
                // We found a file given this remainer
                // Now we construct the full relative path including mount path
                relativePath = mountEntry.mountPath;
                CORE::AppendToPath( relativePath, remainder );
                return true;
            }
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CVFS::DirExists( const CString& dirPath ) const
{GUCEF_TRACE;

    CString path = ConformVfsDirPath( dirPath );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Search for a file and then get the hash
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->DirExists( mountLink.remainder ) )
        {
            return true;
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::FileExists( const CString& file ) const
{GUCEF_TRACE;

    CString path = ConformVfsFilePath( file );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Search for a file and then get the hash
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
        {
            return true;
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::IsFilePathSubOfDirPath( const CORE::CString& dirPath         ,
                              const CORE::CString& filePathToCheck ) const
{GUCEF_TRACE;

    CString conformedDirPath = ConformVfsDirPath( dirPath );
    CString conformedFilePath = ConformVfsFilePath( filePathToCheck );
    return 0 == conformedFilePath.HasSubstr( conformedDirPath, true );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::UnmountArchiveByName( const CString& archiveName )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_rwdataLock );

    TMountVector::iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        TMountEntry& mountEntry = (*i);
        if ( mountEntry.archive->GetArchiveName() == archiveName )
        {
            if ( mountEntry.archive->UnloadArchive() )
            {
                TArchiveUnmountedEventData eData( mountEntry.mountPath );                
                m_archivePtrToMountEntryLookup.erase( mountEntry.archive.GetPointerAlways() );
                m_mountList.erase( i );

                lock.EarlyUnlock();

                NotifyObservers( ArchiveUnmountedEvent, &eData );              
                return true;
            }
            return false;
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CVFS::AddRoot( const CORE::CString& rootpath                                ,
               const CString& archiveName                                   ,
               const bool writeable                                         ,
               const bool autoMountArchives /* = false */                   ,
               const CString& mountPoint /* = "" */                         ,
               const CString& archiveType /* = FileSystemArchiveTypeName */ )
{GUCEF_TRACE;

    CArchiveSettings settings;
    settings.SetArchiveName( archiveName );
    settings.SetActualArchivePath( rootpath );
    settings.SetWriteableRequested( writeable );
    settings.SetAutoMountSubArchives( autoMountArchives );
    settings.SetMountPath( mountPoint );
    settings.SetArchiveType( archiveType );

    MountArchive( settings );
}

/*-------------------------------------------------------------------------*/

void
CVFS::GetListOfSupportedArchiveTypes( TAbstractArchiveFactory::TKeySet& outList ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_rwdataLock );
    m_abstractArchiveFactory.ObtainKeySet( outList );
}

/*-------------------------------------------------------------------------*/

void
CVFS::SetMemloadSize( UInt32 bytesize )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_rwdataLock );

    _maxmemloadsize = bytesize;
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS maximum memory load size set to " + CORE::UInt32ToString( bytesize ) + " Bytes" );
}

/*-------------------------------------------------------------------------*/

UInt32
CVFS::GetMemloadSize( void ) const
{GUCEF_TRACE;

    return _maxmemloadsize;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_rwdataLock );

    CORE::CDataNode* n = tree.Structure( "GUCEF%VFS%CVFS" ,
                                         '%'              );
    n->SetAttribute( "maxmemload", _maxmemloadsize );

    n->DelSubTree();
    UInt32 count = (UInt32) m_mountList.size();
    CORE::CDataNode pathnode( "vfsroot" );
    for ( UInt32 i=0; i<count; ++i )
    {
        pathnode.SetAttribute( "path", m_mountList[ i ].path );
        if ( m_mountList[ i ].archive->IsWriteable() )
        {
            pathnode.SetAttribute( "writeable", "true" );
        }
        else
        {
            pathnode.SetAttribute( "writeable", "false" );
        }
        n->AddChild( pathnode );
    }

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS configuration successfully saved" );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::LoadConfig( const CORE::CDataNode& tree )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_rwdataLock );
    
    bool globalConfigLoadInProgress = IsGlobalConfigLoadInProgress();
    const CORE::CDataNode* n = tree.Search( "GUCEF%VFS%CVFS" ,
                                            '%'              ,
                                            false            );

    if ( GUCEF_NULL != n )
    {
        CORE::CVariant value = n->GetAttributeValueOrChildValueByName( "maxmemload" );
        if ( value.IsInitialized() )
        {
            SetMemloadSize( value.AsUInt32() );
        }

        CORE::CDataNode::TConstDataNodeSet rootNodeList = n->FindChildrenOfType( "VfsRoot" );
        CORE::CDataNode::TConstDataNodeSet::iterator i = rootNodeList.begin();
        while( i != rootNodeList.end() )
        {
            const CORE::CDataNode* legacySettingsNode = (*i);
            CArchiveSettings settings;
            settings.LoadConfig( *legacySettingsNode );
            if ( !globalConfigLoadInProgress )
                MountArchive( settings );
            else
                DelayMountArchive( settings );
            ++i;
        }

        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS legacy configuration load sequence completed" );
    }

    n = tree.Find( "VFS" );
    if ( n )
    {
        CORE::CDataNode::TConstDataNodeSet rootNodeList = n->FindChildrenOfType( "ArchiveSettings", false, false );
        CORE::CDataNode::TConstDataNodeSet::iterator i = rootNodeList.begin();
        while( i != rootNodeList.end() )
        {
            const CORE::CDataNode* settingsNode = (*i);
            CArchiveSettings settings;
            if ( settings.LoadConfig( *settingsNode ) )
            {
                if ( !globalConfigLoadInProgress )
                    MountArchive( settings );
                else
                    DelayMountArchive( settings );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS archive settings entry failed to load" );
            }
            ++i;
        }
        const CORE::CDataNode* listRoot = n->FindChild( "archives", false );
        if ( listRoot != GUCEF_NULL )
        {
            CORE::CDataNode::const_iterator node = listRoot->ConstBegin();
            while( node != n->ConstEnd() )
            {
                const CORE::CDataNode* settingsNode = (*node);

                CArchiveSettings settings;
                if ( settings.LoadConfig( *settingsNode ) )
                {
                    if ( !globalConfigLoadInProgress )
                        MountArchive( settings );
                    else
                        DelayMountArchive( settings );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VFS archive settings entry failed to load" );
                }
                ++node;
            }
        }
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VFS archive configuration load sequence completed" );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CVFS::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::VFS::CVFS";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::FilterValidation( const CORE::CString& filename ,
                        const CORE::CString& filter   )
{GUCEF_TRACE;

    if ( filter.Length() > 0 )
    {
        CVFS::TStringVector segs = filter.ParseElements( '*', false );
        VFS::Int32 lastSeg = 0;
        CVFS::TStringVector::iterator i = segs.begin();
        while ( i != segs.end() )
        {
            lastSeg = filename.HasSubstr( (*i), lastSeg, true );
            if ( 0 > lastSeg )
                return false;
            ++i;
        }
        return true;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void
CVFS::GetEligableMounts( const CString& location                ,
                         bool mustBeWritable                    ,
                         TConstMountLinkVector& mountLinkVector ) const
{GUCEF_TRACE;

    TMountVector::const_iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        const TMountEntry& mountEntry = (*i);

        // Exclude non-writeable archives right away for 'mustBeWritable' scenarios
        if ( !mustBeWritable || mustBeWritable == mountEntry.writeable )
        {
            if ( mountEntry.mountPath.Length() > 0 )
            {
                // Check to see if the mount path is the starting sub string of the location
                if ( location.HasSubstr( mountEntry.mountPath, true ) == 0 )
                {
                    // We need to make sure we dont accidentally match a partial directory name with a similarly names path
                    if ( location.Length() == mountEntry.mountPath.Length() ||
                       ( location[ mountEntry.mountPath.Length()-1 ] == '/' ) )
                    {
                        TConstMountLink mountLink;
                        mountLink.remainder = location.CutChars( mountEntry.mountPath.Length(), true );

                        // make sure the remainder does not have a path seperator prefix
                        if ( mountLink.remainder.Length() > 0 )
                        {
                            if ( ( mountLink.remainder[ 0 ] == '/' ) ||
                                 ( mountLink.remainder[ 0 ] == '\\' ) )
                            {
                                // remove the dir seperator prefix
                                mountLink.remainder = mountLink.remainder.CutChars( 1, true );
                            }
                        }

                        mountLink.mountEntry = &mountEntry;
                        mountLinkVector.push_back( mountLink );

                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VFS: Found eligable mount link using archive of type \"" +
                            mountEntry.archive->GetType() + "\" with remainder \"" + mountLink.remainder + "\". mustBeWritable=" + CORE::BoolToString( mustBeWritable ) );
                    }
                }
            }
            else
            {
                TConstMountLink mountLink;
                mountLink.remainder = location;
                mountLink.mountEntry = &mountEntry;
                mountLinkVector.push_back( mountLink );

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VFS: Found eligable mount link using archive of type \"" +
                    mountEntry.archive->GetType() + "\" with remainder \"" + location + "\". mustBeWritable=" + CORE::BoolToString( mustBeWritable ) );
            }
        }
        ++i;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VFS: Found a total of " + CORE::ToString( mountLinkVector.size() ) + " eligable mounts for location \"" + location + "\"" );
}

/*-------------------------------------------------------------------------*/

bool
CVFS::GetFileList( TStringVector& outputList      ,
                   const CString& location        , 
                   bool recursive                 ,
                   bool includePathInFilename     ,
                   const CString& nameFilter      ,
                   UInt32 maxListEntries          ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    CString path = ConformVfsDirPath( location );
    
    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Get a list from each mount
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        mountLink.mountEntry->archive->GetFileList( outputList                      ,
                                                    mountLink.mountEntry->mountPath ,
                                                    mountLink.remainder             ,
                                                    recursive                       ,
                                                    includePathInFilename           ,
                                                    nameFilter                      ,
                                                    maxListEntries                  );
        ++i;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::GetDirList( TStringVector& outputList  ,
                  const CString& location    ,
                  bool recursive             ,
                  bool includePathInFilename ,
                  const CString& nameFilter  ,
                  UInt32 maxListEntries      ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    CString path = ConformVfsDirPath( location );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Get a list from each mount
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        mountLink.mountEntry->archive->GetDirList( outputList                      ,
                                                   mountLink.mountEntry->mountPath ,
                                                   mountLink.remainder             ,
                                                   recursive                       ,
                                                   includePathInFilename           ,
                                                   nameFilter                      ,
                                                   maxListEntries                  );
        ++i;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

void
CVFS::GetFileList( CORE::CDataNode& outputDataTree        ,
                   const CORE::CString& location          ,
                   bool recursive  /* = false */          ,
                   const CORE::CString& filter /* = "" */ ,
                   const bool addHash /* = false */       ,
                   UInt32 maxListEntries                  ) const
{GUCEF_TRACE;

    // First we get a list of files and their path
    TStringVector outputList;
    GetFileList( outputList      ,
                 location        ,
                 recursive       ,
                 true            ,
                 filter          ,
                 maxListEntries  );

    // First we clear the tree
    outputDataTree.Clear();

    // Now we build our data tree using the list we obtained
    TStringVector::iterator i = outputList.begin();
    while ( i != outputList.end() )
    {
        // First we build the path structure in the data tree
        CString path = (*i).SubstrToChar( '/', false );
        CORE::CDataNode* pathRootNode = outputDataTree.Structure( "DIR", "Name", path, '/' );

        // Extract the filename from the path
        CORE::CString filename( (*i).C_String() + path.Length() ,
                                (*i).Length() - path.Length()   );

        // Create the file node and set all the attributes
        CORE::CDataNode fileNode( "FILE" );
        fileNode.SetAttribute( "Name", filename );
        fileNode.SetAttribute( "Size", CORE::ToString( GetFileSize( (*i) ) ) );

        if ( addHash )
        {
            // Obtain the hash for the file, this can be a lengthy operation
            fileNode.SetAttribute( "Hash", GetFileHash( (*i) ) );
        }

        // Create a copy child node thus storing the file properties in the tree
        pathRootNode->AddChild( fileNode );

        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CVFS::GetFileMetaData( const CString& filename           ,
                       CORE::CResourceMetaData& metaData ) const
{GUCEF_TRACE;

    CString path = ConformVfsFilePath( filename );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Search for a file and then get the meta data
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->GetFileMetaData( mountLink.remainder, metaData ) )
        {
            if ( metaData.resourceExists )
                return true;
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime
CVFS::GetFileModificationTime( const CString& filename ) const
{GUCEF_TRACE;

    CString path = ConformVfsFilePath( filename );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Search for a file and then get the hash
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
        {
            CORE::CDateTime modificationTime = mountLink.mountEntry->archive->GetFileModificationTime( mountLink.remainder );
            return modificationTime;
        }
        ++i;
    }

    return CORE::CDateTime();
}

/*-------------------------------------------------------------------------*/

CORE::CString
CVFS::GetFileHash( const CORE::CString& file ) const
{GUCEF_TRACE;

    CString path = ConformVfsFilePath( file );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Search for a file and then get the hash
    CString hash;
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
        {
            CString hash = mountLink.mountEntry->archive->GetFileHash( mountLink.remainder );
            return hash;
        }
        ++i;
    }

    return CString();
}

/*-------------------------------------------------------------------------*/

UInt64
CVFS::GetFileSize( const CORE::CString& file ) const
{GUCEF_TRACE;

    CString path = ConformVfsFilePath( file );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Search for a file and then get its size
    CString hash;
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        if ( mountLink.mountEntry->archive->FileExists( mountLink.remainder ) )
        {
            UInt64 fileSize = mountLink.mountEntry->archive->GetFileSize( mountLink.remainder );
            return fileSize;
        }
        ++i;
    }

    return 0;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::AddDirToWatch( const CString& dirToWatch       ,
                     const CDirWatchOptions& options )
{GUCEF_TRACE;

    CString path = ConformVfsFilePath( dirToWatch );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Add the path as monitored for all relevant mounted archives
    bool totalSuccess = true;
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        TArchivePtr archiveToWatch = mountLink.mountEntry->archive;

        if ( !archiveToWatch->AddDirToWatch( mountLink.remainder, options ) )
            totalSuccess = false;
        ++i;
    }
    
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::RemoveDirToWatch( const CString& dirToWatch )
{GUCEF_TRACE;

    CString path = ConformVfsFilePath( dirToWatch );

    MT::CScopeReaderLock lock( m_rwdataLock );

    // Get a list of all eligable mounts
    TConstMountLinkVector mountLinks;
    GetEligableMounts( path, false, mountLinks );

    // Remove the path as monitored for all relevant mounted archives
    bool totalSuccess = true;
    TConstMountLinkVector::iterator i = mountLinks.begin();
    while ( i != mountLinks.end() )
    {
        TConstMountLink& mountLink = (*i);
        TArchivePtr archiveToWatch = mountLink.mountEntry->archive;

        if ( !archiveToWatch->RemoveDirToWatch( mountLink.remainder ) )
            totalSuccess = false;
        ++i;
    }
    
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CVFS::RemoveAllWatches( void )
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_rwdataLock );
    
    bool totalSuccess = true;
    TMountVector::iterator i = m_mountList.begin();
    while ( i != m_mountList.end() )
    {
        TMountEntry& mountEntry = (*i);
        if ( !mountEntry.archive->RemoveAllWatches() )
            totalSuccess = false;
        ++i;
    }
    
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

void
CVFS::RegisterArchiveFactory( const CString& archiveType      ,
                              TArchiveFactory& archiveFactory )
{GUCEF_TRACE;

    m_abstractArchiveFactory.RegisterConcreteFactory( archiveType     ,
                                                      &archiveFactory );

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Registered VFS archive factory for type \"" + archiveType + "\"" );
}

/*-------------------------------------------------------------------------*/

void
CVFS::UnregisterArchiveFactory( const CString& archiveType )
{GUCEF_TRACE;

    m_abstractArchiveFactory.UnregisterConcreteFactory( archiveType );

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Unregistered VFS archive factory for type \"" + archiveType + "\"" );
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CVFS::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockStatus( m_rwdataLock.WriterStart( lockWaitTimeoutInMs ) );
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CVFS::Unlock( void ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockStatus( m_rwdataLock.WriterStop() );
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CVFS::ReadOnlyLock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockStatus( m_rwdataLock.ReaderStart( lockWaitTimeoutInMs ) );
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CVFS::ReadOnlyUnlock( void ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockStatus( m_rwdataLock.ReaderStop() );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
