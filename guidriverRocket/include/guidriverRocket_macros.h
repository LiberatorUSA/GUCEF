/*
 *  guceMyGUIOgre: glue module for the MyGUI+Ogre GUI backend
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

#ifndef GUCE_MYGUIOGRE_MACROS_H
#define GUCE_MYGUIOGRE_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCE_MYGUIOGRE_ETYPES_H
#include "guceMyGUIOgre_ETypes.h"        /* simple types */
#define GUCE_MYGUIOGRE_ETYPES_H
#endif /* GUCE_MYGUIOGRE_ETYPES_H ? */

#ifndef GUCE_MYGUIOGRE_CONFIG_H
#include "guceMyGUIOgre_config.h"        /* Module build configuration */
#define GUCE_MYGUIOGRE_CONFIG_H
#endif /* GUCE_MYGUIOGRE_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef GUCE_EXPORTSPEC
#ifdef GUCE_MYGUIOGRE_BUILD_MODULE
  #define GUCE_EXPORTSPEC GUCE_EXPORT
#else
  #define GUCE_EXPORTSPEC GUCE_IMPORT
#endif /* GUCE_MYGUIOGRE_BUILD_MODULE ? */

#undef GUCE_MYGUIOGRE_EXPORT_CPP
#ifdef GUCE_MYGUIOGRE_EXPORT_CPP_CODE
  #define GUCE_MYGUIOGRE_EXPORT_CPP GUCE_EXPORTSPEC
#else
  #define GUCE_MYGUIOGRE_EXPORT_CPP
#endif /* GUCEF_GUI_EXPORT_CPP_CODE */

#undef GUCE_MYGUIOGRE_EXPORT_C
#ifdef GUCE_MYGUIOGRE_EXPORT_C_CODE
  #define GUCE_MYGUIOGRE_EXPORT_C GUCE_EXPORTSPEC
#else
  #define GUCE_MYGUIOGRE_EXPORT_C
#endif /* GUCE_MYGUIOGRE_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCE_MYGUIOGRE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
