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

#define STACK_RESIZE_AMOUNT     256

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

    void RegisterExclusiveLockObtained( void* lockId );

    void RegisterExclusiveLockReleased( void* lockId );

    private:

    class LockTraceInfo
    {
        public:

        TCallStack* m_callstackAtLockObtainment;
        TCallStack* m_callstackAtLockRelease;
        UInt32 m_lastCallerThreadIdAtLockObtainment;
        UInt32 m_lastCallerThreadIdAtLockRelease;
        bool m_isLocked;
        bool m_isExclusivelyLocked;

        LockTraceInfo( void );
        ~LockTraceInfo();
    };
    
    typedef std::map< void*, LockTraceInfo >    TLockIdToLockTraceInfoMap;

    
    TLockIdToLockTraceInfoMap m_inventory;

    static MT::CMutex g_lock;
    static LockInventory* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex LockInventory::g_lock;
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
        MT::CScopeMutex lock( g_lock );
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
        MT::CScopeMutex lock( g_lock );
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
{
}

/*-------------------------------------------------------------------------*/

LockInventory::~LockInventory()
{
    MT::CScopeMutex lock( g_lock );

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
    , m_isLocked( false )
    , m_isExclusivelyLocked( false )
{
}

/*-------------------------------------------------------------------------*/

LockInventory::LockTraceInfo::~LockTraceInfo()
{
    MEMMAN_FreeCallstackCopy( m_callstackAtLockObtainment );
    MEMMAN_FreeCallstackCopy( m_callstackAtLockRelease );
}

/*-------------------------------------------------------------------------*/

void
LockInventory::RegisterExclusiveLockObtained( void* lockId )
{
    MT::CScopeMutex lock( g_lock );

    LockTraceInfo& lockTrace = m_inventory[ lockId ];
    MEMMAN_FreeCallstackCopy( lockTrace.m_callstackAtLockObtainment );
    MEMMAN_GetCallstackCopyForCurrentThread( &lockTrace.m_callstackAtLockObtainment, 0 );
    lockTrace.m_lastCallerThreadIdAtLockObtainment = MT::GetCurrentTaskID();
    lockTrace.m_isLocked = true;
    lockTrace.m_isExclusivelyLocked = true;
}

/*-------------------------------------------------------------------------*/

void
LockInventory::RegisterExclusiveLockReleased( void* lockId )
{
    LockTraceInfo& lockTrace = m_inventory[ lockId ];
    MEMMAN_FreeCallstackCopy( lockTrace.m_callstackAtLockRelease );
    MEMMAN_GetCallstackCopyForCurrentThread( &lockTrace.m_callstackAtLockRelease, 0 );
    lockTrace.m_lastCallerThreadIdAtLockRelease = MT::GetCurrentTaskID();
    lockTrace.m_isLocked = false;    
    lockTrace.m_isExclusivelyLocked = false;
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
