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

#ifndef GUCEF_VFS_CARCHIVE_H
#define GUCEF_VFS_CARCHIVE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include <set>

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CTBASICSHAREDPTR_H
#include "CTBasicSharedPtr.h"
#define GUCEF_CORE_CTBASICSHAREDPTR_H
#endif /* GUCEF_CORE_CTBASICSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CDATETIME_H
#include "gucefCORE_CDateTime.h"
#define GUCEF_CORE_CDATETIME_H
#endif /* GUCEF_CORE_CDATETIME_H ? */

#ifndef GUCEF_CORE_CIDIRECTORYWATCHER_H
#include "gucefCORE_CIDirectoryWatcher.h"
#define GUCEF_CORE_CIDIRECTORYWATCHER_H
#endif /* GUCEF_CORE_CIDIRECTORYWATCHER_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CRESOURCEMETADATA_H
#include "gucefCORE_CResourceMetaData.h"
#define GUCEF_CORE_CRESOURCEMETADATA_H
#endif /* GUCEF_CORE_CRESOURCEMETADATA_H ? */

#ifndef GUCEF_VFS_CVFSHANDLE_H
#include "gucefVFS_CVFSHandle.h"     /* handle for VFS ref counted recources */
#define GUCEF_VFS_CVFSHANDLE_H
#endif /* GUCEF_VFS_CVFSHANDLE_H ? */

#ifndef GUCEF_VFS_CARCHIVESETTINGS_H
#include "gucefVFS_CArchiveSettings.h"
#define GUCEF_VFS_CARCHIVESETTINGS_H
#endif /* GUCEF_VFS_CARCHIVESETTINGS_H ? */

#undef MoveFile

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Base class abstraction for all types of archives that can be mounted into the VFS
 *  An archive is simply a type of file system container which may or may not support
 *  all the features of the local O/S filesystem and it may or may not be abstracting away
 *  complexity that would normally prevent the "archive" from otherwise being thought of 
 *  and used as a local filesystem.
 *  One possible example is a remote archive like cloud storage.
 *  Another example is a zip archive, another is a ISO disc image.
 */
class GUCEF_VFS_PUBLIC_CPP CArchive : public CORE::CObservingNotifier                    ,
                                      public CORE::CIDirectoryWatcher                    ,
                                      public CORE::CTDynamicDestructorBase< CVFSHandle >
{
    public:
    
    typedef CString::StringVector                               TStringVector;
    typedef CString::StringSet                                  TStringSet;
    
    CArchive( void );
    
    CArchive( const CArchive& src );
    
    virtual ~CArchive();
    
    CArchive& operator=( const CArchive& src );
    
    virtual TBasicVfsResourcePtr GetFile( const CString& file          ,
                                          const char* mode = "rb"      ,
                                          const UInt32 memLoadSize = 0 ,
                                          const bool overwrite = false ) = 0;

    /**
     *  Default implementation always fails.
     *  Derived classes should override this method to provide the functionality if feasible.
     *  Recommended implementation is to apply the meta-data 'best effort' based on the constraints of the actual underlying storage
     *  and to return true if within the constraints the meta-data was applied successfully.
     *  Aside from the metadata handling the bahaviour of this member function should be the same as GetFile().
     */
    virtual TBasicVfsResourcePtr GetFileAs( const CString& file                     ,
                                            const CORE::CResourceMetaData& metaData ,
                                            const char* mode = "wb"                 ,
                                            const UInt32 memLoadSize = 0            ,
                                            const bool overwrite = false            );

    virtual bool StoreAsFile( const CORE::CString& filepath    ,
                              const CORE::CDynamicBuffer& data ,
                              const CORE::UInt64 offset        ,
                              const bool overwrite             ) = 0;

    virtual void GetFileList( TStringVector& outputList                                       ,
                              const CString& mountLocation                                    , 
                              const CString& archiveLocation                                  ,
                              bool recursive = false                                          ,
                              bool includePathInFilename = false                              ,
                              const CString::StringSet& nameFilters = CString::EmptyStringSet ,
                              UInt32 maxListEntries = GUCEF_UINT16MAX                         ) const = 0;

    virtual void GetDirList( TStringVector& outputList                                       ,
                             const CString& mountLocation                                    , 
                             const CString& archiveLocation                                  ,
                             bool recursive = false                                          ,
                             bool includeParentDirInName = false                             ,
                             const CString::StringSet& nameFilters = CString::EmptyStringSet ,
                             UInt32 maxListEntries = GUCEF_UINT16MAX                         ) const = 0;
    
    virtual bool DeleteFile( const CString& filePath ) = 0;
    
    virtual bool MoveFile( const CString& oldFilePath ,
                           const CString& newFilePath ,
                           const bool overwrite       ) = 0;

    virtual bool FileExists( const CString& filePath ) const = 0;

    virtual bool DirExists( const CString& dirPath ) const = 0;
    
    virtual UInt64 GetFileSize( const CString& filePath ) const = 0;

    /**
     *  Default implementation always fails.
     *  Derived classes should override this method to provide the functionality if feasible.
     *  Recommended implementation is to apply the meta-data 'best effort' based on the constraints of the actual underlying storage
     *  and to return true if within the constraints the meta-data was applied successfully.
     */
    virtual bool SetFileMetaData( const CString& filePath                 ,
                                  const CORE::CResourceMetaData& metaData );

    virtual bool GetFileMetaData( const CString& filePath           ,
                                  CORE::CResourceMetaData& metaData ) const = 0;
    
    virtual CString GetFileHash( const CString& file ) const = 0;
    
    virtual CORE::CDateTime GetFileModificationTime( const CString& filePath ) const = 0;
    
    virtual const CString& GetArchiveName( void ) const = 0;
    
    virtual bool IsWriteable( void ) const = 0;
    
    virtual bool LoadArchive( const CArchiveSettings& settings ) = 0;

    virtual bool LoadArchive( const CString& archiveName       ,
                              TBasicVfsResourcePtr vfsResource ,
                              const bool writeableRequest      ) = 0;
                              
    virtual bool UnloadArchive( void ) = 0;
    
    virtual const CString& GetType( void ) const = 0;

    virtual bool IsDirectoryWatchingSupported( void ) const;

    /**
     *  Configures the archive to emit DirectoryWatcher events if the given condition occurs
     * 
     *  Note that it not a mandatory feature for archives to support dir waitching and the default implementation
     *  will always return false to the request.
     */
    virtual bool AddDirToWatch( const CString& dirToWatch       ,
                                const CDirWatchOptions& options ,
                                bool tryToCreatePathIfNotExists ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool RemoveDirToWatch( const CString& dirToWatch ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool RemoveAllWatches( void ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Note that it not a mandatory feature for archives to support dir waitching and the default implementation
     *  will always return false to the request.
     */
    virtual bool GetAllWatchedDirs( CString::StringSet& dirs ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsHealthy( void ) const;

    virtual bool IsConnected( void ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_VFS_CARCHIVE_H ? */
