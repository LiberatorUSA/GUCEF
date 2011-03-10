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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefProjectGenerator_CCMakeProjectGenerator.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PROJECTGENERATOR {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static const std::string AllPlatforms = "all";

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
        platformMap[ "WIN32" ].insert( "mswin" );
        platformMap[ "WIN32" ].insert( "win32" );
        platformMap[ "WIN64" ].insert( "mswin" );
        platformMap[ "WIN64" ].insert( "win64" );
        platformMap[ "UNIX" ].insert( "linux" );
        platformMap[ "UNIX" ].insert( "unix" );
        platformMap[ "IPHONEOS" ].insert( "iphone" );
        platformMap[ "SYMBIAN" ].insert( "symbian" );
        platformMap[ "OSX" ].insert( "osx" );
        platformMap[ "OSX" ].insert( "mac" );
        platformMap[ "ANDROID" ].insert( "android" );
        platformMap[ "GLX" ].insert( "glx" );
        platformMap[ "GTK" ].insert( "gtk" );
        platformMap[ "SDL" ].insert( "sdl" );
    }
    return platformMap;
}

/*-------------------------------------------------------------------------*/

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

/*-------------------------------------------------------------------------*/

const CORE::CString&
GetCMakeListsFileHeader( bool addCompileDate = false )
{GUCEF_TRACE;

    static bool initialized = false;
    static CORE::CString fileHeader;
    static CORE::CString fileHeaderWithCompileDate;

    if ( !initialized )
    {
        fileHeader =
        "#-------------------------------------------------------------------\n"
        "# This file is part of a CMake build system designed for GUCEF\n"
        "#     (Galaxy Unlimited Framework)\n"
        "# For the latest info, see http://www.VanvelzenSoftware.com/\n"
        "#\n"
        "# The contents of this file are placed in the public domain. Feel\n"
        "# free to make use of it in any way you like.\n"
        "#-------------------------------------------------------------------\n"
        "#\n";

        fileHeaderWithCompileDate = fileHeader +
        "# THIS FILE IS AUTOMATICALLY GENERATED BY CMAKELISTGENERATOR\n"
        "#\n"
        "# CMAKELISTGENERATOR was compiled on: " __DATE__ " @ " __TIME__ "\n"
        "#\n"
        "#\n"
        "#\n";

        initialized = true;
    }

    if ( addCompileDate )
    {
        return fileHeaderWithCompileDate;
    }
    return fileHeader;
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
GenerateCMakeListsFileIncludeSection( const TModuleInfoEntry& moduleInfoEntry  ,
                                      const CORE::CString& consensusModuleName )
{GUCEF_TRACE;

    CORE::CString sectionContent;
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {        
        const TStringVectorMap& includeFiles = (*i).second.includeDirs;
        
        sectionContent = "set( HEADER_FILES \n";
        sectionContent = GenerateCMakeListsFileSection( sectionContent, includeFiles );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( includeFiles.size() ) + " include dirs for module " + consensusModuleName );
    }
    else
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "The module " + consensusModuleName + " does not have include directories which apply to all platforms, skipping general include section" ); 
    }
    
    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileSrcSection( const TModuleInfoEntry& moduleInfoEntry  ,
                                  const CORE::CString& consensusModuleName )
{GUCEF_TRACE;

    CORE::CString sectionContent;
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {        
        const TStringVectorMap& srcFiles = (*i).second.sourceDirs;
        
        sectionContent = "set( SOURCE_FILES \n";
        sectionContent = GenerateCMakeListsFileSection( sectionContent, srcFiles );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( srcFiles.size() ) + " include dirs for module " + consensusModuleName );
    }
    else
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "The module " + consensusModuleName + " does not have include directories which apply to all platforms, skipping general include section" ); 
    }
    
    return sectionContent;
}

/*---------------------------------------------------------------------------*/

void
GenerateCMakeListsFilePlatformFilesSection( const TModuleInfoEntry& moduleInfoEntry ,
                                            const CORE::CString& platformName       ,
                                            CORE::CString& headerSection            ,
                                            CORE::CString& sourceSection            ,
                                            bool& hasPlatformHeaderFiles            ,
                                            bool& hasPlatformSourceFiles            )
{GUCEF_TRACE;

    TModuleInfoMap::const_iterator m = moduleInfoEntry.modulesPerPlatform.find( platformName );
    if ( m != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const TStringVectorMap& platformHeaderFiles = (*m).second.includeDirs;
        if ( !platformHeaderFiles.empty() )
        {
            hasPlatformHeaderFiles = true;
            headerSection = "  set( PLATFORM_HEADER_FILES \n";

            TStringVectorMap::const_iterator n = platformHeaderFiles.begin();
            while ( n != platformHeaderFiles.end() )
            {
                const TStringVector& platformHeaderFilesDir = (*n).second;
                TStringVector::const_iterator i = platformHeaderFilesDir.begin();
                while ( i != platformHeaderFilesDir.end() )
                {
                    CORE::CString path = (*n).first;
                    CORE::AppendToPath( path, (*i) );
                    path = path.ReplaceChar( '\\', '/' );

                    headerSection += "    " + path + "\n";

                    ++i;
                }
                ++n;
            }
            headerSection += "  )\n\n";

            // Add additional platform specific includes
            //headerSection += "  include_directories( ";
            //n = platformHeaderFiles.begin();
            //while ( n != platformHeaderFiles.end() )
            //{
            //    headerSection += (*n).first + " ";
            //    ++n;
            //}
            //headerSection += ")\n";

            headerSection += "  set( PLATFORM_HEADER_INSTALL \"" + platformName + "\" )\n";
            headerSection += "  source_group( \"Platform Header Files\" FILES ${PLATFORM_HEADER_FILES} )\n\n";
        }
        
        const TStringVectorMap& platformSourceFiles = (*m).second.sourceDirs;
        if ( !platformSourceFiles.empty() )
        {
            hasPlatformSourceFiles = true;
            sourceSection = "  set( PLATFORM_SOURCE_FILES \n";

            TStringVectorMap::const_iterator n = platformSourceFiles.begin();
            while ( n != platformSourceFiles.end() )
            {
                const TStringVector& platformSourceFilesDir = (*n).second;
                TStringVector::const_iterator i = platformSourceFilesDir.begin();
                while ( i != platformSourceFilesDir.end() )
                {
                    CORE::CString path = (*n).first;
                    CORE::AppendToPath( path, (*i) );
                    path = path.ReplaceChar( '\\', '/' );

                    sourceSection += "    " + path + "\n";
                    ++i;
                }
                ++n;
            }
            sourceSection += "  )\n\n";

            sourceSection += "  set( PLATFORM_SOURCE_INSTALL \"" + platformName + "\" )\n";
            sourceSection += "  source_group( \"Platform Source Files\" FILES ${PLATFORM_SOURCE_FILES} )\n\n";
        }
    }
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFilePlatformFilesSection( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    bool hasPlatformHeaderFiles = false;
    bool hasPlatformSourceFiles = false;

    CORE::CString sectionContent;
    bool firstPlatform = true;
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.begin();
    while ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        CORE::CString headerSection;
        CORE::CString sourceSection;
        const CORE::CString& platformName = (*i).first;
        GenerateCMakeListsFilePlatformFilesSection( moduleInfoEntry        ,
                                                    platformName           ,
                                                    headerSection          ,
                                                    sourceSection          ,
                                                    hasPlatformHeaderFiles ,
                                                    hasPlatformSourceFiles );

        if ( firstPlatform )
        {
            sectionContent = "\n\nif (" + platformName + ")\n" + headerSection + sourceSection;
            firstPlatform = false;
        }
        else
        {
            sectionContent += "elseif (" + platformName + ")\n" + headerSection + sourceSection;
        }

        ++i;
    }

    if ( hasPlatformHeaderFiles || hasPlatformSourceFiles )
    {
        // since we added data we have to close the section
        sectionContent += "endif ()\n\n";
    }

    // Make sure the variable is defined even if we have no platform files
    // This allows people to always add the variable in the suffix file without knowing
    // if there are platform files
    if ( !hasPlatformHeaderFiles )
    {
        sectionContent += "# Make sure the PLATFORM_HEADER_FILES variable is always defined\n set( PLATFORM_HEADER_FILES \"\" )\n\n";
    }
    if ( !hasPlatformSourceFiles )
    {
        sectionContent += "# Make sure the PLATFORM_SOURCE_FILES variable is always defined\n set( PLATFORM_SOURCE_FILES \"\" )\n\n";
    }

    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleIncludesSection( const TModuleInfo& moduleInfo ,
                                    const CORE::CString& rootDir  )
{GUCEF_TRACE;

    // Add include dirs for each dependency we know about
    CORE::CString allRelDependencyPaths;
    const TStringSet& includeDirs = moduleInfo.dependencyIncludeDirs;
    TStringSet::const_iterator i = includeDirs.begin();
    while ( i != includeDirs.end() )
    {
        allRelDependencyPaths += (*i) + " ";
        ++i;
    }

    // Add all the regular include dirs for this module
    TStringVectorMap::const_iterator n = moduleInfo.includeDirs.begin();
    while ( n != moduleInfo.includeDirs.end() )
    {
        CORE::CString includeDir = (*n).first.ReplaceChar( '\\', '/' );
        if ( 0 != includeDir.Length() )
        {
            allRelDependencyPaths += includeDir + " ";
        }
        else
        {
            // Check if there is more then one include dir
            // If so we have create an include for an empty include dir
            // to ensure files in subdirs can include the file with the zero length
            // subdir.
            if ( 1 < moduleInfo.includeDirs.size() )
            {
                allRelDependencyPaths += "../" + CORE::LastSubDir( rootDir ) + " ";
            }
        }
        ++n;
    }

    CORE::CString sectionContent;
    if ( allRelDependencyPaths.Length() > 0 )
    {
        sectionContent = "include_directories( " + allRelDependencyPaths + ")\n";
    }
    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleIncludesSection( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;
    
    CORE::CString sectionContent;
    
    // First add the include section which applies to all platforms
    // it should not have an 'if' check around it
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    { 
        sectionContent += "\n";
        sectionContent += GenerateCMakeModuleIncludesSection( (*i).second, moduleInfoEntry.rootDir );
    }
        
    // Now add the include paths which are platform specific
    i = moduleInfoEntry.modulesPerPlatform.begin();
    while ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const CORE::CString& platformName = (*i).first; 
        if ( platformName != AllPlatforms )
        {            
            CORE::CString platformSection = GenerateCMakeModuleIncludesSection( (*i).second, moduleInfoEntry.rootDir );
            if ( platformSection.Length() > 0 )
            {
                sectionContent += "\nif ( "+ platformName + " )\n  ";
                sectionContent += platformSection;
                sectionContent += ")\nendif( " + platformName + " )\n";                
            }
        }
    }
    
    return sectionContent;
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

CORE::CString
LoadLegacyCMakeListsSuffixFileFromDisk( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    // Mainly meant to support backwards compatibility with the old way this tool
    // used to work which is with the use of CMakeListsSuffix.txt files
    // Archives should be updated to the new way of working which is not CMake
    // specific
    
    CORE::CString suffixFilePath = moduleInfoEntry.rootDir;
    CORE::AppendToPath( suffixFilePath, "CMakeListsSuffix.txt" );
    
    CORE::CString fileContent;
    CORE::LoadTextFileAsString( suffixFilePath, fileContent );
    return fileContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileContent( const TModuleInfoEntry& moduleInfoEntry ,
                               bool addCompileDate = false             )
{GUCEF_TRACE;

    // Set file header comment section
    CORE::CString fileContent = GetCMakeListsFileHeader( addCompileDate );

    // Determine the general consensus module name
    CORE::CString consensusModuleName = GetConsensusModuleName( moduleInfoEntry );
    
    // Set project name comment section
    fileContent += "\n# Configuration for module: " + consensusModuleName + "\n\n";

    // Add all the general include files
    fileContent += GenerateCMakeListsFileIncludeSection( moduleInfoEntry, consensusModuleName );

    // Add all the source files
    fileContent += GenerateCMakeListsFileSrcSection( moduleInfoEntry, consensusModuleName );

    // Add all platform files, headers and source
    fileContent += GenerateCMakeListsFilePlatformFilesSection( moduleInfoEntry );

    fileContent += "# For ease of use make a variable that has all files for this module\nset( ALL_FILES ${HEADER_FILES} ${SOURCE_FILES} ${PLATFORM_HEADER_FILES} ${PLATFORM_SOURCE_FILES} )\n\n";

    // Check if we need to add a legacy suffix file section
    CORE::CString suffixFileContent = LoadLegacyCMakeListsSuffixFileFromDisk( moduleInfoEntry );
    if ( suffixFileContent.Length() > 0 )
    {
        fileContent += GenerateAutoGenertedSeperator( true );
        fileContent += suffixFileContent;
        fileContent += GenerateAutoGenertedSeperator( false );
    }
    else
    {
        //@TODO: use generated info
    }
    
    // Add all the include directories for this module
    fileContent += GenerateCMakeModuleIncludesSection( moduleInfoEntry );
    
    // We are done generating the content for the CMake file
    return fileContent;
}

/*---------------------------------------------------------------------------*/

void
WriteCMakeListsFilesToDisk( const TProjectInfo& projectInfo  ,
                            const CORE::CString& logFilename ,
                            bool addCompileDate = false      )
{GUCEF_TRACE;

    // Write all the CMakeLists.txt files
    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        const TModuleInfoEntry& moduleInfoEntry = (*i);

        CORE::CString fileContent = GenerateCMakeListsFileContent( moduleInfoEntry, addCompileDate );
        if ( logFilename.Length() > 0 )
        {
            fileContent += "\n# Generator logfile can be found at: " + logFilename;
        }

        CORE::CString pathToCMakeListsFile = moduleInfoEntry.rootDir;
        CORE::AppendToPath( pathToCMakeListsFile, "CMakeLists.txt" );

        if ( CORE::WriteStringAsTextFile( pathToCMakeListsFile, fileContent ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Created CMakeLists.txt file for project dir: " + moduleInfoEntry.rootDir );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to write CMakeLists.txt file content to disk at path " + moduleInfoEntry.rootDir );
        }
        ++i;
    }
}

/*--------------------------------------------------------------------------*/

CCMakeProjectGenerator::CCMakeProjectGenerator( void )
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/
    
CCMakeProjectGenerator::~CCMakeProjectGenerator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CCMakeProjectGenerator::GenerateProject( TProjectInfo& projectInfo            ,
                                         const CORE::CString& outputDir       ,
                                         bool addGeneratorCompileTimeToOutput )
{GUCEF_TRACE;

    CORE::CString logFilename;  //@TODO: get these from settings
    bool addCompileDate = false;
    
    // Write the gathered info to disk in CMakeList.txt format
    WriteCMakeListsFilesToDisk( projectInfo, logFilename, addCompileDate );   
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGENERATOR */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
