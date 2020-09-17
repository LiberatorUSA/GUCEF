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
{
    _rwlock = rwl_create( writers_overrule ? 1 : 0 );
}

/*--------------------------------------------------------------------------*/

CReadWriteLock::CReadWriteLock( const CReadWriteLock& src )
{
    _rwlock = rwl_create( rwl_writer_overrules( src._rwlock ) );
}

/*--------------------------------------------------------------------------*/

CReadWriteLock::~CReadWriteLock()
{
    rwl_destroy( _rwlock );
    _rwlock = GUCEF_NULL;
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::WriterStart( void ) const
{
    return rwl_writer_start( _rwlock ) != 0;
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::WriterStop( void ) const
{
    rwl_writer_stop( _rwlock );
    return true;
}

/*--------------------------------------------------------------------------*/

UInt32
CReadWriteLock::WriterCount( void ) const
{
    return rwl_writers( _rwlock );
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::ReaderStart( void ) const
{
    return rwl_reader_start( _rwlock ) != 0;
}

/*--------------------------------------------------------------------------*/

void
CReadWriteLock::ReaderStop( void ) const
{
    rwl_reader_stop( _rwlock );
}

/*--------------------------------------------------------------------------*/

UInt32
CReadWriteLock::ReaderCount( void ) const
{
    return rwl_readers( _rwlock );
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::DoWritersOverrule( void ) const
{
    return rwl_writer_overrules( _rwlock ) != 0;
}

/*--------------------------------------------------------------------------*/

const CILockable* 
CReadWriteLock::AsLockable( void ) const
{
    return this;
}

/*--------------------------------------------------------------------------*/

bool 
CReadWriteLock::Lock( void ) const
{
    return WriterStart();
}

/*--------------------------------------------------------------------------*/

bool
CReadWriteLock::Unlock( void ) const
{
    return WriterStop();
}

/*------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
