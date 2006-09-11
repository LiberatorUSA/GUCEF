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
class GUCEFMT_EXPORT_CPP CScopeMutex
{
        public:

        /**
         *      Default constructor, allocates storage for a mutex and locks
         *      the mutex
         */
        CScopeMutex( CMutex& mutex );

        /**
         *      Destructor, Unlocks and then de-allocates storage for a mutex.
         */
        ~CScopeMutex();

        private:
        CMutex* m_mutex;

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

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-08-2005 :
       - Designed and implemented this class.

-----------------------------------------------------------------------------*/