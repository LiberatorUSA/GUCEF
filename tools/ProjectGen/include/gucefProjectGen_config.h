/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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

#ifndef GUCEF_PROJECTGEN_CONFIG_H
#define GUCEF_PROJECTGEN_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      If we are building or using a DLL then it is sometimes desireable to
 *      export only C code or perhaps only C++ code. You can do this with the
 *      following defines
 */
#undef GUCEF_PROJECTGEN_PUBLIC_CPP_CODE        /* do not change this line */ 
#undef GUCEF_PROJECTGEN_PUBLIC_C_CODE          /* do not change this line */  
//#define GUCEF_PROJECTGEN_PUBLIC_CPP_CODE /* do you want to enable the C++ exports ? */
//#define GUCEF_PROJECTGEN_PUBLIC_C_CODE   /* do you want to enable the C exports ? */

/*-------------------------------------------------------------------------*/

/*
 *      Switches for setting the calling convention used by the gucefPROJECTGEN
 *      module. Please enable only 1, no more, no less.
 */
#undef USE_CALLSPEC_C                /* do not change this line */
#undef USE_CALLSPEC_STD              /* do not change this line */ 
#undef USE_CALLSPEC_PASCAL           /* do not change this line */ 
#undef USE_CALLSPEC_FAST             /* do not change this line */
#undef USE_PLUGIN_CALLSPEC_C         /* do not change this line */
#undef USE_PLUGIN_CALLSPEC_STD       /* do not change this line */ 
#undef USE_PLUGIN_CALLSPEC_PASCAL    /* do not change this line */ 
#undef USE_PLUGIN_CALLSPEC_FAST      /* do not change this line */
  
#define USE_CALLSPEC_C 
/* #define USE_CALLSPEC_STD */
/* #define USE_CALLSPEC_PASCAL */
/* #define USE_CALLSPEC_FAST */
/* #define USE_PLUGIN_CALLSPEC_C */
#define USE_PLUGIN_CALLSPEC_STD
/* #define USE_PLUGIN_CALLSPEC_PASCAL */
/* #define USE_PLUGIN_CALLSPEC_FAST */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PROJECTGEN_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/
