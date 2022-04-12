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
#include "gucefMT_CMutex.h"     /* Our mutex class definition */

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
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SMutexData
{
    UInt32 threadOwningLock;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    HANDLE id;

    SMutexData( void )
        : threadOwningLock( 0 )
        , id( 0 )
    {GUCEF_TRACE;
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    pthread_mutex_t id;

    SMutexData( void )
        : threadOwningLock( 0 )
        , id()
    {GUCEF_TRACE;
    }

    #else
    #error Unsuported target platform
    #endif
};

typedef struct SMutexData TMutexData;

/*--------------------------------------------------------------------------*/

CMutex::CMutex( void )
    : _mutexdata( new TMutexData )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    _mutexdata->id = CreateMutex( NULL, FALSE, NULL );
    if ( 0 == _mutexdata->id )
    {
        delete _mutexdata;
        _mutexdata = GUCEF_NULL;
        GUCEF_ASSERT_ALWAYS;
        return;
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    pthread_mutexattr_t attr;

    pthread_mutexattr_init( &attr );
    pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

    if ( pthread_mutex_init( &_mutexdata->id, &attr ) != 0 )
    {
        delete _mutexdata;
        _mutexdata = GUCEF_NULL;
        GUCEF_ASSERT_ALWAYS;
        return;
    }

    #endif
}

/*--------------------------------------------------------------------------*/

CMutex::~CMutex()
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    CloseHandle( ((TMutexData*)_mutexdata)->id );
    delete (TMutexData*)_mutexdata;
    _mutexdata = GUCEF_NULL;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    pthread_mutex_destroy( &((TMutexData*)_mutexdata)->id );
    delete (TMutexData*)_mutexdata;
    _mutexdata = GUCEF_NULL;

    #endif
}

/*--------------------------------------------------------------------------*/

bool
CMutex::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == _mutexdata )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    if ( WaitForSingleObject( ((TMutexData*)_mutexdata)->id ,
                              lockWaitTimeoutInMs == GUCEF_MT_INFINITE_LOCK_TIMEOUT ? INFINITE : (DWORD) lockWaitTimeoutInMs ) == WAIT_FAILED ) 
        return false;

    // In case we are being cleaned up while waiting for the lock
    if ( GUCEF_NULL == _mutexdata )
        return false;    
    
    ((TMutexData*)_mutexdata)->threadOwningLock = (UInt32) ::GetCurrentThreadId();
    return true;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( pthread_mutex_lock( &((TMutexData*)_mutexdata)->id ) < 0 ) return false;
    return true;

    #endif
}

/*--------------------------------------------------------------------------*/

bool
CMutex::Unlock( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == _mutexdata )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    if ( ReleaseMutex( ((TMutexData*)_mutexdata)->id ) == FALSE )
    {
        return false;
    }
    return true;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( pthread_mutex_unlock( &((TMutexData*)_mutexdata)->id ) < 0 ) return false;
    return true;

    #endif
}

/*--------------------------------------------------------------------------*/

const CILockable*
CMutex::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*--------------------------------------------------------------------------*/

UInt32
CMutex::GetThreadIdOfThreadLastHoldingTheLock( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == _mutexdata )
        return 0;

    return ((TMutexData*)_mutexdata)->threadOwningLock;
}

/*--------------------------------------------------------------------------*/

bool
CMutex::IsLocked( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == _mutexdata )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    /* lock with a timeout of 0. Note this is not a cheap operation */
    if ( WaitForSingleObject( ((TMutexData*)_mutexdata)->id , 0 ) == WAIT_FAILED ) 
        return true;
    ReleaseMutex( ((TMutexData*)_mutexdata)->id );
    return false;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( pthread_mutex_trylock( &((TMutexData*)_mutexdata)->id ) != 0 ) 
        return false;
    pthread_mutex_unlock( &((TMutexData*)_mutexdata)->id );
    return true;

    #else

    #error unsupported platform

    #endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
