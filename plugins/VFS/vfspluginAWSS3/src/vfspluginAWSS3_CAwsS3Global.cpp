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

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_PLUGINGLUE_AWSSDK_H
#include "pluginglueAWSSDK.h"
#define GUCEF_PLUGINGLUE_AWSSDK_H
#endif /* GUCEF_PLUGINGLUE_AWSSDK_H ? */

#include "vfspluginAWSS3_CAwsS3Global.h"

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex CAwsS3Global::g_dataLock;
CAwsS3Global* CAwsS3Global::g_instance = GUCEF_NULL;
const CORE::CString CAwsS3Global::AwsS3ArchiveType = "AWS::S3";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CAwsS3Global*
CAwsS3Global::Instance()
{GUCEF_TRACE;

    if ( GUCEF_NULL == g_instance )
    {
        g_dataLock.Lock();
        if ( GUCEF_NULL == g_instance )
        {
            g_instance = new CAwsS3Global();
            g_instance->Initialize();
        }
        g_dataLock.Unlock();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CAwsS3Global::Deinstance( void )
{GUCEF_TRACE;

    g_dataLock.Lock();
    delete g_instance;
    g_instance = GUCEF_NULL;
    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CAwsS3Global::Initialize( void )
{GUCEF_TRACE;

    try
    {
        // Initialize the AWS SDK and its platform tie-ins
        PLUGINGLUE::AWSSDK::CAwsSdkGlobal::Instance();

        VFS::CVfsGlobal::Instance()->GetVfs().RegisterArchiveFactory( AwsS3ArchiveType, m_awsS3ArchiveFactory );

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "AwsS3Global systems initialized" );
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, CORE::CString( "AwsS3Global systems experienced an exception during initialization: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsS3Global systems experienced an unknown exception during initialization, your application may be unstable" );
    }
}

/*-------------------------------------------------------------------------*/

CAwsS3Global::CAwsS3Global( void )
    : m_s3Client( GUCEF_NULL )
    , m_awsS3ArchiveFactory()
{GUCEF_TRACE;

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CAwsS3Global::~CAwsS3Global()
{GUCEF_TRACE;

    try
    {       
        m_bucketInventoryRefreshTimer.SetEnabled( false );

        VFS::CVfsGlobal::Instance()->GetVfs().UnregisterArchiveFactory( AwsS3ArchiveType );

        delete m_s3Client;
        m_s3Client = GUCEF_NULL;
        
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "AwsS3Global systems shutdown completed" );
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, CORE::CString( "AwsS3Global systems experienced an exception during shutdown: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsS3Global systems experienced an unknown exception during shutdown" );
    }
}

/*-------------------------------------------------------------------------*/

Aws::S3::S3Client& 
CAwsS3Global::GetS3Client( void )
{GUCEF_TRACE;

    return *m_s3Client;    
}

/*-------------------------------------------------------------------------*/

void
CAwsS3Global::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CAwsS3Global::OnBucketInventoryRefreshTimerCycle );
    SubscribeTo( &m_bucketInventoryRefreshTimer ,
                 CORE::CTimer::TimerUpdateEvent ,
                 callback                       );

    TEventCallback callback2( this, &CAwsS3Global::OnAwsSdkInitialized );
    SubscribeTo( PLUGINGLUE::AWSSDK::CAwsSdkGlobal::Instance()             ,
                 PLUGINGLUE::AWSSDK::CAwsSdkGlobal::AwsSdkInitializedEvent ,
                 callback2                                                 );
}

/*-------------------------------------------------------------------------*/

void
CAwsS3Global::OnAwsSdkInitialized( CORE::CNotifier* notifier    ,
                                   const CORE::CEvent& eventId  ,
                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    try
    {
        // Now that the SDK is initialized properly we can init the S3 client which depends on it
        PLUGINGLUE::AWSSDK::CAwsSdkGlobal* sdk = PLUGINGLUE::AWSSDK::CAwsSdkGlobal::Instance();
        m_s3Client = new Aws::S3::S3Client( sdk->GetCredentialsProvider(), sdk->GetAwsClientConfig() );
    
        // Trigger auto discovery and auto mounting of S3 buckets
        m_bucketInventoryRefreshTimer.SetInterval( 600000 );
        m_bucketInventoryRefreshTimer.TriggerNow();
        m_bucketInventoryRefreshTimer.SetEnabled( true );
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, CORE::CString( "AwsS3Global:OnAwsSdkInitialized: experienced an exception: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsS3Global:OnAwsSdkInitialized: experienced an unknown exception, your application may be unstable" );
    }
}

/*-------------------------------------------------------------------------*/

void
CAwsS3Global::OnBucketInventoryRefreshTimerCycle( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // Setup for all S3 buckets available based on our credentials.
    try
    {
        auto outcome = m_s3Client->ListBuckets();
        if ( outcome.IsSuccess() )
        {
            Aws::Vector<Aws::S3::Model::Bucket> bucket_list =
                outcome.GetResult().GetBuckets();

            CORE::CString bucketList;
            VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
            for ( auto const &bucket : bucket_list )
            {
                CORE::CString bucketPath = "/aws/s3/" + bucket.GetName();
                if ( vfs.MountArchive( bucket.GetName(), bucketPath, AwsS3ArchiveType, bucketPath, true ) )
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSDK */
}; /* namespace PLUGINGLUE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
