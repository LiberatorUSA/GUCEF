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
 *
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE. 
 */

#ifndef GUCEF_CORE_CNODE_H
#define GUCEF_CORE_CNODE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

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

class EXPORT_CPP CNode
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

