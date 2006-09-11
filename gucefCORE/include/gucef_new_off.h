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
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
