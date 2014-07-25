/*
 *  ProjectGenerator: Tool to generate module/project files
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#ifndef GUCEF_PROJECTGEN_MACROS_H
#define GUCEF_PROJECTGEN_MACROS_H

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
#include "gucef_macros.h"         /* generic often used macros */
#define GUCEF_MACROS_H
#endif /* GUCEF_MACROS_H ? */

#ifndef GUCEF_PROJECTGEN_ETYPES_H
#include "gucefProjectGen_ETypes.h"      /* simple types */
#define GUCEF_PROJECTGEN_ETYPES_H
#endif /* GUCEF_PROJECTGEN_ETYPES_H ? */

#ifndef GUCEF_PROJECTGEN_CONFIG_H
#include "gucefProjectGen_config.h"      /* Module build configuration */
#define GUCEF_PROJECTGEN_CONFIG_H
#endif /* GUCEF_PROJECTGEN_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Auto detection of debug mode
 */
#ifndef GUCEF_PROJECTGEN_DEBUG_MODE
  #if defined( NDEBUG )
    #undef GUCEF_PROJECTGEN_DEBUG_MODE
  #else
    #if defined( _DEBUG ) || defined( __DEBUG ) || defined( __DEBUG__ )
      #define GUCEF_PROJECTGEN_DEBUG_MODE
    #else
      #undef GUCEF_PROJECTGEN_DEBUG_MODE
    #endif /* compiler DEBUG switches */
  #endif /* NDEBUG ? */
#endif /* GUCEF_INPUT_DEBUG_MODE ? */

/*-------------------------------------------------------------------------*/

/*
 *      When compiling the gucefVFS library you should define GUCEF_PROJECTGEN_BUILD_MODULE
 *      before including this macro file.
 */
#undef GUCEF_PROJECTGEN_EXPORTSPEC
#ifdef GUCEF_PROJECTGEN_BUILD_MODULE
  #define GUCEF_PROJECTGEN_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUCEF_PROJECTGEN_EXPORTSPEC GUCEF_IMPORT
#endif

/*-------------------------------------------------------------------------*/

#undef GUCEF_PROJECTGEN_PLUGIN_EXPORTSPEC
#ifdef GUCEF_PROJECTGEN_BUILD_PLUGIN_DLL
  #define GUCEF_PROJECTGEN_PLUGIN_EXPORTSPEC GUCEF_EXPORT
#else
  #define GUCEF_PROJECTGEN_PLUGIN_EXPORTSPEC GUCEF_IMPORT
#endif

/*-------------------------------------------------------------------------*/

#undef GUCEF_PROJECTGEN_PUBLIC_CPP
#ifdef GUCEF_PROJECTGEN_PUBLIC_CPP_CODE
  #define GUCEF_PROJECTGEN_PUBLIC_CPP GUCEF_PROJECTGEN_EXPORTSPEC
#else
  #define GUCEF_PROJECTGEN_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_PROJECTGEN_PUBLIC_CPP_CODE */

#undef GUCEF_PROJECTGEN_PUBLIC_C
#ifdef GUCEF_PROJECTGEN_PUBLIC_C_CODE
  #define GUCEF_PROJECTGEN_PUBLIC_C GUCEF_PROJECTGEN_EXPORTSPEC
#else
  #define GUCEF_PROJECTGEN__PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEF_PROJECTGEN_PUBLIC_C_CODE */

#define GUCEF_PROJECTGEN_PRIVATE_CPP GUCEF_HIDDEN
#define GUCEF_PROJECTGEN_PRIVATE_C GUCEF_HIDDEN

/*-------------------------------------------------------------------------*/

#undef GUCEF_PROJECTGEN_PLUGIN_PUBLIC_CPP
#ifdef GUCEF_PROJECTGEN_PLUGIN_PUBLIC_CPP_CODE
  #define GUCEF_PROJECTGEN_PLUGIN_PUBLIC_CPP GUCEF_PROJECTGEN_PLUGIN_EXPORTSPEC
#else
  #define GUCEF_PROJECTGEN_PLUGIN_PUBLIC_CPP GUCEF_HIDDEN
#endif /* GUCEF_PROJECTGEN_PLUGIN_PUBLIC_CPP_CODE */

#undef GUCEF_PROJECTGEN_PLUGIN_PUBLIC_C
#ifdef GUCEF_PROJECTGEN_PLUGIN_PUBLIC_C_CODE
  #define GUCEF_PROJECTGEN_PLUGIN_PUBLIC_C GUCEF_PROJECTGEN_PLUGIN_EXPORTSPEC
#else
  #define GUCEF_PROJECTGEN_PLUGIN_PUBLIC_C GUCEF_HIDDEN
#endif /* GUCEF_PROJECTGEN_PLUGIN_PUBLIC_C_CODE */

#define GUCEF_PROJECTGEN_PLUGIN_PRIVATE_CPP GUCEF_HIDDEN
#define GUCEF_PROJECTGEN_PLUGIN_PRIVATE_C GUCEF_HIDDEN

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PROJECTGEN_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
