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

	if ( false )
	{

	}
	else
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CDBLNetworkInterface: Failed to obtain adapter stats using Win32 GetIfEntry()" );
	}
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
