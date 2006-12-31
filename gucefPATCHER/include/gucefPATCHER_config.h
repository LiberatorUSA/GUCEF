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

#ifndef GUCEF_PATCHER_CONFIG_H
#define GUCEF_PATCHER_CONFIG_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */
#undef GUCEFPATCHER_EXPORT_CPP_CODE        /* do not change this line */ 
#undef GUCEFPATCHER_EXPORT_C_CODE          /* do not change this line */  
#define GUCEFPATCHER_EXPORT_CPP_CODE /* do you want to enable the C++ exports ? */
#define GUCEFPATCHER_EXPORT_C_CODE   /* do you want to enable the C exports ? */

/*-------------------------------------------------------------------------*/

/*
 *      Wheter or not to determine if we are building in debug mode automaticly.
 *      If you enable this manual setting of debug mode then the next setting
 *      is the one that determines wheter we are in debug mode or not.
 */
#undef GUCEFPATCHER_MANUAL_DEBUG_MODE_DEFINE      /* do not change this line */  
/* #define GUCEFPATCHER_MANUAL_DEBUG_MODE_DEFINE */

/*-------------------------------------------------------------------------*/

/*
 *      Wheter to build in debug mode, meaning additional debug info will be
 *      generated run-time. This will howerver slow down program excecution and
 *      use more memory.
 */
#undef GUCEFPATCHER_DEBUG_MODE      /* do not change this line */ 
/* #define DEBUG_MODE */

/*-------------------------------------------------------------------------*/

/*
 *      If enabled extra information like line and file information is added to
 *      the DEBUGOUTPUT() macro.
 */
#undef GUCEFPATCHER_ADD_EXTRA_INFO_TO_DEBUGOUTPUT    /* do not change this line */ 
/* #define ADD_EXTRA_INFO_TO_DEBUGOUTPUT */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PATCHER_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/