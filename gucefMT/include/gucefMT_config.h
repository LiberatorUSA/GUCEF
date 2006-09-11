/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_MT_CONFIG_H
#define GUCEF_MT_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Compile for platform...
 *      The O/S can be autodetected if you wish instead of defining it here.
 */
#undef GUCEFMT_MANUAL_OS_DEFINE         /* do not change this line */
/* #define GUCEFMT_MANUAL_OS_DEFINE */       /* Do you wish to specify the O/S yourself ? */

/*
 *      Target platform selection switches.
 *      Only has an effect if MANUAL_OS_DEFINE is defined.
 */
#undef GUCEFMT_LINUX_BUILD     /* do not change this line */ 
#undef GUCEFMT_MSWIN_BUILD     /* do not change this line */
//#define GUCEFMT_LINUX_BUILD           /* are we building for Linux ? */
#ifndef GUCEFMT_LINUX_BUILD     /* don't change this line: can't compile for 2 platforms at once */
#define GUCEFMT_MSWIN_BUILD     /* are we building for 32 bit Windows ? */
#define WIN32_LEAN_AND_MEAN     /* don't change this line: trim fat from windoze */
#define WIN32_EXTRA_LEAN        /* don't change this line: trim additional tub of lard from windoze */
#endif /* GUCEFMT_LINUX_BUILD ? */      /* don't change this line */

/*
 *      Do you wish to manually define wheter you are building or using a
 *      dynamicly linked library ?
 *      Manual is not recommended unless the macros do not fully support your
 *      compiler.
 */
#undef GUCEFMT_MANUAL_EXPORT_DEFINE     /* do not change this line */ 
/* #define GUCEFMT_MANUAL_EXPORT_DEFINE */

/*
 *      Dynamic or static linking ?
 *      and build the dynamic linked library or use it ?
 */
#ifdef GUCEFMT_MANUAL_EXPORT_DEFINE          /* don't change this line */
#undef GUCEFMT_BUILD_DLL                     /* don't change this line */
#undef GUCEFMT_USE_DLL                       /* don't change this line */

/*#define GUCEFMT_BUILD_DLL */           /* are we staticly linking ? */
//#define GUCEFMT_USE_DLL            /* use instead of build ?    */

#endif /* GUCEFMT_MANUAL_EXPORT_DEFINE ? */  /* don't change this line */

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */
#undef GUCEFMT_EXPORT_CPP_CODE     /* do not change this line */ 
#undef GUCEFMT_EXPORT_C_CODE       /* do not change this line */ 
#define GUCEFMT_EXPORT_CPP_CODE /* do you want to enable the C++ exports ? */
#define GUCEFMT_EXPORT_C_CODE   /* do you want to enable the C exports ? */

/*
 *      Wheter or not to determine if we are building in debug mode automaticly.
 *      If you enable this manual setting of debug mode then the next setting
 *      is the one that determines wheter we are in debug mode or not.
 */
#undef GUCEFMT_MANUAL_DEBUG_MODE_DEFINE     /* do not change this line */  
/* #define GUCEFMT_MANUAL_DEBUG_MODE_DEFINE */

/*
 *      Wheter to build in debug mode, meaning additional debug info will be
 *      generated run-time. This will howerver slow down program excecution and
 *      use more memory.
 */
#undef GUCEFMT_MANUAL_DEBUG_MODE_DEFINE     /* do not change this line */   
/* #define GUCEFMT_MANUAL_DEBUG_MODE_DEFINE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 25-02-2006 :
       - Added GUCEFMT_ prefix to all defines
- 16-10-2004 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
 
