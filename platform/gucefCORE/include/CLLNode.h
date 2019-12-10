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
class GUCEF_CORE_PUBLIC_CPP CLLNode
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


