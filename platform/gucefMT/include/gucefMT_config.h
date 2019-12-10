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

#ifndef GUCEF_MT_CONFIG_H
#define GUCEF_MT_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"       /* GUCEF platform config */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      GUCEF_MT_BUILD_MODULE
 *
 *      It is best to set this in your project/make file, you can however
 *      set this here if your environment does not allow
 */
/* #define GUCEF_MT_BUILD_MODULE */

#define GUCEF_MT_PUBLIC_C_CODE
#define GUCEF_MT_PUBLIC_CPP_CODE

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-09-2006 :
       - Dinand: moved platform wide build switches to the GUCEF platform config file
- 25-02-2006 :
       - Dinand: Added GUCEFMT_ prefix to all defines
- 16-10-2004 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/

