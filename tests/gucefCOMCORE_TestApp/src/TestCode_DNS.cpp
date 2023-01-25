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

#ifndef GUCEFCORE_H
#include "gucefCORE.h"          /* GUCEF CORE library API */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefCOMCORE.h"       /* GUCEF COMCORE library API */
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;
using namespace GUCEF::MT;
using namespace GUCEF::CORE;
using namespace GUCEF::COMCORE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#if GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX || GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID
  #define DEBUGBREAK __builtin_trap()
#elif GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN
  #define DEBUGBREAK DebugBreak()
#else
  #define DEBUGBREAK
#endif

#define ERRORHERE { std::cout << "Test failed @ " << __FILE__ << "(" << __LINE__ << ")\n"; DEBUGBREAK; }
#define ASSERT_TRUE( test ) if ( !(test) ) { ERRORHERE; } 
#define ASSERT_FALSE( test ) if ( (test) ) { ERRORHERE; }

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformDNSTests( void )
{GUCEF_TRACE;

    // these tests require access to a DNS server!

    CHostAddress testDns;       
    ASSERT_TRUE( testDns.GetHostname().IsNULLOrEmpty() );
    ASSERT_TRUE( testDns.SetHostnameAndPort( "google.com", 80 ) );
    ASSERT_TRUE( testDns.GetHostname() == "google.com" );
    ASSERT_TRUE( testDns.GetPortInHostByteOrder() == 80 );
    ASSERT_TRUE( testDns.HasDnsBasedHostname() );
    ASSERT_TRUE( !testDns.IsIPv4Only() );
    //ASSERT_TRUE( testDns.IsDnsWithIPv4Only() );

    CORE::GUCEFSetEnv( "test_hostname", "google.com" );
    CORE::GUCEFSetEnv( "test_port", "80" );
    testDns.Clear();
    ASSERT_TRUE( testDns.SetHostnameAndPort( "$ENVVAR:test_hostname$:$ENVVAR:test_port$" ) );
    ASSERT_TRUE( testDns.GetHostname() == "google.com" );
    ASSERT_TRUE( testDns.GetPortInHostByteOrder() == 80 );
    ASSERT_TRUE( testDns.HasDnsBasedHostname() );  // we know we have a DNS not just an IPv4 in string form
    ASSERT_TRUE( !testDns.IsIPv4Only() );          // we know we have a DNS not just an IPv4 in string form

    CDnsCache& dnsCache = CComCoreGlobal::Instance()->GetDnsCache();
    ASSERT_TRUE( GUCEF_NULL != &dnsCache );
    CDnsCacheEntryPtr dnsCacheEntry = dnsCache.GetOrAddCacheEntryForDns( "google.com" );
    ASSERT_TRUE( !dnsCacheEntry.IsNULL() );
    ASSERT_TRUE( "google.com" == dnsCacheEntry->GetDnsName() );
    
    COMCORE::CIPv4Address::TIPv4AddressVector ipv4s;
    ASSERT_TRUE( dnsCacheEntry->GetIPv4Addresses( ipv4s ) );
    ASSERT_TRUE( !ipv4s.empty() );

}

/*-------------------------------------------------------------------------*/
