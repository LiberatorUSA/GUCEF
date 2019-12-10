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

/*
 *      Undefine all symbols defined by the Memory Manager.  This is important when including 
 *      header files that also overwrite the new/delete operators to avoid circular definitions
 *      that will result in unexpected behavior and syntax errors from the compiler.
 *
 *      As a rule always include this file BEFORE any STL headers !!!.
 */

#undef malloc
#undef calloc
#undef realloc
#undef free

#undef CHECKMEM
#undef CHECKMEMSEG
#define CHECKMEM( addr, size )
#define CHECKMEMSEG( addr, chunk, size )

#ifdef __cplusplus
  #undef new
  #undef delete
#endif /* __cplusplus ? */

#include <stdlib.h>     /* for malloc ect */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-04-2005 :
        - Initial version of this file.

-----------------------------------------------------------------------------*/
