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
#include <malloc.h>                   /* memory management */

#ifndef GUCEF_MT_MUTEX_H
#include "gucefMT_mutex.h"            /* O/S mutex wrapper */
#define GUCEF_MT_MUTEX_H
#endif /* GUCEF_MT_MUTEX_H ? */

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"       /* wrappers for threading related O/S functionality */
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#include "gucefMT_DVRWLOCK.h"         /* rwlock function prototypes */

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
    UInt8  delflag;
    Int32 activeReaderCount;
    Int32 queuedReaderCount;
    Int32 activeWriterCount;
    Int32 activeWriterReentrancyCount;
    Int32 queuedWriterCount;
    UInt8 writeLockAquisitionInProgress;
    UInt32 writeLockAquisitionThreadId;
    UInt32 wpriority;
    UInt32 lastWriterThreadId;
    struct SMutex* datalock;
    struct SMutex* writerlock;
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
	TRWLock* rwlock = malloc( sizeof( TRWLock ) );
    if ( GUCEF_NULL != rwlock )
    {
        rwlock->delflag = 0;
        rwlock->activeReaderCount = 0;
        rwlock->queuedReaderCount = 0;
        rwlock->activeWriterCount = 0;
        rwlock->activeWriterReentrancyCount = 0;
        rwlock->queuedWriterCount = 0;
        rwlock->writeLockAquisitionInProgress = 0;
        rwlock->writeLockAquisitionThreadId = 0;
        rwlock->wpriority = writer_overrules;
        rwlock->lastWriterThreadId = 0;
        rwlock->datalock = MutexCreate();
        rwlock->writerlock = MutexCreate();
    }
    return rwlock;
}

/*-------------------------------------------------------------------------*/

void
rwl_destroy( TRWLock* rwlock )
{
	if ( GUCEF_NULL != rwlock )
    {
        do 
        {
            if  ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                /*
                 *	Set the delete flag.
                 */
                rwlock->delflag = 1;

                if ( MutexUnlock( rwlock->datalock ) > 0 )
                {                
                    do
                    {
                        /*
                         *	Wait data lock free for all readers and all writers to finish
                         */
                        while ( rwlock->activeReaderCount > 0              || 
                                rwlock->activeWriterCount > 0              ||
                                rwlock->queuedReaderCount > 0              ||
                                rwlock->queuedWriterCount > 0              ||
                                0 != rwlock->writeLockAquisitionInProgress  )
                        {
                            PrecisionDelay( 25 );
                        }

                        /*
                         *	Re-verify with the data lock that all readers and all writers are finished
                         *  If this comes to a different conclusion just go for another round
                         */
                        if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                        {
                            if ( 0 == rwlock->activeReaderCount             &&
                                 0 == rwlock->activeWriterCount             &&
                                 0 == rwlock->queuedReaderCount             &&
                                 0 == rwlock->queuedWriterCount             &&
                                 0 == rwlock->writeLockAquisitionInProgress )
                            {
                                /*
                                 *	Cleanup allocated storage
                                 */
                                MutexDestroy( rwlock->writerlock );
                                MutexDestroy( rwlock->datalock );
                                free( rwlock );
                                return;
                            }
                            else
                            {
                                MutexUnlock( rwlock->datalock );
                            }
                        }
                    }
                    while ( 1 > 0 );
                }
            }            
        }
        while ( 1 > 0 );
    }
}

/*-------------------------------------------------------------------------*/

UInt32
rwl_writer_overrules( const TRWLock* rwlock )
{
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                UInt32 wpriority = rwlock->wpriority;
                MutexUnlock( rwlock->datalock );
                return wpriority;
            }
        }
        while ( 0 == rwlock->delflag );
    }
    return 1;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_active_writers( const TRWLock* rwlock )
{
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                UInt32 activeWriterCount = rwlock->activeWriterCount;
                MutexUnlock( rwlock->datalock );
                return activeWriterCount;
            }
        }
        while ( 0 == rwlock->delflag );
    }
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_active_writer_reentrancy_depth( const TRWLock *rwlock )
{
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                UInt32 activeWriterReentrancyCount = rwlock->activeWriterReentrancyCount;
                MutexUnlock( rwlock->datalock );
                return activeWriterReentrancyCount;
            }
        }
        while ( 0 == rwlock->delflag );
    }
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_queued_writers( const TRWLock* rwlock )
{
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                UInt32 queuedWriterCount = rwlock->queuedWriterCount;
                MutexUnlock( rwlock->datalock );
                return queuedWriterCount;
            }
        }
        while ( 0 == rwlock->delflag );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
rwl_active_readers( const TRWLock *rwlock )
{
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                UInt32 activeReaderCount = rwlock->activeReaderCount;
                MutexUnlock( rwlock->datalock );
                return activeReaderCount;
            }
        }
        while ( 0 == rwlock->delflag );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
rwl_queued_readers( const TRWLock *rwlock )
{
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                UInt32 queuedReaderCount = rwlock->queuedReaderCount;
                MutexUnlock( rwlock->datalock );
                return queuedReaderCount;
            }
        }
        while ( 0 == rwlock->delflag );
    }
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_reader_start( TRWLock* rwlock, UInt32 lockWaitTimeoutInMs )
{
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            UInt8 done = 0;
            UInt32 lockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs ); 
            switch ( lockResult )
            {
	            case GUCEF_MUTEX_OPERATION_SUCCESS :
                case GUCEF_MUTEX_ABANDONED :
                {
                
                    /*
                     *	First we check if destroy has not been called.
                     */
	                if ( 0 != rwlock->delflag )
                    {                
                        MutexUnlock( rwlock->datalock );
                        return GUCEF_RWLOCK_OPERATION_FAILED;
                    }

                    if ( 0 == rwlock->activeWriterCount && 0 == rwlock->queuedWriterCount && 0 == rwlock->writeLockAquisitionInProgress )
                    {
                        /*
                         *  There are no active writers so regardless who takes priority we can proceed
                         */
                        rwlock->activeReaderCount++;        
                        MutexUnlock( rwlock->datalock );
                        return GUCEF_RWLOCK_OPERATION_SUCCESS;                    
                    }

                    if ( 0 == rwlock->wpriority && 0 == rwlock->activeWriterCount && 0 == rwlock->writeLockAquisitionInProgress )
                    {
                        /*
                         *  There are no active writers and readers are taking priority so any queued writers will have to wait
                         */
                        rwlock->activeReaderCount++;        
                        MutexUnlock( rwlock->datalock );
                        return GUCEF_RWLOCK_OPERATION_SUCCESS; 
                    }

                    if ( rwlock->activeWriterCount > 0 )
                    {
                        /*
                         *  The currently active writer could actually be the same thread
                         *  trying to obtain a read lock in another scope, acting as a reader.
                         *  We have to check for this or the thread can self-deadlock
                         */
                        if ( rwlock->lastWriterThreadId == GetCurrentTaskID() )
                        {
                            /*
                             *  The reader thread is also the writer thread
                             *  allow it to proceed since we already have write access anyway
                             *  Treat it as writer reentrancy, in essense taking another write lock
                             */
                            ++rwlock->activeWriterReentrancyCount;
                            MutexUnlock( rwlock->datalock );
                            return GUCEF_RWLOCK_OPERATION_SUCCESS;
                        }
                    }                    
                    
                    /*
                     *	How we behave depends on wheter readers have priority or
                     *	wheter writers have priority.
                     */
                    if ( rwlock->wpriority > 0 )
                    {
                        rwlock->queuedReaderCount++;
                        MutexUnlock( rwlock->datalock );

                        done = 0;
                        do
                        {
                            /*
                             *	If writers have priority we will wait for all writers
                             *	to finish this means the current writer and any writer
                             *	that may be queued. After that we increase the reader
                             *	counter.
                             */
                            Float64 elapsedTimeInMs = 0;
                            Float64 timerResolution = PrecisionTimerResolution() / 1000.0;
                            UInt64 startTicks = PrecisionTickCount();
                            while ( ( rwlock->activeWriterCount > 0 || rwlock->queuedWriterCount > 0 || 0 != rwlock->writeLockAquisitionInProgress ) && 
                                    ( 0 == rwlock->delflag )                                                                                         &&
                                    ( elapsedTimeInMs <= lockWaitTimeoutInMs )                                                                        )
                            {
                                PrecisionDelay( 25 );
                                elapsedTimeInMs = ( PrecisionTickCount() - startTicks ) / timerResolution;
                                if ( elapsedTimeInMs > lockWaitTimeoutInMs )
                                {
                                    /*
                                     *	We exceeded the max time we can wait for the writers to finish their work
                                     *  Unroll the administrative changes and return a timeout
                                     */  
                                    lockResult = MutexLock( rwlock->datalock, GUCEF_MT_SUPER_LONG_LOCK_TIMEOUT ); 
                                    switch ( lockResult )
                                    {
	                                    case GUCEF_MUTEX_OPERATION_SUCCESS :
                                        case GUCEF_MUTEX_ABANDONED :
                                        {
                                            rwlock->queuedReaderCount--;
                                            MutexUnlock( rwlock->datalock );
                                            return GUCEF_RWLOCK_WAIT_TIMEOUT;
                                        }
                                        default:
                                        case GUCEF_MUTEX_WAIT_TIMEOUT:
                                        case GUCEF_MUTEX_OPERATION_FAILED:
                                        {
                                            /*
                                             *	This should not happen
                                             *  Roll the dice on updating without a lock
                                             */
                                            GUCEF_ASSERT_ALWAYS;
                                            rwlock->queuedReaderCount--;
                                            return GUCEF_RWLOCK_WAIT_TIMEOUT;
                                        }
                                    }
                                }
                            }

                            /*
                             *	Re-verify with the data lock that all writers are finished
                             *  If this comes to a different conclusion just go for another round
                             */
                            lockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs );
                            switch ( lockResult )
                            {
	                            case GUCEF_MUTEX_OPERATION_SUCCESS :
                                case GUCEF_MUTEX_ABANDONED :
                                {
                                    if ( 0 == rwlock->activeWriterCount && 0 == rwlock->queuedWriterCount && 0 == rwlock->writeLockAquisitionInProgress )
                                    {
                                        /*
                                         *  The writers are finished and readers can now proceed
                                         */
                                        rwlock->activeReaderCount++;        
                                        rwlock->queuedReaderCount--;
                                        MutexUnlock( rwlock->datalock );
                                        return GUCEF_RWLOCK_OPERATION_SUCCESS;
                                    }
                                    else
                                    {
                                        MutexUnlock( rwlock->datalock );
                                        done = 1;
                                    }
                                    break;
                                }
                                default:
                                case GUCEF_MUTEX_WAIT_TIMEOUT:
                                case GUCEF_MUTEX_OPERATION_FAILED:
                                {
                                    break;
                                }
                            }
                        }
                        while ( 0 == rwlock->delflag && 0 == done );
                        if ( 0 != rwlock->delflag )
                        {
                            UInt8 done = 0;
                            do
                            {
                                /*
                                 *  Lock destruction started while we were waiting to become an active reader
                                 *  Unregister as queued and give up
                                 */
                                lockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs );
                                switch ( lockResult )
                                {
	                                case GUCEF_MUTEX_OPERATION_SUCCESS :
                                    case GUCEF_MUTEX_ABANDONED :
                                    {                                                       
                                        --rwlock->queuedReaderCount;
                                        MutexUnlock( rwlock->datalock );
                                        done = 1;
                                        break;
                                    }
                                    default:
                                    case GUCEF_MUTEX_WAIT_TIMEOUT:
                                    case GUCEF_MUTEX_OPERATION_FAILED:
                                    {
                                        break;
                                    }
                                }
                            }
                            while ( 0 == done );
                        }
                        return GUCEF_RWLOCK_OPERATION_FAILED;
                    }
                    else
                    {
                        rwlock->queuedReaderCount++;
                        MutexUnlock( rwlock->datalock );

                        do
                        {
                            /*
                             *	If readers have priority we will wait for all active writers
                             *	to finish but not queued writers
                             */
                            while ( ( rwlock->activeWriterCount > 0 || 0 != rwlock->writeLockAquisitionInProgress ) && 0 == rwlock->delflag )
                            {
                                PrecisionDelay( 25 );
                            }

                            /*
                             *	Re-verify with the data lock that all active writers are finished
                             *  If this comes to a different conclusion just go for another round
                             */
                            lockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs );
                            switch ( lockResult )
                            {
	                            case GUCEF_MUTEX_OPERATION_SUCCESS :
                                case GUCEF_MUTEX_ABANDONED :
                                {
                                    if ( 0 == rwlock->activeWriterCount && 0 == rwlock->writeLockAquisitionInProgress )
                                    {
                                        /*
                                         *  The active writers are finished and readers can now proceed
                                         */
                                        rwlock->activeReaderCount++;        
                                        rwlock->queuedReaderCount--;
                                        MutexUnlock( rwlock->datalock );
                                        return GUCEF_RWLOCK_OPERATION_SUCCESS;
                                    }
                                    else
                                    {
                                        MutexUnlock( rwlock->datalock );
                                    }
                                    break;
                                }
                                default:
                                case GUCEF_MUTEX_WAIT_TIMEOUT:
                                case GUCEF_MUTEX_OPERATION_FAILED:
                                {
                                    break;
                                }
                            }
                        }
                        while ( 0 == rwlock->delflag );
                        if ( 0 != rwlock->delflag )
                        {
                            do
                            {
                                /*
                                 *  Lock destruction started while we were waiting to become an active reader
                                 *  Unregister as queued and give up
                                 */
                                lockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs );
                                switch ( lockResult )
                                {
	                                case GUCEF_MUTEX_OPERATION_SUCCESS :
                                    case GUCEF_MUTEX_ABANDONED :
                                    {                                                                                  
                                        --rwlock->queuedReaderCount;
                                        MutexUnlock( rwlock->datalock );
                                        break;
                                    }
                                    default:
                                    case GUCEF_MUTEX_WAIT_TIMEOUT:
                                    case GUCEF_MUTEX_OPERATION_FAILED:
                                    {
                                        break;
                                    }
                                }
                            }
                            while ( 1 > 0 );
                        }
                        return GUCEF_RWLOCK_OPERATION_FAILED;
                    }
                    break;
                }
                case GUCEF_MUTEX_WAIT_TIMEOUT :
                {
                    return GUCEF_RWLOCK_WAIT_TIMEOUT;    
                }
                default:
                case GUCEF_MUTEX_OPERATION_FAILED:
                {
                    break;
                }
            }
        }
        while ( 0 == rwlock->delflag );
    }
    return GUCEF_RWLOCK_OPERATION_FAILED;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_reader_stop( TRWLock *rwlock )
{
	GUCEF_BEGIN;
    if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                if ( rwlock->activeWriterCount > 0 )
                {
                    if ( rwlock->lastWriterThreadId == GetCurrentTaskID() )
                    {
                        /*
                         *  The caller is the currently active writer thread who also grabbed a read lock
                         *  this is considered a no-op since the write lock is read-write access and already covered the read access
                         */
                        --rwlock->activeWriterReentrancyCount;
                        MutexUnlock( rwlock->datalock );
                        GUCEF_END;
                        return GUCEF_RWLOCK_OPERATION_SUCCESS;
                    }
                    else
                    {
                        /*
                         *  Something is wrong here.
                         *  How could you have gotten a read lock in the first place if there is an active writer?
                         *  Placing a call to rwl_reader_stop() without a successfull call to rwl_reader_start() ?
                         */
                        MutexUnlock( rwlock->datalock );
                        assert( 0 );
                        GUCEF_END;
                        return GUCEF_RWLOCK_OPERATION_FAILED;
                    }
                }               

                if ( rwlock->activeReaderCount > 0 )
                {
                    rwlock->activeReaderCount--;
                    MutexUnlock( rwlock->datalock );
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_SUCCESS;
                }
                else
                {
                    /*
                     *  Something is wrong here.
                     *  Most likely you have a mismatched calling pattern to lock vs unlock
                     *  It is required to call unlock exactly as many times as you call lock
                     */   
                    GUCEF_ASSERT_ALWAYS;
                    MutexUnlock( rwlock->datalock );
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_FAILED;                     
                }
            }
        }
        while ( 0 == rwlock->delflag );
    }    
    GUCEF_END;
    return GUCEF_RWLOCK_OPERATION_FAILED;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_reader_transition_to_writer( TRWLock* rwlock, UInt32 lockWaitTimeoutInMs )
{
	GUCEF_BEGIN;
    if ( GUCEF_NULL != rwlock )
    {
        UInt32 callerThreadId = GetCurrentTaskID();
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
	            /*
                 *	First we check if destroy has not been called.
                 */
	            if ( 0 != rwlock->delflag )
                {                
                    /*
                     *  Refusing request because we have to prep for deletion
                     */
                    MutexUnlock( rwlock->datalock );
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_FAILED;
                }

                if ( rwlock->activeWriterCount > 0               &&
                     rwlock->activeWriterReentrancyCount > 0     &&
                     rwlock->lastWriterThreadId == callerThreadId )
                {
                    /*
                     *  We are actually in the scenario of a writer which 
                     *  called code that took a reader lock as a reentrancy no-op
                     *  Said reader doesnt itself know this and thinks it has a proper read lock
                     *  when now asking to be turned into a write lock its actually just moving said 
                     *  write reentrancy to the write lock 
                     */
                    MutexUnlock( rwlock->datalock ); 
                    GUCEF_END;
                    return GUCEF_MUTEX_OPERATION_SUCCESS;
                }

	            /*
                 *	Sanity check
                 */
                if ( rwlock->activeWriterCount > 0            ||
                     rwlock->activeWriterReentrancyCount > 0  ||
                     rwlock->activeReaderCount <= 0           )
                {
                    /*
                     *  You should not get here
                     *  We have not met the prerequisite conditions to transition a reader
                     *      - There must be no active writer because presumably you already have an active reader
                     *      - There must be an active reader because how else can you transition it
                     */
                    GUCEF_ASSERT_ALWAYS;
                    MutexUnlock( rwlock->datalock );
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_FAILED;
                }

                rwlock->queuedWriterCount++;
                rwlock->activeReaderCount--;
                
                if ( 0 == rwlock->activeWriterCount && 0 == rwlock->activeReaderCount )
                {
                    /*
                     *  There are no active writers or readers so we can make this quick
                     */
                    while ( GUCEF_MUTEX_OPERATION_SUCCESS != MutexLock( rwlock->writerlock, GUCEF_MUTEX_INFINITE_TIMEOUT ) ) {};
                    rwlock->activeWriterCount++;        
                    rwlock->queuedWriterCount--;
                    rwlock->lastWriterThreadId = GetCurrentTaskID(); 
                    MutexUnlock( rwlock->datalock );
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_SUCCESS;                    
                }

                if ( rwlock->wpriority > 0 )
                {
                    MutexUnlock( rwlock->datalock );
                    
                    do
                    {
                        if ( 0 == rwlock->activeReaderCount             &&
                             0 == rwlock->writeLockAquisitionInProgress )
                        {
                            /*
                             *  Reverify with a data lock
                             *  We have to hurry and a new reader may slip in
                             */
                            UInt8 reverifyFailed = 0;
                            do
                            {
                                if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                {
                                    if ( 0 == rwlock->activeReaderCount             &&
                                         0 == rwlock->writeLockAquisitionInProgress )
                                    {
                                        /*
                                         *  We found a moment without any active readers at all
                                         *  Now we just have to wait for other writers if any
                                         *
                                         *  The 'queuedWriterCount' already blocks new active readers but for consistency we also
                                         *  set the writeLockAquisitionInProgress flag 
                                         */
                                        rwlock->writeLockAquisitionInProgress = 1;
                                        rwlock->writeLockAquisitionThreadId = GetCurrentTaskID();
                                        MutexUnlock( rwlock->datalock );
                    
                                        do
                                        {
                                            if ( MutexLock( rwlock->writerlock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                            {
                                                /*
                                                 *  Now that we have the write lock we need to still go back and update the administration
                                                 *  our queued writer is now an active writer
                                                 */
                                                UInt32 dataLockResult = 0;
                                                do
                                                {
                                                    dataLockResult = MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT );
                                                    if ( GUCEF_MUTEX_OPERATION_SUCCESS == dataLockResult )
                                                    {                            
                                                        /*
                                                         *  We have successfully turned our active reader into a queued writer and 
                                                         *  after that turned the queued writer into the active writer
                                                         */
                                                        rwlock->activeWriterCount++;
                                                        rwlock->lastWriterThreadId = GetCurrentTaskID();
                                                        rwlock->writeLockAquisitionInProgress = 0;
                                                        rwlock->queuedWriterCount--;
                                                        MutexUnlock( rwlock->datalock );
                                                        GUCEF_END;
                                                        return GUCEF_RWLOCK_OPERATION_SUCCESS;
                                                    }
                                                }
                                                while ( GUCEF_MUTEX_OPERATION_SUCCESS != dataLockResult );

                                                /*
                                                 *  Logically we should never get here
                                                 */
                                                GUCEF_ASSERT_ALWAYS;
                                                rwlock->writeLockAquisitionInProgress = 0;
                                                rwlock->queuedWriterCount--;
                                                MutexUnlock( rwlock->writerlock );
                                                GUCEF_END;
                                                return GUCEF_RWLOCK_OPERATION_FAILED;
                                            }
                                        }
                                        while ( 0 == rwlock->delflag );
                                        if ( 0 != rwlock->delflag )
                                        {
                                            do
                                            {
                                                /*
                                                 *  Lock destruction started while we were waiting to grab the lock
                                                 *  Unregister as queued and give up
                                                 */
                                                if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                {                                                        
                                                    --rwlock->queuedWriterCount;
                                                    rwlock->writeLockAquisitionInProgress = 0;
                                                    MutexUnlock( rwlock->datalock );
                                                    break;
                                                }
                                            }
                                            while ( 1 > 0 );
                                        }
                                        GUCEF_END;
                                        return GUCEF_RWLOCK_OPERATION_FAILED;
                                    }
                                    else
                                    {
                                        reverifyFailed = 1;
                                        MutexUnlock( rwlock->datalock );
                                    }
                                }
                            }
                            while ( 0 == rwlock->delflag && 0 == reverifyFailed );
                        }
                        else
                        {
                            PrecisionDelay( 25 );
                        }
                    }
                    while ( 0 == rwlock->delflag );
                    if ( 0 != rwlock->delflag )
                    {
                        do
                        {
                            /*
                             *  Lock destruction started while we were waiting to grab the lock
                             *  Unregister as queued and give up
                             */
                            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                            {                                                        
                                --rwlock->queuedWriterCount;
                                MutexUnlock( rwlock->datalock );
                                break;
                            }
                        }
                        while ( 1 > 0 );
                    }
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_FAILED;
                }
                else
                {
                    /*
                     *  First wait for all the active and queued readers to finish
                     *  We need to grab the moment that happens via constant monitoring
                     */
                    MutexUnlock( rwlock->datalock );
                    
                    do
                    {
                        if ( 0 == rwlock->activeReaderCount             && 
                             0 == rwlock->queuedReaderCount             &&
                             0 == rwlock->writeLockAquisitionInProgress )
                        {
                            /*
                             *  Reverify with a data lock
                             *  We have to hurry and a new reader may slip in
                             */
                            UInt8 reverifyFailed = 0;
                            do
                            {
                                if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                {
                                    if ( 0 == rwlock->activeReaderCount            && 
                                         0 == rwlock->queuedReaderCount            &&
                                         0 == rwlock->writeLockAquisitionInProgress )
                                    {
                                        /*
                                         *  We found a moment without any readers at all
                                         *  Now we just have to wait for other writers if any
                                         *
                                         *  Here we cheat a little to have a chance:
                                         *  The writeLockAquisitionInProgress flag blocks new active readers
                                         *  It temporarly overrules the reader priority while we grab the other lock
                                         */
                                        rwlock->writeLockAquisitionInProgress = 1;
                                        rwlock->writeLockAquisitionThreadId = GetCurrentTaskID();
                                        MutexUnlock( rwlock->datalock );
                    
                                        do
                                        {
                                            if ( MutexLock( rwlock->writerlock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                            {
                                                /*
                                                 *  Now that we have the write lock we need to still go back and update the administration
                                                 *  our queued writer is now an active writer
                                                 */
                                                UInt32 dataLockResult = 0;
                                                do
                                                {
                                                    dataLockResult = MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT );
                                                    if ( GUCEF_MUTEX_OPERATION_SUCCESS == dataLockResult )
                                                    {                            
                                                        /*
                                                         *  We have successfully turned our active reader into a queued writer and 
                                                         *  after that turned the queued writer into the active writer
                                                         */
                                                        rwlock->activeWriterCount++;
                                                        rwlock->lastWriterThreadId = GetCurrentTaskID();
                                                        rwlock->writeLockAquisitionInProgress = 0;
                                                        rwlock->queuedWriterCount--;
                                                        MutexUnlock( rwlock->datalock );
                                                        GUCEF_END;
                                                        return GUCEF_RWLOCK_OPERATION_SUCCESS;                                    
                                                    }
                                                }
                                                while ( GUCEF_MUTEX_OPERATION_SUCCESS != dataLockResult );
                                                
                                                /*
                                                 *  Logically we should never get here
                                                 */
                                                GUCEF_ASSERT_ALWAYS;
                                                rwlock->writeLockAquisitionInProgress = 0;
                                                rwlock->queuedWriterCount--;
                                                MutexUnlock( rwlock->writerlock );
                                                GUCEF_END;
                                                return GUCEF_RWLOCK_OPERATION_FAILED;
                                            }
                                        }
                                        while ( 0 == rwlock->delflag );
                                        if ( 0 != rwlock->delflag )
                                        {
                                            do
                                            {
                                                /*
                                                 *  Lock destruction started while we were waiting to grab the lock
                                                 *  Unregister as queued and give up
                                                 */
                                                if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                {                                                        
                                                    --rwlock->queuedWriterCount;
                                                    rwlock->writeLockAquisitionInProgress = 0;
                                                    MutexUnlock( rwlock->datalock );
                                                    break;
                                                }
                                            }
                                            while ( 1 > 0 );
                                        }
                                        GUCEF_END;
                                        return GUCEF_RWLOCK_OPERATION_FAILED;
                                    }
                                    else
                                    {
                                        reverifyFailed = 1;
                                        MutexUnlock( rwlock->datalock );
                                    }
                                }
                            }
                            while ( 0 == rwlock->delflag && 0 == reverifyFailed );
                        }
                        else
                        {
                            PrecisionDelay( 25 );
                        }
                    }
                    while ( 0 == rwlock->delflag );
                    if ( 0 != rwlock->delflag )
                    {
                        do
                        {
                            /*
                             *  Lock destruction started while we were waiting to grab the lock
                             *  Unregister as queued and give up
                             */
                            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                            {                                                        
                                --rwlock->queuedWriterCount;
                                MutexUnlock( rwlock->datalock );
                                break;
                            }
                        }
                        while ( 1 > 0 );
                    }
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_FAILED;
                }
            }
        }
        while ( 0 == rwlock->delflag );
    }    
    GUCEF_END;
    return GUCEF_RWLOCK_OPERATION_FAILED;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_writer_start( TRWLock* rwlock, UInt32 lockWaitTimeoutInMs )
{
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            UInt8 done = 0;
            UInt32 lockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs ); 
            switch ( lockResult )
            {
	            case GUCEF_MUTEX_OPERATION_SUCCESS :
                case GUCEF_MUTEX_ABANDONED :
                {
	                /*
                     *	First we check if destroy has not been called.
                     */
	                if ( 0 != rwlock->delflag )
                    {                
                        MutexUnlock( rwlock->datalock );
                        return GUCEF_RWLOCK_OPERATION_FAILED;
                    }

                    ++rwlock->queuedWriterCount;
                    if ( 0 < rwlock->activeWriterCount )
                    {
                        /*
                         *  The currently active writer could actually be the same thread
                         *  trying to obtain another write lock in another scope
                         *  We have to check for this or the thread can self-deadlock
                         */
                        if ( rwlock->lastWriterThreadId == GetCurrentTaskID() )
                        {
                            /*
                             *  The caller is already the currently active writer thread
                             *  allow it to proceed since we already have write access anyway
                             */
                            ++rwlock->activeWriterReentrancyCount;
                            --rwlock->queuedWriterCount;
                            do
                            {
                                lockResult = MutexLock( rwlock->writerlock, GUCEF_MT_SHORT_LOCK_TIMEOUT );
                                switch ( lockResult )
                                {
	                                case GUCEF_MUTEX_OPERATION_SUCCESS :
                                    case GUCEF_MUTEX_ABANDONED :
                                    {
                                        MutexUnlock( rwlock->datalock );
                                        return GUCEF_RWLOCK_OPERATION_SUCCESS;
                                    }
                                    case GUCEF_MUTEX_WAIT_TIMEOUT :
                                    {
                                        /*
                                         *  If we are correct about reentrancy there should not have been any lock contention
                                         *  as such logically we should never get here barring some bug/issue since we already have the lock supposedly
                                         */ 
                                        GUCEF_ASSERT_ALWAYS;
                                        --rwlock->activeWriterReentrancyCount;
                                        MutexUnlock( rwlock->datalock );
                                        return GUCEF_RWLOCK_WAIT_TIMEOUT;    
                                    }
                                    default:
                                    case GUCEF_MUTEX_OPERATION_FAILED:
                                    {
                                        break;
                                    }
                                }
                            }
                            while ( 0 == rwlock->delflag );

                            /*
                             *  We should not get here if you cleaned up your threads before cleaning up the read write lock itself
                             */
                            MutexUnlock( rwlock->datalock );
                            return GUCEF_RWLOCK_OPERATION_FAILED;
                        }                    
                    }
                    else
                    {
                        /*
                         *  Check for a cheaper early out 
                         */
                        if ( 0 == rwlock->activeReaderCount            && 
                             0 == rwlock->queuedReaderCount            && 
                             0 == rwlock->activeWriterCount            && 
                             0 == rwlock->writeLockAquisitionInProgress )
                        {
                            /*
                             *  There are no active writers or readers so we can make this quick
                             */
                            do
                            {
                                lockResult = MutexLock( rwlock->writerlock, lockWaitTimeoutInMs );
                                switch ( lockResult )
                                {
	                                case GUCEF_MUTEX_OPERATION_SUCCESS :
                                    case GUCEF_MUTEX_ABANDONED :
                                    {
                                        rwlock->activeWriterCount++;        
                                        rwlock->queuedWriterCount--;
                                        rwlock->lastWriterThreadId = GetCurrentTaskID(); 
                                        MutexUnlock( rwlock->datalock );
                                        return GUCEF_RWLOCK_OPERATION_SUCCESS; 
                                    }
                                    case GUCEF_MUTEX_WAIT_TIMEOUT :
                                    {
                                        /*
                                         *  If we are correct about reentrancy there should not have been any lock contention
                                         *  as such logically we should never get here barring some bug/issue since we already have the lock supposedly
                                         */ 
                                        GUCEF_ASSERT_ALWAYS;
                                        --rwlock->activeWriterReentrancyCount;
                                        MutexUnlock( rwlock->datalock );
                                        return GUCEF_RWLOCK_WAIT_TIMEOUT;    
                                    }
                                    default:
                                    case GUCEF_MUTEX_OPERATION_FAILED:
                                    {
                                        break;
                                    }
                                }
                            }
                            while ( 0 == rwlock->delflag );

                            /*
                             *  We should not get here if you cleaned up your threads before cleaning up the read write lock itself
                             */
                            MutexUnlock( rwlock->datalock );
                            return GUCEF_RWLOCK_OPERATION_FAILED;
                        }
                    }

                    if ( 0 != rwlock->wpriority )
                    {
                        MutexUnlock( rwlock->datalock );
                    
                        /*
                         *  Wait for a moment with no active readers
                         *  We dont care about queued readers since writers take priority
                         */
                        do
                        {
                            if ( 0 == rwlock->activeReaderCount             &&
                                 0 == rwlock->writeLockAquisitionInProgress )
                            {
                                /*
                                 *  Reverify with a data lock
                                 *  We have to hurry and a new reader may slip in
                                 */
                                UInt8 reverifyFailed = 0;
                                do
                                {
                                    lockResult = MutexLock( rwlock->datalock, GUCEF_MT_SHORT_LOCK_TIMEOUT );
                                    switch ( lockResult )
                                    {
	                                    case GUCEF_MUTEX_OPERATION_SUCCESS :
                                        case GUCEF_MUTEX_ABANDONED :
                                        {
                                            if ( 0 == rwlock->activeReaderCount             &&
                                                 0 == rwlock->writeLockAquisitionInProgress )
                                            {
                                                /*
                                                 *  We found a moment without any active readers at all
                                                 *  Now we just have to wait for other writers if any
                                                 *
                                                 *  The 'queuedWriterCount' already blocks new active readers but for consistency we also
                                                 *  set the writeLockAquisitionInProgress flag 
                                                 */
                                                rwlock->writeLockAquisitionInProgress = 1;
                                                rwlock->writeLockAquisitionThreadId = GetCurrentTaskID(); 
                                                MutexUnlock( rwlock->datalock );
                    
                                                do
                                                {
                                                    /*
                                                     *  This is the main call in this function, what its all about
                                                     *  Obtaining the write lock
                                                     *
                                                     */
                                                    lockResult = MutexLock( rwlock->writerlock, lockWaitTimeoutInMs );
                                                    switch ( lockResult )
                                                    {
	                                                    case GUCEF_MUTEX_OPERATION_SUCCESS :
                                                        case GUCEF_MUTEX_ABANDONED :
                                                        {
                                                            /*
                                                             *  Now that we have the write lock we need to still go back and update the administration
                                                             *  our queued writer is now an active writer
                                                             */
                                                            UInt32 dataLockResult = 0;
                                                            do
                                                            {
                                                                dataLockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs );
                                                                if ( dataLockResult == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                                {                            
                                                                    /*
                                                                     *  Perform one last check for the delete flag
                                                                     *  We dont want to wait for all the queued writers to slowly process which could be a lot of them
                                                                     */
                                                                    if ( 0 != rwlock->delflag )
                                                                    {
                                                                        /*
                                                                         *  Destruction started while we were waiting to get the lock
                                                                         *  Undo grabbing the writer lock
                                                                         */
                                                                        MutexUnlock( rwlock->writerlock );
                                                                        rwlock->writeLockAquisitionInProgress = 0;
                                                                        rwlock->queuedWriterCount--;
                                                                        MutexUnlock( rwlock->datalock );
                                                                        return GUCEF_RWLOCK_OPERATION_FAILED;                                                                                                
                                                                    }
                                                                    else
                                                                    {
                                                                        /*
                                                                         *  We have successfully turned our queued writer into the active writer
                                                                         */
                                                                        rwlock->activeWriterCount++;
                                                                        rwlock->lastWriterThreadId = GetCurrentTaskID();
                                                                        rwlock->writeLockAquisitionInProgress = 0;
                                                                        rwlock->queuedWriterCount--;
                                                                        MutexUnlock( rwlock->datalock );
                                                                        return GUCEF_RWLOCK_OPERATION_SUCCESS;                                    
                                                                    }
                                                                }
                                                            }
                                                            while ( GUCEF_MUTEX_OPERATION_SUCCESS != dataLockResult );

                                                            /*
                                                             *  We should never get here logically
                                                             */
                                                            GUCEF_ASSERT_ALWAYS;
                                                            rwlock->writeLockAquisitionInProgress = 0;
                                                            MutexUnlock( rwlock->writerlock );
                                                            return GUCEF_RWLOCK_OPERATION_FAILED;
                                                        }
                                                        case GUCEF_MUTEX_WAIT_TIMEOUT:
                                                        {
                                                            UInt32 dataLockResult = 0;
                                                            do
                                                            {
                                                                dataLockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs );
                                                                if ( dataLockResult == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                                {                            
                                                                    /*
                                                                     *  Update the administration to undo our registration as a queued writer 
                                                                     *  before exiting and reporting a wait timeout
                                                                     */
                                                                    rwlock->writeLockAquisitionInProgress = 0;
                                                                    rwlock->queuedWriterCount--;
                                                                    MutexUnlock( rwlock->datalock );
                                                                    return GUCEF_RWLOCK_WAIT_TIMEOUT;                                                                                                
                                                                }
                                                            }
                                                            while ( GUCEF_MUTEX_OPERATION_SUCCESS != dataLockResult );

                                                            /*
                                                             *  We should never get here logically
                                                             */
                                                            GUCEF_ASSERT_ALWAYS;
                                                            rwlock->writeLockAquisitionInProgress = 0;
                                                            rwlock->queuedWriterCount--;
                                                            MutexUnlock( rwlock->writerlock );
                                                            return GUCEF_RWLOCK_OPERATION_FAILED;
                                                        }
                                                        default:
                                                        case GUCEF_MUTEX_OPERATION_FAILED:
                                                        {
                                                            UInt32 dataLockResult = 0;
                                                            do
                                                            {
                                                                dataLockResult = MutexLock( rwlock->datalock, lockWaitTimeoutInMs );
                                                                if ( dataLockResult == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                                {                            
                                                                    /*
                                                                     *  Update the administration to undo our registration as a queued writer 
                                                                     *  before exiting and reporting a wait timeout
                                                                     */
                                                                    rwlock->writeLockAquisitionInProgress = 0;
                                                                    rwlock->queuedWriterCount--;
                                                                    MutexUnlock( rwlock->datalock );
                                                                    return GUCEF_RWLOCK_OPERATION_FAILED;                                                                                                
                                                                }
                                                            }
                                                            while ( GUCEF_MUTEX_OPERATION_SUCCESS != dataLockResult );

                                                            /*
                                                             *  We should never get here logically
                                                             */
                                                            GUCEF_ASSERT_ALWAYS;
                                                            rwlock->writeLockAquisitionInProgress = 0;
                                                            rwlock->queuedWriterCount--;
                                                            return GUCEF_RWLOCK_OPERATION_FAILED;
                                                        }
                                                    }
                                                }
                                                while ( 0 == rwlock->delflag );
                                                if ( 0 != rwlock->delflag )
                                                {
                                                    do
                                                    {
                                                        /*
                                                         *  Lock destruction started while we were waiting to grab the lock
                                                         *  Unregister as queued and give up
                                                         */
                                                        if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                        {                                                        
                                                            --rwlock->queuedWriterCount;
                                                            rwlock->writeLockAquisitionInProgress = 0;
                                                            MutexUnlock( rwlock->datalock );
                                                            break;
                                                        }
                                                    }
                                                    while ( 1 > 0 );
                                                }
                                                return GUCEF_RWLOCK_OPERATION_FAILED;
                                            }
                                            else
                                            {
                                                reverifyFailed = 1;
                                                MutexUnlock( rwlock->datalock );
                                            }
                                            break;
                                        }
                                        default:
                                        case GUCEF_MUTEX_WAIT_TIMEOUT:
                                        case GUCEF_MUTEX_OPERATION_FAILED:
                                        {
                                            /*
                                             *  Could not get data lock to verify, loop again  
                                             */
                                            break;
                                        }
                                    }
                                }
                                while ( 0 == rwlock->delflag && 0 == reverifyFailed );
                            }
                            else
                            {
                                PrecisionDelay( 25 );
                            }
                        }
                        while ( 0 == rwlock->delflag );
                        if ( 0 != rwlock->delflag )
                        {
                            do
                            {
                                /*
                                 *  Lock destruction started while we were waiting to grab the lock
                                 *  Unregister as queued and give up
                                 */
                                if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                {                                                        
                                    --rwlock->queuedWriterCount;
                                    MutexUnlock( rwlock->datalock );
                                    break;
                                }                                                                 
                            }
                            while ( 1 > 0 );
                        }
                        return GUCEF_RWLOCK_OPERATION_FAILED;

                    }
                    else
                    {
                        /*
                         *  First wait for all the active and queued readers to finish
                         *  We need to grab the moment that happens via constant monitoring
                         */
                        MutexUnlock( rwlock->datalock );
                    
                        do
                        {
                            if ( 0 == rwlock->activeReaderCount            && 
                                 0 == rwlock->queuedReaderCount            &&
                                 0 == rwlock->writeLockAquisitionInProgress )
                            {
                                /*
                                 *  Reverify with a data lock
                                 *  We have to hurry and a new reader may slip in
                                 */
                                UInt8 reverifyFailed = 0;
                                do
                                {
                                    if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                    {
                                        if ( 0 == rwlock->activeReaderCount            && 
                                             0 == rwlock->queuedReaderCount            &&
                                             0 == rwlock->writeLockAquisitionInProgress )
                                        {
                                            /*
                                             *  We found a moment without any readers at all
                                             *  Now we just have to wait for other writers if any
                                             *
                                             *  Here we cheat a little to have a chance:
                                             *  The writeLockAquisitionInProgress flag blocks new active readers
                                             *  It temporarly overrules the reader priority while we grab the other lock
                                             */
                                            rwlock->writeLockAquisitionInProgress = 1;
                                            rwlock->writeLockAquisitionThreadId = GetCurrentTaskID();
                                            MutexUnlock( rwlock->datalock );
                    
                                            do
                                            {
                                                if ( MutexLock( rwlock->writerlock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                {
                                                    /*
                                                     *  Now that we have the write lock we need to still go back and update the administration.
                                                     *  Our queued writer is now an active writer
                                                     */
                                                    do
                                                    {
                                                        if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                        {
                                                            /*
                                                             *  Perform one last check for the delete flag
                                                             *  We dont want to wait for all the queued writers to slowly process which could be a lot of them
                                                             */
                                                            if ( 0 != rwlock->delflag )
                                                            {
                                                                /*
                                                                 *  Destruction started while we were waiting to get the writer lock
                                                                 *  Undo grabbing the writer lock
                                                                 */
                                                                MutexUnlock( rwlock->writerlock );
                                                                rwlock->writeLockAquisitionInProgress = 0;
                                                                rwlock->queuedWriterCount--;
                                                                MutexUnlock( rwlock->datalock );
                                                                return GUCEF_RWLOCK_OPERATION_FAILED;                                                                                                
                                                            }
                                                            else
                                                            {
                                                                /*
                                                                 *  We have successfully turned our queued writer into the active writer
                                                                 */
                                                                rwlock->activeWriterCount++;
                                                                rwlock->lastWriterThreadId = GetCurrentTaskID();
                                                                rwlock->writeLockAquisitionInProgress = 0;
                                                                rwlock->queuedWriterCount--;
                                                                MutexUnlock( rwlock->datalock );
                                                                return GUCEF_RWLOCK_OPERATION_SUCCESS;                                    
                                                            }
                                                        }
                                                    }
                                                    while ( 0 == rwlock->delflag );

                                                    /*
                                                     *  We should never get here logically
                                                     */
                                                    GUCEF_ASSERT_ALWAYS;
                                                    rwlock->writeLockAquisitionInProgress = 0;
                                                    MutexUnlock( rwlock->writerlock );
                                                    return GUCEF_RWLOCK_OPERATION_FAILED;
                                                }
                                            }
                                            while ( 0 == rwlock->delflag );
                                            if ( 0 != rwlock->delflag )
                                            {
                                                do
                                                {
                                                    /*
                                                     *  Lock destruction started while we were waiting to grab the lock
                                                     *  Unregister as queued and give up
                                                     */
                                                    if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                                    {                                                        
                                                        --rwlock->queuedWriterCount;
                                                        rwlock->writeLockAquisitionInProgress = 0;
                                                        MutexUnlock( rwlock->datalock );
                                                        break;
                                                    }
                                                }
                                                while ( 1 > 0 );
                                                return GUCEF_RWLOCK_OPERATION_FAILED;
                                            }
                                        }
                                        else
                                        {
                                            reverifyFailed = 1;
                                            MutexUnlock( rwlock->datalock );
                                        }
                                    }
                                }
                                while ( 0 == rwlock->delflag && 0 == reverifyFailed );
                            }
                            else
                            {
                                PrecisionDelay( 25 );
                            }
                        }
                        while ( 0 == rwlock->delflag );
                        if ( 0 != rwlock->delflag )
                        {
                            do
                            {
                                /*
                                 *  Lock destruction started while we were waiting to grab the lock
                                 *  Unregister as queued and give up
                                 */
                                if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
                                {                                                        
                                    --rwlock->queuedWriterCount;
                                    MutexUnlock( rwlock->datalock );
                                    break;
                                }
                            }
                            while ( 1 > 0 );
                        }
                        return GUCEF_RWLOCK_OPERATION_FAILED;
                    }
                    break;
                }
                case GUCEF_MUTEX_WAIT_TIMEOUT :
                {
                    return GUCEF_RWLOCK_WAIT_TIMEOUT;    
                }
                default:
                case GUCEF_MUTEX_OPERATION_FAILED:
                {
                    break;
                }
            }
        }
        while ( 0 == rwlock->delflag );
    }
    return GUCEF_RWLOCK_OPERATION_FAILED;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_writer_stop( TRWLock *rwlock )
{
    GUCEF_BEGIN;
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
                if ( rwlock->lastWriterThreadId == GetCurrentTaskID() )
                {
                    /*
                     *  The caller is the currently active writer thread
                     *  This should always be the case
                     *  allow it to proceed since we already have write access anyway
                     */
                    assert( rwlock->activeWriterCount > 0 );
                    
                    if ( rwlock->activeWriterReentrancyCount > 0 )
                        --rwlock->activeWriterReentrancyCount;
                    else
                        --rwlock->activeWriterCount;

                    MutexUnlock( rwlock->writerlock );
                }               
                else
                {
                    GUCEF_UNREACHABLE;
                }
                                
                MutexUnlock( rwlock->datalock );
                GUCEF_END;
                return GUCEF_RWLOCK_OPERATION_SUCCESS;
            }
        }
        while ( 0 == rwlock->delflag );
    }    
    GUCEF_END;
    return GUCEF_RWLOCK_OPERATION_FAILED;
}

/*--------------------------------------------------------------------------*/

UInt32
rwl_writer_transition_to_reader( TRWLock* rwlock, UInt32 lockWaitTimeoutInMs )
{
	GUCEF_BEGIN;
	if ( GUCEF_NULL != rwlock )
    {
        do
        {
            if ( MutexLock( rwlock->datalock, GUCEF_MUTEX_INFINITE_TIMEOUT ) == GUCEF_MUTEX_OPERATION_SUCCESS )
            {
	            UInt32 callerThreadId = GetCurrentTaskID();
                
                /*
                 *	Sanity check
                 */
                if ( rwlock->activeWriterCount != 1               ||
                     rwlock->activeReaderCount > 0                ||
                     rwlock->lastWriterThreadId != callerThreadId  )
                {
                    /*
                     *  You should not get here
                     *  We have not met the prerequisite conditions to transition a writer
                     *      - There must be no active reader because presumably you already have an active writer
                     *      - There must be an active writer because how else can you transition it
                     */
                    GUCEF_ASSERT_ALWAYS;
                    MutexUnlock( rwlock->datalock );
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_FAILED;
                }                
                
                /*
                 *  The caller is the currently active writer thread
                 *  This should always be the case
                 *  allow it to proceed since we already have write access anyway
                 */
                assert( rwlock->activeWriterCount == 1 );                    
                if ( rwlock->activeWriterReentrancyCount > 0 )
                {
                    /*
                     *  You cannot transition a write lock that still had outstanding reentrant write levels
                     *  Check your code flow
                     */
                    GUCEF_UNREACHABLE;
                    MutexUnlock( rwlock->datalock );
                    GUCEF_END;
                    return GUCEF_RWLOCK_OPERATION_FAILED;                        
                }

                ++rwlock->activeReaderCount;
                --rwlock->activeWriterCount;
                MutexUnlock( rwlock->writerlock );
                MutexUnlock( rwlock->datalock );
                GUCEF_END;
                return GUCEF_RWLOCK_OPERATION_SUCCESS;
            }
        }
        while ( 0 == rwlock->delflag );
    }
    GUCEF_END;
    return GUCEF_RWLOCK_OPERATION_FAILED;
}

/*--------------------------------------------------------------------------*/


