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

struct SMutexData
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    HANDLE id;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
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

CMutex::CMutex( void )
    : _mutexdata( NULL )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    TMutexData* md = new TMutexData;
    md->id = CreateMutex( NULL, FALSE, NULL );
    if ( !md->id )
    {
            delete md;
            return;
    }
    _mutexdata = md;
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    
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
