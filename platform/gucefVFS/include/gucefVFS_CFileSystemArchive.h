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

#ifndef GUCEF_VFS_CFILESYSTEMARCHIVE_H
#define GUCEF_VFS_CFILESYSTEMARCHIVE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTFACTORY_H
#include "CTFactory.h"
#define GUCEF_CORE_CTFACTORY_H
#endif /* GUCEF_CORE_CTFACTORY_H ? */

#ifndef GUCEF_VFS_CIARCHIVE_H
#include "gucefVFS_CIArchive.h"
#define GUCEF_VFS_CIARCHIVE_H
#endif /* GUCEF_VFS_CIARCHIVE_H ? */

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

class GUCEF_VFS_PUBLIC_CPP CFileSystemArchive : public CIArchive
{
    public:
    
    CFileSystemArchive( void );
    
    CFileSystemArchive( const CString& archiveName ,
                        const CString& rootDir     ,
                        const bool writeable       );
    
    virtual ~CFileSystemArchive();
    
    virtual CVFSHandlePtr GetFile( const CString& file          ,
                                   const char* mode = "rb"      ,
                                   const UInt32 memLoadSize = 0 ,
                                   const bool overwrite = false );

    virtual bool StoreAsFile( const CORE::CString& filepath    ,
                              const CORE::CDynamicBuffer& data ,
                              const CORE::UInt64 offset        ,
                              const bool overwrite             );

    virtual void GetList( TStringSet& outputList             ,
                          const CString& location            , 
                          bool recursive = false             ,
                          bool includePathInFilename = false ,
                          const CString& filter = ""         ,
                          bool addFiles = true               ,
                          bool addDirs  = false              ) const;
    
    virtual bool FileExists( const CString& filePath ) const;
    
    virtual UInt32 GetFileSize( const CString& filePath ) const;
    
    virtual CString GetFileHash( const CString& file ) const;
    
    virtual time_t GetFileModificationTime( const CString& filePath ) const;
    
    virtual const CString& GetArchiveName( void ) const;
    
    virtual bool IsWriteable( void ) const;
    
    virtual bool LoadArchive( const CArchiveSettings& settings );

    virtual bool LoadArchive( const VFS::CString& archiveName ,
                              CVFSHandlePtr vfsResource       ,
                              const bool writeableRequest     );
    
    virtual bool UnloadArchive( void );
    
    virtual void DestroyObject( CVFSHandle* sharedPointer );

    virtual const CString& GetType( void ) const;
    
    const CString& GetRootDir( void ) const;
    
    private:

    void GetListFromRoot( const CORE::CString& root   , 
                          bool recursive              , 
                          bool includePathInFilename  , 
                          const CORE::CString& filter , 
                          TStringSet& outputList      , 
                          bool addFiles               ,
                          bool addDirs                ) const;
    
    CVFSHandle* LoadFromDisk( const CString& file          ,
                              const char* mode = "rb"      ,
                              const UInt32 memLoadSize = 0 ,
                              const bool overwrite = false );

    private:
    typedef CORE::CTSharedPtr< CORE::CDynamicBuffer > TDynamicBufferPtr;
    typedef std::map< CString, TDynamicBufferPtr >    TFileMemCache;
    
    TFileMemCache m_diskCacheList;
    CString m_rootDir;
    CString m_archiveName;
    bool m_writable;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTFactory< VFS::CIArchive, CFileSystemArchive > TFileSystemArchiveFactory;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_VFS_CFILESYSTEMARCHIVE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-08-2005 :
        - Dinand: implemented

---------------------------------------------------------------------------*/