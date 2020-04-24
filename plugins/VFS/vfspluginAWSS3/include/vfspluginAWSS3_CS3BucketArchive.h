/*
 *  vfspluginAWSS3: Generic GUCEF VFS plugin for dealing with S3 storage in AWS
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#ifndef VFSPLUGIN_AWSS3_CS3BUCKETARCHIVE_H
#define VFSPLUGIN_AWSS3_CS3BUCKETARCHIVE_H

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

#ifndef GUCEF_VFSPLUGIN_AWSS3_MACROS_H
#include "vfspluginAWSS3_macros.h"
#define GUCEF_VFSPLUGIN_AWSS3_MACROS_H
#endif /* GUCEF_VFSPLUGIN_AWSS3_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFSPLUGIN {
namespace AWSS3 {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CS3BucketArchive : public VFS::CIArchive
{
    public:

    CS3BucketArchive( void );

    virtual ~CS3BucketArchive();

    virtual CVFSHandlePtr GetFile( const VFS::CString& file          ,
                                   const char* mode = "rb"           ,
                                   const VFS::UInt32 memLoadSize = 0 ,
                                   const bool overwrite = false      );

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

    virtual time_t GetFileModificationTime( const VFS::CString& filePath ) const;

    virtual const VFS::CString& GetArchiveName( void ) const;

    virtual bool IsWriteable( void ) const;

    virtual bool LoadArchive( const VFS::CString& archiveName ,
                              const VFS::CString& archivePath ,
                              const bool writableRequest );

    virtual bool LoadArchive( const VFS::CString& archiveName ,
                              CVFSHandlePtr vfsResource       ,
                              const bool writeableRequest     );

    virtual bool UnloadArchive( void );

    virtual void DestroyObject( VFS::CVFSHandle* objectToBeDestroyed );

    private:

    CS3BucketArchive( const CS3BucketArchive& src );
    CS3BucketArchive& operator=( const CS3BucketArchive& src );

    private:

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSS3 */
}; /* namespace VFSPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* VFSPLUGIN_AWSS3_CS3BUCKETARCHIVE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 04-05-2005 :
        - Dinand: Initial version.

-----------------------------------------------------------------------------*/
