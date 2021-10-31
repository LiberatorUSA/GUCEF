/*
 *  gucef common header: provides header based platform wide facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
/* #define GUCEF_LINUX_BUILD */
/* #define GUCEF_MSWIN_BUILD */

/*-------------------------------------------------------------------------*/

/*
 *      Switches for setting of the calling convention used
 *      Defaults to CALLSPEC_C
 */
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
 *      Switches for manually setting the bit target
 *      Default is 32bit, may be able to autodetect
 */
/* #define GUCEF_32BIT */
/* #define GUCEF_64BIT */

/*-------------------------------------------------------------------------*/

/*
 *      Debug mode
 *      Autodetects from the _DEBUG define
 */
/* #define GUCEF_DEBUG_MODE */
/* #define GUCEF_DEBUG_VERBOSE */

/*-------------------------------------------------------------------------*/

/*
 *  Switches to enable/disable callstack tracking
 */
/* #define GUCEF_RELEASE_CALLSTACK_TRACKING */
/* #define GUCEF_DEBUG_CALLSTACK_TRACKING */

/*-------------------------------------------------------------------------*/

/*
 *  Switches to enable/disable platform provided memory checking and leak detection
 */
/* #define GUCEF_USE_MEMORY_LEAK_CHECKER */
/* #define GUCEF_USE_PLATFORM_MEMORY_LEAK_CHECKER */
/* #define GUCEF_MEMCHECK_OLEAPI */

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
