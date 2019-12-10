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
class GUCEF_CORE_PUBLIC_CPP CStack
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
