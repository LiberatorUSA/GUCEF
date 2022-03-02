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

class GUCEF_MT_PUBLIC_CPP CScopeReaderLock
{
    public:

    CScopeReaderLock( const CReadWriteLock& rwLock );

    ~CScopeReaderLock();

    bool IsReadLocked( void ) const;

    bool IsWriteLocked( void ) const;

    /**
     *  Allows you to unlock before the scope lock triggers destruction of the CScopeMutex object
     *  Useful for more complex code flows where in most code paths you want to retain the lock except for
     *  a small subset of code path(s)
     */
    bool EarlyUnlock( void );

    private:
    const CReadWriteLock* m_rwLock;
    bool m_isReadLocked;

    CScopeReaderLock( void );                                     /* can't use */
    CScopeReaderLock( const CScopeReaderLock& src );              /* Copying doesnt make sense */
    CScopeReaderLock& operator=( const CScopeReaderLock& src );   /* Copying doesnt make sense */
};

/*--------------------------------------------------------------------------*/

class GUCEF_MT_PUBLIC_CPP CScopeWriterLock
{
    public:

    CScopeWriterLock( const CReadWriteLock& rwLock );

    ~CScopeWriterLock();

    bool IsReadLocked( void ) const;

    bool IsWriteLocked( void ) const;

    /**
     *  Allows you to unlock before the scope lock triggers destruction of the CScopeMutex object
     *  Useful for more complex code flows where in most code paths you want to retain the lock except for
     *  a small subset of code path(s)
     */
    bool EarlyUnlock( void );

    private:
    const CReadWriteLock* m_rwLock;
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
