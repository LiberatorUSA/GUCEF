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

#ifndef GUCEF_COMCORE_CDNSCACHEENTRY_H
#define GUCEF_COMCORE_CDNSCACHEENTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#include "CTSGNotifier.h"
#define GUCEF_CORE_CTSGNOTIFIER_H
#endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

#ifndef GUCEF_COMCORE_CDNSRESOLVER_H
#include "gucefCOMCORE_CDnsResolver.h"      
#define GUCEF_COMCORE_CDNSRESOLVER_H
#endif /* GUCEF_COMCORE_CDNSRESOLVER_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"      
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_COMCORE_CIPV4ADDRESS_H
#include "gucefCOMCORE_CIPv4Address.h"      
#define GUCEF_COMCORE_CIPV4ADDRESS_H
#endif /* GUCEF_COMCORE_CIPV4ADDRESS_H ? */

#ifndef GUCEF_COMCORE_CIPV6ADDRESS_H
#include "gucefCOMCORE_CIPv6Address.h"      
#define GUCEF_COMCORE_CIPV6ADDRESS_H
#endif /* GUCEF_COMCORE_CIPV6ADDRESS_H ? */

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

/**
 *  Class representing a DNS cache entry with host name in string form
 *  plus the IPv4 or IPv6 addresses
 */
class GUCEF_COMCORE_EXPORT_CPP CDnsCacheEntry : public CORE::CTSGNotifier                                     ,
                                                public CORE::CTSharedPtrCreator< CDnsCacheEntry, MT::CMutex >
{
    public:

    static const CORE::CEvent DnsInfoChangedEvent;
    
    static void RegisterEvents( void );

    virtual ~CDnsCacheEntry();

    bool GetPrivateCopy( CHostAddress& copy, bool overwritePort = false );

    CORE::CString GetDnsName( void ) const;

    bool GetIPv4Addresses( CIPv4Address::TIPv4AddressVector& ipv4s ) const;

    CIPv4Address GetRandomIPv4Address( void ) const;

    CIPv4Address GetFirstIPv4Address( void ) const;

    bool GetIPv6Addresses( CIPv6Address::TIPv6AddressVector& ipv6s ) const;

    bool operator==( const CHostAddress& other ) const;

    bool operator!=( const CHostAddress& other ) const;

    bool operator<( const CHostAddress& other ) const;

    bool operator<( const CDnsCacheEntry& other ) const;

    bool operator==( const CDnsCacheEntry& other ) const;

    bool operator!=( const CDnsCacheEntry& other ) const;

    private:
    friend class CDnsCache;

    CDnsCacheEntry( void );

    bool SetDnsNameAndResolve( const CORE::CString& hostName );

    bool Refresh( void );

    CDnsCacheEntry( const CDnsCacheEntry& src ); /**< not implemented, not wanted */
    
    private:

    CHostAddress m_data;
};

/*-------------------------------------------------------------------------*/

typedef CDnsCacheEntry::TSharedPtrType  CDnsCacheEntryPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CDNSCACHEENTRY_H ? */
