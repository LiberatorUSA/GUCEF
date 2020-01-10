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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_COMCORE_SOCKETUTILS_H
#include "socketutils.h"
#define GUCEF_COMCORE_SOCKETUTILS_H
#endif /* GUCEF_COMCORE_SOCKETUTILS_H ? */

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
