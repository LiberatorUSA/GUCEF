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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "pluginglueAWSSDK_CClientConfiguration.h"

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/
        
CClientConfiguration::CClientConfiguration( void )
    : Aws::Client::ClientConfiguration()
    , CORE::CIConfigurable()
    , m_settingsPrefix()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CClientConfiguration::CClientConfiguration( const CORE::CString& settingsPrefix )
    : Aws::Client::ClientConfiguration()
    , CORE::CIConfigurable()
    , m_settingsPrefix( settingsPrefix )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CClientConfiguration::CClientConfiguration( const CClientConfiguration& src )
    : Aws::Client::ClientConfiguration( src )
    , CORE::CIConfigurable( src )
    , m_settingsPrefix( src.m_settingsPrefix )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CClientConfiguration::~CClientConfiguration()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CClientConfiguration& 
CClientConfiguration::operator=( const CClientConfiguration& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        Aws::Client::ClientConfiguration::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CClientConfiguration::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    // not implemented
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CClientConfiguration::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    userAgent = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "userAgent" ).AsUtf8String( userAgent );
    scheme = (Aws::Http::Scheme) CORE::StringToInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "userAgent", CORE::Int32ToString( (CORE::Int32) scheme ) ) );
    region = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "region" ).AsUtf8String( region );
    useDualStack = CORE::StringToBool( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "useDualStack", CORE::BoolToString( useDualStack ) ) );
    maxConnections = CORE::StringToUInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "maxConnections", CORE::UInt32ToString( maxConnections ) ) );
    httpRequestTimeoutMs = CORE::StringToInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "httpRequestTimeoutMs", CORE::Int32ToString( httpRequestTimeoutMs ) ) );
    requestTimeoutMs = CORE::StringToInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "requestTimeoutMs", CORE::Int32ToString( requestTimeoutMs ) ) );
    connectTimeoutMs = CORE::StringToInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "connectTimeoutMs", CORE::Int32ToString( connectTimeoutMs ) ) );
    enableTcpKeepAlive = CORE::StringToBool( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "enableTcpKeepAlive", CORE::BoolToString( enableTcpKeepAlive ) ) );
    tcpKeepAliveIntervalMs = CORE::StringToUInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "tcpKeepAliveIntervalMs", CORE::UInt32ToString( tcpKeepAliveIntervalMs ) ) );
    lowSpeedLimit = CORE::StringToUInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "lowSpeedLimit", CORE::UInt32ToString( lowSpeedLimit ) ) );
    endpointOverride = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "endpointOverride", endpointOverride ).AsUtf8String();
    proxyScheme = (Aws::Http::Scheme) CORE::StringToInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyScheme",  CORE::Int32ToString( (CORE::Int32) proxyScheme ) ) );
    proxyHost = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyHost" ).AsUtf8String( proxyHost );
    proxyPort = CORE::StringToUInt32( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyPort", CORE::UInt32ToString( proxyPort ) ) );
    proxyUserName = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyUserName" ).AsUtf8String( proxyUserName );
    proxyPassword = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyPassword" ).AsUtf8String( proxyPassword );
    proxySSLCertPath = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLCertPath" ).AsUtf8String( proxySSLCertPath );
    proxySSLCertType = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLCertType" ).AsUtf8String( proxySSLCertType );
    proxySSLKeyPath = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLKeyPath" ).AsUtf8String( proxySSLKeyPath );
    proxySSLKeyType = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLKeyType" ).AsUtf8String( proxySSLKeyType );
    proxySSLKeyPassword = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLKeyPassword" ).AsUtf8String( proxySSLKeyPassword );
    verifySSL = CORE::StringToBool( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "verifySSL", CORE::BoolToString( verifySSL ) ) );
    caPath = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "caPath", caPath ).AsUtf8String();
    caFile = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "caFile", caFile ).AsUtf8String();
    followRedirects = CORE::StringToBool( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "followRedirects", CORE::BoolToString( followRedirects ) ) );
    disableExpectHeader = CORE::StringToBool( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "disableExpectHeader", CORE::BoolToString( disableExpectHeader ) ) );
    enableClockSkewAdjustment = CORE::StringToBool( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "enableClockSkewAdjustment", CORE::BoolToString( enableClockSkewAdjustment ) ) );
    enableHostPrefixInjection = CORE::StringToBool( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "enableHostPrefixInjection", CORE::BoolToString( enableHostPrefixInjection ) ) );
    enableEndpointDiscovery = CORE::StringToBool( treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "enableEndpointDiscovery", CORE::BoolToString( enableEndpointDiscovery ) ) );
    profileName = treeroot.GetAttributeValueOrChildValueByName( m_settingsPrefix + "profileName", profileName ).AsUtf8String();
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CClientConfiguration::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::PLUGINGLUE::AWSSDK::CClientConfiguration";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void 
CClientConfiguration::SetSettingsPrefix( const CORE::CString& prefix )
{GUCEF_TRACE;

    m_settingsPrefix = prefix;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CClientConfiguration::GetSettingsPrefix( void ) const
{GUCEF_TRACE;

    return m_settingsPrefix;
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
