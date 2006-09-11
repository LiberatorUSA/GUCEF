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
