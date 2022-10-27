/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_SOCKETUTILS_H
#include "socketutils.h"
#define GUCEF_COMCORE_SOCKETUTILS_H
#endif /* GUCEF_COMCORE_SOCKETUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "CHostAddress.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
CDnsResolver::Resolve( const CORE::CString& dns               ,
                       const UInt16 portInHostByteOrder       ,
                       CORE::CString::StringVector& aliases   ,
                       CIPv4Address::TIPv4AddressVector& ipv4 ,
                       CIPv6Address::TIPv6AddressVector& ipv6 )
{GUCEF_TRACE;

    if ( dns.IsNULLOrEmpty() ) 
        return false;
    
    if ( CORE::Check_If_IPv4( dns.C_String() ) )
    {
        // No DNS resolution is needed
        
        CIPv4Address ipv4Entry( dns, portInHostByteOrder );
        if ( ipv4Entry.GetAddress() != INADDR_NONE )    
        {
            ipv4.push_back( ipv4Entry );
            return true;
        }
        return false;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CHostAddress: Resolving DNS name: " + dns );

    /*
        @TODO:
        If the host specified in the name parameter has both IPv4 and IPv6 addresses, only the IPv4 addresses will be returned. 
        The gethostbyname function can only return IPv4 addresses for the name parameter. 
        The getaddrinfo function and associated addrinfo structure should be used if IPv6 addresses for the host are required or if both IPv4 and IPv6 addresses for the host are required.
    */
    
    int errorCode = 0;
    struct hostent* retval = dvsocket_gethostbyname( dns.C_String(), &errorCode );
    if ( retval != GUCEF_NULL )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, CORE::CString( "CHostAddress: DNS resolution: gethostbyname(): full name: " ) + retval->h_name );

        if ( GUCEF_NULL != retval->h_aliases )
        {
            for ( char** alias = retval->h_aliases; *alias != 0; alias++ ) 
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHostAddress: DNS resolution: Alternate name: " + CORE::CString( *alias ) );
                aliases.push_back( *alias );
            }
        }

        if ( GUCEF_NULL != retval->h_addr_list )
        {
            UInt32 i=0;
            while ( retval->h_addr_list[ i ] != 0 ) 
            {
                UInt32 ipv4AddressInNetworkByteOrder = *(UInt32*) retval->h_addr_list[ i ];
                CIPv4Address ipv4Addr( ipv4AddressInNetworkByteOrder, CIPv4Address::HostByteOrderToNetworkByteOrder( portInHostByteOrder ) );

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHostAddress: DNS resolution: IPv4: " + ipv4Addr.AddressAndPortAsString() );
                ipv4.push_back( ipv4Addr );
                ++i;
            }

            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CHostAddress: Could not obtain IPv4 result structure" );
            return false;
        }
    }
    else
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CHostAddress: Failed to resolve DNS name: " + dns + " - ErrorCode: " + CORE::Int32ToString( errorCode ) );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( void )
    : m_hostname()
    , m_aliases()
    , m_ipv4()
    , m_ipv6()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( const CHostAddress& src )
    : m_hostname( src.m_hostname )
    , m_aliases( src.m_aliases )
    , m_ipv4( src.m_ipv4 )
    , m_ipv6( src.m_ipv6 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( const CIPv4Address& ipAddress ,
                            const CORE::CString& hostname )
    : m_hostname( hostname )
    , m_aliases()
    , m_ipv4()
    , m_ipv6()
{GUCEF_TRACE;

    m_ipv4.push_back( ipAddress );
}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( const CORE::CString& hostname ,
                            const UInt16 port             )
    : m_hostname( hostname )
    , m_aliases()
    , m_ipv4()
    , m_ipv6()
{GUCEF_TRACE;

    SetHostnameAndPort( hostname, port );
}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( const CORE::CString& hostAndPort )
    : m_hostname()
    , m_aliases()
    , m_ipv4()
    , m_ipv6()
{GUCEF_TRACE;

    SetHostnameAndPort( hostAndPort );
}

/*-------------------------------------------------------------------------*/

CHostAddress::~CHostAddress()
{GUCEF_TRACE;
    
    Clear();
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::SetHostnameAndPort( const CORE::CString& hostAndPort )
{GUCEF_TRACE;

    Clear();
    
    Int32 sepCharIndex = hostAndPort.HasChar( ':', false );
    if ( sepCharIndex >= 0 )
    {
        m_hostname = hostAndPort.SubstrToIndex( sepCharIndex, true );
    }
    else
    {
        m_hostname = hostAndPort;
    }

    if ( !m_hostname.IsNULLOrEmpty() )
    {
        return CDnsResolver::Resolve( m_hostname               , 
                                      GetPortInHostByteOrder() ,
                                      m_aliases                ,
                                      m_ipv4                   ,
                                      m_ipv6                   );
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::SetHostnameAndPort( const CORE::CString& host , 
                                  UInt16 portInHostOrder    )
{GUCEF_TRACE;

    Clear();
    
    if ( !host.IsNULLOrEmpty() )
    {
        Int32 sepCharIndex = host.HasChar( ':', false );
        if ( sepCharIndex >= 0 )
        {
            // we only take the dns portion not the port since a port was explicitly distinctly provided
            // as such we'd consider that an override
            m_hostname = host.SubstrToIndex( sepCharIndex, true );            
        }
        else
        {
            m_hostname = host;
        }
        
        return CDnsResolver::Resolve( m_hostname      , 
                                      portInHostOrder ,
                                      m_aliases       ,
                                      m_ipv4          ,
                                      m_ipv6          );
    }
    else
    {
        CIPv4Address ipv4;
        ipv4.SetAddress( INADDR_ANY );
        ipv4.SetPortInHostByteOrder( portInHostOrder );
        m_ipv4.push_back( ipv4 );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CHostAddress::SetPortInHostByteOrder( UInt16 portInHostOrder )
{GUCEF_TRACE;

    CIPv4Address::TIPv4AddressVector::iterator i = m_ipv4.begin();
    while ( i != m_ipv4.end() )
    {
        CIPv4Address& ipv4 = (*i);
        ipv4.SetPortInHostByteOrder( portInHostOrder );
        ++i;
    }
    CIPv6Address::TIPv6AddressVector::iterator n = m_ipv6.begin();
    while ( n != m_ipv6.end() )
    {
        CIPv6Address& ipv6 = (*n);
        //ipv6.SetPortInHostByteOrder( portInHostOrder );
        ++n;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::SetHostname( const CORE::CString& hostName )
{GUCEF_TRACE;

    UInt16 currentPort = GetPortInHostByteOrder();

    Clear();
    
    m_hostname = hostName;
    return CDnsResolver::Resolve( m_hostname  , 
                                  currentPort ,
                                  m_aliases   ,
                                  m_ipv4      ,
                                  m_ipv6      );
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CHostAddress::GetHostname( void ) const
{GUCEF_TRACE;

    return m_hostname;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::Refresh( void )
{GUCEF_TRACE;

    UInt16 currentPort = GetPortInHostByteOrder();

    m_aliases.clear();
    m_ipv4.clear();
    m_ipv6.clear();

    return CDnsResolver::Resolve( m_hostname  , 
                                  currentPort ,
                                  m_aliases   ,
                                  m_ipv4      ,
                                  m_ipv6      );
}

/*-------------------------------------------------------------------------*/

UInt16
CHostAddress::GetPortInHostByteOrder( void ) const
{GUCEF_TRACE;

    if ( !m_ipv4.empty() )
    {
        return m_ipv4.front().GetPortInHostByteOrder();
    }
    if ( !m_ipv6.empty() )
    {
        //return m_ipv6.front().GetPortInHostByteOrder();
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CHostAddress::PortAsString( void ) const
{GUCEF_TRACE;

    if ( !m_ipv4.empty() )
    {
        return m_ipv4.front().PortAsString();
    }
    if ( !m_ipv6.empty() )
    {
        //return m_ipv6.front().PortAsString();
    }
    return CORE::CString::Empty;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CHostAddress::HostnameAndPortAsString( void ) const
{GUCEF_TRACE;

    return m_hostname + ':' + PortAsString();
}

/*-------------------------------------------------------------------------*/

void
CHostAddress::Clear( void )
{GUCEF_TRACE;

    m_hostname.Clear();
    m_aliases.clear();
    m_ipv4.clear();
    m_ipv6.clear();
}

/*-------------------------------------------------------------------------*/

CHostAddress&
CHostAddress::operator=( const CHostAddress& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_hostname = src.m_hostname;
        m_aliases = src.m_aliases;
        m_ipv4 = m_ipv4;
        m_ipv6 = m_ipv6;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CHostAddress&
CHostAddress::operator=( const CIPv4Address& src )
{GUCEF_TRACE;

    Clear();
    
    m_hostname = src.AddressAsString();
    m_ipv4.push_back( src );

    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::operator==( const CHostAddress& other ) const
{GUCEF_TRACE;

    return m_hostname == other.m_hostname;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::operator!=( const CHostAddress& other ) const
{GUCEF_TRACE;

    return m_hostname != other.m_hostname;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::operator<( const CHostAddress& other ) const
{GUCEF_TRACE;

    return m_hostname < other.m_hostname;
}

/*-------------------------------------------------------------------------*/

CIPv4Address::TIPv4AddressVector& 
CHostAddress::GetIPv4Addresses( void )
{GUCEF_TRACE;

    return m_ipv4;
}

/*-------------------------------------------------------------------------*/

const CIPv4Address::TIPv4AddressVector& 
CHostAddress::GetIPv4Addresses( void ) const
{GUCEF_TRACE;

    return m_ipv4;
}

/*-------------------------------------------------------------------------*/

const CIPv4Address& 
CHostAddress::GetRandomIPv4Address( void ) const
{GUCEF_TRACE;

    if ( !m_ipv4.empty() )
    {
        float fractionalIndex = ( static_cast< float >( std::rand() ) / RAND_MAX ) * static_cast< float >( m_ipv4.size() );
        UInt32 index = static_cast< UInt32 >( round( fractionalIndex ) );

        return m_ipv4[ index ];
    }
    return CIPv4Address::None;
}

/*-------------------------------------------------------------------------*/

const CIPv4Address& 
CHostAddress::GetFirstIPv4Address( void ) const
{GUCEF_TRACE;

    if ( !m_ipv4.empty() )
    {
        return m_ipv4.front();
    }
    return CIPv4Address::None;
}

/*-------------------------------------------------------------------------*/

CIPv6Address::TIPv6AddressVector& 
CHostAddress::GetIPv6Addresses( void )
{GUCEF_TRACE;

    return m_ipv6;
}

/*-------------------------------------------------------------------------*/

const CIPv6Address::TIPv6AddressVector& 
CHostAddress::GetIPv6Addresses( void ) const
{GUCEF_TRACE;

    return m_ipv6;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
