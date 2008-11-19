/*
 *  gucefMT: GUCEF module providing multithreading solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
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
 *      O/S Mutex wrapper
 */
class GUCEFMT_EXPORT_CPP CMutex
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

        void* _mutexdata;

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
