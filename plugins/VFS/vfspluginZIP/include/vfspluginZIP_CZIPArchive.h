/*
 *  vfspluginZIP: Generic GUCEF VFS plugin for ZIP archives
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef VFSPLUGIN_ZIP_CZIPARCHIVE_H
#define VFSPLUGIN_ZIP_CZIPARCHIVE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_VFSPLUGIN_ZIP_MACROS_H
#include "vfspluginZIP_macros.h"
#define GUCEF_VFSPLUGIN_ZIP_MACROS_H
#endif /* GUCEF_VFSPLUGIN_ZIP_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

// Forward declaration for zziplib to avoid header file dependency.
typedef struct zzip_dir		ZZIP_DIR;
typedef struct zzip_file	ZZIP_FILE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFSPLUGIN {
namespace ZIP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CZIPArchive : public VFS::CIArchive
{
    public:
    
    static const VFS::CString ZipArchiveTypeName;
    
    CZIPArchive( void );
    
    virtual ~CZIPArchive();
    
    virtual CVFSHandlePtr GetFile( const VFS::CString& file          ,
                                   const char* mode = "rb"           ,
                                   const VFS::UInt32 memLoadSize = 0 ,
                                   const bool overwrite = false      );

    virtual bool StoreAsFile( const CORE::CString& filepath    ,
                              const CORE::CDynamicBuffer& data ,
                              const CORE::UInt64 offset        ,
                              const bool overwrite             );
                                  
    virtual void GetList( TStringSet& outputList             ,
                          const VFS::CString& location       , 
                          bool recursive = false             ,
                          bool includePathInFilename = false ,
                          const VFS::CString& filter = ""    ,
                          bool addFiles = true               ,
                          bool addDirs  = false              ) const;
    
    virtual bool FileExists( const VFS::CString& filePath ) const;
    
    virtual VFS::UInt32 GetFileSize( const VFS::CString& filePath ) const;
    
    virtual VFS::CString GetFileHash( const VFS::CString& file ) const;
    
    virtual CORE::CDateTime GetFileModificationTime( const VFS::CString& filePath ) const;
    
    virtual const VFS::CString& GetArchiveName( void ) const;
    
    virtual bool IsWriteable( void ) const;
    
    virtual bool LoadArchive( const VFS::CArchiveSettings& settings );
                              
    virtual bool LoadArchive( const VFS::CString& archiveName ,
                              CVFSHandlePtr vfsResource       ,
                              const bool writeableRequest     );
    
    virtual bool UnloadArchive( void );
    
    virtual const VFS::CString& GetType( void ) const;

    virtual void DestroyObject( VFS::CVFSHandle* objectToBeDestroyed );

    private:

    struct SZipEntry 
    {
        VFS::CString path;
        size_t compressedSize;
        size_t uncompressedSize;
        bool isDir;
    };
    typedef struct SZipEntry TZipEntry;
    
    CZIPArchive( const CZIPArchive& src );
    CZIPArchive& operator=( const CZIPArchive& src );

    void CheckZzipError( int zzipError                 ,
                         const VFS::CString& operation ) const;

    CORE::CIOAccess*
    LoadFile( const VFS::CString& file      ,
              const VFS::UInt32 memLoadSize ) const;
    
    private:
    
    typedef std::vector< TZipEntry > TZipEntryVector;
    
    VFS::CString m_archiveName;
    VFS::CString m_archivePath;
    ZZIP_DIR* m_zipRoot;
    TZipEntryVector m_fileList; 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace ZIP */
}; /* namespace VFSPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* VFSPLUGIN_ZIP_CZIPARCHIVE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 04-05-2005 :
        - Dinand: Initial version.

-----------------------------------------------------------------------------*/
