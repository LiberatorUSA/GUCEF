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

#include <set>
#include <map>
#include <vector>

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
typedef std::vector< void* > TVoidPtrVector;

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

void*
LoadModules( const char* groupName        ,
             unsigned long mayorVersion   ,
             unsigned long minorVersion   ,
             long patchVersion            ,
             long releaseVersion          ,
             char** argv                  ,
             int argc                     )
{
    if ( NULL == groupName ) return NULL;

    // get a list of optional modules that should be loaded
    char** moduleList = NULL;
    int moduleCount = 0;
    ParseListOfExtraModulestoLoad( argv         , 
                                   argc         ,
                                   &moduleList  ,
                                   &moduleCount );
                                   
    // get the root path to where we should load modules from
    CORE::CString moduleRoot = GetModuleRootPath( argv, argc );
    CORE::AppendToPath( moduleRoot, groupName );
    
    // adjust path for desired version
    char versionDir[ 41 ];
    sprintf( versionDir, "%d.%d", mayorVersion, minorVersion );
    CORE::AppendToPath( moduleRoot, versionDir );
    
    if ( patchVersion < 0 || releaseVersion < 0 )
    {
        // We will have to search for the latest version available
        if ( !GetHighestVersionAvailableFromDir( moduleRoot, patchVersion, releaseVersion ) )
        {
            // Unable to find any versions in this dir
            return NULL;
        }
    }

    // Simply append with the specific version requested
    sprintf( versionDir, "%d.%d", patchVersion, releaseVersion );
    CORE::AppendToPath( moduleRoot, versionDir );
    
    // check if all desired modules are present
    TStringSet modulePaths;
    for ( int i=0; i<moduleCount; ++i )
    {
        CORE::CString filePath = moduleRoot;
        CORE::AppendToPath( filePath, moduleList[ i ] );
        
        if ( CORE::FileExists( filePath ) )
        {
            modulePaths.insert( filePath );
        }
        else
        {
            // We are missing one of the requested modules...
            // Abort
            return NULL;
        }
    }
    
    // load the modules into memory
    TVoidPtrVector* modulePtrs = new TVoidPtrVector();
    TStringSet::iterator n = modulePaths.begin();
    while ( n != modulePaths.end() )
    {
        void* modulePtr = CORE::LoadModuleDynamicly( (*n).C_String() );
        if ( NULL != modulePtr )  
        {
            modulePtrs->push_back( modulePtr );
        }
        else
        {
            delete modulePtrs;
            return NULL;
        }
        ++n;
    }
    
    return modulePtrs;
}

/*-------------------------------------------------------------------------*/

void
UnloadModules( void* moduleData )
{
    if ( NULL != moduleData )
    {
        TVoidPtrVector* modulePtrs = (TVoidPtrVector*) moduleData;
        TVoidPtrVector::iterator n = modulePtrs->begin();
        while ( n != modulePtrs->end() )
        {
            CORE::UnloadModuleDynamicly( (*n) );
            ++n;
        }
    }
}

/*-------------------------------------------------------------------------*/

void*
LoadGucefPlatform( unsigned long mayorVersion   ,
                   unsigned long minorVersion   ,
                   char** argv                  ,
                   int argc                     )
{

    return LoadModules( "GUCEF"      ,
                        mayorVersion ,
                        minorVersion ,
                        -1           ,
                        -1           ,
                        argv         ,
                        argc         );
}


/*-------------------------------------------------------------------------*/

void*
LoadGucefPlatformEx( unsigned long mayorVersion   ,
                     unsigned long minorVersion   ,
                     unsigned long patchVersion   ,
                     unsigned long releaseVersion ,
                     char** argv                  ,
                     int argc                     )
{

    return LoadModules( "GUCEF"               ,
                        mayorVersion          ,
                        minorVersion          ,
                        (long) patchVersion   ,
                        (long) releaseVersion ,
                        argv                  ,
                        argc                  );
}

/*-------------------------------------------------------------------------*/
