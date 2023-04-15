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

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#define GUCEF_MT_CSCOPEMUTEX_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"     /* often used gucef macros */
#define GUCEF_MT_GUCEFMT_MACROS_H
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */

#ifndef GUCEF_TIMEOUT_EXCEPTION_H
#include "gucef_timeout_exception.h"
#define GUCEF_TIMEOUT_EXCEPTION_H
#endif /* GUCEF_TIMEOUT_EXCEPTION_H ? */

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

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

class CMutex;

/*--------------------------------------------------------------------------*/

/**
 *      Mutex class implementation that currently supports mswindows and linux
 *      Simply create an object of this class to lock the scope with a mutex.
 *      Perfect if you don't want to make Unlock() calls all over the place.
 */
class GUCEF_MT_PUBLIC_CPP CScopeMutex
{
    public:

    /**
     *  locks the mutex 
     *  throws timeout_exception if unable to attain the lock before the specified timeout 
     */
    CScopeMutex( const CMutex& mutex, UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS );

    /**
     *  attempts to lock the mutex
     *  If the lock operation fails for whatever reason it is the caller's responsibility to handle that scenario via
     *  the lockStatus output parameter
     */
    CScopeMutex( const CMutex& mutex, UInt32 lockWaitTimeoutInMs, TLockStatus& lockStatus );

    /**
     *  Unlocks the mutex.
     */
    ~CScopeMutex();

    /**
     *  Returns whether the current lock state based on the Lock() and Unlock()
     *  operations on the mutex provided.
     */
    bool IsLocked( void ) const;

    /**
     *  Allows you to unlock before the scope lock triggers destruction of the CScopeMutex object
     *  Useful for more complex code flows where in most code paths you want to retain the lock except for
     *  a small subset of code path(s)
     */
    bool EarlyUnlock( void );

    /**
     *  Allows you to re lock the lock after an EarlyUnlock()
     *  throws timeout_exception if unable to attain the lock before the specified timeout 
     */
    bool ReLock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS );

    /**
     *  Allows you to re lock the lock after an EarlyUnlock()
     *  If the lock operation fails for whatever reason it is the caller's responsibility to handle that scenario via
     *  the TLockStatus return value
     */
    TLockStatus TryReLock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS );

    private:
    const CMutex* m_mutex;
    bool m_isLocked;

    CScopeMutex( const CScopeMutex& src );      /* Copying doesnt make sense */
    CScopeMutex& operator=( const CScopeMutex& src );   /* Copying doesnt make sense */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */
