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

#ifndef GUCEF_COMCORE_CINETWORKINTERFACE_H
#define GUCEF_COMCORE_CINETWORKINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CILOCKABLE_H
#include "gucefMT_CILockable.h"
#define GUCEF_MT_CILOCKABLE_H
#endif /* GUCEF_MT_CILOCKABLE_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_COMCORE_CICOMMUNICATIONINTERFACE_H
#include "gucefCOMCORE_CICommunicationInterface.h"
#define GUCEF_COMCORE_CICOMMUNICATIONINTERFACE_H
#endif /* GUCEF_COMCORE_CICOMMUNICATIONINTERFACE_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_COMCORE_CIPINFO_H
#include "CIPInfo.h"
#define GUCEF_COMCORE_CIPINFO_H
#endif /* GUCEF_COMCORE_CIPINFO_H ? */

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


class GUCEF_COMCORE_EXPORT_CPP CINetworkInterface : public virtual CICommunicationInterface ,
                                                    public virtual MT::CILockable
{
    public:

    typedef std::vector< CHostAddress > THostAddressVector;
    typedef std::vector< CIPAddress > TIPAddressVector;
    typedef std::vector< CIPInfo > TIPInfoVector;
    
    virtual ~CINetworkInterface();

    virtual CString GetAdapterName( void ) const = 0;

    virtual CString GetAdapterDescription( void ) const = 0;

    virtual UInt32 GetNrOfDnsAddresses( void ) const = 0;
    
    virtual bool GetDnsAddresses( THostAddressVector& dnsAddresses ) = 0;

    virtual UInt32 GetNrOfIPAddresses( void ) const = 0;

    virtual bool GetIPInfo( TIPInfoVector& ipInfo, bool includeUninitialized = false ) = 0;
		
    virtual bool IsDhcpUsed( void ) const = 0;		

    virtual time_t GetDhcpLeaseObtainedTime( void ) const = 0;

    virtual time_t GetDhcpLeaseExpirationTime( void ) const = 0;
    
    virtual bool ReleaseAddress( void ) = 0;

    virtual bool RenewAddress( void ) = 0;

    virtual bool IsWinsUsed( void ) const = 0;

    virtual CIPAddress GetPrimaryWinsServer( void ) const = 0;

    virtual CIPAddress GetSecondaryWinsServer( void ) const = 0;

    virtual UInt32 GetOsAdapterIndex( void ) const = 0;

    protected:

    CINetworkInterface( void );
    CINetworkInterface( const CINetworkInterface& src );
    CINetworkInterface& operator=( const CINetworkInterface& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CINETWORKINTERFACE_H ? */
