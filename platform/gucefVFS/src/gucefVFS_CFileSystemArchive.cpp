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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#include "gucefVFS_CFileSystemArchive.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CFileSystemArchive::CFileSystemArchive( void )
    : m_diskCacheList()            
    , m_rootDir()         
    , m_archiveName() 
    , m_writable( false )
    , m_fsWatcher()
{GUCEF_TRACE;

    SubscribeTo( &m_fsWatcher );
}

/*-------------------------------------------------------------------------*/

CFileSystemArchive::CFileSystemArchive( const CString& archiveName ,
                                        const CString& rootDir     ,
                                        bool writeable             )
    : m_diskCacheList()            
    , m_rootDir( rootDir )         
    , m_archiveName( archiveName ) 
    , m_writable( writeable )
    , m_fsWatcher()
{GUCEF_TRACE;

    SubscribeTo( &m_fsWatcher );
}

/*-------------------------------------------------------------------------*/

CFileSystemArchive::~CFileSystemArchive()
{GUCEF_TRACE;

    m_diskCacheList.clear();
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::LoadArchive( const CArchiveSettings& settings )
{GUCEF_TRACE;

    m_archiveName = settings.GetArchiveName();
    m_rootDir = settings.GetActualArchivePath();
    m_writable = settings.GetWriteableRequested();

    if ( settings.GetAutoMountSubArchives() )
    {
        CVFS& vfs = CVfsGlobal::Instance()->GetVfs();
        
        // Get a list of all constructable archive types
        CVFS::TAbstractArchiveFactory::TKeySet keySet;
        vfs.GetListOfSupportedArchiveTypes( keySet );

        // Get a list of all files from the root onward
        TStringVector files;
        GetFileList( files, settings.GetMountPath(), CString::Empty, true, true, CString::Empty );

        // Find mountable types
        TStringVector::iterator i = files.begin();
        while ( i != files.end() )
        {
            // Mount based on archive file extention type
            CString fileExtention = CORE::ExtractFileExtention( (*i) );
            if ( !fileExtention.IsNULLOrEmpty() )
            {
                CVFS::TAbstractArchiveFactory::TKeySet::iterator n = keySet.find( fileExtention );
                if ( n != keySet.end() )
                {
                    // Found a compatible type,.. create an archive for the type
                    if ( vfs.MountArchive( (*i)                                                                                , 
                                           (*i)                                                                                , 
                                           (*n)                                                                                , 
                                           (*i)                                                                                , 
                                           settings.GetWriteableRequested()                                                    , 
                                           settings.GetAutoMountSubArchives() && settings.GetAutoMountSubArchivesIsRecursive() , 
                                           (*i)                                                                                ) )
                    {
                        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "FileSystemArchive: Auto mounted sub archive with name " + (*i) );
                    }
                }
            }
            ++i;
        }

    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CFileSystemArchive::LoadArchive( const VFS::CString& archiveName ,
                                 CVFSHandlePtr vfsResource       ,
                                 const bool writeableRequest     )
{GUCEF_TRACE;

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "FileSystemArchive: Attempting to load a \"FileSystem\" archive from a file. That does not make sense. Bad config?" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::UnloadArchive( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

CArchive::CVFSHandlePtr
CFileSystemArchive::GetFile( const CString& file      ,
                             const char* mode         ,
                             const UInt32 memLoadSize ,
                             const bool overwrite     )

{GUCEF_TRACE;

    /*
     *      Try to locate the file using the rootdirs as
     *      a prefix
     */
    CVFSHandle* fh = LoadFromDisk( file        ,
                                   mode        ,
                                   memLoadSize ,
                                   overwrite   );

    if ( fh != GUCEF_NULL )
    {
        return CVFSHandlePtr( fh, this );
    }

    /*
     *      If we get here then the file was not found in one
     *      of the rootdirs. or in any of the packfiles
     */
    return CVFSHandlePtr();
}

/*-------------------------------------------------------------------------*/

bool 
CFileSystemArchive::DeleteFile( const CString& filePath )
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, filePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );    
    if ( CORE::FileExists( path ) )
    {
        return CORE::DeleteFile( path );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::MoveFile( const CString& oldFilePath ,
                              const CString& newFilePath ,
                              const bool overwrite       ) 
{GUCEF_TRACE;

    CString oldPath = CORE::CombinePath( m_rootDir, oldFilePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    CString newPath = CORE::CombinePath( m_rootDir, newFilePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    
    CString newPathDir = CORE::StripFilename( newPath );
    CORE::CreateDirs( newPathDir );

    return CORE::MoveFile( oldPath, newPath, overwrite );
}

/*-------------------------------------------------------------------------*/

bool 
CFileSystemArchive::StoreAsFile( const CORE::CString& filePath    ,
                                 const CORE::CDynamicBuffer& data ,
                                 const CORE::UInt64 offset        ,
                                 const bool overwrite             )
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, filePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    CString dir = CORE::StripFilename( path );
    CORE::CreateDirs( dir );
    return data.WriteContentToFile( path, offset, overwrite );    
}

/*-------------------------------------------------------------------------*/

void
CFileSystemArchive::GetListFromRoot( const CORE::CString& actualFsDir  ,
                                     const CString& vfsMountLocation   , 
                                     const CString& vfsArchiveLocation ,
                                     bool recursive                    ,
                                     bool includePathInFilename        ,
                                     const CString& filter             ,
                                     TStringVector& outputList         ,
                                     bool addFiles                     ,
                                     bool addDirs                      ) const
{GUCEF_TRACE;

    CORE::CString vfsPath = CORE::CombinePath( vfsMountLocation, vfsArchiveLocation );

    CORE::CString filename;
    struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( actualFsDir.C_String() );
    if ( did != NULL )
    {
        /*
         *      Iterate the dir content adding the file entries from
         *      the dir to the list
         */
        do
        {
            if ( CORE::DI_Is_It_A_File( did ) != 0 )
            {
                if ( addFiles )
                {
                    filename = CORE::DI_Name( did );
                    if ( filename != '.' && filename != ".." )
                    {
                        if ( CVFS::FilterValidation( filename ,
                                                     filter   ) )
                        {
                            if ( !includePathInFilename )
                            {
                                outputList.push_back( filename );
                            }
                            else
                            {
                                CORE::CString vfsFilePath = CORE::CombinePath( vfsPath, filename );
                                vfsFilePath = vfsFilePath.ReplaceChar( '\\', '/' );
                                outputList.push_back( vfsFilePath );
                            }
                        }
                    }
                }
            }
            else
            {
                CORE::CString dirName = CORE::DI_Name( did );
                if ( dirName != '.' && dirName != ".." )
                {
                    if ( addDirs )
                    {
                        outputList.push_back( dirName + '/' );
                    }

                    if ( recursive )
                    {

                        CORE::CString fsSubdir = CORE::CombinePath( actualFsDir, dirName );
                        CORE::CString vfsSubdir = CORE::CombinePath( vfsArchiveLocation, filename );

                        // Recursively process the sub-dir
                        GetListFromRoot( fsSubdir              ,
                                         vfsMountLocation      ,
                                         vfsSubdir             ,
                                         recursive             ,
                                         includePathInFilename ,
                                         filter                ,
                                         outputList            ,
                                         true                  ,
                                         false                 );
                    }
                }
            }
        }
        while ( CORE::DI_Next_Dir_Entry( did ) );

        CORE::DI_Cleanup( did );
    }
}

/*-------------------------------------------------------------------------*/

void
CFileSystemArchive::GetFileList( TStringVector& outputList      ,
                                 const CString& mountLocation   , 
                                 const CString& archiveLocation ,
                                 bool recursive                 ,
                                 bool includePathInFilename     ,
                                 const CString& nameFilter      ,
                                 UInt32 maxListEntries          ) const
{GUCEF_TRACE;

    // Translate to a local filesystem path
    CString actualFsRootdir = CombinePath( m_rootDir, archiveLocation.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );

    /*
     *      Process the root
     */
    GetListFromRoot( actualFsRootdir       ,
                     mountLocation         ,
                     archiveLocation       ,
                     recursive             ,
                     includePathInFilename ,
                     nameFilter            ,
                     outputList            ,
                     true                  ,
                     false                 );
}

/*-------------------------------------------------------------------------*/

void
CFileSystemArchive::GetDirList( TStringVector& outputList      ,
                                const CString& mountLocation   , 
                                const CString& archiveLocation ,
                                bool recursive                 ,
                                bool includePathInFilename     ,
                                const CString& nameFilter      ,
                                UInt32 maxListEntries          ) const
{GUCEF_TRACE;

    // Translate to a local filesystem path
    CString actualFsRootdir = CombinePath( m_rootDir, archiveLocation.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );

    /*
     *      Process the root
     */
    GetListFromRoot( actualFsRootdir       ,
                     mountLocation         ,
                     archiveLocation       ,
                     recursive             ,
                     includePathInFilename ,
                     nameFilter            ,
                     outputList            ,
                     false                 ,
                     true                  );
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::FileExists( const CString& filePath ) const
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, filePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    return CORE::FileExists( path );
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::DirExists( const CString& dirPath ) const
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, dirPath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    return CORE::DirExists( path );
}

/*-------------------------------------------------------------------------*/

UInt32
CFileSystemArchive::GetFileSize( const CString& filePath ) const
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, filePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    if ( CORE::FileExists( path ) )
    {
        return CORE::FileSize( path );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CString
CFileSystemArchive::GetFileHash( const CString& filePath ) const
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, filePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    if ( CORE::FileExists( path ) )
    {
        CORE::CFileAccess fileAccess( path );

        UInt8 md5Hash[ 16 ];
        if ( CORE::md5fromfile( fileAccess.CStyleAccess() ,
                                md5Hash                   ) == 1 )
        {
            return CORE::MD5ToString( md5Hash );
        }
    }

    return CString();
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime
CFileSystemArchive::GetFileModificationTime( const CString& filePath ) const
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, filePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    if ( CORE::FileExists( path ) )
    {
        return CORE::GetFileModificationTime( path );
    }
    return CORE::CDateTime();
}

/*-------------------------------------------------------------------------*/

const CString&
CFileSystemArchive::GetArchiveName( void ) const
{GUCEF_TRACE;

    return m_archiveName;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::IsWriteable( void ) const
{GUCEF_TRACE;

    return m_writable;
}

/*-------------------------------------------------------------------------*/

const CString& 
CFileSystemArchive::GetType( void ) const
{GUCEF_TRACE;

    return CVFS::FileSystemArchiveTypeName;
}

/*-------------------------------------------------------------------------*/

CVFSHandle*
CFileSystemArchive::LoadFromDisk( const CString& filePath  ,
                                  const char* mode         ,
                                  const UInt32 memLoadSize ,
                                  const bool overwrite     )
{GUCEF_TRACE;

    // Create a file path for this root
    CString path = CORE::CombinePath( m_rootDir, filePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );

    bool needwriteable( ( strchr( mode, 'a' ) != NULL ) || ( strchr( mode, 'w' ) != NULL ) || ( strchr( mode, '+' ) != NULL ));

    // Check if we can perform read-only access which allows us
    // to share the resource
    if ( ( strcmp( mode, "rb" ) == 0 ) ||
         ( strcmp( mode, "r" ) == 0 )   )
    {
        // Check our cache for this file
        TFileMemCache::iterator n = m_diskCacheList.find( path );
        if ( n != m_diskCacheList.end() )
        {
            // We found the file in our cache, we will link to the existing buffer.
            TDynamicBufferPtr bufferPtr = (*n).second;
            return new CVFSHandle( new CORE::CDynamicBufferAccess( bufferPtr.GetPointer() ,
                                                                   false                  ) ,
                                   path                                                     ,
                                   filePath                                                 ,
                                   bufferPtr                                                );
        }
    }

    // Check if we can proceed under these circumstances
    bool exists( CORE::FileExists( path ) );
    if ( !exists && needwriteable )
    {
        CORE::CString dirPath = CORE::StripFilename( path );
        if ( !CORE::CreateDirs( dirPath ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "FileSystemArchive:LoadFromDisk: Unable to create directories to store new file: " + dirPath );
            return GUCEF_NULL;
        }
    }
    if ( ( exists && overwrite && needwriteable ) ||
         ( !exists && needwriteable )             ||
         ( exists && !needwriteable )              )
    {
        // Attempt to get access to the file
        CORE::CIOAccess* fa = new CORE::CFileAccess( path, mode );
        if ( !fa->IsValid() )
        {
            // try a different root
            delete fa;
            return NULL;
        }

        if ( ( strcmp( mode, "rb" ) == 0 ) ||
             ( strcmp( mode, "r" ) == 0 )   )
        {
            // check if we can load the file into memory
            UInt32 fsize = CORE::Filesize( path.C_String() );
            if ( fsize <= memLoadSize )
            {
                // Create the memory buffer
                TDynamicBufferPtr bufferPtr( new CORE::CDynamicBuffer() );
                CORE::CIOAccess* bufferAccess = new CORE::CDynamicBufferAccess( bufferPtr.GetPointer(), false );

                // Copy the file into the buffer
                if ( fsize == bufferAccess->Write( *fa ) )
                {
                    delete fa;
                    fa = NULL;

                    // reset the carat so the user can access the file from the beginning
                    bufferAccess->Setpos( 0 );

                    // Add the file to our buffered files list
                    m_diskCacheList.insert( std::pair< CORE::CString, TDynamicBufferPtr >( path, bufferPtr ) );

                    // return the file handle
                    return new CVFSHandle( bufferAccess ,
                                           path         ,
                                           filePath     ,
                                           bufferPtr    );
                }
                else
                {
                    // Something went wrong while trying to load the file into the buffer
                    // try a different root
                    fa->Setpos( 0 );
                    delete bufferAccess;
                }
            }
        }

        // return the file handle
        return new CVFSHandle( fa       ,
                               path     ,
                               filePath );

    }

    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CFileSystemArchive::DestroyObject( CVFSHandle* vfshandle )
{GUCEF_TRACE;

    if ( vfshandle != NULL )
    {
        delete vfshandle->GetAccess();

        if ( vfshandle->IsLoadedInMemory() )
        {
            TFileMemCache::iterator n = m_diskCacheList.find( vfshandle->GetFilePath() );
            if ( n != m_diskCacheList.end() )
            {
                // We found the file in our cache, we will link to the existing buffer.
                if ( (*n).second.GetReferenceCount() == 1 )
                {
                    // nobody else is using the memory buffer anymore
                    m_diskCacheList.erase( n );
                }
            }
        }

        delete vfshandle;
    }
}

/*-------------------------------------------------------------------------*/

const CString&
CFileSystemArchive::GetRootDir( void ) const
{GUCEF_TRACE;

    return m_rootDir;
}

/*-------------------------------------------------------------------------*/

bool 
CFileSystemArchive::AddDirToWatch( const CString& dirToWatch       ,
                                   const CDirWatchOptions& options )
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, dirToWatch.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    return m_fsWatcher.AddDirToWatch( path, options );
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::RemoveDirToWatch( const CString& dirToWatch )
{GUCEF_TRACE;

    CString path = CORE::CombinePath( m_rootDir, dirToWatch.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR ) );
    return m_fsWatcher.RemoveDirToWatch( path );
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::RemoveAllWatches( void )
{GUCEF_TRACE;

    return m_fsWatcher.RemoveAllWatches();
}

/*-------------------------------------------------------------------------*/

bool 
CFileSystemArchive::IsDirectoryWatchingSupported( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

CORE::CString 
CFileSystemArchive::GetVfsPathForFileSystemPath( const CORE::CString& fsPath )
{GUCEF_TRACE;

    return fsPath.CutChars( m_rootDir.Length(), true, 0 );
}

/*-------------------------------------------------------------------------*/

void
CFileSystemArchive::OnDirectoryWatcherEvent( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventid  ,
                                             CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // Archives are also DirectoryWatcher's so we pretend the event msg came from us
    // we do need to translate the paths first

    if ( IsDirectoryWatcherDirEvent( eventid ) )
    {
        if ( eventid == CORE::CDirectoryWatcherEvents::DirRenamedEvent )
        {
            CORE::CDirectoryWatcherEvents::TDirRenamedEventData* dirRenameInfo = static_cast< CORE::CDirectoryWatcherEvents::TDirRenamedEventData* >( eventdata );
            if ( GUCEF_NULL != dirRenameInfo )
            {
                CORE::CDirectoryWatcherEvents::TDirRenamedEventData vfsAdjusted;
                vfsAdjusted.GetData().newDirName = GetVfsPathForFileSystemPath( dirRenameInfo->GetData().newDirName );
                vfsAdjusted.GetData().oldDirName = GetVfsPathForFileSystemPath( dirRenameInfo->GetData().oldDirName );
                NotifyObservers( eventid, &vfsAdjusted );
            }              
        }
        else
        {
            CORE::TCloneableString* dirPath = static_cast< CORE::TCloneableString* >( eventdata );
            if ( GUCEF_NULL != dirPath )
            {
                CORE::TCloneableString vfsPath( GetVfsPathForFileSystemPath( *dirPath ) );
                NotifyObservers( eventid, &vfsPath );
            }
        }
    }
    else
    if ( IsDirectoryWatcherFileEvent( eventid ) )
    {
        if ( eventid == CORE::CDirectoryWatcherEvents::FileRenamedEvent )
        {
            CORE::CDirectoryWatcherEvents::TFileRenamedEventData* fileRenameInfo = static_cast< CORE::CDirectoryWatcherEvents::TFileRenamedEventData* >( eventdata );
            if ( GUCEF_NULL != fileRenameInfo )
            {
                CORE::CDirectoryWatcherEvents::TFileRenamedEventData vfsAdjusted;
                vfsAdjusted.GetData().newFilename = GetVfsPathForFileSystemPath( fileRenameInfo->GetData().newFilename );
                vfsAdjusted.GetData().oldFilename = GetVfsPathForFileSystemPath( fileRenameInfo->GetData().oldFilename );
                NotifyObservers( eventid, &vfsAdjusted );
            }
        }
        else
        {
            CORE::TCloneableString* filePath = static_cast< CORE::TCloneableString* >( eventdata );
            if ( GUCEF_NULL != filePath )
            {
                CORE::TCloneableString vfsPath( GetVfsPathForFileSystemPath( *filePath ) );
                NotifyObservers( eventid, &vfsPath );
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
CFileSystemArchive::OnNotify( CORE::CNotifier* notifier    ,
                              const CORE::CEvent& eventId  ,
                              CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // For anything from the file system directory watcher we are merely
    // activing as a proxy to hide we are just reusing it
    if ( notifier == &m_fsWatcher )
    {
        // Archives are also DirectoryWatcher's so we pretend the event msg came from us
        // we do need to translate the paths first
        OnDirectoryWatcherEvent( notifier, eventId, eventdata );
    }
}

/*-------------------------------------------------------------------------*/

bool 
CFileSystemArchive::IsHealthy( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CFileSystemArchive::IsConnected( void ) const
{GUCEF_TRACE;

    // try to interact with the file system and verify the root path exists
    return CORE::DirExists( m_rootDir );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
