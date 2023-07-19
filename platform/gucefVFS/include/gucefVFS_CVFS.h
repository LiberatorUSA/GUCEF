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

#ifndef GUCEF_VFS_CVFS_H
#define GUCEF_VFS_CVFS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_MT_CREADWRITELOCK_H
#include "gucefMT_CReadWriteLock.h"
#define GUCEF_MT_CREADWRITELOCK_H
#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

#ifndef GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#include "gucefCORE_CGloballyConfigurable.h"
#define GUCEF_CORE_CGLOBALLYCONFIGURABLE_H
#endif /* GUCEF_CORE_CGLOBALLYCONFIGURABLE_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#include "CTSGNotifier.h"
#define GUCEF_CORE_CTSGNOTIFIER_H
#endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTABSTRACTFACTORY_H
#include "CTAbstractFactory.h"
#define GUCEF_CORE_CTABSTRACTFACTORY_H
#endif /* GUCEF_CORE_CTABSTRACTFACTORY_H ? */

#ifndef GUCEF_CORE_CDATETIME_H
#include "gucefCORE_CDateTime.h"
#define GUCEF_CORE_CDATETIME_H
#endif /* GUCEF_CORE_CDATETIME_H ? */

#ifndef GUCEF_CORE_CIDIRECTORYWATCHER_H
#include "gucefCORE_CIDirectoryWatcher.h"
#define GUCEF_CORE_CIDIRECTORYWATCHER_H
#endif /* GUCEF_CORE_CIDIRECTORYWATCHER_H ? */

#ifndef GUCEF_CORE_CRESOURCEMETADATA_H
#include "gucefCORE_CResourceMetaData.h"
#define GUCEF_CORE_CRESOURCEMETADATA_H
#endif /* GUCEF_CORE_CRESOURCEMETADATA_H ? */

#ifndef GUCEF_VFS_CARCHIVE_H
#include "gucefVFS_CArchive.h"
#define GUCEF_VFS_CARCHIVE_H
#endif /* GUCEF_VFS_CARCHIVE_H ? */

#ifndef GUCEF_VFS_CARCHIVESETTINGS_H
#include "gucefVFS_CArchiveSettings.h"
#define GUCEF_VFS_CARCHIVESETTINGS_H
#endif /* GUCEF_VFS_CARCHIVESETTINGS_H ? */

#ifndef GUCEF_VFS_CFILESYSTEMARCHIVE_H
#include "gucefVFS_CFileSystemArchive.h"
#define GUCEF_VFS_CFILESYSTEMARCHIVE_H
#endif /* GUCEF_VFS_CFILESYSTEMARCHIVE_H ? */

#ifndef GUCEF_VFS_ASYNCVFSTASKDATA_H
#include "gucefVFS_AsyncVfsTaskData.h"
#define GUCEF_VFS_ASYNCVFSTASKDATA_H
#endif /* GUCEF_VFS_ASYNCVFSTASKDATA_H ? */

#undef CopyFile
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
 *  Virtual File System (VFS)
 *
 *  A VFS allows you to present a "file system" to the rest of the application which is a simplified abstracted view of reality
 *  If you use say compressed files or disk images etc etc you dont want the complexity of dealing with those formats to be exposed
 *  to the rest of your application. The VFS hides this complexity through IArchive's that can be mounted and for which different
 *  plugins can add support for various formats.
 *  The VFS can also be used to make your application use cloud storage without all its specific complexity as if it is a local filesystem
 *  As such using the VFS would be recommended for any application that uses a fair amount of "files"
 */
class GUCEF_VFS_PUBLIC_CPP CVFS : public CORE::CTSGNotifier          ,
                                  public CORE::CGloballyConfigurable ,
                                  public CORE::CIDirectoryWatcher    
                                  
{
    public:
    
    typedef CORE::CTAbstractFactory< CString, CArchive, MT::CMutex >    TAbstractArchiveFactory;
    typedef TAbstractArchiveFactory::TProductPtr                        TArchivePtr;
    typedef TAbstractArchiveFactory::TConcreteFactory                   TArchiveFactory;
    typedef CArchive::CVFSHandlePtr                                     CVFSHandlePtr;
    typedef CArchive::TStringVector                                     TStringVector;
    typedef CArchive::TStringSet                                        TStringSet;
    
    static const CORE::CString FileSystemArchiveTypeName;
    static const CORE::CString DefaultASyncOpsThreadPoolName;

    static const CORE::CEvent AsyncVfsOperationCompletedEvent;
    static const CORE::CEvent ArchiveMountedEvent;
    static const CORE::CEvent ArchiveUnmountedEvent;
    static const CORE::CEvent DelayedArchiveMountingCompletedEvent;
    static const CORE::CEvent VfsInitializationCompletedEvent;

    typedef CORE::TCloneableString      TArchiveMountedEventData; 
    typedef CORE::TCloneableString      TArchiveUnmountedEventData;

    /**
     *  Adds a filesystem root to the VFS 
     */
    void AddRoot( const CString& rootdir                                 ,
                  const CString& archiveName                             ,
                  const bool writeable = false                           ,
                  const bool autoMountArchives = false                   ,
                  const CString& mountPoint = CString::Empty             ,
                  const CString& archiveType = FileSystemArchiveTypeName );

    bool MountArchive( const CString& archiveName  ,
                       const CString& archivePath  ,
                       const bool writeableRequest );
                  
    bool MountArchive( const CString& archiveName  ,
                       const CString& archivePath  ,
                       const CString& mountPoint   ,
                       const bool writeableRequest );
    
    bool MountArchive( const CString& archiveName                               ,
                       const CString& archivePath                               ,
                       const CString& archiveType                               ,
                       const CString& mountPoint                                ,
                       const bool writeableRequest                              ,
                       const bool autoMountSubArchives                          ,
                       const CString& actualNonVfsPathOverride = CString::Empty );

    bool MountArchive( const CArchiveSettings& settings );

    /**
     *  Same as MountArchive() except delays the archive mounting until after a global config 
     *  loading event notification is received
     */
    bool DelayMountArchive( const CArchiveSettings& settings );

    /**
     *  Async attempts to mount an archive with the provided settings.
     *  This is especially useful in cases where the setup phase of a given archive type might take a while
     *
     *  @param settings         settings that are to be used for determining which archive to attempt to mount and how
     *  @param requestorData    optional user defined data to pass along for the async request to be provided back in the response
     *  @param asyncRequestId   optional: user defined identifier for the async request to be provided back in the response
     */
    bool MountArchiveAsync( const CArchiveSettings& settings                           ,
                            CORE::CICloneable* requestorData = GUCEF_NULL              ,
                            const CORE::CString& asyncRequestId = CORE::CString::Empty );
    
    bool MountArchive( const CString& archiveName    ,
                       CVFSHandlePtr archiveResource ,
                       const CString& archiveType    ,
                       const CString& mountPoint     ,
                       const bool writeableRequest   );
    
    bool UnmountArchiveByName( const CString& archiveName );

    void UnmountAllArchives( void );
                  
    void RegisterArchiveFactory( const CString& archiveType      ,
                                 TArchiveFactory& archiveFactory );
    
    void UnregisterArchiveFactory( const CString& archiveType );

    void UnregisterAllArchiveFactories( void );

    void GetListOfSupportedArchiveTypes( TAbstractArchiveFactory::TKeySet& outList ) const;
    
    CVFSHandlePtr GetFile( const CORE::CString& file    ,
                           const char* mode = "rb"      ,
                           const bool overwrite = false );

    /**
     *  Loads the entire file at the given location into memory if possible
     */
    bool LoadFile( CORE::CDynamicBuffer& destinationBuffer ,
                   const CORE::CString& file               ,
                   const char* mode = "rb"                 );

    /**
     *  Loads the entire file at the given location and converts it using the
     *  given codec if possible. If no DataNode codec is named the file extension will be 
     *  used to determine the DataNode codec to use
     */
    bool LoadFile( CORE::CDataNode& destination                           ,
                   const CORE::CString& file                              ,
                   const CORE::CString& codecToUse = CORE::CString::Empty );
                                  
    /**
     *  Attempts to store the provided data as file content
     *  This is a syncronous blocking call and it returns when the operation is 
     *  completed or failed.
     */
    bool StoreAsFile( const CORE::CString& filepath       ,
                      const CORE::CDynamicBuffer& data    ,
                      const CORE::UInt64 bufferOffset = 0 ,
                      const bool overwrite = false        );

    /**
     *  Attempts to store the provided data as file content
     *  This is a asyncronous non-blocking call and it returns right after 
     *  attempting to queue the storage task.
     *  Due to this being an async operation you must subscribe to the appropriote events
     *  in order to know if the operation eventually failed or succeeded.
     *
     *  @param asyncRequestId   optional: user defined identifier for the async request to be provided back in the response
     */
    bool StoreAsFileAsync( const CORE::CString& filepath                              ,
                           const CORE::CDynamicBuffer& data                           ,
                           const CORE::UInt64 offset = 0                              ,
                           const bool overwrite = false                               ,
                           CORE::CICloneable* requestorData = GUCEF_NULL              ,
                           const CORE::CString& asyncRequestId = CORE::CString::Empty );

    /**
     *  Copies a file from one vfs path to another if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param originalFilepath   path to the original file to be copied
     *  @param copyFilepath       destination path for the file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists    
     */
    bool CopyFile( const CORE::CString& originalFilepath ,
                   const CORE::CString& copyFilepath     ,
                   const bool overwrite = false          );

    /**
     *  Async copies a file from one vfs path to another if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param originalFilepath       path to the original file to be copied
     *  @param copyFilepath           destination path for the file copy
     *  @param overwrite              whether to overwrite any existing file at the target path if one exists    
     *  @param asyncRequestId         optional: user defined identifier for the async request to be provided back in the response
     */
    bool CopyFileAsync( const CORE::CString& originalFilepath                      ,
                        const CORE::CString& copyFilepath                          ,
                        const bool overwrite = false                               ,
                        const CORE::CString& asyncRequestId = CORE::CString::Empty );

    /**
     *  Encodes a file from one vfs path to another if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param originalFilepath   path to the original file to be copied
     *  @param encodedFilepath    destination path for the encoded file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param encodeCodec        which named codec to use when encoding the file
     */
    bool EncodeFile( const CORE::CString& originalFilepath ,
                     const CORE::CString& encodedFilepath  ,
                     const bool overwrite                  ,
                     const CORE::CString& codecFamily      ,
                     const CORE::CString& encodeCodec      );

    /**
     *  Async encodes a file from one vfs path to another if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param originalFilepath   path to the original file to be copied
     *  @param encodedFilepath    destination path for the encoded file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param encodeCodec        which named codec to use when encoding the file
     *  @param asyncRequestId     optional: user defined identifier for the async request to be provided back in the response
     */
    bool EncodeFileAsync( const CORE::CString& originalFilepath                      ,
                          const CORE::CString& encodedFilepath                       ,
                          const bool overwrite                                       ,
                          const CORE::CString& codecFamily                           ,
                          const CORE::CString& encodeCodec                           ,
                          const CORE::CString& asyncRequestId = CORE::CString::Empty );

    /**
     *  Encodes a file from the buffer given if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param data               Buffer with the data to be encoded and stored as a file
     *  @param bufferOffset       Offset into the buffer from where to start
     *  @param encodedFilepath    destination path for the encoded file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param encodeCodec        which named codec to use when encoding the file
     */
    bool EncodeAsFile( const CORE::CDynamicBuffer& data     ,
                       const CORE::UInt64 bufferOffset      ,
                       const CORE::CString& encodedFilepath ,
                       const bool overwrite                 ,
                       const CORE::CString& codecFamily     ,
                       const CORE::CString& encodeCodec     );

    /**
     *  Async encodes a file from the buffer given if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param data               Buffer with the data to be encoded and stored as a file
     *  @param bufferOffset       Offset into the buffer from where to start
     *  @param encodedFilepath    destination path for the encoded file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param encodeCodec        which named codec to use when encoding the file
     *  @param asyncRequestId     optional: user defined identifier for the async request to be provided back in the response
     */
    bool EncodeAsFileAsync( const CORE::CDynamicBuffer& data                           ,
                            const CORE::UInt64 bufferOffset                            ,
                            const CORE::CString& encodedFilepath                       ,
                            const bool overwrite                                       ,
                            const CORE::CString& codecFamily                           ,
                            const CORE::CString& encodeCodec                           ,
                            const CORE::CString& asyncRequestId = CORE::CString::Empty );

    /**
     *  Encodes data sourced outside the VFS and stored it as a file in the VFS if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param externalData       Access to data sourced outside the VFS which is to be encoded and stored as a file
     *  @param encodedFilepath    destination path for the encoded file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param encodeCodec        which named codec to use when encoding the file
     */
    bool EncodeAsFile( CORE::CIOAccess& externalData        ,
                       const CORE::CString& encodedFilepath ,
                       const bool overwrite                 ,
                       const CORE::CString& codecFamily     ,
                       const CORE::CString& encodeCodec     );

    /**
     *  Async encodes data sourced outside the VFS and stored it as a file in the VFS if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param externalData       Access to data sourced outside the VFS which is to be encoded and stored as a file
     *  @param encodedFilepath    destination path for the encoded file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param encodeCodec        which named codec to use when encoding the file
     *  @param asyncRequestId     optional: user defined identifier for the async request to be provided back in the response
     */
    bool EncodeAsFileAsync( CORE::CIOAccess& externalData                              ,
                            const CORE::CString& encodedFilepath                       ,
                            const bool overwrite                                       ,
                            const CORE::CString& codecFamily                           ,
                            const CORE::CString& encodeCodec                           ,
                            const CORE::CString& asyncRequestId = CORE::CString::Empty );

    /**
     *  Decodes a file from one vfs path to another if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param originalFilepath   path to the original file to be copied
     *  @param decodedFilepath    destination path for the encoded file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param decodeCodec        which named codec to use when decoding the file
     */
    bool DecodeFile( const CORE::CString& originalFilepath ,
                     const CORE::CString& decodedFilepath  ,
                     const bool overwrite                  ,
                     const CORE::CString& codecFamily      ,
                     const CORE::CString& decodeCodec      );

    /**
     *  Async decodes a file from one vfs path to another if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param originalFilepath   path to the original file to be copied
     *  @param decodedFilepath    destination path for the encoded file copy
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param decodeCodec        which named codec to use when decoding the file
     *  @param asyncRequestId     optional: user defined identifier for the async request to be provided back in the response
     */
    bool DecodeFileAsync( const CORE::CString& originalFilepath                      ,
                          const CORE::CString& decodedFilepath                       ,
                          const bool overwrite                                       ,
                          const CORE::CString& codecFamily                           ,
                          const CORE::CString& decodeCodec                           ,
                          const CORE::CString& asyncRequestId = CORE::CString::Empty );

    /**
     *  Encodes a file from the buffer given if possible
     *  the target file must be located in a writable target mounted archive
     *
     *  @param outputData         Buffer to be filled with the data to be decoded from the file
     *  @param bufferOffset       Offset into the outputData buffer from where to start writing
     *  @param encodedFilePath    Path for the encoded file we wish to decode
     *  @param overwrite          whether to overwrite any existing file at the target path if one exists
     *  @param codecFamily        which codec family to reference when looking for the named codec
     *  @param encodeCodec        which named codec to use when encoding the file
     */
    bool DecodeAsFile( CORE::CDynamicBuffer& outputData     ,
                       const CORE::UInt64 bufferOffset      ,
                       const CORE::CString& encodedFilePath ,
                       const CORE::CString& codecFamily     ,
                       const CORE::CString& decodeCodec     );

    /**
     *  Checks if the given 'filePathToCheck' points to a resource that is a sub path of the given 'dirPath'
     *  Note that this does not actually check if the file resource exists on the storage medium nor does it
     *  validate the existance of the dir. It merely applies VFS path formatting rules for comparison reasons.
     */
    bool IsFilePathSubOfDirPath( const CORE::CString& dirPath         ,
                                 const CORE::CString& filePathToCheck ) const;

    bool DeleteFile( const CString& filePath ,
                     bool okIfItDoesNotExist );

    bool MoveFile( const CString& oldFilePath ,
                   const CString& newFilePath ,
                   const bool overwrite       );

    bool MoveFileAsync( const CString& oldFilePath                                 ,
                        const CString& newFilePath                                 ,
                        const bool overwrite                                       ,
                        const CORE::CString& asyncRequestId = CORE::CString::Empty );

    virtual bool GetFileList( TStringVector& outputList                       ,
                              const CString& location                         ,
                              bool recursive = false                          ,
                              bool includePathInFilename = false              ,
                              const VFS::CString& nameFilter = CString::Empty ,
                              UInt32 maxListEntries = GUCEF_UINT16MAX         ) const;

    virtual bool GetDirList( TStringVector& outputList                  ,
                             const CString& location                    ,
                             bool recursive = false                     ,
                             bool includeParentDirInName = false        ,
                             const CString& nameFilter = CString::Empty ,
                             UInt32 maxListEntries = GUCEF_UINT16MAX    ) const;

    void GetFileList( CORE::CDataNode& outputDataTree                    ,
                      const CORE::CString& location                      ,               
                      bool recursive = false                             ,
                      const CORE::CString& filter = CORE::CString::Empty ,
                      const bool addHash = false                         ,
                      UInt32 maxListEntries = GUCEF_UINT16MAX            ) const;
                  
    /**
     *  Checks if the item pointed at is a mounted archive
     */
    bool IsMountedArchive( const CString& location ) const;

    bool IsDelayedArchiveMountingCompleted( void ) const;

    /**
     *  Obtains a snapshot-in-time evaluation of the initialization completeness of the 
     *  persistence backends used. Given the abstractions one cannot assume that merely accessing the
     *  VFS equals full initialization. There may be dependencies at play which need to be satisfied async.
     */
    bool IsInitialized( void ) const;

    /**
     *  Obtains a snapshot-in-time evaluation of the current health of the archives
     *  mounted into the VFS plus the overall VFS health. 
     */
    bool IsHealthy( void ) const;

    /**
     *  Obtains a snapshot-in-time evaluation of the current connectivity of the archives
     *  mounted into the VFS. This mainly comes into play for various forms of networked
     *  storage.
     */
    bool IsConnected( void ) const;
    
    /**
     *  Searches for archives mounted at the "location"
     *  directory and below (if recursive is true) and filters the results.
     */
    void GetMountedArchiveList( const CString& location ,
                                const CString& filter   ,
                                const bool recursive    ,
                                TStringSet& outputList  ) const;
    
    bool DirExists( const CString& dirPath ) const; 
    
    bool FileExists( const CString& filePath ) const;
        
    // @deprecated: use GetFileMetaData()
    UInt64 GetFileSize( const CString& filePath ) const;
    
    CORE::CString GetFileHash( const CString& filename ) const;
    
    CORE::CDateTime GetFileModificationTime( const CString& filename ) const;

    bool GetFileMetaData( const CString& filename           ,
                          CORE::CResourceMetaData& metaData ) const; 

    bool GetActualFilePath( const CString& file ,
                            CString& path       ) const;

    bool GetVfsPathForAbsolutePath( const CString& absolutePath ,
                                    CString& relativePath       ) const;

    void SetMemloadSize( UInt32 bytesize );
    
    UInt32 GetMemloadSize( void ) const;

    virtual bool AddDirToWatch( const CString& dirToWatch       ,
                                const CDirWatchOptions& options ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool RemoveDirToWatch( const CString& dirToWatch ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool RemoveAllWatches( void ) GUCEF_VIRTUAL_OVERRIDE;
    
    /**
     *      Attempts to store the given tree in the file
     *      given according to the method of the codec metadata
     *
     *      @param tree the data tree you wish to store
     *      @return wheter storing the tree was successfull
     */
    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;
                                
    /**
     *      Attempts to load data from the given file to the 
     *      root node given. The root data will be replaced 
     *      and any children the node may already have will be deleted.
     *
     *      @param treeroot pointer to the node that is to act as root of the data tree
     *      @return whether building the tree from the given file was successfull.
     */                                    
    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;                                             

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    static bool FilterValidation( const CString& filename , 
                                  const CString& filter   );

    static void RegisterEvents( void );
    
    protected:

    virtual MT::TLockStatus Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual MT::TLockStatus Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual MT::TLockStatus ReadOnlyLock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual MT::TLockStatus ReadOnlyUnlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnPumpedNotify( CORE::CNotifier* notifier                 ,
                                 const CORE::CEvent& eventid               ,
                                 CORE::CICloneable* eventdata = GUCEF_NULL ) GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CVfsGlobal;

    CVFS( void );

    virtual ~CVFS();
    
    private:

    struct SMountEntry
    {
        CORE::CString abspath;
        CORE::CString path;
        bool writeable;
        TArchivePtr archive;
        CORE::CString mountPath;
    };
    typedef struct SMountEntry TMountEntry;
    
    struct SConstMountLink
    {
        const TMountEntry* mountEntry;
        CString remainder;
    };
    typedef struct SConstMountLink TConstMountLink;
    
    typedef std::vector< TMountEntry, basic_allocator< TMountEntry > >              TMountVector;
    typedef std::vector< TConstMountLink, basic_allocator< TConstMountLink > >      TConstMountLinkVector;
    typedef CORE::CTSharedPtr< CORE::CDynamicBuffer, MT::CMutex >                   TDynamicBufferPtr;
    typedef std::vector< CArchiveSettings, basic_allocator< CArchiveSettings > >    TArchiveSettingsVector;
    typedef std::map< VFS::CArchive*, TMountEntry* >                                TArchivePtrToMountEntryMap;

    typedef CORE::CTEventHandlerFunctor< CVFS >          TEventCallback;
    
    CVFS( const CVFS& src );                /**< not implemented, must be unique */
    CVFS& operator=( const CVFS& src );     /**< not implemented, must be unique */

    void GetEligableMounts( const CString& location                ,
                            bool mustBeWritable                    ,
                            TConstMountLinkVector& mountLinkVector ) const;
    
    void RegisterEventHandlers( void );

    void OnGlobalConfigLoadFinished( CORE::CNotifier* notifier                 ,
                                     const CORE::CEvent& eventid               ,
                                     CORE::CICloneable* eventdata = GUCEF_NULL );

    void MountAllDelayMountedArchives( void );

    VFS::CString ConformVfsDirPath( const VFS::CString& originalDirPath ) const;

    VFS::CString ConformVfsFilePath( const VFS::CString& originalFilePath ) const;

    void OnArchiveDirectoryWatcherEvent( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventid  ,
                                         CORE::CICloneable* eventdata );
    
    bool LoadVfsSystemConfig( const CORE::CDataNode& cfg );
    
    private:    
    
    TMountVector m_mountList;
    UInt32 m_maxmemloadsize;
    CString m_asyncOpsThreadpool;
    UInt32 m_asyncOpsMinWorkerThreads;
    TAbstractArchiveFactory m_abstractArchiveFactory;
    TFileSystemArchiveFactory m_fileSystemArchiveFactory;
    TArchiveSettingsVector m_delayMountedArchiveSettings;
    TArchivePtrToMountEntryMap m_archivePtrToMountEntryLookup;
    bool m_delayedArchiveMountingIsComplete;
    MT::CReadWriteLock m_rwdataLock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_VFS_CVFS_H ? */
