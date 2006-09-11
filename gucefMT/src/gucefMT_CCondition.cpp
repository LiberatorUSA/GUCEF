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

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"          /* mutex class definition, needed for mutex locking */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_MT_CCONDITION_H
#include "gucefMT_CCondition.h"
#define GUCEF_MT_CCONDITION_H
#endif /* GUCEF_MT_CCONDITION_H ? */

//#include "SDL_mutex.h"

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

CCondition::CCondition( void )
{
        assert( 0 );
        //cond = SDL_CreateCond();
}

/*--------------------------------------------------------------------------*/

CCondition::~CCondition()
{
       /// SDL_DestroyCond( cond );
}

/*--------------------------------------------------------------------------*/

/**
 *      Restart one of the threads that are waiting on the condition
 *      variable, returns false on error.
 */
bool
CCondition::Signal( void )
{
        return false;//SDL_CondSignal( cond ) >= 0;
}

/*--------------------------------------------------------------------------*/

/**
 *      Restart all threads that are waiting on the condition variable
 *      returns false on error.
 */
bool
CCondition::Broadcast( void )
{
        return false; //SDL_CondBroadcast( cond ) >= 0;
}

/*--------------------------------------------------------------------------*/

/**
 *      Wait on the condition variable, unlocking the provided mutex.
 *      The mutex must be locked before entering this function!
 *      returns false on error.
 */
bool
CCondition::WaitAndUnlock( CMutex& mutex )
{
      //  return SDL_CondWait( cond        ,
      //                       mutex.mutex ) >= 0;
      return false;
}

/*--------------------------------------------------------------------------*/

/**
 *      Waits for at most 'ms' milliseconds, and returns 0 if the
 *      condition variable is signaled, 1 if the condition is not
 *      signaled in the allotted time, and -1 on error.
 *      On some platforms this function is implemented by looping with a
 *      delay of 1 ms, and so should be avoided if possible.
 */
Int32
CCondition::WaitAndUnlockOrTimeout( CMutex& mutex ,
                                    UInt32 ms     )
{
   /*     return SDL_CondWaitTimeout( cond        ,
                                    mutex.mutex ,
                                    ms          ); */
        return 0;                                   
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/


