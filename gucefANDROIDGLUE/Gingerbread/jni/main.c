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
#include <sys/stat.h>
#include <unistd.h>             /* POSIX utilities */
#include <limits.h>             /* Linux OS limits */
#include <string.h>
#include <dlfcn.h>              /* dynamic linking utilities */
#include <stdio.h>

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>

#include "gucefLOADER.h"

void
android_syslog(int level, const char *format, ...)
{
    va_list arglist;
    va_start( arglist, format );
    __android_log_vprint( level, "GalaxyUnlimitedPlatform", format, arglist );
    va_end( arglist );
    return;
}


#define FLOGI( format, ... ) ( (void) android_syslog( ANDROID_LOG_INFO, format, __VA_ARGS__) )
#define FLOGW( format, ... ) ( (void) android_syslog( ANDROID_LOG_WARN, format, __VA_ARGS__) )
#define FLOGF( format, ... ) ( (void) android_syslog( ANDROID_LOG_FATAL, format, __VA_ARGS__) )
#define FLOGE( format, ... ) ( (void) android_syslog( ANDROID_LOG_ERROR, format, __VA_ARGS__) )
#define FLOGD( format, ... ) ( (void) android_syslog( ANDROID_LOG_DEBUG, format, __VA_ARGS__) )

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GalaxyUnlimitedPlatform", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "GalaxyUnlimitedPlatform", __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, "GalaxyUnlimitedPlatform", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "GalaxyUnlimitedPlatform", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "GalaxyUnlimitedPlatform", __VA_ARGS__))

typedef int ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_LoadAndRunGucefPlatformApp ) ( const char* appName, const char* rootDir, const char* resRootDir, const char* libRootDir, int platformArgc, char** platformArgv, int appArgc, char** appArgv );

#define NULLPTR ((void*)(0))

/*-------------------------------------------------------------------------*/

char*
Combine2Strings( const char* str1 ,
                 const char* str2 )
{
    int str1len = strlen( str1 );
    int str2len = strlen( str2 );
    char* strBuffer = (char*) malloc( str1len + str2len + 1 );
    memcpy( strBuffer, str1, str1len );
    memcpy( strBuffer+str1len, str2, str2len+1 );
    return strBuffer;
}

/*-------------------------------------------------------------------------*/

char*
Combine3Strings( const char* str1 ,
                 const char* str2 ,
                 const char* str3 )
{
    int str1len = strlen( str1 );
    int str2len = strlen( str2 );
    int str3len = strlen( str3 );
    char* strBuffer = (char*) malloc( str1len + str2len + str3len + 1 );
    memcpy( strBuffer, str1, str1len );
    memcpy( strBuffer+str1len, str2, str2len );
    memcpy( strBuffer+str1len+str2len, str3, str3len+1 );
    return strBuffer;
}

/*-------------------------------------------------------------------------*/

char*
GetAssetPath( const char* packageDir ,
              const char* fileName   )
{
    return Combine3Strings( packageDir, "/assets/", fileName );
}

/*-------------------------------------------------------------------------*/

char*
GetLibPath( const char* packageDir ,
            const char* moduleName )
{
    return Combine3Strings( packageDir, "/lib/", moduleName );
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

    char* libRootDir = GetLibPath( rootDir, "" );
    char* assetRootDir = GetAssetPath( rootDir, "" );

    int returnValue = loadAndRunGucefPlatformApp( appName      ,
                                                  rootDir      ,
                                                  assetRootDir ,
                                                  libRootDir   ,
                                                  platformArgc ,
                                                  platformArgv ,
                                                  appArgc      ,
                                                  appArgv      );

    free( libRootDir );
    libRootDir = NULL;
    free( assetRootDir );
    assetRootDir = NULL;
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

int
FileExists( const char* filename )
{
    struct stat buf;
    return stat( filename, &buf ) == 0;
}

/*-------------------------------------------------------------------------*/

int
MakeResourceDirs( const char* packageDir )
{
    char* assetsDir = Combine2Strings( packageDir, "/assets" );
    int retValue = mkdir( assetsDir, 00777 );

    if ( retValue == 0 )
    {
        FLOGI( "created dir: %s", assetsDir );
    }
    else
    {
        if ( EEXIST == errno )
        {
            FLOGI( "found existing dir: %s", assetsDir );
        }
        else
        {
            FLOGI( "error %i creating dir: %s", errno, assetsDir );
        }
    }
    free( assetsDir );

    // No problem
    return retValue;
}

/*-------------------------------------------------------------------------*/

void
ExtractResources( struct android_app* state ,
                  const char* packageDir    )
{
    AAssetManager* assetManager = state->activity->assetManager;

/*    AAssetDir* resRawDir = AAssetManager_openDir( assetManager, "" );
    if ( NULL == resRawDir )
    {
        LOGI( "Unable to open res/raw asset dir" );
        return;
    }

    LOGI( "Files in dir:" );
    const char* filename = AAssetDir_getNextFileName( resRawDir );
    while ( NULL != filename )
    {
            LOGI( filename );
            filename = AAssetDir_getNextFileName( resRawDir );
    }
*/

    AAsset* extractionIndex = AAssetManager_open( assetManager, "filestoextract.txt", AASSET_MODE_BUFFER );
    if ( NULL == extractionIndex )
    {
        //AAssetDir_close( resRawDir );
        LOGI( "Unable to open filestoextract.txt" );
        return;
    }

    const void* fileBuffer = AAsset_getBuffer( extractionIndex );
    off_t bufferSize = AAsset_getLength( extractionIndex );

    char* destPath = GetAssetPath( packageDir, "filestoextract.txt" );
    FILE* destFile = fopen( destPath, "wb" );
    free( destPath );

    if ( 1 != fwrite( fileBuffer, bufferSize, 1, destFile ) )
    {
        LOGE( "Error writing extracted file to new destination" );
    }
    fclose( destFile );

    AAsset_close( extractionIndex );
    //AAssetDir_close( resRawDir );
}

/*-------------------------------------------------------------------------*/

int
IsFirstRun( const char* packageDir )
{
    // We know the gucefLOADER relies on a text file named firstrun.complete.txt
    // we will use the same convention here to keep things consistent
    char* firstrunFile = GetAssetPath( packageDir, "firstrun.completed.txt" );
    int existsBool = FileExists( firstrunFile );
    free( firstrunFile );
    return existsBool;
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

    char packageDir[ 512 ];
    GetPackageDir( state, packageDir, 512 );

    // Check if we need to perform first time initialization
    if ( 0 == IsFirstRun( packageDir ) )
    {
        LOGI( "Performing first run initialization" );

        // Make the resource dirs if they do not exist yet
        if ( 0 != MakeResourceDirs( packageDir ) )
        {
            return;
        }

        ExtractResources( state, packageDir );
    }

    int appStatus = InvokeLoadAndRunGucefPlatformApp( "gucefPRODMAN", packageDir, 0, NULLPTR, 0, NULLPTR );
}

/*-------------------------------------------------------------------------*/

//END_INCLUDE(all)
