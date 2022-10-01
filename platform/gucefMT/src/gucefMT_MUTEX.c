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

#ifndef GUCEF_MT_MUTEX_H
#include "gucefMT_mutex.h"     /* mutex utility prototypes */
#define GUCEF_MT_MUTEX_H
#endif /* GUCEF_MT_MUTEX_H ? */

#include <malloc.h>

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

#include <windows.h>
#include <processthreadsapi.h>

#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SMutex
{
    UInt32 lockedByThread;
    Int32 reentrancyCount;
    
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    HANDLE id;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    pthread_mutex_t id;

    #else
    #error Unsuported target platform
    #endif
};

typedef struct SMutex TMutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SMutex*
MutexCreate( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    TMutex* mutex = malloc( sizeof( TMutex ) );
    if ( GUCEF_NULL != mutex )
    {
        mutex->lockedByThread = 0;
        mutex->reentrancyCount = 0;
        mutex->id = CreateMutex( NULL, FALSE, NULL );
        if ( 0 == mutex->id )
        {
            free( mutex );
            return GUCEF_NULL;
        }
        GUCEF_TRACE_EXCLUSIVE_LOCK_CREATED( mutex->id );
    }
    return mutex;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    pthread_mutexattr_t attr;
    pthread_mutexattr_init( &attr );
    pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE_NP );

    TMutex* mutex = malloc( sizeof( TMutex ) );
    mutex->reentrancyCount = 0;
    mutex->lockedByThread = 0;

    if ( pthread_mutex_init( &mutex->id, &attr ) != 0 )
    {
        free( mutex );
        return GUCEF_NULL;
    }
    
    GUCEF_TRACE_EXCLUSIVE_LOCK_CREATED( &mutex->id );
    return mutex;
    
    #endif
}

/*--------------------------------------------------------------------------*/

void
MutexDestroy( struct SMutex* mutex )
{
    MutexLock( mutex, GUCEF_UINT32MAX );
    
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    GUCEF_TRACE_EXCLUSIVE_LOCK_DESTROY( mutex->id );
    CloseHandle( mutex->id );
    free( mutex );
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    GUCEF_TRACE_EXCLUSIVE_LOCK_DESTROY( &mutex->id );
    pthread_mutex_destroy( &mutex->id );
    free( mutex );
    
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
MutexLock( struct SMutex* mutex, UInt32 timeoutInMs )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    GUCEF_BEGIN;
    DWORD waitResult = WaitForSingleObject( mutex->id, (DWORD) timeoutInMs );
    switch ( waitResult )
    {
        case WAIT_OBJECT_0:
        {
            UInt32 callerThreadId = (UInt32) GetCurrentThreadId();
            if ( callerThreadId == mutex->lockedByThread )             
                ++mutex->reentrancyCount;

            GUCEF_TRACE_EXCLUSIVE_LOCK_OBTAINED( mutex->id );
            GUCEF_END;
            return GUCEF_MUTEX_OPERATION_SUCCESS;
        }
        case WAIT_TIMEOUT:
        {
            GUCEF_END;
            return GUCEF_MUTEX_WAIT_TIMEOUT;
        }
        case WAIT_ABANDONED:
        {
            UInt32 callerThreadId = (UInt32) GetCurrentThreadId();
            mutex->lockedByThread = callerThreadId;
            mutex->reentrancyCount = 0;

            GUCEF_TRACE_EXCLUSIVE_LOCK_ABANDONED( mutex->id );
            GUCEF_END;
            return GUCEF_MUTEX_ABANDONED;
        }
        default:
        {
            GUCEF_END;
            return GUCEF_MUTEX_OPERATION_FAILED;
        }
    }
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    UInt32 callerThreadId = (UInt32) pthread_self();
    int lockResult = pthread_mutex_lock( &mutex->id );

    if ( 0 != lockResult ) 
        return GUCEF_MUTEX_OPERATION_FAILED;
    
    mutex->lockedByThread = callerThreadId;
    if ( callerThreadId == mutex->lockedByThread )             
        ++mutex->reentrancyCount;
    GUCEF_TRACE_EXCLUSIVE_LOCK_OBTAINED( &mutex->id );
    return GUCEF_MUTEX_OPERATION_SUCCESS;
    
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
MutexUnlock( struct SMutex* mutex )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    UInt32 callerThreadId = (UInt32) GetCurrentThreadId();
    if ( mutex->lockedByThread == callerThreadId )
    {
        if ( mutex->reentrancyCount > 0 )
            --mutex->reentrancyCount;
        else
            mutex->lockedByThread = 0;
    }
    
    GUCEF_TRACE_EXCLUSIVE_LOCK_RELEASED( mutex->id );
    if ( 0 == ReleaseMutex( mutex->id ) )
    {
        /*
         *  Since we failed to release the lock we have to undo our 
         *  administration updates
         */
        if ( mutex->lockedByThread == callerThreadId )
        {
            if ( 0 == mutex->lockedByThread )
                mutex->lockedByThread = callerThreadId;
            else
                ++mutex->reentrancyCount;    

            GUCEF_TRACE_EXCLUSIVE_LOCK_OBTAINED( mutex->id );
        }
        return GUCEF_MUTEX_OPERATION_FAILED;
    }
    return GUCEF_MUTEX_OPERATION_SUCCESS;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    UInt32 callerThreadId = (UInt32) pthread_self();
    if ( mutex->lockedByThread == callerThreadId )
    {
        if ( mutex->reentrancyCount > 0 )
            --mutex->reentrancyCount;
        else
            mutex->lockedByThread = 0;
    }

    GUCEF_TRACE_EXCLUSIVE_LOCK_RELEASED( &mutex->id );
    if ( pthread_mutex_unlock( &mutex->id ) != 0 ) 
    {
        /*
         *  Since we failed to release the lock we have to undo our 
         *  administration updates
         */
        if ( mutex->lockedByThread == callerThreadId )
        {
            if ( 0 == mutex->lockedByThread )
                mutex->lockedByThread = callerThreadId;
            else
                ++mutex->reentrancyCount;    

            GUCEF_TRACE_EXCLUSIVE_LOCK_OBTAINED( &mutex->id );
        }
        return GUCEF_MUTEX_OPERATION_FAILED;
    }
    return GUCEF_MUTEX_OPERATION_SUCCESS;
    
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
MutexLocked( struct SMutex* mutex )
{
    return mutex->lockedByThread;
}

/*--------------------------------------------------------------------------*/
