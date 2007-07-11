/*
 *  gucefIMAGE: GUCEF module providing image utilities
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

#ifndef GUCEF_IMAGE_MACROS_H
#define GUCEF_IMAGE_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_BASICMACROS_H
#include "macros.h"             /* generic often used macros */
#define GUCEF_CORE_BASICMACROS_H
#endif /* GUCEF_CORE_BASICMACROS_H ? */

#ifndef GUCEF_IMAGE_ETYPES_H
#include "gucefIMAGE_ETypes.h"      /* simple types */
#define GUCEF_IMAGE_ETYPES_H
#endif /* GUCEF_IMAGE_ETYPES_H ? */

#ifndef GUCEF_IMAGE_CONFIG_H
#include "gucefIMAGE_config.h"      /* Module build configuration */
#define GUCEF_IMAGE_CONFIG_H
#endif /* GUCEF_IMAGE_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef GUCEF_EXPORTSPEC
#ifdef GUCEFIMAGE_BUILD_MODULE
  #define GUCEF_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUCEF_EXPORTSPEC GUCEF_IMPORT
#endif /* BUILD_DLL */

#undef GUCEF_IMAGE_EXPORT_CPP
#ifdef GUCEF_IMAGE_EXPORT_CPP_CODE
  #define GUCEF_IMAGE_EXPORT_CPP GUCEF_EXPORTSPEC
#else
  #define GUCEF_IMAGE_EXPORT_CPP
#endif /* GUCEF_IMAGE_EXPORT_CPP_CODE */

#undef GUCEF_IMAGE_EXPORT_C
#ifdef GUCEF_IMAGE_EXPORT_C_CODE
  #define GUCEF_IMAGE_EXPORT_C GUCEF_EXPORTSPEC
#else
  #define GUCEF_IMAGE_EXPORT_C
#endif /* GUCEF_IMAGE_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_MACROS_H ? */




