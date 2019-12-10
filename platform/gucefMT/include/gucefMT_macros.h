/*
 *  gucefMT: GUCEF module providing multithreading solutions
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

#ifndef GUCEF_MT_MACROS_H
#define GUCEF_MT_MACROS_H

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
#include "gucef_macros.h"          /* GUCEF platform wide macros */
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_MT_CONFIG_H
#include "gucefMT_config.h"        /* Module build configuration */
#define GUCEF_MT_CONFIG_H
#endif /* GUCEF_MT_CONFIG_H ? */

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"        /* type definitions of simple types */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

#ifndef GUCEF_MT_BASICMACROS_H
#include "gucefMT_basicMacros.h"   /* generic often used macros */
#define GUCEF_MT_BASICMACROS_H
#endif /* GUCEF_MT_BASICMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines:
 *      GUCEF_EXPORT_C_CODE
 *      GUCEF_EXPORT_CPP_CODE
 */
#if defined( GUCEF_MT_BUILD_MODULE )
  #define GUCEF_MT_EXPORTSPEC GUCEF_EXPORT
#elif defined( GUCEF_MT_EMBED_CODE )
  #define GUCEF_MT_EXPORTSPEC
#else
  #define GUCEF_MT_EXPORTSPEC GUCEF_IMPORT
#endif /* GUCEF_MT_BUILD_MODULE */

/*-------------------------------------------------------------------------*/

/*
 *  Seperate switch for C++ exports
 */
#undef GUCEF_MT_PUBLIC_CPP
#ifdef GUCEF_MT_PUBLIC_CPP_CODE
  #define GUCEF_MT_PUBLIC_CPP GUCEF_MT_EXPORTSPEC
#else
  #define GUCEF_MT_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_MT_PUBLIC_CPP_CODE */

/*-------------------------------------------------------------------------*/

/*
 *  Seperate switch for C exports
 */
#undef GUCEF_MT_PUBLIC_C
#ifdef GUCEF_MT_PUBLIC_C_CODE
  #define GUCEF_MT_PUBLIC_C GUCEF_MT_EXPORTSPEC
#else
  #define GUCEF_MT_PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEFMT_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-09-2006 :
       - Dinand: reduced macros in this file.

-----------------------------------------------------------------------------*/
