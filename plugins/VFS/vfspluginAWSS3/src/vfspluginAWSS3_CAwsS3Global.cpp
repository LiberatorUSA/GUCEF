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
        PLUGINGLUE::AWSSDK::CAwsSdkGlobal* sdk = PLUGINGLUE::AWSSDK::CAwsSdkGlobal::Instance();

        m_s3Client = new Aws::S3::S3Client( sdk->GetAwsCredentialsProvider(), sdk->GetAwsClientConfig() );
        
        VFS::CVfsGlobal::Instance()->GetVfs().RegisterArchiveFactory( "aws::s3", m_awsS3ArchiveFactory );


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

}

/*-------------------------------------------------------------------------*/

CAwsS3Global::~CAwsS3Global()
{GUCEF_TRACE;

    try
    {       
        VFS::CVfsGlobal::Instance()->GetVfs().UnregisterArchiveFactory( "aws::s3" );

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSDK */
}; /* namespace PLUGINGLUE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
