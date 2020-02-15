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

#ifndef GUCEF_MT_CCONDITION_H
#define GUCEF_MT_CCONDITION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"     /* often used gucef macros */
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"             /* simple types used */
#endif /* GUCEF_MT_ETYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Foreward declaration of the actual condition storage structure.
 */
struct SDL_cond;

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

/**
 *      Wrapper class for SDL condition's
 *      The usage of this class allows you to forgo having to type the same
 *      tests and allocation/de-allocation code all the time.
 */
class GUCEF_MT_PUBLIC_CPP CCondition
{
        private:

        struct SDL_cond* cond;

        CCondition( const CCondition& src );  /* Copying doesnt make sense */
        CCondition& operator=( const CCondition& src );   /* Copying doesnt make sense */

        public:

        /**
         *      Restart one of the threads that are waiting on the condition
         *      variable, returns false on error.
         */
        bool Signal( void );

        /**
         *      Restart all threads that are waiting on the condition variable
         *      returns false on error.
         */
        bool Broadcast( void );

        /**
         *      Wait on the condition variable, unlocking the provided mutex.
         *      The mutex must be locked before entering this function!
         *      returns false on error.
         */
        bool WaitAndUnlock( CMutex& mutex );

        /**
         *      Waits for at most 'ms' milliseconds, and returns 0 if the
         *      condition variable is signaled, 1 if the condition is not
         *      signaled in the allotted time, and -1 on error.
         *      On some platforms this function is implemented by looping with a
         *      delay of 1 ms, and so should be avoided if possible.
         */
        Int32 WaitAndUnlockOrTimeout( CMutex& mutex ,
                                      UInt32 ms     );

        CCondition( void );

        ~CCondition();

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CCONDITION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 15-10-2004 :
       - Designed and implemented this class.

-----------------------------------------------------------------------------*/
