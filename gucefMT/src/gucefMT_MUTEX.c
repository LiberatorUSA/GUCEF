/*
 *  gucefMT: GUCEF module providing multithreading solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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

#ifdef GUCEF_MSWIN_BUILD
#include <windows.h>
#endif

#ifdef GUCEF_LINUX_BUILD
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
        UInt8 locked;
        #ifdef GUCEF_MSWIN_BUILD
        HANDLE id;
        #else
        #ifdef GUCEF_LINUX_BUILD
        pthread_mutex_t id;
        #else
        #error Unsuported target platform
        #endif
        #endif
};

typedef struct SMutex TMutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      allocates storage for a mutex.
 */
struct SMutex*
MutexCreate( void )
{
        #ifdef GUCEF_MSWIN_BUILD
        TMutex* mutex = malloc( sizeof( TMutex ) );
        mutex->locked = 0;
        mutex->id = CreateMutex( NULL, FALSE, NULL );
        if ( !mutex->id )
        {
                free( mutex );
                return NULL;
        }
        return mutex;
        #else
        #ifdef GUCEF_LINUX_BUILD
        pthread_mutexattr_t attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE_NP );

        TMutex* mutex = malloc( sizeof( TMutex ) );
        mutex->locked = 0;

        if ( pthread_mutex_init( &mutex->id, &attr ) != 0 )
        {
                free( mutex );
                return NULL;
        }
        return mutex;
        #endif
        #endif
}

/*--------------------------------------------------------------------------*/

/**
 *      de-allocates storage for a mutex.
 */
void
MutexDestroy( struct SMutex* mutex )
{
        #ifdef GUCEF_MSWIN_BUILD
        CloseHandle( mutex->id );
        free( mutex );
        #else
        #ifdef GUCEF_LINUX_BUILD
        pthread_mutex_destroy( &mutex->id );
        free( mutex );
        #endif
        #endif
}

/*--------------------------------------------------------------------------*/

/**
 *      Lock the mutex. If the mutex is already locked the calling
 *      process will have to wait for the mutex to allow a lock.
 *      The return value indicates wheter the lock failed or succeeded.
 */
UInt32
MutexLock( struct SMutex* mutex )
{
        #ifdef GUCEF_MSWIN_BUILD
        if ( WaitForSingleObject( mutex->id ,
                                  INFINITE  ) == WAIT_FAILED ) return 0;
        mutex->locked = 1;
        return 1;
        #else
        #ifdef GUCEF_LINUX_BUILD
        if ( pthread_mutex_lock( &mutex->id ) < 0 ) return 0;
        mutex->locked = 1;
        return 1;
        #endif
        #endif
}

/*--------------------------------------------------------------------------*/

/**
 *      Unlocks the mutex after a call to Lock_Mutex(). Other processes
 *      will have the ability to get a mutex lock after this call.
 *      The return value indicates wheter the unlock failed or succeeded.
 */
UInt32
MutexUnlock( struct SMutex* mutex )
{
        #ifdef GUCEF_MSWIN_BUILD
        if ( ReleaseMutex( mutex->id ) == FALSE ) return 0;
        mutex->locked = 0;
        return 1;
        #else
        #ifdef GUCEF_LINUX_BUILD
        if ( pthread_mutex_unlock( &mutex->id ) < 0 ) return 0;
        mutex->locked = 0;
        return 1;
        #endif
        #endif
}

/*--------------------------------------------------------------------------*/

/**
 *      Returns wheter the mutex is currently locked.
 */
UInt32
MutexLocked( struct SMutex* mutex )
{
        return mutex->locked;
}


/*--------------------------------------------------------------------------*/
