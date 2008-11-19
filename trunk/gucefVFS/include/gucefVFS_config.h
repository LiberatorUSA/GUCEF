/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef GUCEF_VFS_CONFIG_H
#define GUCEF_VFS_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Compile for platform...
 *      The O/S can be autodetected if you wish instead of defining it here.
 */
#undef MANUAL_OS_DEFINE         /* do not change this line */ 
#define MANUAL_OS_DEFINE

/*-------------------------------------------------------------------------*/        

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
#undef MANUAL_EXPORT_DEFINE      /* do not change this line */  
#define MANUAL_EXPORT_DEFINE

/*-------------------------------------------------------------------------*/

/*
 *      Dynamic or static linking ?
 *      and build the dynamic linked library or use it ?
 */
#undef BUILD_DLL      /* do not change this line */  
#undef USE_DLL        /* do not change this line */   
#define BUILD_DLL                 
/*#define USE_DLL */

/*-------------------------------------------------------------------------*/

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */
#undef EXPORT_CPP_CODE        /* do not change this line */ 
#undef EXPORT_C_CODE          /* do not change this line */  
#define EXPORT_CPP_CODE /* do you want to enable the C++ exports ? */
#define EXPORT_C_CODE   /* do you want to enable the C exports ? */

/*-------------------------------------------------------------------------*/

/*
 *      Switches for setting the calling convention used by the gucefCORE
 *      module. Please enable only 1, no more, no less.
 */
#undef USE_CALLSPEC_C                /* do not change this line */
#undef USE_CALLSPEC_STD              /* do not change this line */ 
#undef USE_CALLSPEC_PASCAL           /* do not change this line */ 
#undef USE_CALLSPEC_FAST             /* do not change this line */
#undef USE_PLUGIN_CALLSPEC_C         /* do not change this line */
#undef USE_PLUGIN_CALLSPEC_STD       /* do not change this line */ 
#undef USE_PLUGIN_CALLSPEC_PASCAL    /* do not change this line */ 
#undef USE_PLUGIN_CALLSPEC_FAST      /* do not change this line */
  
#define USE_CALLSPEC_C 
/* #define USE_CALLSPEC_STD */
/* #define USE_CALLSPEC_PASCAL */
/* #define USE_CALLSPEC_FAST */
/* #define USE_PLUGIN_CALLSPEC_C */
#define USE_PLUGIN_CALLSPEC_STD
/* #define USE_PLUGIN_CALLSPEC_PASCAL */
/* #define USE_PLUGIN_CALLSPEC_FAST */

/*-------------------------------------------------------------------------*/

/*
 *      Wheter or not to use the build swiches here to specify manually what
 *      the bit target is.
 */
#undef MANUAL_BITTARGET_DEFINE      /* do not change this line */  
#define MANUAL_BITTARGET_DEFINE

/*-------------------------------------------------------------------------*/

/*
 *      Switches for manually setting the bit target to either 32 or 64 bit
 *      These switches have no effect if MANUAL_BITTARGET_DEFINE is not defined.
 */
#undef GUCEF32BIT      /* do not change this line */
#undef GUCEF64BIT      /* do not change this line */ 
#define GUCEF32BIT
/* #define GUCEF64BIT */

/*-------------------------------------------------------------------------*/

/*
 *      Wheter or not to determine if we are building in debug mode automaticly.
 *      If you enable this manual setting of debug mode then the next setting
 *      is the one that determines wheter we are in debug mode or not.
 */
#undef MANUAL_DEBUG_MODE_DEFINE      /* do not change this line */  
/* #define MANUAL_DEBUG_MODE_DEFINE */

/*-------------------------------------------------------------------------*/

/*
 *      Wheter to build in debug mode, meaning additional debug info will be
 *      generated run-time. This will howerver slow down program excecution and
 *      use more memory.
 */
#undef DEBUG_MODE      /* do not change this line */ 
/* #define DEBUG_MODE */

/*-------------------------------------------------------------------------*/

/*
 *      If enabled extra information like line and file information is added to
 *      the DEBUGOUTPUT() macro.
 */
#undef ADD_EXTRA_INFO_TO_DEBUGOUTPUT    /* do not change this line */ 
/* #define ADD_EXTRA_INFO_TO_DEBUGOUTPUT */

/*-------------------------------------------------------------------------*/

/*
 *      Switch for removing/adding the GUCEF namespace.
 */
#undef USE_GUCEF_NAMESPACE      /* do not change this line */  
#define USE_GUCEF_NAMESPACE

/*-------------------------------------------------------------------------*/

/*
 *      Switch for removing/adding the CORE namespace.
 */
#undef USE_VFS_NAMESPACE      /* do not change this line */   
#define USE_VFS_NAMESPACE

/*-------------------------------------------------------------------------*/

/*
 *      Switch that activates or deactivates the module memory manager.
 *      This is a debugging tool that will allow you to search for
 *      memory leaks and bounds violations. 
 */
#undef ACTIVATE_MEMORY_MANAGER  /* do not change this line */
//#define ACTIVATE_MEMORY_MANAGER

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_VFS_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/
