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

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#define GUCEF_CORE_GUCEF_ESSENTIALS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
  #include "gucefCORE_macros.h"
  #define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

#ifndef GUCEF_CORE_CALLSTACK_H 
#include "callstack.h"
#define GUCEF_CORE_CALLSTACK_H
#endif /* GUCEF_CORE_CALLSTACK_H ? */

#ifdef __cplusplus
  #ifndef GUCEF_CORE_CTRACER_H
    #include "CTracer.h"
    #define GUCEF_CORE_CTRACER_H
  #endif /* GUCEF_CORE_CTRACER_H ? */ 
#endif /* __cplusplus ? */ 

#ifdef ADD_MEMORY_MANAGER
  #ifndef GUCEF_CORE_MEMORYMANAGER_H
    #include "MemoryManager.h"    /* memory manager API */
    #define GUCEF_CORE_MEMORYMANAGER_H
  #endif /* GUCEF_CORE_MEMORYMANAGER_H ? */
  #ifndef GUCEF_NEW_ON_H
    #include "gucef_new_on.h"     /* Memory manager redirect macros & code */
    #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#else
  #include "gucef_new_off.h"
#endif /* ADD_MEMORY_MANAGER ? */    

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-04-2005 :
        - Initial version: Makes it easier to add essential header includes 
          in the future because only a change in this file will be required.

---------------------------------------------------------------------------*/
