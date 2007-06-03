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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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
                        const CORE::CString& port    )
    : m_address( CORE::StringToUInt32( address ) )  ,
      m_port( CORE::StringToUInt16( port ) )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPAddress::~CIPAddress()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CIPAddress::SetPort( const UInt16 port )
{GUCEF_TRACE;

    m_port = port;
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
}

/*-------------------------------------------------------------------------*/

void
CIPAddress::SetAddress( const UInt32 address )
{GUCEF_TRACE;

    m_address = address;
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
    
CORE::CString
CIPAddress::AddressAsString( void ) const
{GUCEF_TRACE;

    UInt32 address = ntohl( m_address );
    return CORE::UInt32ToString( address );
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