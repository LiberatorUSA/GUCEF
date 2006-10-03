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
 *      Target platform selection switches
 *      By default autodetects
 */
// #define GUCEF_LINUX_BUILD
#define GUCEF_MSWIN_BUILD

/*-------------------------------------------------------------------------*/

/*
 *      Switches for setting of the calling convention used
 *      Defaults to CALLSPEC_C
 */
#define GUCEF_USE_CALLSPEC_C
// #define GUCEF_USE_CALLSPEC_STD
// #define GUCEF_USE_CALLSPEC_PASCAL
// #define GUCEF_USE_CALLSPEC_FAST

#define GUCEF_USE_PLUGIN_CALLSPEC_C
// #define GUCEF_USE_PLUGIN_CALLSPEC_STD
// #define GUCEF_USE_PLUGIN_CALLSPEC_PASCAL
// #define GUCEF_USE_PLUGIN_CALLSPEC_FAST

/*-------------------------------------------------------------------------*/

/*
 *      Switches for manually setting the bit target
 *      Default is 32bit, may be able to autodetect
 */
#define GUCEF_32BIT
// #define GUCEF_64BIT

/*-------------------------------------------------------------------------*/

/*
 *      Debug mode
 *      Autodetects from the _DEBUG define
 */
// #define GUCEF_DEBUG_MODE
// #define GUCEF_DEBUG_VERBOSE

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 30-09-2006 :
       - Logan: Modified to remove GUCEF_MANUAL_* defines, cleaned up
                Lets keep this simple :)
- 29-09-2006 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/
