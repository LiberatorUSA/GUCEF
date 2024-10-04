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

#ifndef COMCOREPLUGIN_DBL_CDBLNETWORKINTERFACE_H
#define COMCOREPLUGIN_DBL_CDBLNETWORKINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CINETWORKINTERFACE_H
#include "gucefCOMCORE_CINetworkInterface.h"
#define GUCEF_COMCORE_CINETWORKINTERFACE_H
#endif /* GUCEF_COMCORE_CINETWORKINTERFACE_H ? */

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
namespace COMCOREPLUGIN {
namespace DBL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/


class CDBLNetworkInterface : public virtual COMCORE::CINetworkInterface
{
    public:

    static bool EnumNetworkAdapters( TINetworkInterfacePtrVector& interfaces );

    virtual ~CDBLNetworkInterface() GUCEF_VIRTUAL_OVERRIDE;
    
    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual const CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetCommunicationInterfaceType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetAdapterName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetAdapterDescription( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt32 GetNrOfDnsAddresses( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool GetDnsAddresses( THostAddressVector& dnsAddresses ) GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt32 GetNrOfIPAddresses( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetIPInfo( TIPv4InfoVector& ipInfo, bool includeUninitialized = false ) const GUCEF_VIRTUAL_OVERRIDE;
		
    virtual bool IsDhcpUsedForIPv4( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsDhcpUsedForIPv6( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CDateTime GetDhcpLeaseObtainedTime( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CDateTime GetDhcpLeaseExpirationTime( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool ReleaseAddress( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool RenewAddress( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsWinsUsed( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CIPv4Address GetPrimaryWinsServer( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CIPv4Address GetSecondaryWinsServer( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt32 GetOsAdapterIndex( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetMetrics( COMCORE::CNetworkInterfaceMetrics& metrics ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual MT::TLockStatus Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual MT::TLockStatus Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CDBLNetworkInterface( void );

    CDBLNetworkInterface( const CDBLNetworkInterface& src );            /**< not implemented, doesnt make sense */
    CDBLNetworkInterface& operator=( const CDBLNetworkInterface& src ); /**< not implemented, doesnt make sense */

    private:

    bool SetupAdapterInfo( void* pAdaptInfoVoid );

    private:

    MT::CMutex m_dataLock;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CDBLNetworkInterface, MT::CMutex >  CDBLNetworkInterfacePtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DBL */
}; /* namespace COMCOREPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* COMCOREPLUGIN_DBL_CDBLNETWORKINTERFACE_H ? */
