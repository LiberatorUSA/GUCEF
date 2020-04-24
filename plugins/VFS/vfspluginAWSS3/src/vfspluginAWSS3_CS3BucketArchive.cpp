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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CSUBFILEACCESS_H
#include "gucefCORE_CSubFileAccess.h"
#define GUCEF_CORE_CSUBFILEACCESS_H
#endif /* GUCEF_CORE_CSUBFILEACCESS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "vfspluginAWSS3_CS3BucketArchive.h"

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CS3BucketArchive::CS3BucketArchive( void )
    : CIArchive()       
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CS3BucketArchive::~CS3BucketArchive()
{GUCEF_TRACE;

    UnloadArchive();
}

/*-------------------------------------------------------------------------*/

VFS::CIArchive::CVFSHandlePtr
CS3BucketArchive::GetFile( const VFS::CString& file      ,
                           const char* mode              ,
                           const VFS::UInt32 memLoadSize ,
                           const bool overwrite          )
{GUCEF_TRACE;

    return VFS::CVFS::CVFSHandlePtr();
}

/*-------------------------------------------------------------------------*/

void
CS3BucketArchive::GetList( TStringSet& outputList       ,
                           const VFS::CString& location ,
                           bool recursive               ,
                           bool includePathInFilename   ,
                           const VFS::CString& filter   ,
                           bool addFiles                ,
                           bool addDirs                 ) const
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::FileExists( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

VFS::UInt32
CS3BucketArchive::GetFileSize( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

time_t
CS3BucketArchive::GetFileModificationTime( const VFS::CString& filePath ) const
{
    return 0;
}

/*-------------------------------------------------------------------------*/

VFS::CString
CS3BucketArchive::GetFileHash( const VFS::CString& file ) const
{GUCEF_TRACE;

    return VFS::CString();
}

/*-------------------------------------------------------------------------*/

const VFS::CString&
CS3BucketArchive::GetArchiveName( void ) const
{GUCEF_TRACE;

    return "";
}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::IsWriteable( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::LoadArchive( const VFS::CString& archiveName ,
                               CVFSHandlePtr vfsResource       ,
                               const bool writeableRequest     )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CS3BucketArchive::LoadArchive( const VFS::CString& archiveName ,
                               CVFSHandlePtr vfsResource       ,
                               const bool writeableRequest     )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::UnloadArchive( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

void
CS3BucketArchive::DestroyObject( VFS::CVFSHandle* objectToBeDestroyed )
{GUCEF_TRACE;

    delete objectToBeDestroyed;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSS3 */
}; /* namespace VFSPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
