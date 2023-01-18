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

#include "gucefCOMCORE_CDnsResolver.h"

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
    
    CORE::CString dnsToResolve = CORE::ResolveVars( dns );

    if ( CORE::Check_If_IPv4( dnsToResolve.C_String() ) )
    {
        // No DNS resolution is needed
        
        CIPv4Address ipv4Entry( dnsToResolve, portInHostByteOrder );
        if ( ipv4Entry.GetAddress() != INADDR_NONE )    
        {
            ipv4.push_back( ipv4Entry );
            return true;
        }
        return false;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CDnsResolver: Resolving DNS name: " + dnsToResolve );

    /*
        @TODO:
        If the host specified in the name parameter has both IPv4 and IPv6 addresses, only the IPv4 addresses will be returned. 
        The gethostbyname function can only return IPv4 addresses for the name parameter. 
        The getaddrinfo function and associated addrinfo structure should be used if IPv6 addresses for the host are required or if both IPv4 and IPv6 addresses for the host are required.
    */
    
    int errorCode = 0;
    struct hostent* retval = dvsocket_gethostbyname( dnsToResolve.C_String(), &errorCode );
    if ( retval != GUCEF_NULL )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, CORE::CString( "CDnsResolver: DNS resolution: gethostbyname(): full name: " ) + retval->h_name );

        if ( GUCEF_NULL != retval->h_aliases )
        {
            for ( char** alias = retval->h_aliases; *alias != 0; alias++ ) 
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDnsResolver: DNS resolution: Alternate name: " + CORE::CString( *alias ) );
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

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDnsResolver: DNS resolution: IPv4: " + ipv4Addr.AddressAndPortAsString() );
                ipv4.push_back( ipv4Addr );
                ++i;
            }

            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CDnsResolver: Could not obtain IPv4 result structure" );
            return false;
        }
    }
    else
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDnsResolver: Failed to resolve DNS name: " + dns + " ( " + dnsToResolve + " )- ErrorCode: " + CORE::Int32ToString( errorCode ) );
        return false;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
