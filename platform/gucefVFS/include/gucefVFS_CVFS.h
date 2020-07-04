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

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"          /* abstract base class interface for configurable objects */
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

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

#ifndef GUCEF_VFS_CIARCHIVE_H
#include "gucefVFS_CIArchive.h"
#define GUCEF_VFS_CIARCHIVE_H
#endif /* GUCEF_VFS_CIARCHIVE_H ? */

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
class GUCEF_VFS_PUBLIC_CPP CVFS : public CORE::CTSGNotifier   ,
                                  public CORE::CIConfigurable
{
    public:
    
    typedef CORE::CTAbstractFactory< CString, CIArchive > TAbstractArchiveFactory;
    typedef TAbstractArchiveFactory::TConcreteFactory     TArchiveFactory;
    typedef CIArchive::CVFSHandlePtr                      CVFSHandlePtr;
    typedef CIArchive::TStringList                        TStringList;
    typedef CIArchive::TStringSet                         TStringSet;
    
    static const CORE::CString FileSystemArchiveTypeName;
    static const CORE::CEvent AsyncVfsOperationCompletedEvent;

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

    bool MountArchiveAsync( const CArchiveSettings& settings              ,
                            CORE::CICloneable* requestorData = GUCEF_NULL );
    
    bool MountArchive( const CString& archiveName    ,
                       CVFSHandlePtr archiveResource ,
                       const CString& archiveType    ,
                       const CString& mountPoint     ,
                       const bool writeableRequest   );
    
    bool UnmountArchiveByName( const CString& archiveName );
                  
    void RegisterArchiveFactory( const CString& archiveType      ,
                                 TArchiveFactory& archiveFactory );
    
    void UnregisterArchiveFactory( const CString& archiveType );

    void GetListOfSupportedArchiveTypes( TAbstractArchiveFactory::TKeySet& outList ) const;
    
    CVFSHandlePtr GetFile( const CORE::CString& file    ,
                           const char* mode = "rb"      ,
                           const bool overwrite = false );
                                  
    /**
     *  Attempts to store the provided data as file content
     *  This is a syncronous blocking call and it returns when the operation is 
     *  completed or failed.
     */
    bool StoreAsFile( const CORE::CString& filepath    ,
                      const CORE::CDynamicBuffer& data ,
                      const CORE::UInt64 offset = 0    ,
                      const bool overwrite = false     );

    /**
     *  Attempts to store the provided data as file content
     *  This is a asyncronous non-blocking call and it returns right after 
     *  attempting to queue the storage task.
     *  Due to this being an async operation you must subscribe to the appropriote events
     *  in order to know if the operation eventually failed or succeeded.
     */
    bool StoreAsFileAsync( const CORE::CString& filepath                 ,
                           const CORE::CDynamicBuffer& data              ,
                           const CORE::UInt64 offset = 0                 ,
                           const bool overwrite = false                  ,
                           CORE::CICloneable* requestorData = GUCEF_NULL );

    void GetList( TStringSet& outputList             ,
                  const CORE::CString& location      , 
                  bool recursive = false             ,
                  bool includePathInFilename = false ,
                  const CORE::CString& filter = ""   ,
                  bool addFiles = true               ,
                  bool addDirs  = false              ) const;

    void GetList( CORE::CDataNode& outputDataTree    ,
                  const CORE::CString& location      , 
                  bool recursive = false             ,
                  const CORE::CString& filter = ""   ,
                  const bool addHash = false         ) const;
                  
    /**
     *  Checks if the item pointed at is a mounted archive
     */
    bool IsMountedArchive( const CString& location ) const;
    
    /**
     *  Searches for archives mounted at the "location"
     *  directory and below (if recursive is true) and filters the results.
     */
    void GetMountedArchiveList( const CString& location ,
                                const CString& filter   ,
                                const bool recursive    ,
                                TStringSet& outputList  ) const;
    
    bool FileExists( const CString& filePath ) const;
    
    UInt32 GetFileSize( const CString& filePath ) const;
    
    CORE::CString GetFileHash( const CString& filename ) const;
    
    time_t GetFileModificationTime( const CString& filename ) const;

    bool GetActualFilePath( const CString& file ,
                            CString& path       ) const;

    bool GetVfsPathForAbsolutePath( const CString& absolutePath ,
                                    CString& relativePath       ) const;

    void SetMemloadSize( UInt32 bytesize );
    
    UInt32 GetMemloadSize( void ) const;
    
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
        CIArchive* archive;
        CORE::CString mountPath;
    };
    typedef struct SMountEntry TMountEntry;
    
    struct SConstMountLink
    {
        const TMountEntry* mountEntry;
        CString remainder;
    };
    typedef struct SConstMountLink TConstMountLink;
    
    typedef std::vector< TMountEntry >                   TMountVector;
    typedef std::vector< TConstMountLink >               TConstMountLinkVector;
    typedef CORE::CTSharedPtr< CORE::CDynamicBuffer >    TDynamicBufferPtr;
    typedef std::vector< CArchiveSettings >              TArchiveSettingsVector;

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
    
    private:    
    
    TMountVector m_mountList;
    UInt32 _maxmemloadsize;
    TAbstractArchiveFactory m_abstractArchiveFactory;
    TFileSystemArchiveFactory m_fileSystemArchiveFactory;
    TArchiveSettingsVector m_delayMountedArchiveSettings;
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-05-2008 :
        - Dinand: Reworked the VFS system to support a mounting system that
          groups resources in archives that become part of the resource tree in
          a transparent manner. This rework allows the VFS plugins to become 
          operational again as archive factories.
- 24-08-2005 :
        - Dinand: implemented GetNewFile()
        - Dinand: reworked the AddRoot() and root administration mechanisms. This allows
          support for writeable roots and storage of the relative paths without
          data duplication.
        - Dinand: Added support for new writeable files (exclusive access only)
        - Dinand: Added support for read & write files (exclusive access only)
- 18-05-2005 :
        - Dinand: Fixed a bug in FileExists(): If the recource didn't exist the code would get
          stuck in a loop.
- 01-05-2005 :
        - Dinand: Fixed array bounds write violation in LoadFromDiskCache()
        - Dinand: Fixed a bug in Unload(): The last cached item's access object was
          deleted twice.
        - Dinand: Unload() no longer has to search for the handle entry saving us some time           
- 26-04-2005 :
        - Dinand: Fixed a bug in LoadFromDisk(): file information storage accessed an
          invalid heap entry if no storage space was available.
        - Dinand: Fixed a bug in Unload(): The last cached item's access object was
          deleted twice.  
- 12-02-2005 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/
