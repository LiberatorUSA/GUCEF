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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( void )
    : CIPAddress() ,
      m_hostname()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( const CHostAddress& src )
    : CIPAddress( src )            ,
      m_hostname( src.m_hostname )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( const CIPAddress& ipAddress   ,
                            const CORE::CString& hostname )
    : CIPAddress( ipAddress ) ,
      m_hostname( hostname )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( const CORE::CString& hostname ,
                            const UInt16 port             )
    : CIPAddress()           ,
      m_hostname( hostname )
{GUCEF_TRACE;

    SetPortInHostByteOrder( port );
    if ( !m_hostname.IsNULLOrEmpty() )
    {
        ResolveDNS( hostname, port );
    }
    else
    {
        SetAddress( INADDR_ANY );
    }
}

/*-------------------------------------------------------------------------*/

CHostAddress::CHostAddress( const CORE::CString& hostAndPort )
    : CIPAddress() ,
      m_hostname()
{GUCEF_TRACE;

    SetHostnameAndPort( hostAndPort );
}

/*-------------------------------------------------------------------------*/

CHostAddress::~CHostAddress()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CHostAddress::OnChange( const bool addressChanged ,
                        const bool portChanged    )
{GUCEF_TRACE;

    if ( addressChanged )
    {
        m_hostname = AddressAsString();
    }
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::SetHostnameAndPort( const CORE::CString& hostAndPort )
{GUCEF_TRACE;

    Int32 sepCharIndex = hostAndPort.HasChar( ':', false );
    if ( sepCharIndex >= 0 )
    {
        m_hostname = hostAndPort.SubstrToIndex( sepCharIndex, true );
        SetPortInHostByteOrder( CORE::StringToUInt16( hostAndPort.SubstrToIndex( sepCharIndex+1, false ) ) );
    }
    else
    {
        m_hostname = hostAndPort;
    }

    if ( !m_hostname.IsNULLOrEmpty() )
    {
        return ResolveDNS( m_hostname, GetPortInHostByteOrder() );
    }
    else
    {
        SetAddress( INADDR_ANY );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::SetHostnameAndPort( const CORE::CString& host, UInt16 portInHostOrder )
{GUCEF_TRACE;

    if ( !host.IsNULLOrEmpty() )
    {
        m_hostname = host;
        return ResolveDNS( host, portInHostOrder );
    }
    else
    {
        SetAddress( INADDR_ANY );
        SetPortInHostByteOrder( portInHostOrder );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::SetHostname( const CORE::CString& hostName )
{GUCEF_TRACE;

    m_hostname = hostName;
    return ResolveDNS( hostName                 ,
                       GetPortInHostByteOrder() );
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

    return ResolveDNS( m_hostname               ,
                       GetPortInHostByteOrder() );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CHostAddress::HostnameAndPortAsString( void ) const
{GUCEF_TRACE;

    return m_hostname + ':' + PortAsString();
}

/*-------------------------------------------------------------------------*/

CHostAddress&
CHostAddress::operator=( const CHostAddress& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CIPAddress::operator=( src );

        m_hostname = src.m_hostname;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CHostAddress&
CHostAddress::operator=( const CIPAddress& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CIPAddress::operator=( src );
        m_hostname = src.AddressAsString();
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::operator==( const CHostAddress& other ) const
{GUCEF_TRACE;

    return CIPAddress::operator==( other )   &&
           ( m_hostname == other.m_hostname ) ;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::operator!=( const CHostAddress& other ) const
{GUCEF_TRACE;

    return CIPAddress::operator!=( other )   ||
           ( m_hostname != other.m_hostname ) ;
}

/*-------------------------------------------------------------------------*/

bool
CHostAddress::operator<( const CHostAddress& other ) const
{GUCEF_TRACE;

    return m_hostname < other.m_hostname;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
