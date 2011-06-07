/*
 *  gucefPRODMAN: Product management module
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_PRODUCTMAN_MACROS_H
#define GUCEF_PRODUCTMAN_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_PRODMAN_CONFIG_H
#include "gucefPRODMAN_config.h"
#define GUCEF_PRODMAN_CONFIG_H
#endif /* GUCEF_PRODMAN_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#undef GUCEF_PRODMAN_EXPORT
#ifdef GUCEF_PRODMAN_BUILD_MODULE
  #define GUCEF_PRODMAN_EXPORT GUCEF_EXPORT
#else
  #define GUCEF_PRODMAN_EXPORT GUCEF_IMPORT
#endif /* GUCEF_PRODMAN_BUILD_MODULE ? */

#undef GUCEF_PRODMAN_PUBLIC_CPP
#ifdef GUCEF_PRODMAN_EXPORT_CPP_CODE
  #define GUCEF_PRODMAN_PUBLIC_CPP GUCEF_PRODMAN_EXPORT  
#else
  #define GUCEF_PRODMAN_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_PRODMAN_EXPORT_CPP_CODE */

#undef GUCEF_PRODMAN_PUBLIC_C 
#ifdef GUCEF_PRODMAN_EXPORT_C_CODE
  #define GUCEF_PRODMAN_PUBLIC_C GUCEF_PRODMAN_EXPORT
#else
  #define GUCEF_PRODMAN_PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEF_PRODMAN_EXPORT_C_CODE */

#define GUCEF_PRODMAN_PRIVATE_C    GUCEF_HIDDEN
#define GUCEF_PRODMAN_PRIVATE_CPP  GUCEF_HIDDEN

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PRODUCTMAN_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
