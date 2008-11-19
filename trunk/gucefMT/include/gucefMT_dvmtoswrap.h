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
GUCEFMT_EXPORT_C void
ThreadDelay( UInt32 delay );

/*--------------------------------------------------------------------------*/

/**
 *      Creates a thread that uses the given function with the given data
 *      parameter. The SThreadData structure is deallocated automaticaly
 *      when the thread ends of when you call ThreadKill().
 *
 *      @param func pointer to the function that is to serve as the main thread routine
 *      @param data data argument for the thread function
 *      @return structure containing O/S specific thread data.
 */
GUCEFMT_EXPORT_C struct SThreadData*
ThreadCreate( void* func ,
              void* data );

/*--------------------------------------------------------------------------*/

GUCEFMT_EXPORT_C UInt32
ThreadID( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

GUCEFMT_EXPORT_C UInt32
ThreadSuspend( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

GUCEFMT_EXPORT_C UInt32
ThreadResume( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

GUCEFMT_EXPORT_C UInt32
ThreadKill( struct SThreadData* td );

/*--------------------------------------------------------------------------*/

GUCEFMT_EXPORT_C UInt32
ThreadWait( struct SThreadData* td ,
            Int32 timeout          );

/*--------------------------------------------------------------------------*/

GUCEFMT_EXPORT_C UInt32
GetCurrentTaskID( void );

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
GUCEFMT_EXPORT_C void
PrecisionDelay( UInt32 delay );

/*--------------------------------------------------------------------------*/

/**
 *  Returns the application tick count in ticks.
 *  The time a single tick represents can be obtained with PrecisionTimerResolution()
 */
GUCEFMT_EXPORT_C UInt64
PrecisionTickCount( void );

/*--------------------------------------------------------------------------*/

/**
 *  Returns the resolution of the precision timer in time-slices per second
 *  So a return value of 1000 would indicate the system's precision timer (if one is available)
 *  is capable of a precision in the millisecond range with each time-slice having a resolution
 *  of about 1 millisecond. This means that each 'tick' will count for about 1 milliseconds in this
 *  example.
 */
GUCEFMT_EXPORT_C UInt64
PrecisionTimerResolution( void );

/*--------------------------------------------------------------------------*/

/**
 *      Does whatever initialization is required for PrecisionDelay() and
 *      PrecisionTickCount() to perform their magic.
 *      MUST be called at application startup.
 *      Also see PrecisionTimerShutdown()
 */
GUCEFMT_EXPORT_C void
PrecisionTimerInit( void );

/*--------------------------------------------------------------------------*/

/**
 *      Cleans up whatever needs to be cleaned up after a class to 
 *      PrecisionTimerInit().
 *      MUST be called if you called PrecisionTimerInit() !!!
 *      call at application shutdown.
 */
GUCEFMT_EXPORT_C void
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
