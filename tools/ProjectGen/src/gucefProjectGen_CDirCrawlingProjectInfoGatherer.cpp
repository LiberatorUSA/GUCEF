/*
 *  ProjectGenerator: Tool to generate module/project files
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#include "CDStoreCodecPluginManager.h"
#define GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_PROJECTGEN_CPROJECTGENGLOBAL_H
#include "gucefProjectGen_CProjectGenGlobal.h"
#define GUCEF_PROJECTGEN_CPROJECTGENGLOBAL_H
#endif /* GUCEF_PROJECTGEN_CPROJECTGENGLOBAL_H ? */

#ifndef GUCEF_PROJECTGEN_CDIRPREPROCESSORMANAGER_H
#include "gucefProjectGen_CDirPreprocessorManager.h"
#define GUCEF_PROJECTGEN_CDIRPREPROCESSORMANAGER_H
#endif /* GUCEF_PROJECTGEN_CDIRPREPROCESSORMANAGER_H ? */

#include "gucefProjectGen_CDirCrawlingProjectInfoGatherer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PROJECTGEN {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static const CORE::CString AllPlatforms = "all";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static const TStringSetMap&
GetSupportedPlatformDirMap( void )
{GUCEF_TRACE;

    static TStringSetMap platformMap;
    if ( platformMap.empty() )
    {
        platformMap[ "win32" ].insert( "mswin" );
        platformMap[ "win32" ].insert( "win32" );
        platformMap[ "win64" ].insert( "mswin" );
        platformMap[ "win64" ].insert( "win64" );
        platformMap[ "linux" ].insert( "linux" );
        platformMap[ "unix" ].insert( "unix" );
        platformMap[ "iphoneos" ].insert( "iphone" );
        platformMap[ "symbian" ].insert( "symbian" );
        platformMap[ "osx" ].insert( "osx" );
        platformMap[ "osx" ].insert( "mac" );
        platformMap[ "android" ].insert( "android" );
        platformMap[ "glx" ].insert( "glx" );
        platformMap[ "gtk" ].insert( "gtk" );
        platformMap[ "sdl" ].insert( "sdl" );
    }
    return platformMap;
}

/*---------------------------------------------------------------------------*/

const TStringVector&
GetSourceFileExtensions( void )
{GUCEF_TRACE;

    static TStringVector fileTypes;

    if ( fileTypes.empty() )
    {
        fileTypes.push_back( "c" );
        fileTypes.push_back( "cc" );
        fileTypes.push_back( "cpp" );
        fileTypes.push_back( "cxx" );
        fileTypes.push_back( "asm" );
    }
    return fileTypes;
}

/*--------------------------------------------------------------------------*/

const TStringVector&
GetHeaderFileExtensions( void )
{GUCEF_TRACE;

    static TStringVector fileTypes;

    if ( fileTypes.empty() )
    {
        fileTypes.push_back( "h" );
        fileTypes.push_back( "hpp" );
    }
    return fileTypes;
}

/*---------------------------------------------------------------------------*/

static const TStringSet&
GetSupportedPlatforms( void )
{GUCEF_TRACE;

    static TStringSet platforms;
    if ( platforms.empty() )
    {
        const TStringSetMap& dirMap = GetSupportedPlatformDirMap();
        TStringSetMap::const_iterator i = dirMap.begin();
        while ( i != dirMap.end() )
        {
            platforms.insert( (*i).first );
            ++i;
        }
    }
    return platforms;
}

/*---------------------------------------------------------------------------*/

const TStringSet&
GetSupportedPlatformDirs()
{GUCEF_TRACE;

    static TStringSet platformDirs;
    if ( platformDirs.empty() )
    {
        const TStringSetMap& dirMap = GetSupportedPlatformDirMap();
        TStringSetMap::const_iterator i = dirMap.begin();
        while ( i != dirMap.end() )
        {
            const TStringSet& dirs = (*i).second;
            TStringSet::const_iterator n = dirs.begin();
            while ( n != dirs.end() )
            {
                platformDirs.insert( (*n) );
                ++n;
            }
            ++i;
        }
    }
    return platformDirs;
}

/*---------------------------------------------------------------------------*/

bool
IsDirAPlatformDir( const CORE::CString& path )
{GUCEF_TRACE;

    CORE::CString lastSubDir = CORE::LastSubDir( path ).Lowercase();
    const TStringSet& supportedPlatformDirs = GetSupportedPlatformDirs();
    return supportedPlatformDirs.end() != supportedPlatformDirs.find( lastSubDir );
}

/*---------------------------------------------------------------------------*/

bool
IsDirAPlatformDirForPlatform( const CORE::CString& path     ,
                              const CORE::CString& platform )
{GUCEF_TRACE;

    CORE::CString lastSubDir = CORE::LastSubDir( path ).Lowercase();
    const TStringSetMap& platformDirMap = GetSupportedPlatformDirMap();
    TStringSetMap::const_iterator i = platformDirMap.find( platform );
    if ( i != platformDirMap.end() )
    {
        const TStringSet& dirsForPlatform = (*i).second;
        TStringSet::const_iterator n = dirsForPlatform.find( lastSubDir.Lowercase() );
        if ( n != dirsForPlatform.end() )
        {
            return true;
        }        
    }
    return false;
}

/*--------------------------------------------------------------------------*/

bool
RemoveString( TStringVector& list            ,
              const CORE::CString& searchStr )
{GUCEF_TRACE;

    //@TODO: make wildcard processing more advanced then this :)
    if ( searchStr == "*" )
    {
        list.clear();
        return true;
    }

    bool removedString = false;
    TStringVector::iterator i = list.begin();
    while ( i != list.end() )
    {
        if ( (*i) == searchStr )
        {
            list.erase( i );
            i = list.begin();
            removedString = true;
        }
        else
        {
            ++i;
        }
    }
    return removedString;
}

/*---------------------------------------------------------------------------*/

static bool
IsStringInList( const TStringVector& list       ,
                bool caseSensitive              ,
                const CORE::CString& testString ,
                bool wildcardMatching = false   )
{GUCEF_TRACE;

    TStringVector::const_iterator i = list.begin();
    while ( i != list.end() )
    {
        if ( (*i).Equals( testString, caseSensitive ) )
        {
            return true;
        }
        if ( wildcardMatching )
        {
            if ( testString.WildcardEquals( (*i), '*', caseSensitive ) )
            {
                return true;
            }
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

TModuleInfoEntry*
FindModuleAccordingToBuildOrderImp( TProjectInfo& projectInfo           ,
                                    const CORE::CString& targetPlatform ,
                                    int buildOrderIndex                 )
{GUCEF_TRACE;

    TModuleInfoEntryVector::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfoEntry& moduleEntry = (*i);
        
        // Check to see if we have an entry for this platform
        TModuleInfoMap::iterator n = moduleEntry.modulesPerPlatform.find( targetPlatform );
        if ( n != moduleEntry.modulesPerPlatform.end() )
        {
            // Check to see if the entry has a platform specific build order
            TModuleInfo& info = (*n).second;
            if ( buildOrderIndex == info.buildOrder )
            {
                return &(*i);
            }
        }
        ++i;
    }
    
    return NULL;
}

/*-------------------------------------------------------------------------*/

TModuleInfoEntry*
FindFirstModuleAccordingToBuildOrder( TProjectInfo& projectInfo           ,
                                      const CORE::CString& targetPlatform )
{GUCEF_TRACE;

    TModuleInfoEntry* platformEntry = FindModuleAccordingToBuildOrderImp( projectInfo    ,
                                                                          targetPlatform , 
                                                                          0              );
    if ( NULL == platformEntry && targetPlatform != AllPlatforms )
    {
        // If we get here we did not find a module with build order 0
        // instead of a platform specific build order check the AllPlatforms build order
        platformEntry = FindModuleAccordingToBuildOrderImp( projectInfo  ,
                                                            AllPlatforms , 
                                                            0            );        
    }
   
    return platformEntry;
}

/*-------------------------------------------------------------------------*/

int
GetModuleBuildOrder( const TModuleInfoEntry& moduleEntry , 
                     const CORE::CString& targetPlatform )
{
    // Check to see if we have an entry for this platform
    TModuleInfoMap::const_iterator n = moduleEntry.modulesPerPlatform.find( targetPlatform );
    if ( n != moduleEntry.modulesPerPlatform.end() )
    {
        // Check to see if the entry has a platform specific build order
        const TModuleInfo& info = (*n).second;
        if ( -1 != info.buildOrder )
        {
            return info.buildOrder;
        }
    }
    
    if ( targetPlatform != AllPlatforms )
    {
        TModuleInfoMap::const_iterator n = moduleEntry.modulesPerPlatform.find( AllPlatforms );
        if ( n != moduleEntry.modulesPerPlatform.end() )
        {
            // Check to see if the entry has a platform specific build order
            const TModuleInfo& info = (*n).second;
            if ( -1 != info.buildOrder )
            {
                return info.buildOrder;
            }
        }
    }
    
    return -1;
}

/*-------------------------------------------------------------------------*/

TModuleInfoEntry*
FindNextModuleAccordingToBuildOrder( TProjectInfo& projectInfo            ,
                                     TModuleInfoEntry& currentModuleEntry ,
                                     const CORE::CString& targetPlatform  ,
                                     int desiredBuildOrder = -1           )
{GUCEF_TRACE;

    if ( desiredBuildOrder == -1 )
    {
        desiredBuildOrder = GetModuleBuildOrder( currentModuleEntry, targetPlatform ) + 1;
    }
    
    TModuleInfoEntry* platformEntry = FindModuleAccordingToBuildOrderImp( projectInfo       ,
                                                                          targetPlatform    , 
                                                                          desiredBuildOrder );
    if ( NULL == platformEntry && targetPlatform != AllPlatforms )
    {
        // If we get here we did not find a module with build order 0
        // instead of a platform specific build order check the AllPlatforms build order
        platformEntry = FindModuleAccordingToBuildOrderImp( projectInfo       ,
                                                            AllPlatforms      , 
                                                            desiredBuildOrder );        
    }
   
    return platformEntry;        
}

/*---------------------------------------------------------------------------*/

void
FilterStringVectorForFilesWithExtensions( TStringVector& outputVector         ,
                                          const TStringVector& extensions     ,
                                          const TStringVector& originalVector )
{GUCEF_TRACE;

    TStringVector::const_iterator i = originalVector.begin();
    while ( i != originalVector.end() )
    {
        CORE::CString foundExtension = CORE::ExtractFileExtention( (*i) );
        if ( IsStringInList( extensions, false, foundExtension ) )
        {
            outputVector.push_back( (*i) );
        }
        ++i;
    }
}
/*---------------------------------------------------------------------------*/

bool
IsDirALegacyProjectDir( const CORE::CString& dir )
{GUCEF_TRACE;

    // The dir is a module dir if it has a suffix file in it
    CORE::CString suffixFilePath = dir;
    CORE::AppendToPath( suffixFilePath, "CMakeListsSuffix.txt" );

    return CORE::FileExists( suffixFilePath );
}

/*---------------------------------------------------------------------------*/

bool
IsDirAProjectDir( const CORE::CString& dir )
{GUCEF_TRACE;

    // The dir is a module dir if it has a suffix file in it
    CORE::CString moduleInfoFilePath = dir;
    CORE::AppendToPath( moduleInfoFilePath, "ModuleInfo.xml" );

    if ( !CORE::FileExists( moduleInfoFilePath ) )
    {
        return IsDirALegacyProjectDir( dir );
    }
    return true;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetProcessingInstructionsPath( const CORE::CString& dir )
{GUCEF_TRACE;

    CORE::CString instructionsFile = dir;
    CORE::AppendToPath( instructionsFile, "ProjectGenInstructions.xml" );
    return instructionsFile;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetLegacyProcessingInstructionsPath( const CORE::CString& dir )
{GUCEF_TRACE;

    CORE::CString instructionsFile = dir;
    CORE::AppendToPath( instructionsFile, "CMakeGenInstructions.xml" );
    return instructionsFile;
}

/*---------------------------------------------------------------------------*/

bool
GetProcessingInstructions( const CORE::CString& dir      ,
                           CORE::CDataNode& instructions )
{GUCEF_TRACE;

    CORE::CString instructionsFile = GetProcessingInstructionsPath( dir );
    if ( !CORE::FileExists( instructionsFile ) )
    {
        instructionsFile = GetLegacyProcessingInstructionsPath( dir );
        if ( !CORE::FileExists( instructionsFile ) )
        {
            return false;
        }
    }
    
    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = GetXmlDStoreCodec();
    if ( !codecPtr.IsNULL() )
    {
        if ( codecPtr->BuildDataTree( &instructions    ,
                                      instructionsFile ) )
        {
            return true;
        }
    }
    return false;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetExcludeListPath( const CORE::CString& dir )
{GUCEF_TRACE;                            

    CORE::CString excludeFile = dir;
    CORE::AppendToPath( excludeFile, "ProjectGenExcludeList.txt" );
    return excludeFile;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetLegacyExcludeListPath( const CORE::CString& dir )
{GUCEF_TRACE;

    CORE::CString excludeFile = dir;
    CORE::AppendToPath( excludeFile, "CMakeGenExcludeList.txt" );
    return excludeFile;
}

/*---------------------------------------------------------------------------*/

TStringVector
GetExcludeList( const CORE::CString& dir )
{GUCEF_TRACE;

    CORE::CString excludeFile = GetExcludeListPath( dir );
    if ( !CORE::FileExists( excludeFile ) )    
    {
        // try legacy filename
        excludeFile = GetLegacyExcludeListPath( dir );
        if ( !CORE::FileExists( excludeFile ) )
        {
            return TStringVector();
        }
    }

    CORE::CString excludeFileContent;
    if ( CORE::LoadTextFileAsString( excludeFile        ,
                                     excludeFileContent ) )
    {
        excludeFileContent = excludeFileContent.RemoveChar( '\r' );

        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Loaded simple exclude list for directory \"" + dir + "\"" );
        return excludeFileContent.ParseElements( '\n' );
    }
    return TStringVector();
}

/*---------------------------------------------------------------------------*/

void
ExcludeOrIncludeEntriesAsSpecifiedForDir( const TDirProcessingInstructions& allInstructions ,
                                          const CORE::CString& platform                     ,
                                          TStringVector& allEntries                         )
{GUCEF_TRACE;
        
    // see if we have info for this platform
    TStringVectorMap::const_iterator q = allInstructions.dirExcludeList.find( platform );
    if ( q != allInstructions.dirExcludeList.end() )
    {
        // apply changes 
        const TStringVector& excludeList = (*q).second;
        TStringVector::const_iterator n = excludeList.begin();
        while ( n != excludeList.end() )
        {
            if ( RemoveString( allEntries, (*n) ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Excluded the directory \"" + (*n) + "\" based on the exclude list for this dir" );
            }
            ++n;
        }
    }

    // see if we have info for this platform
    q = allInstructions.dirIncludeList.find( platform );
    if ( q != allInstructions.dirIncludeList.end() )
    {
        // apply changes
        const TStringVector& includeList = (*q).second;
        TStringVector::const_iterator n = includeList.begin();
        while ( n != includeList.end() )
        {
            allEntries.push_back( (*n) );
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Included the directory \"" + (*n) + "\" based on the include list for this dir" );
            ++n;
        }
    }
}

/*---------------------------------------------------------------------------*/

void
ExcludeEntriesAsSpecifiedForFile( const TDirProcessingInstructions& allInstructions ,
                                  const CORE::CString& platform                     ,
                                  TStringVector& allEntries                         )
{GUCEF_TRACE;

    // see if we have info for this platform
    TStringVectorMap::const_iterator q = allInstructions.fileExcludeList.find( platform );
    if ( q != allInstructions.fileExcludeList.end() )
    {
        // apply changes
        const TStringVector& excludeList = (*q).second;
        TStringVector::const_iterator n = excludeList.begin();
        while ( n != excludeList.end() )
        {
            if ( RemoveString( allEntries, (*n) ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Excluded the file \"" + (*n) + "\" based on the exclude list for this dir" );
            }
            ++n;
        }
    }
}

/*---------------------------------------------------------------------------*/

void
IncludeEntriesAsSpecifiedForFile( const TDirProcessingInstructions& allInstructions ,
                                  const CORE::CString& platform                     ,
                                  const CORE::CString& currentPath                  ,
                                  TStringVectorMap& allEntries                      ,
                                  const TStringVector& fileTypes                    )
{GUCEF_TRACE;
    
    // see if we have info for this platform
    TStringVectorMap::const_iterator q = allInstructions.fileIncludeList.find( platform );
    if ( q != allInstructions.fileIncludeList.end() )
    {
        // apply changes
        const TStringVector& includeList = (*q).second;
        TStringVector::const_iterator n = includeList.begin();
        while ( n != includeList.end() )
        {
            const CORE::CString& fullEntry = (*n);
            CORE::CString fileExt = CORE::ExtractFileExtention( fullEntry );
            
            if ( IsStringInList( fileTypes, false, fileExt ) )
            {
                CORE::CString filename = CORE::ExtractFilename( fullEntry );
                CORE::CString path = currentPath;
                CORE::AppendToPath( path, fullEntry.CutChars( filename.Length(), false ) );
                path = CORE::RelativePath( path );
                path = path.ReplaceChar( '\\', '/' );
            
                allEntries[ path ].push_back( filename );
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Included the file \"" + filename + "\" based on the include list for this dir using path " + path );
            }
            ++n;
        }
    }
}

/*---------------------------------------------------------------------------*/

void
ExcludeOrIncludeDirEntriesAsSpecifiedForDir( const CORE::CString& dir                          ,
                                             const TDirProcessingInstructions& allInstructions ,
                                             const CORE::CString& platform                     ,
                                             bool applyPlatformChangesOnly                     ,
                                             TStringVector& allEntries                         )
{GUCEF_TRACE;

    if ( !applyPlatformChangesOnly )
    {
        ExcludeOrIncludeEntriesAsSpecifiedForDir( allInstructions ,
                                                  AllPlatforms    ,
                                                  allEntries      );
                                                  
    }

    if ( !platform.IsNULLOrEmpty() && platform != AllPlatforms )
    {
        ExcludeOrIncludeEntriesAsSpecifiedForDir( allInstructions ,
                                                  platform        ,
                                                  allEntries      );
    }
}

/*---------------------------------------------------------------------------*/

void
ExcludeFileEntriesAsSpecifiedForDir( const TDirProcessingInstructions& allInstructions ,
                                     const CORE::CString& platform                     ,
                                     bool applyPlatformChangesOnly                     ,
                                     TStringVector& allEntries                         )
{GUCEF_TRACE;

    if ( !applyPlatformChangesOnly )
    {
        ExcludeEntriesAsSpecifiedForFile( allInstructions ,
                                          AllPlatforms    ,
                                          allEntries      );
    }

    if ( !platform.IsNULLOrEmpty() && platform != AllPlatforms )
    {
        ExcludeEntriesAsSpecifiedForFile( allInstructions ,
                                          platform        ,
                                          allEntries      );
    }
}

/*---------------------------------------------------------------------------*/

void
IncludeFileEntriesAsSpecifiedForDir( const TDirProcessingInstructions& allInstructions ,
                                     const CORE::CString& platform                     ,
                                     const CORE::CString& currentPath                  ,
                                     bool applyPlatformChangesOnly                     ,
                                     TStringVectorMap& allEntries                      ,
                                     const TStringVector& fileTypes                    )
{GUCEF_TRACE;

    if ( !applyPlatformChangesOnly )
    {
        IncludeEntriesAsSpecifiedForFile( allInstructions ,
                                          AllPlatforms    ,
                                          currentPath     ,
                                          allEntries      ,
                                          fileTypes       );
    }

    if ( !platform.IsNULLOrEmpty() && platform != AllPlatforms )
    {
        IncludeEntriesAsSpecifiedForFile( allInstructions ,
                                          platform        ,
                                          currentPath     ,
                                          allEntries      ,
                                          fileTypes       );
    }
}

/*---------------------------------------------------------------------------*/

bool
IsProcessingInstructionsItemADir( const CORE::CString& instructionDir ,
                                  const CORE::CString& itemName       )
{GUCEF_TRACE;

    CORE::CString itemPath = instructionDir;
    CORE::AppendToPath( itemPath, itemName );

    if ( CORE::FileExists( itemPath ) )
    {
        return false;
    }
    else
    if ( CORE::IsPathValid( itemPath ) )
    {
        return true;
    }
    else
    {
        // Unable to determine by checking if the item exists on disk because it isnt there
        // As a fallback we will try to check for an extension, its the best we can do
        return CORE::ExtractFileExtention( itemName ).Length() == 0;
    }
}

/*---------------------------------------------------------------------------*/

bool
IsProcessingInstructionsItemADir( const CORE::CString& instructionDir ,
                                  const CORE::CString& itemName       ,
                                  const CORE::CString& itemType       )
{GUCEF_TRACE;

    if ( itemType.IsNULLOrEmpty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "The type of an item (\"" + itemName + "\") in the processing instructions is not specified, will try to auto-determine the type" );
        if ( IsProcessingInstructionsItemADir( instructionDir, itemName ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Determined that item \"" + itemName + "\" is a directory" );
            return true;
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Determined that item \"" + itemName + "\" is a file" );
            return false;
        }
    }
    else
    {
        static const CORE::CString typeAttribDirValue = "DIR";
        return itemType.Equals( typeAttribDirValue, false );
    }
}

/*---------------------------------------------------------------------------*/

void
ParseProcessingInstructions( const CORE::CString& instructionsDir           ,
                             TDirProcessingInstructions& instructionStorage )
{GUCEF_TRACE;

    // Now we exclude based on the processing instructions for this dir
    // if any exist
    const CORE::CDataNode& instructions = instructionStorage.processingInstructions;
    if ( instructions.HasChildren() )
    {
        // write down the tags we will parse for
        static const CORE::CString oldRootNodeName = "CMAKELISTGENERATOR";
        static const CORE::CString rootNodeName = "PROJECTGENERATOR";
        static const CORE::CString excludesNodeName = "EXCLUDES";
        static const CORE::CString includesNodeName = "INCLUDES";
        static const CORE::CString platformNodeName = "PLATFORM";
        static const CORE::CString itemNodeName = "ITEM";
        static const CORE::CString nameAttribName = "NAME";
        static const CORE::CString typeAttribName = "TYPE";
        static const CORE::CString allPlatformsValue = "ALL";

        // Parse the instructions for the information we are looking for
        CORE::CString leftOver;
        const CORE::CDataNode* rootNode = instructions.Find( rootNodeName );
        if ( 0 == rootNode )
        {
            rootNode = instructions.Find( oldRootNodeName );
        }
        if ( 0 != rootNode )
        {
            CORE::CDataNode::const_iterator i = rootNode->ConstBegin();
            while ( i != rootNode->End() )
            {
                const CORE::CDataNode* curNode = (*i);
                if ( curNode->GetName() == excludesNodeName )
                {
                    // Store whether the instructions apply to a particular platform
                    CORE::CString platformValue = curNode->GetAttributeValue( platformNodeName ).Lowercase();

                    // apply 1 to n platform mappings if applicable
                    TStringSet platforms = ResolveMultiPlatformName( platformValue );
                    TStringSet::iterator p = platforms.begin();
                    while ( p != platforms.end() )
                    {
                        const CORE::CString& platformName = (*p); 

                        // Parse instructions
                        const CORE::CDataNode* excludesNode = curNode;
                        CORE::CDataNode::const_iterator n = excludesNode->ConstBegin();
                        while ( n != excludesNode->ConstEnd() )
                        {
                            // Process all the items in this instruction set
                            const CORE::CDataNode* curNode2 = (*n);
                            if ( curNode2->GetName() == itemNodeName )
                            {
                                // Get the name and type of the item to add to the list
                                CORE::CString itemName = curNode2->GetAttributeValue( nameAttribName );
                                if ( !itemName.IsNULLOrEmpty() )
                                {
                                    CORE::CString itemType = curNode2->GetAttributeValue( typeAttribName );
                                    bool itemIsADir = IsProcessingInstructionsItemADir( instructionsDir, itemName, itemType );

                                    // We found an item to add to our list
                                    if ( platformName.IsNULLOrEmpty() || platformName.Equals( allPlatformsValue, false ) )
                                    {
                                        // Add as non-platform specific
                                        if ( itemName.HasChar( '*' ) != -1 && itemType.IsNULLOrEmpty() )
                                        {
                                            instructionStorage.dirExcludeList[ AllPlatforms ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added wildcard directory exclude entry \"" + itemName + "\" based on the processing instructions for this dir" );
                                            instructionStorage.fileExcludeList[ AllPlatforms ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added wildcard file exclude entry \"" + itemName + "\" based on the processing instructions for this dir" );
                                        }
                                        else
                                        {
                                            if ( itemIsADir )
                                            {
                                                instructionStorage.dirExcludeList[ AllPlatforms ].push_back( itemName );
                                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added directory exclude entry \"" + itemName + "\" based on the processing instructions for this dir" );
                                            }
                                            else
                                            {
                                                instructionStorage.fileExcludeList[ AllPlatforms ].push_back( itemName );
                                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added file exclude entry \"" + itemName + "\" based on the processing instructions for this dir" );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // Add for the given platform
                                        if ( itemName.HasChar( '*' ) != -1 && itemType.IsNULLOrEmpty() )
                                        {
                                            instructionStorage.dirExcludeList[ platformName ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added wildcard directory exclude entry \"" + itemName + "\" for platform " + platformName + " based on the processing instructions for this dir" );
                                            instructionStorage.fileExcludeList[ platformName ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added wildcard file exclude entry \"" + itemName + "\" for platform " + platformName + " based on the processing instructions for this dir" );
                                        }
                                        else
                                        {
                                            if ( itemIsADir )
                                            {
                                                instructionStorage.dirExcludeList[ platformName ].push_back( itemName );
                                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added directory exclude entry \"" + itemName + "\" for platform " + platformName + " based on the processing instructions for this dir" );
                                            }
                                            else
                                            {
                                                instructionStorage.fileExcludeList[ platformName ].push_back( itemName );
                                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added file exclude entry \"" + itemName + "\" for platform " + platformName + " based on the processing instructions for this dir" );
                                            }
                                        }
                                    }
                                }
                            }
                            ++n;
                        }
                        ++p;
                    }
                }
                else
                if ( curNode->GetName() == includesNodeName )
                {
                    // Store whether the instructions apply to a particular platform
                    CORE::CString platformValue = curNode->GetAttributeValue( platformNodeName ).Lowercase();

                    // apply 1 to n platform mappings if applicable
                    TStringSet platforms = ResolveMultiPlatformName( platformValue );
                    TStringSet::iterator p = platforms.begin();
                    while ( p != platforms.end() )
                    {
                        CORE::CString platformName = (*p);

                        // Parse instructions
                        const CORE::CDataNode* includesNode = curNode;
                        CORE::CDataNode::const_iterator n = includesNode->ConstBegin();
                        while ( n != includesNode->ConstEnd() )
                        {
                            // Process all the items in this instruction set
                            const CORE::CDataNode* curNode2 = (*n);
                            if ( curNode2->GetName() == itemNodeName )
                            {
                                // Get the name of the item to add to the list
                                CORE::CString itemName = curNode2->GetAttributeValue( nameAttribName );
                                if ( !itemName.IsNULLOrEmpty() )
                                {
                                    CORE::CString itemType = curNode2->GetAttributeValue( typeAttribName );
                                    bool itemIsADir = IsProcessingInstructionsItemADir( instructionsDir, itemName, itemType );

                                    // We found an item to add to our list
                                    if ( platformName.IsNULLOrEmpty() || platformName.Equals( allPlatformsValue, false ) )
                                    {
                                        // Add as non-platform specific
                                        if ( itemName.HasChar( '*' ) != -1 && itemType.IsNULLOrEmpty() )
                                        {
                                            instructionStorage.dirIncludeList[ AllPlatforms ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added wildcard directory include entry \"" + itemName + "\" based on the processing instructions for this dir" );
                                            instructionStorage.fileIncludeList[ AllPlatforms ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added wildcard file include entry \"" + itemName + "\" based on the processing instructions for this dir" );
                                        }
                                        else
                                        if ( itemIsADir )
                                        {
                                            instructionStorage.dirIncludeList[ AllPlatforms ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added directory include entry \"" + itemName + "\" based on the processing instructions for this dir" );
                                        }
                                        else
                                        {
                                            instructionStorage.fileIncludeList[ AllPlatforms ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added file include entry \"" + itemName + "\" based on the processing instructions for this dir" );
                                        }
                                    }
                                    else
                                    {
                                        // Add for the given platform
                                        if ( itemName.HasChar( '*' ) != -1 && itemType.IsNULLOrEmpty() )
                                        {
                                            instructionStorage.dirIncludeList[ platformName ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added wildcard directory include entry \"" + itemName + "\" for platform " + platformName + " based on the processing instructions for this dir" );
                                            instructionStorage.fileIncludeList[ platformName ].push_back( itemName );
                                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added wildcard file include entry \"" + itemName + "\" for platform " + platformName + " based on the processing instructions for this dir" );
                                        }
                                        else
                                        {
                                            if ( itemIsADir )
                                            {
                                                instructionStorage.dirIncludeList[ platformName ].push_back( itemName );
                                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added directory include entry \"" + itemName + "\" for platform " + platformName + " based on the processing instructions for this dir" );
                                            }
                                            else
                                            {
                                                instructionStorage.fileIncludeList[ platformName ].push_back( itemName );
                                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Added file include entry \"" + itemName + "\" for platform " + platformName + " based on the processing instructions for this dir" );
                                            }
                                        }
                                    }
                                }
                            }
                            ++n;
                        }
                        ++p;
                    }
                }

                ++i;
            }
        }
    }
}

/*---------------------------------------------------------------------------*/

bool
AreProcessingInstructionsOnDisk( const CORE::CString& dir )
{GUCEF_TRACE;

    return CORE::FileExists( GetExcludeListPath( dir ) )               ||
           CORE::FileExists( GetLegacyExcludeListPath( dir ) )         ||
           CORE::FileExists( GetProcessingInstructionsPath( dir ) )    ||
           CORE::FileExists( GetLegacyProcessingInstructionsPath( dir ) );
}

/*---------------------------------------------------------------------------*/

TDirProcessingInstructions*
GetProcessingInstructions( TProjectInfo& projectInfo ,
                           const CORE::CString& dir  )
{GUCEF_TRACE;

    // See if we have already stored instructions for this directory
    TDirProcessingInstructionsMap::iterator i = projectInfo.dirProcessingInstructions.find( dir );
    if ( i != projectInfo.dirProcessingInstructions.end() )
    {
        return &( (*i).second );
    }

    if ( AreProcessingInstructionsOnDisk( dir ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Located processing instructions for directory \"" + dir + "\"" );

        // See if we can load instructions for this directory
        TDirProcessingInstructions& instructions = projectInfo.dirProcessingInstructions[ dir ];

        // Load the simple exclude list and put the excludes in the correct list
        TStringVector simpleExcludeList = GetExcludeList( dir );
        TStringVector::iterator n = simpleExcludeList.begin();
        while ( n != simpleExcludeList.end() )
        {
            CORE::CString& entry = (*n);
            if ( !entry.IsNULLOrEmpty() )
            {
                if ( entry.HasChar( '*' ) != -1 )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Determined that the simple exclude list wildcard item \"" + entry + "\" is a directory" );
                    instructions.dirExcludeList[ AllPlatforms ].push_back( entry );
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Determined that the simple exclude list wildcard item \"" + entry + "\" is a file" );
                    instructions.fileExcludeList[ AllPlatforms ].push_back( entry );
                }
                else
                if ( IsProcessingInstructionsItemADir( dir, entry ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Determined that the simple exclude list item \"" + entry + "\" is a directory" );
                    instructions.dirExcludeList[ AllPlatforms ].push_back( entry );
                }
                else
                {
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Determined that the simple exclude list item \"" + entry + "\" is a file" );
                    instructions.fileExcludeList[ AllPlatforms ].push_back( entry );
                }
            }
            ++n;
        }

        // Now process the more fancy processing instructions
        if ( GetProcessingInstructions( dir, instructions.processingInstructions ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Parsing advanced processing instructions for directory \"" + dir + "\"" );
            ParseProcessingInstructions( dir, instructions );
        }
        return &instructions;
    }

    return NULL;
}

/*---------------------------------------------------------------------------*/

void
LoadAllProcessingInstructions( TProjectInfo& projectInfo    ,
                               const CORE::CString& rootDir )
{GUCEF_TRACE;

    // Load instructions for the root dir itself
    TDirProcessingInstructions* dirInstructions = GetProcessingInstructions( projectInfo, rootDir );

    // Recurse through sub-dirs to find more instructions
    CORE::SDI_Data* sdiData = CORE::DI_First_Dir_Entry( rootDir.C_String() );
    if ( NULL != sdiData )
    {
        do
        {
            // make sure we are dealing with a directory
            if ( 0 == DI_Is_It_A_File( sdiData ) )
            {
                CORE::CString dirName = DI_Name( sdiData );
                if ( ( dirName != "." ) && ( dirName != ".." ) )
                {
                    CORE::CString subRoot = rootDir;
                    CORE::AppendToPath( subRoot, dirName );

                    // Optimization: Check to see if the sub-dir is excluded
                    // If so then we don't even look for processing instructions in said sub-dir
                    // Note that this optimization can only be applied for dirs that are excluded on all platforms
                    bool skipSubDir = false;
                    if ( IsStringInList( projectInfo.globalDirExcludeList, true, dirName, true ) )
                    {
                        skipSubDir = true;
                        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Skipping the search for processing instructions, based on global dir exclude list, for dir \"" + subRoot + "\"" );
                    }
                    else
                    if ( NULL != dirInstructions )
                    {
                        TStringVectorMap::iterator i = dirInstructions->dirExcludeList.find( AllPlatforms );
                        if ( i != dirInstructions->dirExcludeList.end() )
                        {
                            skipSubDir = IsStringInList( (*i).second, true, dirName, true );
                            if ( skipSubDir )
                            {
                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Skipping the search for processing instructions, based on already located processing instructions, for dir \"" + subRoot + "\"" );
                            }
                        }
                    }

                    if ( !skipSubDir )
                    {
                        // Recurse into sub-dir
                        LoadAllProcessingInstructions( projectInfo, subRoot );
                    }
                }
            }
        }
        while ( 0 != DI_Next_Dir_Entry( sdiData ) );
        DI_Cleanup( sdiData );
    }
}

/*---------------------------------------------------------------------------*/

void
ExcludeOrIncludeDirEntriesAsSpecifiedForDir( TProjectInfo& projectInfo     ,
                                             const CORE::CString& dir      ,
                                             const CORE::CString& platform ,
                                             bool applyPlatformChangesOnly ,
                                             TStringVector& allEntries     )
{GUCEF_TRACE;

    // Fetch processing instructions from directory
    TDirProcessingInstructions* instructionStorage = GetProcessingInstructions( projectInfo, dir );

    // Perform processing of global dir excludes
    TStringVector::iterator n = projectInfo.globalDirExcludeList.begin();
    while ( n != projectInfo.globalDirExcludeList.end() )
    {
        if ( RemoveString( allEntries, (*n) ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Excluded the dir \"" + (*n) + "\" based on the global dir exclude list" );
        }
        ++n;
    }

    if ( NULL != instructionStorage )
    {
        // Carry out the process using the fetched instructions
        ExcludeOrIncludeDirEntriesAsSpecifiedForDir( dir, *instructionStorage, platform, applyPlatformChangesOnly, allEntries );
    }
}

/*---------------------------------------------------------------------------*/

void
ExcludeFileEntriesAsSpecifiedForDir( TProjectInfo& projectInfo     ,
                                     const CORE::CString& dir      ,
                                     const CORE::CString& platform ,
                                     bool applyPlatformChangesOnly ,
                                     TStringVector& allEntries     )
{GUCEF_TRACE;

    // Fetch processing instructions from directory
    TDirProcessingInstructions* instructionStorage = GetProcessingInstructions( projectInfo, dir );

    if ( NULL != instructionStorage )
    {
        // Carry out the process using the fetched instructions
        ExcludeFileEntriesAsSpecifiedForDir( *instructionStorage, platform, applyPlatformChangesOnly, allEntries );
    }
}

/*---------------------------------------------------------------------------*/

void
IncludeFileEntriesAsSpecifiedForDir( TProjectInfo& projectInfo        ,
                                     const CORE::CString& dir         ,
                                     const CORE::CString& platform    ,
                                     const CORE::CString& currentPath ,
                                     bool applyPlatformChangesOnly    ,
                                     TStringVectorMap& allEntries     ,
                                     const TStringVector& fileTypes   )
{GUCEF_TRACE;

    // Fetch processing instructions from directory
    TDirProcessingInstructions* instructionStorage = GetProcessingInstructions( projectInfo, dir );

    if ( NULL != instructionStorage )
    {
        // Carry out the process using the fetched instructions
        IncludeFileEntriesAsSpecifiedForDir( *instructionStorage      , 
                                             platform                 , 
                                             currentPath              ,
                                             applyPlatformChangesOnly , 
                                             allEntries               , 
                                             fileTypes                );
    }
}

/*---------------------------------------------------------------------------*/

void
ExcludeOrIncludeDirEntriesAsSpecifiedForDir( TProjectInfo& projectInfo ,
                                             const CORE::CString& dir  ,
                                             TStringVector& allEntries )
{GUCEF_TRACE;

    ExcludeOrIncludeDirEntriesAsSpecifiedForDir( projectInfo,
                                                 dir,
                                                 CORE::CString(),
                                                 false,
                                                 allEntries );
}

/*---------------------------------------------------------------------------*/

void
ExcludeFileEntriesAsSpecifiedForDir( TProjectInfo& projectInfo ,
                                     const CORE::CString& dir  ,
                                     TStringVector& allEntries )
{GUCEF_TRACE;

    ExcludeFileEntriesAsSpecifiedForDir( projectInfo     ,
                                         dir             ,
                                         CORE::CString() ,
                                         false           ,
                                         allEntries      );
}

/*---------------------------------------------------------------------------*/

void
IncludeFileEntriesAsSpecifiedForDir( TProjectInfo& projectInfo        ,
                                     const CORE::CString& dir         ,
                                     TStringVectorMap& allEntries     ,
                                     const TStringVector& fileTypes   ,
                                     const CORE::CString& currentPath )
{GUCEF_TRACE;

    IncludeFileEntriesAsSpecifiedForDir( projectInfo     ,
                                         dir             ,
                                         CORE::CString() ,
                                         currentPath     ,
                                         false           ,
                                         allEntries      ,
                                         fileTypes       );
}

/*-------------------------------------------------------------------------*/

void
PopulateFileListFromDir( const CORE::CString& path      ,
                         const TStringVector& fileTypes ,
                         TStringVector& fileList        ,
                         const CORE::CString& platform  )
{GUCEF_TRACE;

    if ( platform.IsNULLOrEmpty() || platform == AllPlatforms )
    {
        // current dir cannot be a platform dir
        if ( IsDirAPlatformDir( path ) ) return;
    }
    else    
    {
        // current dir must be a dir which is considered to be a platform dir
        // for the platform specified
        if ( !IsDirAPlatformDirForPlatform( path, platform ) ) return;
    }
    
    CORE::SDI_Data* sdiData = CORE::DI_First_Dir_Entry( path.C_String() );
    if ( NULL != sdiData )
    {
        do
        {
            if ( 0 != DI_Is_It_A_File( sdiData ) )
            {
                CORE::CString filename = DI_Name( sdiData );
                CORE::CString fileExt = CORE::ExtractFileExtention( filename );

                if ( IsStringInList( fileTypes, false, fileExt ) )
                {
                    fileList.push_back( filename );
                }
            }
        }
        while ( 0 != DI_Next_Dir_Entry( sdiData ) );
        DI_Cleanup( sdiData );
    }
}

/*---------------------------------------------------------------------------*/

void
PopulateDirListFromDir( const CORE::CString& path     ,
                        TStringVector& dirList        ,
                        const CORE::CString& platform ,
                        bool excludeGenericDirs       )
{GUCEF_TRACE;

    if ( ( platform.IsNULLOrEmpty() || platform == AllPlatforms ) || !excludeGenericDirs )
    {
        // Get a list of all platform dirs
        const TStringSet& platformsDirs = GetSupportedPlatformDirs();
        
        // Go through the dir adding dirs which qualify
        CORE::SDI_Data* sdiData = CORE::DI_First_Dir_Entry( path.C_String() );
        if ( NULL != sdiData )
        {
            do
            {
                if ( 0 == DI_Is_It_A_File( sdiData ) )
                {
                    // Add the dir if it is a real dir and if its not a platform dir
                    CORE::CString dirName = DI_Name( sdiData );
                    if ( ( dirName != "." ) && ( dirName != ".." )               && 
                         ( platformsDirs.find( dirName ) == platformsDirs.end() ) )
                    {
                        dirList.push_back( dirName );
                    }
                }
            }
            while ( 0 != DI_Next_Dir_Entry( sdiData ) );
            DI_Cleanup( sdiData );
        }
    }
    
    if ( !platform.IsNULLOrEmpty() && platform != AllPlatforms )
    {
        // We are looking for dirs for the given platform
        const TStringSetMap& platformsDirMap = GetSupportedPlatformDirMap();    
        TStringSetMap::const_iterator i = platformsDirMap.find( platform );
        if ( i != platformsDirMap.end() )
        {
            const TStringSet& dirsForPlatform = (*i).second;
            
            // Check each dir to see if it exists
            CORE::SDI_Data* sdiData = CORE::DI_First_Dir_Entry( path.C_String() );
            if ( NULL != sdiData )
            {
                do
                {
                    if ( 0 == DI_Is_It_A_File( sdiData ) )
                    {
                        // Add the dir if it is a real dir and if its a platform dir for
                        // the specified platform
                        CORE::CString dirName = DI_Name( sdiData );
                        if ( ( dirName != "." ) && ( dirName != ".." )                               && 
                             ( dirsForPlatform.find( dirName.Lowercase() ) != dirsForPlatform.end() ) )
                        {
                            dirList.push_back( dirName );
                        }
                    }
                }
                while ( 0 != DI_Next_Dir_Entry( sdiData ) );
                DI_Cleanup( sdiData );
            }
        }
    }
}

/*---------------------------------------------------------------------------*/

void
GetListOfAllModuleDirs( TModuleInfo& moduleInfo      ,
                        TStringSet& moduleDirs       ,
                        bool relativePaths           ,
                        const CORE::CString& rootDir )
{GUCEF_TRACE;

    TStringVectorMap::iterator i = moduleInfo.includeDirs.begin();
    while ( i != moduleInfo.includeDirs.end() )
    {
        if ( relativePaths )
        {
            moduleDirs.insert( (*i).first );
        }
        else
        {
            CORE::CString fullPath = rootDir;
            CORE::AppendToPath( fullPath, (*i).first );

            moduleDirs.insert( fullPath );
        }
        ++i;
    }
    i = moduleInfo.sourceDirs.begin();
    while ( i != moduleInfo.sourceDirs.end() )
    {
        if ( relativePaths )
        {
            moduleDirs.insert( (*i).first );
        }
        else
        {
            CORE::CString fullPath = rootDir;
            CORE::AppendToPath( fullPath, (*i).first );

            moduleDirs.insert( fullPath );
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
GetListOfAllModuleDirs( TModuleInfoEntry& moduleInfoEntry ,
                        TStringSet& moduleDirs            ,
                        bool relativePaths                ,
                        const CORE::CString& platform     )
{GUCEF_TRACE;

    TModuleInfoMap::iterator i = moduleInfoEntry.modulesPerPlatform.find( platform );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        GetListOfAllModuleDirs( (*i).second             ,
                                moduleDirs              ,
                                relativePaths           ,
                                moduleInfoEntry.rootDir ); 
    }
}

/*---------------------------------------------------------------------------*/

void
GetListOfAllModuleDirs( TModuleInfoEntry& moduleInfoEntry ,
                        TStringSet& moduleDirs            ,
                        bool relativePaths                )
{GUCEF_TRACE;

    // Add generic dirs
    GetListOfAllModuleDirs( moduleInfoEntry ,
                            moduleDirs      ,
                            relativePaths   ,
                            AllPlatforms    ); 
                                
    // Add all platform dirs
    const TStringSet& platforms = GetSupportedPlatforms();    
    TStringSet::const_iterator i = platforms.begin();
    while ( i != platforms.end() )
    {
        GetListOfAllModuleDirs( moduleInfoEntry ,
                                moduleDirs      ,
                                relativePaths   ,
                                (*i)            ); 
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

TStringVector
CMakeParseFileLines( const CORE::CString& fileSuffix )
{GUCEF_TRACE;

    CORE::CString testStr = fileSuffix.ReplaceChar( '\r', '\n' );
    testStr = testStr.CompactRepeatingChar( '\n' );
    return testStr.ParseElements( '\n', false );
}

/*---------------------------------------------------------------------------*/

TStringSet
CMakeParseIncludeDirs( const CORE::CString& fileSuffix )
{GUCEF_TRACE;

    TStringSet includeDirs;

    TStringVector suffixFileLines = CMakeParseFileLines( fileSuffix );
    TStringVector::iterator i = suffixFileLines.begin();
    while ( i != suffixFileLines.end() )
    {
        CORE::CString testStr = (*i).Lowercase();
        CORE::Int32 subStrIdx = testStr.HasSubstr( "include_directories(", true );
        CORE::Int32 commentCharIdx = testStr.HasChar( '#', true );

        // Is this function call commented out?
        if ( commentCharIdx > -1 && commentCharIdx < subStrIdx )
        {
            // Then ignore it
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Found include_directories function in suffix file but it is commented out, ignoring it" );
            ++i;
            continue;
        }

        // Does this line have the function call we are looking for?
        if ( subStrIdx >= 0 )
        {
            // It does, parse the parameters
            CORE::CString dependenciesStr = (*i).SubstrToChar( ')', (CORE::UInt32)subStrIdx+20, true );
            dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
            dependenciesStr = dependenciesStr.Trim( true );
            dependenciesStr = dependenciesStr.Trim( false );
            TStringVector elements = dependenciesStr.ParseElements( ' ' );
            if ( !elements.empty() )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Found " + CORE::Int32ToString( elements.size() ) + " include dirs in suffix file" );
            }

            // Add this collection to the list of all include dirs we found
            TStringVector::iterator n = elements.begin();
            while ( n != elements.end() )
            {
                includeDirs.insert( (*n) );
                ++n;
            }
        }
        ++i;
    }
    return includeDirs;
}

/*---------------------------------------------------------------------------*/

void
CMakeParseSuffixFile( TModuleInfo& moduleInfo, const CORE::CString& cmakeListSuffixFileContent )
{GUCEF_TRACE;
        
    TStringVector suffixFileLines = CMakeParseFileLines( cmakeListSuffixFileContent );
    TStringVector::iterator i = suffixFileLines.begin();
    while ( i != suffixFileLines.end() )
    {
        CORE::CString testStr = (*i).Lowercase();
        CORE::Int32 subStrIdx = testStr.HasSubstr( "target_link_libraries(", true );
        CORE::Int32 commentCharIdx = testStr.HasChar( '#', true );

        // Is this function call commented out?
        if ( commentCharIdx > -1 && commentCharIdx < subStrIdx )
        {
            // Then ignore it
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Found target_link_libraries function in suffix file but it is commented out, ignoring it" );
            ++i;
            continue;
        }

        // Does this line have the function call we are looking for?
        if ( subStrIdx >= 0 )
        {
            // It does, parse the parameters
            CORE::CString dependenciesStr = (*i).SubstrToChar( ')', (CORE::UInt32)subStrIdx+22, true );
            dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
            dependenciesStr = dependenciesStr.Trim( true );
            dependenciesStr = dependenciesStr.Trim( false );
            TStringVector elements = dependenciesStr.ParseElements( ' ' );
            if ( !elements.empty() )
            {
                // the first element is the name of the module, we don't need it
                elements.erase( elements.begin() );
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Found " + CORE::Int32ToString( elements.size() ) + " libraries to link to in suffix file" );
            }

            // Add this collection to the list of all linked libraries we found
            TStringVector::iterator n = elements.begin();
            while ( n != elements.end() )
            {
                moduleInfo.linkerSettings.linkedLibraries[ (*n) ] = MODULETYPE_UNDEFINED;
                ++n;
            }
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

TStringVector
CMakeParseDependencies( const CORE::CString& fileSuffix ,
                        CORE::CString& moduleName       )
{GUCEF_TRACE;

    TStringVector dependencies;

    TStringVector suffixFileLines = CMakeParseFileLines( fileSuffix );
    TStringVector::iterator i = suffixFileLines.begin();
    while ( i != suffixFileLines.end() )
    {
        CORE::CString testStr = (*i).Lowercase();
        CORE::Int32 subStrIdx = testStr.HasSubstr( "add_dependencies(", true );
        CORE::Int32 commentCharIdx = testStr.HasChar( '#', true );

        // Is this function call commented out?
        if ( commentCharIdx > -1 && commentCharIdx < subStrIdx )
        {
            // Then ignore it
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Found add_dependencies function in suffix file but it is commented out, ignoring it" );
            ++i;
            continue;
        }

        // Does this line have the function call we are looking for?
        if ( subStrIdx >= 0 )
        {
            // It does, parse the parameters
            CORE::CString dependenciesStr = (*i).SubstrToChar( ')', (CORE::UInt32)subStrIdx+17, true );
            dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
            dependenciesStr = dependenciesStr.Trim( true );
            dependenciesStr = dependenciesStr.Trim( false );
            TStringVector elements = dependenciesStr.ParseElements( ' ' );
            if ( !elements.empty() )
            {
                moduleName = *(elements.begin());
                elements.erase( elements.begin() );
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Found " + CORE::Int32ToString( elements.size() ) + " dependencies in suffix file" );
            }

            // Add this collection to the list of all dependencies we found
            TStringVector::iterator n = elements.begin();
            while ( n != elements.end() )
            {
                dependencies.push_back( (*n) );
                ++n;
            }
        }
        ++i;
    }
    return dependencies;
}

/*---------------------------------------------------------------------------*/

bool
CMakeParseModuleProperties( const CORE::CString& fileSuffix ,
                            CORE::CString& moduleName       ,
                            bool& isExecutable              )
{GUCEF_TRACE;

    CORE::CString testStr = fileSuffix.Lowercase();
    CORE::Int32 subStrIdx = testStr.HasSubstr( "add_library(", true );
    if ( subStrIdx >= 0 )
    {
        isExecutable = false;

        CORE::CString dependenciesStr = fileSuffix.SubstrToChar( ')', (CORE::UInt32)subStrIdx+12, true );
        dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
        dependenciesStr = dependenciesStr.Trim( true );
        dependenciesStr = dependenciesStr.Trim( false );
        TStringVector elements = dependenciesStr.ParseElements( ' ' );
        if ( !elements.empty() )
        {
            moduleName = *(elements.begin());
            return true;
        }

        // Malformed file!
        return false;
    }
    else
    {
        subStrIdx = testStr.HasSubstr( "add_executable(", true );
        if ( subStrIdx >= 0 )
        {
            isExecutable = true;

            CORE::CString dependenciesStr = fileSuffix.SubstrToChar( ')', (CORE::UInt32)subStrIdx+15, true );
            dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
            dependenciesStr = dependenciesStr.Trim( true );
            dependenciesStr = dependenciesStr.Trim( false );
            TStringVector elements = dependenciesStr.ParseElements( ' ' );
            if ( !elements.empty() )
            {
                moduleName = *(elements.begin());
                return true;
            }

            // Malformed file!
            return false;
        }
    }

    return false;
}

/*---------------------------------------------------------------------------*/

const TModuleInfo*
GetModuleInfo( const TProjectInfo& projectInfo ,
               const CORE::CString& moduleName ,
               const CORE::CString& platform   )
{GUCEF_TRACE;

    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        const TModuleInfo* moduleInfo = NULL;
        const CORE::CString* nameOfCurrentModule = GetModuleName( (*i), platform, &moduleInfo );
        if ( NULL != nameOfCurrentModule )
        {
            if ( *nameOfCurrentModule == moduleName )
            {
                return moduleInfo;
            }
        }
        ++i;
    }
    return NULL;
}

/*---------------------------------------------------------------------------*/

void
GenerateModuleDependencyIncludes( TModuleInfoEntry& moduleInfoEntry             ,
                                  const TModuleInfoEntry* dependencyModuleEntry ,
                                  const CORE::CString& platformName             )
{GUCEF_TRACE;

    // Narrow it down to the platform we are interested in    
    TModuleInfoMap::const_iterator n = dependencyModuleEntry->modulesPerPlatform.find( platformName );
    if ( n != dependencyModuleEntry->modulesPerPlatform.end() )
    {
        // this dependency has module info which is specfic to this platform
        const TModuleInfo& dependencyModule = (*n).second;

        TModuleInfo* moduleInfo = NULL;

        // For header include locations we want to include the module definition
        // location regardless of whether headers were found there. This takes care of the
        // use-case whereby people use complex relative paths in their headers which need a particular
        // starting point
        if ( dependencyModule.moduleType == MODULETYPE_HEADER_INCLUDE_LOCATION )
        {
            moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, platformName, true );
            
            // Determine the relative path to this other module's root
            CORE::CString relativePath = CORE::GetRelativePathToOtherPathRoot( moduleInfoEntry.rootDir        ,
                                                                               dependencyModuleEntry->rootDir );
            relativePath = relativePath.ReplaceChar( '\\', '/' );

            moduleInfo->dependencyIncludeDirs.insert( relativePath );
        }

        // Now construct the relative path to each of the dependency module's include dirs
        // These dir will all become include dirs for this module
        const TStringVectorMap& headerFiles = dependencyModule.includeDirs;
        if ( !headerFiles.empty() )
        {
            // Since this platform specific entry has header files we will need to generate
            // and include platform specific dependency paths to this module
            // to that end we will grab this module's definition for this platform or make a
            // new one if no entry exists yet
            if ( NULL == moduleInfo )
                moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, platformName, true );
                                
            // Determine the relative path to this other module and subsequently the include dirs
            CORE::CString relativePath = CORE::GetRelativePathToOtherPathRoot( moduleInfoEntry.rootDir        ,
                                                                               dependencyModuleEntry->rootDir );
            relativePath = relativePath.ReplaceChar( '\\', '/' );                
            
            TStringVectorMap::const_iterator n = headerFiles.begin();
            while ( n != headerFiles.end() )
            {
                CORE::CString dependencyInclDir = relativePath;
                CORE::AppendToPath( dependencyInclDir, (*n).first );
                dependencyInclDir = CORE::RelativePath( dependencyInclDir );
                dependencyInclDir = dependencyInclDir.ReplaceChar( '\\', '/' );

                // Add the contructed include directory to the list of dependency directories
                // for the current module. This can later be used again by other modules which
                // include this one.
                moduleInfo->dependencyIncludeDirs.insert( dependencyInclDir );
                ++n;
            }

            // On top of that we have to include all the include dirs that the dependency module
            // was including itself since it's headers might be referring to those files.
            TStringSet::const_iterator m = dependencyModule.dependencyIncludeDirs.begin();
            while ( m != dependencyModule.dependencyIncludeDirs.end() )
            {
                CORE::CString dependencyInclDir = relativePath;
                CORE::AppendToPath( dependencyInclDir, (*m) );
                dependencyInclDir = CORE::RelativePath( dependencyInclDir );
                dependencyInclDir = dependencyInclDir.ReplaceChar( '\\', '/' );

                // Add the contructed include directory to the list of dependency directories
                // for the current module. This can later be used again by other modules which
                // include this one.
                moduleInfo->dependencyIncludeDirs.insert( dependencyInclDir );
                ++m;
            }
        }
    }
}

/*---------------------------------------------------------------------------*/

void
GenerateModuleDependencyIncludesForPlatform( const TProjectInfo& projectInfo     ,
                                             TModuleInfoEntry& moduleInfoEntry   ,
                                             const CORE::CString& platformName   ,
                                             const CORE::CString& dependencyName )
{GUCEF_TRACE;

    // Get a dependency module which is already fully processed
    // We know it is already processed since it is a dependency and thus
    // the build order sorting which is dependency based should allow us to iterate
    // based on build order 
    const TModuleInfoEntry* dependencyModule = GetModuleInfoEntry( projectInfo, dependencyName, platformName );
    if ( NULL != dependencyModule )
    {
        // Includes defined as being for all platforms are always included since other platform defintions
        // are addative and do not replace the All platforms definition. Some dependencies are only on certain platforms
        // so we cannot rely on a previous pass for AllPlatforms to have added the correct include paths
        GenerateModuleDependencyIncludes( moduleInfoEntry, dependencyModule, AllPlatforms );

        // Now we can add the platform specific includes
        if ( platformName != AllPlatforms )
        {
            GenerateModuleDependencyIncludes( moduleInfoEntry, dependencyModule, platformName );
        }
    }
}

/*---------------------------------------------------------------------------*/

// Generates include paths specific to the platform given
void
GenerateModuleDependencyIncludesForPlatform( const TProjectInfo& projectInfo   ,
                                             TModuleInfoEntry& moduleInfoEntry ,
                                             const CORE::CString& platformName )
{GUCEF_TRACE;
    
    // First we grab all the dependencies for this module. 
    // We are going to check each of the dependent modules for platform specific includes
    TStringVector dependencies;
    GetModuleDependencies( moduleInfoEntry, platformName, dependencies );
    
    // Check whether we need to add 'AllPlatforms' includes of dependencies to the includes
    // for this platform instead of relying on them being added via a 'AllPlatforms' version of this module
    bool addAllPlatformsIncludes = false;
    if ( platformName != AllPlatforms )
    {
        addAllPlatformsIncludes = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms ) == moduleInfoEntry.modulesPerPlatform.end();
    }
    
    TStringVector::iterator i = dependencies.begin();
    while ( i != dependencies.end() )
    {
        GenerateModuleDependencyIncludesForPlatform( projectInfo     ,
                                                     moduleInfoEntry ,
                                                     platformName    ,
                                                     (*i)            );
                                                     
        if ( addAllPlatformsIncludes )
        {
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "The given module does not have a 'AllPlatforms' definition and "
                          "as such the generic includes will be added to a new autogenerated 'AllPlatforms' definitions "
                          "which will retain these includes. We are using platform " + platformName );
            
            GenerateModuleDependencyIncludesForPlatform( projectInfo     ,
                                                         moduleInfoEntry ,
                                                         AllPlatforms    ,
                                                         (*i)            );
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
GenerateDependencyIncludesForPlatform( TProjectInfo& projectInfo         ,
                                       const CORE::CString& platformName )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating dependency inhertited includes for platform " + platformName );
    
    int buildOrderIndex = 0;    
    TModuleInfoEntry* moduleInfoEntry = FindFirstModuleAccordingToBuildOrder( projectInfo  ,
                                                                              platformName );
    while ( NULL != moduleInfoEntry )
    {        
        ++buildOrderIndex;
        
        GenerateModuleDependencyIncludesForPlatform( projectInfo      ,
                                                     *moduleInfoEntry ,
                                                     platformName     );

        moduleInfoEntry = FindNextModuleAccordingToBuildOrder( projectInfo      ,
                                                               *moduleInfoEntry ,
                                                               platformName     ,
                                                               buildOrderIndex  );
    }
}

/*---------------------------------------------------------------------------*/

void
GenerateDependencyIncludes( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    GenerateDependencyIncludesForPlatform( projectInfo, AllPlatforms );
    
    const TStringSet& supportedPlatforms = GetSupportedPlatforms();
    TStringSet::const_iterator i = supportedPlatforms.begin();
    while ( i != supportedPlatforms.end() )
    {
        GenerateDependencyIncludesForPlatform( projectInfo, (*i).Lowercase() );
        ++i;
    }

    // Normalize the include paths,..
    // It is possible that some dependency include paths are set as empty dir locations
    // or that dependency include paths are actually already include dirs of the project itself
    // we automatically clean that up now
    TModuleInfoEntryVector::iterator n = projectInfo.modules.begin();
    while ( n != projectInfo.modules.end() )
    {
        CleanupIncludeDirs( (*n) );
        ++n;
    }
}

/*---------------------------------------------------------------------------*/

void
FindSubDirsWithFileTypes( TProjectInfo& projectInfo          ,
                          TStringVectorMap& fileMap          ,
                          const TStringVector& fileTypes     ,
                          const CORE::CString& platform      ,
                          bool applyOnlyPlatformInstructions ,
                          const CORE::CString& curRootDir    ,
                          const CORE::CString& curRootDirSeg )
{GUCEF_TRACE;

    TStringVector fileList;
    PopulateFileListFromDir( curRootDir, fileTypes, fileList, platform );

    // Now we add/substract files based on generator instructions
    ExcludeFileEntriesAsSpecifiedForDir( projectInfo                   ,
                                         curRootDir                    ,
                                         platform                      ,
                                         applyOnlyPlatformInstructions ,
                                         fileList                      );
    IncludeFileEntriesAsSpecifiedForDir( projectInfo                   ,
                                         curRootDir                    ,
                                         platform                      ,
                                         curRootDirSeg                 ,
                                         applyOnlyPlatformInstructions ,
                                         fileMap                       ,
                                         fileTypes                     );
                                         
    if ( fileList.size() > 0 )
    {
        // found files in the current root
        TStringVectorMap::iterator i = fileMap.find( curRootDirSeg );
        if ( i == fileMap.end() )
        {
            if ( !fileMap.insert( std::pair< CORE::CString, TStringVector >( curRootDirSeg, fileList ) ).second )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to add files to the file map for subdir \"" + curRootDirSeg + "\"" );
            }
        }
        else
        {
            // We already have a list of files for this dir,.. merge
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Subdir \"" + curRootDirSeg + "\" already has files defined that "
                     "should be used for the module, this list of files will be merged with files we automatically locate in the same dir" );
                     
            MergeStringVector( (*i).second, fileList, true );
        }
    }
    
    // Get a list of sub-dirs
    TStringVector dirList;
    PopulateDirListFromDir( curRootDir, dirList, platform, false );

    // Now we add/substract dirs based on generator instructions
    ExcludeOrIncludeDirEntriesAsSpecifiedForDir( projectInfo                   ,
                                                 curRootDir                    ,
                                                 platform                      ,
                                                 applyOnlyPlatformInstructions ,
                                                 dirList                       );

    TStringVector::iterator i = dirList.begin();
    while ( i != dirList.end() )
    {
        CORE::CString subDir = curRootDir;
        CORE::AppendToPath( subDir, (*i) );

        // Do not recurse into other module dirs
        if ( !IsDirAProjectDir( subDir ) )
        {
            CORE::CString subDirSeg = curRootDirSeg;
            CORE::AppendToPath( subDirSeg, (*i) );
            subDirSeg.ReplaceChar( '\\', '/' );

            FindSubDirsWithFileTypes( projectInfo                   ,
                                      fileMap                       ,
                                      fileTypes                     ,
                                      platform                      ,
                                      applyOnlyPlatformInstructions ,
                                      subDir                        ,
                                      subDirSeg                     );
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
FindSubDirsWithHeaders( TProjectInfo& projectInfo         ,
                        TModuleInfoEntry& moduleInfoEntry ,
                        const CORE::CString& platform     )
{GUCEF_TRACE;

    TStringVectorMap fileMap;
    FindSubDirsWithFileTypes( projectInfo               , 
                              fileMap                   , 
                              GetHeaderFileExtensions() ,
                              platform                  ,
                              false                     ,
                              moduleInfoEntry.rootDir   ,
                              ""                        );
                              
    if ( !fileMap.empty() )
    {
        TModuleInfoMap::iterator i = moduleInfoEntry.modulesPerPlatform.find( platform );
        if ( i != moduleInfoEntry.modulesPerPlatform.end() )
        {
            if ( (*i).second.considerSubDirs )
            {
                MergeStringVectorMap( (*i).second.includeDirs, fileMap, true );
            }
            else
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Skipping processing subdirs since recursive processing is disabled" );
            }
        }
        else        
        {
            // If the files we are looking for are not by definition for a specific
            // platform via a platform dir then how they are processed depends on 
            // the module definition. If the module is specific to a certain platform
            // then we don't want to add a all platforms definition but rather apply
            // the files to all platforms for which the module is valid according to
            // its module definition. This is a critical difference between 'AllPlatforms'
            // and specific platforms which is important for proper overrides/merging etc
            if ( AllPlatforms == platform || platform.IsNULLOrEmpty() )
            {
                i = moduleInfoEntry.modulesPerPlatform.begin();
                while ( i != moduleInfoEntry.modulesPerPlatform.end() )
                {
                    if ( (*i).second.considerSubDirs )
                    {
                        TModuleInfo& moduleInfo = (*i).second;
                        MergeStringVectorMap( moduleInfo.includeDirs, fileMap, true );
                    }
                    else
                    {
                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Skipping processing subdirs since recursive processing is disabled" );
                    }
                    ++i;
                }
            }
            else
            {
                TModuleInfo& moduleInfo = moduleInfoEntry.modulesPerPlatform[ platform ];
                InitializeModuleInfo( moduleInfo );
                moduleInfo.includeDirs = fileMap;
            }
        } 
    }
}

/*---------------------------------------------------------------------------*/

void
FindSubDirsWithHeaders( TProjectInfo& projectInfo         ,
                        TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Locating headers which apply to all platforms" );
    
    // Add all generic headers
    FindSubDirsWithHeaders( projectInfo     ,
                            moduleInfoEntry ,
                            AllPlatforms    );

    // Add platform specific headers
    const TStringSet& platforms = GetSupportedPlatforms();
    TStringSet::const_iterator i = platforms.begin();
    while ( i != platforms.end() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Locating headers which apply to platform " + (*i) );
        
        FindSubDirsWithHeaders( projectInfo     ,
                                moduleInfoEntry ,
                                (*i)            );

        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
FindSubDirsWithSource( TProjectInfo& projectInfo         ,
                       TModuleInfoEntry& moduleInfoEntry ,
                       const CORE::CString& platform     )
{GUCEF_TRACE;

    TStringVectorMap fileMap;
    FindSubDirsWithFileTypes( projectInfo               , 
                              fileMap                   , 
                              GetSourceFileExtensions() ,
                              platform                  ,
                              false                     ,
                              moduleInfoEntry.rootDir   ,
                              ""                        );
                              
    if ( !fileMap.empty() )
    {
        TModuleInfoMap::iterator i = moduleInfoEntry.modulesPerPlatform.find( platform );
        if ( i != moduleInfoEntry.modulesPerPlatform.end() )
        {  
            if ( (*i).second.considerSubDirs )
            {
                MergeStringVectorMap( (*i).second.sourceDirs, fileMap, true );
            }
            else
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Skipping processing subdirs since recursive processing is disabled" );
            }
        }
        else        
        {
            // If the files we are looking for are not by definition for a specific
            // platform via a platform dir then how they are processed depends on 
            // the module definition. If the module is specific to a certain platform
            // then we don't want to add a all platforms definition but rather apply
            // the files to all platforms for which the module is valid according to
            // its module definition. This is a critical difference between 'AllPlatforms'
            // and specific platforms which is important for proper overrides/merging etc
            if ( AllPlatforms == platform || platform.IsNULLOrEmpty() )
            {
                i = moduleInfoEntry.modulesPerPlatform.begin();
                while ( i != moduleInfoEntry.modulesPerPlatform.end() )
                {
                    if ( (*i).second.considerSubDirs )
                    {
                        TModuleInfo& moduleInfo = (*i).second;
                        MergeStringVectorMap( moduleInfo.sourceDirs, fileMap, true );
                    }
                    else
                    {
                        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Skipping processing subdirs since recursive processing is disabled" );
                    }

                    ++i;
                }
            }
            else
            {
                TModuleInfo& moduleInfo = moduleInfoEntry.modulesPerPlatform[ platform ];
                InitializeModuleInfo( moduleInfo );
                moduleInfo.sourceDirs = fileMap;
            }
        } 
    }
}

/*---------------------------------------------------------------------------*/

void
FindSubDirsWithSource( TProjectInfo& projectInfo         ,
                       TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Locating sources which apply to all platforms" );
    
    // Add all generic sources
    FindSubDirsWithSource( projectInfo     ,
                           moduleInfoEntry ,
                           AllPlatforms    );

    // Add platform specific source
    const TStringSet& platforms = GetSupportedPlatforms();
    TStringSet::const_iterator i = platforms.begin();
    while ( i != platforms.end() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Locating sources which apply to platform " + (*i) );
        
        FindSubDirsWithSource( projectInfo     ,
                               moduleInfoEntry ,
                               (*i)            );

        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
LegacyCMakeProcessProjectDir( TProjectInfo& projectInfo         ,
                              TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    CORE::CString pathToSuffixFile = moduleInfoEntry.rootDir;
    CORE::AppendToPath( pathToSuffixFile, "CMakeListsSuffix.txt" );
    
    CORE::CString cmakeListSuffixFileContent;
    if ( !CORE::LoadTextFileAsString( pathToSuffixFile, cmakeListSuffixFileContent ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load legacy project file: " + pathToSuffixFile );
        return;
    }
    
    if ( CORE::LoadTextFileAsString( pathToSuffixFile, cmakeListSuffixFileContent ) )
    {
        // get a platform entry to use
        // this legacy system only supported AllPlatforms via CMake
        TModuleInfo& moduleInfo = *FindModuleInfoForPlatform( moduleInfoEntry, AllPlatforms, true );

        // Set a project name based off the module sub-dir name
        // Best we can do unless we can get it from the suffix file later
        moduleInfo.name = CORE::LastSubDir( moduleInfoEntry.rootDir );
        
        // Fill in the dependencies as specified in the suffix file
        CORE::CString actualModuleName;
        moduleInfo.dependencies = CMakeParseDependencies( cmakeListSuffixFileContent, actualModuleName );
        moduleInfo.dependencyIncludeDirs = CMakeParseIncludeDirs( cmakeListSuffixFileContent );
        bool isExecutable = false;
        CMakeParseModuleProperties( cmakeListSuffixFileContent, actualModuleName, isExecutable );
        
        // This legacy system only supports this simple test
        moduleInfo.moduleType = isExecutable ? MODULETYPE_EXECUTABLE : MODULETYPE_SHARED_LIBRARY;
        
        CMakeParseSuffixFile( moduleInfo, cmakeListSuffixFileContent );
        if ( actualModuleName != moduleInfo.name )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Detected module name that differs from module sub-dir name, correcting module name from \"" + moduleInfo.name + "\" to \"" + actualModuleName + "\"" );
            moduleInfo.name = actualModuleName;
        }

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed suffix file for project " + moduleInfo.name );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to locate the required module file " + pathToSuffixFile );
    }
}

/*---------------------------------------------------------------------------*/

void
ProcessProjectDir( TProjectInfo& projectInfo         ,
                   TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    CORE::CString pathToModuleInfoFile = moduleInfoEntry.rootDir;
    CORE::AppendToPath( pathToModuleInfoFile, "ModuleInfo.xml" );
    
    if ( CORE::FileExists( pathToModuleInfoFile ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processing ModuleInfo file " + pathToModuleInfoFile );        
        DeserializeModuleInfo( moduleInfoEntry, pathToModuleInfoFile );
        
        // If there is any module info specified for 'AllPlatforms' but it does not have a 
        // module name set then we shall determine a default which is the based ont he directory the
        // project is in. Note that platform specific info can overwrite this of course but at least
        // this gives us a default if they don't have a name specified either.
        TModuleInfo* moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, AllPlatforms, false );
        if ( NULL != moduleInfo && moduleInfo->name.IsNULLOrEmpty() )
        {        
            // Set a project name based off the module sub-dir name
            moduleInfo->name = CORE::LastSubDir( moduleInfoEntry.rootDir ); 
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Setting module name to " + moduleInfo->name + " based on the sub-dir name because no other name is available" ); 
        }
    }
    else
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Attempting to process legacy cmake suffix file" );
        LegacyCMakeProcessProjectDir( projectInfo     ,
                                      moduleInfoEntry );
    }

    FindSubDirsWithHeaders( projectInfo, moduleInfoEntry );
    FindSubDirsWithSource( projectInfo, moduleInfoEntry );

    // If we have a module name then use it for our logging output
    // we want to be able to see in the log which modules where successfully processed
    CORE::CString consensusModuleName = GetConsensusModuleName( moduleInfoEntry );

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed module " + consensusModuleName + " from project dir: " + moduleInfoEntry.rootDir );
}

/*---------------------------------------------------------------------------*/

void
PreprocessDir( const CORE::CString& path )
{GUCEF_TRACE;

    CDirPreprocessorManager& dirPreprocessorManager = CProjectGenGlobal::Instance()->GetDirPreprocessorManager();
    const CDirPreprocessorManager::TDirPreprocessorsList& dirPreprocessorsList = dirPreprocessorManager.GetDirPreprocessors();

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "There are " + CORE::UInt32ToString( dirPreprocessorsList.size() ) + " preprocessors registered" );

    CDirPreprocessorManager::TDirPreprocessorsList::const_iterator i = dirPreprocessorsList.begin();
    while ( i != dirPreprocessorsList.end() )
    {
        if ( (*i)->ProccessDir( path ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Preprocessed directory " + path );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to preprocess directory " + path );
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
LocateAndProcessProjectDirsRecusively( TProjectInfo& projectInfo        ,
                                       const CORE::CString& topLevelDir )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_EVERYTHING, "Recursively processing directory for module info: " + topLevelDir );

    // Run any custom preprocessing logic thats registered
    PreprocessDir( topLevelDir );

    // Is this a project dir or some other dir?
    if ( IsDirAProjectDir( topLevelDir ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determined that the following directory is a project directory: " + topLevelDir );

        // Process this dir
        TModuleInfoEntry moduleInfoEntry;
        InitializeModuleInfoEntry( moduleInfoEntry );        
        moduleInfoEntry.rootDir = topLevelDir;
        ProcessProjectDir( projectInfo, moduleInfoEntry );
        projectInfo.modules.push_back( moduleInfoEntry );
    }

    // Get all subdir's
    TStringVector dirList;
    PopulateDirListFromDir( topLevelDir, dirList, AllPlatforms, false );

    // Add/subtract dirs from the list based on generator instructions
    // This early application (before module definition) of processing instructions allows us
    // to limit the number of directories that need to be processed and thus speed things up a bit
    ExcludeOrIncludeDirEntriesAsSpecifiedForDir( projectInfo, topLevelDir, dirList );

    // Process all sub-dirs
    TStringVector::iterator i = dirList.begin();
    while ( i != dirList.end() )
    {
        CORE::CString subDir = topLevelDir;
        CORE::AppendToPath( subDir, (*i) );

        LocateAndProcessProjectDirsRecusively( projectInfo, subDir );
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

int
GetModulePrio( TModuleInfoEntryPrioMap& prioMap    ,
               const CORE::CString& moduleName     ,
               const CORE::CString& targetPlatform )
{GUCEF_TRACE;

    TModuleInfoEntryPrioMap::iterator i = prioMap.begin();
    while ( i != prioMap.end() )
    {
        TModuleInfoEntry* moduleInfoEntry = (*i).second;
        const CORE::CString* currentModuleName = GetModuleName( *(*i).second, targetPlatform );
        if ( NULL != currentModuleName )
        {
            if ( *currentModuleName == moduleName )
            {
                return (*i).first;
            }
        }
        ++i;
    }
    return -1;
}

/*---------------------------------------------------------------------------*/

CORE::UInt32
GetModuleDependencyCount( const TModuleInfoEntry& moduleInfoEntry ,
                          const CORE::CString& targetPlatform     )
{GUCEF_TRACE;

    // Get dependencies which apply to all platforms
    CORE::UInt32 dependencyCount = 0;
    TModuleInfoMap::const_iterator n = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( n != moduleInfoEntry.modulesPerPlatform.end() )
    {
        dependencyCount = (*n).second.dependencies.size();
    }    
    
    // Get dependencies which are specific for the target platform
    if ( targetPlatform != AllPlatforms )
    {
        n = moduleInfoEntry.modulesPerPlatform.find( targetPlatform );
        if ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            dependencyCount += (*n).second.dependencies.size();
        }
    }
    return dependencyCount;  
}

/*---------------------------------------------------------------------------*/

TModuleInfoEntryPtrVector
GetModulesWithDependencyCountOf( TModuleInfoEntryVector& modules     ,
                                 CORE::UInt32 dependencyCount        ,
                                 const CORE::CString& targetPlatform )
{GUCEF_TRACE;

    TModuleInfoEntryPtrVector resultSet;
    TModuleInfoEntryVector::iterator i = modules.begin();
    while ( i != modules.end() )
    {
        CORE::UInt32 moduleDependencyCount = GetModuleDependencyCount( (*i), targetPlatform );
        if ( moduleDependencyCount == dependencyCount )
        {
            resultSet.push_back( &(*i) );
        }
        ++i;
    }
    return resultSet;
}

/*---------------------------------------------------------------------------*/

CORE::UInt32
GetHighestDependencyCount( TModuleInfoEntryVector& modulesForAllPlatforms ,
                           const CORE::CString& targetPlatform            )
{GUCEF_TRACE;

    CORE::UInt32 greatestDependencyCount = 0;
    TModuleInfoEntryVector::iterator i = modulesForAllPlatforms.begin();
    while ( i != modulesForAllPlatforms.end() )
    {
        CORE::UInt32 dependencyCount = GetModuleDependencyCount( (*i), targetPlatform );
        if ( dependencyCount > greatestDependencyCount )
        {
            greatestDependencyCount = dependencyCount;
        }
        ++i;
    }
    return greatestDependencyCount;
}

/*---------------------------------------------------------------------------*/

void
DetermineBuildOrderForAllModules( TProjectInfo& projectInfo            ,
                                  const CORE::CString& targetPlatform  )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determining the build order for all modules using platform \"" + targetPlatform + "\",..." );

    TModuleInfoEntryPrioMap prioMap;

    // First we make sorting easier by putting all modules in the priority list
    // in such a way that they are already sorted somewhat based on their dependency count
    int prioInc=0;
    CORE::UInt32 highestDependencyCount = GetHighestDependencyCount( projectInfo.modules, targetPlatform );
    for ( CORE::UInt32 i=0; i<=highestDependencyCount; ++i )
    {
        // Grab a list of modules with *i* dependencies
        TModuleInfoEntryPtrVector modules = GetModulesWithDependencyCountOf( projectInfo.modules, i, targetPlatform );
        TModuleInfoEntryPtrVector::iterator n = modules.begin();
        while ( n != modules.end() )
        {
            prioMap[ prioInc ] = (*n);
            ++n;
            ++prioInc;
        }
    }
    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Performed initial sorting based on number of dependencies. The target platform is \"" + targetPlatform + "\"" );

    // Now we can bubble sort the priority map, because of the initial sorting done above
    // the number of iterations should be somewhat reduced.
    bool changes = true;
    while ( changes )
    {
        changes = false;

        TModuleInfoEntryPrioMap::iterator n = prioMap.begin();
        while ( n != prioMap.end() )
        {
            int modulePrio = (*n).first;
            TModuleInfoEntry* moduleInfoEntry = (*n).second;
            
            TModuleInfo* moduleInfo = FindModuleInfoForPlatform( *moduleInfoEntry, targetPlatform, false );
            if ( NULL == moduleInfo && targetPlatform != AllPlatforms )
            {
                // If no platform specific info is available we will use the info which applies to all platforms
                moduleInfo = FindModuleInfoForPlatform( *moduleInfoEntry, AllPlatforms, false );
                if ( NULL != moduleInfo )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Using AllPlatforms definition for module " + moduleInfo->name + " for build order determination since no definition was provided for platform \"" + targetPlatform + "\"" );
                }
            }
            else
            if ( targetPlatform != AllPlatforms )
            {
                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Using platform specific definition for module " + moduleInfo->name + " for build order determination. The target platform is \"" + targetPlatform + "\"" );
            }
            if ( NULL != moduleInfo )
            {
                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Module \"" + moduleInfo->name + "\" currently has build order " + CORE::Int32ToString( moduleInfo->buildOrder ) + ". The target platform is \"" + targetPlatform + "\""  );
            
                TStringVector::iterator m = moduleInfo->dependencies.begin();
                while ( m != moduleInfo->dependencies.end() )
                {
                    // Logically we cannot have a prio higher then the dependency
                    // so we will ensure it is lower
                    int dependencyPrio = GetModulePrio( prioMap, (*m), targetPlatform );
                    if ( dependencyPrio >= modulePrio )
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Module " + moduleInfo->name + " with build order index " + CORE::Int32ToString( modulePrio ) +
                                    " has dependency " + (*m) + " which has build order index " + CORE::Int32ToString( dependencyPrio ) + ", the dependency should be build before the module that requires it!"  );

                        TModuleInfoEntryPrioMap newPrioMap;

                        // Set the new priority, the priority should be higher then the dependency
                        // causing it to be build after the dependency (lower prio = builder earlier)
                        modulePrio = dependencyPrio+1;

                        // Now insert our reprioritized item at this location
                        newPrioMap[ modulePrio ] = moduleInfoEntry;

                        // Now add everything around the reprioritized item to our
                        // new prio map
                        TModuleInfoEntryPrioMap::iterator p = prioMap.begin();
                        while ( p != prioMap.end() )
                        {
                            const CORE::CString* otherName = GetModuleName( *(*p).second, targetPlatform ); 
                            const CORE::CString* thisName = GetModuleName( *moduleInfoEntry, targetPlatform );
                                
                            if ( (*p).first < modulePrio )
                            {
                                if ( ( ( NULL != otherName ) && ( NULL != thisName ) ) &&
                                     ( *otherName != *thisName )                        )
                                {
                                    newPrioMap[ (*p).first ] = (*p).second;
                                }
                            }
                            else
                            if ( (*p).first >= modulePrio )
                            {
                                if ( ( ( NULL != otherName ) && ( NULL != thisName ) ) &&
                                     ( *otherName != *thisName )                        )
                                {
                                    newPrioMap[ (*p).first + 1 ] = (*p).second;
                                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Changed build priority for module: " + *otherName +
                                                " from " + CORE::Int32ToString( (*p).first ) + " to " + CORE::Int32ToString( (*p).first+1 ) );
                                }
                            }
                            ++p;
                        }

                        // Reindex list to close gap
                        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Reindexing the build priority list" );
                        CORE::Int32 i=0;
                        TModuleInfoEntryPrioMap newestPrioMap;
                        p = newPrioMap.begin();
                        while ( p != newPrioMap.end() )
                        {
                            newestPrioMap[ i ] = (*p).second;

                            const CORE::CString* moduleName = GetModuleName( *(*p).second, targetPlatform );
                            if ( i != (*p).first )
                            {
                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Changed build priority for module: " + *moduleName +
                                            " from " + CORE::Int32ToString( (*p).first ) + " to " + CORE::Int32ToString( i ) );
                            }
                            ++i; ++p;
                        }

                        #ifdef GUCEF_CORE_DEBUG_MODE

                        // For debug: output final differeces between the altered list and the original
                        TModuleInfoEntryPrioMap::iterator q = prioMap.begin();
                        p = newestPrioMap.begin();
                        while ( p != newestPrioMap.end() )
                        {
                            if ( (*p).second != (*q).second )
                            {
                                CORE::CString moduleName1 = GetModuleNameAlways( *(*p).second, targetPlatform );
                                CORE::CString moduleName2 = GetModuleNameAlways( *(*q).second, targetPlatform );
                            
                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Difference with original build order: module " + moduleName1 +
                                            " is now at index " + CORE::Int32ToString( (*p).first ) + " where module " + moduleName2 + " used to be" );
                            }
                            ++q; ++p;
                        }

                        #endif

                        // Replace the old map with the new one and start the next bubbling iteration
                        prioMap = newestPrioMap;
                        changes = true;

                        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Completed changing the build priority for module: " + moduleInfo->name + ". The target platform is \"" + targetPlatform + "\"" );
                        break;
                    }
                    ++m;
                }

                // Restart the process if something had to be changed
                if ( changes )
                {
                    break;
                }
            }
            ++n;                
        }
    }

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Finished determining the correct build order (priority) for all modules, assigning priorities and reordering modules to reflect this" );

    // Assign the determined build order index to the modules
    TModuleInfoEntryPrioMap::iterator n = prioMap.begin();
    while ( n != prioMap.end() )
    {
        const CORE::CString* moduleName = NULL;
        TModuleInfo* moduleInfo = FindModuleInfoForPlatform( *(*n).second, targetPlatform, false );
        if ( NULL == moduleInfo )
        {
            // No module info is available for this platform
            // We will use the AllPlatforms info but keep in mind we cannot change it's build order since
            // this sorting is platform specific
            moduleInfo = FindModuleInfoForPlatform( *(*n).second, AllPlatforms, false );
            if ( NULL != moduleInfo )
            {
                moduleName = &moduleInfo->name;
                
                // We do have a 'AllPlatforms' entry for this module instead of a platform specific one
                // Lets check if it's existing buildOrder can be used. In order for this to happen the build order
                // must be identical for 'AllPlatforms' and this target platform
                if ( ( moduleInfo->buildOrder == -1 )         ||
                     ( moduleInfo->buildOrder != (*n).first )  )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "The build order for module " + *moduleName + " needs to be " 
                        + CORE::Int32ToString( (*n).first ) + " for platform \"" + targetPlatform + "\", however no platform specific module definition exists. The build order is "
                        "different from the AllPlatforms build order which is " + CORE::Int32ToString( moduleInfo->buildOrder ) 
                        + " as such we will have to create a platform specific entry for this module to retain the different build order" );

                    // We will have to just create a new entry for this platform because we need to store the platform specific build order
                    moduleInfo = FindModuleInfoForPlatform( *(*n).second, targetPlatform, true );
                }                        
                else
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "The build order for module " + *moduleName + " needs to be " 
                        + CORE::Int32ToString( (*n).first ) + " for platform \"" + targetPlatform + "\", however no platform specific module definition exists."
                        " Because a 'AllPlatforms' module defintion does exist for this module and its build order is the same we can just use the 'AllPlatforms' build order for this module" );
                        
                    // we can use the existing 'AllPlatforms' build order, no need to generate a platform entry
                    ++n;
                    continue;
                }
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Skipping module entry since no 'AllPlatforms' defintion exists nor does a definition exist for the target platform which is \"" + targetPlatform + "\"" );
                ++n;
                continue;
            }

        }
        else
        {
            moduleName = &moduleInfo->name;
        }
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Assigning build order " + CORE::Int32ToString( (*n).first ) + " to module " + *moduleName + " for the target platform which is \"" + targetPlatform + "\"" );
        moduleInfo->buildOrder = (*n).first;
        ++n;
    }

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Finished assigning the correct build order for all modules and sorted them accordingly" );
}

/*-------------------------------------------------------------------------*/

void
DetermineBuildOrderForAllModules( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    // Important: First determine the build order which applies to all modules
    // This build order will be used for specific platforms as well unless no 'AllPlatforms'
    // target is available or if the build order differs
    // Doing it this way cuts down on the number platform spefic entries generated just to store
    // the build order
    DetermineBuildOrderForAllModules( projectInfo, AllPlatforms );
    
    // Now determine the build order for all the other platforms
    const TStringSet& supportedPlatforms = GetSupportedPlatforms();
    TStringSet::const_iterator i = supportedPlatforms.begin();
    while ( i != supportedPlatforms.end() )
    {
        DetermineBuildOrderForAllModules( projectInfo, (*i).Lowercase() );        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

TMutableModuleInfoEntryPairVector
FindModulesWhichDependOnModule( TProjectInfo& projectInfo           ,
                                const CORE::CString& targetPlatform ,
                                const CORE::CString& dependencyName )
{GUCEF_TRACE;

    // Loop trough all modules and check if they depend on the given module
    TMutableModuleInfoEntryPairVector resultList;
    TModuleInfoEntryVector::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfoEntry& moduleInfoEntry = (*i);
        TModuleInfoMap::iterator n = moduleInfoEntry.modulesPerPlatform.find( targetPlatform );
        if ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            // This module has info for for this module, check its type
            TModuleInfo& moduleInfo = (*n).second;
            if ( IsStringInList( moduleInfo.dependencies, false, dependencyName ) )
            {
                TMutableModuleInfoEntryPair infoPair( &moduleInfoEntry, &moduleInfo );
                resultList.push_back( infoPair );
            }
        }
        
        ++i;
    }

    return resultList;
}

/*-------------------------------------------------------------------------*/

void
MergeIntegrationLocationsIntoModuleForPlatform( TProjectInfo& projectInfo              ,
                                                const CORE::CString& targetPlatform    ,
                                                const TModuleInfo& moduleInfoToMergeIn ,
                                                const CORE::CString& codeIncludeRoot   ,
                                                const TModuleType moduleType           )
{GUCEF_TRACE;

    // First find all the modules which depend on this integration location
    TMutableModuleInfoEntryPairVector targetModules = FindModulesWhichDependOnModule( projectInfo              ,
                                                                                      targetPlatform           ,
                                                                                      moduleInfoToMergeIn.name );

    // Now for each of these modules merge in the files
    TMutableModuleInfoEntryPairVector::iterator i = targetModules.begin();
    while ( i != targetModules.end() )
    {
        TModuleInfoEntry& moduleInfoEntry = *(*i).first;
        TModuleInfo& moduleInfo = *(*i).second;
        
        // Determine the relative path to this other module
        CORE::CString pathToCodeLocation = CORE::GetRelativePathToOtherPathRoot( moduleInfoEntry.rootDir ,
                                                                                 codeIncludeRoot         );
        
        // merge in the headers for both header integration locations as well as code integration locations
        TStringVectorMap::const_iterator n;
        
        if ( ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfo.moduleType )  ||
             ( MODULETYPE_HEADER_INTEGRATE_LOCATION == moduleInfo.moduleType ) )
        {
            n = moduleInfoToMergeIn.includeDirs.begin();
            while ( n != moduleInfoToMergeIn.includeDirs.end() )
            {
                // Create the full path to the files
                CORE::CString fullPathToIncludeLocation = pathToCodeLocation;
                CORE::AppendToPath( fullPathToIncludeLocation, (*n).first );

                // Use the merge function just in case this location is already added by other means
                TStringVector& targetVector = moduleInfo.includeDirs[ fullPathToIncludeLocation ];
                MergeStringVector( targetVector, (*n).second, true );
                              
                ++n;
            }
        }

        if ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfo.moduleType )
        {
            n = moduleInfoToMergeIn.sourceDirs.begin();
            while ( n != moduleInfoToMergeIn.sourceDirs.end() )
            {
                // Create the full path to the files
                CORE::CString fullPathToSourceLocation = pathToCodeLocation;
                CORE::AppendToPath( fullPathToSourceLocation, (*n).first );

                // Use the merge function just in case this location is already added by other means
                TStringVector& targetVector = moduleInfo.sourceDirs[ fullPathToSourceLocation ];
                MergeStringVector( targetVector, (*n).second, true );
                              
                ++n;
            }
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
MergeIntegrationLocationsIntoModuleForPlatform( TProjectInfo& projectInfo           ,
                                                const CORE::CString& targetPlatform )
{GUCEF_TRACE;

    // Loop trough all modules and process each integration as we go
    TModuleInfoEntryVector::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfoEntry& moduleInfoEntry = (*i);
        TModuleInfoMap::iterator n = moduleInfoEntry.modulesPerPlatform.find( targetPlatform );
        if ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            // This module has info for for this module, check its type
            TModuleInfo& moduleInfo = (*n).second;
            if ( ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfo.moduleType ) ||
                 ( MODULETYPE_HEADER_INTEGRATE_LOCATION == moduleInfo.moduleType ) )
            {
                // We found a code include location, now process it for all modules which proclaim to have a dependency on it
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processing integration location labeled as \"" + moduleInfo.name + "\" for platform " + targetPlatform );
                MergeIntegrationLocationsIntoModuleForPlatform( projectInfo             ,
                                                                targetPlatform          ,
                                                                moduleInfo              ,
                                                                moduleInfoEntry.rootDir ,
                                                                moduleInfo.moduleType   );
            }
        }
        
        ++i;
    }

}

/*-------------------------------------------------------------------------*/

TModuleInfoEntryPtrSet
FindModulesWhichDependOnModule( TProjectInfo& projectInfo           ,
                                const CORE::CString& dependencyName )
{GUCEF_TRACE;

    // Loop trough all modules and check if they depend on the given module
    TModuleInfoEntryPtrSet resultList;
    TModuleInfoEntryVector::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfoEntry& moduleInfoEntry = (*i);
        TModuleInfoMap::iterator n = moduleInfoEntry.modulesPerPlatform.begin();
        while ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            // Check if this module depends on the module we are looking for irregardless of platform
            TModuleInfo& moduleInfo = (*n).second;
            if ( IsStringInList( moduleInfo.dependencies, false, dependencyName ) )
            {
                resultList.insert( &moduleInfoEntry );
            }
            ++n;
        }
        
        ++i;
    }

    return resultList;
}

/*-------------------------------------------------------------------------*/

void
MergeIntegrationLocationsIntoModuleForAllPlatformsPlatform( TProjectInfo& projectInfo              ,
                                                            const TModuleInfo& moduleInfoToMergeIn ,
                                                            const CORE::CString& codeIncludeRoot   )
{GUCEF_TRACE;

    // First find all the modules which depend on this code include location
    TModuleInfoEntryPtrSet targetModules = FindModulesWhichDependOnModule( projectInfo              ,
                                                                           moduleInfoToMergeIn.name );

    // Now for each of these modules merge in the files
    TModuleInfoEntryPtrSet::iterator i = targetModules.begin();
    while ( i != targetModules.end() )
    {
        TModuleInfoEntry& moduleInfoEntry = *(*i);
        
        // Determine the relative path to this other module
        CORE::CString pathToCodeLocation = CORE::GetRelativePathToOtherPathRoot( moduleInfoEntry.rootDir ,
                                                                                 codeIncludeRoot         );
        
        TModuleInfoMap::iterator m = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
        if ( m != moduleInfoEntry.modulesPerPlatform.end() )
        {
            TModuleInfo& moduleInfo = (*m).second;
            
            TStringVectorMap::const_iterator n;
            
            if ( ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfoToMergeIn.moduleType )   ||
                 ( MODULETYPE_HEADER_INTEGRATE_LOCATION == moduleInfoToMergeIn.moduleType )  )
            {
                n = moduleInfoToMergeIn.includeDirs.begin();
                while ( n != moduleInfoToMergeIn.includeDirs.end() )
                {
                    // Create the full path to the files
                    CORE::CString fullPathToIncludeLocation = pathToCodeLocation;
                    CORE::AppendToPath( fullPathToIncludeLocation, (*n).first );

                    // Use the merge function just in case this location is already added by other means
                    TStringVector& targetVector = moduleInfo.includeDirs[ fullPathToIncludeLocation ];
                    MergeStringVector( targetVector, (*n).second, true );
                              
                    ++n;
                }
            }

            if ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfoToMergeIn.moduleType )
            {
                n = moduleInfoToMergeIn.sourceDirs.begin();
                while ( n != moduleInfoToMergeIn.sourceDirs.end() )
                {
                    // Create the full path to the files
                    CORE::CString fullPathToSourceLocation = pathToCodeLocation;
                    CORE::AppendToPath( fullPathToSourceLocation, (*n).first );

                    // Use the merge function just in case this location is already added by other means
                    TStringVector& targetVector = moduleInfo.sourceDirs[ fullPathToSourceLocation ];
                    MergeStringVector( targetVector, (*n).second, true );
                              
                    ++n;
                }
            }
        }
        else
        {
            // Since there is no AllPlatforms entry we will merge into each of the platform specific
            // entries which requires the code include dependency
            TModuleInfoMap::iterator m = moduleInfoEntry.modulesPerPlatform.begin();
            while ( m != moduleInfoEntry.modulesPerPlatform.end() )
            {
                TModuleInfo& moduleInfo = (*m).second;

                // Check if this particular platform needs the dependency
                if ( IsStringInList( moduleInfo.dependencies, false, moduleInfoToMergeIn.name ) )
                {
                    TStringVectorMap::const_iterator n;
                    
                    if ( ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfoToMergeIn.moduleType )   ||
                         ( MODULETYPE_HEADER_INTEGRATE_LOCATION == moduleInfoToMergeIn.moduleType )  )
                    {                    
                        n = moduleInfoToMergeIn.includeDirs.begin();
                        while ( n != moduleInfoToMergeIn.includeDirs.end() )
                        {
                            // Create the full path to the files
                            CORE::CString fullPathToIncludeLocation = pathToCodeLocation;
                            CORE::AppendToPath( fullPathToIncludeLocation, (*n).first );

                            // Use the merge function just in case this location is already added by other means
                            TStringVector& targetVector = moduleInfo.includeDirs[ fullPathToIncludeLocation ];
                            MergeStringVector( targetVector, (*n).second, true );
                              
                            ++n;
                        }
                    }

                    if ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfoToMergeIn.moduleType )
                    {
                        n = moduleInfoToMergeIn.sourceDirs.begin();
                        while ( n != moduleInfoToMergeIn.sourceDirs.end() )
                        {
                            // Create the full path to the files
                            CORE::CString fullPathToSourceLocation = pathToCodeLocation;
                            CORE::AppendToPath( fullPathToSourceLocation, (*n).first );

                            // Use the merge function just in case this location is already added by other means
                            TStringVector& targetVector = moduleInfo.sourceDirs[ fullPathToSourceLocation ];
                            MergeStringVector( targetVector, (*n).second, true );
                              
                            ++n;
                        }
                    }
                }
                ++m;
            }
        }

        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
MergeIntegrationLocationsIntoModuleForAllPlatformsPlatform( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    // Loop trough all modules and process each code include as we go
    TModuleInfoEntryVector::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfoEntry& moduleInfoEntry = (*i);
        TModuleInfoMap::iterator n = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
        if ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            // This module has info for for this module, check its type
            TModuleInfo& moduleInfo = (*n).second;
            if ( ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfo.moduleType )   ||
                 ( MODULETYPE_HEADER_INTEGRATE_LOCATION == moduleInfo.moduleType )  )
            {
                // We found a code include location, now process it for all modules which proclaim to have a dependency on it
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processing integration location labeled as \"" + moduleInfo.name + "\" for platform " + AllPlatforms );
                MergeIntegrationLocationsIntoModuleForAllPlatformsPlatform( projectInfo             ,
                                                                            moduleInfo              ,
                                                                            moduleInfoEntry.rootDir );
            }
        }
        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
RemoveDependencyToModule( TProjectInfo& projectInfo       ,
                          const CORE::CString& moduleName )
{GUCEF_TRACE;

    // Loop trough all modules and process each as we go
    TModuleInfoEntryVector::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfoEntry& moduleInfoEntry = (*i);
        TModuleInfoMap::iterator n = moduleInfoEntry.modulesPerPlatform.begin();
        while ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            RemoveString( (*n).second.dependencies, moduleName );
            ++n;
        }        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
RemoveDependenciesOnIntegrationLocations( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    // Loop trough all modules and process each code include as we go
    TModuleInfoEntryVector::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfoEntry& moduleInfoEntry = (*i);
        TModuleInfoMap::iterator n = moduleInfoEntry.modulesPerPlatform.begin();
        while ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            TModuleInfo& moduleInfo = (*n).second;
            if ( ( MODULETYPE_CODE_INTEGRATE_LOCATION == moduleInfo.moduleType ) ||
                 ( MODULETYPE_HEADER_INTEGRATE_LOCATION == moduleInfo.moduleType ) )
            {
                // We found a code include location, now process it for all modules which proclaim to have a dependency on it
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Removing dependency labeled as \"" + moduleInfo.name + "\" for platform " + (*n).first + " because it is an integration location and it has been processed" );
                RemoveDependencyToModule( projectInfo     ,
                                          moduleInfo.name );
            }
            ++n;
        }
        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
MergeIntegrationLocationsIntoModule( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    MergeIntegrationLocationsIntoModuleForAllPlatformsPlatform( projectInfo );
    
    const TStringSet& supportedPlatforms = GetSupportedPlatforms();
    TStringSet::const_iterator i = supportedPlatforms.begin();
    while ( i != supportedPlatforms.end() )
    {
        MergeIntegrationLocationsIntoModuleForPlatform( projectInfo, (*i).Lowercase() );        
        ++i;
    }

    RemoveDependenciesOnIntegrationLocations( projectInfo );
}

/*-------------------------------------------------------------------------*/

CDirCrawlingProjectInfoGatherer::CDirCrawlingProjectInfoGatherer( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CDirCrawlingProjectInfoGatherer::~CDirCrawlingProjectInfoGatherer()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CDirCrawlingProjectInfoGatherer::GatherInfo( const TStringVector& rootDirs ,
                                             TProjectInfo& projectInfo     )
{GUCEF_TRACE;

    // Gather all processing instructions
    TStringVector::const_iterator i = rootDirs.begin();
    while ( i != rootDirs.end() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Recursively loading all processing instructions for root directory \"" + (*i) + "\"" );
        LoadAllProcessingInstructions( projectInfo, CORE::RelativePath( (*i) ) );
        ++i;
    }

    // Gather all module information
    i = rootDirs.begin();
    while ( i != rootDirs.end() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Identifying all modules for root directory \"" + (*i) + "\"" );
        LocateAndProcessProjectDirsRecusively( projectInfo, CORE::RelativePath( (*i) ) );
        ++i;
    }

    // Merge headers and code from integration locations into modules
    MergeIntegrationLocationsIntoModule( projectInfo );

    // Based on all the information we have gathered we can now determine the correct build order
    // for all platforms
    DetermineBuildOrderForAllModules( projectInfo );

    // Now we can generate all the include paths
    // this functionality relies on the build orders having been determined ahead of time
    GenerateDependencyIncludes( projectInfo );
    
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGEN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
