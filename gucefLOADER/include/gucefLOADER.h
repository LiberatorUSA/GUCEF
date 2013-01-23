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
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace LOADER {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *  Modules are to be stored according to the following path convention:
 *      <LoadRoot>/libs/<GroupName>/<MayorVersion>.<MinorVersion>/<ModuleName>/<PatchVersion>.<ReleaseVersion>/<ModuleFileName>
 *  Apps are to be stored according to the following path convention:
 *      <LoadRoot>/apps/<AppName>/<MayorVersion>.<MinorVersion>/<PatchVersion>.<ReleaseVersion>/<AppModuleFileName>
 */

/*
 *      Prevent C++ name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*-------------------------------------------------------------------------*/

/**
 *  Attempts to load the specified version of the app identified as "appName"
 *  using the parameters given as appArgc and appArgv. Passing less then 0 as
 *  a version value signals you want the latest version.
 */
GUCEF_LOADER_PUBLIC_C Int32
LoadAndRunGucefPlatformAppEx( const char* appName    ,
                              const char* rootDir    ,
                              const char* resRootDir ,
                              const char* libRootDir ,
                              Int32 platformArgc     ,
                              char** platformArgv    ,
                              Int32 appArgc          ,
                              char** appArgv         ,
                              Int32 majorVersion     ,
                              Int32 minorVersion     ,
                              Int32 patchVersion     ,
                              Int32 releaseVersion   );

/*-------------------------------------------------------------------------*/

/**
 *  Attempts to load the latest version of the app identified as "appName"
 *  using the parameters given as appArgc and appArgv
 */
GUCEF_LOADER_PUBLIC_C Int32
LoadAndRunGucefPlatformApp( const char* appName    ,
                            const char* rootDir    ,
                            const char* resRootDir ,
                            const char* libRootDir ,
                            Int32 platformArgc     ,
                            char** platformArgv    ,
                            Int32 appArgc          ,
                            char** appArgv         );

/*-------------------------------------------------------------------------*/

/**
 *  Sets all the information for what app to run after the current app
 *  finishes its execution. This functionality can be used to chain execute
 *  applications.
 */
GUCEF_LOADER_PUBLIC_C Int32
SetNextGucefPlatformAppToLoadAndRunEx( const char* appName    ,
                                       const char* rootDir    ,
                                       const char* resRootDir ,
                                       const char* libRootDir ,
                                       Int32 platformArgc     ,
                                       char** platformArgv    ,
                                       Int32 appArgc          ,
                                       char** appArgv         ,
                                       Int32 majorVersion     ,
                                       Int32 minorVersion     ,
                                       Int32 patchVersion     ,
                                       Int32 releaseVersion   );

/*-------------------------------------------------------------------------*/

/**
 *  Sets all the information for what app to run after the current app
 *  finishes its execution. This functionality can be used to chain execute
 *  applications.
 */
GUCEF_LOADER_PUBLIC_C Int32
SetNextGucefPlatformAppToLoadAndRun( const char* appName    ,
                                     const char* rootDir    ,
                                     const char* resRootDir ,
                                     const char* libRootDir ,
                                     Int32 platformArgc     ,
                                     char** platformArgv    ,
                                     Int32 appArgc          ,
                                     char** appArgv         );

/*-------------------------------------------------------------------------*/

/**
 *  Attenpts to load the given module.
 *  The module will be located using the module storage schema of gucefLOADER
 *  No functions will be loaded and/or executed. This function is only meant to load
 *  plugin modules using the same logic as the loader system from another location
 *  such as gucefCORE's plugin management system.
 *
 *  This should be considered an internal function, it is not for regular use.
 */
GUCEF_LOADER_PUBLIC_C void*
LoadSpecificModule( const char* moduleFilename  ,
                    const char* rootDir         ,
                    const char* moduleGroupName ,
                    Int32 majorVersion          ,
                    Int32 minorVersion          ,
                    Int32 patchVersion          ,
                    Int32 releaseVersion        );

/*-------------------------------------------------------------------------*/

/**
 *  Should be used to unload modules which were loaded using "LoadSpecificModule"
 */
GUCEF_LOADER_PUBLIC_C void
UnloadSpecificModule( void* modulePtr );

/*-------------------------------------------------------------------------*/

/**
 *  Provides the complete system path for the module specified.
 *  This allows you to get the path while retaining all the actual module organization logic
 *  inside gucefLOADER.
 *
 *  Note that if you pass -1 for any of the version attributes then the module specified must
 *  exist in order to determine the path. If a complete version is specified then the path will be
 *  determined irregardless of whether the module actually exists.
 *
 *  Possible return values are:
 *      - 0 : Successfully determined the path for the given module
 *      - 1 : No such module was found, unable to determine path
 *      - 2 : The given output buffer is not large enough to hold the complete path
 *  
 */
GUCEF_LOADER_PUBLIC_C UInt32
GetSystemPathForModule( char* pathOutputBuffer      ,
                        UInt32 outputBufferSize     ,
                        const char* moduleFilename  ,
                        const char* rootDir         ,
                        const char* moduleGroupName ,
                        Int32 majorVersion          ,
                        Int32 minorVersion          ,
                        Int32 patchVersion          ,
                        Int32 releaseVersion        );

/*-------------------------------------------------------------------------*/

/**
 *  Provides the complete system path for the application module specified.
 *  This allows you to get the path while retaining all the actual module organization logic
 *  inside gucefLOADER.
 *
 *  Note that if you pass -1 for any of the version attributes then the module specified must
 *  exist in order to determine the path. If a complete version is specified then the path will be
 *  determined irregardless of whether the module actually exists.
 *
 *  Possible return values are:
 *      - 0 : Successfully determined the path for the given module
 *      - 1 : No such module was found, unable to determine path
 *      - 2 : The given output buffer is not large enough to hold the complete path
 *  
 */
GUCEF_LOADER_PUBLIC_C UInt32
GetSystemPathForAppModule( char* pathOutputBuffer  ,
                           UInt32 outputBufferSize ,
                           const char* appName     ,
                           const char* rootDir     ,
                           Int32 majorVersion      ,
                           Int32 minorVersion      ,
                           Int32 patchVersion      ,
                           Int32 releaseVersion    );

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* extern "C" */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace LOADER */
}; /* namespace GUCEF */
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
