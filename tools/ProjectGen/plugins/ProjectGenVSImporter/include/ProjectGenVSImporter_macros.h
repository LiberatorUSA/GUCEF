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

#ifndef PROJECTGENVSIMPORTER_MACROS_H
#define PROJECTGENVSIMPORTER_MACROS_H

/*
 *      Build configuration specific macros.
 *      Also includes the generic macros which are build config independant.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PROJECTGEN_ETYPES_H
#include "gucefProjectGen_ETypes.h"      /* simple types */
#define GUCEF_PROJECTGEN_ETYPES_H
#endif /* GUCEF_PROJECTGEN_ETYPES_H ? */

#ifndef GUCEF_PROJECTGEN_MACROS_H
#include "gucefProjectGen_macros.h"
#define GUCEF_PROJECTGEN_MACROS_H
#endif /* GUCEF_PROJECTGEN_MACROS_H ? */

#ifndef PROJECTGENVSIMPORTER_CONFIG_H
#include "ProjectGenVSImporter_config.h"
#define PROJECTGENVSIMPORTER_CONFIG_H
#endif /* PROJECTGENVSIMPORTER_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#undef PROJECTGENVSIMPORTER_EXPORT
#ifdef PROJECTGENVSIMPORTER_BUILD_MODULE
  #define PROJECTGENVSIMPORTER_EXPORT GUCEF_EXPORT
#else
  #define PROJECTGENVSIMPORTER_EXPORT GUCEF_IMPORT
#endif /* PROJECTGENVSIMPORTER_BUILD_MODULE ? */

#undef PROJECTGENVSIMPORTER_EXPORT_CPP
#ifdef PROJECTGENVSIMPORTER_EXPORT_CPP_CODE
  #define PROJECTGENVSIMPORTER_EXPORT_CPP PROJECTGENVSIMPORTER_EXPORT
#else
  #define PROJECTGENVSIMPORTER_EXPORT_CPP GUCEF_HIDDEN
#endif /* PROJECTGENVSIMPORTER_EXPORT_CPP_CODE */

#undef PROJECTGENVSIMPORTER_EXPORT_C
#ifdef PROJECTGENVSIMPORTER_EXPORT_C_CODE
  #define PROJECTGENVSIMPORTER_EXPORT_C PROJECTGENVSIMPORTER_EXPORT
#else
  #define PROJECTGENVSIMPORTER_EXPORT_C GUCEF_HIDDEN
#endif /* PROJECTGENVSIMPORTER_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* PROJECTGENVSIMPORTER_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
