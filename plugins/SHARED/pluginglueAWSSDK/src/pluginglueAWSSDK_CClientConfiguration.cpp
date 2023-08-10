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
CClientConfiguration::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    userAgent = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "userAgent" ).AsUtf8String( userAgent, true );
    scheme = (Aws::Http::Scheme) CORE::StringToInt32( cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "scheme", CORE::Int32ToString( (CORE::Int32) scheme ) ) );
    region = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "region" ).AsUtf8String( region, true );
    useDualStack = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "useDualStack", useDualStack ).AsBool( useDualStack, true );
    maxConnections = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "maxConnections", maxConnections ).AsInt32( maxConnections, true );
    httpRequestTimeoutMs = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "httpRequestTimeoutMs", httpRequestTimeoutMs ).AsInt32( httpRequestTimeoutMs, true );
    requestTimeoutMs = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "requestTimeoutMs", requestTimeoutMs ).AsInt32( requestTimeoutMs, true );
    connectTimeoutMs = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "connectTimeoutMs", connectTimeoutMs ).AsInt32( connectTimeoutMs, true );
    enableTcpKeepAlive = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "enableTcpKeepAlive", enableTcpKeepAlive ).AsBool( enableTcpKeepAlive, true );
    tcpKeepAliveIntervalMs = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "tcpKeepAliveIntervalMs", (UInt32) tcpKeepAliveIntervalMs ).AsUInt32( tcpKeepAliveIntervalMs, true );
    lowSpeedLimit = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "lowSpeedLimit", (UInt32) lowSpeedLimit ).AsUInt32( lowSpeedLimit, true );
    endpointOverride = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "endpointOverride", endpointOverride ).AsUtf8String( endpointOverride, true );
    proxyScheme = (Aws::Http::Scheme) CORE::StringToInt32( cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyScheme",  CORE::Int32ToString( (CORE::Int32) proxyScheme ) ) );
    proxyHost = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyHost", proxyHost ).AsUtf8String( proxyHost, true );
    proxyPort = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyPort", proxyPort ).AsUInt32( proxyPort, true );
    proxyUserName = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyUserName", proxyUserName ).AsUtf8String( proxyUserName, true );
    proxyPassword = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxyPassword", proxyPassword ).AsUtf8String( proxyPassword, true );
    proxySSLCertPath = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLCertPath", proxySSLCertPath ).AsUtf8String( proxySSLCertPath, true );
    proxySSLCertType = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLCertType", proxySSLCertType ).AsUtf8String( proxySSLCertType, true );
    proxySSLKeyPath = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLKeyPath", proxySSLKeyPath ).AsUtf8String( proxySSLKeyPath, true );
    proxySSLKeyType = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLKeyType", proxySSLKeyType ).AsUtf8String( proxySSLKeyType, true );
    proxySSLKeyPassword = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "proxySSLKeyPassword", proxySSLKeyPassword ).AsUtf8String( proxySSLKeyPassword, true );
    verifySSL = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "verifySSL", verifySSL ).AsBool( verifySSL, true );
    caPath = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "caPath", caPath ).AsUtf8String( caPath, true );
    caFile = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "caFile", caFile ).AsUtf8String( caFile, true );
    followRedirects = (Aws::Client::FollowRedirectsPolicy) CORE::StringToInt32( cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "followRedirects", CORE::Int32ToString( (CORE::Int32) followRedirects ) ) );
    disableExpectHeader = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "disableExpectHeader", disableExpectHeader ).AsBool( disableExpectHeader, true );
    enableClockSkewAdjustment = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "enableClockSkewAdjustment", enableClockSkewAdjustment ).AsBool( enableClockSkewAdjustment, true );
    enableHostPrefixInjection = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "enableHostPrefixInjection", enableHostPrefixInjection ).AsBool( enableHostPrefixInjection, true );
    enableEndpointDiscovery = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "enableEndpointDiscovery", enableEndpointDiscovery.has_value() ? (bool) enableEndpointDiscovery.value() : false ).
        AsBool( enableEndpointDiscovery.has_value() ? (bool) enableEndpointDiscovery.value() : false, true ); // Per AWS: "By default, endpoint discovery is off."
    profileName = cfg.GetAttributeValueOrChildValueByName( m_settingsPrefix + "profileName", profileName ).AsUtf8String( profileName, true );
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
