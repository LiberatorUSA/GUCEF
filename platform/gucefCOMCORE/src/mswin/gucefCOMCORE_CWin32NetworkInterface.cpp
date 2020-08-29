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

#include "gucefCOMCORE_CWin32NetworkInterface.h"

#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

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

#include <windows.h>
#include <tchar.h>
#include <iphlpapi.h>
#include <ipifcons.h>

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
CWin32NetworkInterface::SetupAdapterInfo( void* pAdaptInfoVoid ) 
{
	IP_ADAPTER_INFO* pAdaptInfo = (IP_ADAPTER_INFO*) pAdaptInfoVoid;
	IP_ADDR_STRING* pNext			= NULL;
	IP_PER_ADAPTER_INFO* pPerAdapt	= NULL;
	ULONG ulLen						= 0;
	ULONG errorCode					= 0;
	
	if( NULL == pAdaptInfo )
		return false;
	
	m_name = pAdaptInfo->AdapterName;		
	m_desc = pAdaptInfo->Description;

	m_priWins.SetAddress( pAdaptInfo->PrimaryWinsServer.IpAddress.String );
	m_secWins.SetAddress( pAdaptInfo->SecondaryWinsServer.IpAddress.String );
	m_nicIndex = pAdaptInfo->Index;		
	m_adapterType = pAdaptInfo->Type;	
	m_dhcpUsed = pAdaptInfo->DhcpEnabled;
	m_winsUsed = pAdaptInfo->HaveWins;	
	m_leaseObtained = pAdaptInfo->LeaseObtained;
	m_leaseExpires = pAdaptInfo->LeaseExpires;
	m_dhcpAddr.SetAddress( pAdaptInfo->DhcpServer.IpAddress.String );
		
	if( pAdaptInfo->CurrentIpAddress ) 
	{
		m_curIpAddr.ip.SetAddress( pAdaptInfo->CurrentIpAddress->IpAddress.String );
		m_curIpAddr.subnet.SetAddress( pAdaptInfo->CurrentIpAddress->IpMask.String );
	}
	else
	{
		m_curIpAddr.Clear();
	}

	// since an adapter may have more than one ip address we need
	// to populate the array we have setup with all available
	// ip addresses.
	pNext = &( pAdaptInfo->IpAddressList );
	while( pNext ) 
	{
		m_ipAddresses.push_back( CIPInfo( pNext->IpAddress.String, pNext->IpMask.String, false ) );
		pNext = pNext->Next;
	}

	// an adapter usually has just one gateway however the provision exists
	// for more than one so to "play" as nice as possible we allow for it here
	// as well.
	pNext = &( pAdaptInfo->GatewayList );
	while( pNext ) 
	{
		CIPAddress address;
		if ( address.SetAddress( pNext->IpAddress.String ) )
			m_gatewayList.push_back( address );
		pNext = pNext->Next;
	}	
			
	// we need to generate a IP_PER_ADAPTER_INFO structure in order
	// to get the list of dns addresses used by this adapter.
	bool obtainedtPerAdapterInfo = false;
	CORE::CDynamicBuffer perAdapterInfoBuffer;
	errorCode = ::GetPerAdapterInfo( m_nicIndex, NULL, &ulLen );
	if( errorCode == ERROR_BUFFER_OVERFLOW || errorCode == ERROR_INVALID_PARAMETER ) 
	{
		if ( perAdapterInfoBuffer.SetBufferSize( ulLen ) )
		{
			pPerAdapt = (IP_PER_ADAPTER_INFO*) perAdapterInfoBuffer.GetBufferPtr();
			errorCode = ::GetPerAdapterInfo( m_nicIndex, pPerAdapt, &ulLen );
			if( errorCode == ERROR_SUCCESS )
			{
				obtainedtPerAdapterInfo = true;
				perAdapterInfoBuffer.SetDataSize( ulLen );
			}
			else
			{
				GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CWin32NetworkInterface: Failed to obtain per adapter information using Win32 GetPerAdapterInfo()" );
			}
		}
		else
		{
			GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CWin32NetworkInterface: Failed to allocate required storage to hold per adapter information. Need " + CORE::UInt32ToString( ulLen ) + " bytes" );
		}
	}
	else
	{
		GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CWin32NetworkInterface: Failed to get required storage size to hold per adapter information using Win32 GetPerAdapterInfo()" );
	}

	if ( obtainedtPerAdapterInfo )
	{
		pNext = &( pPerAdapt->DnsServerList );
		while( pNext ) 
		{
			CHostAddress address( pNext->IpAddress.String );
			m_dnsAddresses.push_back( address );
			pNext = pNext->Next;
		}				
	}

	return obtainedtPerAdapterInfo;
}

/*-------------------------------------------------------------------------*/

bool 
CWin32NetworkInterface::EnumNetworkAdapters( TNetworkInterfaceVector& interfaces ) 
{	
	IP_ADAPTER_INFO* pAdptInfo	= NULL;
	IP_ADAPTER_INFO* pNextAd	= NULL;	
	ULONG ulLen					= 0;
	ULONG errorCode				= 0;
	
	CORE::CDynamicBuffer adapterInfoBuffer;
	errorCode = ::GetAdaptersInfo( NULL, &ulLen );
	if( errorCode == ERROR_BUFFER_OVERFLOW ) 
	{
		if ( adapterInfoBuffer.SetBufferSize( ulLen ) )
		{
			pAdptInfo = (IP_ADAPTER_INFO*) adapterInfoBuffer.GetBufferPtr();
			errorCode = ::GetAdaptersInfo( pAdptInfo, &ulLen );
			if( errorCode != ERROR_SUCCESS )
			{
				GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CWin32NetworkInterface: Failed to obtain adapter information using Win32 GetAdaptersInfo()" );
				return false;
			}
			else
			{
				adapterInfoBuffer.SetDataSize( ulLen );	
			}
		}
		else
		{
			GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CWin32NetworkInterface: Failed to allocate required storage to hold adapter information. Need " + CORE::UInt32ToString( ulLen ) + " bytes" );
			return false;
		}
	}
	else
	{
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CWin32NetworkInterface: Failed to get required storage size to hold adapter information using GetAdaptersInfo()" );
		return false;
	}

	// loop through for all available interfaces and setup an associated
	// CNetworkAdapter class.
	pNextAd = pAdptInfo;
	while( pNextAd ) 
	{
		CWin32NetworkInterface* nic = new CWin32NetworkInterface();
		if ( nic->SetupAdapterInfo( pNextAd ) )
		{
			interfaces.push_back( nic );
		}
		else
		{
			delete nic;
		}
		pNextAd = pNextAd->Next;
	}

	return true;
}

/*-------------------------------------------------------------------------*/

CWin32NetworkInterface::CWin32NetworkInterface( void )
    : CINetworkInterface()
    , m_name()
    , m_desc()
    , m_priWins()
    , m_secWins()
    , m_defGateway()
    , m_dhcpAddr()
    , m_curIpAddr()
    , m_nicIndex( 0 )
    , m_adapterType( 0 )	
    , m_dhcpUsed( false )
    , m_winsUsed( false )		
    , m_dnsAddresses()
    , m_ipAddresses()
    , m_gatewayList()
    , m_leaseObtained( 0 )
    , m_leaseExpires( 0 )
	, m_dataLock()
{GUCEF_TRACE;
          
}

/*-------------------------------------------------------------------------*/
    
CWin32NetworkInterface::~CWin32NetworkInterface()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CWin32NetworkInterface::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::COMCORE::CWin32NetworkInterface"; 
    return classTypeName;
}

/*-------------------------------------------------------------------------*/
    
CORE::CString
CWin32NetworkInterface::GetCommunicationInterfaceType( void ) const
{GUCEF_TRACE;

    return "network";
}

/*-------------------------------------------------------------------------*/

CString 
CWin32NetworkInterface::GetAdapterName( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_name;
}

/*-------------------------------------------------------------------------*/

CString 
CWin32NetworkInterface::GetAdapterDescription( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_desc;
}

/*-------------------------------------------------------------------------*/

UInt32 
CWin32NetworkInterface::GetNrOfDnsAddresses( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return (UInt32) m_dnsAddresses.size();
}

/*-------------------------------------------------------------------------*/
    
bool 
CWin32NetworkInterface::GetDnsAddresses( THostAddressVector& dnsAddresses )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	THostAddressVector::const_iterator i = m_dnsAddresses.begin();
	while ( i != m_dnsAddresses.end() )
	{
		dnsAddresses.push_back( (*i) );
		++i;
	}
	return true;
}

/*-------------------------------------------------------------------------*/

UInt32 
CWin32NetworkInterface::GetNrOfIPAddresses( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return (UInt32) m_ipAddresses.size();
}

/*-------------------------------------------------------------------------*/

bool 
CWin32NetworkInterface::GetIPInfo( TIPInfoVector& ipInfo, bool includeUninitialized )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	TIPInfoVector::const_iterator i = m_ipAddresses.begin();
	while ( i != m_ipAddresses.end() )
	{
		if ( includeUninitialized || 0 != (*i).ip.GetAddress() )
			ipInfo.push_back( (*i) );
		++i;
	}
	return true;
}

/*-------------------------------------------------------------------------*/
		
bool 
CWin32NetworkInterface::IsDhcpUsed( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_dhcpUsed;
}

/*-------------------------------------------------------------------------*/

time_t 
CWin32NetworkInterface::GetDhcpLeaseObtainedTime( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_leaseObtained;
}

/*-------------------------------------------------------------------------*/

time_t 
CWin32NetworkInterface::GetDhcpLeaseExpirationTime( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_leaseExpires;
}

/*-------------------------------------------------------------------------*/

bool 
CWin32NetworkInterface::ReleaseAddress( void )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

bool 
CWin32NetworkInterface::RenewAddress( void )
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return false;
}

/*-------------------------------------------------------------------------*/

bool 
CWin32NetworkInterface::IsWinsUsed( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_winsUsed;
}

/*-------------------------------------------------------------------------*/

CIPAddress
CWin32NetworkInterface::GetPrimaryWinsServer( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_priWins;
}

/*-------------------------------------------------------------------------*/

CIPAddress 
CWin32NetworkInterface::GetSecondaryWinsServer( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_secWins;
}

/*-------------------------------------------------------------------------*/

UInt32 
CWin32NetworkInterface::GetOsAdapterIndex( void ) const
{GUCEF_TRACE;

	MT::CObjectScopeLock lock( this );
	return m_nicIndex;
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CWin32NetworkInterface::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

bool
CWin32NetworkInterface::Lock( void ) const
{GUCEF_TRACE;

    return m_dataLock.Lock();
}

/*-------------------------------------------------------------------------*/

bool
CWin32NetworkInterface::Unlock( void ) const
{GUCEF_TRACE;

    return m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */
