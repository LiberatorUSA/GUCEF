/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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

#ifdef GUCEFMT_MSWIN_BUILD
#include <windows.h>
#endif

#ifdef GUCEFMT_LINUX_BUILD
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
        #ifdef GUCEFMT_MSWIN_BUILD
        HANDLE id;
        #elif GUCEFMT_LINUX_BUILD
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

/**
 *      allocates storage for a mutex.
 */
struct SMutex*
MutexCreate( void )
{
        #ifdef GUCEFMT_MSWIN_BUILD
        TMutex* mutex = malloc( sizeof( TMutex ) );
        mutex->locked = 0;        
        mutex->id = CreateMutex( NULL, FALSE, NULL );
        if ( !mutex->id )
        {
                free( mutex );
                return NULL;
        }
        return mutex;
        #elif GUCEFMT_LINUX_BUILD
        pthread_mutexattr_t attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

        TMutex* mutex = malloc( sizeof( TMutex ) );
        mutex->locked = 0;        

        if ( pthread_mutex_init( &mutex->id, &attr ) != 0 )
        {
                free( mutex );
                return NULL;
        }
        return mutex;
        #endif        
}

/*--------------------------------------------------------------------------*/

/**
 *      de-allocates storage for a mutex.
 */
void 
MutexDestroy( struct SMutex* mutex )
{
        #ifdef GUCEFMT_MSWIN_BUILD
        CloseHandle( mutex->id );
        free( mutex );
        #elif GUCEFMT_LINUX_BUILD
        pthread_mutex_destroy( &mutex->id );
        free( mutex );
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
        #ifdef GUCEFMT_MSWIN_BUILD
        if ( WaitForSingleObject( mutex->id ,
                                  INFINITE  ) == WAIT_FAILED ) return 0;
        mutex->locked = 1;
        return 1;
        #elif GUCEFMT_LINUX_BUILD
        if ( pthread_mutex_lock( &mutex->id ) < 0 ) return 0;
        mutex->locked = 1;
        return 1;
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
        #ifdef GUCEFMT_MSWIN_BUILD
        if ( ReleaseMutex( mutex->id ) == FALSE ) return 0;
        mutex->locked = 0;
        return 1;
        #elif GUCEFMT_LINUX_BUILD
        if ( pthread_mutex_unlock( &mutex->id ) < 0 ) return 0;
        mutex->locked = 0;
        return 1;
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
