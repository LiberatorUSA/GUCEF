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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <set>
#include <map>
#include <vector>

#ifndef GUCEF_CORE_ESTRUCTS_H
#include "EStructs.h"
#define GUCEF_CORE_ESTRUCTS_H
#endif /* GUCEF_CORE_ESTRUCTS_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#include "gucefLOADER.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef std::set< CORE::CString > TStringSet;

struct SModuleInfo
{
    void* handle;
    CORE::TVersion version;
    CORE::CString name;
};
typedef struct SModuleInfo TModuleInfo;

typedef std::vector< TModuleInfo > TModuleInfoVector;

struct SModuleGroup
{
    TModuleInfoVector modules;
    CORE::CString name;
};
typedef struct SModuleGroup TModuleGroup;

/*-------------------------------------------------------------------------*/

/*
 *  The following are type definitions for the CORE module C interface functions
 */
typedef int ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_AppMain ) ( int argc, char** argv, int runAppBool ) GUCEF_CALLSPEC_SUFFIX;
typedef int ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_AppLoadConfig ) ( const char* configData, const char* dataCodec ) GUCEF_CALLSPEC_SUFFIX;
typedef int ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_AppLoadGenericPlugin ) ( const char* pluginPath, int argc, char** argv ) GUCEF_CALLSPEC_SUFFIX;

struct SGucefCoreCInterface
{
    TGUCEFCORECINTERFACE_AppMain appMain;
    TGUCEFCORECINTERFACE_AppLoadConfig appLoadConfig;
    TGUCEFCORECINTERFACE_AppLoadGenericPlugin appLoadGenericPlugin;
};
typedef struct SGucefCoreCInterface TGucefCoreCInterface;

/*-------------------------------------------------------------------------*/

/*
 *  The following is a struct containing info parsed from a loader
 *  config file. It is meant to drive the actions of the app load functions
 */
struct SLoaderAppConfig
{
    unsigned long gucefMajorVersion;
    unsigned long gucefMinorVersion;
    long gucefPatchVersion;
    long gucefReleaseVersion;
}
typedef struct SLoaderAppConfig TLoaderAppConfig;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_C void
FindAnyParamKey( char** argv    ,
                 int argc       ,
                 int startIndex ,
                 int* keyIndex  )
{
    int i=startIndex;

    *keyIndex = -1;
    for ( i; i<argc; ++i )
    {
        if ( argv[ i ][ 0 ] == '-' )
        {
            *keyIndex = i;
            return;
        }
    }
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_C void
FindParamKey( char** argv        ,
              int argc           ,
              int startIndex     ,
              const char* keyStr ,
              int* keyIndex      )
{
    int curKeyIndex = -1;
    do
    {
        FindAnyParamKey( argv         ,
                         argc         ,
                         startIndex   ,
                         &curKeyIndex );

        if ( curKeyIndex > -1 )
        {
            if ( 0 == strcmp( argv[ curKeyIndex ], keyStr ) )
            {
                *keyIndex = curKeyIndex;
                return;
            }
        }
        startIndex = curKeyIndex+1;
    }
    while ( curKeyIndex  > -1 );
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_C void
FindParam( const char* paramKey ,
           int* paramStartIndex ,
           int* paramEndIndex   ,
           char** argv          ,
           int argc             )
{
    int keyIndex = -1;

    // Init return values
    *paramStartIndex = -1;
    *paramEndIndex = -1;

    // First try to locate the requested key
    FindParamKey( argv, argc, 0, paramKey, &keyIndex );
    if ( keyIndex > -1 )
    {
        // Found it, now find the end delimeter which is either another key
        // or the end of the string list
        *paramStartIndex = keyIndex;
        keyIndex = -1;

        FindAnyParamKey( argv       ,
                         argc       ,
                         keyIndex+1 ,
                         &keyIndex  );

        if ( keyIndex > -1 )
        {
            // end delimiter is another key
            *paramEndIndex = keyIndex;
        }
        else
        {
            // end delimiter is the end of the string set
            *paramEndIndex = argc-1;
        }
    }
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_C CORE::CString
GetModuleRootPath( char** argv ,
                   int argc    )
{
    CORE::CString moduleRoot;
    int paramStartIndex = -1;
    int paramEndIndex = -1;

    FindParam( "-moduleRoot"    ,
               &paramStartIndex ,
               &paramEndIndex   ,
               argv             ,
               argc             );

    if ( ( paramStartIndex > -1 && paramEndIndex > -1 ) &&
         ( paramStartIndex != paramEndIndex )            )
    {
        if ( paramStartIndex+1 < argc-1 )
        {
            moduleRoot = CORE::RelativePath( argv[ paramStartIndex+1 ] );
        }
    }

    if ( moduleRoot.IsNULLOrEmpty() )
    {
        moduleRoot = CORE::RelativePath( "$MODULEDIR$" );
    }

    return moduleRoot;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_C void
ParseListOfExtraModulestoLoad( char** argv        ,
                               int argc           ,
                               char*** moduleList ,
                               int* moduleCount   )
{
    int paramStartIndex = -1;
    int paramEndIndex = -1;

    FindParam( "-gucefModules",
               &paramStartIndex ,
               &paramEndIndex   ,
               argv             ,
               argc             );

    if ( ( paramStartIndex > -1 && paramEndIndex > -1 ) &&
         ( paramStartIndex != paramEndIndex > -1 )       )
    {
        if ( paramStartIndex+1 < argc-1 )
        {
            *moduleList = argv+paramStartIndex+1;
            *moduleCount = (paramEndIndex-paramStartIndex)+1;
            return;
        }
    }

    *moduleList = NULL;
    *moduleCount = 0;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
GetHighestVersionAvailableFromDir( const CORE::CString rootDir ,
                                   long& patchVersion          ,
                                   long& releaseVersion        )
{
    typedef std::map< CORE::Int32, CORE::Int32 > TVersionMap;

    TVersionMap versionMap;
    struct CORE::SDI_Data* itData = CORE::DI_First_Dir_Entry( rootDir.C_String() );
    if ( NULL != itData )
    {
        do
        {
            // We only care about dirs
            if ( 0 == CORE::DI_Is_It_A_File( itData ) )
            {
                // Filter out . and .. dirs
                if ( 0 != strcmp( ".", CORE::DI_Name( itData ) ) &&
                     0 != strcmp( "..", CORE::DI_Name( itData ) ) )
                {
                    CORE::CString dirName = CORE::DI_Name( itData );
                    CORE::Int32 dirPatchVersion = CORE::StringToInt32( dirName.SubstrToChar( '.', 0, true ) );
                    CORE::Int32 dirReleaseVersion = CORE::StringToInt32( dirName.SubstrToChar( '.', 0, false ) );

                    TVersionMap::iterator i = versionMap.find( dirPatchVersion );
                    if ( i != versionMap.end() )
                    {
                        CORE::Int32& higestFoundReleaseVersion = (*i).second;
                        if ( dirReleaseVersion > higestFoundReleaseVersion )
                        {
                            higestFoundReleaseVersion = dirReleaseVersion;
                        }
                    }
                    else
                    {
                        versionMap[ dirPatchVersion ] = dirReleaseVersion;
                    }
                }
            }
        }
        while ( 0 != CORE::DI_Next_Dir_Entry( itData ) );

        CORE::DI_Cleanup( itData );
    }

    if ( patchVersion > -1 )
    {
        // We need to match a specific patch version so we only look at the release version
        TVersionMap::iterator n = versionMap.find( patchVersion );
        if ( n != versionMap.end() )
        {
            releaseVersion = (*n).second;
            return true;
        }

        // Unable to find anything for this patch version
        releaseVersion = -1;
        return false;
    }

    // if we get here then just get the highest patch/release combo
    if ( !versionMap.empty() )
    {
        TVersionMap::iterator n = versionMap.begin();
        patchVersion = (*n).first;
        releaseVersion = (*n).second;
        return true;
    }
    patchVersion = -1;
    releaseVersion = -1;
    return false;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
IsModuleVersionAlreadyLoadedIfNotUnloadExisting( const char* moduleGroupName         ,
                                                 const char* moduleName              ,
                                                 const CORE::TVersion& moduleVersion ,
                                                 void* previousLoadData              )
{
    if ( NULL == previousLoadData ) return false;

    TModuleGroup* moduleGroup = (TModuleGroup*) previousLoadData;

    try
    {
        if ( moduleGroup->name == moduleGroupName )
        {
            TModuleInfoVector::iterator i = moduleGroup->modules.begin();
            while ( i != moduleGroup->modules.end() )
            {
                TModuleInfo& moduleInfo = (*i);
                if ( moduleInfo.name == moduleName )
                {
                    // We found a module which is loaded which belongs to the same group
                    // it also has the same name. Lets check the version
                    if ( ( moduleInfo.version.major == moduleVersion.major )   &&
                         ( moduleInfo.version.minor == moduleVersion.minor )   &&
                         ( moduleInfo.version.patch == moduleVersion.patch )   &&
                         ( moduleInfo.version.release == moduleVersion.release ) )
                    {
                        return true;
                    }
                    else
                    {
                        // This module is already loaded but with a different version
                        // We should unload this module
                        CORE::UnloadModuleDynamicly( moduleInfo.handle );
                        moduleInfo.handle = NULL;
                        moduleGroup->modules.erase( i );
                        return false;
                    }
                }
                ++i;
            }
        }
        return false;
    }
    catch ( ... )
    {
        return false;
    }
}

/*-------------------------------------------------------------------------*/

void*
LoadModules( const char* groupName        ,
             unsigned long majorVersion   ,
             unsigned long minorVersion   ,
             long patchVersion            ,
             long releaseVersion          ,
             char** argv                  ,
             int argc                     ,
             void* previousLoadData       )
{
    if ( NULL == groupName ) return NULL;
    TModuleGroup* moduleGroup = NULL;

    try
    {
        // get a list of optional modules that should be loaded
        char** moduleList = NULL;
        int moduleCount = 0;
        ParseListOfExtraModulestoLoad( argv         ,
                                       argc         ,
                                       &moduleList  ,
                                       &moduleCount );

        // get the root path to where we should load modules from
        // This gives us: <LoadRoot>/<GroupName>
        CORE::CString moduleRoot = GetModuleRootPath( argv, argc );
        CORE::AppendToPath( moduleRoot, groupName );

        CORE::TVersion moduleVersion;
        moduleVersion.major = (CORE::UInt16) majorVersion;
        moduleVersion.minor = (CORE::UInt16) minorVersion;

        // adjust path for desired version
        // This gives us: <LoadRoot>/<GroupName>/<MajorVersion>.<MinorVersion>
        char versionDir[ 41 ];
        sprintf( versionDir, "%d.%d", majorVersion, minorVersion );
        CORE::AppendToPath( moduleRoot, versionDir );

        // check if all desired modules are present
        moduleGroup = new TModuleGroup();
        moduleGroup->name = groupName;
        for ( int i=0; i<moduleCount; ++i )
        {
            // Add the module name to our root path
            // This gives us: <LoadRoot>/<GroupName>/<MajorVersion>.<MinorVersion>/<ModuleName>
            CORE::CString filePath = moduleRoot;
            CORE::AppendToPath( filePath, moduleList[ i ] );

            long modulePatchVersion = patchVersion;
            long moduleReleaseVersion = releaseVersion;
            if ( patchVersion < 0 || releaseVersion < 0 )
            {
                // We will have to search for the latest version available
                if ( !GetHighestVersionAvailableFromDir( filePath             ,
                                                         modulePatchVersion   ,
                                                         moduleReleaseVersion ) )
                {
                    // Unable to find any versions in this dir for this module
                    UnloadModules( moduleGroup );
                    return NULL;
                }
            }
            moduleVersion.patch = (CORE::UInt16) modulePatchVersion;
            moduleVersion.release = (CORE::UInt16) moduleReleaseVersion;

            if ( IsModuleVersionAlreadyLoadedIfNotUnloadExisting( groupName        ,
                                                                  moduleList[ i ]  ,
                                                                  moduleVersion    ,
                                                                  previousLoadData ) )
            {
                // Simply append with the specific version requested or if no specific version was
                // requested then the highest version located.
                // This gives us: <LoadRoot>/<GroupName>/<MajorVersion>.<MinorVersion>/<ModuleName>/<PatchVersion>.<ReleaseVersion>
                sprintf( versionDir, "%d.%d", patchVersion, releaseVersion );
                CORE::AppendToPath( filePath, versionDir );

                // Now that we constructed the full load path for this module we will try and load it
                void* modulePtr = CORE::LoadModuleDynamicly( filePath.C_String() );
                if ( NULL != modulePtr )
                {
                    TModuleInfo moduleInfo;
                    moduleInfo.handle = modulePtr;
                    moduleInfo.name = moduleList[ i ];
                    moduleInfo.version = moduleVersion;

                    moduleGroup->modules.push_back( moduleInfo );
                }
                else
                {
                    // We are missing one of the requested modules...
                    // Abort
                    UnloadModules( moduleGroup );
                    return NULL;
                }
            }
        }

        // Return a pointer to the loaded modules which the user
        // should give back to us later
        return moduleGroup;
    }
    catch ( ... )
    {
        // Don't allow exceptions to escape outside the loader
        UnloadModules( moduleGroup );
        return NULL;
    }
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP void*
GetModulePtr( void* moduleData       ,
              const char* moduleName )
{
    try
    {
        if ( NULL != moduleData )
        {
            TModuleGroup* moduleGroup = (TModuleGroup*) moduleData;
            TModuleInfoVector& moduleInfoVector = moduleGroup->modules;
            TModuleInfoVector::iterator n = moduleInfoVector.begin();
            while ( n != moduleInfoVector.end() )
            {
                if ( moduleName == (*n).name )
                {
                    return (*n).handle;
                }
                ++n;
            }
        }
    }
    catch ( ... )
    {
        // Don't allow exceptions to escape outside the loader
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

void
UnloadModules( void* moduleData )
{
    try
    {
        if ( NULL != moduleData )
        {
            TModuleGroup* moduleGroup = (TModuleGroup*) moduleData;
            TModuleInfoVector& moduleInfoVector = moduleGroup->modules;
            TModuleInfoVector::iterator n = moduleInfoVector.begin();
            while ( n != moduleInfoVector.end() )
            {
                CORE::UnloadModuleDynamicly( (*n).handle );
                ++n;
            }
            delete moduleGroup;
        }
    }
    catch ( ... )
    {
        // Don't allow exceptions to escape outside the loader
    }
}

/*-------------------------------------------------------------------------*/

void*
LoadGucefPlatform( unsigned long mayorVersion   ,
                   unsigned long minorVersion   ,
                   char** argv                  ,
                   int argc                     ,
                   void* previousLoadData       )
{

    return LoadModules( "GUCEF"          ,
                        mayorVersion     ,
                        minorVersion     ,
                        -1               ,
                        -1               ,
                        argv             ,
                        argc             ,
                        previousLoadData );
}


/*-------------------------------------------------------------------------*/

void*
LoadGucefPlatformEx( unsigned long mayorVersion   ,
                     unsigned long minorVersion   ,
                     unsigned long patchVersion   ,
                     unsigned long releaseVersion ,
                     char** argv                  ,
                     int argc                     ,
                     void* previousLoadData       )
{

    return LoadModules( "GUCEF"               ,
                        mayorVersion          ,
                        minorVersion          ,
                        (long) patchVersion   ,
                        (long) releaseVersion ,
                        argv                  ,
                        argc                  ,
                        previousLoadData      );
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
LinkGucefCoreCInterface( void* moduleData                 ,
                         TGucefCoreCInterface& cInterface )
{
    // grab pointer to gucefCORE module
    void* coreModulePtr = GetModulePtr( moduleData, "gucefCORE" );    
    if ( NULL == coreModulePtr )
    {
        return false;
    }
    
    // Get pointers to the CORE functions we need
    // We use the C interface to be independent of class changes
    cInterface.appLoadConfig = (TGUCEFCORECINTERFACE_AppLoadConfig)
           = CORE::GetFunctionAddress( coreModulePtr              ,
                                       "GUCEF_CORE_AppLoadConfig" ,
                                       sizeof(const char*)*2      ).funcPtr; 
    cInterface.appMain = (TGUCEFCORECINTERFACE_AppMain)
           = CORE::GetFunctionAddress( coreModulePtr        ,
                                       "GUCEF_CORE_AppMain" ,
                                       sizeof(int)    +
                                       sizeof(char**) +
                                       sizeof(int)          ).funcPtr; 
    cInterface.appLoadGenericPlugin = (TGUCEFCORECINTERFACE_AppLoadGenericPlugin)
           = CORE::GetFunctionAddress( coreModulePtr                     ,
                                       "GUCEF_CORE_AppLoadGenericPlugin" ,
                                       sizeof(const char*) +
                                       sizeof(int)         +
                                       sizeof(const char**)              ).funcPtr;

    if ( NULL != cInterface.appLoadConfig        &&
         NULL != cInterface.appLoadGenericPlugin &&
         NULL != cInterface.appMain               )
    {
        return true;
    }
    
    cInterface.appMain = NULL;
    cInterface.appLoadConfig = NULL;
    cInterface.appLoadGenericPlugin = NULL;
    return false;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP CORE::CString
GetPathToAppDir( const char* appName ,
                 const char* rootDir ,
                 long majorVersion   ,
                 long minorVersion   ,
                 long patchVersion   ,
                 long releaseVersion )
{
    CORE::CString pathToAppDir = rootDir;
    CORE::AppendToPath( pathToAppDir, "APPS" );

    if ( majorVersion < 0 || minorVersion < 0 )    
    {
        if ( !GetHighestVersionAvailableFromDir( pathToAppDir , 
                                                 majorVersion ,
                                                 minorVersion ) )
        {
            return CORE::CString();
        }
    }
    
    pathToAppDir += GUCEF_DIRSEPCHAR                    + 
                    CORE::Int32ToString( majorVersion ) +
                    '.'                                 +
                    CORE::Int32ToString( minorVersion ) ;
    
    if ( patchVersion < 0 || releaseVersion < 0 )    
    {
        if ( !GetHighestVersionAvailableFromDir( pathToAppDir   , 
                                                 patchVersion   ,
                                                 releaseVersion ) )
        {
            return CORE::CString();
        }
    }
    
    pathToAppDir += GUCEF_DIRSEPCHAR                      + 
                    CORE::Int32ToString( patchVersion )   +
                    '.'                                   +
                    CORE::Int32ToString( releaseVersion ) ;
                    
    return pathToAppDir;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
LoadLoaderAppConfig( const CORE::CString& appDir ,
                     TLoaderAppConfig& appConfig )
{

}

/*-------------------------------------------------------------------------*/

int
LoadAndRunGucefPlatformAppEx( const char* appName ,
                              const char* rootDir ,
                              int appArgc         ,
                              char** appArgv      ,
                              long majorVersion   ,
                              long minorVersion   ,
                              long patchVersion   ,
                              long releaseVersion )
{
    // Apps load from: <LoadRoot>/APPS/<AppName>/<MajorVersion>.<MinorVersion>/<PatchVersion>.<ReleaseVersion>
    // For the app we will check the config file and based on that we will load correct platform.
    // Platform loads from: <LoadRoot>/<Groupname>/<MajorVersion>.<MinorVersion>/<ModuleName>/<PatchVersion>.<ReleaseVersion>

    // First get the path to the app dir because we need the loader config for this app
    CORE::CString appDir = GetPathToAppDir( appName        ,
                                            rootDir        ,
                                            majorVersion   ,
                                            minorVersion   ,
                                            patchVersion   ,
                                            releaseVersion );
    // Sanity check on the path
    if ( appDir.IsNULLOrEmpty() )
    {
        return 0;
    }

    // Get the application config file
    TLoaderAppConfig loaderAppConfig;
    if ( !LoadLoaderAppConfig( appDir          ,
                               loaderAppConfig ) )
    {
        // Failed to load a loader config for this app
        // Now we dont know what to do which is a fatal error
        return 0;
    }
    
    // Try to load the platform version required for this application
    void* moduleData = LoadModules( "GUCEF"             ,
                                    loaderAppConfig.gucefMajorVersion   ,
                                    loaderAppConfig.gucefMinorVersion   ,
                                    loaderAppConfig.gucefPatchVersion   ,
                                    loaderAppConfig.gucefReleaseVersion ,
                        argv                  ,
                        argc                  ,
                        NULL      );
    
    TGucefCoreCInterface cInterface;
    if ( LinkGucefCoreCInterface( moduleData ,
                                  cInterface ) )
    {
        //cInterface.appLoadConfig( 
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

int
LoadAndRunGucefPlatformApp( const char* appName ,
                            const char* rootDir ,
                            int appArgc         ,
                            char** appArgv      )
{
    return LoadAndRunGucefPlatformApp( appName ,
                                       rootDir ,
                                       appArgc ,
                                       appArgv ,
                                       -1      ,
                                       -1      ,
                                       -1      ,
                                       -1      );
}

/*-------------------------------------------------------------------------*/
