/*
 *  vfspluginVP: Generic GUCEF VFS plugin for "Violation Pack" archives
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

#ifndef GUCEF_LOGSERVICECLIENTPLUGIN_MACROS_H
#define GUCEF_LOGSERVICECLIENTPLUGIN_MACROS_H

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
#include "macros.h"                /* generic often used macros */
#define GUCEF_CORE_BASICMACROS_H
#endif /* GUCEF_CORE_BASICMACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"      /* simple types */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"      /* CORE Module build configuration */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_LOGSERVICECLIENTPLUGIN_CONFIG_H
#include "GucefLogServiceClientPlugin_config.h"
#define GUCEF_LOGSERVICECLIENTPLUGIN_CONFIG_H
#endif /* GUCEF_LOGSERVICECLIENTPLUGIN_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#undef GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT
#ifdef GUCEF_LOGSERVICECLIENTPLUGIN_BUILD_MODULE
  #define GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT GUCEF_EXPORT
#else
  #define GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT GUCEF_IMPORT
#endif /* GUCEF_LOGSERVICECLIENTPLUGIN_BUILD_MODULE ? */

#undef GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_CPP
#ifdef GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_CPP_CODE
  #define GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_CPP GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT  
#else
  #define GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_CPP
#endif /* GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_CPP_CODE */

#undef GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_C 
#ifdef GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_C_CODE
  #define GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_C GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT
#else
  #define GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_C
#endif /* GUCEF_LOGSERVICECLIENTPLUGIN_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_LOGSERVICECLIENTPLUGIN_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
       - Initial version of this file.

---------------------------------------------------------------------------*/
