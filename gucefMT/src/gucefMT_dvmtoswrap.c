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
#include <malloc.h>

#include "gucefMT_dvmtoswrap.h"         /* the function prototypes */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
  #include <Mmsystem.h>  /* needed for timeBeginPeriod() etc */
#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
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
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    DWORD threadid;
    HANDLE threadhandle;
    LPTHREAD_START_ROUTINE func;
    void* data;
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
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
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
static LARGE_INTEGER m_high_perf_timer_freq = { 0 };
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
ThreadDelay( UInt32 delay )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    Sleep( delay );
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
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
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
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
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    return td->threadid;
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    return (UInt32) td->thread;
    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadSuspend( struct SThreadData* td )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    return ( -1 != SuspendThread( td->threadhandle ) );
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadResume( struct SThreadData* td )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    return ( -1 != ResumeThread( td->threadhandle ) );
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadKill( struct SThreadData* td )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    if ( td != NULL )
    {
        UInt32 retval = TerminateThread( td->threadhandle ,
                                         1                );
        free( td );
        return retval;
    }
    return 0;
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
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
            Int32 timeout          )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    if ( timeout >= 0 )
    {
        return ( WAIT_OBJECT_0 == WaitForSingleObject( td->threadhandle ,
                                                       timeout          ) );
    }
    return ( WAIT_OBJECT_0 == WaitForSingleObject( td->threadhandle ,
                                                   INFINITE         ) );
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt32
GetCurrentTaskID( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    return GetCurrentThreadId();
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    return (UInt32) pthread_self();
    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt64
PrecisionTickCount( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
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
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

UInt64
PrecisionTimerResolution( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    ULARGE_INTEGER t;
    if ( QueryPerformanceFrequency( (LARGE_INTEGER*) &t ) == TRUE )
    {
        return t.QuadPart;
    }
    else
    {
        return 100; /* this is the WIN32 resolution of GetTickCount(); which has a time-slice size of about 10 ms */
    }
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

    #else
    #error unsupported target platform
    #endif
}

/*--------------------------------------------------------------------------*/

void
PrecisionDelay( UInt32 delay )
{
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )

        /*
         *      Original code obtained from http://www.geisswerks.com/ryan/FAQS/timing.html
         */

        /*
         *      note: Possible problem in some cases:
         *              http://support.microsoft.com/default.aspx?scid=KB;EN-US;Q274323&
         *              Performance counter value may unexpectedly leap forward
         */

        /*
         *      note: BE SURE YOU CALL timeBeginPeriod(1) at program startup!!!
         *      note: BE SURE YOU CALL timeEndPeriod(1) at program exit!!!
         *      note: that this code will require linking to winmm.lib !!!
         */

        Int32 ticks_passed;
        Int32 ticks_left;
        Int32 i;

        static LARGE_INTEGER m_prev_end_of_frame = { 0 };

        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);

        if (m_prev_end_of_frame.QuadPart != 0)
        {
                Int32 ticks_to_wait = (Int32)m_high_perf_timer_freq.QuadPart / delay;
                Int32 done = 0;
                do
                {
                        QueryPerformanceCounter(&t);

                        ticks_passed = (Int32)( (__int64)t.QuadPart - (__int64)m_prev_end_of_frame.QuadPart );
                        ticks_left = ticks_to_wait - ticks_passed;

                        if ( t.QuadPart < m_prev_end_of_frame.QuadPart )    /* time wrap */
                        {
                                done = 1;
                        }
                        if (ticks_passed >= ticks_to_wait)
                        {
                                done = 1;
                        }

                        if ( !done )
                        {
                                /*
                                 *      if > 0.002s left, do Sleep(1), which will actually sleep some
                                 *      steady amount, probably 1-2 ms,
                                 *      and do so in a nice way (cpu meter drops; laptop battery spared).
                                 *      otherwise, do a few Sleep(0)'s, which just give up the time slice,
                                 *      but don't really save cpu or battery, but do pass a tiny
                                 *     amount of time.
                                 */
                                if ( ticks_left > (Int32)m_high_perf_timer_freq.QuadPart*2/1000)
                                {
                                        Sleep(1);
                                }
                                else
                                {
                                        for ( i=0; i<10; ++i )
                                        {
                                                Sleep(0);  /* causes thread to give up its time slice */
                                        }
                                }
                        }
                }
                while (!done);
        }

        m_prev_end_of_frame = t;

        #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

        #else
        #error unsupported target platform
        #endif
}

/*--------------------------------------------------------------------------*/

void
PrecisionTimerInit( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    if ( m_high_perf_timer_freq.QuadPart == 0 )
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
        QueryPerformanceFrequency( &m_high_perf_timer_freq );
    }
    #endif
}

/*--------------------------------------------------------------------------*/

void
PrecisionTimerShutdown( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )
    if ( m_high_perf_timer_freq.QuadPart != 0 )
    {
        /*
         *      Undo timer resolution change
         */
        timeEndPeriod( 1 );
    }
    #endif
}

/*--------------------------------------------------------------------------*/
