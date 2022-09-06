/*
 *  gucefMT: GUCEF module providing multithreading solutions
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

#include <assert.h>
#include <map>

#include "gucefMLF_locktrace.h"

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_MT_MUTEX_H
#include "gucefMT_mutex.h"
#define GUCEF_MT_MUTEX_H
#endif /* GUCEF_MT_MUTEX_H ? */

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h" 
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_MT_CREADWRITELOCK_H
#include "gucefMT_CReadWriteLock.h"
#define GUCEF_MT_CREADWRITELOCK_H
#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

#ifndef GUCEF_CALLSTACK_H
#include "gucefMLF_callstack.h"
#define GUCEF_CALLSTACK_H
#endif /* GUCEF_CALLSTACK_H ? */

/*
 *  We specifically do NOT want to redirect memory management here
 */
#ifndef GUCEF_DYNNEWOFF_H
#include "gucef_dynnewoff.h"
#define GUCEF_DYNNEWOFF_H
#endif /* GUCEF_DYNNEWOFF_H ? */

#undef GUCEF_USE_CALLSTACK_TRACING
#undef GUCEF_USE_PLATFORM_CALLSTACK_TRACING
#undef GUCEF_USE_PLATFORM_LOCK_TRACER

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MLF {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_HIDDEN LockInventory
{
    public:

    LockInventory( void );
    ~LockInventory();

    static LockInventory* Instance( void );
    static void Deinstance( void );

    void RegisterExclusiveLockObtained( void* lockId );

    void RegisterExclusiveLockReleased( void* lockId );

    private:

    class GUCEF_HIDDEN LockTraceInfo
    {
        public:

        TCallStack* m_callstackAtLockObtainment;
        TCallStack* m_callstackAtLockRelease;
        UInt32 m_lastCallerThreadIdAtLockObtainment;
        UInt32 m_lastCallerThreadIdAtLockRelease;
        Int32 m_lockReentrancyDepth;
        bool m_isLocked;
        bool m_isExclusivelyLocked;

        LockTraceInfo( void );
        ~LockTraceInfo();
    };
    
    typedef std::map< void*, LockTraceInfo >    TLockIdToLockTraceInfoMap;
    
    LockTraceInfo* GetLockTraceInfo( MT::CScopeReaderLock& readLock, void* lockId );
    
    TLockIdToLockTraceInfoMap m_inventory;
    MT::CReadWriteLock m_datalock;

    static MT::CMutex g_instanceLock;
    static LockInventory* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex LockInventory::g_instanceLock;
LockInventory* LockInventory::g_instance = GUCEF_NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

LockInventory*
LockInventory::Instance( void )
{
    if ( GUCEF_NULL == g_instance )
    {
        MT::CScopeMutex lock( g_instanceLock );
        if ( GUCEF_NULL == g_instance )
        {
            g_instance = new LockInventory();
        }
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
LockInventory::Deinstance( void )
{
    if ( GUCEF_NULL != g_instance )
    {
        MT::CScopeMutex lock( g_instanceLock );
        if ( GUCEF_NULL != g_instance )
        {
            delete g_instance;
            g_instance = GUCEF_NULL;
        }
    }
}

/*-------------------------------------------------------------------------*/

LockInventory::LockInventory( void )
    : m_inventory()
    , m_datalock( true )
{
}

/*-------------------------------------------------------------------------*/

LockInventory::~LockInventory()
{
    MT::CScopeWriterLock writeLock( m_datalock );

    TLockIdToLockTraceInfoMap::iterator i = m_inventory.begin();
    while ( i != m_inventory.end() )
    {
       
        ++i;
    }
    m_inventory.clear();
}

/*-------------------------------------------------------------------------*/

LockInventory::LockTraceInfo::LockTraceInfo( void )
    : m_callstackAtLockObtainment( GUCEF_NULL )
    , m_callstackAtLockRelease( GUCEF_NULL )
    , m_lastCallerThreadIdAtLockObtainment( 0 )
    , m_lastCallerThreadIdAtLockRelease( 0 )
    , m_lockReentrancyDepth( 0 )
    , m_isLocked( false )
    , m_isExclusivelyLocked( false )
{
}

/*-------------------------------------------------------------------------*/

LockInventory::LockTraceInfo::~LockTraceInfo()
{
    MEMMAN_FreeCallstackCopy( m_callstackAtLockObtainment );
    m_callstackAtLockObtainment = GUCEF_NULL;
    MEMMAN_FreeCallstackCopy( m_callstackAtLockRelease );
    m_callstackAtLockRelease = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

LockInventory::LockTraceInfo*
LockInventory::GetLockTraceInfo( MT::CScopeReaderLock& readLock, void* lockId )
{
    LockTraceInfo* lockTraceInfo = GUCEF_NULL;
    TLockIdToLockTraceInfoMap::iterator i = m_inventory.find( lockId );
    if ( i != m_inventory.end() )
    {
        return &(*i).second;
    }
    else
    {
        // Escalate to a write lock since this is the first time we are seeing this lock id
        MT::CScopeWriterLock writeLock( readLock );
        LockTraceInfo& newLockTrace = m_inventory[ lockId ];

        // Now degrade back to a read lock
        writeLock.TransitionToReader( readLock );
        return &newLockTrace;
    }
}

/*-------------------------------------------------------------------------*/

void
LockInventory::RegisterExclusiveLockObtained( void* lockId )
{
    MT::CScopeReaderLock readLock( m_datalock );

    // We rely on the fact that any caller would have already obtained a lock on an exclusive access lock
    // as such, if true, we know that only 1 thread at a time will access the lock trace information
    // so as long as protect the lock administration itself the lock info updates can safely be done in paralell
    // hence the reader lock

    UInt32 callerThreadId = MT::GetCurrentTaskID(); 
    LockTraceInfo* lockTrace = GetLockTraceInfo( readLock, lockId );
    assert( GUCEF_NULL != lockTrace );

    if ( lockTrace->m_isLocked )
    {
        if ( callerThreadId == lockTrace->m_lastCallerThreadIdAtLockObtainment )
        {
            ++lockTrace->m_lockReentrancyDepth;
        }
        else
        {
            // You should not be able to get here
            // If means there is a race condition on the lock trace information
            // or your calls to RegisterExclusiveLockObtained() vs RegisterExclusiveLockReleased() are not matched up properly
            // Check your caller code
            GUCEF_UNREACHABLE;
        }
    }

    // Retain information on the new lock session
    MEMMAN_FreeCallstackCopy( lockTrace->m_callstackAtLockObtainment );
    MEMMAN_GetCallstackCopyForCurrentThread( &lockTrace->m_callstackAtLockObtainment, 0 );
    lockTrace->m_lastCallerThreadIdAtLockObtainment = callerThreadId;
    lockTrace->m_isLocked = true;
    lockTrace->m_isExclusivelyLocked = true;
}

/*-------------------------------------------------------------------------*/

void
LockInventory::RegisterExclusiveLockReleased( void* lockId )
{
    MT::CScopeReaderLock readLock( m_datalock );
    
    // We rely on the fact that any caller would have already obtained a lock on an exclusive access lock
    // as such, if true, we know that only 1 thread at a time will access the lock trace information
    // so as long as protect the lock administration itself the lock info updates can safely be done in paralell
    // hence the reader lock

    UInt32 callerThreadId = MT::GetCurrentTaskID(); 
    LockTraceInfo* lockTrace = GetLockTraceInfo( readLock, lockId );
    assert( GUCEF_NULL != lockTrace );
    assert( lockTrace->m_isLocked );
    assert( callerThreadId == lockTrace->m_lastCallerThreadIdAtLockObtainment );
    
    lockTrace->m_lastCallerThreadIdAtLockRelease = callerThreadId;

    if ( callerThreadId == lockTrace->m_lastCallerThreadIdAtLockObtainment )
    {
        if ( lockTrace->m_lockReentrancyDepth > 0 )
        {
            --lockTrace->m_lockReentrancyDepth;
        }
        else
        {
            lockTrace->m_isLocked = false;    
            lockTrace->m_isExclusivelyLocked = false;
        }

        MEMMAN_FreeCallstackCopy( lockTrace->m_callstackAtLockRelease );
        MEMMAN_GetCallstackCopyForCurrentThread( &lockTrace->m_callstackAtLockRelease, 0 );        
    }
    else
    {
        // You should not be able to get here
        // If means there is a race condition on the lock trace information
        // or your calls to RegisterExclusiveLockObtained() vs RegisterExclusiveLockReleased() are not matched up properly
        // Check your caller code
        GUCEF_UNREACHABLE;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
MEMMAN_ExclusiveLockObtained( void* lockId )
{
    LockInventory::Instance()->RegisterExclusiveLockObtained( lockId );
}

/*-------------------------------------------------------------------------*/

void
MEMMAN_ExclusiveLockReleased( void* lockId )
{
    LockInventory::Instance()->RegisterExclusiveLockReleased( lockId );
}

/*-------------------------------------------------------------------------*/

void
MEMMAN_LockTraceInit( void )
{
    LockInventory::Instance();
}

/*-------------------------------------------------------------------------*/

void
MEMMAN_LockTraceShutdown( void )
{
    LockInventory::Deinstance();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MLF */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
