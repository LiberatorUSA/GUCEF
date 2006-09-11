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

#include <malloc.h>                   /* memory management */

#ifndef GUCEF_MT_MUTEX_H
#include "gucefMT_mutex.h"            /* O/S mutex wrapper */
#define GUCEF_MT_MUTEX_H
#endif /* GUCEF_MT_MUTEX_H ? */

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"       /* wrappers for threading related O/S functionality */  
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */ 

#include "gucefMT_dvrwlock.h"         /* rwlock function prototypes */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *	Readers/Writers lock data storage.
 */
struct SRWLock
{
	Ubyte  delflag;
        Ubyte  wflag;
	UInt32 rcount;
        UInt32 wcount;
        UInt32 wpriority;
        struct SMutex* datalock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *	Function that creates a readers/writers lock data storage struct
 *      writer_overrules is a boolean. When non zero writers will be given
 *	a higher priority then readers. If 0 then readers will have priority
 *	over writers.
 */
TRWLock*
rwl_create( UInt32 writer_overrules )
{
	TRWLock *rwlock = malloc( sizeof( TRWLock ) );
        rwlock->delflag = 0;
        rwlock->rcount = 0;
        rwlock->wcount = 0;
        rwlock->wflag = 0;
        rwlock->wpriority = writer_overrules;
        rwlock->datalock = MutexCreate();
        return rwlock;
}

/*-------------------------------------------------------------------------*/

/**
 *	Cleanup a readers/writers lock.
 */
void
rwl_destroy( TRWLock *rwlock )
{
	/*
         *	Set the delete flag.
         */
	rwlock->delflag = 1;

        /*
         *	Wait for all readers and all writers to finish
         */
        while ( rwlock->rcount || rwlock->wcount )
        {
        	ThreadDelay( 10 );
        }

        /*
         *	Cleanup allocated storage
         */
        MutexDestroy( rwlock->datalock );
        free( rwlock );
}

/*-------------------------------------------------------------------------*/

/**
 *	Returns a boolean value. 1 if writers have priority and 0 if readers
 *	have priority.
 */
UInt32
rwl_writer_overrules( const TRWLock *rwlock )
{
	/*
         *	Atomic, just return the priority 'bool'
         */
	return rwlock->wpriority;
}

/*--------------------------------------------------------------------------*/

/**
 *	Returns the current amount of writers. Note that this function
 *	is meant for output to humans. It has no use in relation with
 *	the other rwl_ functions.
 */
UInt32
rwl_writers( const TRWLock *rwlock )
{
	return rwlock->wcount;
}

/*-------------------------------------------------------------------------*/

/**
 *	Returns the current amount of readers. Note that this function
 *	is meant for output to humans. It has no use in relation with
 *	the other rwl_ functions.
 */
UInt32
rwl_readers( const TRWLock *rwlock ) 
{
	return rwlock->rcount;
}

/*--------------------------------------------------------------------------*/

/**
 *	Call this function when a reader task needs access to the data
 *	protected by the given rwlock.
 */
UInt32
rwl_reader_start( TRWLock *rwlock )
{
	/*
         *	First we check if destroy has not been called.
         *	In a propper design this test should never fail.
         */
	if ( rwlock->delflag ) return 0;

        /*
         *	How we behave depends on wheter readers have priority or
         *	wheter writers have priority.
         */
        if ( rwlock->wpriority )
        {
        	/*
                 *	If writers have priority we will wait for all writers
                 *	to finish this means the current writer and any writer
                 *	that may be queued. After that we increase the reader
                 *	counter.
                 */
                while ( rwlock->wcount )
                {
                        ThreadDelay( 10 );
                }
                rwlock->rcount++;
        }
        else
        {
        	/*
                 *	If readers have priority then we will increase
                 *	the readers count first. This causes any new writer
                 *	to wait for the readers to finish. After that we wait
                 *	for any writer that may already be busy to finish.
                 */
        	rwlock->rcount++;
                while ( rwlock->wflag )
                {
                        ThreadDelay( 10 );
                }
        }

        /*
         *	Start reading
         */
        return 1;
}

/*--------------------------------------------------------------------------*/

/**
 *	Call this function when a reader task finished using data that is
 *	protected by the given rwlock.
 */
void
rwl_reader_stop( TRWLock *rwlock )
{
	/*
         *	Atomic, decrement readers count
         */
	rwlock->rcount--;
}

/*--------------------------------------------------------------------------*/

/**
 *	Call this function when a writer task needs access to the data
 *	protected by the given rwlock.
 */
UInt32
rwl_writer_start( TRWLock *rwlock )
{
	/*
         *	First we check if destroy has not been called.
         *	In a propper design this test should never fail.
         */
	if ( rwlock->delflag ) return 0;

	rwlock->wcount++;

        /*
	 *	We will wait for all readers to finish.
         *	A writer must have exclusive access.
         */
        while ( rwlock->rcount )
        {
                ThreadDelay( 10 );
        }

        /*
         *	Lock mutex for exclusive access and set the writer in progress
         *	flag.
         */
        rwlock->wflag = 1;
        MutexLock( rwlock->datalock );

        /*
         *	Start writing
         */
        return 1;
}

/*--------------------------------------------------------------------------*/

/**
 *	Call this function when a writer task finished using data that is
 *	protected by the given rwlock.
 */
void
rwl_writer_stop( TRWLock *rwlock )
{
	rwlock->wcount--;
        rwlock->wflag = 0;
        MutexUnlock( rwlock->datalock );
}

/*--------------------------------------------------------------------------*/


