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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
  #define EOL                   "\r\n"
#else
  #define EOL                   "\n"
#endif

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

    void RegisterExclusiveLockCreation( void* lockId );
    
    void RegisterExclusiveLockObtained( void* lockId );

    void RegisterExclusiveLockReleased( void* lockId );

    void RegisterExclusiveLockAbandonment( void* lockId );

    void RegisterExclusiveLockDestruction( void* lockId );

    void PrintLockStacks( void* lockId );

    private:

    class GUCEF_HIDDEN LockTraceInfo
    {
        public:

        TCallStack* m_callstackAtLockCreation;
        TCallStack* m_callstackAtLockObtainment;
        TCallStack* m_callstackAtLockRelease;
        UInt32 m_threadIdAtLockCreation;
        UInt32 m_lastCallerThreadIdAtLockObtainment;
        UInt32 m_lastCallerThreadIdAtLockRelease;
        Int32 m_lockReentrancyDepth;
        UInt32 m_abandonmentCounter;
        TCallStack* m_lastAbandonedCallstackAtLockObtainment;
        UInt32 m_lastAbandonedCallerThreadIdAtLockObtainment; 
        bool m_isLocked;
        bool m_isExclusivelyLocked;
        UInt32 m_surplusLockReleases;
        TCallStack* m_callstackAtLastSurplusLockRelease;

        LockTraceInfo( void );
        ~LockTraceInfo();
    };
    
    typedef std::map< void*, LockTraceInfo >    TLockIdToLockTraceInfoMap;
    
    LockTraceInfo* GetLockTraceInfo( MT::CScopeReaderLock& readLock, void* lockId );
    
    void PrintLockStacks( void* lockId, LockTraceInfo* lockTrace, FILE* dest );
    
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
    : m_callstackAtLockCreation( GUCEF_NULL )
    , m_callstackAtLockObtainment( GUCEF_NULL )
    , m_callstackAtLockRelease( GUCEF_NULL )
    , m_threadIdAtLockCreation( 0 )
    , m_lastCallerThreadIdAtLockObtainment( 0 )
    , m_lastCallerThreadIdAtLockRelease( 0 )
    , m_lockReentrancyDepth( 0 )
    , m_abandonmentCounter( 0 )
    , m_lastAbandonedCallstackAtLockObtainment( GUCEF_NULL )
    , m_lastAbandonedCallerThreadIdAtLockObtainment( 0 )
    , m_isLocked( false )
    , m_isExclusivelyLocked( false )
    , m_surplusLockReleases( 0 )
    , m_callstackAtLastSurplusLockRelease( GUCEF_NULL )
{
}

/*-------------------------------------------------------------------------*/

LockInventory::LockTraceInfo::~LockTraceInfo()
{
    MEMMAN_FreeCallstackCopy( m_callstackAtLockCreation );
    m_callstackAtLockCreation = GUCEF_NULL;
    MEMMAN_FreeCallstackCopy( m_callstackAtLockObtainment );
    m_callstackAtLockObtainment = GUCEF_NULL;
    MEMMAN_FreeCallstackCopy( m_callstackAtLockRelease );
    m_callstackAtLockRelease = GUCEF_NULL;
    MEMMAN_FreeCallstackCopy( m_lastAbandonedCallstackAtLockObtainment );
    m_lastAbandonedCallstackAtLockObtainment = GUCEF_NULL;
    MEMMAN_FreeCallstackCopy( m_callstackAtLastSurplusLockRelease );
    m_callstackAtLastSurplusLockRelease = GUCEF_NULL;    
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
LockInventory::RegisterExclusiveLockCreation( void* lockId )
{
    MT::CScopeReaderLock readLock( m_datalock );

    // We rely on the fact that any caller would be the one creating the lock in a given scope and as such only 1 thread at a time will access the lock trace information
    // so as long as protect the lock administration itself the lock info updates can safely be done in paralell
    // hence the reader lock

    UInt32 callerThreadId = MT::GetCurrentTaskID(); 
    LockTraceInfo* lockTrace = GetLockTraceInfo( readLock, lockId );
    
    assert( GUCEF_NULL != lockTrace );
    assert( GUCEF_NULL == lockTrace->m_callstackAtLockCreation );
    assert( GUCEF_NULL == lockTrace->m_callstackAtLockObtainment );
    assert( 0 == lockTrace->m_threadIdAtLockCreation );
    assert( 0 == lockTrace->m_lockReentrancyDepth );
    assert( !lockTrace->m_isLocked );
    assert( !lockTrace->m_isExclusivelyLocked );

    // setup information on the new lock
    MEMMAN_FreeCallstackCopy( lockTrace->m_callstackAtLockCreation );
    MEMMAN_GetCallstackCopyForCurrentThread( &lockTrace->m_callstackAtLockCreation, 0 );
    lockTrace->m_threadIdAtLockCreation = callerThreadId;
    lockTrace->m_isLocked = false;
    lockTrace->m_isExclusivelyLocked = false;
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
                        
            // freeze everything
            MT::CScopeWriterLock writeLock( readLock );
            
            // print all current stacks
            GUCEF_PrintCallstack();

            // print lock stacks
            PrintLockStacks( lockId, lockTrace, stdout );

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
LockInventory::RegisterExclusiveLockAbandonment( void* lockId )
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
        // Since the lock is abandoned we'd expect our administration here to still show the lock 
        // as being locked by another thread. A thread that is presumably no longer around and who will not be contacting us to release said lock
        // instead the caller now has ownership of the lock
        
        if ( callerThreadId != lockTrace->m_lastCallerThreadIdAtLockObtainment )
        {
            // move information over the abandonment tracking
            ++lockTrace->m_abandonmentCounter;
            MEMMAN_FreeCallstackCopy( lockTrace->m_lastAbandonedCallstackAtLockObtainment );
            lockTrace->m_lastAbandonedCallstackAtLockObtainment = lockTrace->m_callstackAtLockObtainment;
            lockTrace->m_lastAbandonedCallerThreadIdAtLockObtainment = lockTrace->m_lastCallerThreadIdAtLockObtainment;

            // reset 
            lockTrace->m_lockReentrancyDepth = 0;
        }
        else
        {
            // You should not be able to get here
            // How can the lock be abandoned by the very thread we are in right now???
                        
            // freeze everything
            MT::CScopeWriterLock writeLock( readLock );
            
            // print all current stacks
            GUCEF_PrintCallstack();

            // print lock stacks
            PrintLockStacks( lockId, lockTrace, stdout );

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
    
    if ( !lockTrace->m_isLocked || !lockTrace->m_isExclusivelyLocked )
    {
        // How can you release the lock if its not locked?
        // you likely have a lock-unlock mismatch in your code
        // Check your caller code
        ++lockTrace->m_surplusLockReleases;
        MEMMAN_FreeCallstackCopy( lockTrace->m_callstackAtLastSurplusLockRelease );
        MEMMAN_GetCallstackCopyForCurrentThread( &lockTrace->m_callstackAtLastSurplusLockRelease, 0 );

        MT::CScopeWriterLock writeLock( readLock );
            
        // print all current stacks
        GUCEF_PrintCallstack();

        // print lock stacks
        PrintLockStacks( lockId, lockTrace, stdout );

        GUCEF_UNREACHABLE;
    }    
    assert( lockTrace->m_isLocked );

    if ( callerThreadId == lockTrace->m_lastCallerThreadIdAtLockObtainment )
    {
        lockTrace->m_lastCallerThreadIdAtLockRelease = callerThreadId;
        
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

        // freeze everything
        MT::CScopeWriterLock writeLock( readLock );
            
        // print all current stacks
        GUCEF_PrintCallstack();

        // print lock stacks
        PrintLockStacks( lockId, lockTrace, stdout );

        GUCEF_UNREACHABLE;
    }
}

/*-------------------------------------------------------------------------*/

void
LockInventory::RegisterExclusiveLockDestruction( void* lockId )
{
    MT::CScopeReaderLock readLock( m_datalock );

    UInt32 callerThreadId = MT::GetCurrentTaskID(); 
    LockTraceInfo* lockTrace = GetLockTraceInfo( readLock, lockId );
    assert( GUCEF_NULL != lockTrace );

    // A lock being destroyed should not be in use by other threads
    if ( lockTrace->m_isLocked && callerThreadId != lockTrace->m_lastCallerThreadIdAtLockObtainment )
    {
        // We should not have any lock still active by another thread while you are destroying said lock

        // freeze everything
        MT::CScopeWriterLock writeLock( readLock );
            
        // print all current stacks
        GUCEF_PrintCallstack();

        // print lock stacks
        PrintLockStacks( lockId, lockTrace, stdout );

        GUCEF_UNREACHABLE;
    }

    MT::CScopeWriterLock writeLock( readLock );
    m_inventory.erase( lockId );
}

/*-------------------------------------------------------------------------*/

void 
LockInventory::PrintLockStacks( void* lockId )
{
    MT::CScopeReaderLock readLock( m_datalock );
    LockTraceInfo* lockTrace = GetLockTraceInfo( readLock, lockId );
    PrintLockStacks( lockId, lockTrace, stdout );
}

/*-------------------------------------------------------------------------*/

void 
LockInventory::PrintLockStacks( void* lockId, LockTraceInfo* lockTrace, FILE* dest )
{
    fprintf( dest, "------------------------------%s", EOL );
    fprintf( dest, "Trace for lock %p:%s%s", lockId, EOL, EOL );
    fprintf( dest, "   threadIdAtLockCreation: %d%s", lockTrace->m_threadIdAtLockCreation, EOL );
    fprintf( dest, "   isExclusivelyLocked: %d%s", lockTrace->m_isExclusivelyLocked ? 1 : 0, EOL );
    fprintf( dest, "   isLocked: %d%s", lockTrace->m_isLocked ? 1 : 0, EOL );
    fprintf( dest, "   lastCallerThreadIdAtLockObtainment: %d%s", lockTrace->m_lastCallerThreadIdAtLockObtainment, EOL );
    fprintf( dest, "   lastCallerThreadIdAtLockRelease: %d%s", lockTrace->m_lastCallerThreadIdAtLockRelease, EOL );
    fprintf( dest, "   lockReentrancyDepth: %d%s", lockTrace->m_lockReentrancyDepth, EOL );
    fprintf( dest, "   surplusLockReleases: %d%s", lockTrace->m_surplusLockReleases, EOL );    
    fprintf( dest, "   abandonmentCounter: %d%s", lockTrace->m_abandonmentCounter, EOL );    
    fprintf( dest, "   lastAbandonedCallerThreadIdAtLockObtainment: %d%s", lockTrace->m_lastAbandonedCallerThreadIdAtLockObtainment, EOL );
    fprintf( dest, "------------------------------%s%s", EOL, EOL );
    fprintf( dest, "Stack at lock creation: %s%s", EOL, EOL );
    GUCEF_PrintCallstackCopy( lockTrace->m_callstackAtLockCreation );
    fprintf( dest, "------------------------------%s%s", EOL, EOL );
    fprintf( dest, "Stack at lock obtainment: %s%s", EOL, EOL );
    fprintf( dest, "------------------------%s%s", EOL, EOL );
    GUCEF_PrintCallstackCopy( lockTrace->m_callstackAtLockObtainment );
    fprintf( dest, "Stack at lock release: %s%s", EOL, EOL );
    fprintf( dest, "------------------------%s%s", EOL, EOL );    
    GUCEF_PrintCallstackCopy( lockTrace->m_callstackAtLockRelease );
    fprintf( dest, "------------------------------%s%s", EOL, EOL );    
    if ( lockTrace->m_abandonmentCounter > 0 )
    {
        fprintf( dest, "Stack at lock obtainment from last lock abandonment: %s%s", EOL, EOL );
        fprintf( dest, "------------------------%s%s", EOL, EOL );    
        GUCEF_PrintCallstackCopy( lockTrace->m_lastAbandonedCallstackAtLockObtainment );
    }
    if ( lockTrace->m_surplusLockReleases > 0 )
    {
        fprintf( dest, "Stack at last surplus lock release call: %s%s", EOL, EOL );
        fprintf( dest, "------------------------%s%s", EOL, EOL );    
        GUCEF_PrintCallstackCopy( lockTrace->m_callstackAtLastSurplusLockRelease );    
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
MEMMAN_ExclusiveLockCreated( void* lockId )
{
    LockInventory::Instance()->RegisterExclusiveLockCreation( lockId );
}

/*-------------------------------------------------------------------------*/

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
MEMMAN_ExclusiveLockAbandoned( void* lockId )
{
    LockInventory::Instance()->RegisterExclusiveLockAbandonment( lockId );
}

/*-------------------------------------------------------------------------*/

void
MEMMAN_ExclusiveLockDestroy( void* lockId )
{
    LockInventory::Instance()->RegisterExclusiveLockDestruction( lockId );
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
