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
typedef std::vector< CORE::CString > TStringVector;

/*-------------------------------------------------------------------------*/

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
typedef std::vector< TModuleGroup > TModuleGroupVector;

/*-------------------------------------------------------------------------*/

/*
 *  The following are type definitions for the CORE module C interface functions
 */
typedef int ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_GucefMain ) ( int argc, char** argv, int runAppBool ) GUCEF_CALLSPEC_SUFFIX;
typedef int ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_GucefLoadConfig ) ( const char* configPath, const char* dataCodec ) GUCEF_CALLSPEC_SUFFIX;
typedef int ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_GucefLoadGenericPlugin ) ( const char* pluginPath, int argc, char** argv ) GUCEF_CALLSPEC_SUFFIX;
typedef void ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_GucefAddPluginDir ) ( const char* pluginDir ) GUCEF_CALLSPEC_SUFFIX;
typedef void ( GUCEF_CALLSPEC_PREFIX *TGUCEFCORECINTERFACE_GucefSetDefaultPluginLoaderLogicType ) ( const char* pluginLoaderLogicTypeName ) GUCEF_CALLSPEC_SUFFIX;

struct SGucefCoreCInterface
{
    TGUCEFCORECINTERFACE_GucefMain gucefMain;
    TGUCEFCORECINTERFACE_GucefLoadConfig gucefLoadConfig;
    TGUCEFCORECINTERFACE_GucefLoadGenericPlugin gucefLoadGenericPlugin;
    TGUCEFCORECINTERFACE_GucefAddPluginDir gucefAddPluginDir;
    TGUCEFCORECINTERFACE_GucefSetDefaultPluginLoaderLogicType gucefSetDefaultPluginLoaderLogicType;
};
typedef struct SGucefCoreCInterface TGucefCoreCInterface;

/*-------------------------------------------------------------------------*/

struct SGucefMainLoaderConfig
{
    TStringVector params;
    int runAppBool;
    bool invokeAppMain;
};
typedef struct SGucefMainLoaderConfig TGucefMainLoaderConfig;

/*-------------------------------------------------------------------------*/

/*
 *  The following is a struct containing info parsed from a loader
 *  config file. It is meant to drive the actions of the app load functions
 */
struct SLoaderAppData
{
    TModuleGroupVector modulesToLoad;
    TGucefMainLoaderConfig gucefMainConfig;
    TModuleInfo appModule;
    CORE::CString appConfigFile;
};
typedef struct SLoaderAppData TLoaderAppData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

//GUCEF_LOADER_PRIVATE_CPP void
//FindAnyParamKey( char** argv    ,
//                 int argc       ,
//                 int startIndex ,
//                 int* keyIndex  )
//{
//    int i=startIndex;
//
//    *keyIndex = -1;
//    for ( i; i<argc; ++i )
//    {
//        if ( argv[ i ][ 0 ] == '-' )
//        {
//            *keyIndex = i;
//            return;
//        }
//    }
//}
//
///*-------------------------------------------------------------------------*/
//
//GUCEF_LOADER_PRIVATE_CPP void
//FindParamKey( char** argv        ,
//              int argc           ,
//              int startIndex     ,
//              const char* keyStr ,
//              int* keyIndex      )
//{
//    int curKeyIndex = -1;
//    do
//    {
//        FindAnyParamKey( argv         ,
//                         argc         ,
//                         startIndex   ,
//                         &curKeyIndex );
//
//        if ( curKeyIndex > -1 )
//        {
//            if ( 0 == strcmp( argv[ curKeyIndex ], keyStr ) )
//            {
//                *keyIndex = curKeyIndex;
//                return;
//            }
//        }
//        startIndex = curKeyIndex+1;
//    }
//    while ( curKeyIndex  > -1 );
//}
//
///*-------------------------------------------------------------------------*/
//
//GUCEF_LOADER_PRIVATE_CPP void
//FindParam( const char* paramKey ,
//           int* paramStartIndex ,
//           int* paramEndIndex   ,
//           char** argv          ,
//           int argc             )
//{
//    int keyIndex = -1;
//
//    // Init return values
//    *paramStartIndex = -1;
//    *paramEndIndex = -1;
//
//    // First try to locate the requested key
//    FindParamKey( argv, argc, 0, paramKey, &keyIndex );
//    if ( keyIndex > -1 )
//    {
//        // Found it, now find the end delimeter which is either another key
//        // or the end of the string list
//        *paramStartIndex = keyIndex;
//        keyIndex = -1;
//
//        FindAnyParamKey( argv       ,
//                         argc       ,
//                         keyIndex+1 ,
//                         &keyIndex  );
//
//        if ( keyIndex > -1 )
//        {
//            // end delimiter is another key
//            *paramEndIndex = keyIndex;
//        }
//        else
//        {
//            // end delimiter is the end of the string set
//            *paramEndIndex = argc-1;
//        }
//    }
//}
//
///*-------------------------------------------------------------------------*/
//
//GUCEF_LOADER_PRIVATE_CPP CORE::CString
//GetModuleRootPath( char** argv ,
//                   int argc    )
//{
//    CORE::CString moduleRoot;
//    int paramStartIndex = -1;
//    int paramEndIndex = -1;
//
//    FindParam( "-moduleRoot"    ,
//               &paramStartIndex ,
//               &paramEndIndex   ,
//               argv             ,
//               argc             );
//
//    if ( ( paramStartIndex > -1 && paramEndIndex > -1 ) &&
//         ( paramStartIndex != paramEndIndex )            )
//    {
//        if ( paramStartIndex+1 < argc-1 )
//        {
//            moduleRoot = CORE::RelativePath( argv[ paramStartIndex+1 ] );
//        }
//    }
//
//    if ( moduleRoot.IsNULLOrEmpty() )
//    {
//        moduleRoot = CORE::RelativePath( "$MODULEDIR$" );
//    }
//
//    return moduleRoot;
//}
//
///*-------------------------------------------------------------------------*/
//
//GUCEF_LOADER_PRIVATE_C void
//ParseListOfExtraModulestoLoad( char** argv        ,
//                               int argc           ,
//                               char*** moduleList ,
//                               int* moduleCount   )
//{
//    int paramStartIndex = -1;
//    int paramEndIndex = -1;
//
//    FindParam( "-gucefModules",
//               &paramStartIndex ,
//               &paramEndIndex   ,
//               argv             ,
//               argc             );
//
//    if ( ( paramStartIndex > -1 && paramEndIndex > -1 ) &&
//         ( paramStartIndex != paramEndIndex > -1 )       )
//    {
//        if ( paramStartIndex+1 < argc-1 )
//        {
//            *moduleList = argv+paramStartIndex+1;
//            *moduleCount = (paramEndIndex-paramStartIndex)+1;
//            return;
//        }
//    }
//
//    *moduleList = NULL;
//    *moduleCount = 0;
//}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
GetHighestVersionAvailableFromDir( const CORE::CString& rootDir ,
                                   CORE::Int16& patchVersion    ,
                                   CORE::Int16& releaseVersion  )
{
    typedef std::map< CORE::Int16, CORE::Int16 > TVersionMap;

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
                    if ( -1 < dirName.HasChar( '.' ) )
                    {
                        CORE::Int16 dirPatchVersion = CORE::StringToInt16( dirName.SubstrToChar( '.', 0, true ) );
                        CORE::Int16 dirReleaseVersion = CORE::StringToInt16( dirName.SubstrToChar( '.', 0, false ) );

                        TVersionMap::iterator i = versionMap.find( dirPatchVersion );
                        if ( i != versionMap.end() )
                        {
                            CORE::Int16& higestFoundReleaseVersion = (*i).second;
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
IsModuleVersionAlreadyLoadedIfNotUnloadExisting( const CORE::CString& moduleGroupName ,
                                                 const CORE::CString& moduleName      ,
                                                 const CORE::TVersion& moduleVersion  ,
                                                 TModuleGroup* previousLoadData       )
{
    if ( NULL == previousLoadData ) return false;

    try
    {
        if ( previousLoadData->name == moduleGroupName )
        {
            TModuleInfoVector::iterator i = previousLoadData->modules.begin();
            while ( i != previousLoadData->modules.end() )
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
                        previousLoadData->modules.erase( i );
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

GUCEF_LOADER_PRIVATE_CPP void
UnloadModules( TModuleGroup& moduleGroup )
{
    try
    {
        TModuleInfoVector& moduleInfoVector = moduleGroup.modules;
        TModuleInfoVector::iterator n = moduleInfoVector.begin();
        while ( n != moduleInfoVector.end() )
        {
            TModuleInfo& moduleInfo = (*n);
            CORE::UnloadModuleDynamicly( moduleInfo.handle );
            moduleInfo.handle = NULL;

            ++n;
        }
    }
    catch ( ... )
    {
        // Don't allow exceptions to escape outside the loader
    }
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP void
UnloadModules( TLoaderAppData& loaderAppData )
{
    try
    {
        TModuleGroupVector::iterator i = loaderAppData.modulesToLoad.begin();
        while ( i != loaderAppData.modulesToLoad.end() )
        {
            TModuleGroup& moduleGroup = (*i);
            UnloadModules( moduleGroup );
            ++i;
        }

        CORE::UnloadModuleDynamicly( loaderAppData.appModule.handle );
        loaderAppData.appModule.handle = NULL;
    }
    catch ( ... )
    {
        // Don't allow exceptions to escape outside the loader
    }
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
LoadASingleModule( const CORE::CString& rootDir ,
                   TModuleInfo& moduleInfo      )
{
    CORE::CString filePath = rootDir;

    // adjust path for desired version
    // This gives us: <LoadRoot>/LIBS/<GroupName>/<MajorVersion>.<MinorVersion>
    char versionDir[ 41 ];
    sprintf( versionDir, "%d.%d", moduleInfo.version.major, moduleInfo.version.minor );
    CORE::AppendToPath( filePath, versionDir );

    // Add the module name to our root path
    // This gives us: <LoadRoot>/LIBS/<GroupName>/<MajorVersion>.<MinorVersion>/<ModuleName>
    CORE::AppendToPath( filePath, moduleInfo.name );

    if ( moduleInfo.version.patch < 0 || moduleInfo.version.release < 0 )
    {
        // We will have to search for the latest version available
        if ( !GetHighestVersionAvailableFromDir( filePath                   ,
                                                 moduleInfo.version.patch   ,
                                                 moduleInfo.version.release ) )
        {
            // Unable to find any versions in this dir for this module
            return false;
        }
    }

    // Simply append with the specific version requested or if no specific version was
    // requested then the highest version located.
    // This gives us: <LoadRoot>/LIBS/<GroupName>/<MajorVersion>.<MinorVersion>/<ModuleName>/<PatchVersion>.<ReleaseVersion>
    sprintf( versionDir, "%d.%d", moduleInfo.version.patch, moduleInfo.version.release );
    CORE::AppendToPath( filePath, versionDir );

    // Add the name of the module which contains the application code
    CORE::AppendToPath( filePath, moduleInfo.name );

    // Now that we constructed the full load path for this module we will try and load it
    moduleInfo.handle = CORE::LoadModuleDynamicly( filePath.C_String() );
    if ( NULL == moduleInfo.handle )
    {
        // We are missing one of the requested modules...
        // Abort
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
LoadModules( const char* rootDir       ,
             TModuleGroup& moduleGroup )
{
    try
    {
        // get the root path to where we should load modules from
        // This gives us: <LoadRoot>/LIBS/<GroupName>
        CORE::CString groupRoot = rootDir;
        CORE::AppendToPath( groupRoot, "LIBS" );
        CORE::AppendToPath( groupRoot, moduleGroup.name );

        // check if all desired modules are present
        for ( int i=0; i<(int)moduleGroup.modules.size(); ++i )
        {
            TModuleInfo& moduleInfo = moduleGroup.modules[ i ];

            if ( !LoadASingleModule( groupRoot  ,
                                     moduleInfo ) )
            {
                UnloadModules( moduleGroup );
                return false;
            }
        }

        // Return a pointer to the loaded modules which the user
        // should give back to us later
        return true;
    }
    catch ( ... )
    {
        // Don't allow exceptions to escape outside the loader
        UnloadModules( moduleGroup );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP void*
GetModulePtr( TModuleGroup& moduleGroup ,
              const char* moduleName    )
{
    try
    {
        TModuleInfoVector& moduleInfoVector = moduleGroup.modules;
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
    catch ( ... )
    {
        // Don't allow exceptions to escape outside the loader
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
LinkGucefCoreCInterface( TModuleGroup& moduleGroup        ,
                         TGucefCoreCInterface& cInterface )
{
    // grab pointer to gucefCORE module
    void* coreModulePtr = GetModulePtr( moduleGroup, "gucefCORE" );
    if ( NULL == coreModulePtr )
    {
        return false;
    }

    // Get pointers to the CORE functions we need
    // We use the C interface to be independent of class changes
    cInterface.gucefLoadConfig = (TGUCEFCORECINTERFACE_GucefLoadConfig)
             CORE::GetFunctionAddress( coreModulePtr                ,
                                       "GUCEF_CORE_GucefLoadConfig" ,
                                       sizeof(const char*)*2        ).funcPtr;
    cInterface.gucefMain = (TGUCEFCORECINTERFACE_GucefMain)
             CORE::GetFunctionAddress( coreModulePtr          ,
                                       "GUCEF_CORE_GucefMain" ,
                                       sizeof(int)    +
                                       sizeof(char**) +
                                       sizeof(int)            ).funcPtr;
    cInterface.gucefLoadGenericPlugin = (TGUCEFCORECINTERFACE_GucefLoadGenericPlugin)
             CORE::GetFunctionAddress( coreModulePtr                       ,
                                       "GUCEF_CORE_GucefLoadGenericPlugin" ,
                                       sizeof(const char*) +
                                       sizeof(int)         +
                                       sizeof(const char**)                ).funcPtr;
    cInterface.gucefAddPluginDir = (TGUCEFCORECINTERFACE_GucefAddPluginDir)
             CORE::GetFunctionAddress( coreModulePtr                       ,
                                       "GUCEF_CORE_GucefAddPluginDir" ,
                                       sizeof(const char*) +
                                       sizeof(const char**)           ).funcPtr;
    cInterface.gucefSetDefaultPluginLoaderLogicType = (TGUCEFCORECINTERFACE_GucefSetDefaultPluginLoaderLogicType)
             CORE::GetFunctionAddress( coreModulePtr                                     ,
                                       "GUCEF_CORE_GucefSetDefaultPluginLoaderLogicType" ,
                                       sizeof(const char*)                               ).funcPtr;

    if ( NULL != cInterface.gucefLoadConfig                      &&
         NULL != cInterface.gucefLoadGenericPlugin               &&
         NULL != cInterface.gucefMain                            &&
         NULL != cInterface.gucefAddPluginDir                    &&
         NULL != cInterface.gucefSetDefaultPluginLoaderLogicType )
    {
        return true;
    }

    cInterface.gucefMain = NULL;
    cInterface.gucefLoadConfig = NULL;
    cInterface.gucefLoadGenericPlugin = NULL;
    cInterface.gucefAddPluginDir = NULL;
    return false;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP CORE::CString
GetPathToAppDir( const char* appName        ,
                 const char* rootDir        ,
                 CORE::TVersion& appVersion )
{
    CORE::CString pathToAppDir = rootDir;
    CORE::AppendToPath( pathToAppDir, "APPS" );
    CORE::AppendToPath( pathToAppDir, appName );
    bool usingAutoMajorMinor = false;

    if ( appVersion.major < 0 || appVersion.minor < 0 )
    {
        usingAutoMajorMinor = true;
        if ( !GetHighestVersionAvailableFromDir( pathToAppDir     ,
                                                 appVersion.major ,
                                                 appVersion.minor ) )
        {
            return CORE::CString();
        }
    }

    pathToAppDir += GUCEF_DIRSEPCHAR                        +
                    CORE::Int16ToString( appVersion.major ) +
                    '.'                                     +
                    CORE::Int16ToString( appVersion.minor ) ;

    if ( usingAutoMajorMinor || ( appVersion.patch < 0 || appVersion.release < 0 ) )
    {
        if ( !GetHighestVersionAvailableFromDir( pathToAppDir       ,
                                                 appVersion.patch   ,
                                                 appVersion.release ) )
        {
            return CORE::CString();
        }
    }

    pathToAppDir += GUCEF_DIRSEPCHAR                          +
                    CORE::Int16ToString( appVersion.patch )   +
                    '.'                                       +
                    CORE::Int16ToString( appVersion.release ) ;

    return pathToAppDir;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
ParseVersion( const CORE::CString& versionString ,
              CORE::Int16& versionMajor          ,
              CORE::Int16& versionMinor          ,
              CORE::Int16& versionPatch          ,
              CORE::Int16& versionRelease        )
{
    TStringVector versionSegments = versionString.ParseElements( '.', false );
    if ( versionSegments.size() >= 2 )
    {
        versionMajor = CORE::StringToUInt16( versionSegments[ 0 ] );
        versionMinor = CORE::StringToUInt16( versionSegments[ 1 ] );
    }
    else
    {
        // Corrupt version string
        return false;
    }

    // Check for a more granular version definition (optional)
    if ( versionSegments.size() >= 4 )
    {
        versionPatch = CORE::StringToInt16( versionSegments[ 2 ] );
        versionRelease = CORE::StringToInt16( versionSegments[ 3 ] );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP void
ParseModulesToLoad( const CORE::CString& commandText ,
                    TLoaderAppData& appConfig        )
{
    TStringVector cmdParams = commandText.ParseElements( ' ', false );
    TModuleGroup moduleGroup;

    // The first param is the name of the module group
    TStringVector::iterator i = cmdParams.begin();
    if ( i != cmdParams.end() )
    {
        moduleGroup.name = (*i);
        ++i;
    }
    else
    {
        // Invalid command, this is a mandatory param
        return;
    }

    // The second param is the version of the module group in the
    // format <majorVersion>.<minorVersion>
    CORE::Int16 groupVersionMajor = -1;
    CORE::Int16 groupVersionMinor = -1;
    CORE::Int16 groupVersionPatch = -1;
    CORE::Int16 groupVersionRelease = -1;
    if ( i != cmdParams.end() )
    {
        if ( !ParseVersion( (*i)                ,
                            groupVersionMajor   ,
                            groupVersionMinor   ,
                            groupVersionPatch   ,
                            groupVersionRelease ) )
        {
            // Invalid version param
            return;
        }
        ++i;
    }
    else
    {
        // Invalid command, this is a mandatory param
        return;
    }

    while ( i != cmdParams.end() )
    {
        // the remaining segments are all module names and optionally also
        // a specific version for that module
        CORE::CString& moduleString = (*i);

        // Check if the module string has a version attached to it
        if ( ')' == moduleString[ moduleString.Length()-1 ] )
        {
            // Seperate the version string from the module name
            CORE::CString versionString = moduleString.SubstrToChar( '(', moduleString.Length()-2, false );
            CORE::CString moduleName = moduleString.CutChars( versionString.Length()+2, false );

            // Further parse the versions string
            CORE::Int16 moduleVersionPatch = -1;
            CORE::Int16 moduleVersionRelease = -1;
            CORE::Int16 dummy = -1;
            if ( !ParseVersion( versionString        ,
                                moduleVersionPatch   ,
                                moduleVersionRelease ,
                                dummy                ,
                                dummy                ) )
            {
                // Invalid version param
                return;
            }

            // Define the module
            TModuleInfo moduleInfo;
            moduleInfo.handle = NULL;
            moduleInfo.name = moduleName;
            moduleInfo.version.major = (CORE::UInt16) groupVersionMajor;
            moduleInfo.version.minor = (CORE::UInt16) groupVersionMinor;
            moduleInfo.version.patch = moduleVersionPatch > -1 ? moduleVersionPatch : (CORE::UInt16) groupVersionPatch;
            moduleInfo.version.release = moduleVersionRelease > -1 ? moduleVersionRelease : (CORE::UInt16) groupVersionRelease;
            moduleGroup.modules.push_back( moduleInfo );
        }
        else
        {
            // No version suffix. The module string is the module name
            TModuleInfo moduleInfo;
            moduleInfo.handle = NULL;
            moduleInfo.name = moduleString;
            moduleInfo.version.major = (CORE::UInt16) groupVersionMajor;
            moduleInfo.version.minor = (CORE::UInt16) groupVersionMinor;
            moduleInfo.version.patch = (CORE::UInt16) groupVersionPatch;
            moduleInfo.version.release = (CORE::UInt16) groupVersionRelease;
            moduleGroup.modules.push_back( moduleInfo );
        }
        ++i;
    }

    appConfig.modulesToLoad.push_back( moduleGroup );
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP void
ParseGucefMainCommand( const CORE::CString& commandText ,
                       TLoaderAppData& appConfig        )
{
    // The first item is a boolean telling us whether to run the app's main loop
    // Most apps will use 'true' for this although some may wish to execute via the
    // load of the app module itself
    TStringVector cmdParams = commandText.ParseElements( ' ', false );
    TStringVector::iterator i = cmdParams.begin();
    if ( i != cmdParams.end() )
    {
        appConfig.gucefMainConfig.runAppBool = CORE::StringToBool( (*i) ) ? 1 : 0;
        appConfig.gucefMainConfig.invokeAppMain = true;
        ++i;
    }
    else
    {
        // Invalid command, we need the first param always
        appConfig.gucefMainConfig.runAppBool = false;
        appConfig.gucefMainConfig.invokeAppMain = false;
        appConfig.gucefMainConfig.params.clear();
        return;
    }

    // The next params (if any) are the params that will be passed to the app main
    while ( i != cmdParams.end() )
    {
        appConfig.gucefMainConfig.params.push_back( *i );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP void
ParseAppConfigCommand( const CORE::CString& commandText ,
                       TLoaderAppData& appConfig        )
{
    appConfig.appConfigFile = commandText;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
LoadLoaderAppConfig( const CORE::CString& appDir ,
                     TLoaderAppData& appConfig   )
{
    CORE::CString appLoaderConfigPath = appDir;
    CORE::AppendToPath( appLoaderConfigPath, "LoaderConfig.txt" );

    // Load the file as a string into memory
    CORE::CString configFile;
    if ( CORE::LoadTextFileAsString( appLoaderConfigPath, configFile, true, "\n" ) )
    {
        // Seperate the different lines for easy processing
        TStringVector lines = configFile.ParseElements( '\n', false );
        configFile.Clear();

        TStringVector::iterator i = lines.begin();
        while ( i != lines.end() )
        {
            const CORE::CString& lineText = (*i);
            CORE::CString instructionTag = lineText.SubstrToChar( ' ', true );

            if ( instructionTag == "APPMODULE" )
            {
                appConfig.appModule.version.major = -1;
                appConfig.appModule.version.minor = -1;
                appConfig.appModule.version.patch = -1;
                appConfig.appModule.version.release = -1;
                appConfig.appModule.handle = NULL;
                appConfig.appModule.name = lineText.CutChars( 10, true );
            }
            else
            if ( instructionTag == "LOADMODULES" )
            {
                ParseModulesToLoad( lineText.CutChars( 12, true ) ,
                                    appConfig                     );
            }
            else
            if ( instructionTag == "APPCONFIG" )
            {
                ParseAppConfigCommand( lineText.CutChars( 10, true ) ,
                                       appConfig                     );
            }
            else
            if ( instructionTag == "GUCEFMAIN" )
            {
                ParseGucefMainCommand( lineText.CutChars( 10, true ) ,
                                       appConfig                     );
            }
            ++i;
        }
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP TModuleGroup*
GetModuleGroup( TLoaderAppData* appLoaderData  ,
                const CORE::CString& groupName )
{
    if ( NULL == appLoaderData ) return NULL;

    TModuleGroupVector::iterator i = appLoaderData->modulesToLoad.begin();
    while ( i != appLoaderData->modulesToLoad.end() )
    {
        if ( (*i).name == groupName )
        {
            return &(*i);
        }
        ++i;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
LoadMultipleModuleGroups( const char* rootDir              ,
                          TLoaderAppData& appLoaderData    ,
                          TLoaderAppData* previousLoadData )
{
    // Load the groups in the order they are specified in the list
    TModuleGroupVector::iterator i = appLoaderData.modulesToLoad.begin();
    while ( i != appLoaderData.modulesToLoad.end() )
    {
        TModuleGroup& moduleGroup = (*i);
        TModuleGroup* previousLoadGroup = GetModuleGroup( previousLoadData, moduleGroup.name );

        if ( !LoadModules( rootDir, moduleGroup ) )
        {
            // Failure during the load of one of the prereq module groups
            return false;
        }
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
LoadApplicationModule( TLoaderAppData& loaderAppData    ,
                       const CORE::CString& appDir      ,
                       TGucefCoreCInterface& cInterface ,
                       int appArgc                      ,
                       char** appArgv                   )
{
    try
    {
        CORE::CString appModulePath = appDir;
        CORE::AppendToPath( appModulePath, loaderAppData.appModule.name );
        loaderAppData.appModule.handle = NULL;

        return 0 != cInterface.gucefLoadGenericPlugin( appModulePath.C_String() ,
                                                       appArgc                  ,
                                                       appArgv                  );
    }
    catch ( ... )
    {
        return false;
    }
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP void
InitLoaderAppData( TLoaderAppData& data )
{
    data.appModule.handle = NULL;
    data.appModule.version.major = -1;
    data.appModule.version.minor = -1;
    data.appModule.version.patch = -1;
    data.appModule.version.release = -1;
    data.appModule.name.Clear();
    data.modulesToLoad.clear();
    data.gucefMainConfig.invokeAppMain = false;
    data.gucefMainConfig.runAppBool = false;
    data.gucefMainConfig.params.clear();
}

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PRIVATE_CPP bool
ProcessFirstTimeInitialization( const CORE::CString& resRootDir ,
                                const CORE::CString& rootDir    )
{
    CORE::CString firstRunFilePath = resRootDir + "firstrun.txt";
    if ( CORE::FileExists( firstRunFilePath ) )
    {
        CORE::CString firstRunFile;
        if ( CORE::LoadTextFileAsString( firstRunFilePath, firstRunFile, true, "\n" ) )
        {
            // Seperate the different lines for easy processing
            TStringVector lines = firstRunFile.ParseElements( '\n', false );
            firstRunFile.Clear();

            TStringVector::iterator i = lines.begin();
            while ( i != lines.end() )
            {
                const CORE::CString& lineText = (*i);
                CORE::CString instructionTag = lineText.SubstrToChar( ' ', true );

                if ( instructionTag == "MOVEFILE" )
                {
                    CORE::CString remainder = lineText.CutChars( 9, true );
                    CORE::CString srcPath = rootDir;
                    CORE::AppendToPath( srcPath, remainder.SubstrToChar( ' ', true ) );
                    CORE::CString destPath = rootDir;
                    CORE::AppendToPath( destPath, remainder.CutChars( srcPath.Length()+1, true ) );

                    if ( 0 == CORE::Move_File( destPath.C_String(), srcPath.C_String() ) )
                    {
                        // Failed to execute command
                        return false;
                    }
                }
                else
                if ( instructionTag == "COPYFILE" )
                {
                    CORE::CString remainder = lineText.CutChars( 9, true );
                    CORE::CString srcPath = rootDir;
                    CORE::AppendToPath( srcPath, remainder.SubstrToChar( ' ', true ) );
                    CORE::CString destPath = rootDir;
                    CORE::AppendToPath( destPath, remainder.CutChars( srcPath.Length()+1, true ) );

                    if ( 0 == CORE::Copy_File( destPath.C_String(), srcPath.C_String() ) )
                    {
                        // Failed to execute command
                        return false;
                    }
                }
                ++i;
            }

            // Now that we have successfully carried out all the actions we can rename the file
            // This keeps the file for later review but it wont trigger another initialization
            CORE::CString afterFirstRunFilePath = resRootDir + "firstrun.completed.txt";
            CORE::Delete_File( afterFirstRunFilePath.C_String() );
            if ( 0 == CORE::Move_File( afterFirstRunFilePath.C_String(), firstRunFilePath.C_String() ) )
            {
                return false;
            }
            return true;

        }

        // Failed to load the first run file even though it exists
        return false;
    }

    // Nothing to do, success
    return true;
}

/*-------------------------------------------------------------------------*/

int
LoadAndRunGucefPlatformAppEx( const char* appName    ,
                              const char* rootDir    ,
                              const char* resRootDir ,
                              const char* libRootDir ,
                              int platformArgc       ,
                              char** platformArgv    ,
                              int appArgc            ,
                              char** appArgv         ,
                              long majorVersion      ,
                              long minorVersion      ,
                              long patchVersion      ,
                              long releaseVersion    )
{
    // Apps load from: <LoadRoot>/APPS/<AppName>/<MajorVersion>.<MinorVersion>/<PatchVersion>.<ReleaseVersion>
    // For the app we will check the config file and based on that we will load correct platform.
    // Platform loads from: <LoadRoot>/LIBS/<Groupname>/<MajorVersion>.<MinorVersion>/<ModuleName>/<PatchVersion>.<ReleaseVersion>

    // If the resources root dir is NULL then we will use the generic root dir as the root for resources as well
    if ( NULL == resRootDir )
    {
        resRootDir = rootDir;
    }

    // If the library root dir is NULL then we will use the resource root dir as the root for libraries as well
    if ( NULL == libRootDir )
    {
        libRootDir = resRootDir;
    }

    // Process first time initialization (if needed)
    if ( !ProcessFirstTimeInitialization( resRootDir ,
                                          rootDir    ) )
    {
        // Abort: failed to run first time init
        return -1;
    }

    // First get the path to the app dir because we need the loader config for this app
    CORE::TVersion appVersion;
    appVersion.major = (CORE::Int16) majorVersion;
    appVersion.minor = (CORE::Int16) minorVersion;
    appVersion.patch = (CORE::Int16) patchVersion;
    appVersion.release = (CORE::Int16) releaseVersion;
    CORE::CString appDir = GetPathToAppDir( appName    ,
                                            libRootDir ,
                                            appVersion );
    // Sanity check on the path
    if ( appDir.IsNULLOrEmpty() )
    {
        return 0;
    }

    // Get the application config file
    TLoaderAppData loaderAppData;
    InitLoaderAppData( loaderAppData );
    if ( !LoadLoaderAppConfig( appDir        ,
                               loaderAppData ) )
    {
        // Failed to load a loader config for this app
        // Now we dont know what to do which is a fatal error
        return 0;
    }

    // First try to load the platform version required for this application
    // Modules are loaded in the order they are written in the config
    if ( !LoadMultipleModuleGroups( libRootDir, loaderAppData, NULL ) )
    {
        // Failed to load prereq modules for this app
        return 0;
    }

    TModuleGroup* coreModuleGroup = GetModuleGroup( &loaderAppData, "GUCEF" );
    if ( NULL == coreModuleGroup )
    {
        // Since this is a GUCEF App loader we don't support apps not using GUCEF
        UnloadModules( loaderAppData );
        return 0;
    }

    // Link the GUCEF::CORE C interface so we can invoke functions
    // as specified by the loader app config
    TGucefCoreCInterface cInterface;
    if ( !LinkGucefCoreCInterface( *coreModuleGroup ,
                                   cInterface       ) )
    {
        UnloadModules( loaderAppData );
        return 0;
    }

    // Before we load anything else we want to make sure that the application can load plugins on its own
    // In order to do this in cases where the plugins are not stored with the other app modules we have to
    // make the gucefLOADER load logic available to the rest of the app. Basically we make it possible to route
    // plugin load calls trough this module for correct path lookup. This works because the gucefCORE module
    // is aware of this module and will work with it to complete the circuit and link the logic transparently.
    // The advantage here is that you have a central loader which is easy to upgrade without making it harder on
    // C++ interface based modules.
    cInterface.gucefSetDefaultPluginLoaderLogicType( "gucefLOADER" );
    cInterface.gucefAddPluginDir( rootDir );

    // Now that the correct prereq modules are loaded we can load the application
    // module itself
    if ( !LoadApplicationModule( loaderAppData ,
                                 appName       ,
                                 cInterface    ,
                                 appArgc       ,
                                 appArgv       ) )
    {
        // There was an error loading the application module
        UnloadModules( loaderAppData );
        return 0;
    }

    // Now load the app config if so desired



    // Now invoke the Application class main if so desired
    if ( loaderAppData.gucefMainConfig.invokeAppMain )
    {
        // Add the paths as params to main
        loaderAppData.gucefMainConfig.params.push_back( "LOADERAPPDIR=" + appDir );
        loaderAppData.gucefMainConfig.params.push_back( "LOADERLIBDIR=" + CORE::CString( libRootDir ) );
        loaderAppData.gucefMainConfig.params.push_back( "LOADERRESDIR=" + CORE::CString( resRootDir ) );
        loaderAppData.gucefMainConfig.params.push_back( "LOADERROOTDIR=" + CORE::CString( resRootDir ) );

        // Make a C style param list
        // We combine the param list given to this function with the params
        // we fetch from the app's loader config. This way we offer multiple ways
        // of passing application argument information
        int paramCount = (int) loaderAppData.gucefMainConfig.params.size();
        char** paramArray = new char*[ paramCount + platformArgc ];
        int i=0;
        for ( i; i<platformArgc; ++i )
        {
            paramArray[ i ] = platformArgv[ i ];
            ++i;
        }
        int n=i;
        for ( i=0; i<(int)loaderAppData.gucefMainConfig.params.size(); ++i )
        {
            paramArray[ n ] = loaderAppData.gucefMainConfig.params[ i ].C_String();
            ++i; ++n;
        }

        int mainReturnValue = cInterface.gucefMain( paramCount                                       ,
                                                    paramArray                                       ,
                                                    loaderAppData.gucefMainConfig.runAppBool ? 1 : 0 );

        delete []paramArray;
        paramArray = NULL;

        return mainReturnValue;
    }

    return 0;
}

/*-------------------------------------------------------------------------*/

int
LoadAndRunGucefPlatformApp( const char* appName    ,
                            const char* rootDir    ,
                            const char* resRootDir ,
                            const char* libRootDir ,
                            int platformArgc       ,
                            char** platformArgv    ,
                            int appArgc            ,
                            char** appArgv         )
{
    return LoadAndRunGucefPlatformAppEx( appName      ,
                                         rootDir      ,
                                         resRootDir   ,
                                         libRootDir   ,
                                         platformArgc ,
                                         platformArgv ,
                                         appArgc      ,
                                         appArgv      ,
                                         -1           ,
                                         -1           ,
                                         -1           ,
                                         -1           );
}

/*-------------------------------------------------------------------------*/

void
UnloadSpecificModule( void* modulePtr )
{
    // We don't do anything special here since we don't keep state
    CORE::UnloadModuleDynamicly( modulePtr );
}

/*-------------------------------------------------------------------------*/

void*
LoadSpecificModule( const char* moduleFilename  ,
                    const char* rootDir         ,
                    const char* moduleGroupName ,
                    long majorVersion           ,
                    long minorVersion           ,
                    long patchVersion           ,
                    long releaseVersion         )
{
    TModuleInfo moduleInfo;
    moduleInfo.name = moduleFilename;
    moduleInfo.version.major = (CORE::Int16) majorVersion;
    moduleInfo.version.minor = (CORE::Int16) minorVersion;
    moduleInfo.version.patch = (CORE::Int16) patchVersion;
    moduleInfo.version.release = (CORE::Int16) releaseVersion;
    moduleInfo.handle = NULL;

    TModuleGroup moduleGroup;
    moduleGroup.modules.push_back( moduleInfo );
    moduleGroup.name = moduleGroupName;

    if ( LoadModules( rootDir     ,
                      moduleGroup ) )
    {
        return GetModulePtr( moduleGroup    ,
                             moduleFilename );
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/
