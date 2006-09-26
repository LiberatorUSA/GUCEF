/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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
 *  This header contains build switches relative to the entire GUCEF platform
 */

#ifndef GUCEF_CONFIG_H
#define GUCEF_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Compile for platform...
 *      The O/S can be autodetected if you wish instead of defining it here.
 */
#undef GUCEF_MANUAL_OS_DEFINE         /* do not change this line */ 
/* #define GUCEF_MANUAL_OS_DEFINE */

/*
 *      Target platform selection switches.
 *      Only has an effect if MANUAL_OS_DEFINE is defined.
 */
#undef GUCEF_LINUX_BUILD         /* do not change this line */ 
#undef GUCEF_MSWIN_BUILD         /* do not change this line */  
/* #define GUCEF_LINUX_BUILD */
#define GUCEF_MSWIN_BUILD 

/*-------------------------------------------------------------------------*/

/*
 *      Do you wish to manually define wheter you are building or using a
 *      dynamicly linked library ?
 *      Manual is not recommended unless the macros do not fully support your
 *      compiler.
 */
#undef GUCEF_MANUAL_EXPORT_DEFINE      /* do not change this line */  
/* #define GUCEF_MANUAL_EXPORT_DEFINE */

/*
 *      Dynamic or static linking ?
 *      and build the dynamic linked library or use it ?
 */
#undef GUCEF_BUILD_DLL      /* do not change this line */  
#undef GUCEF_USE_DLL        /* do not change this line */   
/* #define GUCEF_BUILD_DLL */
/* #define GUCEF_USE_DLL */

/*-------------------------------------------------------------------------*/

/*
 *      Switches for setting the calling convention used by the gucefCORE
 *      module. Please enable only 1, no more, no less.
 */
#undef GUCEF_USE_CALLSPEC_C                /* do not change this line */
#undef GUCEF_USE_CALLSPEC_STD              /* do not change this line */ 
#undef GUCEF_USE_CALLSPEC_PASCAL           /* do not change this line */ 
#undef GUCEF_USE_CALLSPEC_FAST             /* do not change this line */
#undef GUCEF_USE_PLUGIN_CALLSPEC_C         /* do not change this line */
#undef GUCEF_USE_PLUGIN_CALLSPEC_STD       /* do not change this line */ 
#undef GUCEF_USE_PLUGIN_CALLSPEC_PASCAL    /* do not change this line */ 
#undef GUCEF_USE_PLUGIN_CALLSPEC_FAST      /* do not change this line */
  
#define GUCEF_USE_CALLSPEC_C 
/* #define GUCEF_USE_CALLSPEC_STD */
/* #define GUCEF_USE_CALLSPEC_PASCAL */
/* #define GUCEF_USE_CALLSPEC_FAST */

#define GUCEF_USE_PLUGIN_CALLSPEC_C 
/* #define GUCEF_USE_PLUGIN_CALLSPEC_STD */
/* #define GUCEF_USE_PLUGIN_CALLSPEC_PASCAL */
/* #define GUCEF_USE_PLUGIN_CALLSPEC_FAST */

/*-------------------------------------------------------------------------*/

/*
 *      Wheter or not to use the build swiches here to specify manually what
 *      the bit target is.
 */
#undef GUCEF_MANUAL_BITTARGET_DEFINE      /* do not change this line */  
#define GUCEF_MANUAL_BITTARGET_DEFINE

/*
 *      Switches for manually setting the bit target to either 32 or 64 bit
 *      These switches have no effect if MANUAL_BITTARGET_DEFINE is not defined.
 */
#undef GUCEF_32BIT      /* do not change this line */
#undef GUCEF_64BIT      /* do not change this line */ 
#define GUCEF_32BIT
/* #define GUCEF_64BIT */

/*-------------------------------------------------------------------------*/

/*
 *      Wheter or not to determine if we are building in debug mode automaticly.
 *      If you enable this manual setting of debug mode then the next setting
 *      is the one that determines wheter we are in debug mode or not.
 */
#undef GUCEF_MANUAL_DEBUG_MODE_DEFINE      /* do not change this line */  
/* #define GUCEF_MANUAL_DEBUG_MODE_DEFINE */

/*
 *      Wheter to build in debug mode, meaning additional debug info will be
 *      generated run-time. This will howerver slow down program excecution and
 *      use more memory.
 */
#undef GUCEF_DEBUG_MODE      /* do not change this line */ 
/* #define GUCEF_DEBUG_MODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-09-2006 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/

