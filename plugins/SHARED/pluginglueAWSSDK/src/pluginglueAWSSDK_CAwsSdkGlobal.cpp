/*
 *  pluginglueAWSSDK: Library to support multiple AWS SDK based plugins that share overlap
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

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_PLUGINGLUE_AWSSDK_CLOGGINGADAPTER_H
#include "pluginglueAWSSDK_CLoggingAdapter.h"
#define GUCEF_PLUGINGLUE_AWSSDK_CLOGGINGADAPTER_H
#endif /* GUCEF_PLUGINGLUE_AWSSDK_CLOGGINGADAPTER_H ? */

#include "pluginglueAWSSDK_CAwsSdkGlobal.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PLUGINGLUE {
namespace AWSSDK {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex CAwsSdkGlobal::g_dataLock;
CAwsSdkGlobal* CAwsSdkGlobal::g_instance = GUCEF_NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CAwsSdkGlobal*
CAwsSdkGlobal::Instance()
{GUCEF_TRACE;

    if ( GUCEF_NULL == g_instance )
    {
        g_dataLock.Lock();
        if ( GUCEF_NULL == g_instance )
        {
            g_instance = new CAwsSdkGlobal();
            g_instance->Initialize();
        }
        g_dataLock.Unlock();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CAwsSdkGlobal::Deinstance( void )
{GUCEF_TRACE;

    g_dataLock.Lock();
    delete g_instance;
    g_instance = GUCEF_NULL;
    g_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CAwsSdkGlobal::Initialize( void )
{GUCEF_TRACE;

    try
    {
        m_awsSdkOptions.loggingOptions.logger_create_fn = [ ] { return std::make_shared< CLoggingAdapter >(); };
    
        // @TODO: Hook up to GUCEF Metrics
        //m_awsSdkOptions.monitoringOptions.customizedMonitoringFactory_create_fn

        m_credsProvider = std::make_shared< CCredentialsProviderAdapter >();
    
        Aws::InitAPI( m_awsSdkOptions );

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "AwsSdkGlobal systems initialized" );
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, CORE::CString( "AwsSdkGlobal systems experienced an exception during initialization: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsSdkGlobal systems experienced an unknown exception during initialization, your application may be unstable" );
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CAwsSdkGlobal::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::PLUGINGLUE::AWSSDK::CAwsSdkGlobal";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSdkGlobal::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSdkGlobal::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    if ( m_credsProvider )
        m_credsProvider->LoadConfig( treeroot );

    return true;
}

/*-------------------------------------------------------------------------*/

CAwsSdkGlobal::CAwsSdkGlobal( void )
    : CORE::CIConfigurable( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAwsSdkGlobal::~CAwsSdkGlobal()
{GUCEF_TRACE;

    try
    {
        Aws::ShutdownAPI( m_awsSdkOptions );
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "AwsSdkGlobal systems shutdown completed" );
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, CORE::CString( "AwsSdkGlobal systems experienced an exception during shutdown: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsSdkGlobal systems experienced an unknown exception during shutdown" );
    }
}

/*-------------------------------------------------------------------------*/

Aws::SDKOptions& 
CAwsSdkGlobal::GetAwsSdkOptions( void )
{GUCEF_TRACE;

    return m_awsSdkOptions;    
}

/*-------------------------------------------------------------------------*/

CAwsSdkGlobal::CredentialsProviderPtr  
CAwsSdkGlobal::GetCredentialsProvider( void )
{GUCEF_TRACE;

    return m_credsProvider;   
}

/*-------------------------------------------------------------------------*/

const Aws::Client::ClientConfiguration& 
CAwsSdkGlobal::GetAwsClientConfig( void )
{GUCEF_TRACE;

    static  Aws::Client::ClientConfiguration a;
    return a;
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
