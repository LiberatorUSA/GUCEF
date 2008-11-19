/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
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
class GUCEFCORE_EXPORT_CPP CDataQueue
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
 