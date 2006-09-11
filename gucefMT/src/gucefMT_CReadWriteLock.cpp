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

#ifndef GUCEF_MT_DVRWLOCK_H
#include "gucefMT_dvrwlock.h"           /* my C implementation of an RW lock */
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
