/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef GUCEF_INPUT_CONFIG_H
#define GUCEF_INPUT_CONFIG_H

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
 *      Do you wish to manually define wheter you are building or using a
 *      dynamicly linked library ?
 *      Manual is not recommended unless the macros do not fully support your
 *      compiler.
 */
#undef GUCEF_INPUT_MANUAL_EXPORT_DEFINE      /* do not change this line */  
#define GUCEF_INPUT_MANUAL_EXPORT_DEFINE

/*-------------------------------------------------------------------------*/

/*
 *      Dynamic or static linking ?
 *      and build the dynamic linked library or use it ?
 */
#undef GUCEF_INPUT_BUILD_DLL      /* do not change this line */  
#undef GUCEF_INPUT_USE_DLL        /* do not change this line */   
/* #define GUCEF_INPUT_BUILD_DLL */

/*-------------------------------------------------------------------------*/

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */
#undef GUCEF_INPUT_PUBLIC_CPP_CODE        /* do not change this line */ 
#undef GUCEF_INPUT_PUBLIC_C_CODE          /* do not change this line */  
#define GUCEF_INPUT_PUBLIC_CPP_CODE /* do you want to enable the C++ exports ? */
#define GUCEF_INPUT_PUBLIC_C_CODE   /* do you want to enable the C exports ? */

/*-------------------------------------------------------------------------*/

/*
 *      Wheter to build in debug mode, meaning additional debug info will be
 *      generated run-time. This will howerver slow down program excecution and
 *      use more memory.
 */
#undef GUCEF_INPUT_DEBUG_MODE      /* do not change this line */ 
/* #define GUCEF_INPUT_DEBUG_MODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_INPUT_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
