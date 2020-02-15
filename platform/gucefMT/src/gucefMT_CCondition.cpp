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


