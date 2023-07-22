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

	char* counterStrs = (char*) malloc( nrOfCounterStats * 128 );
	int32_t* counters1 = (int32_t*) malloc( nrOfCounterStats * 4 );
	int64_t* counters2 = (int64_t*) malloc( nrOfCounterStats * 8 );
	int32_t* counters3 = (int32_t*) calloc( nrOfCounterStats, 4 );

	if ( GUCEF_NULL == counterStrs ||
	     GUCEF_NULL == counters1   ||
		 GUCEF_NULL == counters2   ||
		 GUCEF_NULL == counters3    )
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DBLNetworkInterface: Failed to allocate memory to store counter stats for board " + CORE::ToString( nicIndex ) );
		dblwrapper_mal_close( deviceContext );

		if ( GUCEF_NULL != counterStrs )
			free( counterStrs );
		if ( GUCEF_NULL != counters1 )
			free( counters1 );
		if ( GUCEF_NULL != counters2 )
			free( counters2 );
		if ( GUCEF_NULL != counters3 )
			free( counters3 );

		return false;
	}

 	// Query the counter strings
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





	// @TODO




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
