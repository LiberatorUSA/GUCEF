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

#ifndef GUCEF_CORE_CIOACCESSTOIOSTREAM_H
#include "gucefCORE_CIOAccessToIOStream.h"
#define GUCEF_CORE_CIOACCESSTOIOSTREAM_H
#endif /* GUCEF_CORE_CIOACCESSTOIOSTREAM_H ? */

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

#ifndef GUCEF_VFSPLUGIN_AWSS3_CAWSS3GLOBAL_H
#include "vfspluginAWSS3_CAwsS3Global.h"
#define GUCEF_VFSPLUGIN_AWSS3_CAWSS3GLOBAL_H
#endif /* GUCEF_VFSPLUGIN_AWSS3_CAWSS3GLOBAL_H ? */

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
    : CORE::CObservingNotifier()
    , VFS::CIArchive() 
    , m_objects()
    , m_archiveName()
    , m_autoMountBuckets( false )
    , m_writeableRequest( false )
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CS3BucketArchive::~CS3BucketArchive()
{GUCEF_TRACE;

    UnloadArchive();
}

/*-------------------------------------------------------------------------*/

void
CS3BucketArchive::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CS3BucketArchive::OnAwsS3Initialized );
    SubscribeTo( VFSPLUGIN::AWSS3::CAwsS3Global::Instance()            ,
                 VFSPLUGIN::AWSS3::CAwsS3Global::AwsS3InitializedEvent ,
                 callback                                              );
}

/*-------------------------------------------------------------------------*/

VFS::CIArchive::CVFSHandlePtr
CS3BucketArchive::GetFile( const VFS::CString& file      ,
                           const char* mode              ,
                           const VFS::UInt32 memLoadSize ,
                           const bool overwrite          )
{GUCEF_TRACE;

    try
    {
        Aws::S3::S3Client* s3Client = CAwsS3Global::Instance()->GetS3Client();
        if ( GUCEF_NULL == s3Client )
            return VFS::CIArchive::CVFSHandlePtr();

        Aws::S3::Model::GetObjectRequest objectRequest;
        objectRequest.SetBucket( m_archiveName );
        objectRequest.SetKey( file );

        Aws::S3::Model::GetObjectOutcome getObjectOutcome = s3Client->GetObject( objectRequest );
        if ( getObjectOutcome.IsSuccess() )
        {
            //getObjectOutcome.GetResult().GetBody()
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "S3BucketArchive: Obtained Object \"" + file + "\" from Bucket \"" + m_archiveName + "\"" );
            return VFS::CIArchive::CVFSHandlePtr();
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "S3BucketArchive: GetObject error: " + 
                getObjectOutcome.GetError().GetExceptionName() + " - " + getObjectOutcome.GetError().GetMessage() );
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, CORE::CString( "S3BucketArchive: Exception trying to S3 get bucket object: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "S3BucketArchive: Unknown exception trying to S3 get bucket object" );
    }
    return VFS::CIArchive::CVFSHandlePtr();
}

/*-------------------------------------------------------------------------*/

bool 
CS3BucketArchive::StoreAsFile( const CORE::CString& filepath    ,
                               const CORE::CDynamicBuffer& data ,
                               const CORE::UInt64 offset        ,
                               const bool overwrite             )
{GUCEF_TRACE;

    if ( 0 != offset )
        return false; // not supported or emulated right now
    
    try
    {
        Aws::S3::S3Client* s3Client = CAwsS3Global::Instance()->GetS3Client();
        if ( GUCEF_NULL == s3Client )
            return VFS::CIArchive::CVFSHandlePtr();

        Aws::S3::Model::PutObjectRequest objectRequest;
        objectRequest.SetBucket( m_archiveName );
        objectRequest.SetKey( filepath );

        CORE::CDynamicBufferAccess bufferAccess( data );
        std::shared_ptr< CORE::CIOAccessToIOStream > dataAccess( std::make_shared< CORE::CIOAccessToIOStream >( bufferAccess ) );
        std::shared_ptr< Aws::IOStream > baseDataAccess = std::static_pointer_cast< Aws::IOStream >( dataAccess );
        objectRequest.SetBody( baseDataAccess );
        
        Aws::S3::Model::PutObjectOutcome putObjectOutcome = s3Client->PutObject( objectRequest );
        if ( putObjectOutcome.IsSuccess() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "S3BucketArchive: Wrote data for file \"" + filepath + "\" to Bucket \"" + m_archiveName + "\"" );
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "S3BucketArchive: PutObject error: " + 
                putObjectOutcome.GetError().GetExceptionName() + " - " + putObjectOutcome.GetError().GetMessage() );
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, CORE::CString( "S3BucketArchive: Exception trying Put() to S3 bucket object: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "S3BucketArchive: Unknown exception trying Put() to S3 bucket object" );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CS3BucketArchive::GetList( TStringSet& outputList              ,
                           const VFS::CString& mountLocation   , 
                           const VFS::CString& archiveLocation ,
                           bool recursive                      ,
                           bool includePathInFilename          ,
                           const VFS::CString& filter          ,
                           bool addFiles                       ,
                           bool addDirs                        ) const
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::FileExists( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    auto i = m_objects.find( filePath );
    if ( i != m_objects.end() )
    {
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CS3BucketArchive::DeleteFile( const VFS::CString& filePath )
{GUCEF_TRACE;

    // Delete requires write access, not just read
    if ( !m_writeableRequest )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "S3BucketArchive: Refusing to deleted file (aka object) \"" + 
            filePath + "\" in Bucket \"" + m_archiveName + "\" because the archive is mounted as read-only" );
        return false;
    }
    
    try
    {
        Aws::S3::S3Client* s3Client = CAwsS3Global::Instance()->GetS3Client();
        if ( GUCEF_NULL == s3Client )
            return VFS::CIArchive::CVFSHandlePtr();

        Aws::S3::Model::DeleteObjectRequest objectRequest;
        objectRequest.SetBucket( m_archiveName );
        objectRequest.SetKey( filePath );

        Aws::S3::Model::DeleteObjectOutcome deletetObjectOutcome = s3Client->DeleteObject( objectRequest );
        if ( deletetObjectOutcome.IsSuccess() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "S3BucketArchive: Deleted file (aka object) \"" + filePath + "\" in Bucket \"" + m_archiveName + "\"" );
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "S3BucketArchive: DeleteObject error: " + 
                deletetObjectOutcome.GetError().GetExceptionName() + " - " + deletetObjectOutcome.GetError().GetMessage() );
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, CORE::CString( "S3BucketArchive: Exception trying DeleteFile() to S3 bucket object: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "S3BucketArchive: Unknown exception trying DeleteFile() to S3 bucket object" );
    }
    return false;
}
 
/*-------------------------------------------------------------------------*/

VFS::UInt32
CS3BucketArchive::GetFileSize( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    auto i = m_objects.find( filePath );
    if ( i != m_objects.end() )
    {
        const auto& objectRef = (*i).second;
        return (VFS::UInt32) objectRef.GetSize();
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime
CS3BucketArchive::GetFileModificationTime( const VFS::CString& filePath ) const
{
    auto i = m_objects.find( filePath );
    if ( i != m_objects.end() )
    {
        const auto& objectRef = (*i).second;
        return CORE::CDateTime( std::chrono::system_clock::to_time_t( objectRef.GetLastModified().UnderlyingTimestamp() ), true );
    }
    return CORE::CDateTime();
}

/*-------------------------------------------------------------------------*/

VFS::CString
CS3BucketArchive::GetFileHash( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    auto i = m_objects.find( filePath );
    if ( i != m_objects.end() )
    {
        const auto& objectRef = (*i).second;
        return objectRef.GetETag();  // Per the SDK the ETag is actually implemented as a MD5 hash
    }
    return VFS::CString::Empty;
}

/*-------------------------------------------------------------------------*/

const VFS::CString&
CS3BucketArchive::GetArchiveName( void ) const
{GUCEF_TRACE;

    return m_archiveName;
}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::IsWriteable( void ) const
{GUCEF_TRACE;

    return m_writeableRequest;
}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::LoadBucketObjectIndex( void )
{GUCEF_TRACE;

    try
    {
        Aws::S3::S3Client* s3Client = CAwsS3Global::Instance()->GetS3Client();
        if ( GUCEF_NULL == s3Client )
            return false;

        Aws::S3::Model::ListObjectsRequest objectsRequest;
        objectsRequest.WithBucket( m_archiveName );

        auto listObjectsOutcome = s3Client->ListObjects( objectsRequest );
        if ( listObjectsOutcome.IsSuccess() )
        {
            // Get up to 1000 objects

            #ifdef GUCEF_DEBUG_MODE
            CORE::CString objectKeys;
            #endif

            Aws::Vector< Aws::S3::Model::Object > objectList = listObjectsOutcome.GetResult().GetContents();
            for (auto const& s3Object : objectList )
            {
                auto const& key = s3Object.GetKey();

                #ifdef GUCEF_DEBUG_MODE
                objectKeys += key + ", ";
                #endif
                
                m_objects[ key ] = s3Object;
            }
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "S3BucketArchive: Bucket \"" + m_archiveName + "\"has the following objects: " + objectKeys );
            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "S3BucketArchive: ListObjects error: " + 
                listObjectsOutcome.GetError().GetExceptionName() + " - " + listObjectsOutcome.GetError().GetMessage() );
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_IMPORTANT, CORE::CString( "S3BucketArchive: Exception trying to S3 load bucket object index: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "S3BucketArchive: Unknown exception trying to S3 load bucket object index" );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CS3BucketArchive::OnAwsS3Initialized( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventId  ,
                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;
    
    LoadBucketObjectIndex();
}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::LoadArchive( const VFS::CArchiveSettings& settings )
{GUCEF_TRACE;

    m_archiveName = settings.GetArchiveName();
    m_autoMountBuckets = settings.GetAutoMountSubArchives();
    m_writeableRequest = settings.GetWriteableRequested();

    if ( CAwsS3Global::Instance()->IsS3AccessInitialized() )
        return LoadBucketObjectIndex();
    
    // We will wait for the initialization event, assuming deferred success
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::LoadArchive( const VFS::CString& archiveName ,
                               CVFSHandlePtr vfsResource       ,
                               const bool writeableRequest     )
{GUCEF_TRACE;

    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "S3BucketArchive: Attempting to load a \"AWS::S3::Bucket\" archive from a resource. That does not make sense. Bad config?" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CS3BucketArchive::UnloadArchive( void )
{GUCEF_TRACE;

    m_objects.clear();
    return true;
}

/*-------------------------------------------------------------------------*/

const VFS::CString& 
CS3BucketArchive::GetType( void ) const
{GUCEF_TRACE;

    return CAwsS3Global::AwsS3ArchiveType;
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
