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

#ifndef GUCEF_CORE_CNODE_H
#define GUCEF_CORE_CNODE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CNode
{
        public:
	bool IsFirst( void ) const;
	bool IsLast( void ) const;

        CNode* GetNextNode( void ) const;
        CNode* GetPrevNode( void ) const;
        CNode* GetParentNode( void ) const;
        CNode* GetChildNode( void ) const;

	void AttachChildNode( CNode *pnewchild );  /* attach new child to beginning of list */
        void AppendChildNode( CNode *pnewchild  ); /* attach new child to end of list */
	void AttachToNode( CNode *pnewparent );    /* attach to new parent */
	void DetachNode();

	CNode* FindRootNode( void );

        /*
         *      The following member functions allow you mutate the ordering
         *      of nodes and subsequently there children. the child node is
         *      considdered front and the last node the back. Note that the
         *      Move_To_Front() and Move_To_Back() member functions have no
         *      effect unless the node has a parent.
         */
        void MoveToFront( void );
        void MoveToBack( void );
        void MoveForward( void );
        void MoveBackward( void );

	CNode( void );
	CNode( CNode *pnode );
	virtual ~CNode();

        protected:
	CNode* _pparent;
	CNode* _pchild;
        CNode* _pprev;
	CNode* _pnext;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif	/* GUCEF_CORE_CNODE_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 05-04-2005 :
       - Updated file layout
       - Got rid of all the inlined code
- 23-08-2003 :
       - Removed friend with itself relationship
- 26-06-2003 :
       - Added member functions that allow me to influence the ordering of
         nodes in the list at the node level of implementation.
- 11-06-2003 :
        - Added this section.
        - Added Append_Child() because then i can use indexes to refrence nodes
          whithout needing to change the indexes everytime i add to the list.
        - Removed any and all memory mangement from this class to ensure
          in-scope memory management.

---------------------------------------------------------------------------*/

