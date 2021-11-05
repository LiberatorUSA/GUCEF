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

#ifndef GUCEF_MT_CSEMAPHORE_H
#include "gucefMT_CSemaphore.h"
#endif /* GUCEF_MT_CSEMAPHORE_H ? */

//#ifndef _SDL_mutex_h
//#include "SDL_mutex.h"  /* SDL O/S Mutex wrapper */
//#endif /* _SDL_mutex_h ? */

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"     /* Our mutex cless definition */
#endif /* GUCEF_MT_CMUTEX_H ? */

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
 *      constructor, allocates storage for a semaphore.
 */
CSemaphore::CSemaphore( UInt32 initial_value )
{GUCEF_TRACE;

      assert( 0 ); 
       // sem = SDL_CreateSemaphore( initial_value );
}

/*--------------------------------------------------------------------------*/

/**
 *      Destructor, de-allocates storage for a semaphore.
 */
CSemaphore::~CSemaphore()
{GUCEF_TRACE;

       // SDL_DestroySemaphore( sem );
}

/*--------------------------------------------------------------------------*/

/**
 *      Suspends the calling thread until the semaphore has a positive
 *      count. It then atomically decreases the semaphore count.
 *      returns false in case of error.
 */
bool
CSemaphore::Wait( void )
{GUCEF_TRACE;

        return false;// SDL_SemWait( sem ) == 1;
}

/*--------------------------------------------------------------------------*/

/**
 *      Non-blocking variant of Wait(), returns 0 if the wait succeeds,
 *      1 if the wait would block, and -1 on error.
 */
Int32
CSemaphore::TryWait( void )
{GUCEF_TRACE;

        return 0;//SDL_SemTryWait( sem );
}

/*--------------------------------------------------------------------------*/

/**
 *      Variant of Wait() with a timeout in milliseconds, returns 0 if
 *      the wait succeeds, 1 if the wait does not succeed in
 *      the allotted time, and -1 on error. On some platforms this
 *      function is implemented by looping with a delay of 1 ms, and so
 *      should be avoided if possible.
 */
Int32
CSemaphore::WaitOrTimeout( UInt32 ms )
{GUCEF_TRACE;

        return 0;// SDL_SemWaitTimeout( sem ,
                   //                ms  );
}

/*--------------------------------------------------------------------------*/

/**
 *      Atomically increases the semaphore's count (not blocking)
 *      returns false in case of error.
 */
bool
CSemaphore::Post( void )
{GUCEF_TRACE;

        return false;//SDL_SemPost( sem ) > 0;       
}

/*--------------------------------------------------------------------------*/

/**
 *      Returns the current count of the semaphore
 */
UInt32
CSemaphore::Value( void ) const
{GUCEF_TRACE;

        return false; //SDL_SemValue( (SDL_sem*) sem );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
