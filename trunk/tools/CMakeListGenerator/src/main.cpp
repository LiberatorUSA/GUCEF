/*
 *  CMakeListGenerator: Tool to generate CMakeList.txt with headers and sources
 *  Copyright (C) 2002 - 2009.  Dinand Vanvelzen
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

#include <stdio.h>

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

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifdef GUCEF_MSWIN_BUILD
#include <windows.h>
#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*---------------------------------------------------------------------------*/

typedef std::vector< CORE::CString > TStringVector;
typedef std::map< CORE::CString, TStringVector > TStringVectorMap;

/*---------------------------------------------------------------------------*/

struct SModuleInfo
{
    TStringVector dependencies;    
    TStringVectorMap includeDirs;
    TStringVectorMap sourceDirs;
    CORE::CString name;
    CORE::CString rootDir;
    
    CORE::CString cmakeListFileContentPreSuffix;
    CORE::CString cmakeListSuffixFileContent;
    CORE::CString cmakeListFileContentPostSuffix;
    
    int buildOrder;
};
typedef struct SModuleInfo TModuleInfo;

/*---------------------------------------------------------------------------*/

typedef std::vector< TModuleInfo > TModuleInfoVector;
typedef std::vector< TModuleInfo* > TModuleInfoPtrVector;

/*---------------------------------------------------------------------------*/

struct SProjectInfo
{
    TModuleInfoVector modules;
};
typedef struct SProjectInfo TProjectInfo;

/*---------------------------------------------------------------------------*/

typedef std::map< int, TModuleInfo* > TModulePrioMap;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

TStringVector
GetSourceFileExtensions( void )
{GUCEF_TRACE;

    TStringVector fileTypes;
    fileTypes.push_back( "c" );
    fileTypes.push_back( "cpp" );
    fileTypes.push_back( "cxx" );
    fileTypes.push_back( "asm" );
    return fileTypes;
}

/*---------------------------------------------------------------------------*/

TStringVector
GetHeaderFileExtensions( void )
{GUCEF_TRACE;

    TStringVector fileTypes;
    fileTypes.push_back( "h" );
    fileTypes.push_back( "hpp" );
    fileTypes.push_back( "asm" );
    return fileTypes;
}

/*---------------------------------------------------------------------------*/

bool
RemoveString( std::vector< CORE::CString >& list , 
              const CORE::CString& searchStr     )
{GUCEF_TRACE;

    bool removedString = false;
    std::vector< CORE::CString >::iterator i = list.begin();
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

bool
IsDirAProjectDir( CORE::CString dir )
{GUCEF_TRACE;

    // We detect module directories in 2 ways
    // Either is has to have certain directories or
    // it has to have a CMakeListsSuffix.txt file
    
    CORE::CString includeDir = dir;
    CORE::AppendToPath( includeDir, "include" );
    CORE::CString srcDir = dir;
    CORE::AppendToPath( srcDir, "src" );
    CORE::CString suffixFilePath = dir;
    CORE::AppendToPath( suffixFilePath, "CMakeListsSuffix.txt" );
    
    return ( CORE::IsPathValid( includeDir ) &&
             CORE::IsPathValid( srcDir )      ) ||
           CORE::FileExists( suffixFilePath );  
}

/*---------------------------------------------------------------------------*/

std::vector<CORE::CString>
GetExcludeList( CORE::CString dir )
{GUCEF_TRACE;

    CORE::CString excludeFile = dir;
    CORE::AppendToPath( excludeFile, "CMakeGenExcludeList.txt" );
    
    if ( CORE::FileExists( excludeFile ) )
    {
        CORE::CString excludeFileContent;
        if ( CORE::LoadTextFileAsString( excludeFile        ,
                                         excludeFileContent ) )
        {
            excludeFileContent = excludeFileContent.RemoveChar( '\r' );
            return excludeFileContent.ParseElements( '\n' );            
        } 
    }
    
    return std::vector<CORE::CString>();
}

/*---------------------------------------------------------------------------*/

bool
IsStringInList( const TStringVector& list       ,
                bool caseSensitive              ,
                const CORE::CString& testString )
{GUCEF_TRACE;

    TStringVector::const_iterator i = list.begin();
    while ( i != list.end() )
    {
        if ( (*i).Equals( testString, caseSensitive ) )
        {
            return true;
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
PopulateFileListFromDir( const CORE::CString& path      ,
                         const TStringVector& fileTypes , 
                         TStringVector& fileList        )
{GUCEF_TRACE;

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
PopulateDirListFromDir( const CORE::CString& path , 
                        TStringVector& dirList    )
{GUCEF_TRACE;

    CORE::SDI_Data* sdiData = CORE::DI_First_Dir_Entry( path.C_String() );
    if ( NULL != sdiData )
    {
        do
        {
            if ( 0 == DI_Is_It_A_File( sdiData ) )
            {
                CORE::CString dirName = DI_Name( sdiData ); 
                if ( ( dirName != "." ) && ( dirName != ".." ) )
                {
                    dirList.push_back( dirName );
                }
            }
        }
        while ( 0 != DI_Next_Dir_Entry( sdiData ) );
        DI_Cleanup( sdiData );
    }
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetCMakeListsFileHeader( void )
{GUCEF_TRACE;

    return
    "#-------------------------------------------------------------------\n"
    "# This file is part of the CMake build system for GUCEF\n"
    "#     (Galaxy Unlimited Framework)\n"
    "# For the latest info, see http://www.VanvelzenSoftware.com/\n"
    "#\n"
    "# The contents of this file are placed in the public domain. Feel\n"
    "# free to make use of it in any way you like.\n"
    "#-------------------------------------------------------------------\n\n"
    "#\n"
    "# THIS FILE IS AUTOMATICALLY GENERATED BY CMAKELISTGENERATOR\n"
    "#\n"
    "#\n"
    "#\n"
    "# -- Basic CMake concepts:\n"
    "#      CMAKE_MODULE_PATH\n"
    "#      – Path to where the CMake modules are located\n"
    "#      CMAKE_INSTALL_PREFIX\n"
    "#      – Where to put files when calling 'make install'\n"
    "#      CMAKE_BUILD_TYPE\n"
    "#      – Type of build (Debug, Release, ...)\n"
    "#      BUILD_SHARED_LIBS\n"
    "#      – Switch between shared and static libraries\n";
    "#\n"
    "# Variables can be changed directly in the build files (CMakeLists.txt) or through\n"
    "# the command line by prefixing a variable's name with '-D':\n"
    "#               Example: cmake -DBUILD_SHARED_LIBS=OFF\n"
    "#           Here BUILD_SHARED_LIBS is the variable prefixed by -D\n"
    "#\n"
    "#\n";    
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileSection( const CORE::CString& sectionContent , 
                               const TStringVectorMap& fileMap     )
{GUCEF_TRACE;

    CORE::CString newSectionContent = sectionContent;
    TStringVectorMap::const_iterator i = fileMap.begin();
    while ( i != fileMap.end() )
    {
        TStringVector::const_iterator n = (*i).second.begin();
        while ( n != (*i).second.end() )
        {        
            CORE::CString path = (*i).first;
            CORE::AppendToPath( path, (*n) ); 
            path = path.ReplaceChar( '\\', '/' );
            
            newSectionContent += "  " + path + "\n";
            ++n;
        }
        ++i;
    }
    newSectionContent += ")\n\n";
    return newSectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileIncludeSection( const TStringVectorMap& includeFiles )
{GUCEF_TRACE;

    CORE::CString sectionContent = "set(HEADER_FILES \n";
    return GenerateCMakeListsFileSection( sectionContent, includeFiles );
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileSrcSection( const TStringVectorMap& srcFiles )
{GUCEF_TRACE;

    CORE::CString sectionContent = "set(SOURCE_FILES \n";
    return GenerateCMakeListsFileSection( sectionContent, srcFiles );
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsPlatformFilesSection( TModuleInfo& moduleInfo           ,
                                        const CORE::CString& platformName ,
                                        const CORE::CString& platformDir  ,
                                        bool firstPlatform                ,
                                        bool& validPlatform               )
{GUCEF_TRACE;

    bool hasPlatformIncludes = false;
    bool hasPlatformSrc = false;
    
    CORE::CString includeDir = moduleInfo.rootDir;
    CORE::AppendToPath( includeDir, "include" );
    CORE::CString srcDir = moduleInfo.rootDir;
    CORE::AppendToPath( srcDir, "src" );

    CORE::CString sectionContent;
    CORE::CString subDirLastSeg = CORE::LastSubDir( moduleInfo.rootDir ); 

    if ( firstPlatform )
    {
        sectionContent = "if (" + platformName + ")\n";
    }
    else
    {
        sectionContent = "elseif (" + platformName + ")\n";
    }
    
    CORE::CString platformSubDir = moduleInfo.rootDir;
    CORE::AppendToPath( platformSubDir, "include" );    
    CORE::AppendToPath( platformSubDir, platformDir );
    if ( CORE::IsPathValid( platformSubDir ) )
    {
        hasPlatformIncludes = true;
        sectionContent += "  set(PLATFORM_HEADER_FILES \n";
        
        CORE::CString platformSubDirSeg = "include";
        CORE::AppendToPath( platformSubDirSeg, platformDir );
        platformSubDirSeg = platformSubDirSeg.ReplaceChar( '\\', '/' );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Discovered valid platform sub-dir " + platformSubDirSeg );
        
        TStringVector subFiles;
        PopulateFileListFromDir( platformSubDir, GetHeaderFileExtensions(), subFiles );
        
        moduleInfo.includeDirs.insert( std::pair< CORE::CString, TStringVector >( platformSubDirSeg, subFiles ) );        
        
        TStringVectorMap filesMap;
        filesMap[ "  " + platformSubDirSeg ] = subFiles;
        sectionContent = GenerateCMakeListsFileSection( sectionContent, filesMap );
        
        sectionContent += "  include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/" + platformSubDirSeg + " )\n";
        sectionContent += "  set(PLATFORM_HEADER_INSTALL \"" + platformName + "\")\n";
        sectionContent += "  source_group( \"Platform Header Files\" FILES ${PLATFORM_HEADER_FILES} )\n\n";
    }
    else
    {
        hasPlatformIncludes = false;
    }
    
    platformSubDir = moduleInfo.rootDir;
    CORE::AppendToPath( platformSubDir, "src" );    
    CORE::AppendToPath( platformSubDir, platformDir );
    if ( CORE::IsPathValid( platformSubDir ) )
    {
        hasPlatformSrc = true;
        
        if ( firstPlatform )
        {
            sectionContent += "  set(PLATFORM_SOURCE_FILES \n";
        }
        else
        {
            sectionContent += "elseif (" + platformName + ")\n  set(PLATFORM_SOURCE_FILES \n";
        }
        
        CORE::CString platformSubDirSeg = "src";
        CORE::AppendToPath( platformSubDirSeg, platformDir );        
        platformSubDirSeg = platformSubDirSeg.ReplaceChar( '\\', '/' );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Discovered valid platform sub-dir " + platformSubDirSeg );
        
        std::vector< CORE::CString > subFiles;
        PopulateFileListFromDir( platformSubDir, GetSourceFileExtensions(), subFiles );
        
        moduleInfo.sourceDirs.insert( std::pair< CORE::CString, TStringVector >( platformSubDirSeg, subFiles ) );

        TStringVectorMap filesMap;
        filesMap[ "  " + platformSubDirSeg ] = subFiles;
        sectionContent = GenerateCMakeListsFileSection( sectionContent, filesMap );
        
        sectionContent += "  set(PLATFORM_SOURCE_INSTALL \"" + platformName + "\")\n";
        sectionContent += "  source_group( \"Platform Source Files\" FILES ${PLATFORM_SOURCE_FILES} )\n\n";
    }
    else
    {
        hasPlatformSrc = false;
    }
    
    if ( hasPlatformIncludes || hasPlatformSrc )
    {
        validPlatform = true;
    }
    
    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFilePlatformFilesSection( TModuleInfo& moduleInfo )
{GUCEF_TRACE;

    CORE::CString sectionContent;
    
    bool validPlatform = true;
    sectionContent += GenerateCMakeListsPlatformFilesSection( moduleInfo, "WIN32", "mswin", true, validPlatform );
    sectionContent += GenerateCMakeListsPlatformFilesSection( moduleInfo, "UNIX", "linux", !validPlatform, validPlatform );
    
    if ( sectionContent.Length() > 0 )
    {
        // since we added data we have to close the section
        sectionContent += "endif ()\n";
    }
    
    return sectionContent;
}

/*---------------------------------------------------------------------------*/

bool
IsDirAPlatformDir( const CORE::CString& path )
{GUCEF_TRACE;

    CORE::CString lastSubDir = CORE::LastSubDir( path ).Lowercase();
    return lastSubDir == "mswin" || lastSubDir == "linux";
}

/*---------------------------------------------------------------------------*/

std::vector< CORE::CString >
ParseDependencies( const CORE::CString& fileSuffix ,
                   CORE::CString& moduleName       )
{GUCEF_TRACE;

    CORE::CString testStr = fileSuffix.Lowercase();
    CORE::Int32 subStrIdx = testStr.HasSubstr( "add_dependencies(", true );
    if ( subStrIdx >= 0 )
    {
        CORE::CString dependenciesStr = fileSuffix.SubstrToChar( ')', (CORE::UInt32)subStrIdx+17, true );
        dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
        dependenciesStr = dependenciesStr.Trim( true );
        dependenciesStr = dependenciesStr.Trim( false );
        std::vector< CORE::CString > elements = dependenciesStr.ParseElements( ' ' );
        if ( !elements.empty() )
        {
            moduleName = *(elements.begin());
            elements.erase( elements.begin() );
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Found " + CORE::Int32ToString( elements.size() ) + " dependencies in suffix file" );
        }
        return elements;
    }
    return std::vector< CORE::CString >();
}

/*---------------------------------------------------------------------------*/

CORE::CString
ParseModuleName( const CORE::CString& fileSuffix )
{GUCEF_TRACE;

    CORE::CString testStr = fileSuffix.Lowercase();
    CORE::Int32 subStrIdx = testStr.HasSubstr( "add_library(", true );
    if ( subStrIdx >= 0 )
    {
        CORE::CString dependenciesStr = fileSuffix.SubstrToChar( ')', (CORE::UInt32)subStrIdx+12, true );
        dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
        dependenciesStr = dependenciesStr.Trim( true );
        dependenciesStr = dependenciesStr.Trim( false );
        std::vector< CORE::CString > elements = dependenciesStr.ParseElements( ' ' );
        if ( !elements.empty() )
        {
            return *(elements.begin());
        }
        
        // Malformed file!
        return CORE::CString();
    }
    else
    {
        subStrIdx = testStr.HasSubstr( "add_executable(", true );
        if ( subStrIdx >= 0 )
        {
            CORE::CString dependenciesStr = fileSuffix.SubstrToChar( ')', (CORE::UInt32)subStrIdx+15, true );
            dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
            dependenciesStr = dependenciesStr.Trim( true );
            dependenciesStr = dependenciesStr.Trim( false );
            std::vector< CORE::CString > elements = dependenciesStr.ParseElements( ' ' );
            if ( !elements.empty() )
            {
                return *(elements.begin());
            }
            
            // Malformed file!
            return CORE::CString();
        }
        else
        {
            CORE::CString moduleName;
            ParseDependencies( fileSuffix, moduleName );
            return moduleName;
        }
    }
}

/*---------------------------------------------------------------------------*/

const TModuleInfo*
GetModuleInfo( const TProjectInfo& projectInfo ,
               const CORE::CString& moduleName )
{GUCEF_TRACE;

    TModuleInfoVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        if ( (*i).name == moduleName )
        {
            return &(*i);
        }        
        ++i;
    }
    return NULL;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetRelativePathToOtherPathRoot( const CORE::CString& fromPath ,
                                const CORE::CString& toPath   )
{GUCEF_TRACE;

    CORE::Int32 pathEquality = (CORE::Int32) fromPath.FindMaxSubstrEquality( toPath, 0, true ); 
    CORE::CString toPathRemainder = toPath.ReplaceChar( '\\', '/' );
    CORE::CString fromPathRemainder = fromPath.ReplaceChar( '\\', '/' );
    pathEquality -= toPathRemainder.SubstrToChar( '/', pathEquality, false ).Length() - 1;    
    toPathRemainder = toPathRemainder.CutChars( pathEquality, true );
    fromPathRemainder = fromPathRemainder.CutChars( pathEquality, true ); 
    TStringVector upDirElements = fromPathRemainder.ParseElements( '/', false );
    
    CORE::CString relativePath;
    for ( CORE::UInt32 i=0; i<upDirElements.size(); ++i )
    {
        relativePath += "../";
    }
    CORE::AppendToPath( relativePath, toPathRemainder );
    relativePath = relativePath.ReplaceChar( '\\', '/' );
    return relativePath;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateModuleIncludes( const TProjectInfo& projectInfo ,
                        TModuleInfo& moduleInfo         )
{GUCEF_TRACE;

    CORE::CString allRelDependencyPaths;
    TStringVector& dependencies = moduleInfo.dependencies;
    TStringVector::iterator i = dependencies.begin();
    while ( i != dependencies.end() )
    {
        const TModuleInfo* dependencyModule = GetModuleInfo( projectInfo, (*i) );
        if ( NULL != dependencyModule )
        {
            CORE::CString relativePath = GetRelativePathToOtherPathRoot( moduleInfo.rootDir        , 
                                                                         dependencyModule->rootDir );
            
            TStringVectorMap::const_iterator n = dependencyModule->includeDirs.begin();
            while ( n != dependencyModule->includeDirs.end() )
            {
                CORE::CString dependencyInclDir = relativePath;
                CORE::AppendToPath( dependencyInclDir, (*n).first );
                dependencyInclDir = dependencyInclDir.ReplaceChar( '\\', '/' );
                
                allRelDependencyPaths += dependencyInclDir + " ";
                ++n;
            }                         
        }
        ++i;
    }

    TStringVectorMap::iterator n = moduleInfo.includeDirs.begin();
    while ( n != moduleInfo.includeDirs.end() )
    {
        CORE::CString includeDir = (*n).first.ReplaceChar( '\\', '/' );        
        allRelDependencyPaths += includeDir + " ";
        ++n;
    }
    if ( allRelDependencyPaths.Length() > 0 )
    {
        return "\ninclude_directories( " + allRelDependencyPaths + ")\n";
    }
    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateAutoGenertedSeperator( bool end )
{GUCEF_TRACE;

    CORE::CString seperator = "\n\n\n#********";
    if ( end )
    {
        seperator += " END OF AUTOGENERATED CONTENT ";
    }
    else
    {
        seperator += " START OF AUTOGENERATED CONTENT ";
    }
    seperator += "********\n\n\n";
    return seperator;
}

/*---------------------------------------------------------------------------*/

void
GenerateCMakeListsFile( const TProjectInfo& projectInfo ,
                        TModuleInfo& moduleInfo         )
{GUCEF_TRACE;
    
    // Set file header comment section
    CORE::CString fileContent = GetCMakeListsFileHeader();
    
    // Set project name comment section
    fileContent += "\n# Configure " + moduleInfo.name + "\n\n";
    
    // Add all the include files
    fileContent += GenerateCMakeListsFileIncludeSection( moduleInfo.includeDirs );
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( moduleInfo.includeDirs.size() ) + " include dirs for project " + moduleInfo.name );
    
    // Add all the source files
    fileContent += GenerateCMakeListsFileSrcSection( moduleInfo.sourceDirs );
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( moduleInfo.sourceDirs.size() ) + " source dirs for project " + moduleInfo.name );
    
    // Add all platform files, headers and source
    fileContent += GenerateCMakeListsFilePlatformFilesSection( moduleInfo );
    
    fileContent += GenerateAutoGenertedSeperator( true );
    
    // We have completed generating the file content and gathering info
    moduleInfo.cmakeListFileContentPreSuffix = fileContent;
    
    fileContent = GenerateAutoGenertedSeperator( false );
    
    // Add all the include directories for this module
    fileContent += GenerateModuleIncludes( projectInfo, moduleInfo );

    fileContent += GenerateAutoGenertedSeperator( true );
    
    moduleInfo.cmakeListFileContentPostSuffix = fileContent;
}

/*---------------------------------------------------------------------------*/

void
FindSubDirsWithFileTypes( TStringVectorMap& fileMap          ,
                          const TStringVector& fileTypes     ,
                          const CORE::CString& curRootDir    ,
                          const CORE::CString& curRootDirSeg )
{GUCEF_TRACE;

    TStringVector fileList;
    PopulateFileListFromDir( curRootDir, fileTypes, fileList );
    
    // Remove excluded files from file list
    std::vector< CORE::CString > excludeList = GetExcludeList( curRootDir );
    std::vector< CORE::CString >::iterator n = excludeList.begin();
    while ( n != excludeList.end() )
    {
        if ( RemoveString( fileList, (*n) ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Excluded the file \"" + (*n) + "\" based on the exclude list for this dir" );
        }
        ++n;
    }    
    
    if ( fileList.size() > 0 )
    {
        // found files in the current root
        fileMap[ curRootDirSeg ] = fileList;
    }
    
    // Get a list of sub-dirs
    TStringVector dirList;
    PopulateDirListFromDir( curRootDir, dirList );
    
    TStringVector::iterator i = dirList.begin();
    while ( i != dirList.end() )
    { 
        CORE::CString subDir = curRootDir;
        CORE::AppendToPath( subDir, (*i) );
        
        // Honor the exclude list
        if ( !IsStringInList( excludeList, false, (*i) ) )
        {
            // Do not recurse into other project dirs or platform dirs
            if ( !IsDirAProjectDir( subDir ) && !IsDirAPlatformDir( subDir ) ) 
            {
                CORE::CString subDirSeg = curRootDirSeg;
                CORE::AppendToPath( subDirSeg, (*i) );
                subDirSeg.ReplaceChar( '\\', '/' );
                
                FindSubDirsWithFileTypes( fileMap, fileTypes, subDir, subDirSeg );
            }
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
FindSubDirsWithHeaders( TModuleInfo& moduleInfo )
{GUCEF_TRACE;

    // Look into the root itself and recuse downwards
    FindSubDirsWithFileTypes( moduleInfo.includeDirs, GetHeaderFileExtensions(), moduleInfo.rootDir, "" );  
}

/*---------------------------------------------------------------------------*/

void
FindSubDirsWithSource( TModuleInfo& moduleInfo )
{GUCEF_TRACE;

    // Look into the root itself and recuse downwards
    FindSubDirsWithFileTypes( moduleInfo.sourceDirs, GetSourceFileExtensions(), moduleInfo.rootDir, "" );
}

/*---------------------------------------------------------------------------*/

void
ProcessProjectDir( TModuleInfo& moduleInfo )
{GUCEF_TRACE;
   
    // Set a project name based off the module sub-dir name
    // Best we can do unless we can get it from the suffix file later
    moduleInfo.name = CORE::LastSubDir( moduleInfo.rootDir ); 
    
    CORE::CString pathToSuffixFile = moduleInfo.rootDir;
    CORE::AppendToPath( pathToSuffixFile, "CMakeListsSuffix.txt" );
    
    if ( CORE::LoadTextFileAsString( pathToSuffixFile, moduleInfo.cmakeListSuffixFileContent ) )
    {
        // Fill in the dependencies as specified in the suffix file
        CORE::CString actualModuleName;
        moduleInfo.dependencies = ParseDependencies( moduleInfo.cmakeListSuffixFileContent, actualModuleName );
        actualModuleName = ParseModuleName( moduleInfo.cmakeListSuffixFileContent );
        if ( actualModuleName != moduleInfo.name )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Detected module name that differs from module sub-dir name, correcting module name from \"" + moduleInfo.name + "\" to \"" + actualModuleName + "\"" );
            moduleInfo.name = actualModuleName;
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed suffix file for project " + moduleInfo.name );
    }
    else
    {
        // add suffix example section instead
        moduleInfo.cmakeListSuffixFileContent  = "## TODO: the following is an example suffix section, you have to complete it\n";
        moduleInfo.cmakeListSuffixFileContent += "#add_definitions(-DTIXML_USE_STL)\n";
        moduleInfo.cmakeListSuffixFileContent += "#add_executable(" + moduleInfo.name + " ${HEADER_FILES} ${SOURCE_FILES})\n";
        moduleInfo.cmakeListSuffixFileContent += "#target_link_libraries(" + moduleInfo.name + " ${GUCEF_LIBRARIES})\n";
        moduleInfo.cmakeListSuffixFileContent += "#gucef_config_tool(" + moduleInfo.name + ")\n";
    }
    
    FindSubDirsWithHeaders( moduleInfo );
    FindSubDirsWithSource( moduleInfo );
    
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Generated CMake List file content for project dir: " + moduleInfo.rootDir );
}

/*---------------------------------------------------------------------------*/

void
LocateAndProcessProjectDirsRecusively( TProjectInfo& projectInfo ,
                                       CORE::CString topLevelDir )
{GUCEF_TRACE;
    
    GUCEF_LOG( CORE::LOGLEVEL_EVERYTHING, "Recursively processing directory: " + topLevelDir );
    
    // Is this a project dir or some other dir?    
    if ( IsDirAProjectDir( topLevelDir ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Determined that the following directory is a project directory: " + topLevelDir );
        
        // Process this dir
        TModuleInfo moduleInfo;
        moduleInfo.rootDir = topLevelDir;
        moduleInfo.buildOrder = 0;
        ProcessProjectDir( moduleInfo );        
        projectInfo.modules.push_back( moduleInfo );
    }
    
    // Get all subdir's
    std::vector< CORE::CString > dirList;
    PopulateDirListFromDir( topLevelDir, dirList );
    
    // Get list of dirs to exclude
    std::vector< CORE::CString > excludeList = GetExcludeList( topLevelDir );
    
    // Remove excluded dirs from dir list
    std::vector< CORE::CString >::iterator i = excludeList.begin();
    while ( i != excludeList.end() )
    {
        if ( RemoveString( dirList, (*i) ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Excluded the sub-dir \"" + (*i) + "\" based on the exclude list for this dir" );
        }
        ++i;
    }    
    
    // Process all sub-dirs
    i = dirList.begin();
    while ( i != dirList.end() )
    {
        CORE::CString subDir = topLevelDir;
        CORE::AppendToPath( subDir, (*i) );
        
        LocateAndProcessProjectDirsRecusively( projectInfo, subDir );
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
WriteCMakeListsFilesToDisk( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    // Write all the CMakeLists.txt files
    std::vector< TModuleInfo >::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfo& moduleInfo = (*i);
        
        CORE::CString pathToCMakeListsFile = moduleInfo.rootDir;
        CORE::AppendToPath( pathToCMakeListsFile, "CMakeLists.txt" );
        
        CORE::CString fileContent = moduleInfo.cmakeListFileContentPreSuffix + 
                                    moduleInfo.cmakeListSuffixFileContent +
                                    moduleInfo.cmakeListFileContentPostSuffix;
        CORE::WriteStringAsTextFile( pathToCMakeListsFile, fileContent );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Created CMakeLists.txt file for project dir: " + moduleInfo.rootDir );
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
GenerateCMakeListsContentForModules( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    // Generate all the CMakeLists.txt file content
    std::vector< TModuleInfo >::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfo& moduleInfo = (*i);     
        GenerateCMakeListsFile( projectInfo, moduleInfo );        
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Generated CMakeLists.txt file contents for project dir: " + moduleInfo.rootDir );
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

int
GetModulePrio( TModulePrioMap& prioMap         ,
               const CORE::CString& moduleName )
{GUCEF_TRACE;

    TModulePrioMap::iterator i = prioMap.begin();
    while ( i != prioMap.end() )
    {
        if ( (*i).second->name == moduleName )
        {
            return (*i).first;
        }
        ++i;
    }
    return -1;
}

/*---------------------------------------------------------------------------*/

TModuleInfoPtrVector
GetModulesWithDependencyCounfOf( TModuleInfoVector& modules   ,
                                 CORE::UInt32 dependencyCount )
{GUCEF_TRACE;

    TModuleInfoPtrVector resultSet;
    TModuleInfoVector::iterator i = modules.begin();
    while ( i != modules.end() )
    {
        if ( (*i).dependencies.size() == dependencyCount )
        {
            resultSet.push_back( &( (*i) ) );
        }
        ++i;
    }
    return resultSet;
}

/*---------------------------------------------------------------------------*/

CORE::UInt32
GetHighestDependencyCount( TModuleInfoVector& modules )
{GUCEF_TRACE;

    CORE::UInt32 greatestDependencyCount = 0;
    TModuleInfoVector::iterator i = modules.begin();
    while ( i != modules.end() )
    {
        if ( (*i).dependencies.size() > greatestDependencyCount )
        {
            greatestDependencyCount = (*i).dependencies.size();
        }
        ++i;
    }
    return greatestDependencyCount;
}

/*---------------------------------------------------------------------------*/

void
SortModulesInDependencyOrder( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Sorting all modules based on build priority,.." );
    
    TModulePrioMap prioMap;
    
    // First we make sorting easier by putting all modules in the priority list
    // in such a way that they are already sorted somewhat based on their dependency count
    int prioInc=0;
    CORE::UInt32 highestDependencyCount = GetHighestDependencyCount( projectInfo.modules );
    for ( CORE::UInt32 i=0; i<=highestDependencyCount; ++i )
    {
        // Grab a list of modules with *i* dependencies
        TModuleInfoPtrVector modules = GetModulesWithDependencyCounfOf( projectInfo.modules, i );
        TModuleInfoPtrVector::iterator n = modules.begin();
        while ( n != modules.end() )
        {
            prioMap[ prioInc ] = (*n);
            ++n;
            ++prioInc;
        }        
    }
    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Performed initial sorting based on number of dependencies" );
    
    // Now we can bubble sort the priority map, because of the initial sorting done above
    // the number of iterations should be greatly reduced.
    bool changes = true;
    while ( changes )
    {
        changes = false;
        
        TModulePrioMap::iterator n = prioMap.begin();
        while ( n != prioMap.end() )
        {
            int modulePrio = (*n).first;
            TModuleInfo* moduleInfo = (*n).second;
            
            TStringVector::iterator m = moduleInfo->dependencies.begin();
            while ( m != moduleInfo->dependencies.end() )
            {
                // Logically we cannot have a prio higher then the dependency
                // so we will ensure it is lower 
                int dependencyPrio = GetModulePrio( prioMap, (*m) );
                if ( dependencyPrio >= modulePrio )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Module " + moduleInfo->name + " with build priority " + CORE::Int32ToString( modulePrio ) + 
                                " has dependency " + (*m) + " which has build priority " + CORE::Int32ToString( dependencyPrio ) + ", the dependency should have a lower priority (thus build earlier) then the module that requires it!"  );
                    
                    TModulePrioMap newPrioMap;
                    
                    // Set the new priority, the priority should be higher then the dependency 
                    // causing it to be build after the dependency (lower prio = builder earlier)
                    modulePrio = dependencyPrio+1;

                    // Now insert our reprioritized item at this location
                    newPrioMap[ modulePrio ] = moduleInfo;

                    // Now add everything around the reprioritized item to our
                    // new prio map
                    TModulePrioMap::iterator p = prioMap.begin();
                    while ( p != prioMap.end() )
                    {                        
                        if ( (*p).first < modulePrio )
                        {
                            if ( (*p).second->name != moduleInfo->name )
                            {
                                newPrioMap[ (*p).first ] = (*p).second;
                            }
                        }
                        else
                        if ( (*p).first >= modulePrio )
                        {
                            if ( (*p).second->name != moduleInfo->name )
                            {
                                newPrioMap[ (*p).first + 1 ] = (*p).second;
                                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Changed build priority for module: " + (*p).second->name + 
                                            " from " + CORE::Int32ToString( (*p).first ) + " to " + CORE::Int32ToString( (*p).first+1 ) );
                            }                                                                                                     
                        }
                        ++p;
                    }

                    // Reindex list to close gap
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Reindexing the build priority list" );
                    CORE::Int32 i=0;                    
                    TModulePrioMap newestPrioMap;
                    p = newPrioMap.begin();
                    while ( p != newPrioMap.end() )
                    {                        
                        newestPrioMap[ i ] = (*p).second;

                        if ( i != (*p).first )
                        {                        
                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Changed build priority for module: " + (*p).second->name + 
                                        " from " + CORE::Int32ToString( (*p).first ) + " to " + CORE::Int32ToString( i ) );
                        }                        
                        ++i; ++p;
                    }
                    
                    #ifdef GUCEF_CORE_DEBUG_MODE
                    
                    // For debug: output final differeces between the altered list and the original 
                    TModulePrioMap::iterator q = prioMap.begin();
                    p = newestPrioMap.begin();
                    while ( p != newestPrioMap.end() )
                    {                        
                        if ( (*p).second != (*q).second )
                        {
                            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Difference with original build order: module " + (*p).second->name + 
                                        " is now at index " + CORE::Int32ToString( (*p).first ) + " where module " + (*q).second->name + " used to be" );
                        }                  
                        ++q; ++p;
                    }
                    
                    #endif

                    // Replace the old map with the new one and start the next bubbling iteration
                    prioMap = newestPrioMap;
                    changes = true;
                    
                    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Completed changing the build priority for module: " + moduleInfo->name );
                    break;
                }
                ++m;
            }
            
            // Restart the process if something had to be changed
            if ( changes )
            {
                break;
            }
            
            ++n;
        }        
    }
    
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Finished determining the correct build order (priority) for all modules, assigning priorities and reordering modules to refect this" );
    
    // First assign the determined build order index to the module
    TModuleInfoVector newModuleInfoVector;
    TModulePrioMap::iterator n = prioMap.begin();
    while ( n != prioMap.end() )
    {
        TModuleInfo* moduleInfo = (*n).second;
        moduleInfo->buildOrder = (*n).first;
        
        newModuleInfoVector.push_back( *moduleInfo );
        ++n;
    }
    projectInfo.modules = newModuleInfoVector;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Finished assigning the correct build order for all modules and sorted them accordingly" );
}

/*---------------------------------------------------------------------------*/

void
ParseParams( const int argc                 , 
             char* argv[]                   ,
             CORE::CValueList& keyValueList )
{GUCEF_TRACE;
    
    keyValueList.DeleteAll();
    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        // Combine the argument strings back into a single string because we don't want to use
        // a space as the seperator
        for ( int i=1; i<argc; ++i )
        {
            argString += ' ' + CORE::CString( argv[ i ] );
        }
        
        // Parse the param list based on the ' symbol
        keyValueList.SetMultiple( argString, '\'' );
    }
}

/*---------------------------------------------------------------------------*/

int
main( int argc , char* argv[] )
{GUCEF_TRACE;

    CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( logFilename, "CMakeListsGenerator_Log.txt" );
    CORE::CFileAccess logFileAccess( logFilename, "w" );

    CORE::CStdLogger logger( logFileAccess );
    CORE::CLogManager::Instance()->AddLogger( &logger );
    
    #ifdef GUCEF_MSWIN_BUILD
    CORE::CMSWinConsoleLogger consoleOut;
    CORE::CLogManager::Instance()->AddLogger( &consoleOut );
    #endif /* GUCEF_MSWIN_BUILD ? */

    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );
                                              
    TStringVector rootDirs;
    try
    {
        rootDirs = keyValueList.GetValueVector( "rootDir" );
    }
    catch ( CORE::CValueList::EUnknownKey& )
    {
        rootDirs.push_back( CORE::RelativePath( "$CURWORKDIR$" ) );
    }
    
    // Gather all the information
    TProjectInfo projectInfo;
    TStringVector::iterator i = rootDirs.begin();
    while ( i != rootDirs.end() )
    {
        LocateAndProcessProjectDirsRecusively( projectInfo, (*i) );
        ++i;
    }
    // Order the modules in the list such so that they are placed in the order they need to be build
    SortModulesInDependencyOrder( projectInfo );
    
    // Generate the contents of the CMakeLists files
    GenerateCMakeListsContentForModules( projectInfo );
    
    // Now write what we created to disk
    WriteCMakeListsFilesToDisk( projectInfo );
}

/*---------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

int __stdcall
WinMain( HINSTANCE hinstance     ,
         HINSTANCE hprevinstance ,
         LPSTR lpcmdline         ,
         int ncmdshow            )
{GUCEF_TRACE;

    int argc = 0;
    char** argv = &lpcmdline;
    if ( lpcmdline != NULL )
    {
        if ( *lpcmdline != '\0' )
        {
            argc = 1;
        }
    }

    return main( argc, argv );
}

#endif

/*---------------------------------------------------------------------------*/
