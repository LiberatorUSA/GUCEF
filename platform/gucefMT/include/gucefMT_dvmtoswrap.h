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

#ifndef GUCEF_MT_DVMTOSWRAP_H
#define GUCEF_MT_DVMTOSWRAP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"             /* Simple types used */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H */

#ifndef GUCEF_MT_MACROS_H
#include "gucefMT_macros.h"             /* module config */
#define GUCEF_MT_MACROS_H
#endif /* GUCEF_MT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace MT {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct TThreadData;

typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TThreadFunc)( void* ) GUCEF_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ Name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*--------------------------------------------------------------------------*/

/**
 *      Delays the caller thread for a minimum of 'delay' number of milliseconds
 *
 *      @param delay the minimum delay in miliseconds
 */
GUCEF_MT_PUBLIC_C void
ThreadDelay( UInt32 delay );

/*--------------------------------------------------------------------------*/

/**
 *      Reserves a storage structure with sufficient space to hold thread data
 *      To be used to create the storage passed to ThreadCreate()
 * 
 *      @return structure reserved for containing O/S specific thread data.
 */
GUCEF_MT_PUBLIC_C struct SThreadData*
ThreadDataReserve( void );

/*--------------------------------------------------------------------------*/

/**
 *      Cleans up the storage structure allocated by ThreadDataReserve()
 * 
 *      @return structure reserved for containing O/S specific thread data.
 */
GUCEF_MT_PUBLIC_C void
ThreadDataCleanup( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

/**
 *      Creates a thread that uses the given function with the given data
 *      parameter. 
 *
 *      @param td structure containing O/S specific thread data.
 *      @param func pointer to the function that is to serve as the main thread routine
 *      @param data data argument for the thread function
 *      @return integer boolean with 1 for success and 0 for failure
 */
GUCEF_MT_PUBLIC_C UInt32
ThreadCreate( struct SThreadData* td ,
              TThreadFunc func       ,
              void* data             );

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
ThreadID( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
ThreadSuspend( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
ThreadResume( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
ThreadKill( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
ThreadWait( struct SThreadData* td ,
            Int32 timeoutInMs      );

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
ThreadSetCpuAffinity( struct SThreadData* td  ,
                      UInt32 affinityMaskSize ,
                      void* affinityMask      );

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
GetCurrentTaskID( void );

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
GetProcessID( void );

/*--------------------------------------------------------------------------*/

/**
 *      Very accurate delay mechanism.
 *      Uses the target O/S to attempt to delay
 *      for the given number of milliseconds as accurately as possible.
 *      The CPU will be yielded whenever possible as long as possible
 *      while attempting to guarantee a delay as close to the requested number
 *      of milliseconds as possible.
 *
 *      This function requires you to call PrecisionTimerInit() at
 *      application startup and PrecisionTimerShutdown() at application
 *      shutdown.
 *
 *      Note that for the MSWIN platform you will have to link to winmm.lib
 *      The code used for that platform is not linked to by default
 *
 *      @param delay the number of milliseconds you wish to delay the caller thread
 */
GUCEF_MT_PUBLIC_C void
PrecisionDelay( UInt32 delayInMs );

/*--------------------------------------------------------------------------*/

/**
 *  Returns the application tick count in ticks.
 *  The time a single tick represents can be obtained with PrecisionTimerResolution()
 */
GUCEF_MT_PUBLIC_C UInt64
PrecisionTickCount( void );

/*--------------------------------------------------------------------------*/

/**
 *  Returns the resolution of the precision timer in time-slices per second
 *  So a return value of 1000 would indicate the system's precision timer (if one is available)
 *  is capable of a precision in the millisecond range with each time-slice having a resolution
 *  of about 1 millisecond. This means that each 'tick' will count for about 1 milliseconds in this
 *  example.
 */
GUCEF_MT_PUBLIC_C UInt64
PrecisionTimerResolution( void );

/*--------------------------------------------------------------------------*/

/**
 *      Does whatever initialization is required for PrecisionDelay() and
 *      PrecisionTickCount() to perform their magic.
 *      MUST be called at application startup.
 *      Also see PrecisionTimerShutdown()
 */
GUCEF_MT_PUBLIC_C void
PrecisionTimerInit( void );

/*--------------------------------------------------------------------------*/

/**
 *      Cleans up whatever needs to be cleaned up after a class to
 *      PrecisionTimerInit().
 *      MUST be called if you called PrecisionTimerInit() !!!
 *      call at application shutdown.
 */
GUCEF_MT_PUBLIC_C void
PrecisionTimerShutdown( void );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace MT */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-02-2005 :
       - Initial implementation.

---------------------------------------------------------------------------*/
