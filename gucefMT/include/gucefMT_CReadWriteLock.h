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
class GUCEFMT_EXPORT_CPP CReadWriteLock
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
