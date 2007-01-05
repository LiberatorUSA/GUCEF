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

#include <windows.h>

#include "gucefMT_dvmtoswrap.h"         /* the function prototypes */

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


static UInt32 GUCEF_CALLSPEC_PREFIX
ThreadMain( void* tdvptr ) GUCEF_CALLSPEC_SUFFIX
{
        UInt32 retval = ((TThreadData*)tdvptr)->func( ((TThreadData*)tdvptr)->data );
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
        #elif GUCEF_LINUX_BUILD

        #endif
};

/*--------------------------------------------------------------------------*/

UInt32
ThreadSuspend( struct SThreadData* td )
{
        #ifdef GUCEF_MSWIN_BUILD
        return ( -1 != SuspendThread( td->threadhandle ) );
        #elif GUCEF_LINUX_BUILD

        #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadResume( struct SThreadData* td )
{
        #ifdef GUCEF_MSWIN_BUILD
        return ( -1 != ResumeThread( td->threadhandle ) );
        #elif GUCEF_LINUX_BUILD

        #endif
}

/*--------------------------------------------------------------------------*/

UInt32
ThreadKill( struct SThreadData* td )
{
        #ifdef GUCEF_MSWIN_BUILD
        UInt32 retval = TerminateThread( td->threadhandle ,
                                         1                );
        free( td );
        return retval;
        #elif GUCEF_LINUX_BUILD

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
        #elif GUCEF_LINUX_BUILD

        #endif

}

/*--------------------------------------------------------------------------*/
