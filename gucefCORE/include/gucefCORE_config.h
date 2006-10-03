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

#ifndef GUCEF_CORE_CONFIG_H
#define GUCEF_CORE_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"      /* GUCEF platform build configuration */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  Are we building the module itself or are we simply linking it ?
 *  If possible you can also set this define in your project instead of altering the switch below    
 */           
/* #define GUCEFCORE_BUILD_MODULE */

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */ 
#define GUCEFCORE_EXPORT_CPP_CODE /* do you want to enable the C++ exports ? */
#define GUCEFCORE_EXPORT_C_CODE   /* do you want to enable the C exports ? */

/*
 *      Wheter or not to use the build swiches here to specify manually what
 *      the bit target is.
 */
#undef MANUAL_BITTARGET_DEFINE      /* do not change this line */  
#define MANUAL_BITTARGET_DEFINE

/*
 *      Switches for manually setting the bit target to either 32 or 64 bit
 *      These switches have no effect if MANUAL_BITTARGET_DEFINE is not defined.
 */
#undef GUCEF32BIT      /* do not change this line */
#undef GUCEF64BIT      /* do not change this line */ 
#define GUCEF32BIT
/* #define GUCEF64BIT */

/*
 *      Wheter or not to determine if we are building in debug mode automaticly.
 *      If you enable this manual setting of debug mode then the next setting
 *      is the one that determines wheter we are in debug mode or not.
 */
#undef MANUAL_DEBUG_MODE_DEFINE      /* do not change this line */  
/* #define MANUAL_DEBUG_MODE_DEFINE */

/*
 *      Wheter to build in debug mode, meaning additional debug info will be
 *      generated run-time. This will howerver slow down program excecution and
 *      use more memory.
 */
#undef DEBUG_MODE      /* do not change this line */ 
/* #define DEBUG_MODE */

/*
 *      If enabled extra information like line and file information is added to
 *      the DEBUGOUTPUT() macro.
 */
#undef ADD_EXTRA_INFO_TO_DEBUGOUTPUT    /* do not change this line */ 
/* #define ADD_EXTRA_INFO_TO_DEBUGOUTPUT */

/*
 *      Switch that adds or removes the memory manager code from
 *      the module. If no modules use the memory manager then 
 *      comment this switch to get rid of the then unused code.
 */
#undef ADD_MEMORY_MANAGER     /* do not change this line */
//#define ADD_MEMORY_MANAGER

/*
 *      Switch that activates or deactivates the module memory manager.
 *      This is a debugging tool that will allow you to search for
 *      memory leaks and bounds violations. 
 */
#undef ACTIVATE_MEMORY_MANAGER  /* do not change this line */
//#define ACTIVATE_MEMORY_MANAGER

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/
 
