/*
 *  gucefLOADER: GUCEF module which loads up the GUCEF platform
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

#ifndef GUCEF_LOADER_MACROS_H
#define GUCEF_LOADER_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"              /* platform macros */
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_LOADER_ETYPES_H
#include "gucefLOADER_ETypes.h"        /* simple types */
#define GUCEF_LOADER_ETYPES_H
#endif /* GUCEF_LOADER_ETYPES_H ? */

#ifndef GUCEF_LOADER_CONFIG_H
#include "gucefLOADER_config.h"        /* Module build configuration */
#define GUCEF_LOADER_CONFIG_H
#endif /* GUCEF_LOADER_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */
#undef GUCEF_LOADER_EXPORTSPEC
#ifdef GUCEF_LOADER_BUILD_MODULE
  #define GUCEF_LOADER_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUCEF_LOADER_EXPORTSPEC GUCEF_IMPORT
#endif /* GUCEF_LOADER_BUILD_MODULE ? */

#undef GUCEF_LOADER_PUBLIC_CPP
#ifdef GUCEF_LOADER_EXPORT_CPP_CODE
  #define GUCEF_LOADER_PUBLIC_CPP GUCEF_LOADER_EXPORTSPEC
#else
  #define GUCEF_LOADER_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_LOADER_EXPORT_CPP_CODE */

#undef GUCEF_LOADER_PUBLIC_C
#ifdef GUCEF_LOADER_EXPORT_C_CODE
  #define GUCEF_LOADER_PUBLIC_C GUCEF_LOADER_EXPORTSPEC
#else
  #define GUCEF_LOADER_PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEF_LOADER_EXPORT_C_CODE */

#define GUCEF_LOADER_PRIVATE_CPP GUCEF_HIDDEN
#define GUCEF_LOADER_PRIVATE_C   GUCEF_HIDDEN

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_LOADER_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
