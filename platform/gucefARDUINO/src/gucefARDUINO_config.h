/*
 *  gucefLOADER: GUCEF module which loads up the GUCEF platform
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_ARDUINO_CONFIG_H
#define GUCEF_ARDUINO_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/* Switches for the export of C++ and/or C code */
#undef GUCEF_ARDUINO_EXPORT_CPP_CODE     /* do not change this line */
#undef GUCEF_ARDUINO_EXPORT_C_CODE       /* do not change this line */
/* #define GUCEF_ARDUINO_EXPORT_CPP_CODE */
#define GUCEF_ARDUINO_EXPORT_C_CODE

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_ARDUINO_CONFIG_H */

 
