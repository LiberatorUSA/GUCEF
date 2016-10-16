/*
 *  guidriverMyGUIOgre: glue module for the MyGUI GUI backend using OpenGL
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_MYGUIOGRE_MACROS_H
#define GUCEF_MYGUIOGRE_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MYGUI_ETYPES_H
#include "guceMyGUI_ETypes.h"        /* simple types */
#define GUCEF_MYGUI_ETYPES_H
#endif /* GUCEF_MYGUI_ETYPES_H ? */

#ifndef GUCEF_MYGUIOGRE_CONFIG_H
#include "guidriverMyGUIOgre_config.h"        /* Module build configuration */
#define GUCEF_MYGUIOGRE_CONFIG_H
#endif /* GUCEF_MYGUIOGRE_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef GUCEF_MYGUIOGRE_EXPORTSPEC
#ifdef GUIDRIVERMYGUIOGRE_BUILD_MODULE
  #define GUCEF_MYGUIOGRE_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUCEF_MYGUIOGRE_EXPORTSPEC GUCEF_IMPORT
#endif /* guidriverMyGUIOgre_BUILD_MODULE ? */

#undef GUCEF_MYGUIOGRE_EXPORT_CPP
#ifdef GUCEF_MYGUIOGRE_EXPORT_CPP_CODE
  #define GUCEF_MYGUIOGRE_EXPORT_CPP GUCEF_MYGUIOGRE_EXPORTSPEC
#else
  #define GUCEF_MYGUIOGRE_EXPORT_CPP GUCEF_HIDDEN
#endif /* GUCEF_MYGUIOGRE_EXPORT_CPP_CODE */

#undef GUCEF_MYGUIOGRE_EXPORT_C
#ifdef GUCEF_MYGUIOGRE_EXPORT_C_CODE
  #define GUCEF_MYGUIOGRE_EXPORT_C GUCEF_MYGUIOGRE_EXPORTSPEC
#else
  #define GUCEF_MYGUIOGRE_EXPORT_C GUCEF_HIDDEN
#endif /* GUCEF_MYGUIOGRE_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MYGUIOGRE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
