/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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
class GUCEF_EXPORT_CPP CSemaphore
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

