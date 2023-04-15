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

#ifndef GUCEF_MT_CILOCKABLE_H
#define GUCEF_MT_CILOCKABLE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"             /* simple types */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

#ifndef GUCEF_MT_MACROS_H
#include "gucefMT_macros.h"             /* often used gucef macros */
#define GUCEF_MT_MACROS_H
#endif /* GUCEF_MT_MACROS_H ? */

#ifndef GUCEF_MT_LOCKSTATUS_H
#include "gucefMT_LockStatus.h"
#define GUCEF_MT_LOCKSTATUS_H
#endif /* GUCEF_MT_LOCKSTATUS_H ? */

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

/**
 *  Interface class that adds a locking interface to derived classes.
 */
class GUCEF_MT_PUBLIC_CPP CILockable
{
    public:

    CILockable( void );

    CILockable( const CILockable& src );

    virtual ~CILockable();

    CILockable& operator=( const CILockable& src );

    virtual const CILockable* AsLockable( void ) const = 0;

    protected:
    friend class CObjectScopeLock;
    friend class CObjectScopeReadOnlyLock;

    /**
     *  Should be implemented by the derived classes such that all interactions with the class
     *  member functions and variables are protected against multiple threads accessing them
     *  Typical implementation would be to have this call Lock() on a mutex member in a derived class
     */
    virtual TLockStatus Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const = 0;

    /**
     *  Counterpart to the Lock() member function. This releases the lock obtained using Lock() 
     *  Typical implementation would be to have this call Unlock() on a mutex member in a derived class
     */
    virtual TLockStatus Unlock( void ) const = 0;

    /**
     *  Should be implemented by the derived classes such that all interactions with the class
     *  member functions and variables are protected against multiple threads accessing them
     *  Typical implementation would be to have this call Lock() as a reader on a reader-writer lock in a derived class
     */
    virtual TLockStatus ReadOnlyLock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const { return Lock( lockWaitTimeoutInMs ); }

    /**
     *  Counterpart to the ReadOnlyLock() member function. This releases the lock obtained using ReadOnlyLock() 
     *  Typical implementation would be to have this call Unlock() as a reader on a reader-writer lock in a derived class
     */
    virtual TLockStatus ReadOnlyUnlock( void ) const { return Unlock(); };
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CILOCKABLE_H  ? */
