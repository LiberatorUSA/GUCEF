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

#ifndef GUCEF_MT_CSEMAPHORE_H
#define GUCEF_MT_CSEMAPHORE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"             /* simple types */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"             /* often used gucef macros */
#define GUCEF_MT_GUCEFMT_MACROS_H
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Foreward declaration of the actual mutex storage structure.
 */
struct SDL_semaphore;

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
 *      Wrapper class for SDL semaphore's.
 *      The usage of this class allows you to forgo having to type the same
 *      tests and allocation/de-allocation code all the time.
 */
class GUCEF_MT_PUBLIC_CPP CSemaphore
{
        public:

        /**
         *      Suspends the calling thread until the semaphore has a positive
         *      count. It then atomically decreases the semaphore count.
         *      returns false in case of error.
         */
        bool Wait( void );

        /**
         *      Non-blocking variant of Wait(), returns 0 if the wait succeeds,
         *      1 if the wait would block, and -1 on error.
         */
        Int32 TryWait( void );

        /**
         *      Variant of Wait() with a timeout in milliseconds, returns 0 if
         *      the wait succeeds, 1 if the wait does not succeed in
         *      the allotted time, and -1 on error. On some platforms this
         *      function is implemented by looping with a delay of 1 ms, and so
         *      should be avoided if possible.
         */
        Int32 WaitOrTimeout( UInt32 ms );

        /**
         *      Atomically increases the semaphore's count (not blocking)
         *      returns false in case of error.
         */
        bool Post( void );

        /**
         *      Returns the current count of the semaphore
         */
        UInt32 Value( void ) const;

        /**
         *      constructor, allocates storage for a semaphore.
         */
        CSemaphore( UInt32 initial_value );

        /**
         *      Destructor, de-allocates storage for a semaphore.
         */
        ~CSemaphore();

        private:

        struct SDL_semaphore *sem;    /* SDL semaphore structure which we wrap */

        CSemaphore( void );                   /* we want an initial value  */
        CSemaphore( const CSemaphore& src );  /* Copying doesnt make sense */
        CSemaphore& operator=( const CSemaphore& src );   /* Copying doesnt make sense */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CSEMAPHORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-03-2004 :
       - Designed and implemented this class.

-----------------------------------------------------------------------------*/

