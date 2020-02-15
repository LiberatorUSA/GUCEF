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

#ifndef GUCEF_MT_DVRWLOCK_H
#include "gucefMT_DVRWLOCK.h"           /* my C implementation of an RW lock */
#define GUCEF_MT_DVRWLOCK_H
#endif /* GUCEF_MT_DVRWLOCK_H ? */

#include "gucefMT_CReadWriteLock.h"     /* definition of the class implemented here */

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

/**
 *      Creates the readers/writers lock with the given priority setting
 *      @param writers_overrule Wheter writers or readers have priority.
 */
CReadWriteLock::CReadWriteLock( bool writers_overrule )
{
        _rwlock = rwl_create( writers_overrule );
}

/*--------------------------------------------------------------------------*/

/**
 *      Cleans up the lock.
 *      The destructor will block untill all readers and writers are
 *      finished !!!.
 */
CReadWriteLock::~CReadWriteLock()
{
        rwl_destroy( _rwlock );
}

/*--------------------------------------------------------------------------*/

/**
 *      Use when a writer task needs access to the data protected by
 *      the given lock. Blocks untill access is granted.
 */
bool
CReadWriteLock::WriterStart( void )
{
        return rwl_writer_start( _rwlock ) != 0;
}

/*--------------------------------------------------------------------------*/

/**
 *      Use when a writer task finished using data/code that is
 *      protected by this lock.
 */
void
CReadWriteLock::WriterStop( void )
{
        rwl_writer_stop( _rwlock );
}

/*--------------------------------------------------------------------------*/

/**
 *      Returns the current number of writers. Note that this function
 *      is meant for output to humans. Race conditions ect. make it
 *      useless for any control code.
 */
UInt32
CReadWriteLock::WriterCount( void )
{
        return rwl_writers( _rwlock );
}

/*--------------------------------------------------------------------------*/

/**
 *      Use when a reader task needs access to the data protected by
 *      the given lock. Blocks untill access is granted (if a writer is
 *      currently busy).
 */
bool
CReadWriteLock::ReaderStart( void )
{
        return rwl_reader_start( _rwlock ) != 0;
}

/*--------------------------------------------------------------------------*/

/**
 *      Use when a reader task finished using data/code that is
 *      protected by this lock.
 */
void
CReadWriteLock::ReaderStop( void )
{
        rwl_reader_stop( _rwlock );
}

/*--------------------------------------------------------------------------*/

/**
 *      Returns the current number of readers. Note that this function
 *      is meant for output to humans. Race conditions ect. make it
 *      useless for any control code.
 */
UInt32
CReadWriteLock::ReaderCount( void )
{
        return rwl_readers( _rwlock );
}

/*--------------------------------------------------------------------------*/

/**
 *      Returns wheter readers or writers have prioritity.
 *      True in case the writers have priority and false if the readers
 *      have priority.
 */
bool
CReadWriteLock::DoWritersOverrule( void )
{
        return rwl_writer_overrules( _rwlock ) != 0;
}

/*------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
