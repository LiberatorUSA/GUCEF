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

#ifndef GUCEF_MT_CREADWRITELOCK_H
#define GUCEF_MT_CREADWRITELOCK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"           /* simple types used */
#endif /* GUCEF_MT_ETYPES_H ? */

#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"     /* often used gucef macros */
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Forward decalaration of the storage structure for the actual readers
 *      writers lock implementation.
 */
struct SRWLock;

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
 *      Wrapper class for my C implementation of a readers/writers lock
 */
class GUCEF_MT_PUBLIC_CPP CReadWriteLock
{
        public:

        /**
         *      Creates the readers/writers lock with the given priority setting
         *      @param writers_overrule Wheter writers or readers have priority.
         */
        CReadWriteLock( bool writers_overrule );

        /**
         *      Cleans up the lock.
         *      The destructor will block untill all readers and writers are
         *      finished !!!.
         */
        ~CReadWriteLock();

        /**
         *      Use when a writer task needs access to the data protected by
         *      the given lock. Blocks untill access is granted.
         *      Returns wheter the operation was successfull.
         *      In case of failure it returns imediatly and the return value
         *      will be false. In such a case the lock should no longer be used.
         */
        bool WriterStart( void );

        /**
         *      Use when a writer task finished using data/code that is
         *      protected by this lock.
         */
        void WriterStop( void );

        /**
         *      Returns the current number of writers. Note that this function
         *      is meant for output to humans. Race conditions ect. make it
         *      useless for any control code.
         */
        UInt32 WriterCount( void );

        /**
         *      Use when a reader task needs access to the data protected by
         *      the given lock. Blocks untill access is granted (if a writer is
         *      currently busy). Returns wheter the operation was successfull.
         *      In case of failure it returns imediatly and the return value
         *      will be false. In such a case the lock should no longer be used.
         */
        bool ReaderStart( void );

        /**
         *      Use when a reader task finished using data/code that is
         *      protected by this lock.
         */
        void ReaderStop( void );

        /**
         *      Returns the current number of readers. Note that this function
         *      is meant for output to humans. Race conditions ect. make it
         *      useless for any control code.
         */
        UInt32 ReaderCount( void );

        /**
         *      Returns wheter readers or writers have prioritity.
         *      True in case the writers have priority and false if the readers
         *      have priority.
         */
        bool DoWritersOverrule( void );

        private:
        struct SRWLock *_rwlock;                  /* encapsulated rwlock struct */

        CReadWriteLock( void );                   /* can't use this: need data */
        CReadWriteLock( const CReadWriteLock& );  /* copy's arent allowed */
        CReadWriteLock& operator=( const CReadWriteLock& );  /* copy's arent allowed */
};

/*------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Designed and implemented this class.

-----------------------------------------------------------------------------*/
