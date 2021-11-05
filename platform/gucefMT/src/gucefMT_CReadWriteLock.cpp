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
#include "gucefMT_DVRWLOCK.h"           /* my C implementation of a RW lock */
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

CReadWriteLock::CReadWriteLock( bool writers_overrule )
    : _rwlock( GUCEF_NULL )
{GUCEF_TRACE;

    _rwlock = rwl_create( writers_overrule ? 1 : 0 );
}

/*--------------------------------------------------------------------------*/

CReadWriteLock::CReadWriteLock( const CReadWriteLock& src )
{GUCEF_TRACE;

    _rwlock = rwl_create( rwl_writer_overrules( src._rwlock ) );
}

/*--------------------------------------------------------------------------*/

CReadWriteLock::~CReadWriteLock()
{GUCEF_TRACE;

    rwl_destroy( _rwlock );
    _rwlock = GUCEF_NULL;
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::WriterStart( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return rwl_writer_start( _rwlock ) != 0;
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::WriterStop( void ) const
{GUCEF_TRACE;

    rwl_writer_stop( _rwlock );
    return true;
}

/*--------------------------------------------------------------------------*/

UInt32
CReadWriteLock::WriterCount( void ) const
{GUCEF_TRACE;

    return rwl_writers( _rwlock );
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::ReaderStart( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return rwl_reader_start( _rwlock ) != 0;
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::ReaderStop( void ) const
{GUCEF_TRACE;

    rwl_reader_stop( _rwlock );
    return true;
}

/*--------------------------------------------------------------------------*/

UInt32
CReadWriteLock::ReaderCount( void ) const
{GUCEF_TRACE;

    return rwl_readers( _rwlock );
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::DoWritersOverrule( void ) const
{GUCEF_TRACE;

    return rwl_writer_overrules( _rwlock ) != 0;
}

/*--------------------------------------------------------------------------*/

const CILockable* 
CReadWriteLock::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*--------------------------------------------------------------------------*/

bool 
CReadWriteLock::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return WriterStart();
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::Unlock( void ) const
{GUCEF_TRACE;

    return WriterStop();
}

/*--------------------------------------------------------------------------*/

bool 
CReadWriteLock::ReadOnlyLock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return ReaderStart();
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::ReadOnlyUnlock( void ) const
{GUCEF_TRACE;

    return ReaderStop();
}

/*------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
