/*
 *  guidriverAndroidGLES: module implementing GLES based window management for Android
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

#ifndef GUIDRIVERANDROIDGLES_MACROS_H
#define GUIDRIVERANDROIDGLES_MACROS_H

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

#ifndef GUIDRIVERANDROIDGLES_CONFIG_H
#include "guidriverAndroidGLES_config.h"
#define GUIDRIVERANDROIDGLES_CONFIG_H
#endif /* GUIDRIVERANDROIDGLES_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */

#undef GUIDRIVERANDROIDGLES_EXPORTSPEC
#ifdef GUIDRIVERANDROIDGLES_BUILD_MODULE
  #define GUIDRIVERANDROIDGLES_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUIDRIVERANDROIDGLES_EXPORTSPEC GUCEF_IMPORT
#endif /* GUCEF_GUI_BUILD_MODULE ? */

#undef GUIDRIVERANDROIDGLES_PUBLIC_CPP
#ifdef GUIDRIVERANDROIDGLES_PUBLIC_CPP_CODE
  #define GUIDRIVERANDROIDGLES_PUBLIC_CPP GUIDRIVERANDROIDGLES_EXPORTSPEC
#else
  #define GUIDRIVERANDROIDGLES_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUIDRIVERANDROIDGLES_EXPORT_CPP_CODE */

#define GUIDRIVERANDROIDGLES_PRIVATE_CPP GUCEF_HIDDEN

#undef GUIDRIVERANDROIDGLES_PUBLIC_C
#ifdef GUIDRIVERANDROIDGLES_PUBLIC_C_CODE
  #define GUIDRIVERANDROIDGLES_PUBLIC_C GUIDRIVERANDROIDGLES_EXPORTSPEC
#else
  #define GUIDRIVERANDROIDGLES_PUBLIC_C GUCEF_HIDDEN
#endif /* GUIDRIVERANDROIDGLES_PUBLIC_C_CODE */

#define GUIDRIVERANDROIDGLES_PRIVATE_C GUCEF_HIDDEN

/*-------------------------------------------------------------------------*/

#endif /* GUIDRIVERANDROIDGLES_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

---------------------------------------------------------------------------*/
