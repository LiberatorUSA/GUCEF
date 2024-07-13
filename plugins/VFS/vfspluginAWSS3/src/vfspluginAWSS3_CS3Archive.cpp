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

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFSPLUGIN_AWSS3_CAWSS3GLOBAL_H
#include "vfspluginAWSS3_CAwsS3Global.h"
#define GUCEF_VFSPLUGIN_AWSS3_CAWSS3GLOBAL_H
#endif /* GUCEF_VFSPLUGIN_AWSS3_CAWSS3GLOBAL_H ? */

#include "vfspluginAWSS3_CS3Archive.h"

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

CS3Archive::CS3Archive( void )
    : CArchive()
    , m_bucketList()
    , m_archiveName()
    , m_autoMountBuckets( false )
    , m_writeableRequest( false )
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CS3Archive::~CS3Archive()
{GUCEF_TRACE;

    UnloadArchive();
}

/*-------------------------------------------------------------------------*/

void
CS3Archive::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CS3Archive::OnAwsS3Initialized );
    SubscribeTo( VFSPLUGIN::AWSS3::CAwsS3Global::Instance()            ,
                 VFSPLUGIN::AWSS3::CAwsS3Global::AwsS3InitializedEvent ,
                 callback                                              );
}

/*-------------------------------------------------------------------------*/

VFS::TBasicVfsResourcePtr
CS3Archive::GetFile( const VFS::CString& file      ,
                     const char* mode              ,
                     const VFS::UInt32 memLoadSize ,
                     const bool overwrite          )
{GUCEF_TRACE;

    return VFS::TBasicVfsResourcePtr();
}

/*-------------------------------------------------------------------------*/

VFS::TBasicVfsResourcePtr 
CS3Archive::GetFileAs( const VFS::CString& file                ,
                       const CORE::CResourceMetaData& metaData ,
                       const char* mode                        ,
                       const UInt32 memLoadSize                ,
                       const bool overwrite                    )
{GUCEF_TRACE;

    // We dont currently support applying meta-data
    return VFS::TBasicVfsResourcePtr();
}

/*-------------------------------------------------------------------------*/

bool 
CS3Archive::StoreAsFile( const CORE::CString& filepath    ,
                         const CORE::CDynamicBuffer& data ,
                         const CORE::UInt64 offset        ,
                         const bool overwrite             )
{GUCEF_TRACE;

    // Files are stored in buckets in S3 not in S3 in general
    // you have to mount a bucket and perform the store action there
    return false;
}

/*-------------------------------------------------------------------------*/

void
CS3Archive::GetFileList( TStringVector& outputList                  ,
                         const VFS::CString& mountLocation          , 
                         const VFS::CString& archiveLocation        ,
                         bool recursive                             ,
                         bool includePathInFilename                 ,
                         const VFS::CString::StringSet& nameFilters ,
                         UInt32 maxListEntries                      ) const
{GUCEF_TRACE;

    // You need to mount a bucket to see its contents
}

/*-------------------------------------------------------------------------*/

void
CS3Archive::GetDirList( TStringVector& outputList                  ,
                        const VFS::CString& mountLocation          , 
                        const VFS::CString& archiveLocation        ,
                        bool recursive                             ,
                        bool includeParentDirInName                ,
                        const VFS::CString::StringSet& nameFilters ,
                        UInt32 maxListEntries                      ) const
{GUCEF_TRACE;

    // @TODO: handle archiveLocation & recursive
    
    // List all S3 buckets available based on our credentials.
    try
    {
        Aws::S3::S3Client* s3Client = CAwsS3Global::Instance()->GetS3Client();
        auto outcome = s3Client->ListBuckets();
        if ( outcome.IsSuccess() )
        {
            const TBucketList& bucketList = outcome.GetResult().GetBuckets();
            for ( auto const &bucket : bucketList )
            {
                if ( outputList.size() >= maxListEntries )
                    break;
                
                CORE::CString bucketName = bucket.GetName();

                if ( !VFS::CVFS::FilterValidation( bucketName, nameFilters ) )
                    continue;
                
                if ( includeParentDirInName )
                {
                    CORE::CString bucketPath = "/aws/s3/" + bucket.GetName();
                    outputList.push_back( bucketPath );
                }
                else
                {
                    outputList.push_back( bucket.GetName() );
                }
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AwsS3Global: ListBuckets error: " + 
                outcome.GetError().GetExceptionName() + " - " + outcome.GetError().GetMessage() );
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, CORE::CString( "AwsS3Global: Exception trying to update S3 bucket inventory: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsS3Global: Unknown exception trying to update S3 bucket inventory" );
    }
}

/*-------------------------------------------------------------------------*/

bool 
CS3Archive::DeleteFile( const VFS::CString& filePath )
{GUCEF_TRACE;

    // We do not allow deleting "files" at this time which would be an entire bucket
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CS3Archive::MoveFile( const VFS::CString& oldFilePath ,
                      const VFS::CString& newFilePath ,
                      const bool overwrite            )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CS3Archive::GetFileMetaData( const VFS::CString& filePath      ,
                             CORE::CResourceMetaData& metaData ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CS3Archive::FileExists( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CS3Archive::DirExists( const VFS::CString& dirPath ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

VFS::UInt64
CS3Archive::GetFileSize( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime
CS3Archive::GetFileModificationTime( const VFS::CString& filePath ) const
{
    return CORE::CDateTime();
}

/*-------------------------------------------------------------------------*/

VFS::CString
CS3Archive::GetFileHash( const VFS::CString& file ) const
{GUCEF_TRACE;

    return VFS::CString();
}

/*-------------------------------------------------------------------------*/

const VFS::CString&
CS3Archive::GetArchiveName( void ) const
{GUCEF_TRACE;

    return m_archiveName;
}

/*-------------------------------------------------------------------------*/

bool
CS3Archive::IsWriteable( void ) const
{GUCEF_TRACE;

    return m_writeableRequest;
}

/*-------------------------------------------------------------------------*/

bool
CS3Archive::LoadBucketList( const bool autoMountBuckets , 
                            const bool writeableRequest )
{GUCEF_TRACE;

    // Setup for all S3 buckets available based on our credentials.
    try
    {
        Aws::S3::S3Client* s3Client = CAwsS3Global::Instance()->GetS3Client();
        auto outcome = s3Client->ListBuckets();
        if ( outcome.IsSuccess() )
        {
            m_bucketList = outcome.GetResult().GetBuckets();

            if ( autoMountBuckets )
            {
                CORE::CString bucketList;
                VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
                for ( auto const &bucket : m_bucketList )
                {
                    CORE::CString bucketPath = "/aws/s3/" + bucket.GetName();
                    if ( vfs.MountArchive( bucket.GetName(), bucketPath, CAwsS3Global::AwsS3BucketArchiveType, bucketPath, writeableRequest, autoMountBuckets, bucketPath ) )
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "AwsS3Global: Auto mounted S3 bucket on VFS at path: \"" + bucketPath + "\"" );
                    }
                    else
                    {
                        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsS3Global: Failed to auto-mount S3 bucket on VFS at path: \"" + bucketPath + "\"" );    
                    }
                    bucketList +=  bucket.GetName() + ", ";
                }
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AwsS3Global: All available Amazon S3 buckets with the current credentials: " + bucketList );
            }
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AwsS3Global: ListBuckets error: " + 
                outcome.GetError().GetExceptionName() + " - " + outcome.GetError().GetMessage() );
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, CORE::CString( "AwsS3Global: Exception trying to update S3 bucket inventory: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsS3Global: Unknown exception trying to update S3 bucket inventory" );
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void
CS3Archive::OnAwsS3Initialized( CORE::CNotifier* notifier    ,
                                const CORE::CEvent& eventId  ,
                                CORE::CICloneable* eventData )
{GUCEF_TRACE;
    
    LoadBucketList( m_autoMountBuckets, m_writeableRequest );
}

/*-------------------------------------------------------------------------*/

bool
CS3Archive::LoadArchive( const VFS::CArchiveSettings& settings )
{GUCEF_TRACE;

    m_archiveName = settings.GetArchiveName();
    m_autoMountBuckets = settings.GetAutoMountSubArchives();
    m_writeableRequest = settings.GetWriteableRequested();
    
    if ( CAwsS3Global::Instance()->IsS3AccessInitialized() )
        return LoadBucketList( m_autoMountBuckets, m_writeableRequest );
    
    // We will wait for the initialization event, assuming deferred success
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CS3Archive::LoadArchive( const VFS::CString& archiveName       ,
                         VFS::TBasicVfsResourcePtr vfsResource ,
                         const bool writeableRequest           )
{GUCEF_TRACE;

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "S3Archive: Attempting to load a \"AWS::S3\" archive from a resource. That does not make sense. Bad config?" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CS3Archive::UnloadArchive( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

const VFS::CString& 
CS3Archive::GetType( void ) const
{GUCEF_TRACE;

    return CAwsS3Global::AwsS3ArchiveType;
}

/*-------------------------------------------------------------------------*/

void
CS3Archive::DestroyObject( VFS::CVFSHandle* objectToBeDestroyed ) const
{GUCEF_TRACE;

    GUCEF_DELETE objectToBeDestroyed;
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
