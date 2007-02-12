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
