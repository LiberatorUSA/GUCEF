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
        mutex->id = CreateMutex( NULL, FALSE, NULL );
        if ( !mutex->id )
        {
            free( mutex );
            return GUCEF_NULL;
        }
    }
    return mutex;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    pthread_mutexattr_t attr;
    pthread_mutexattr_init( &attr );
    pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE_NP );

    TMutex* mutex = malloc( sizeof( TMutex ) );
    mutex->lockedByThread = 0;

    if ( pthread_mutex_init( &mutex->id, &attr ) != 0 )
    {
        free( mutex );
        return GUCEF_NULL;
    }
    return mutex;
    
    #endif
}

/*--------------------------------------------------------------------------*/

void
MutexDestroy( struct SMutex* mutex )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    CloseHandle( mutex->id );
    free( mutex );
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    pthread_mutex_destroy( &mutex->id );
    free( mutex );
    
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
MutexLock( struct SMutex* mutex, UInt32 timeoutInMs )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    UInt32 result = GUCEF_MUTEX_OPERATION_FAILED;
    DWORD waitResult = WaitForSingleObject( mutex->id, (DWORD) timeoutInMs );
    switch ( waitResult )
    {
        case WAIT_OBJECT_0:
        {
            mutex->lockedByThread = (UInt32) GetCurrentThreadId();
            return GUCEF_MUTEX_OPERATION_SUCCESS;
        }
        case WAIT_TIMEOUT:
            return GUCEF_MUTEX_WAIT_TIMEOUT;
        case WAIT_ABANDONED:
            return GUCEF_MUTEX_ABANDONED;
        default:
            return GUCEF_MUTEX_OPERATION_FAILED;

    }
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    if ( pthread_mutex_lock( &mutex->id ) < 0 ) 
        return 0;
    
    mutex->lockedByThread = (UInt32) pthread_self();
    return 1;
    
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
MutexUnlock( struct SMutex* mutex )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    if ( mutex->lockedByThread == (UInt32) GetCurrentThreadId() )
        mutex->lockedByThread = 0;

    if ( 0 == ReleaseMutex( mutex->id ) )
    {
        mutex->lockedByThread = (UInt32) GetCurrentThreadId();
        return 0;
    }
    return 1;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    if ( mutex->lockedByThread == (UInt32) pthread_self() )
        mutex->lockedByThread = 0;

    if ( pthread_mutex_unlock( &mutex->id ) < 0 ) 
        return 0;
    return 1;
    
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
MutexLocked( struct SMutex* mutex )
{
    return mutex->lockedByThread;
}

/*--------------------------------------------------------------------------*/
