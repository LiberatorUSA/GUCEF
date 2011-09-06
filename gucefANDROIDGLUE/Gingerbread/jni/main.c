/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>
#include <unistd.h>             /* POSIX utilities */
#include <limits.h>             /* Linux OS limits */
#include <string.h>

#include <android/log.h>
#include <android_native_app_glue.h>

#include "gucefLOADER.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))


#define NULLPTR ((void*)0)

/*-------------------------------------------------------------------------*/
    
int
GetModulePath( char* buffer, int bufferSize )
{
    pid_t pid = getpid();

    char linkStr[ 24 ];
    sprintf( linkStr, "%s%d%s", "/proc/", pid, "/exe\0" );

    int ch = readlink( linkStr, buffer, bufferSize );
    if ( -1 != ch )
    {
        buffer[ ch ] = 0;
        return 0;
    }
    return 1;
}

/*-------------------------------------------------------------------------*/

void
StripFilename( char* buffer, int bufferSize )
{
}

/*-------------------------------------------------------------------------*/

void
GetModuleDir( char* buffer, int bufferSize )
{
    GetModulePath( buffer, bufferSize );
    StripFilename( buffer, bufferSize );
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
    
    char appDirBuffer[ PATH_MAX ];    
    GetModuleDir( appDirBuffer, PATH_MAX );
    
    int appStatus = LoadAndRunGucefPlatformApp( "gucefPRODMAN", appDirBuffer, 0, NULLPTR, 0, NULLPTR ); 
}

/*-------------------------------------------------------------------------*/

//END_INCLUDE(all)
