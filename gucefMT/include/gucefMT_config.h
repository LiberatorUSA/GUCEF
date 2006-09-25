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
 *      Target platform selection.
 *      Defines:
 *      GUCEF_LINUX_BUILD
 *      GUCEF_MSWIN_BUILD
 */
#if !(defined(GUCEF_LINUX_BUILD) | defined(GUCEF_MSWIN_BUILD))
    #error You need to define a build/target OS

#elif defined(GUCEF_LINUX_BUILD) & defined(GUCEF_MSWIN_BUILD)
    #error You have defined more than 1 build/target OS's
#endif

/*
 *      Building or using the library?
 *      Defines:
 *      GUCEF_BUILD_DLL
 *      GUCEF_USE_DLL
 */
#if !(defined(GUCEFMT_BUILD_DLL) | defined(GUCEFMT_USE_DLL))
    #error You need to define whether we are building or using the library

#elif defined(GUCEFMT_BUILD_DLL) & defined(GUCEFMT_USE_DLL)
    #error You have defined more than 1 build/target OS's
#endif /* GUCEFMT_LINUX_BUILD ? */  /* don't change this line */

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines:
 *      GUCEF_EXPORT_C_CODE
 *      GUCEF_EXPORT_CPP_CODE
 */
#ifdef GUCEF_BUILD_DLL
#if !(defined(GUCEF_EXPORT_C_CODE) | defined(GUCEF_EXPORT_CPP_CODE))
    #warning It would be best to define these when building a dll, but as a default we will define both
    #define GUCEF_EXPORT_C_CODE
    #define GUCEF_EXPORT_CPP_CODE
#endif
#endif /* GUCEF_BUILD_DLL */

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

