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

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */ 

#ifndef GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H
#include "gucefMT_CObjectScopeReadOnlyLock.h"
#define GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H
#endif /* GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H ? */ 

#include "gucefCOMCORE_CDnsCacheEntry.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CDnsCacheEntry::DnsInfoChangedEvent = "GUCEF::COMCORE::CDnsCache::DnsInfoChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void 
CDnsCacheEntry::RegisterEvents( void )
{GUCEF_TRACE;

    DnsInfoChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CDnsCacheEntry::CDnsCacheEntry( void )
    : CORE::CTSGNotifier()
    , CORE::CTSharedPtrCreator< CDnsCacheEntry, MT::CMutex >( this )
    , m_data()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CDnsCacheEntry::~CDnsCacheEntry()
{GUCEF_TRACE;
    
    MT::CObjectScopeLock lock( this );
    m_data.Clear();
    SignalUpcomingDestruction();
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheEntry::SetDnsNameAndResolve( const CORE::CString& dnsName )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    return m_data.SetHostname( dnsName );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CDnsCacheEntry::GetDnsName( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data.GetHostname();
}

/*-------------------------------------------------------------------------*/

bool 
CDnsCacheEntry::GetIPv4Addresses( CIPv4Address::TIPv4AddressVector& ipv4s ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    ipv4s = m_data.GetIPv4Addresses();
    return true;
}

/*-------------------------------------------------------------------------*/
CIPv4Address 
CDnsCacheEntry::GetRandomIPv4Address( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data.GetRandomIPv4Address();
}

/*-------------------------------------------------------------------------*/
    
CIPv4Address 
CDnsCacheEntry::GetFirstIPv4Address( void ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data.GetFirstIPv4Address();
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheEntry::GetIPv6Addresses( CIPv6Address::TIPv6AddressVector& ipv6s ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    ipv6s = m_data.GetIPv6Addresses();
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CDnsCacheEntry::GetPrivateCopy( CHostAddress& copy, bool overwritePort )
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    copy.Assign( m_data, overwritePort );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheEntry::operator==( const CHostAddress& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data == other;
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheEntry::operator!=( const CHostAddress& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data != other;
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheEntry::operator<( const CHostAddress& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data < other;
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheEntry::operator<( const CDnsCacheEntry& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data < other.m_data;
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheEntry::operator==( const CDnsCacheEntry& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data == other.m_data;
}

/*-------------------------------------------------------------------------*/

bool
CDnsCacheEntry::operator!=( const CDnsCacheEntry& other ) const
{GUCEF_TRACE;

    MT::CObjectScopeReadOnlyLock lock( this );
    return m_data != other.m_data;
}

/*-------------------------------------------------------------------------*/

bool 
CDnsCacheEntry::Refresh( void )
{GUCEF_TRACE;

    MT::CObjectScopeLock lock( this );
    
    CHostAddress preRefreshData( m_data );

    if ( m_data.Refresh() )
    {
        if ( !m_data.Equals( preRefreshData, true ) )
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "DnsCacheEntry:Refresh: Detected a change for DNS target " + preRefreshData.GetHostname() +
                                                         " from IPv4 \"" + m_data.GetIPv4AddressesAsString() + 
                                                         "\" to IPv4 \"" + preRefreshData.GetIPv4AddressesAsString() + "\"" );            
            
            NotifyObserversFromThread( DnsInfoChangedEvent );
        }
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
