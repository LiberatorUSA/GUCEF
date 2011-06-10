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

#ifndef GUCEF_LOADER_H
#define GUCEF_LOADER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_LOADER_MACROS_H
#include "gucefLOADER_macros.h"
#define GUCEF_LOADER_MACROS_H
#endif /* GUCEF_LOADER_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  Modules are should be stored according to the following path convention:
 *      <LoadRoot>/<GroupName>/<MayorVersion>.<MinorVersion>/<ModuleName>/<PatchVersion>.<ReleaseVersion>
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

/**
 *  Attempts to load the specified version of the app identified as "appName"
 *  using the parameters given as appArgc and appArgv. Passing less then 0 as
 *  a version value signals you want the latest version.
 */
GUCEF_LOADER_PUBLIC_C int
LoadAndRunGucefPlatformAppEx( const char* appName ,
                              const char* rootDir ,
                              int appArgc         ,
                              char** appArgv      ,
                              long majorVersion   ,
                              long minorVersion   ,
                              long patchVersion   ,
                              long releaseVersion );

/*-------------------------------------------------------------------------*/

/**
 *  Attempts to load the latest version of the app identified as "appName"
 *  using the parameters given as appArgc and appArgv
 */
GUCEF_LOADER_PUBLIC_C int
LoadAndRunGucefPlatformApp( const char* appName ,
                            const char* rootDir ,
                            int appArgc         ,
                            char** appArgv      );

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* extern "C" */
#endif /* __cplusplus ? */
                   
/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_LOADER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

---------------------------------------------------------------------------*/
