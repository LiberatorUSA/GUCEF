/*
 *  gucefCOM: GUCEF module providing communication implementations
 *  for standardized protocols
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

#include <inttypes.h>
#include <cmath>

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#include "gucefCOM_CStatsDClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CStatsDClient::CStatsDClient( void )
    : CORE::CIMetricsSystemClient()
    , m_udpSender( false )
    , m_statsDestination()
    , m_statsInterface()
    , m_statNamePrefix()
    , m_transmit( false )
{GUCEF_TRACE;

    m_statsDestination.SetPortInHostByteOrder( 8125 );
}

/*-------------------------------------------------------------------------*/

CStatsDClient::CStatsDClient( CORE::CPulseGenerator& pulseGenerator )
    : CORE::CIMetricsSystemClient()
    , m_udpSender( pulseGenerator, false )
    , m_statsDestination()
    , m_statsInterface()
    , m_statNamePrefix()
    , m_transmit( false )
{GUCEF_TRACE;

    m_statsDestination.SetPortInHostByteOrder( 8125 );
}

/*-------------------------------------------------------------------------*/

CStatsDClient::~CStatsDClient()
{GUCEF_TRACE;

    m_udpSender.Close();
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Increment( const CString& key, const Float32 frequency ) const
{GUCEF_TRACE;

    Count( key, 1, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Decrement( const CString& key, const Float32 frequency ) const
{GUCEF_TRACE;

    Count( key, -1, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Count( const CString& key, const Int32 delta, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "c";
    Transmit( key, delta, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Count( const CString& key, const Int64 delta, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "c";
    Transmit( key, delta, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Count( const CString& key, const UInt32 delta, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "c";
    Transmit( key, (Int32)delta, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Count( const CString& key, const UInt64 delta, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "c";
    Transmit( key, (Int64)delta, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Gauge( const CString& key, const UInt32 value, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "g";
    Transmit( key, (Int64) value, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Gauge( const CString& key, const UInt64 value, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "g";
    Transmit( key, (Int64) value, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Gauge( const CString& key, const Float32 value, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "g";
    Transmit( key, (Int64) value, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Gauge( const CString& key, const Float64 value, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "g";
    Transmit( key, (Int64) value, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Timing( const CString& key, const UInt32 ms, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "ms";
    Transmit( key, (Int64) ms, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Timing( const CString& key, const UInt64 ms, const Float32 frequency ) const
{GUCEF_TRACE;

    static const CString statTypeName = "ms";
    Transmit( key, (Int64) ms, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

// @TODO: Improve this to better handle different value types
void
CStatsDClient::Transmit( const CString& key      ,
                         const Int64 value       ,
                         const CString& type     ,
                         const Float32 frequency ) const
{GUCEF_TRACE;

    if ( !m_transmit )
        return;

    if ( !m_udpSender.IsActive() )
    {
        CStatsDClient* thisObj = const_cast< CStatsDClient* >( this );
        if ( !thisObj->Open() )
            return;
    }

    const auto isFrequencyOne = []( const Float32 frequency ) noexcept
    {
        constexpr Float32 epsilon{ 0.0001f };
        return std::fabs( frequency - 1.0f ) < epsilon;
    };

    // Test if one should send or not, according to the frequency rate
    if ( !isFrequencyOne( frequency ) )
    {
        if ( frequency < static_cast<float>( std::rand() ) / RAND_MAX )
        {
            return;
        }
    }

    // Prepare the buffer, with a sampling rate if specified different from 1.0f
    char buffer[256];
    int msgSize=0;
    if ( isFrequencyOne( frequency ) )
    {
        // Sampling rate is 1.0f, no need to specify it
        msgSize = std::snprintf( buffer, sizeof(buffer), "%s%s:%" PRIu64 "|%s", m_statNamePrefix.C_String(), key.C_String(), value, type.C_String() );
    }
    else
    {
        // Sampling rate is different from 1.0f, hence specify it
        msgSize = std::snprintf( buffer, sizeof(buffer), "%s%s:%" PRIu64 "|%s|@%.2f", m_statNamePrefix.C_String(), key.C_String(), value, type.C_String(), frequency );
    }

    // Send the message via the UDP sender
    if ( msgSize > 0 )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, CORE::CString( "StatsDClient:Transmit: " ) + buffer );
        m_udpSender.SendPacketTo( m_statsDestination, buffer, (UInt16) msgSize );
    }
}

/*-------------------------------------------------------------------------*/

bool
CStatsDClient::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    CORE::CDataNode* node = tree.Find( "StatsDClient" );
    if ( GUCEF_NULL == node )
    {
        node = tree.AddChild( "StatsDClient" );
        if ( GUCEF_NULL == node )
            return false;
    }

    node->SetAttribute( "StatsDestination", m_statsDestination.AddressAndPortAsString() );
    node->SetAttribute( "StatsNamePrefix", m_statNamePrefix );
    node->SetAttribute( "StatsInterface", m_statsInterface.AddressAndPortAsString() );
    node->SetAttribute( "Transmit", CORE::BoolToString( m_transmit ) );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStatsDClient::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    const CORE::CDataNode* node = treeroot.Find( "StatsDClient" );
    if ( GUCEF_NULL != node )
    {
        // First load the mandatory settings
        CORE::CString value;
        value = node->GetAttributeValueOrChildValueByName( "StatsDestination" );
        if ( !value.IsNULLOrEmpty() )
        {
            value = CORE::ResolveVars( value );
            m_statsDestination.SetHostnameAndPort( value );
        }
        else
            return false;

        // Load extra settings for which the default can also be good enough
        value = node->GetAttributeValueOrChildValueByName( "StatsNamePrefix" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_statNamePrefix = CORE::ResolveVars( value );
        }
        value = node->GetAttributeValueOrChildValueByName( "StatsInterface" );
        if ( !value.IsNULLOrEmpty() )
        {
            value = CORE::ResolveVars( value );
            m_statsInterface.SetHostnameAndPort( value );
        }
        value = node->GetAttributeValueOrChildValueByName( "Transmit" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_transmit = CORE::StringToBool( value );
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CStatsDClient::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::COM::CStatsDClient";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

bool
CStatsDClient::Open( void )
{GUCEF_TRACE;

    if ( m_udpSender.IsActive() )
        m_udpSender.Close();

    if ( m_udpSender.Open( m_statsInterface ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StatsDClient:Open(): Successfully opened socket at " + m_statsInterface.AddressAndPortAsString() );
        return true;
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StatsDClient:Open(): Failed to open socket at " + m_statsInterface.AddressAndPortAsString() );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CStatsDClient::Close( void )
{GUCEF_TRACE;

    m_udpSender.Close();
    return true;
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::SetStatsDestination( const COMCORE::CHostAddress& dest )
{GUCEF_TRACE;

    m_statsDestination = dest;
}

/*-------------------------------------------------------------------------*/

const COMCORE::CHostAddress&
CStatsDClient::GetStatsDestination( void ) const
{GUCEF_TRACE;

    return m_statsDestination;
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::SetStatsInterface( const COMCORE::CHostAddress& interface )
{GUCEF_TRACE;

    m_statsInterface = interface;
}

/*-------------------------------------------------------------------------*/

const COMCORE::CHostAddress&
CStatsDClient::GetStatsInterface( void ) const
{GUCEF_TRACE;

    return m_statsInterface;
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::SetStatNamePrefix( const CString& prefix )
{GUCEF_TRACE;

    m_statNamePrefix = prefix;
}

/*-------------------------------------------------------------------------*/

const CString&
CStatsDClient::GetStatNamePrefix( void ) const
{GUCEF_TRACE;

    return m_statNamePrefix;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
