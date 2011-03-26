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

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

#include <windows.h>

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
    UInt8 locked;
    
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
    mutex->locked = 0;
    mutex->id = CreateMutex( NULL, FALSE, NULL );
    if ( !mutex->id )
    {
            free( mutex );
            return NULL;
    }
    return mutex;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
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
MutexLock( struct SMutex* mutex )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    if ( WaitForSingleObject( mutex->id ,
                              INFINITE  ) == WAIT_FAILED ) return 0;
    mutex->locked = 1;
    return 1;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    if ( pthread_mutex_lock( &mutex->id ) < 0 ) return 0;
    mutex->locked = 1;
    return 1;
    
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
MutexUnlock( struct SMutex* mutex )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    if ( ReleaseMutex( mutex->id ) == FALSE ) return 0;
    mutex->locked = 0;
    return 1;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
    if ( pthread_mutex_unlock( &mutex->id ) < 0 ) return 0;
    mutex->locked = 0;
    return 1;
    
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
MutexLocked( struct SMutex* mutex )
{
    return mutex->locked;
}

/*--------------------------------------------------------------------------*/
