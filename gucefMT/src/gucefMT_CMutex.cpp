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

#include "gucefMT_CMutex.h"     /* Our mutex class definition */

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

struct SMutexData
{
        #ifdef GUCEF_MSWIN_BUILD
        HANDLE id;
        #elif GUCEF_LINUX_BUILD
        pthread_mutex_t id;
        #else
        #error Unsuported target platform
        #endif
};

typedef struct SMutexData TMutexData;

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

/**
 *      Default constructor, allocates storage for a mutex.
 */
CMutex::CMutex( void )
        : _mutexdata( NULL )
{
        #ifdef GUCEF_MSWIN_BUILD
        TMutexData* md = new TMutexData;
        md->id = CreateMutex( NULL, FALSE, NULL );
        if ( !md->id )
        {
                delete md;
                return;
        }
        _mutexdata = md;
        #elif GUCEF_LINUX_BUILD
        TMutexData* md = new TMutexData;
        pthread_mutexattr_t attr;

        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

        if ( pthread_mutex_init( &md->id, &attr ) != 0 )
        {
                delete md;
                return;
        }
        _mutexdata = md;
        #endif
}

/*--------------------------------------------------------------------------*/

/**
 *      Destructor, de-allocates storage for a mutex.
 */
CMutex::~CMutex()
{
        #ifdef GUCEF_MSWIN_BUILD
        CloseHandle( ((TMutexData*)_mutexdata)->id );
        delete (TMutexData*)_mutexdata;
        #elif GUCEF_LINUX_BUILD
        pthread_mutex_destroy( &((TMutexData*)_mutexdata)->id );
        delete (TMutexData*)_mutexdata;
        #endif
}

/*--------------------------------------------------------------------------*/

/**
 *      Lock the mutex. If the mutex is already locked the calling
 *      process will have to wait for the mutex to allow a lock.
 *      The return value indicates wheter the lock failed or succeeded.
 */
bool
CMutex::Lock( void ) const
{
        #ifdef GUCEF_MSWIN_BUILD
        if ( WaitForSingleObject( ((TMutexData*)_mutexdata)->id ,
                                  INFINITE                      ) == WAIT_FAILED ) return false;
        return true;
        #elif GUCEF_LINUX_BUILD
        if ( pthread_mutex_lock( &((TMutexData*)_mutexdata)->id ) < 0 ) return false;
        return true;
        #endif
}

/*--------------------------------------------------------------------------*/

/**
 *      Unlocks the mutex after a call to Lock_Mutex(). Other processes
 *      will have the ability to get a mutex lock after this call.
 *      The return value indicates wheter the unlock failed or succeeded.
 */
bool
CMutex::Unlock( void ) const
{
        #ifdef GUCEF_MSWIN_BUILD
        if ( ReleaseMutex( ((TMutexData*)_mutexdata)->id ) == FALSE ) return false;
        return true;
        #elif GUCEF_LINUX_BUILD
        if ( pthread_mutex_unlock( &mutex->id ) < 0 ) return false;
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
