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

#ifndef PROJECTGENDEPENDSFILTER_MACROS_H
#define PROJECTGENDEPENDSFILTER_MACROS_H

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

#ifndef PROJECTGENDEPENDSFILTER_CONFIG_H
#include "ProjectGenDependsFilter_config.h"
#define PROJECTGENDEPENDSFILTER_CONFIG_H
#endif /* PROJECTGENDEPENDSFILTER_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GENERAL MACROS                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

#undef PROJECTGENDEPFILTER_EXPORT
#ifdef PROJECTGENDEPFILTER_BUILD_MODULE
  #define PROJECTGENDEPFILTER_EXPORT GUCEF_EXPORT
#else
  #define PROJECTGENDEPFILTER_EXPORT GUCEF_IMPORT
#endif /* PROJECTGENDEPFILTER_BUILD_MODULE ? */

#undef PROJECTGENDEPFILTER_EXPORT_CPP
#ifdef PROJECTGENDEPFILTER_EXPORT_CPP_CODE
  #define PROJECTGENDEPFILTER_EXPORT_CPP PROJECTGENDEPFILTER_EXPORT
#else
  #define PROJECTGENDEPFILTER_EXPORT_CPP GUCEF_HIDDEN
#endif /* PROJECTGENDEPFILTER_EXPORT_CPP_CODE */

#undef PROJECTGENDEPFILTER_EXPORT_C
#ifdef PROJECTGENDEPFILTER_EXPORT_C_CODE
  #define PROJECTGENDEPFILTER_EXPORT_C PROJECTGENDEPFILTER_EXPORT
#else
  #define PROJECTGENDEPFILTER_EXPORT_C GUCEF_HIDDEN
#endif /* PROJECTGENDEPFILTER_EXPORT_C_CODE */

/*-------------------------------------------------------------------------*/

#endif /* PROJECTGENDEPENDSFILTER_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-03-2005 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
