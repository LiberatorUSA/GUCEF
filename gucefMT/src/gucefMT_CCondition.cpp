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


