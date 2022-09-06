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

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#define GUCEF_MT_CSCOPERWLOCK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_MACROS_H
#include "gucefMT_macros.h"     /* often used gucef macros */
#define GUCEF_MT_MACROS_H
#endif /* GUCEF_MT_MACROS_H ? */

#ifndef GUCEF_MT_CREADWRITELOCK_H
#include "gucefMT_CReadWriteLock.h"
#define GUCEF_MT_CREADWRITELOCK_H
#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

// Forward declarations for mutual friendship
class CScopeReaderLock;
class CScopeWriterLock;

/*--------------------------------------------------------------------------*/

class GUCEF_MT_PUBLIC_CPP CScopeReaderLock
{
    public:

    typedef CReadWriteLock::TRWLockStates   TRWLockStates;

    CScopeReaderLock( const CReadWriteLock& rwLock );

    /**
     *  Performs a secure handoff from a write lock into a read lock for the given scope
     *  If successfull the scoped write lock will no longer be considered locked similar to an EarlyUnlock()
     */
    CScopeReaderLock( CScopeWriterLock& writerToTransition );

    ~CScopeReaderLock();

    /**
     *  Provides a snapshot-in-time state of active readers existing for the referenced lock
     *  This cannot be used for coordination across threads or to drive any logic.
     *  Its advisory only for human consuption or at most a unit test under controlled conditions
     */
    bool HasActiveReaders( void ) const;

    /**
     *  Provides a snapshot-in-time state of active writers existing for the referenced lock
     *  This cannot be used for coordination across threads or to drive any logic.
     *  Its advisory only for human consuption or at most a unit test under controlled conditions
     */
    bool HasActiveWriters( void ) const;
    
    bool IsScopeReadLocked( void ) const;    

    /**
     *  Allows you to unlock before the scope lock triggers destruction of the CScopeMutex object
     *  Useful for more complex code flows where in most code paths you want to retain the lock except for
     *  a small subset of code path(s)
     */
    TRWLockStates EarlyUnlock( void );

    /**
     *  Allows you to release the read lock before the end of the scope but do so in a manner that transfers
     *  the active scope lock to the given write lock by escalating to a write lock. This particular scope lock object will be considered unlocked after success.
     *
     *  Using these types of transitions allows one to signal that threads are accessing protected data in a read or read-write
     *  fashion without having a potential time window where the accounting would suggest no threads are looking at protected data at all anymore
     */
    bool TransitionToWriter( CScopeWriterLock& targetWriter );

    private:
    friend class CScopeWriterLock;

    const CReadWriteLock* m_rwLock;
    TRWLockStates m_lockState;
    bool m_isReadLocked;

    CScopeReaderLock( void );                                     /* can't use */
    CScopeReaderLock( const CScopeReaderLock& src );              /* Copying doesnt make sense */
    CScopeReaderLock& operator=( const CScopeReaderLock& src );   /* Copying doesnt make sense */
};

/*--------------------------------------------------------------------------*/

class GUCEF_MT_PUBLIC_CPP CScopeWriterLock
{
    public:

    typedef CReadWriteLock::TRWLockStates   TRWLockStates;
    
    CScopeWriterLock( const CReadWriteLock& rwLock );

    /**
     *  Performs a secure handoff from a read lock into a write lock for the given scope
     *  If successfull the scoped read lock will no longer be considered locked similar to an EarlyUnlock()
     *
     *  If you unlock your read lock to then independently attain a write lock you may find
     *  that your data references in your scope have become invalid leading to access violations and the like
     *  since the code could be shutting down and be cleaning up if the lock is not kept or similar scenarios.
     */
    CScopeWriterLock( CScopeReaderLock& readerToTransition );

    ~CScopeWriterLock();

    /**
     *  Provides a snapshot-in-time state of active readers existing for the referenced lock
     *  This cannot be used for coordination across threads or to drive any logic.
     *  Its advisory only for human consuption or at most a unit test under controlled conditions
     */
    bool HasActiveReaders( void ) const;

    /**
     *  Provides a snapshot-in-time state of active writers existing for the referenced lock
     *  This cannot be used for coordination across threads or to drive any logic.
     *  Its advisory only for human consuption or at most a unit test under controlled conditions
     */
    bool HasActiveWriters( void ) const;

    bool IsScopeWriteLocked( void ) const;

    UInt32 GetWriterReentrancyDepth( void ) const;

    /**
     *  Allows you to unlock before the scope lock triggers destruction of the CScopeMutex object
     *  Useful for more complex code flows where in most code paths you want to retain the lock except for
     *  a small subset of code path(s)
     */
    TRWLockStates EarlyUnlock( void );

    /**
     *  Allows you to release the write lock before the end of the scope but do so in a manner that transfers
     *  The active scope lock to the given read-only lock. This particular scope lock object will be considered unlocked after success.
     *
     *  Using these types of transitions allows one to signal that threads are accessing protected data in a read or read-write
     *  fashion without having a potential time window where the accounting would suggest no threads are looking at protected data at all anymore
     */
    bool TransitionToReader( CScopeReaderLock& targetReader );

    private:
    friend class CScopeReaderLock;

    const CReadWriteLock* m_rwLock;
    TRWLockStates m_lockState;
    bool m_isWriteLocked;

    CScopeWriterLock( void );                                     /* can't use */
    CScopeWriterLock( const CScopeWriterLock& src );              /* Copying doesnt make sense */
    CScopeWriterLock& operator=( const CScopeWriterLock& src );   /* Copying doesnt make sense */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */
