/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

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
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CStatsDClient::CStatsDClient( CORE::CPulseGenerator& pulseGenerator )
    : CORE::CIMetricsSystemClient() 
    , m_udpSender( pulseGenerator, false )
{GUCEF_TRACE;
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
CStatsDClient::Gauge( const CString& key, const UInt32 value, const Float32 frequency ) const
{GUCEF_TRACE;
    
    static const CString statTypeName = "g";
    Transmit( key, value, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void
CStatsDClient::Timing( const CString& key, const UInt32 ms, const Float32 frequency ) const
{GUCEF_TRACE;
    
    static const CString statTypeName = "ms";
    Transmit( key, ms, statTypeName, frequency );
}

/*-------------------------------------------------------------------------*/

void 
CStatsDClient::Transmit( const CString& key      , 
                         const Int32 value       , 
                         const CString& type     , 
                         const Float32 frequency ) const
{GUCEF_TRACE;

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
    if ( isFrequencyOne( frequency ) ) 
    {
        // Sampling rate is 1.0f, no need to specify it
        std::snprintf( buffer, sizeof(buffer), "%s%s:%d|%s", m_statNamePrefix.C_String(), key.C_String(), value, type.C_String() );
    } 
    else 
    {
        // Sampling rate is different from 1.0f, hence specify it
        std::snprintf( buffer, sizeof(buffer), "%s%s:%d|%s|@%.2f", m_statNamePrefix.C_String(), key.C_String(), value, type.C_String(), frequency );
    }

    // Send the message via the UDP sender
    m_udpSender.SendPacketTo( m_statsDestination, buffer, sizeof( buffer ) );
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

        return true;
    }
    return true;
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
