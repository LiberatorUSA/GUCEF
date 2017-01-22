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

#include "gucefProjectGen_CCMakeProjectGenerator.h"

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
static TStringMap cmakeAdditionTemplates;

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
        platformMap[ "UNIX" ].insert( "linux32" );
        platformMap[ "UNIX" ].insert( "linux64" );
        platformMap[ "UNIX" ].insert( "unix" );
        platformMap[ "IPHONEOS" ].insert( "iphone" );
        platformMap[ "SYMBIAN" ].insert( "symbian" );
        platformMap[ "OSX" ].insert( "osx" );
        platformMap[ "OSX" ].insert( "mac" );
        platformMap[ "ANDROID" ].insert( "android" );
        platformMap[ "GLX" ].insert( "glx" );
        platformMap[ "GTK" ].insert( "gtk" );
        platformMap[ "SDL" ].insert( "sdl" );
        platformMap[ "EMSCRIPTEN" ].insert( "emscripten" );
        platformMap[ "NACL" ].insert( "nacl" );
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

static CString
ConvertEnvVarStrings( const CString& inStr )
{GUCEF_TRACE;

    return inStr.ReplaceEnvelopingSubstr( "$ENVVAR:", "$", "$ENV{", "}" );
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
        "# This file was automatically generated by ProjectGenerator\n"
        "# which is tooling part the build system designed for GUCEF\n"
        "#     (Galaxy Unlimited Framework)\n"
        "# For the latest info, see http://www.VanvelzenSoftware.com/\n"
        "#\n"
        "# The contents of this file are placed in the public domain. Feel\n"
        "# free to make use of it in any way you like.\n"
        "#-------------------------------------------------------------------\n"
        "#\n";

        fileHeaderWithCompileDate = fileHeader +
        "#\n"
        "# The ProjectGenerator tool used was compiled on: " __DATE__ " @ " __TIME__ "\n"
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
GenerateAutoGenertedTemplatedAdditionSeperator( bool end )
{GUCEF_TRACE;

    CORE::CString seperator = "\n\n\n#********";
    if ( end )
    {
        seperator += " END OF AUTOGENERATED TEMPLATED ADDITION CONTENT ";
    }
    else
    {
        seperator += " START OF AUTOGENERATED TEMPLATED ADDITION CONTENT ";
    }
    seperator += "********\n\n\n";
    return seperator;
}

/*---------------------------------------------------------------------------*/

bool
LoadCMakeFileAdditionTemplates( const CORE::CString& templateDir )
{GUCEF_TRACE;

    CORE::CString pathToTemplates = CORE::RelativePath( templateDir );
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Attempting to load CMake file addition templates from: " + pathToTemplates );

    CORE::SDI_Data* sdiData = CORE::DI_First_Dir_Entry( pathToTemplates.C_String() );
    if ( NULL != sdiData )
    {
        do
        {
            if ( 0 != DI_Is_It_A_File( sdiData ) )
            {
                CORE::CString filename = pathToTemplates;
                CORE::AppendToPath( filename, DI_Name( sdiData ) );

                // Filter 'bad' files based on extention in case someone uses a non-dedicated template dir
                CORE::CString fileExt = CORE::ExtractFileExtention( filename ).Lowercase();
                if ( fileExt == "cmt" )
                {
                    // Use the filename as the template name
                    CORE::CString templateName = DI_Name( sdiData );
                    templateName = templateName.CutChars( fileExt.Length()+1, false ).Lowercase();

                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Found template with name \"" + templateName + "\" in dir " + pathToTemplates );

                    CORE::CString templateText;
                    if ( CORE::LoadTextFileAsString( filename, templateText ) )
                    {
                        cmakeAdditionTemplates[ templateName ] = templateText;
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded template with name \"" + templateName + "\" from dir " + pathToTemplates );
                    }
                    else
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to load template with name \"" + templateName + "\" from dir " + pathToTemplates );
                    }
                }
            }
        }
        while ( 0 != DI_Next_Dir_Entry( sdiData ) );
        DI_Cleanup( sdiData );
    }
    return true;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeTemplatedAdditionSection( const TModuleInfoEntry& moduleInfoEntry ,
                                       const CORE::CString& platformName       )
{GUCEF_TRACE;

    const TModuleInfo* moduleInfo = NULL;
    const CORE::CString* moduleName = GetModuleName( moduleInfoEntry, platformName, &moduleInfo );

    if ( ( NULL != moduleName ) && ( NULL != moduleInfo ) )
    {
        // Use the module type as the index to find a template.
        // We can make this more advanced later if needed but for now differentiating based on module
        // type provides enough flexibility since you can do the rest in CMake utility functions
        CORE::CString moduleTypeSttr = ModuleTypeToString( GetModuleType( moduleInfoEntry, platformName ) ).Lowercase();
        TStringMap::iterator i = cmakeAdditionTemplates.find( moduleTypeSttr );
        if ( i != cmakeAdditionTemplates.end() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processing template with name \"" + moduleTypeSttr + "\" for module with name " + *moduleName + " using platform " + platformName );

            const CORE::CString& templateContent = (*i).second;

            // Now perform the main task of a template,.. variable substitution
            CORE::CString sectionContent = templateContent.ReplaceSubstr( "$#$MODULENAME$#$", "${MODULE_NAME}" );

            // --> Later we could add additional replacements here as needed

            return sectionContent;
        }
    }

    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeTemplatedAdditionSection( const TModuleInfoEntry& moduleInfoEntry )

{GUCEF_TRACE;

    CORE::CString sectionContent;
    bool platformAdded = false;
    bool allPlatformsSectionAdded = false;

    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.begin();
    while ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const CORE::CString& platformName = (*i).first;
        if ( AllPlatforms != platformName )
        {
            if ( !platformAdded )
            {
                CORE::CString templatedContent = GenerateCMakeTemplatedAdditionSection( moduleInfoEntry, platformName );
                if ( !templatedContent.IsNULLOrEmpty() )
                {
                    sectionContent += "\n\nif (" + platformName.Uppercase() + ")\n" + templatedContent;
                    platformAdded = true;
                }
            }
            else
            {
                CORE::CString templatedContent = GenerateCMakeTemplatedAdditionSection( moduleInfoEntry, platformName );
                if ( !templatedContent.IsNULLOrEmpty() )
                {
                    sectionContent += "\nelseif (" + platformName.Uppercase() + ")\n" + templatedContent;
                }
            }
        }
        ++i;
    }

    i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        CORE::CString templatedContent = GenerateCMakeTemplatedAdditionSection( moduleInfoEntry, AllPlatforms );
        if ( !templatedContent.IsNULLOrEmpty() )
        {
            if ( platformAdded )
            {
                sectionContent += "\nelse()\n" + templatedContent;
            }
            else
            {
                sectionContent = templatedContent;
            }
            allPlatformsSectionAdded = true;
        }
    }

    if ( platformAdded )
    {
        sectionContent += "\nendif()\n";
    }
    if ( platformAdded || allPlatformsSectionAdded )
    {
        sectionContent = GenerateAutoGenertedTemplatedAdditionSeperator( false ) +
                         sectionContent                                          +
                         GenerateAutoGenertedTemplatedAdditionSeperator( true );
    }

    return sectionContent;
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
            path = ConvertEnvVarStrings( path );

            CORE::AppendToPath( path, (*n) );
            path = path.ReplaceChar( '\\', '/' );

            // CMake needs spaces in paths to be escaped
            path = path.ReplaceSubstr( " ", "\\ " );

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
        if ( !includeFiles.empty() )
        {
            sectionContent = "set( HEADER_FILES \n";
            sectionContent = GenerateCMakeListsFileSection( sectionContent, includeFiles );

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( includeFiles.size() ) + " include dirs for module " + consensusModuleName );
            return sectionContent;
        }
    }

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "The module " + consensusModuleName + " does not have include directories which apply to all platforms, skipping general include section" );
    return CORE::CString();
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
        if ( !srcFiles.empty() )
        {
            sectionContent = "set( SOURCE_FILES \n";
            sectionContent = GenerateCMakeListsFileSection( sectionContent, srcFiles );

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( srcFiles.size() ) + " include dirs for module " + consensusModuleName );
            return sectionContent;
        }
    }

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "The module " + consensusModuleName + " does not have include directories which apply to all platforms, skipping general include section" );
    return CORE::CString();
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
                    CORE::CString path = CORE::CombinePath( (*n).first, (*i) );
                    path = path.ReplaceChar( '\\', '/' );

                    // CMake needs spaces in paths to be escaped
                    path = path.ReplaceSubstr( " ", "\\ " );

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

            headerSection += "  set( PLATFORM_HEADER_INSTALL \"" + platformName.Uppercase() + "\" )\n";
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
                    CORE::CString path = CORE::CombinePath( (*n).first, (*i) );
                    path = path.ReplaceChar( '\\', '/' );

                    // CMake needs spaces in paths to be escaped
                    path = path.ReplaceSubstr( " ", "\\ " );

                    sourceSection += "    " + path + "\n";
                    ++i;
                }
                ++n;
            }
            sourceSection += "  )\n\n";

            sourceSection += "  set( PLATFORM_SOURCE_INSTALL \"" + platformName.Uppercase() + "\" )\n";
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

        if ( AllPlatforms != platformName && !platformName.IsNULLOrEmpty() )
        {
            GenerateCMakeListsFilePlatformFilesSection( moduleInfoEntry        ,
                                                        platformName           ,
                                                        headerSection          ,
                                                        sourceSection          ,
                                                        hasPlatformHeaderFiles ,
                                                        hasPlatformSourceFiles );

            if ( !headerSection.IsNULLOrEmpty() || !sourceSection.IsNULLOrEmpty() )
            {
                if ( firstPlatform )
                {
                    sectionContent = "\n\nif (" + platformName.Uppercase() + ")\n" + headerSection + sourceSection;
                    firstPlatform = false;
                }
                else
                {
                    sectionContent += "elseif (" + platformName.Uppercase() + ")\n" + headerSection + sourceSection;
                }
            }
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

    if ( moduleInfo.name == "CEGUI.RendererModule.Direct3D9" )
    {
        int b=0;
    }
    
    // Add include dirs for each dependency we know about
    CORE::CString allRelDependencyPaths;
    const TStringSet& includeDirs = moduleInfo.dependencyIncludeDirs;
    TStringSet::const_iterator i = includeDirs.begin();
    while ( i != includeDirs.end() )
    {
        // CMake needs spaces in paths to be escaped
        CORE::CString path = (*i).ReplaceSubstr( " ", "\\ " );

        allRelDependencyPaths += ConvertEnvVarStrings( path ) + " ";
        ++i;
    }

    // Add all the regular include dirs for this module
    TStringVectorMap::const_iterator n = moduleInfo.includeDirs.begin();
    while ( n != moduleInfo.includeDirs.end() )
    {        
        CORE::CString includeDir = ConvertEnvVarStrings( (*n).first ).ReplaceChar( '\\', '/' ); 

        // CMake needs spaces in paths to be escaped
        includeDir = includeDir.ReplaceSubstr( " ", "\\ " );

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
                CORE::CString path = "../" + CORE::LastSubDir( rootDir );
                path = path.ReplaceSubstr( " ", "\\ " );
                allRelDependencyPaths += path + " ";
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
                sectionContent += "\nif ( "+ platformName.Uppercase() + " )\n  ";
                sectionContent += platformSection;
                sectionContent += "endif( " + platformName.Uppercase() + " )\n";
            }
        }
        ++i;
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
LoadCMakeListsAdditionFileFromDisk( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    CORE::CString additionFilePath = moduleInfoEntry.rootDir;
    CORE::AppendToPath( additionFilePath, "CMakeListsAddition.txt" );

    CORE::CString fileContent;
    CORE::LoadTextFileAsString( additionFilePath, fileContent );
    return fileContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleDescriptionLine( const TModuleInfo& moduleInfo     ,
                                    const CORE::CString& moduleName   ,
                                    const CORE::CString& platformName )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating CMake module description line for module " + moduleName + " and platform " + platformName );

    switch ( moduleInfo.moduleType )
    {
        case MODULETYPE_EXECUTABLE:
        {
            if ( platformName == "win32" || platformName == "win64" )
            {
                return "add_executable( ${MODULE_NAME} WIN32 ${ALL_FILES} )\n";
            }
            else
            {
                return "add_executable( ${MODULE_NAME} ${ALL_FILES} )\n";
            }
        }
        case MODULETYPE_SHARED_LIBRARY:
        {
            return "add_library( ${MODULE_NAME} ${ALL_FILES} )\n";
        }
        case MODULETYPE_STATIC_LIBRARY:
        {
            return "add_library( ${MODULE_NAME} STATIC ${ALL_FILES} )\n";
        }
        case MODULETYPE_UNKNOWN:
        case MODULETYPE_UNDEFINED:
        default:
        {
            return CORE::CString();
        }
    }
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleDependenciesLine( const TProjectInfo& projectInfo   ,
                                     const TModuleInfo& moduleInfo     ,
                                     const CORE::CString& moduleName   ,
                                     const CORE::CString& platformName )
{GUCEF_TRACE;

    if ( !moduleInfo.dependencies.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating CMake module dependencies line for module " + moduleName + " and platform " + platformName );

        CORE::CString sectionContent = "add_dependencies( ${MODULE_NAME}";

        TStringSet dependencies;
        TStringVector::const_iterator i = moduleInfo.dependencies.begin();
        while ( i != moduleInfo.dependencies.end() )
        {
            // We add all dependencies except for header include locations which are not real modules
            // and CMake will not be using a make file for those.
            const TModuleInfoEntry* dependencyModule = GetModuleInfoEntry( projectInfo, (*i), AllPlatforms );
            if ( NULL != dependencyModule )
            {
                TModuleType moduleType = GetModuleType( *dependencyModule, AllPlatforms );
                if ( ( MODULETYPE_HEADER_INCLUDE_LOCATION != moduleType )   &&
                     ( MODULETYPE_HEADER_INTEGRATE_LOCATION != moduleType ) &&
                     ( MODULETYPE_CODE_INTEGRATE_LOCATION != moduleType )   &&
                     ( MODULETYPE_BINARY_PACKAGE != moduleType )             )
                {
                    dependencies.insert( (*i) );
                }
            }
            else
            {
                dependencies.insert( (*i) );
            }
            ++i;
        }

        // The reason we first put the dependency strings in a set is to sort them alphabetically.
        // this way the output remains the same regardless of what order the modules were processed in.
        TStringSet::iterator n = dependencies.begin();
        while ( n != dependencies.end() )
        {
            sectionContent += ' ' + ConvertEnvVarStrings( (*n) );
            ++n;
        }
        if ( !dependencies.empty() )
        {
            sectionContent += " )\n";
            return sectionContent;
        }
    }

    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleLinkerLine( const TModuleInfo& moduleInfo     ,
                               const CORE::CString& moduleName   ,
                               const CORE::CString& platformName )
{GUCEF_TRACE;

    if ( !moduleInfo.linkerSettings.linkedLibraries.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating CMake module linker line for module " + moduleName + " and platform " + platformName );

        CORE::CString sectionContent = "target_link_libraries( ${MODULE_NAME}";

        TModuleTypeMap::const_iterator i = moduleInfo.linkerSettings.linkedLibraries.begin();
        while ( i != moduleInfo.linkerSettings.linkedLibraries.end() )
        {
            sectionContent += ' ' + ConvertEnvVarStrings( (*i).first );
            ++i;
        }
        sectionContent += " )\n";
        return sectionContent;
    }

    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleDefinesLine( const TModuleInfo& moduleInfo     ,
                                const CORE::CString& moduleName   ,
                                const CORE::CString& platformName )
{GUCEF_TRACE;

    if ( !moduleInfo.preprocessorSettings.defines.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating CMake module preprocessor defines for module " + moduleName + " and platform " + platformName );

        CORE::CString sectionContent = "set_property( TARGET ${MODULE_NAME} APPEND_STRING PROPERTY COMPILE_DEFINITIONS \"";

        bool first = true;
        TStringSet::const_iterator i = moduleInfo.preprocessorSettings.defines.begin();
        while ( i != moduleInfo.preprocessorSettings.defines.end() )
        {
            if ( first )
            {
                sectionContent += ConvertEnvVarStrings( (*i) );
                first = false;
            }
            else
            {
                sectionContent += ';' + ConvertEnvVarStrings( (*i) );
            }
            ++i;
        }
        sectionContent += "\" )\n";
        return sectionContent;
    }

    return CORE::CString();
}

/*---------------------------------------------------------------------------*/          

CORE::CString
GenerateCMakeListsModuleNameSection( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating CMake section which defines the module name variable ${MODULE_NAME} depending on the platform flags" );

    CORE::CString sectionContent;

    // First get a list of platforms for which a platform specific module name is defined
    TConstModuleInfoPtrMap moduleNameMap;
    GetModuleInfoWithUniqueModuleNames( moduleInfoEntry, moduleNameMap );

    // Now we will use this list to define the module name CMake variable
    bool platformAdded = false;
    TConstModuleInfoPtrMap::iterator i = moduleNameMap.begin();
    while ( i != moduleNameMap.end() )
    {
        const CORE::CString& platformName = (*i).first;
        const CORE::CString& moduleName = (*i).second->name;

        if ( platformName != AllPlatforms )
        {
            if ( platformAdded )
            {
                sectionContent += "elseif( " + platformName.Uppercase() + " )\n  set( MODULE_NAME \"" + moduleName + "\" )\n";
            }
            else
            {
                sectionContent += "\nif( " + platformName.Uppercase() + " )\n  set( MODULE_NAME \"" + moduleName + "\" )\n";
                platformAdded = true;
            }
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "${MODULE_NAME} = " + moduleName + " for platform " + platformName );
        }
        ++i;
    }

    i = moduleNameMap.find( AllPlatforms );
    if ( i != moduleNameMap.end() )
    {
        const CORE::CString& platformName = (*i).first;
        const CORE::CString& moduleName = (*i).second->name;

        if ( platformAdded )
        {
            sectionContent += "else()\n  set( MODULE_NAME \"" + moduleName +"\" )\nendif()\n";
        }
        else
        {
            sectionContent += "set( MODULE_NAME \"" + moduleName +"\" )\n";
        }
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "${MODULE_NAME} = " + moduleName + " for platform " + platformName );
    }
    else
    {
        if ( platformAdded )
        {
            sectionContent += "endif()\n";
        }
    }

    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeModuleDescriptionSection( const TModuleInfoEntry& moduleInfoEntry  ,
                                       const CORE::CString& consensusModuleName )
{GUCEF_TRACE;

    // This bit of information is not additive and is in fact mutually exclusive per platform
    // As such we have to encompass each different definition in an if-else
    CORE::CString sectionContent;
    bool platformAdded = false;
    TConstModuleInfoPtrMap moduleTypeMap;
    GetModuleInfoWithUniqueModulesTypes( moduleInfoEntry, moduleTypeMap );
    TConstModuleInfoPtrMap::iterator n = moduleTypeMap.begin();
    while ( n != moduleTypeMap.end() )
    {
        const CORE::CString& platformName = (*n).first;
        const TModuleInfo* moduleInfo = (*n).second;

        if ( platformName != AllPlatforms )
        {
            if ( platformAdded )
            {
                sectionContent += "elseif( " + platformName.Uppercase() + " )\n  " + GenerateCMakeModuleDescriptionLine( *moduleInfo, consensusModuleName, platformName );
            }
            else
            {
                sectionContent += "if( " + platformName.Uppercase() + " )\n  " + GenerateCMakeModuleDescriptionLine( *moduleInfo, consensusModuleName, platformName );
                platformAdded = true;
            }
        }
        ++n;
    }

    n = moduleTypeMap.find( AllPlatforms );
    if ( n != moduleTypeMap.end() )
    {
        const TModuleInfo* moduleInfo = (*n).second;

        if ( platformAdded )
        {
            // This module has platform module descriptions which override the AllPlatforms version which we will define here
            sectionContent += "else()\n  " + GenerateCMakeModuleDescriptionLine( *moduleInfo, consensusModuleName, AllPlatforms ) + "endif()\n";
        }
        else
        {
            // This module only has a AllPlatforms description and no deviating description for any platform
            // AllPlatforms means we also support MS windows. On that platform we still want to use the WinMain
            // entry point.
            if ( moduleInfo->moduleType == MODULETYPE_EXECUTABLE )
            {
                sectionContent += "if ( WIN32 )\n  " +
                                    GenerateCMakeModuleDescriptionLine( *moduleInfo, consensusModuleName, "win32" ) +
                                  "else()\n  " +
                                    GenerateCMakeModuleDescriptionLine( *moduleInfo, consensusModuleName, AllPlatforms ) +
                                  "endif()\n";
            }
            else
            {
                sectionContent += GenerateCMakeModuleDescriptionLine( *moduleInfo, consensusModuleName, AllPlatforms );
            }
        }
    }
    else
    {
        if ( platformAdded )
        {
            // This module only has platform specific module descriptions, no AllPlatforms version exists
            sectionContent += "endif()\n";
        }
    }

    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsModuleInfoSection( const TProjectInfo& projectInfo         ,
                                     const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    CORE::CString consensusName = GetConsensusModuleName( moduleInfoEntry );

    // First we define the module name which can differ per platform
    CORE::CString sectionContent = GenerateCMakeListsModuleNameSection( moduleInfoEntry );

    // Add the module description which says what type of module this is
    sectionContent += GenerateCMakeModuleDescriptionSection( moduleInfoEntry, consensusName );

    // Add module info which is addative meaning AllPlatforms info can be
    // supplemented with it and it is not mutually exclusive
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const CORE::CString& platformName = (*i).first;
        const TModuleInfo& moduleInfo = (*i).second;

        // Generate the different instructions for all platforms (if any exist)
        CORE::CString moduleDependenciesStr = GenerateCMakeModuleDependenciesLine( projectInfo, moduleInfo, consensusName, platformName );
        CORE::CString moduleLinkingStr = GenerateCMakeModuleLinkerLine( moduleInfo, consensusName, platformName );
        CORE::CString moduleDefinesStr = GenerateCMakeModuleDefinesLine( moduleInfo, consensusName, platformName );

        // make sure we actually have any instructions
        if ( !moduleDependenciesStr.IsNULLOrEmpty() ||
             !moduleLinkingStr.IsNULLOrEmpty()      ||
             !moduleDefinesStr.IsNULLOrEmpty()       )
        {
            sectionContent += ( moduleDependenciesStr + moduleLinkingStr + moduleDefinesStr );
        }
    }

    // Now do the same for all adative platform specific info
    i = moduleInfoEntry.modulesPerPlatform.begin();
    while ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const CORE::CString& platformName = (*i).first;

        if ( platformName != AllPlatforms )
        {
            const TModuleInfo& moduleInfo = (*i).second;

            // Generate the different instructions for this platform (if any exist)
            CORE::CString moduleDependenciesStr = GenerateCMakeModuleDependenciesLine( projectInfo, moduleInfo, consensusName, platformName );
            CORE::CString moduleLinkingStr = GenerateCMakeModuleLinkerLine( moduleInfo, consensusName, platformName );
            CORE::CString moduleDefinesStr = GenerateCMakeModuleDefinesLine( moduleInfo, consensusName, platformName );

            // Encompass inside an if section if we have any instructions
            if ( !moduleDependenciesStr.IsNULLOrEmpty() ||
                 !moduleLinkingStr.IsNULLOrEmpty()      ||
                 !moduleDefinesStr.IsNULLOrEmpty()       )
            {
                CORE::CString platformSection;

                platformSection = "\nif ( "+ platformName.Uppercase() + " )\n";

                if ( !moduleDependenciesStr.IsNULLOrEmpty() )
                {
                    platformSection += "  " + moduleDependenciesStr;
                }
                if ( !moduleLinkingStr.IsNULLOrEmpty() )
                {
                    platformSection += "  " + moduleLinkingStr;
                }
                if ( !moduleDefinesStr.IsNULLOrEmpty() )
                {
                    platformSection += "  " + moduleDefinesStr;
                }
                platformSection += "endif( " + platformName.Uppercase() + " )\n";

                // Now that we finished generating the section for this platform add it to the entire info content section
                sectionContent += platformSection;
            }
        }
        ++i;
    }

    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GenerateCMakeListsFileContent( const TProjectInfo& projectInfo         ,
                               const TModuleInfoEntry& moduleInfoEntry ,
                               bool treatTagsAsOptions                 ,
                               bool addCompileDate = false             )
{GUCEF_TRACE;

    // Set file header comment section
    CORE::CString fileContent = GetCMakeListsFileHeader( addCompileDate );

    // Determine the general consensus module name
    CORE::CString consensusModuleName = GetConsensusModuleName( moduleInfoEntry );

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating CMakeLists content for module " + consensusModuleName );

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

        // Add all the include directories for this module
        fileContent += GenerateCMakeModuleIncludesSection( moduleInfoEntry );
    }
    else
    {
        // Since we are not using a legacy suffix file we have to auto generate more info then before
        // mainly the module description, dependencies, definitions, etc.
        fileContent += GenerateCMakeListsModuleInfoSection( projectInfo, moduleInfoEntry );

        CORE::CString additionFileContent = LoadCMakeListsAdditionFileFromDisk( moduleInfoEntry );
        if ( !additionFileContent.IsNULLOrEmpty() )
        {
            fileContent += GenerateAutoGenertedSeperator( true );
            fileContent += additionFileContent;
            fileContent += GenerateAutoGenertedSeperator( false );
        }

        // Add all the include directories for this module
        fileContent += GenerateCMakeModuleIncludesSection( moduleInfoEntry );

        // Add the templated addition section which is halfway between a completely manually defined
        // addition and completely autogenerated content
        fileContent += GenerateCMakeTemplatedAdditionSection( moduleInfoEntry );
    }

    // We are done generating the content for the CMake file
    return fileContent;
}

/*---------------------------------------------------------------------------*/

void
WriteCMakeListsFilesToDisk( const TProjectInfo& projectInfo  ,
                            const CORE::CString& logFilename ,
                            bool treatTagsAsOptions          ,
                            bool addCompileDate = false      )
{GUCEF_TRACE;

    // Write all the CMakeLists.txt files
    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        const TModuleInfoEntry& moduleInfoEntry = (*i);
        TModuleType allPlatformsType = GetModuleType( moduleInfoEntry, AllPlatforms );
        if ( ( MODULETYPE_HEADER_INCLUDE_LOCATION != allPlatformsType )   &&
             ( MODULETYPE_HEADER_INTEGRATE_LOCATION != allPlatformsType ) &&
             ( MODULETYPE_CODE_INTEGRATE_LOCATION != allPlatformsType )   &&
             ( MODULETYPE_BINARY_PACKAGE != allPlatformsType )             )
        {
            CORE::CString fileContent = GenerateCMakeListsFileContent( projectInfo, moduleInfoEntry, treatTagsAsOptions, addCompileDate );
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
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Skipping CMakeLists.txt generation for module of type \"HeaderIncludeLocation\"" );
        }
        ++i;
    }
}
/*--------------------------------------------------------------------------*/

void
WriteCMakeOptionsListToDisk( const TProjectInfo& projectInfo  ,
                             const CORE::CString& outputDir   ,
                             bool addCompileDate              ,
                             bool treatTagsAsOptions          ,
                             bool taggedOptionsEnabledDefault )
{GUCEF_TRACE;

    // Creates a list with all the options
    // They can be explicitly defined but then you have no overview and the intial value would always be OFF
    // option(<option_variable> "help string describing option" [initial value])

    bool hasOptions = false;
    CORE::CString fileContent;
    if ( treatTagsAsOptions )
    {
        TStringSet tagsUsed;
        GetAllTagsUsed( projectInfo, tagsUsed );
        if ( !tagsUsed.empty() )
        {
            hasOptions = true;
            CORE::CString optionValue = taggedOptionsEnabledDefault ? "ON" : "OFF";

            // Define default CMake option states for all tags
            TStringSet::iterator i = tagsUsed.begin();
            while ( i != tagsUsed.end() )
            {
                fileContent += "option( TAGGED_BUILD_OPTION_" + (*i) + " \"build option based on tag " + (*i) + "\" " + optionValue + ")\n";
                ++i;
            }
            fileContent += "\n\n";

            // Make the build print out the available options to help the devs reviewing build logs
            // The settings above are just defaults, this will print out the actual state when CMake runs
            i = tagsUsed.begin();
            while ( i != tagsUsed.end() )
            {
                fileContent += "message( STATUS \"TAGGED_BUILD_OPTION_" + (*i) + ": ${TAGGED_BUILD_OPTION_" + (*i) + "} )\n";
                ++i;
            }
        }
    }

    // @TODO: Alternate option management? scripts?

    if ( hasOptions )
    {        
        fileContent = GetCMakeListsFileHeader( addCompileDate ) + "\n\n" + fileContent;

        CORE::CString pathToOptionsListFile = CORE::CombinePath( outputDir, "OptionsList.cmake" );
        if ( CORE::WriteStringAsTextFile( pathToOptionsListFile, fileContent ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Created OptionsList.cmake file in output dir: " + outputDir );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to write OptionsList.cmake file content to disk at path " + outputDir );
        }
    }
}

/*--------------------------------------------------------------------------*/

void
WriteCMakeModulesListToDisk( const TProjectInfo& projectInfo ,
                             const CORE::CString& outputDir  ,
                             bool addCompileDate             )
{GUCEF_TRACE;

    CORE::CString fileContent = GetCMakeListsFileHeader( addCompileDate );
    
    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        const TModuleInfoEntry& moduleInfoEntry = (*i);

        TModuleType allPlatformsType = GetModuleType( moduleInfoEntry, AllPlatforms );
        if ( ( MODULETYPE_HEADER_INCLUDE_LOCATION != allPlatformsType )   &&
             ( MODULETYPE_HEADER_INTEGRATE_LOCATION != allPlatformsType ) &&
             ( MODULETYPE_CODE_INTEGRATE_LOCATION != allPlatformsType )   &&
             ( MODULETYPE_BINARY_PACKAGE != allPlatformsType )             )
        {
            CORE::CString pathToModuleDir = CORE::GetRelativePathToOtherPathRoot( outputDir, moduleInfoEntry.rootDir );
            pathToModuleDir = pathToModuleDir.ReplaceChar( '\\', '/' );
            CORE::CString cmakeLine = "add_subdirectory( " + pathToModuleDir + " )\n";
            fileContent += cmakeLine;
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Skipping CMakeLists.txt generation for module of type \"HeaderIncludeLocation\"" );
        }
        ++i;
    }
   
    CORE::CString pathToCMakeModuleDirsFile = CORE::CombinePath( outputDir, "ModuleDirs.cmake" );
    if ( CORE::WriteStringAsTextFile( pathToCMakeModuleDirsFile, fileContent ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Created ModuleDirs.cmake file in output dir: " + outputDir );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to write ModuleDirs.cmake file content to disk at path " + outputDir );
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
                                         bool addGeneratorCompileTimeToOutput ,
                                         const CORE::CValueList& params       )
{GUCEF_TRACE;

    // Prepare by loading up templates if any where provided
    CORE::CString templateDir = params.GetValueAlways( "cmakgen:TemplateDir" );
    if ( !templateDir.IsNULLOrEmpty() )
    {
        LoadCMakeFileAdditionTemplates( templateDir );
    }

    // Write the gathered info to disk in CMakeList.txt format
    CORE::CString logfilePath;
    bool addLogfilePathToOutput = CORE::StringToBool( params.GetValueAlways( "writeLogLocationToOutput" ) );
    if ( addLogfilePathToOutput )
    {
        logfilePath = params.GetValueAlways( "logfile" );
    }

    bool treatTagsAsOptions = CORE::StringToBool( params.GetValueAlways( "TreatTagsAsOptions", "true" ) );
    bool taggedOptionsEnabledDefault = CORE::StringToBool( params.GetValueAlways( "TaggedOptionsEnabledDefault", "true" ) );

    WriteCMakeListsFilesToDisk( projectInfo                     , 
                                logfilePath                     , 
                                treatTagsAsOptions              , 
                                addGeneratorCompileTimeToOutput );

    WriteCMakeModulesListToDisk( projectInfo, outputDir, addGeneratorCompileTimeToOutput );

    WriteCMakeOptionsListToDisk( projectInfo                     , 
                                 outputDir                       , 
                                 addGeneratorCompileTimeToOutput , 
                                 treatTagsAsOptions              , 
                                 taggedOptionsEnabledDefault     );

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
