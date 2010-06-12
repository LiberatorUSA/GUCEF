/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
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
/* #define GUCEF_CORE_BUILD_MODULE */

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */
#define GUCEF_CORE_PUBLIC_CPP_CODE /* do you want to enable the C++ exports ? */
#define GUCEF_CORE_PUBLIC_C_CODE   /* do you want to enable the C exports ? */

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
#undef GUCEF_CORE_DEBUG_MODE      /* do not change this line */
/* #define GUCEF_CORE_DEBUG_MODE */

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

/*
 *  Switch for enabling/disabling the logging of exceptions that are thrown
 *  using the GUCEF exception macros
 */
#undef GUCEF_LOG_EXCEPTIONS
#define GUCEF_LOG_EXCEPTIONS

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

