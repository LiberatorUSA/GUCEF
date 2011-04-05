/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

#ifndef GUCEF_COM_MACROS_H
#define GUCEF_COM_MACROS_H

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
#include "gucef_macros.h"
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_COM_ETYPES_H
#include "gucefCOM_ETypes.h"
#define GUCEF_COM_ETYPES_H
#endif /* GUCEF_COM_ETYPES_H ? */

#ifndef GUCEF_COM_CONFIG_H
#include "gucefCOM_config.h"      /* Module build configuration */
#define GUCEF_COM_CONFIG_H
#endif /* GUCEF_COM_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Auto detection of debug mode
 */
#ifndef MANUAL_DEBUG_MODE_DEFINE
  #if defined( _DEBUG )
    #define DEBUG_MODE
  #else
    #undef DEBUG_MODE
  #endif /* compiler DEBUG switches */
#endif /* MANUAL_DEBUG_MODE_DEFINE ? */

/*-------------------------------------------------------------------------*/

/*
 *      Macros for dynamic linking or static linking. Use the switches in the
 *      config file to control the export type.
 */

#undef GUCEF_COM_EXPORT
#ifdef GUCEF_COM_BUILD_MODULE
  #define GUCEF_COM_EXPORT GUCEF_EXPORT
#else
  #define GUCEF_COM_EXPORT GUCEF_IMPORT
#endif

#undef GUCEF_COM_EXPORT_CPP
#ifdef GUCEF_COM_EXPORT_CPP_CODE
  #define GUCEF_COM_EXPORT_CPP GUCEF_COM_EXPORT
#else
  #define GUCEF_COM_EXPORT_CPP
#endif /* GUCEF_COM_EXPORT_CPP_CODE */

#undef GUCEF_COM_EXPORT_C
#ifdef GUCEF_COM_EXPORT_C_CODE
  #define GUCEF_COM_EXPORT_C GUCEF_COM_EXPORT
#else
  #define GUCEF_COM_EXPORT_C
#endif /* GUCEF_COM_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_MACROS_H ? */
