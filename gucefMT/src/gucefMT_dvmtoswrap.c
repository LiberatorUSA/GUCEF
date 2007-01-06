/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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
#include <malloc.h>

#include "gucefMT_dvmtoswrap.h"         /* the function prototypes */

#ifdef GUCEF_MSWIN_BUILD
  #include <Mmsystem.h>  /* needed for timeBeginPeriod() etc */
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/
               
struct SThreadData
{
        #ifdef GUCEF_MSWIN_BUILD
        DWORD threadid;
        HANDLE threadhandle;
        LPTHREAD_START_ROUTINE func;
        void *data;
        #elif GUCEF_LINUX_BUILD

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
#ifdef GUCEF_MSWIN_BUILD
static LARGE_INTEGER m_high_perf_timer_freq = { 0 };
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Delays the caller thread for a minimum of 'delay' number of milliseconds
 *
 *      @param delay the minimum delay in miliseconds
 */
void
ThreadDelay( UInt32 delay )
{
        #ifdef GUCEF_MSWIN_BUILD
        Sleep( delay );
        #elif GUCEF_LINUX_BUILD

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

/**
 *      Creates a thread that uses the given function with the given data
 *      parameter.
 *
 *      @param func pointer to the function that is to serve as the main thread routine
 *      @param data data argument for the thread function
 *      @return structure containing O/S specific thread data.
 */
TThreadData*
ThreadCreate( void* func ,
              void* data )
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
        #else
        #error unsupported target platform
        #endif
};

/*--------------------------------------------------------------------------*/

UInt32
ThreadSuspend( struct SThreadData* td )
{
        #ifdef GUCEF_MSWIN_BUILD
        return ( -1 != SuspendThread( td->threadhandle ) );
        #else
        #error unsupported target platform
        #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadResume( struct SThreadData* td )
{
        #ifdef GUCEF_MSWIN_BUILD
        return ( -1 != ResumeThread( td->threadhandle ) );
        #else
        #error unsupported target platform
        #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadKill( struct SThreadData* td )
{
    #ifdef GUCEF_MSWIN_BUILD
    if ( td != NULL )
    {
        UInt32 retval = TerminateThread( td->threadhandle ,
                                         1                );
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
        #ifdef GUCEF_MSWIN_BUILD
        if ( timeout >= 0 )
        {
                return ( WAIT_OBJECT_0 == WaitForSingleObject( td->threadhandle ,
                                                               timeout          ) );
        }
        return ( WAIT_OBJECT_0 == WaitForSingleObject( td->threadhandle ,
                                                       INFINITE         ) );
        #else
        #error unsupported target platform
        #endif
}

/*--------------------------------------------------------------------------*/

UInt32
GetCurrentTaskID( void )
{
        #ifdef GUCEF_MSWIN_BUILD
        return GetCurrentThreadId();
        #else
        #error unsupported target platform
        #endif
}

/*--------------------------------------------------------------------------*/

UInt64
PrecisionTickCount( void )
{         
    #ifdef GUCEF_MSWIN_BUILD
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
    #else
    #error unsupported target platform
    #endif    
}

/*--------------------------------------------------------------------------*/

UInt64
PrecisionTimerResolution( void )
{
    #ifdef GUCEF_MSWIN_BUILD
    LARGE_INTEGER t;
    if ( QueryPerformanceFrequency( &t ) == TRUE )
    {
        return t.QuadPart;
    }
    else
    {
        return 100; /* this is the WIN32 resolution of GetTickCount(); which has a time-slice size of about 10 ms */
    }
    #else
    #error unsupported target platform
    #endif 
}

/*--------------------------------------------------------------------------*/

void
PrecisionDelay( UInt32 delay )
{
        #ifdef GUCEF_MSWIN_BUILD
        
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
        
        #else
        #error unsupported target platform
        #endif        
}

/*--------------------------------------------------------------------------*/

void
PrecisionTimerInit( void )
{
    #ifdef GUCEF_MSWIN_BUILD        
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
        #ifdef GUCEF_MSWIN_BUILD
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
