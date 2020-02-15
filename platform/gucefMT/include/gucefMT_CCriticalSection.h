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

#ifndef GUCEF_MT_CCRITICALSECTION_H
#define GUCEF_MT_CCRITICALSECTION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"           /* simple types used */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

#ifndef GUCEF_MT_MACROS_H
#include "gucefMT_macros.h"           /* often used gucef macros */
#define GUCEF_MT_MACROS_H
#endif /* GUCEF_MT_MACROS_H ? */

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
 *      O/S Wrapper class for a critical section.
 *
 *      A critical section object provides synchronization similar to that
 *      provided by a mutex object, except that a critical section can be used
 *      only by the threads of a single process. Event, mutex, and semaphore
 *      objects can also be used in a single-process application, but critical
 *      section objects provide a slightly faster, more efficient mechanism
 *      for mutual-exclusion synchronization (a processor-specific test and set
 *      instruction). Like a mutex object, a critical section object can be
 *      owned by only one thread at a time, which makes it useful for protecting
 *      a shared resource from simultaneous access. There is no guarantee about
 *      the order that threads obtain ownership of the critical section,
 *      however, the system is fair to all threads. Unlike a mutex object, there
 *      is no way to tell whether a critical section has been abandoned.
 *
 */
class GUCEF_MT_PUBLIC_CPP CCriticalSection
{
        private:
        void* _data;
        CCriticalSection( const CCriticalSection& ); /* copy's arent allowed */
        CCriticalSection& operator=( const CCriticalSection& );        /* copy's arent allowed */

        public:

        CCriticalSection( void );

        ~CCriticalSection();

        /**
         *      Signals a willingness to start using protected data/code.
         *      blocks until the thread can take ownership of the critical
         *      section.
         */
        void Enter( void );

        /**
         *      Signals that we are done with the protected data/code.
         */
        void Leave( void );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CCRITICALSECTION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Designed and implemented this class.

-----------------------------------------------------------------------------*/
