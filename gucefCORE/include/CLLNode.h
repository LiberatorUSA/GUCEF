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

#ifndef GUCEF_CORE_CCLLNODE_H
#define GUCEF_CORE_CCLLNODE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucefCORE macros */
#endif /* GUCEF_CORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

//-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------//

/**
 *      Simple class to serve as a node in a linked list. Provides the link
 *      void pointer as a generic link to whatever you want linked in a linked
 *      list. Note that this is not a cyclic linked list, thus the last and prev
 *      nodes will be NULL at either end of the list.
 */
class GUCEFCORE_EXPORT_CPP CLLNode
{
        private :
        CLLNode *next;  /* next node in list */
        CLLNode *prev;  /* previous node in list */
        void *link;     /* our generic link to whatever we want in our list */

        public :

        /*
         *      Get and Set
         */
        CLLNode* GetNext( void );                  /* Get next node in list */
        void SetNext( CLLNode *set_next );         /* Set next node in list */
        CLLNode* GetPrev( void );                  /* Get previous node in list */
        void SetPrev( CLLNode *set_prev );         /* Set previuos node in list */
        void* GetLink( void ) { return link; }     /* Get our linked data */ 

        /*
         *      Linked list manipulation utils
         */
        void AppendTo( CLLNode *node );        /* append this node to the linked list given */
        void PrefixTo( CLLNode *node );        /* prefix this node to the linked list given */
        void Detach( void );                   /* detach this node from the list */

        /*
         *      Con & Destructor
         */
        CLLNode( void ); 
        CLLNode( void* set_link );
        virtual ~CLLNode();
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CCLLNODE_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 14-11-2004 :
        - Added this section.

-----------------------------------------------------------------------------*/


