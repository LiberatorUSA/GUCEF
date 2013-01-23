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

#ifndef GUCEF_MLF_CONFIG_H
#define GUCEF_MLF_CONFIG_H

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
/* #define GUCEF_MLF_BUILD_MODULE */

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */
#define GUCEF_CORE_PUBLIC_CPP_CODE /* do you want to enable the C++ exports ? */
#define GUCEF_CORE_PUBLIC_C_CODE   /* do you want to enable the C exports ? */

/*
 *      Switch that adds or removes the memory manager code from
 *      the module. If no modules use the memory manager then
 *      comment this switch to get rid of the then unused code.
 */
#undef ADD_MEMORY_MANAGER     /* do not change this line */
#define ADD_MEMORY_MANAGER

/*
 *      Switch that activates or deactivates the module memory manager.
 *      This is a debugging tool that will allow you to search for
 *      memory leaks and bounds violations.
 */
#undef ACTIVATE_MEMORY_MANAGER  /* do not change this line */
#define ACTIVATE_MEMORY_MANAGER

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MLF_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/

