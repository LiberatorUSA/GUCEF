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
{
      assert( 0 ); 
       // sem = SDL_CreateSemaphore( initial_value );
}

/*--------------------------------------------------------------------------*/

/**
 *      Destructor, de-allocates storage for a semaphore.
 */
CSemaphore::~CSemaphore()
{
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
{
        return false;// SDL_SemWait( sem ) == 1;
}

/*--------------------------------------------------------------------------*/

/**
 *      Non-blocking variant of Wait(), returns 0 if the wait succeeds,
 *      1 if the wait would block, and -1 on error.
 */
Int32
CSemaphore::TryWait( void )
{
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
{
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
{
        return false;//SDL_SemPost( sem ) > 0;       
}

/*--------------------------------------------------------------------------*/

/**
 *      Returns the current count of the semaphore
 */
UInt32
CSemaphore::Value( void ) const
{
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
