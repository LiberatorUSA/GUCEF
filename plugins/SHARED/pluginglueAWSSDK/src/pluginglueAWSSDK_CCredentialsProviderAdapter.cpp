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

#include <memory>

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h" 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "pluginglueAWSSDK_CCredentialsProviderAdapter.h"

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CCredentialsProviderAdapter::CCredentialsProviderAdapter( void )
    : Aws::Auth::AWSCredentialsProviderChain()
    , CORE::CIConfigurable()
    , m_awsAccessKeyId()
    , m_awsSecretAccessKey()
    , m_awsSessionToken()
    , m_providersToUse()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CCredentialsProviderAdapter::~CCredentialsProviderAdapter()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CCredentialsProviderAdapter::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::PLUGINGLUE::AWSSDK::CCredentialsProviderAdapter";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

bool 
CCredentialsProviderAdapter::EnableCredentialsProviders( const TStringVector& providersToUse )
{GUCEF_TRACE;

    bool addAllProviders = true;
    TStringVector::const_iterator i = providersToUse.begin();
    while ( i != providersToUse.end() )
    {
        CORE::CString providerName = (*i).Lowercase();
        if ( "environment" == providerName )
        {
            AddProvider( std::make_shared< Aws::Auth::EnvironmentAWSCredentialsProvider >() );
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CredentialsProviderAdapter:EnableCredentialsProviders: Added \"Environment\" provider" );
        }
        else
        if ( "profileconfigfile" == providerName )
        {
            AddProvider( std::make_shared< Aws::Auth::ProfileConfigFileAWSCredentialsProvider >() );
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CredentialsProviderAdapter:EnableCredentialsProviders: Added \"ProfileConfigFile\" provider" );
        }
        else
        if ( "instanceprofile" == providerName )
        {
            AddProvider( std::make_shared< Aws::Auth::InstanceProfileCredentialsProvider >() );
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CredentialsProviderAdapter:EnableCredentialsProviders: Added \"InstanceProfile\" provider" );
        }
        else
        if ( "config" == providerName )
        {
            if ( !m_awsAccessKeyId.IsNULLOrEmpty() && !m_awsSecretAccessKey.IsNULLOrEmpty() )
            {
                AddProvider( std::make_shared< Aws::Auth::SimpleAWSCredentialsProvider >( m_awsAccessKeyId, m_awsSecretAccessKey, m_awsSessionToken ) );
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CredentialsProviderAdapter:EnableCredentialsProviders: Added \"Config\" provider" );
            }
        }
        else
        {
            addAllProviders = false;
        }
        ++i;
    }
    m_providersToUse = providersToUse;
    return addAllProviders;
}

/*-------------------------------------------------------------------------*/

bool 
CCredentialsProviderAdapter::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CCredentialsProviderAdapter::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    const CORE::CDataNode* node = treeroot.Find( "AWSSDK" );
    if ( GUCEF_NULL != node )
    {
        CORE::CString value;

        // Try to load the auth values first from the config
        // These will be needed later to enable the "config" credentials provider
        value = node->GetAttributeValueOrChildValueByName( "AWS_ACCESS_KEY_ID" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_awsAccessKeyId = CORE::ResolveVars( value );
        }
        value = node->GetAttributeValueOrChildValueByName( "AWS_SECRET_ACCESS_KEY" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_awsSecretAccessKey = CORE::ResolveVars( value );
        }
        value = node->GetAttributeValueOrChildValueByName( "AWS_SESSION_TOKEN" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_awsSessionToken = CORE::ResolveVars( value );
        }

        // Now determine the order in which to add the providers
        // This is the order in which they will be attempted
        TStringVector providersToUse;
        value = node->GetAttributeValueOrChildValueByName( "AWSCredentialsProviders" );
        if ( !value.IsNULLOrEmpty() )
        {
            value = CORE::ResolveVars( value );
            providersToUse = value.ParseElements( ';', false );
        }
        else
        {
            // Fall back to hardcoded default ordering which is hopefully good enough
            providersToUse.push_back( "environment" );
            providersToUse.push_back( "config" );
            providersToUse.push_back( "profileconfigfile" );
            providersToUse.push_back( "instanceprofile" );

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CredentialsProviderAdapter:LoadConfig: No provider ordering was configured, falling back to hardcoded default" );
        }

        if ( !EnableCredentialsProviders( providersToUse ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CredentialsProviderAdapter:LoadConfig: Failed to enable all credential providers. Your application might not be able to authenticate!" );
        }
    }
    return true;
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

