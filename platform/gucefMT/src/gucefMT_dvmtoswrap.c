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
#include <malloc.h>

#include "gucefMT_dvmtoswrap.h"         /* the function prototypes */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #include <Mmsystem.h>  /* needed for timeBeginPeriod() etc */
  #include <processthreadsapi.h>
#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <pthread.h>
  #include <signal.h>
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SThreadData
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    DWORD threadid;
    HANDLE threadhandle;
    LPTHREAD_START_ROUTINE func;
    void* data;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    pthread_t thread;
    TThreadFunc func;
    void* data;
    #endif
};
typedef struct SThreadData TThreadData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      needed for PrecisionDelay() and PrecisionTickCount()
 *      Initialized in PrecisionTimerInit()
 */
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
static LARGE_INTEGER highPerfTimerFrequency = { 0 };
static double highPerfTimerFrequencyTicksPerMs = 0;
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
ThreadDelay( UInt32 delay )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    Sleep( delay );
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    sleep( delay );
    #endif
}

/*--------------------------------------------------------------------------*/


static UInt32 GUCEF_CALLSPEC_STD_PREFIX
ThreadMain( void* tdvptr ) GUCEF_CALLSPEC_STD_SUFFIX
{
    UInt32 retval = ( (TThreadData*)tdvptr)->func( ((TThreadData*)tdvptr)->data );
    free( ((TThreadData*)tdvptr) );
    return retval;
}

/*--------------------------------------------------------------------------*/

TThreadData*
ThreadCreate( TThreadFunc func ,
              void* data       )
{
    #ifdef GUCEF_MSWIN_BUILD
    TThreadData* td = malloc( sizeof( TThreadData ) );
    td->func = (LPTHREAD_START_ROUTINE)func;
    td->data = data;
    td->threadhandle = CreateThread( NULL                                ,  /* Security Descriptor */
                                     0                                   ,  /* initial stack size */
                                     (LPTHREAD_START_ROUTINE) ThreadMain ,  /* thread function */
                                     (LPVOID) td                         ,  /* thread argument */
                                     0                                   ,  /* creation option */
                                     &td->threadid                       ); /* thread identifier */
    if ( td->threadhandle == NULL )
    {
        free( td );
        return NULL;
    }
    return td;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    TThreadData* td = malloc( sizeof( TThreadData ) );
    td->data = data;
    if ( 0 != pthread_create( &td->thread         ,
                              NULL                ,
                              (void*) ThreadMain  ,
                              (void*) td          ) )
    {
        free( td );
        return NULL;
    }
    return td;
    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C UInt32
ThreadID( struct SThreadData* td )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    return td->threadid;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    return (UInt32) td->thread;
    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadSuspend( struct SThreadData* td )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    return ( -1 != SuspendThread( td->threadhandle ) );
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadResume( struct SThreadData* td )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    return ( -1 != ResumeThread( td->threadhandle ) );
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadKill( struct SThreadData* td )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    if ( td != NULL )
    {
        UInt32 retval = TerminateThread( td->threadhandle ,
                                         1                );
        free( td );
        return retval;
    }
    return 0;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    if ( td != NULL )
    {
        Int32 retval = (Int32) pthread_kill( td->thread, 0 );
        if ( 0 != retval )
        {
            /* an error occured */

        }
        free( td );
        return retval;
    }
    return 0;
    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadWait( struct SThreadData* td ,
            Int32 timeoutInMs      )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    if ( NULL != td && NULL != td->threadhandle )
    {
        if ( timeoutInMs >= 0 )
        {
            if ( WAIT_OBJECT_0 == WaitForSingleObject( td->threadhandle ,
                                                       timeoutInMs      ) )
                return 1;
            else
                return 0;
        }
        if ( WAIT_OBJECT_0 == WaitForSingleObject( td->threadhandle ,
                                                   INFINITE         ) )
            return 1;
        else
            return 0;
    }
    return 0;
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
GetCurrentTaskID( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    return GetCurrentThreadId();
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    return (UInt32) pthread_self();
    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt64
PrecisionTickCount( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    LARGE_INTEGER t;

    assert( sizeof( LARGE_INTEGER ) <= sizeof( UInt64 ) );

    if ( QueryPerformanceCounter( &t ) == TRUE )
    {
        return t.QuadPart;
    }
    else
    {
        return GetTickCount();
    }
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    struct timeval time;
    gettimeofday( &time, NULL );

    return time.tv_usec +    /* Microseconds. */
           time.tv_sec * 1000000; /* Seconds. */

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt64
PrecisionTimerResolution( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    ULARGE_INTEGER t;
    if ( QueryPerformanceFrequency( (LARGE_INTEGER*) &t ) == TRUE )
    {
        return t.QuadPart;
    }
    else
    {
        return 100; /* this is the WIN32 resolution of GetTickCount(); which has a time-slice size of about 10 ms */
    }
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return 1000000;

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

void
PrecisionDelay( UInt32 delayInMs )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    /*
     *      note: BE SURE YOU CALL timeBeginPeriod(1) at program startup!!!
     *      note: BE SURE YOU CALL timeEndPeriod(1) at program exit!!!
     *      note: that this code will require linking to winmm.lib
     */

    LARGE_INTEGER startTime;
    LARGE_INTEGER currentTime;
    LONGLONG ticksPassed;
    LONGLONG ticksToWait;
    LONGLONG ticksLeft;
    BOOL qpcSuccess;
    int done = 0;

    qpcSuccess = QueryPerformanceCounter( &startTime );
    if ( FALSE == qpcSuccess )
    {
        /*
         *  Fall back to regular sleep
         */
         Sleep( delayInMs );
         return;
    }
             
    ticksToWait = (LONGLONG) ( highPerfTimerFrequencyTicksPerMs * delayInMs );

    while ( 1 )
    {
        QueryPerformanceCounter( &currentTime );
        ticksPassed = (LONGLONG) ( currentTime.QuadPart - startTime.QuadPart );
        if ( ticksPassed < 0 )
            return;
        
        ticksLeft = (LONGLONG) ( ticksToWait - ticksPassed );
        if ( ticksLeft <= 0 )
            return;

        if ( ticksLeft > (Int32) ( highPerfTimerFrequencyTicksPerMs * 50 ) )
        {
            // Lets not hog the CPU if the time we have to wait is lengthy
            UInt32 longSleepTimeInMs = (UInt32) ( ( ticksLeft - (highPerfTimerFrequencyTicksPerMs*15) ) / highPerfTimerFrequencyTicksPerMs );
            Sleep( longSleepTimeInMs );
        }
        else        
        if ( ticksLeft > (Int32) ( highPerfTimerFrequencyTicksPerMs * 20 ) )
        {
            Sleep( 10 );
        }
        else

        /*
         *      if > 2ms left, do Sleep(1), which will actually sleep some
         *      steady amount, probably 1-2 ms,
         *      and do so in a nice way (cpu meter drops; laptop battery spared).
         *      otherwise, do a few Sleep(0)'s, which just give up the time slice,
         *      but don't really save cpu or battery, but do pass a tiny
         *     amount of time.
         */
        if ( ticksLeft > (Int32) ( highPerfTimerFrequencyTicksPerMs * 2 ) )
        {
            Sleep( 1 );
        }
        else
        {
            int i;
            for ( i=0; i<10; ++i )
            {
                Sleep( 0 );  /* causes thread to give up its time slice */
            }
            return;
        }
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    usleep( delay * 1000 );

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

void
PrecisionTimerInit( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    if ( highPerfTimerFrequency.QuadPart == 0 )
    {
        /*
         *      Change Sleep() resolution to 1-2 milliseconds
         *
         *      Note that calling timeBeginPeriod() also affects the granularity of some
         *      other timing calls, such as CreateWaitableTimer() and WaitForSingleObject();
         *      however, some functions are still unaffected, such as _ftime().
         */
        timeBeginPeriod( 1 );

        /*
         *      Initialize the timer frequency structure for use
         */
        QueryPerformanceFrequency( &highPerfTimerFrequency );
        highPerfTimerFrequencyTicksPerMs = (double) ( highPerfTimerFrequency.QuadPart / 1000.0 );
    }
    #endif
}

/*--------------------------------------------------------------------------*/

void
PrecisionTimerShutdown( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    if ( highPerfTimerFrequency.QuadPart != 0 )
    {
        /*
         *      Undo timer resolution change
         */
        timeEndPeriod( 1 );
    }
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32 
GetProcessID( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    return (UInt32) GetCurrentProcessId();
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
    return (UInt32) getpid();
    #else
    #error unsupported target platform
    return 0;
    #endif
}

/*--------------------------------------------------------------------------*/
