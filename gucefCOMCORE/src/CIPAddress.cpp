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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#include "CIPAddress.h"

#ifdef GUCEF_MSWIN_BUILD
  #define FD_SETSIZE 1      /* should set the size of the FD set struct to 1 for VC */
  #include <winsock2.h>
  #include <Ws2tcpip.h>
  #include <Wspiapi.h>
#else
 #ifdef GUCEF_LINUX_BUILD
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
 #endif
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

CIPAddress::CIPAddress( const UInt32 address ,
                        const UInt16 port    )
    : m_address( address ) ,
      m_port( port )
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
CIPAddress::ResolveDNS( const CORE::CString& address ,
                        const UInt16 port            )
{GUCEF_TRACE;

    if ( CORE::Check_If_IP( address.C_String() ) )
    {
        m_address = inet_addr( address.C_String() );
        if ( m_address == INADDR_NONE ) return false;
        return true;
    }
    else
    {              
        #if 1

        struct hostent* retval = gethostbyname( address.C_String() );        
        if ( retval != NULL )
        {
            GUCEF_DEBUG_LOG( 1, CORE::CString( "CIPAddress::CIPAddress() DNS resolution: gethostbyname(): full name: " ) + retval->h_name );
            char* addrStr = inet_ntoa( *( struct in_addr*)( retval->h_addr_list[0] ) );
            Int32 netaddr = inet_addr( addrStr );
            if ( netaddr >= 0 ) 
            {   
                m_address = netaddr;
                m_port = htons( port );
                return true;
            }            
        }
        return false;
        
        #else
        
        struct addrinfo* info = NULL;
        CORE::CString portString( CORE::Int32ToString( destport ) );
        int retval = getaddrinfo( destaddrstr.C_String() ,
                                  portString.C_String()  ,
                                  NULL                   ,
                                  &info                  );
        if ( retval == 0 )
        {
            struct in_addr* addr = (struct in_addr*)info->ai_addr;
            GUCEF_DEBUG_LOG( "CSocket::ConvertToIPAddress(): resolved DNS name " + destaddrstr + " to " + CORE::CString( inet_ntoa( *addr ) ) );
            resolvedDest.netaddr = inet_addr( inet_ntoa( *addr ) ); // <- does this actually work ???
            resolvedDest.port = htons( destport );
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

void
CIPAddress::SetAddress( const UInt32 address )
{GUCEF_TRACE;

    m_address = address;
    OnChange( true, false );
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