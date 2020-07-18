/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#include <string.h>

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

#ifndef GUCEF_COMCORE_DVSOCKET_H
#include "dvwinsock.h"
#define GUCEF_COMCORE_DVSOCKET_H
#endif /* GUCEF_COMCORE_DVSOCKET_H ? */

#include "CIPAddress.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

  #undef FD_SETSIZE
  #define FD_SETSIZE 1      /* should set the size of the FD set struct to 1 for VC */
  #include <winsock2.h>
  #include <Ws2tcpip.h>
  #include <Wspiapi.h>

#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

#else

    #error Unsupported platform

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CIPAddress CIPAddress::LoopbackIP = CIPAddress( "127.0.0.1", 0 );

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CIPAddress::CIPAddress( void )
    : m_address( 0 ) ,
      m_port( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPAddress::CIPAddress( const CIPAddress& src )
    : m_address( src.m_address ) ,
      m_port( src.m_port )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPAddress::CIPAddress( const UInt32 addressInNetworkByteOrder ,
                        const UInt16 portInNetworkByteOrder    )
    : m_address( addressInNetworkByteOrder ) ,
      m_port( portInNetworkByteOrder )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPAddress::CIPAddress( const CORE::CString& address ,
                        const UInt16 port            )
    : m_address( 0 )          ,
      m_port( htons( port ) )
{GUCEF_TRACE;

    ResolveDNS( address, port );
}

/*-------------------------------------------------------------------------*/

CIPAddress::~CIPAddress()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CIPAddress::OnChange( const bool addressChanged ,
                      const bool portChanged    )
{GUCEF_TRACE;

    // Dummy to avoid mandatory implementation by descending classes
}

/*-------------------------------------------------------------------------*/

bool
CIPAddress::ResolveDNS( const CORE::CString& address     ,
                        const UInt16 portInHostByteOrder )
{GUCEF_TRACE;

    if ( address.IsNULLOrEmpty() ) 
        return false;
    
    if ( CORE::Check_If_IP( address.C_String() ) )
    {
        m_address = inet_addr( address.C_String() );
        m_port = htons( portInHostByteOrder );
        if ( m_address == INADDR_NONE ) 
            return false;
        return true;
    }
    else
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CIPAddress: Resolving DNS name: " + address );

        #if 1

        int errorCode;
        struct hostent* retval = dvsocket_gethostbyname( address.C_String(), &errorCode );
        if ( retval != NULL )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, CORE::CString( "CIPAddress::CIPAddress() DNS resolution: gethostbyname(): full name: " ) + retval->h_name );
            char* addrStr = inet_ntoa( *( struct ::in_addr*)( retval->h_addr_list[0] ) );
            UInt32 netaddr = inet_addr( addrStr );
            if ( netaddr >= 0 )
            {
                m_address = netaddr;
                m_port = htons( portInHostByteOrder );
                return true;
            }
        }

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CIPAddress: Failed to resolve DNS name: " + address + " - ErrorCode: " + CORE::Int32ToString( errorCode ) );
        return false;

        #else

        /* Alternate method */

        struct addrinfo* info = NULL;
        CORE::CString portString( CORE::Int32ToString( portInHostByteOrder ) );
        int retval = getaddrinfo( destaddrstr.C_String() ,
                                  portString.C_String()  ,
                                  NULL                   ,
                                  &info                  );
        if ( retval == 0 )
        {
            struct in_addr* addr = (struct in_addr*)info->ai_addr;
            GUCEF_DEBUG_LOG( "CSocket::ConvertToIPAddress(): resolved DNS name " + destaddrstr + " to " + CORE::CString( inet_ntoa( *addr ) ) );
            resolvedDest.netaddr = inet_addr( inet_ntoa( *addr ) ); // <- does this actually work ???
            resolvedDest.port = htons( portInHostByteOrder );
            return true;
        }
        #ifdef GUCEF_COMCORE_DEBUG_MODE
        GUCEF_DEBUG_LOG( "CSocket::ConvertToIPAddress(): gethostbyname() failed with code " CORE::Int32ToString( retval ) );
        #endif
        return false;

        #endif
    }
}

/*-------------------------------------------------------------------------*/

void
CIPAddress::SetPort( const UInt16 port )
{GUCEF_TRACE;

    m_port = port;
    OnChange( false, true );
}

/*-------------------------------------------------------------------------*/

UInt16
CIPAddress::GetPort( void ) const
{GUCEF_TRACE;

    return m_port;
}

/*-------------------------------------------------------------------------*/

void
CIPAddress::SetPortInHostByteOrder( const UInt16 port )
{GUCEF_TRACE;

    m_port = htons( port );
    OnChange( false, true );
}

/*-------------------------------------------------------------------------*/

UInt16
CIPAddress::GetPortInHostByteOrder( void ) const
{GUCEF_TRACE;

    return ntohs( m_port );
}

/*-------------------------------------------------------------------------*/

void
CIPAddress::SetAddressInHostByteOrder( const UInt32 address )
{GUCEF_TRACE;

    m_address = htonl( address );
    OnChange( true, false );
}

/*-------------------------------------------------------------------------*/

bool
CIPAddress::SetAddress( const UInt32 address )
{GUCEF_TRACE;

    m_address = address;
    OnChange( true, false );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CIPAddress::SetAddress( const CString& address )
{GUCEF_TRACE;

    if ( address.IsNULLOrEmpty() ) 
        return false;
    
    if ( CORE::Check_If_IP( address.C_String() ) )
    {
        UInt32 newAddress = inet_addr( address.C_String() );
        if ( newAddress == INADDR_NONE ) 
            return false;

        if ( m_address != newAddress )
        {
            m_address = newAddress;
            OnChange( true, false );
        }
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

UInt32
CIPAddress::GetAddress( void ) const
{GUCEF_TRACE;

    return m_address;
}

/*-------------------------------------------------------------------------*/

UInt32
CIPAddress::GetAddressInHostByteOrder( void ) const
{GUCEF_TRACE;

    return ntohl( m_address );
}

/*-------------------------------------------------------------------------*/

CString
CIPAddress::AddressAsString( void ) const
{GUCEF_TRACE;

    UInt32 address = ntohl( m_address );
    UInt8* addressArray = (UInt8*) &address;

    CString addressStr =
      CORE::UInt8ToString( addressArray[ 3 ] ) + '.' +
      CORE::UInt8ToString( addressArray[ 2 ] ) + '.' +
      CORE::UInt8ToString( addressArray[ 1 ] ) + '.' +
      CORE::UInt8ToString( addressArray[ 0 ] );

    return addressStr;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIPAddress::PortAsString( void ) const
{GUCEF_TRACE;

    UInt32 port = ntohs( m_port );
    return CORE::UInt32ToString( port );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIPAddress::AddressAndPortAsString( void ) const
{GUCEF_TRACE;

    return AddressAsString() + ':' + PortAsString();
}

/*-------------------------------------------------------------------------*/

char 
CIPAddress::GetIPClass( void ) const
{GUCEF_TRACE;

    if ( m_address & 0xE ) // First octet starts with 1110
        return 'D';
    if ( m_address & 0x6 ) // First octet starts with 110
        return 'C';
    if ( m_address & 0x2 ) // First octet starts with 10
        return 'B';
    if ( !( m_address & 0x1 ) ) // First octet starts with 0
        return 'A';
    return 'E';
}

/*-------------------------------------------------------------------------*/

bool 
CIPAddress::IsMulticast( void ) const
{GUCEF_TRACE;

    // Multicast is always a class D address
    if ( m_address & 0xE ) // First octet starts with 1110
        return true;
    return false;
}

/*-------------------------------------------------------------------------*/

void
CIPAddress::Clear( void )
{GUCEF_TRACE;

    m_address = 0;
    m_port = 0;
}

/*-------------------------------------------------------------------------*/

CIPAddress&
CIPAddress::operator=( const CIPAddress& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_address = src.m_address;
        m_port = src.m_port;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CIPAddress::operator==( const CIPAddress& other ) const
{GUCEF_TRACE;

    return ( m_address == other.m_address ) &&
           ( m_port == other.m_port );
}

/*-------------------------------------------------------------------------*/

bool
CIPAddress::operator!=( const CIPAddress& other ) const
{GUCEF_TRACE;

    return ( m_address != other.m_address ) ||
           ( m_port != other.m_port );
}

/*-------------------------------------------------------------------------*/

bool
CIPAddress::operator<( const CIPAddress& other ) const
{GUCEF_TRACE;

    // Implemented in this manner because it appears to result in the best
    // runtime performance
    char buff[ 6 ];
    *static_cast< UInt32* >( (void*)buff ) = m_address;
    *static_cast< UInt16* >( (void*)(buff+4) ) = m_port;

    char otherBuff[ 6 ];
    *static_cast< UInt32* >( (void*)otherBuff ) = other.m_address;
    *static_cast< UInt16* >( (void*)(otherBuff+4) ) = other.m_port;

    return memcmp( buff, otherBuff, 6 ) < 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
