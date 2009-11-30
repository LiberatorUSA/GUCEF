/*
 *  gucef common: GUCEF source and headers shared among all GUCEF modules
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
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
#define GUCEF_MSWIN_BUILD

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
#define GUCEF_32BIT
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
