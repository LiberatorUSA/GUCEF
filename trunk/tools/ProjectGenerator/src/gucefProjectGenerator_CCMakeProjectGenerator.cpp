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

void
GenerateCMakeListsFilePlatformFilesSection( TModuleInfo& moduleInfo           ,
                                            const CORE::CString& platformName ,
                                            CORE::CString& headerSection      ,
                                            CORE::CString& sourceSection      ,
                                            bool& hasPlatformHeaderFiles      ,
                                            bool& hasPlatformSourceFiles      )
{GUCEF_TRACE;



    TStringVectorMapMap::iterator m = moduleInfo.platformHeaderFiles.find( platformName );
    if ( m != moduleInfo.platformHeaderFiles.end() )
    {
        const TStringVectorMap& platformHeaderFiles = (*m).second;
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
    }

    m = moduleInfo.platformSourceFiles.find( platformName );
    if ( m != moduleInfo.platformSourceFiles.end() )
    {
        const TStringVectorMap& platformSourceFiles = (*m).second;
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
GenerateCMakeListsFilePlatformFilesSection( TModuleInfo& moduleInfo )
{GUCEF_TRACE;

    bool hasPlatformHeaderFiles = false;
    bool hasPlatformSourceFiles = false;

    CORE::CString sectionContent;
    bool firstPlatform = true;
    TStringVectorMapMap::iterator i = moduleInfo.platformHeaderFiles.begin();
    while ( i != moduleInfo.platformHeaderFiles.end() )
    {
        CORE::CString headerSection;
        CORE::CString sourceSection;
        const CORE::CString& platformName = (*i).first;
        GenerateCMakeListsFilePlatformFilesSection( moduleInfo             ,
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
GenerateCMakeModuleIncludesSectionForPlatform( const TProjectInfo& projectInfo   ,
                                               const TModuleInfo& moduleInfo     ,
                                               const CORE::CString& platformName )
{GUCEF_TRACE;

    CORE::CString allRelDependencyPaths;

    // Add all the include dirs inherited from dependency modules
    TStringSetMap::const_iterator m = moduleInfo.dependencyPlatformIncludeDirs.find( platformName );
    if ( m != moduleInfo.dependencyPlatformIncludeDirs.end() )
    {
        const TStringSet& platformIncludes = (*m).second;
        TStringSet::const_iterator i = platformIncludes.begin();
        while ( i != platformIncludes.end() )
        {
            allRelDependencyPaths += (*i) + " ";
            ++i;
        }
    }

    // Add all the regular platform include dirs for this module itself
    TStringVectorMapMap::const_iterator t = moduleInfo.platformHeaderFiles.find( platformName );
    if ( t != moduleInfo.platformHeaderFiles.end() )
    {
        const TStringVectorMap& platformHeaderFiles = (*t).second;
        TStringVectorMap::const_iterator n = platformHeaderFiles.begin();
        while ( n != platformHeaderFiles.end() )
        {
            CORE::CString includeDir = (*n).first;
            includeDir = CORE::RelativePath( includeDir ).ReplaceChar( '\\', '/' );
            allRelDependencyPaths += includeDir + " ";
            ++n;
        }
    }

    CORE::CString sectionContent;
    if ( allRelDependencyPaths.Length() > 0 )
    {
        sectionContent = "\nif ( "+ platformName + " )\n  include_directories( " + allRelDependencyPaths + ")\nendif( "+ platformName + " )\n";
    }

    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleIncludesSectionForAllPlatforms( const TProjectInfo& projectInfo ,
                                                   const TModuleInfo& moduleInfo   )
{GUCEF_TRACE;

    CORE::CString sectionContent;

    TStringSet relevantPlatformDirs = GetSupportedPlatforms();
    TStringSet::iterator i = relevantPlatformDirs.begin();
    while ( i != relevantPlatformDirs.end() )
    {
        sectionContent += GenerateCMakeModuleIncludesSectionForPlatform( projectInfo, moduleInfo, (*i) );
        ++i;
    }
    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleIncludesSection( const TProjectInfo& projectInfo ,
                                    const TModuleInfo& moduleInfo   )
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
                allRelDependencyPaths += "../" + CORE::LastSubDir( moduleInfo.rootDir ) + " ";
            }
        }
        ++n;
    }

    CORE::CString sectionContent;
    if ( allRelDependencyPaths.Length() > 0 )
    {
        sectionContent = "\ninclude_directories( " + allRelDependencyPaths + ")\n";
    }

    sectionContent += GenerateCMakeModuleIncludesSectionForAllPlatforms( projectInfo, moduleInfo );

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

void
GenerateCMakeListsFile( const TProjectInfo& projectInfo ,
                        TModuleInfo& moduleInfo         ,
                        bool addCompileDate = false     )
{GUCEF_TRACE;

    // Set file header comment section
    CORE::CString fileContent = GetCMakeListsFileHeader( addCompileDate );

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

    fileContent += "# For ease of use make a variable that has all files for this module\nset( ALL_FILES ${HEADER_FILES} ${SOURCE_FILES} ${PLATFORM_HEADER_FILES} ${PLATFORM_SOURCE_FILES} )\n\n";

    fileContent += GenerateAutoGenertedSeperator( true );

    // We have completed generating the file content and gathering info
    moduleInfo.cmakeListFileContentPreSuffix = fileContent;

    fileContent = GenerateAutoGenertedSeperator( false );

    // Add all the include directories for this module
    fileContent += GenerateCMakeModuleIncludesSection( projectInfo, moduleInfo );

    fileContent += GenerateAutoGenertedSeperator( true );

    moduleInfo.cmakeListFileContentPostSuffix = fileContent;
}

/*---------------------------------------------------------------------------*/

void
WriteCMakeListsFilesToDisk( TProjectInfo& projectInfo        ,
                            const CORE::CString& logFilename )
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

        if ( logFilename.Length() > 0 )
        {
            fileContent += "# Generator logfile can be found at: " + logFilename;
        }

        CORE::WriteStringAsTextFile( pathToCMakeListsFile, fileContent );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Created CMakeLists.txt file for project dir: " + moduleInfo.rootDir );
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
WriteCMakeListsFilesToDisk( TProjectInfo& projectInfo )
{GUCEF_TRACE;

    WriteCMakeListsFilesToDisk( projectInfo, CORE::CString() );
}

/*---------------------------------------------------------------------------*/

void
GenerateCMakeListsContentForModules( TProjectInfo& projectInfo   ,
                                     bool addCompileDate = false )
{GUCEF_TRACE;

    // Generate all the CMakeLists.txt file content
    std::vector< TModuleInfo >::iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        TModuleInfo& moduleInfo = (*i);
        GenerateCMakeListsFile( projectInfo, moduleInfo, addCompileDate );

        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Generated CMakeLists.txt file contents for project dir: " + moduleInfo.rootDir );
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

    // Generate the contents of the CMakeLists files
    GenerateCMakeListsContentForModules( projectInfo                     , 
                                         addGeneratorCompileTimeToOutput );

    bool writeLogLocationToOutput = false;//CORE::StringToBool( keyValueList.GetValueAlways( "writeLogLocationToOutput" ) );

    // Now write what we created to disk
    if ( writeLogLocationToOutput )
    {
//        WriteCMakeListsFilesToDisk( projectInfo, logFilename );
    }
    else
    {
        WriteCMakeListsFilesToDisk( projectInfo );
    }
    
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
