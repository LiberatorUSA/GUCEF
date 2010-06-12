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

/*
 *      These are some functions which represent my implementation of
 *      the reader/writers problem and subsequently it's solution.
 *      Reader tasks can work parallel while writer tasks will be given
 *      exclusive access. This can allow some applications to work much faster
 *      then with a simple mutex lock implementation since readers can operate
 *      paralell.
 *
 *      You might wonder when something is a reader and when is it a writer task
 *      Well,.. if you use a thread that uses data but does not change the data
 *      at any time then you have a reader task. If the data gets changed at any
 *      time by the task then you have a writer task.
 */

#ifndef GUCEF_MT_DVRWLOCK_H
#define GUCEF_MT_DVRWLOCK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"             /* simple types used */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"     /* often used gucef macros */
#define GUCEF_MT_GUCEFMT_MACROS_H
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *	Storage used for the readers/writers lock.
 */
struct SRWLock;
typedef struct SRWLock TRWLock;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace MT {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*-------------------------------------------------------------------------*/

/**
 *      Function that creates a readers/writers lock data storage struct
 *      writer_overrules is a boolean. When non zero writers will be given
 *      a higher priority then readers. If 0 then readers will have priority
 *      over writers.
 */
GUCEF_MT_PUBLIC_C TRWLock*
rwl_create( UInt32 writer_overrules );

/*-------------------------------------------------------------------------*/

/**
 *      Cleanup a readers/writers lock.
 *      This function should only be called when all threads using the lock have
 *      finished using it. There are some safeguards but they are not fullproof.
 */
GUCEF_MT_PUBLIC_C void
rwl_destroy( TRWLock *rwlock );

/*-------------------------------------------------------------------------*/

/**
 *      Returns a boolean value. 1 if writers have priority and 0 if readers
 *      have priority.
 */
GUCEF_MT_PUBLIC_C UInt32
rwl_writer_overrules( const TRWLock *rwlock );

/*-------------------------------------------------------------------------*/

/**
 *      Returns the current amount of writers. Note that this function
 *      is meant for output to humans. It has no use in relation with
 *      the other rwl_ functions.
 */
GUCEF_MT_PUBLIC_C UInt32
rwl_writers( const TRWLock *rwlock );

/*-------------------------------------------------------------------------*/

/**
 *      Returns the current amount of readers. Note that this function
 *      is meant for output to humans. It has no use in relation with
 *      the other rwl_ functions.
 */
GUCEF_MT_PUBLIC_C UInt32
rwl_readers( const TRWLock *rwlock );

/*--------------------------------------------------------------------------*/

/**
 *      Call this function when a reader task needs access to the data
 *      protected by the given rwlock. Fails if destroy has been called for
 *      the given rwlock. If it fails you should no longer access the given
 *      rwlock.
 */
GUCEF_MT_PUBLIC_C UInt32
rwl_reader_start( TRWLock *rwlock );

/*--------------------------------------------------------------------------*/

/**
 *      Call this function when a reader task finished using data that is
 *      protected by the given rwlock.
 */
GUCEF_MT_PUBLIC_C void
rwl_reader_stop( TRWLock *rwlock );

/*--------------------------------------------------------------------------*/

/**
 *      Call this function when a writer task needs access to the data
 *      protected by the given rwlock. Fails if destroy has been called for
 *      the given rwlock. If it fails you should no longer access the given
 *      rwlock. A return value of 1 indicates success and 0 failure.
 */
GUCEF_MT_PUBLIC_C UInt32
rwl_writer_start( TRWLock *rwlock );

/*--------------------------------------------------------------------------*/

/**
 *      Call this function when a writer task finished using data that is
 *      protected by the given rwlock.
 */
GUCEF_MT_PUBLIC_C void
rwl_writer_stop( TRWLock *rwlock );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace MT */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_DVRWLOCK_H ? */
