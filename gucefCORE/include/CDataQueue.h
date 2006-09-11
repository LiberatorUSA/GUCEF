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

#ifndef GUCEF_CORE_CDATAQUEUE_H
#define GUCEF_CORE_CDATAQUEUE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CBLOCKSTACK_H
#include "CBlockStack.h"         /* stack implementation with fixed memory block sizes */
#define GUCEF_CORE_CBLOCKSTACK_H
#endif /* GUCEF_CORE_CBLOCKSTACK_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Class implementing a queue for data.
 *      Memory allocations are kept to a minimum with this class using memory
 *      pools ect. Simply allows you to set data in the FILO queue and later
 *      retrieve a pointer to it. When your done with the queued item you drop
 *      it from the queue.
 */
class EXPORT_CPP CDataQueue
{
        public:
        
        /**
         *      Simply constructs a dataqueue, nothing special
         */
        CDataQueue( void );

        /**
         *      Deallocates all remaining buffers and thus invalidates 
         *      all outstanding pointers retrieved with Get()
         */
        ~CDataQueue();

        /**
         *      Adds the data given in src to the data queue. 
         *      If no buffer is available to store the data one will be created
         */
        void Set( const void* src ,
                  UInt32 datasize );
                       
        /**
         *      Retrieves a pointer to queued data and the 
         *      size of the queued item.
         *      If there is no more queued data the datasize will be set to 0
         *      Note that Get() only retrieves a pointer to a queued item, it does
         *      not remove the item from the queue. Use Drop() to drop the item from 
         *      the queue. This will invalidate the pointer retrieved with Get()
         */                                                 
        void Get( const void** dataptr ,
                  UInt32* datasize     );
        
        /**
         *      Drops the item last retrieved with Get() from the data queue
         *      This free's up the buffer for reuse.
         */
        void Drop( void );       

        private:        
        CBlockStack _queue;
        CBlockStack _freebuffers;      
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDATAQUEUE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
 