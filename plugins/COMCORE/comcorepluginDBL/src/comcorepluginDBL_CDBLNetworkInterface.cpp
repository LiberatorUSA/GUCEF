/*
 *  comcorepluginDBL: Generic GUCEF COMCORE plugin for providing access to the DBL network driver
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

#include "comcorepluginDBL_CDBLNetworkInterface.h"

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#ifndef COMCOREPLUGIN_DBL_DRIVERAPI_H
#include "comcorepluginDBL_driverapi.h"
#define COMCOREPLUGIN_DBL_DRIVERAPI_H
#endif /* COMCOREPLUGIN_DBL_DRIVERAPI_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCOREPLUGIN {
namespace DBL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
CDBLNetworkInterface::SetupAdapterInfo( void* pAdaptInfoVoid ) 
{


	return false;
}

/*-------------------------------------------------------------------------*/

bool 
CDBLNetworkInterface::EnumNetworkAdapters( TINetworkInterfacePtrVector& interfaces ) 
{	

	return true;
}

/*-------------------------------------------------------------------------*/

CDBLNetworkInterface::CDBLNetworkInterface( void )
    : CINetworkInterface()
	, m_dataLock()
{GUCEF_TRACE;
          
}

/*-------------------------------------------------------------------------*/
    
CDBLNetworkInterface::~CDBLNetworkInterface()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CDBLNetworkInterface::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::COMCOREPLUGIN::DBL::::CDBLNetworkInterface"; 
    return classTypeName;
}

/*-------------------------------------------------------------------------*/
    
CORE::CString
CDBLNetworkInterface::GetCommunicationInterfaceType( void ) const
{GUCEF_TRACE;

    return "network";
}

/*-------------------------------------------------------------------------*/

CORE::CString 
CDBLNetworkInterface::GetAdapterName( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return "";
}

/*-------------------------------------------------------------------------*/

CORE::CString 
CDBLNetworkInterface::GetAdapterDescription( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return "";
}

/*-------------------------------------------------------------------------*/

UInt32 
CDBLNetworkInterface::GetNrOfDnsAddresses( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return 0;
}

/*-------------------------------------------------------------------------*/
    
bool 
CDBLNetworkInterface::GetDnsAddresses( THostAddressVector& dnsAddresses )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDBLNetworkInterface::GetNrOfIPAddresses( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return 0;
}

/*-------------------------------------------------------------------------*/

bool 
CDBLNetworkInterface::GetIPInfo( TIPInfoVector& ipInfo, bool includeUninitialized ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/
		
bool 
CDBLNetworkInterface::IsDhcpUsed( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime 
CDBLNetworkInterface::GetDhcpLeaseObtainedTime( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	CORE::CDateTime result;
	return result;
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime 
CDBLNetworkInterface::GetDhcpLeaseExpirationTime( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	CORE::CDateTime result;
	return result;
}

/*-------------------------------------------------------------------------*/

bool 
CDBLNetworkInterface::ReleaseAddress( void )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

bool 
CDBLNetworkInterface::RenewAddress( void )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

bool 
CDBLNetworkInterface::IsWinsUsed( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

COMCORE::CIPv4Address
CDBLNetworkInterface::GetPrimaryWinsServer( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return COMCORE::CIPv4Address();
}

/*-------------------------------------------------------------------------*/

COMCORE::CIPv4Address 
CDBLNetworkInterface::GetSecondaryWinsServer( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return COMCORE::CIPv4Address();
}

/*-------------------------------------------------------------------------*/

UInt32 
CDBLNetworkInterface::GetOsAdapterIndex( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return 0;
}

/*-------------------------------------------------------------------------*/

bool 
CDBLNetworkInterface::GetMetrics( COMCORE::CNetworkInterfaceMetrics& metrics ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );

	// Before we can access the board we need to translate to product ids to further identify which board we
	// are referencing in our interactions
	int64_t nicIndex = 0;
	char productIdPartA = 0;
	int32_t productIdPartB = 0;
	int32_t productLookupResult = dblwrapper_mal_translate_to_product( nicIndex, &productIdPartA, &productIdPartB );
	if ( 0 != productLookupResult )
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBLNetworkInterface: Couldn't find board " + CORE::ToString( nicIndex ) );
		return false;
	}

	// Open up management access to the board
	int64_t v8 = 0;
	int32_t optionFlags = v8 & 0xFFFFFFFFFFFFFF00 | productIdPartA;
	void* deviceContext = GUCEF_NULL;
	int32_t openResult = dblwrapper_mal_open( productIdPartB, 
										      0xffffffff, 
											  &deviceContext, 
											  optionFlags, 
											  productIdPartB );
	if ( 0 != openResult )
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBLNetworkInterface: Cannot open board " + CORE::ToString( nicIndex ) );
		return false;
	}

	// Query the numeric NIC ID first as a sanity check to check our access
	int32_t driverId = productIdPartB; // <- on input we need to give the product id for the board we are asking for
	int32_t queryResult = dblwrapper_mal_ioctl( deviceContext, 
												MAL_IOCTL_INFOTYPEID_NIC, 
												&driverId, 
												MAL_IOCTL_PROPERTYID_INT32_ID );
	if ( 0 != queryResult )
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBLNetworkInterface: Failed to get driver id for board " + CORE::ToString( nicIndex ) );
		dblwrapper_mal_close( deviceContext );
		return false;
	}
	GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBLNetworkInterface: Board numeric NIC id: " + CORE::ToString( driverId ) );

	int64_t dummy = 0;
	const char* idStr = GUCEF_NULL;
	dblwrapper_mal_nic_id_to_str( &idStr, dummy, 0x12 );

	// Query how many ports the NIC has, it can have multiple ports on a single card
	int32_t	boardPortCount = productIdPartB; // <- on input we need to give the product id for the board we are asking for
	queryResult = dblwrapper_mal_ioctl( deviceContext, 
									    MAL_IOCTL_INFOTYPEID_PORTS, 
										&boardPortCount, 
										MAL_IOCTL_PROPERTYID_INT32_COUNT );
	if ( 0 != queryResult )
	{
		// we wont count this as a fatal error. log but keep going
		GUCEF_WARNING_LOG( CORE::LOGLEVEL_IMPORTANT, "DBLNetworkInterface: Cannot get port count for board " + CORE::ToString( nicIndex ) );
	}

	GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DBLNetworkInterface: Board NIC ID string is \"" + CORE::ToString( idStr ) +
		"\" with " + CORE::ToString( boardPortCount ) + " ports" );

	// Query how many counter stats are available, we will need to allocate sufficient storage for them
	int32_t nrOfCounterStats = productIdPartB; // <- on input we need to give the product id for the board we are asking for
	queryResult = dblwrapper_mal_ioctl( deviceContext, 
										MAL_IOCTL_INFOTYPEID_COUNTERS_METADATA, 
										&nrOfCounterStats, 
										MAL_IOCTL_PROPERTYID_INT32_COUNT );
	if ( 0 != queryResult )
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBLNetworkInterface: Failed to get nr of counter stats for board " + CORE::ToString( nicIndex ) );
		dblwrapper_mal_close( deviceContext );
		return false;
	}

	CORE::CDynamicBuffer counterStrsBuffer( nrOfCounterStats * 128, true );
	char* counterStrs = counterStrsBuffer.AsTypePtr< char >();
	CORE::CDynamicBuffer counters1Buffer( nrOfCounterStats * sizeof(int32_t), true );
	int32_t* counters1 = counters1Buffer.AsTypePtr< int32_t >();
	CORE::CDynamicBuffer counters2Buffer( nrOfCounterStats * sizeof(int64_t), true );
	int64_t* counters2 = counters2Buffer.AsTypePtr< int64_t >();
	CORE::CDynamicBuffer counters3Buffer( nrOfCounterStats * sizeof(int32_t), true );
	counters3Buffer.SetBytes( 0 );
	int32_t* counters3 = counters3Buffer.AsTypePtr< int32_t >();

	if ( GUCEF_NULL == counterStrs ||
	     GUCEF_NULL == counters1   ||
		 GUCEF_NULL == counters2   ||
		 GUCEF_NULL == counters3    )
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBLNetworkInterface: Failed to allocate memory to store counter stats for board " + CORE::ToString( nicIndex ) );
		dblwrapper_mal_close( deviceContext );
		return false;
	}

 	// Query the counter strings
	counterStrsBuffer.AsType< int32_t >() = productIdPartB;	// <- on input we need to give the product id for the board we are asking for
	queryResult = dblwrapper_mal_ioctl( deviceContext, 
										MAL_IOCTL_INFOTYPEID_COUNTERS_STRINGS, 
										counterStrs, 
										MAL_IOCTL_PROPERTYID_INT32_COUNT );	// <- ???
	if ( 0 != queryResult )
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBLNetworkInterface: Failed to get nr of counter stats for board " + CORE::ToString( nicIndex ) );
		dblwrapper_mal_close( deviceContext );
		return false;
	}
	GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "DBLNetworkInterface: Obtained counter strings" );

/*

	counters2Buffer.AsType< int32_t >() = productIdPartB;	// <- on input we need to give the product id for the board we are asking for
    int32_t counterQueryType = 0;
	queryResult = dblwrapper_mal_ioctl( deviceContext, 2236563, counters2, MAL_IOCTL_PROPERTYID_INT32_COUNT );
	if( (uint32_t)queryResult == 0 ) 
	{
		*v14 = productIdPartB;
		v18 = 1;
		v19 = v14;
		counterQueryType = MAL_IOCTL_INFOTYPEID_COUNTERS;
	} 
	else 
	{
		GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "DBLNetworkInterface: no classes support" );
		*v13 = productIdPartB;
		v18 = 0;
		v2 = 1;
		v19 = v13;
		counterQueryType = MAL_IOCTL_INFOTYPEID_COUNTERS_NOCLASSES;
	}


	// @TODO


        queryResult = dblwrapper_mal_ioctl( deviceContext, counterQueryType, v19, MAL_IOCTL_PROPERTYID_INT32_COUNT );
        if( (uint32_t)queryResult != 0 ) 
		{
            _ErrMsg = "Cannot get counters values";
        } 
		else 
		{
            if( local_0x90 != 0 ) 
			{
                v22 = local_0x90;
                v23 = v13;
                while( 1 ) 
				{
                    if( local_0x74 < 2 ) 
					{
                        v25 = strstr( _Str, "(Port 1)" );
                        if( v25 == 0 ) 
						{
                            goto node_244;
                        }
                    } 
					else 
					{
                        node_244:
                        if( (int32_t)v18 == 0 ) 
						{
                            if( ((int32_t)v3 == 0 || (int32_t)v18 != *v23) && ((int32_t)v2 != 0 || (uint32_t)*(v15 - v13 + v23) == 0) ) 
							{
                                v24 = ntohl( *v23 );
                                printf( "%34s: %10u\n", _Str, v24 );
                            }
                        } 
						else 
						if( ((int32_t)v3 == 0 || *v14 != 0) && ((int32_t)v2 != 0 || (uint32_t)*(v15 - v13 + v23) == 0) ) 
						{
                            printf( "%34s: %20llu\n", _Str, *v14 );
                        }
                    }
                    if( v22 == 1 ) 
					{
                        break;
                    }
                    v22 -= 1;
                    (uint8_t *)v14 += 8;
                    _Str -= -128;
                    (uint8_t *)v23 += 4;
                }
            }
            if( local_0x78 != 0 ) 
			{
                local_0x70 = local_0x98;
                queryResult = dblwrapper_mal_ioctl( deviceContext, MAL_IOCTL_INFOTYPEID_IRQ, &local_0x70, 16 );
                if( (uint32_t)queryResult == 0 ) 
				{
                    printf( "Host irq info: %10u intr, %10u events, %10u intr w/o event\n", local_0x6C, local_0x68, local_0x64 );
                } 
				else 
				{
                    perror( "Cannot get interrupt counters -- try enabling MAL_DEBUG in the kernel?" );
                }
            }
            if( local_0x80 == 0 ) 
			{
                dblwrapper_mal_close( deviceContext );
                __security_check_cookie( local_0x30 ^ &v1 );
                return 0;
            }
            queryResult = dblwrapper_mal_ioctl( deviceContext, MAL_IOCTL_INFOTYPEID_CLEAR_COUNTERS, &local_0x98, 4 );
            if( (uint32_t)queryResult == 0 ) 
			{
                dblwrapper_mal_close( local_0x88 );
                __security_check_cookie( local_0x30 ^ &v1 );
                return 0;
            }
            _ErrMsg = "Cannot clear counters";
        }

	*/

	return false;
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CDBLNetworkInterface::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CDBLNetworkInterface::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_dataLock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CDBLNetworkInterface::Unlock( void ) const
{GUCEF_TRACE;

    return m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DBL */
}; /* namespace COMCOREPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
