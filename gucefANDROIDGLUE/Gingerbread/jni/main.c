/*
 *  gucefANDROIDGLUE: GUCEF module providing glue code for Android
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

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>
#include <unistd.h>             /* POSIX utilities */
#include <limits.h>             /* Linux OS limits */
#include <string.h>
#include <dlfcn.h>              /* dynamic linking utilities */

#include <android/log.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>

#include "gucefLOADER.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GalaxyUnlimitedPlatform", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "GalaxyUnlimitedPlatform", __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, "GalaxyUnlimitedPlatform", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "GalaxyUnlimitedPlatform", __VA_ARGS__))

typedef int ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_LoadAndRunGucefPlatformApp ) ( const char* appName, const char* resRootDir, const char* libRootDir, int platformArgc, char** platformArgv, int appArgc, char** appArgv );

#define NULLPTR ((void*)(0))

/*-------------------------------------------------------------------------*/

char*
GetRawResourcePath( const char* packageDir ,
                    const char* fileName   )
{
    int pathLength = strlen( packageDir );
    int fileNameLength = strlen( fileName );
    char* filePath = malloc( pathLength+fileNameLength+10 );
    memcpy( filePath, packageDir, pathLength );
    memcpy( filePath+pathLength, "/res/raw/", 9 );
    memcpy( filePath+pathLength+9, fileName, fileNameLength+1 );
    return filePath;
}

/*-------------------------------------------------------------------------*/

char*
GetLibPath( const char* packageDir ,
            const char* moduleName )
{
    int pathLength = strlen( packageDir );
    int moduleNameLength = strlen( moduleName );
    char* modulePath = malloc( pathLength+moduleNameLength+6 );
    memcpy( modulePath, packageDir, pathLength );
    memcpy( modulePath+pathLength, "/lib/", 5 );
    memcpy( modulePath+pathLength+5, moduleName, moduleNameLength+1 );
    return modulePath;
}

/*-------------------------------------------------------------------------*/

int
InvokeLoadAndRunGucefPlatformApp( const char* appName ,
                                  const char* rootDir ,
                                  int platformArgc    ,
                                  char** platformArgv ,
                                  int appArgc         ,
                                  char** appArgv      )
{
    char* modulePath = GetLibPath( rootDir, "libgucefLOADER.so" );
    void* modulePtr = (void*) dlopen( modulePath, RTLD_NOW );
    if ( NULL == modulePtr )
    {
        free( modulePath );
        LOGF( "Unable to link gucefLOADER module" );
        return 0;
    }
    LOGI( "Loading loader module from:" );
    LOGI( modulePath );
    free( modulePath );
    modulePath = NULL;

    TGUCEFCORECINTERFACE_LoadAndRunGucefPlatformApp loadAndRunGucefPlatformApp =
        (TGUCEFCORECINTERFACE_LoadAndRunGucefPlatformApp) dlsym( modulePtr, "LoadAndRunGucefPlatformApp" );

    if ( NULL == loadAndRunGucefPlatformApp )
    {
        LOGF( "Unable to link gucefLOADER function: LoadAndRunGucefPlatformApp" );
        dlclose( modulePtr );
        return 0;
    }

    const char* libRootDir = GetLibPath( rootDir, "" );
    const char* resRootDir = GetRawResourcePath( rootDir, "" );

    int returnValue = loadAndRunGucefPlatformApp( appName      ,
                                                  resRootDir   ,
                                                  libRootDir   ,
                                                  platformArgc ,
                                                  platformArgv ,
                                                  appArgc      ,
                                                  appArgv      );

    free( libRootDir );
    libRootDir = NULL;
    free( resRootDir );
    resRootDir = NULL;
    dlclose( modulePtr );
    modulePtr = NULL;

    return returnValue;
}

/*-------------------------------------------------------------------------*/

void
GetPackageDir( struct android_app* state ,
               char* pathBuffer          ,
               int bufferSize            )
{
    ANativeActivity* activity = state->activity;
    int pathLength = strlen( activity->internalDataPath );
    memcpy( pathBuffer, activity->internalDataPath, pathLength+1 );

    int i=pathLength;
    while ( i > 0 )
    {
        if ( pathBuffer[ i ] == '/' )
        {
            pathBuffer[ i ] = '\0';
            break;
        }
        --i;
    }
}

/*-------------------------------------------------------------------------*/

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void
android_main( struct android_app* state )
{
    // Make sure glue isn't stripped.
    app_dummy();

    char appDir[ 512 ];
    GetPackageDir( state, appDir, 512 );

    int appStatus = InvokeLoadAndRunGucefPlatformApp( "gucefPRODMAN", appDir, 0, NULLPTR, 0, NULLPTR );
}

/*-------------------------------------------------------------------------*/

//END_INCLUDE(all)
