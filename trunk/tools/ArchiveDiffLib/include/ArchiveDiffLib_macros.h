/*
 *  ArchiveDiffLib: library with some archive diff util functions
 *
 *  Copyright (C) 2002 - 2010.  Dinand Vanvelzen
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

#ifndef ARCHIVEDIFFLIB_MACROS_H
#define ARCHIVEDIFFLIB_MACROS_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MACROS_H
#include "gucef_macros.h"
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#include "ArchiveDiffLib_config.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/


/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */

#undef ARCHIVEDIFFLIB_EXPORTSPEC
#ifdef ARCHIVEDIFFLIB_BUILD_MODULE
  #define ARCHIVEDIFFLIB_EXPORTSPEC GUCEF_EXPORT
#else
  #define ARCHIVEDIFFLIB_EXPORTSPEC GUCEF_IMPORT
#endif /* ARCHIVEDIFFLIB_BUILD_MODULE ? */

#undef ARCHIVEDIFFLIB_PUBLIC_CPP
#ifdef ARCHIVEDIFFLIB_PUBLIC_CPP_CODE
  #define ARCHIVEDIFFLIB_PUBLIC_CPP ARCHIVEDIFFLIB_EXPORTSPEC
#else
  #define ARCHIVEDIFFLIB_PUBLIC_CPP GUCEF_HIDDEN
#endif /* ARCHIVEDIFFLIB_EXPORT_CPP_CODE */

#define ARCHIVEDIFFLIB_PRIVATE_CPP GUCEF_HIDDEN

#undef ARCHIVEDIFFLIB_PUBLIC_C
#ifdef ARCHIVEDIFFLIB_PUBLIC_C_CODE
  #define ARCHIVEDIFFLIB_PUBLIC_C ARCHIVEDIFFLIB_EXPORTSPEC
#else
  #define ARCHIVEDIFFLIB_PUBLIC_C GUCEF_HIDDEN
#endif /* ARCHIVEDIFFLIB_PUBLIC_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* ARCHIVEDIFFLIB_MACROS_H ? */
