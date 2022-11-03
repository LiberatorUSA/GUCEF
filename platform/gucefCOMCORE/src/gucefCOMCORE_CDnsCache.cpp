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

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_COMCORE_CDNSCACHEREFRESHTASKCONSUMER_H
#include "gucefCOMCORE_CDnsCacheRefreshTaskConsumer.h"
#define GUCEF_COMCORE_CDNSCACHEREFRESHTASKCONSUMER_H
#endif /* GUCEF_COMCORE_CDNSCACHEREFRESHTASKCONSUMER_H ? */

#include "gucefCOMCORE_CDnsCache.h"

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

#define GUCEF_DEFAULT_DNSCACHE_ASYNC_REFRESH_INTERVAL_IN_MS     300000   // default to every 5 mins

const CORE::CEvent CDnsCache::DnsCacheRefreshStartedEvent = "GUCEF::COMCORE::CDnsCache::DnsCacheRefreshStartedEvent";
const CORE::CEvent CDnsCache::DnsCacheRefreshStoppedEvent = "GUCEF::COMCORE::CDnsCache::DnsCacheRefreshStoppedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CDnsCache::RegisterEvents( void )
{GUCEF_TRACE;

    DnsCacheRefreshStartedEvent.Initialize();
    DnsCacheRefreshStoppedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CDnsCache::CDnsCache( void )
    : CORE::CTSGNotifier()
    , CORE::CTSharedObjCreator< CDnsCache, MT::CMutex >( this )
    , m_dnsEntryCache()
    , m_asyncRefreshIntervalInMs( GUCEF_DEFAULT_DNSCACHE_ASYNC_REFRESH_INTERVAL_IN_MS )
    , m_lock( true )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CDnsCache::~CDnsCache()
{GUCEF_TRACE;

    ClearCache();
}

/*-------------------------------------------------------------------------*/

void
CDnsCache::SetAsyncRefreshIntervalInMs( UInt32 intervalInMs )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_lock );
    m_asyncRefreshIntervalInMs = intervalInMs;
}

/*-------------------------------------------------------------------------*/

UInt32
CDnsCache::GetAsyncRefreshIntervalInMs( void ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock readLock( m_lock );
    return m_asyncRefreshIntervalInMs;
}

/*-------------------------------------------------------------------------*/

void
CDnsCache::ClearCache( void )
{GUCEF_TRACE;

    MT::CScopeWriterLock lock( m_lock );
    m_dnsEntryCache.clear();
}

/*-------------------------------------------------------------------------*/

CDnsCacheEntryPtr
CDnsCache::GetOrAddCacheEntryForDns( const CString& dns )
{GUCEF_TRACE;

    MT::CScopeReaderLock readLock( m_lock );

    TDnsCacheEntryMap::iterator i = m_dnsEntryCache.find( dns );
    if ( i != m_dnsEntryCache.end() )
    {
        return (*i).second;
    }

    MT::CScopeWriterLock writeLock( m_lock );

    CDnsCacheEntryPtr dnsEntry = ( GUCEF_NEW CDnsCacheEntry() )->CreateSharedPtr();
    dnsEntry->SetPulseGenerator( GetPulseGenerator() );
    if ( dnsEntry->SetDnsNameAndResolve( dns ) )
    {
        m_dnsEntryCache.insert( TDnsCacheEntryMap::value_type( dns, dnsEntry ) );
        return dnsEntry;
    }

    return CDnsCacheEntryPtr();
}

/*-------------------------------------------------------------------------*/

void
CDnsCache::RemoveCacheEntryForDns( const CString& dns )
{GUCEF_TRACE;

    MT::CScopeWriterLock writeLock( m_lock );

    TDnsCacheEntryMap::iterator i = m_dnsEntryCache.find( dns );
    if ( i != m_dnsEntryCache.end() )
    {
        m_dnsEntryCache.erase( i );
    }
}

/*-------------------------------------------------------------------------*/

void
CDnsCache::Refresh( void )
{GUCEF_TRACE;

    // we take a read lock since we only need to gaurd against the cache index changing
    // the entries themselves have their own lock
    MT::CScopeReaderLock readLock( m_lock );

    if ( !NotifyObservers( DnsCacheRefreshStartedEvent ) ) return;

    TDnsCacheEntryMap::iterator i = m_dnsEntryCache.begin();
    while ( i != m_dnsEntryCache.end() )
    {
        CDnsCacheEntryPtr& dnsEntry = (*i).second;
        dnsEntry->Refresh();
        ++i;
    }

    NotifyObservers( DnsCacheRefreshStoppedEvent );
}

/*-------------------------------------------------------------------------*/

bool
CDnsCache::StartRefreshingASync( void )
{GUCEF_TRACE;

    CDnsCachePtr thisPtr( CreateSharedPtr() );
    return CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool()->QueueTask( CDnsCacheRefreshTaskConsumer::TaskType ,
                                                                                       &thisPtr                               );
}

/*-------------------------------------------------------------------------*/

bool
CDnsCache::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockOpBool( m_lock.WriterStart( lockWaitTimeoutInMs ) );
}

/*-------------------------------------------------------------------------*/

bool
CDnsCache::Unlock( void ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockOpBool( m_lock.WriterStop() );
}

/*-------------------------------------------------------------------------*/

bool
CDnsCache::ReadOnlyLock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockOpBool( m_lock.ReaderStart( lockWaitTimeoutInMs ) );
}

/*-------------------------------------------------------------------------*/

bool
CDnsCache::ReadOnlyUnlock( void ) const
{GUCEF_TRACE;

    return MT::CReadWriteLock::RwLockStateToLockOpBool( m_lock.ReaderStop() );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
