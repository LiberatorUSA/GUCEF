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

#ifndef GUCEF_MT_CMUTEX_H
#define GUCEF_MT_CMUTEX_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"     /* often used gucef macros */
#define GUCEF_MT_GUCEFMT_MACROS_H
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */

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

struct SMutexData;

/*-------------------------------------------------------------------------*/

/**
 *      O/S Mutex wrapper
 */
class GUCEF_MT_PUBLIC_CPP CMutex
{
    public:

    /**
     *      Lock the mutex. If the mutex is already locked the calling
     *      process will have to wait for the mutex to allow a lock.
     *      The return value indicates whether the lock failed or succeeded.
     */
    bool Lock( void ) const;

    /**
     *      Unlocks the mutex after a call to Lock_Mutex(). Other processes
     *      will have the ability to get a mutex lock after this call.
     *      The return value indicates whether the unlock failed or succeeded.
     */
    bool Unlock( void ) const;

    /**
     *      Default constructor, allocates storage for a mutex.
     */
    CMutex( void );

    /**
     *      Destructor, de-allocates storage for a mutex.
     */
    ~CMutex();

    private:
    friend class CCondition;

    struct SMutexData* _mutexdata;

    CMutex( const CMutex& src );      /* Copying doesnt make sense */
    CMutex& operator=( const CMutex& src );   /* Copying doesnt make sense */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_MUTEX( varName )       ( GUCEF::MT::CMutex varName; )
#define GUCEF_LOCKMUTEX( varName )   ( varName.Lock(); )
#define GUCEF_UNLOCKMUTEX( varName ) ( varName.Unlock(); )

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CMUTEX_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
       - Wrote a new implementation of the member functions to get rid of the
         dependency on SDL.
- 11-03-2004 :
       - Designed and implemented this class.

-----------------------------------------------------------------------------*/
