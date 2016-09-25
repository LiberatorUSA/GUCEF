/*
 *  guidriverWin32GL: module implementing GL based window management for Win32
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#ifndef GUIDRIVEROGRE_MACROS_H
#define GUIDRIVEROGRE_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_MACROS_H
#include "gucefGUI_macros.h"
#define GUCEF_GUI_MACROS_H
#endif /* GUCEF_GUI_MACROS_H ? */

#ifndef GUIDRIVEROGRE_CONFIG_H
#include "guidriverOgre_config.h"
#define GUIDRIVEROGRE_CONFIG_H
#endif /* GUIDRIVEROGRE_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */

#undef GUIDRIVEROGRE_EXPORTSPEC
#ifdef GUIDRIVEROGRE_BUILD_MODULE
  #define GUIDRIVEROGRE_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUIDRIVEROGRE_EXPORTSPEC GUCEF_IMPORT
#endif /* GUIDRIVEROGRE_BUILD_MODULE ? */

#undef GUIDRIVEROGRE_PUBLIC_CPP
#ifdef GUIDRIVEROGRE_PUBLIC_CPP_CODE
  #define GUIDRIVEROGRE_PUBLIC_CPP GUIDRIVEROGRE_EXPORTSPEC
#else
  #define GUIDRIVEROGRE_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUIDRIVEROGRE_EXPORT_CPP_CODE */

#define GUIDRIVEROGRE_PRIVATE_CPP GUCEF_HIDDEN

#undef GUIDRIVEROGRE_PUBLIC_C
#ifdef GUIDRIVEROGRE_PUBLIC_C_CODE
  #define GUIDRIVEROGRE_PUBLIC_C GUIDRIVEROGRE_EXPORTSPEC
#else
  #define GUIDRIVEROGRE_PUBLIC_C GUCEF_HIDDEN
#endif /* GUIDRIVEROGRE_PUBLIC_C_CODE */

#define GUIDRIVEROGRE_PRIVATE_C GUCEF_HIDDEN

/*-------------------------------------------------------------------------*/

#endif /* GUIDRIVEROGRE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

---------------------------------------------------------------------------*/
