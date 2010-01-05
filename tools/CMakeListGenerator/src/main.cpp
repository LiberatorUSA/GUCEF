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

struct SModuleInfo
{
    std::vector< CORE::CString > dependencies;    
    std::vector< CORE::CString > includeDirs;
    std::vector< CORE::CString > sourceDirs;
    CORE::CString name;
    CORE::CString rootDir;
    
    CORE::CString cmakeListFileContent;
    CORE::CString cmakeListSuffixFileContent;
};
typedef struct SModuleInfo TModuleInfo;

/*---------------------------------------------------------------------------*/

struct SProjectInfo
{
    std::vector< TModuleInfo > modules;
};
typedef struct SProjectInfo TProjectInfo;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PopulateFileListFromDir( const CORE::CString& path              , 
                         std::vector< CORE::CString >& fileList )
{GUCEF_TRACE;

    CORE::SDI_Data* sdiData = CORE::DI_First_Dir_Entry( path.C_String() );
    if ( NULL != sdiData )
    {
        do
        {
            if ( 0 != DI_Is_It_A_File( sdiData ) )
            {
                fileList.push_back( DI_Name( sdiData ) );
            }
        }
        while ( 0 != DI_Next_Dir_Entry( sdiData ) );
        DI_Cleanup( sdiData );
    }
}

/*---------------------------------------------------------------------------*/

void
PopulateDirListFromDir( const CORE::CString& path              , 
                         std::vector< CORE::CString >& dirList )
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
GenerateCMakeListsFileSection( const CORE::CString& sectionContent       , 
                               const CORE::CString& filePrefix           , 
                               const std::vector< CORE::CString >& files )
{GUCEF_TRACE;

    CORE::CString newSectionContent = sectionContent;
    std::vector< CORE::CString >::const_iterator i = files.begin();
    while ( i != files.end() )
    {
        CORE::CString path = filePrefix;
        CORE::AppendToPath( path, (*i) ); 
        path = path.ReplaceChar( '\\', '/' );
        
        newSectionContent += "  " + path + "\n";
        ++i;
    }
    newSectionContent += ")\n\n";
    return newSectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileIncludeSection( const std::vector< CORE::CString >& includeFiles )
{GUCEF_TRACE;

    CORE::CString sectionContent = "set(HEADER_FILES \n";
    sectionContent = GenerateCMakeListsFileSection( sectionContent, "include/", includeFiles );
    return sectionContent + "include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )\n\n";
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileSrcSection( const std::vector< CORE::CString >& includeFiles )
{GUCEF_TRACE;

    CORE::CString sectionContent = "set(SOURCE_FILES \n";
    return GenerateCMakeListsFileSection( sectionContent, "src/", includeFiles );
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
        
        moduleInfo.includeDirs.push_back( platformSubDirSeg );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Discovered valid platform sub-dir " + platformSubDirSeg );
        
        std::vector< CORE::CString > subFiles;
        PopulateFileListFromDir( platformSubDir, subFiles );        
        
        sectionContent = GenerateCMakeListsFileSection( sectionContent, "  " + platformSubDirSeg, subFiles );
        
        sectionContent += "  include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/" + platformSubDirSeg + " )\n";
        sectionContent += "  set(PLATFORM_HEADER_INSTALL \"" + platformName + "\")\n";
        sectionContent += "  source_group( \"Platform Header Files\" FILES ${PLATFORM_SOURCE_FILES} )\n\n";
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
        
        moduleInfo.sourceDirs.push_back( platformSubDirSeg );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Discovered valid platform sub-dir " + platformSubDirSeg );
        
        std::vector< CORE::CString > subFiles;
        PopulateFileListFromDir( platformSubDir, subFiles );

        sectionContent = GenerateCMakeListsFileSection( sectionContent, "  " + platformSubDirSeg, subFiles );
        
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

std::vector< CORE::CString >
ParseDependencies( const CORE::CString& fileSuffix )
{GUCEF_TRACE;

    CORE::CString testStr = fileSuffix.Lowercase();
    CORE::Int32 subStrIdx = testStr.HasSubstr( "add_dependencies(", true );
    if ( subStrIdx >= 0 )
    {
        CORE::CString dependenciesStr = fileSuffix.SubstrToChar( ')', (CORE::UInt32)subStrIdx+17, true );
        dependenciesStr = dependenciesStr.CompactRepeatingChar( ' ' );
        return dependenciesStr.ParseElements( ' ' );
    }
    return std::vector< CORE::CString >();
}

/*---------------------------------------------------------------------------*/

void
GenerateCMakeListsFile( const TProjectInfo& projectInfo ,
                        TModuleInfo& moduleInfo         )
{GUCEF_TRACE;

    CORE::CString includeDir = moduleInfo.rootDir;
    CORE::AppendToPath( includeDir, "include" );
    moduleInfo.includeDirs.push_back( "include" );
    
    CORE::CString srcDir = moduleInfo.rootDir;
    CORE::AppendToPath( srcDir, "src" );
    moduleInfo.sourceDirs.push_back( "src" );
    
    std::vector< CORE::CString > includeFiles;
    PopulateFileListFromDir( includeDir, includeFiles );
    
    std::vector< CORE::CString > srcFiles;
    PopulateFileListFromDir( srcDir, srcFiles );

    // Set file header comment section
    CORE::CString fileContent = GetCMakeListsFileHeader();
    
    // Set project name comment section
    fileContent += "\n# Configure " + moduleInfo.name + "\n\n";
    
    // Add all the include files
    fileContent += GenerateCMakeListsFileIncludeSection( includeFiles );
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( includeFiles.size() ) + " include files for project " + moduleInfo.name );
    
    // Add all the source files
    fileContent += GenerateCMakeListsFileSrcSection( srcFiles );
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( srcFiles.size() ) + " source files for project " + moduleInfo.name );
    
    // Add all platform files, headers and source
    fileContent += GenerateCMakeListsFilePlatformFilesSection( moduleInfo );
    
    // We have completed generating the file content and gathering info
    moduleInfo.cmakeListFileContent = fileContent;
}

/*---------------------------------------------------------------------------*/

void
ProcessProjectDir( const TProjectInfo& projectInfo , 
                   TModuleInfo& moduleInfo         )
{GUCEF_TRACE;
   
    moduleInfo.name = CORE::LastSubDir( moduleInfo.rootDir ); 
    
    CORE::CString pathToSuffixFile = moduleInfo.rootDir;
    CORE::AppendToPath( pathToSuffixFile, "CMakeListsSuffix.txt" );
    
    if ( CORE::LoadTextFileAsString( pathToSuffixFile, moduleInfo.cmakeListSuffixFileContent ) )
    {
        // Fill in the dependencies as specified in the suffix file
        moduleInfo.dependencies = ParseDependencies( fileSuffix );
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed suffix file for project " + moduleInfo.name );
    }
    else
    {
        // add suffix example section instead
        moduleInfo.cmakeListSuffixFileContent  = "## TODO: the following is an example suffix section, you have to complete it\n";
        moduleInfo.cmakeListSuffixFileContent += "#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)\n";
        moduleInfo.cmakeListSuffixFileContent += "#add_definitions(-DTIXML_USE_STL)\n";
        moduleInfo.cmakeListSuffixFileContent += "#add_executable(" + moduleInfo.name + " ${HEADER_FILES} ${SOURCE_FILES})\n";
        moduleInfo.cmakeListSuffixFileContent += "#target_link_libraries(" + moduleInfo.name + " ${GUCEF_LIBRARIES})\n";
        moduleInfo.cmakeListSuffixFileContent += "#gucef_config_tool(" + moduleInfo.name + ")\n";
    }
    
    GenerateCMakeListsFile( projectInfo, moduleInfo );
    
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Generated CMake List file content for project dir: " + moduleInfo.rootDir );
}

/*---------------------------------------------------------------------------*/

bool
IsDirAProjectDir( CORE::CString dir )
{GUCEF_TRACE;

    CORE::CString includeDir = dir;
    CORE::AppendToPath( includeDir, "include" );
    CORE::CString srcDir = dir;
    CORE::AppendToPath( srcDir, "src" );
    
    return 0 != CORE::Is_Path_Valid( includeDir.C_String() ) &&
           0 != CORE::Is_Path_Valid( srcDir.C_String() );
}

/*---------------------------------------------------------------------------*/

std::vector<CORE::CString>
GetSubDirExcludeList( CORE::CString dir )
{GUCEF_TRACE;

    CORE::CString excludeFile = dir;
    CORE::AppendToPath( excludeFile, "CMakeGenExcludeList.txt" );
    
    if ( CORE::FileExists( excludeFile ) )
    {
        CORE::CString excludeFileContent;
        if ( CORE::LoadTextFileAsString( excludeFile        ,
                                         excludeFileContent ) )
        {
            return excludeFileContent.ParseElements( '\n' );            
        } 
    }
    
    return std::vector<CORE::CString>();
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
        ProcessProjectDir( projectInfo, moduleInfo );        
        projectInfo.modules.push_back( moduleInfo );
    }
    
    // Get all subdir's
    std::vector< CORE::CString > dirList;
    PopulateDirListFromDir( topLevelDir, dirList );
    
    // Get list of dirs to exclude
    std::vector< CORE::CString > dirExcludeList = GetSubDirExcludeList( topLevelDir );
    
    // Remove excluded dirs from dir list
    std::vector< CORE::CString >::iterator i = dirExcludeList.begin();
    while ( i != dirExcludeList.end() )
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
                                              
    CORE::CString topLevelProjectDir;
    try
    {
        topLevelProjectDir = keyValueList.GetValue( "rootDir" );
    }
    catch ( CORE::CValueList::EUnknownKey& )
    {
        topLevelProjectDir = CORE::RelativePath( "$CURWORKDIR$" );
    }
    
    // Generate all the data
    TProjectInfo projectInfo;
    LocateAndProcessProjectDirsRecusively( projectInfo, topLevelProjectDir );
    
    // Write all the CMakeLists.txt files
    std::vector< TModuleInfo >::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfo& moduleInfo = (*i);
        
        CORE::CString pathToCMakeListsFile = moduleInfo.rootDir;
        CORE::AppendToPath( pathToCMakeListsFile, "CMakeLists.txt" );
        
        CORE::CString fileContent = moduleInfo.cmakeListFileContent + moduleInfo.cmakeListSuffixFileContent;        
        CORE::WriteStringAsTextFile( pathToCMakeListsFile, fileContent );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Created CMakeLists.txt file for project dir: " + moduleInfo.rootDir );
        ++i;
    }

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
