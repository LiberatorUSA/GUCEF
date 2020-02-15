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
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    HANDLE id;
    
    SMutexData( void )
        : id( 0 )
    {
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    pthread_mutex_t id;

    #else
    #error Unsuported target platform
    #endif
};

typedef struct SMutexData TMutexData;

/*--------------------------------------------------------------------------*/

CMutex::CMutex( void )
    : _mutexdata( new TMutexData )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    _mutexdata->id = CreateMutex( NULL, FALSE, NULL );
    if ( 0 == _mutexdata->id )
    {
        delete _mutexdata;
        _mutexdata = NULL;
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
        _mutexdata = NULL;
        GUCEF_ASSERT_ALWAYS;
        return;
    }

    #endif
}

/*--------------------------------------------------------------------------*/

CMutex::~CMutex()
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    CloseHandle( ((TMutexData*)_mutexdata)->id );
    delete (TMutexData*)_mutexdata;
    _mutexdata = NULL;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    pthread_mutex_destroy( &((TMutexData*)_mutexdata)->id );
    delete (TMutexData*)_mutexdata;
    _mutexdata = NULL;

    #endif
}

/*--------------------------------------------------------------------------*/

bool
CMutex::Lock( void ) const
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    if ( WaitForSingleObject( ((TMutexData*)_mutexdata)->id ,
                              INFINITE                      ) == WAIT_FAILED ) return false;
    return true;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( pthread_mutex_lock( &((TMutexData*)_mutexdata)->id ) < 0 ) return false;
    return true;

    #endif
}

/*--------------------------------------------------------------------------*/

bool
CMutex::Unlock( void ) const
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    if ( ReleaseMutex( ((TMutexData*)_mutexdata)->id ) == FALSE ) return false;
    return true;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( pthread_mutex_unlock( &((TMutexData*)_mutexdata)->id ) < 0 ) return false;
    return true;

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
