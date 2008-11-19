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

//-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------//

#include "CStack.h"      /* definition of this CStack class */
#include "CLLNode.h"     /* class as single node in a linked list */
#include "tsprinting.h"	 /* threadsafe printing */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */  

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

CStack::CStack( void )
{
        GUCEF_BEGIN;

        /*
         *      Default contructor
         */
        first = NULL;
        last = NULL;
        nodecount = 0;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CStack::CStack( const CStack &stack )
{
        GUCEF_BEGIN;
        /*
         *      Copy contructor
         */
/*        nodecount = 0;
        CStack tmp;
        for ( int i=0; i<stack.nodecount; i++ )
        {
                /*
                 *      Copy CLLNode's into new stack
                 */
/*                CLLNode *node;
                stack.Pop( ( void** )&node );
                tmp.Pop( node );
                Push( new CLLNode (  ) );
        }
*/
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CStack::~CStack()
{
        GUCEF_BEGIN;

        /*
         *      Destructor, cleanup list
         */
        Clear_Stack();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CStack::Push( void *data )
{
        GUCEF_BEGIN;

        /*
         *      Push node onto the stack - FILO
         */
        CLLNode *new_node = new CLLNode( data );
        CHECKMEM( new_node, sizeof( CLLNode ) );

        nodecount++;
        if ( first )
        {
                new_node->PrefixTo( first );
                first = new_node;
                GUCEF_END;
                return;
        }
        first = new_node;
        last = new_node;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CStack::Pop( void **data )
{
        GUCEF_BEGIN;

        /*
         *      Remove node from stack - FILO
         */
        if ( !nodecount )
        {
                /*
                 *      Stack is empty, nothing to pop
                 */
                *data = NULL;
                GUCEF_END;
                return;
        }

        CLLNode *tmp_node = first;
        first = first->GetNext();
        if ( first ) first->SetPrev( NULL );
        nodecount--;
        *data = tmp_node->GetLink();
        if ( tmp_node == last )
        {
                /*
                 *      Stack is now empty
                 */
                first = NULL;
                last = NULL;
        }
        delete tmp_node;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CStack::Get_Top( void **data )
{
        GUCEF_BEGIN;

        /*
         *      Return topmost node on stack, together with Push FIFO.
         *      reduces stack size with 1
         */
        if ( last )
        {
                CLLNode *tmp_node = last;
                last = last->GetPrev();
                nodecount--;
                *data = tmp_node->GetLink();
                if ( first == tmp_node )
                {
                	first = NULL;
                }
                CHECKMEM( tmp_node, sizeof(CLLNode) );
                delete tmp_node;

                GUCEF_END;
                return;
        }
        *data = NULL;
        GUCEF_END;
        return;
}

/*-------------------------------------------------------------------------*/

void
CStack::Set_Top( void *data )
{
        GUCEF_BEGIN;

        /*
         *      Set topmost node on stack, together with Push FIFO
         */
        CLLNode *new_node = new CLLNode( data );
        CHECKMEM( new_node, sizeof(CLLNode) );

        nodecount++;
        if ( last )
        {
                /*
                 *      Append the new node to the end of the linked list
                 */
                new_node->AppendTo( last );
                last = new_node;
                GUCEF_END;
                return;
        }
        first = new_node;
        last = new_node;
        GUCEF_END;
}


/*-------------------------------------------------------------------------*/

UInt32
CStack::Count( void ) const
{
        GUCEF_BEGIN;

        /*
         *      Return number of nodes on stack
         */
        GUCEF_END; 
        return nodecount;
}

/*-------------------------------------------------------------------------*/

void
CStack::Clear_Stack( void )
{
        GUCEF_BEGIN;

        /*
         *      Clear all nodes from stack
         *      begin at last a somewhat greater security
         */
        CLLNode *tmp_node = last;
        CLLNode *delnode;
        while ( tmp_node )
        {
                delnode = tmp_node;
                tmp_node = tmp_node->GetPrev();
                CHECKMEM( delnode, sizeof( CLLNode ) );
                delete delnode;
        }
        nodecount=0;
        GUCEF_END;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

