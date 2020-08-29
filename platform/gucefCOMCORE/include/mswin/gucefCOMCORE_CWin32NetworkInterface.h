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

#ifndef GUCEF_COMCORE_CWIN32NETWORKINTERFACE_H
#define GUCEF_COMCORE_CWIN32NETWORKINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CINETWORKINTERFACE_H
#include "gucefCOMCORE_CINetworkInterface.h"
#define GUCEF_COMCORE_CINETWORKINTERFACE_H
#endif /* GUCEF_COMCORE_CINETWORKINTERFACE_H ? */

#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/


class CWin32NetworkInterface : public virtual CINetworkInterface
{
    public:

    typedef std::vector< CINetworkInterface* > TNetworkInterfaceVector;

    static bool EnumNetworkAdapters( TNetworkInterfaceVector& interfaces );

    virtual ~CWin32NetworkInterface();
    
    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual const CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetCommunicationInterfaceType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CString GetAdapterName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CString GetAdapterDescription( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt32 GetNrOfDnsAddresses( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool GetDnsAddresses( THostAddressVector& dnsAddresses ) GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt32 GetNrOfIPAddresses( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetIPInfo( TIPInfoVector& ipInfo, bool includeUninitialized = false ) GUCEF_VIRTUAL_OVERRIDE;
		
    virtual bool IsDhcpUsed( void ) const GUCEF_VIRTUAL_OVERRIDE;		

    virtual time_t GetDhcpLeaseObtainedTime( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual time_t GetDhcpLeaseExpirationTime( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool ReleaseAddress( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool RenewAddress( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsWinsUsed( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CIPAddress GetPrimaryWinsServer( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CIPAddress GetSecondaryWinsServer( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt32 GetOsAdapterIndex( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CWin32NetworkInterface( void );

    CWin32NetworkInterface( const CWin32NetworkInterface& src );            /**< not implemented, doesnt make sense */
    CWin32NetworkInterface& operator=( const CWin32NetworkInterface& src ); /**< not implemented, doesnt make sense */

    private:

    bool SetupAdapterInfo( void* pAdaptInfoVoid );

    private:

    CString m_name;
    CString m_desc;
    CIPAddress m_priWins;
    CIPAddress m_secWins;
    CIPAddress m_defGateway;
    CIPAddress m_dhcpAddr;
    CIPInfo m_curIpAddr;    // this is also in the ip address list but this is the address currently active.
    UInt32 m_nicIndex;      // machine index of the adapter.
    UInt32 m_adapterType;	
    bool m_dhcpUsed;
    bool m_winsUsed;		
    THostAddressVector m_dnsAddresses;
    TIPInfoVector m_ipAddresses;
    TIPAddressVector m_gatewayList;
    time_t m_leaseObtained;
    time_t m_leaseExpires;
    MT::CMutex m_dataLock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CWIN32NETWORKINTERFACE_H ? */
