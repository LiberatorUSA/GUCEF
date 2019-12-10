/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CONFIG_H
#define GUCEF_COMCORE_CONFIG_H

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
#undef GUCEF_COMCORE_EXPORT_CPP_CODE        /* do not change this line */ 
#undef GUCEF_COMCORE_EXPORT_C_CODE          /* do not change this line */  
#define GUCEF_COMCORE_EXPORT_CPP_CODE /* do you want to enable the C++ exports ? */
#define GUCEF_COMCORE_EXPORT_C_CODE   /* do you want to enable the C exports ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-10-2004 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
 
