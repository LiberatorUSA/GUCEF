/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2003.  All rights reserved.
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CLLNode.h"              /* definition for this class */
#ifndef GUCEF_CORE_TSPRINTING_H
#include "tsprinting.h"		  /* threadsafe printing */ 
#endif /* GUCEF_CORE_TSPRINTING_H ? */

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------//

CLLNode::CLLNode( void )
{
        GUCEF_BEGIN;

        /*
         *      Default contructor
         */
        next = NULL;
        prev = NULL;
        link = NULL;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CLLNode::CLLNode( void* set_link )
{
        GUCEF_BEGIN;

        /*
         *      contructor, init link to given void*
         */
        next = NULL;
        prev = NULL;
        link = set_link;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CLLNode::~CLLNode()
{
        GUCEF_BEGIN;

        /*
         *      destructor
         */
        Detach();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CLLNode::Detach( void )
{
        GUCEF_BEGIN;

	/*
         *	Detaches this node from the linked list
         */

        if ( prev )
        {
        	if ( next )
                {
                	next->prev = prev;
                        prev->next = next;
                        GUCEF_END;
                        return;
                }
        	prev->next = NULL;
        	GUCEF_END;
                return;
        }
        if ( next )
        {
        	next->prev = NULL;
        }
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CLLNode::AppendTo( CLLNode *node )
{
        GUCEF_BEGIN;
        
        /*
         *      Append this node to the end of the list given by the node
         *      specified
         */
        CLLNode *tmp_node = node; 
        while ( tmp_node->GetNext() )
        {
                tmp_node = tmp_node->GetNext();
        }

        tmp_node->SetNext( this );
        SetPrev( tmp_node );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CLLNode::PrefixTo( CLLNode *node )
{
        GUCEF_BEGIN;

        /*
         *      prefix this node to the start of the list given by the node
         *      specified
         */
        CLLNode *tmp_node = node;
        while ( tmp_node->GetPrev() )
        {
                tmp_node = tmp_node->GetPrev();
        }
        tmp_node->SetPrev( this );
        SetNext( tmp_node );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CLLNode*
CLLNode::GetNext( void )
{
        GUCEF_BEGIN;

        /*
         *      return the next pointer
         */
        GUCEF_END;
        return next;
}

/*-------------------------------------------------------------------------*/

void
CLLNode::SetNext( CLLNode *set_next )
{
        GUCEF_BEGIN;

        /*
         *      Set the next pointer
         */
        next = set_next;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CLLNode*
CLLNode::GetPrev( void )
{
        GUCEF_BEGIN;

        /*
         *      Return the previous node
         */
        GUCEF_END; 
        return prev;
}

/*-------------------------------------------------------------------------*/

void
CLLNode::SetPrev( CLLNode *set_prev )
{
        GUCEF_BEGIN;
        
        /*
         *      Set the prev pointer
         */
        prev = set_prev; 
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

