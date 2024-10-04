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

#include "gucefCOMCORE_CIPv6Info.h"  

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

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

CIPv6Info::CIPv6Info( void )
    : ip()
    , subnet()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPv6Info::CIPv6Info( const CString& ipAsString, const CString& subnetAsString, bool resolveDns )
    : ip()
    , subnet()
{GUCEF_TRACE;

    if ( resolveDns )
    {
        CORE::CString::StringVector aliases;
        CIPv4Address::TIPv4AddressVector ipv4s;
        CIPv6Address::TIPv6AddressVector ipv6s;

        CDnsResolver::Resolve( ipAsString, 0, aliases, ipv4s, ipv6s );        
        if ( !ipv6s.empty() )
            ip = ipv6s[ 0 ];
        
        aliases.clear();
        ipv4s.clear();
        ipv6s.clear();
        CDnsResolver::Resolve( subnetAsString, 0, aliases, ipv4s, ipv6s );        
        if ( !ipv6s.empty() )
            subnet = ipv6s[ 0 ];
    }
    else
    {
        ip.FromIPv6String( ipAsString );
        subnet.FromIPv6String( subnetAsString );
    }
}

/*-------------------------------------------------------------------------*/

CIPv6Info::CIPv6Info( const CIPv6Info& src )
    : ip( src.ip )
    , subnet( src.subnet )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPv6Info::~CIPv6Info()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPv6Info& 
CIPv6Info::operator=( const CIPv6Info& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        ip = src.ip;
        subnet = src.subnet;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CIPv6Info::operator<( const CIPv6Info& other ) const
{GUCEF_TRACE;

    return ( ip < other.ip ) && ( subnet < other.subnet );
}

/*-------------------------------------------------------------------------*/

void
CIPv6Info::Clear( void )
{GUCEF_TRACE;

    ip.Clear();
    subnet.Clear();
}

/*-------------------------------------------------------------------------*/

bool
CIPv6Info::TrySetFromCIDRNotationString( const CString& cidrIpv6Str )
{GUCEF_TRACE;

// @TODO: implement
    
    Clear();
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
