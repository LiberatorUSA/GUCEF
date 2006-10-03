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

#ifndef GUCEF_CORE_CSTACK_H
#define GUCEF_CORE_CSTACK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_ETypes.h"     /* simple types used */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
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

/*
 *      Forward declarations of classes used
 */
class CLLNode; 

/*-------------------------------------------------------------------------*/

/*
 *      Simple implementation of a stack class. Both FIFO ( First In First Out )
 *      and FILO ( First In Last Out ) are supported, uses a linked list
 *      implementation internally with the CLLNode class
 */
class GUCEFCORE_EXPORT_CPP CStack
{
        private :
        CLLNode *first;                      /* bottom node of stack, which is the first in our linked list */
        CLLNode *last;                       /* top node of stack, which is the last in our linked list */
        UInt32 nodecount;                    /* number of nodes on our stack */

        public :

        /*
         *      Stack manipulation
         */
        void Push( void *data );             /* push node to bottom of stack, prefixing it - Together with Pop() FIFO */
        void Pop( void **data );             /* pop node from bottom of stack, removing the first node - Together with Push() FIFO  */
        void Get_Top( void **data );         /* Get top node on top of stack, removing it from the top of the stack - Together with Push() FILO  */
        void Set_Top( void *data );          /* place new node on top of stack */
        void Clear_Stack( void );            /* clear the stack from all nodes */

        /*
         *      Stack information
         */
        UInt32 Count( void ) const;          /* returns the total number of nodes on the stack */

        /*
         *      Con & Destructors
         */
        CStack( void );                      /* default constructor, creates an empty stack */
        CStack( const CStack &stack );       /* copy constructor, copy's nodes and there links to this stack */
        ~CStack();                           /* destructor, clears the list uppon destruction */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTACK_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-8-2003 :
       - Added this section.
       - Removed friend with itself relationship

//-------------------------------------------------------------------------*/
