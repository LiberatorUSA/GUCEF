/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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

/**
 *      Mutex class implementation that currently supports mswindows and linux
 */
class GUCEFMT_EXPORT_CPP CMutex
{
        public:

        /**
         *      Lock the mutex. If the mutex is already locked the calling
         *      process will have to wait for the mutex to allow a lock.
         *      The return value indicates wheter the lock failed or succeeded.
         */
        bool Lock( void ) const;

        /**
         *      Unlocks the mutex after a call to Lock_Mutex(). Other processes
         *      will have the ability to get a mutex lock after this call.
         *      The return value indicates wheter the unlock failed or succeeded.
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

        void* _mutexdata;

        CMutex( const CMutex& src );      /* Copying doesnt make sense */
        CMutex& operator=( const CMutex& src );   /* Copying doesnt make sense */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_MUTEX( varname ) (CMutex varname;)
#define GUCEF_LOCKMUTEX( varname ) (varname.Lock();)
#define GUCEF_UNLOCKMUTEX( varname ) (varname.Unlock();)

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CMUTEX_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
       - Wrote a new implementation of the member functions to get rid of the
         dependancy on SDL.
- 11-03-2004 :
       - Designed and implemented this class.

-----------------------------------------------------------------------------*/
