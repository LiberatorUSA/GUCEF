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

#include "gucefProjectGenerator_CPremake4ProjectGenerator.h"

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

static const CORE::CString AllPlatforms = "all";
static TStringMap premake4AdditionTemplates;

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
GetPremake4FileHeader( bool addCompileDate = false )
{GUCEF_TRACE;

    static bool initialized = false;
    static CORE::CString fileHeader;
    static CORE::CString fileHeaderWithCompileDate;

    if ( !initialized )
    {
        fileHeader =
        "--------------------------------------------------------------------\n"
        "-- This file was automatically generated by ProjectGenerator\n"
        "-- which is tooling part the build system designed for GUCEF\n"
        "--     (Galaxy Unlimited Framework)\n"
        "-- For the latest info, see http://www.VanvelzenSoftware.com/\n"
        "--\n"
        "-- The contents of this file are placed in the public domain. Feel\n"
        "-- free to make use of it in any way you like.\n"
        "--------------------------------------------------------------------\n"
        "--\n";

        fileHeaderWithCompileDate = fileHeader +
        "--\n"
        "-- The ProjectGenerator tool used was compiled on: " __DATE__ " @ " __TIME__ "\n"
        "--\n"
        "--\n"
        "--\n";

        initialized = true;
    }

    if ( addCompileDate )
    {
        return fileHeaderWithCompileDate;
    }
    return fileHeader;
}

/*---------------------------------------------------------------------------*/
/*
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
LoadPremake4FileAdditionTemplates( const CORE::CString& templateDir )
{GUCEF_TRACE;

    CORE::CString pathToTemplates = CORE::RelativePath( templateDir );
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Attempting to load Premake4 file addition templates from: " + pathToTemplates );

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
                if ( fileExt == "pm4t" )
                {
                    // Use the filename as the template name
                    CORE::CString templateName = DI_Name( sdiData );
                    templateName = templateName.CutChars( fileExt.Length()+1, false ).Lowercase();

                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Found template with name \"" + templateName + "\" in dir " + pathToTemplates );

                    CORE::CString templateText;
                    if ( CORE::LoadTextFileAsString( filename, templateText ) )
                    {
                        premake4AdditionTemplates[ templateName ] = templateText;
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
GeneratePremake4TemplatedAdditionSection( const TModuleInfoEntry& moduleInfoEntry ,
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
        TStringMap::iterator i = premake4AdditionTemplates.find( moduleTypeSttr );
        if ( i != premake4AdditionTemplates.end() )
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
GeneratePremake4TemplatedAdditionSection( const TModuleInfoEntry& moduleInfoEntry )

{GUCEF_TRACE;

    CORE::CString sectionContent;     /*
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
               */
    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4FileSection( const CORE::CString& sectionContent ,
                             const TStringVectorMap& fileMap     )
{GUCEF_TRACE;

    CORE::CString newSectionContent = sectionContent;

    newSectionContent = "files( {\n";
    bool first = true;
    TStringVectorMap::const_iterator i = fileMap.begin();
    while ( i != fileMap.end() )
    {
        TStringVector::const_iterator n = (*i).second.begin();
        while ( n != (*i).second.end() )
        {
            CORE::CString path = (*i).first;
            CORE::AppendToPath( path, (*n) );
            path = path.ReplaceChar( '\\', '/' );

            if ( first )
            {
                newSectionContent += "  \"" + path + '\"';
                first = false;
            }
            else
            {
                newSectionContent += ",\n  \"" + path + '\"';
            }
            ++n;
        }
        ++i;
    }

    newSectionContent += "\n } )\n\n";
    return newSectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4FileIncludeSection( const TModuleInfoEntry& moduleInfoEntry  ,
                                    const CORE::CString& consensusModuleName )
{GUCEF_TRACE;

    CORE::CString sectionContent;
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const TStringVectorMap& includeFiles = (*i).second.includeDirs;
        if ( !includeFiles.empty() )
        {
            sectionContent = "\n\nconfiguration( {} )\nvpaths { [\"Headers\"] = { \"**.h\", \"**.hpp\", \"**.hxx\" } }\n";
            sectionContent += GeneratePremake4FileSection( sectionContent, includeFiles );

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( includeFiles.size() ) + " include file dirs for module " + consensusModuleName );
            return sectionContent;
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "The module " + consensusModuleName + " does not have include file directories which apply to all platforms, skipping general include section" );
        }
    }

    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4FileSrcSection( const TModuleInfoEntry& moduleInfoEntry  ,
                                const CORE::CString& consensusModuleName )
{GUCEF_TRACE;

    CORE::CString sectionContent;
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const TStringVectorMap& srcFiles = (*i).second.sourceDirs;
        if ( !srcFiles.empty() )
        {
            sectionContent = "\n\nconfiguration( {} )\nvpaths { [\"Source\"] = { \"**.c\", \"**.cpp\", \"**.cs\", \"**.asm\" } }\n";
            sectionContent += GeneratePremake4FileSection( sectionContent, srcFiles );

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Processed " + CORE::UInt32ToString( srcFiles.size() ) + " source dirs for module " + consensusModuleName );
            return sectionContent;
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "The module " + consensusModuleName + " does not have source directories which apply to all platforms, skipping general source section" );
        }
    }
    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

void
GeneratePremake4FilePlatformFilesSection( const TModuleInfoEntry& moduleInfoEntry ,
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
            headerSection = "    vpaths { [\"Platform Headers\"] = { \"**.h\", \"**.hpp\", \"**.hxx\" } }\n    files( {\n";

            TStringVectorMap::const_iterator n = platformHeaderFiles.begin();
            while ( n != platformHeaderFiles.end() )
            {
                bool first = true;
                const TStringVector& platformHeaderFilesDir = (*n).second;
                TStringVector::const_iterator i = platformHeaderFilesDir.begin();
                while ( i != platformHeaderFilesDir.end() )
                {
                    CORE::CString path = (*n).first;
                    CORE::AppendToPath( path, (*i) );
                    path = path.ReplaceChar( '/', '\\' );

                    if ( first )
                    {
                        headerSection += "      \"" + path + '\"';
                        first = false;
                    }
                    else
                    {
                        headerSection += ",\n      \"" + path + '\"';
                    }
                    ++i;
                }
                ++n;
            }
            headerSection += "\n    } )\n\n";

        }

        const TStringVectorMap& platformSourceFiles = (*m).second.sourceDirs;
        if ( !platformSourceFiles.empty() )
        {
            hasPlatformSourceFiles = true;
            sourceSection = "    vpaths { [\"Platform Source\"] = { \"**.c\", \"**.cpp\", \"**.cs\", \"**.asm\" } }\n    files( {\n";

            TStringVectorMap::const_iterator n = platformSourceFiles.begin();
            while ( n != platformSourceFiles.end() )
            {
                bool first = true;
                const TStringVector& platformSourceFilesDir = (*n).second;
                TStringVector::const_iterator i = platformSourceFilesDir.begin();
                while ( i != platformSourceFilesDir.end() )
                {
                    CORE::CString path = (*n).first;
                    CORE::AppendToPath( path, (*i) );
                    path = path.ReplaceChar( '/', '\\' );

                    if ( first )
                    {
                        sourceSection += "      \"" + path + '\"';
                        first = false;
                    }
                    else
                    {
                        sourceSection += ",\n      \"" + path + '\"';
                    }
                    ++i;
                }
                ++n;
            }
            sourceSection += "\n    } )\n\n";
        }
    }
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4FilePlatformFilesSection( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    bool hasPlatformHeaderFiles = false;
    bool hasPlatformSourceFiles = false;

    CORE::CString sectionContent;
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.begin();
    while ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        CORE::CString headerSection;
        CORE::CString sourceSection;
        const CORE::CString& platformName = (*i).first;

        if ( AllPlatforms != platformName && !platformName.IsNULLOrEmpty() )
        {
            GeneratePremake4FilePlatformFilesSection( moduleInfoEntry        ,
                                                      platformName           ,
                                                      headerSection          ,
                                                      sourceSection          ,
                                                      hasPlatformHeaderFiles ,
                                                      hasPlatformSourceFiles );

            if ( !headerSection.IsNULLOrEmpty() || !sourceSection.IsNULLOrEmpty() )
            {
                sectionContent += "\n\nconfiguration( { \"" + platformName.Uppercase() + "\" } )\n" + headerSection + sourceSection;
            }
        }
        ++i;
    }

    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4ModuleIncludesSection( const TModuleInfo& moduleInfo ,
                                       const CORE::CString& rootDir  )
{GUCEF_TRACE;

    // Add include dirs for each dependency we know about
    bool first = true;
    CORE::CString allRelDependencyPaths;
    const TStringSet& includeDirs = moduleInfo.dependencyIncludeDirs;
    TStringSet::const_iterator i = includeDirs.begin();
    while ( i != includeDirs.end() )
    {
        if ( first )
        {
            allRelDependencyPaths += " \"" + (*i) + '\"';
            first = false;
        }
        else
        {
            allRelDependencyPaths += ", \"" + (*i) + '\"';
        }
        ++i;
    }

    // Add all the regular include dirs for this module
    TStringVectorMap::const_iterator n = moduleInfo.includeDirs.begin();
    while ( n != moduleInfo.includeDirs.end() )
    {
        CORE::CString includeDir = (*n).first.ReplaceChar( '/', '\\' );
        if ( 0 != includeDir.Length() )
        {
            if ( first )
            {
                allRelDependencyPaths += " \"" + includeDir + '\"';
                first = false;
            }
            else
            {
                allRelDependencyPaths += ", \"" + includeDir + '\"';
            }
        }
        else
        {
            // Check if there is more then one include dir
            // If so we have create an include for an empty include dir
            // to ensure files in subdirs can include the file with the zero length
            // subdir.
            if ( 1 < moduleInfo.includeDirs.size() )
            {
                allRelDependencyPaths += ", \"..\\\"" + CORE::LastSubDir( rootDir ) + '\"';
            }
        }
        ++n;
    }

    CORE::CString sectionContent;
    if ( allRelDependencyPaths.Length() > 0 )
    {
        sectionContent = "includedirs( {" + allRelDependencyPaths + " } )\n";
    }
    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4ModuleIncludesSection( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    CORE::CString sectionContent;

    // First add the include section which applies to all platforms
    // it should not have an 'if' check around it
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        sectionContent += "\nconfiguration( {} )\n";
        sectionContent += GeneratePremake4ModuleIncludesSection( (*i).second, moduleInfoEntry.rootDir );
    }

    // Now add the include paths which are platform specific
    i = moduleInfoEntry.modulesPerPlatform.begin();
    while ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const CORE::CString& platformName = (*i).first;
        if ( platformName != AllPlatforms )
        {
            CORE::CString platformSection = GeneratePremake4ModuleIncludesSection( (*i).second, moduleInfoEntry.rootDir );
            if ( platformSection.Length() > 0 )
            {
                sectionContent += "\nconfiguration( { \"" + platformName.Uppercase() + "\" } )\n" + platformSection;
            }
        }
        ++i;
    }

    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4AutoGenertedSeperator( bool end )
{GUCEF_TRACE;

    CORE::CString seperator = "\n\n\n--********";
    if ( end )
    {
        seperator += " END OF AUTOGENERATED CONTENT ";
    }
    else
    {
        seperator += " START OF AUTOGENERATED CONTENT ";
    }
    seperator += "**********\n\n\n";
    return seperator;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4ModuleDescriptionLine( const TModuleInfo& moduleInfo     ,
                                       const CORE::CString& moduleName   ,
                                       const CORE::CString& platformName )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating Premake4 module description line for module " + moduleName + " and platform " + platformName );

    switch ( moduleInfo.moduleType )
    {
        case MODULETYPE_EXECUTABLE:
        {
            if ( platformName == "win32" || platformName == "win64" )
            {
                return "kind( \"WindowedApp\" )\n";
            }
            else
            {
                return "kind( \"ConsoleApp\" )\n";
            }
        }
        case MODULETYPE_SHARED_LIBRARY:
        {
            return "kind( \"SharedLib\" )\n";
        }
        case MODULETYPE_STATIC_LIBRARY:
        {
            return "kind( \"StaticLib\" )\n";
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
GeneratePremake4ModuleDependenciesLine( const TProjectInfo& projectInfo   ,
                                        const TModuleInfo& moduleInfo     ,
                                        const CORE::CString& moduleName   ,
                                        const CORE::CString& platformName )
{GUCEF_TRACE;

    if ( !moduleInfo.dependencies.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating Premake4 module dependencies line for module " + moduleName + " and platform " + platformName );

        TStringSet dependencies;
        TStringVector::const_iterator i = moduleInfo.dependencies.begin();
        while ( i != moduleInfo.dependencies.end() )
        {
            // We add all dependencies except for header include locations which are not real modules
            // and Premake4 will not be using a make file for those.
            const TModuleInfoEntry* dependencyModule = GetModuleInfoEntry( projectInfo, (*i), AllPlatforms );
            if ( NULL != dependencyModule )
            {
                TModuleType moduleType = GetModuleType( *dependencyModule, AllPlatforms );
                if ( ( MODULETYPE_HEADER_INCLUDE_LOCATION != moduleType ) &&
                     ( MODULETYPE_CODE_INCLUDE_LOCATION != moduleType )    )
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

        if ( !dependencies.empty() )
        {
            bool first = true;
            CORE::CString sectionContent = "links( {";

            // The reason we first put the dependency strings in a set is to sort them alphabetically.
            // this way the output remains the same regardless of what order the modules were processed in.
            TStringSet::iterator n = dependencies.begin();
            while ( n != dependencies.end() )
            {
                if ( first )
                {
                    sectionContent += " \"" + (*n) + '\"';
                    first = false;
                }
                else
                {
                    sectionContent += ", \"" + (*n) + '\"';
                }
                ++n;
            }
            sectionContent += " } )\n";
            return sectionContent;
        }
    }

    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4ModuleLinkerLine( const TModuleInfo& moduleInfo     ,
                                  const CORE::CString& moduleName   ,
                                  const CORE::CString& platformName )
{GUCEF_TRACE;

    if ( !moduleInfo.linkerSettings.linkedLibraries.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating Premake4 module linker line for module " + moduleName + " and platform " + platformName );

        bool first = true;
        CORE::CString sectionContent = "links( {";
        TModuleTypeMap::const_iterator i = moduleInfo.linkerSettings.linkedLibraries.begin();
        while ( i != moduleInfo.linkerSettings.linkedLibraries.end() )
        {
            if ( first )
            {
                sectionContent += " \"" + (*i).first + '\"';
                first = false;
            }
            else
            {
                sectionContent += ", \"" + (*i).first + '\"';
            }
            ++i;
        }
        sectionContent += " } )\n";
        return sectionContent;
    }

    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4ModuleDefinesLine( const TModuleInfo& moduleInfo     ,
                                   const CORE::CString& moduleName   ,
                                   const CORE::CString& platformName )
{GUCEF_TRACE;

    if ( !moduleInfo.preprocessorSettings.defines.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating Premake4 module preprocessor defines for module " + moduleName + " and platform " + platformName );

        CORE::CString sectionContent = "defines( { ";

        bool first = true;
        TStringSet::const_iterator i = moduleInfo.preprocessorSettings.defines.begin();
        while ( i != moduleInfo.preprocessorSettings.defines.end() )
        {
            if ( first )
            {
                sectionContent += "\"" + (*i) + "\"";
                first = false;
            }
            else
            {
                sectionContent += ", \"" + (*i) + "\"";
            }
            ++i;
        }
        sectionContent += " } )\n";
        return sectionContent;
    }

    return CORE::CString();
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4ModuleNameSection( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating Premake4 section which defines the module name depending on the platform flags" );

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
            sectionContent += "\nconfiguration( { \"" +  platformName.Uppercase() + "\" } )\n  project( \"" + moduleName + "\" )\n";
            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "module name = " + moduleName + " for platform " + platformName );
            platformAdded = true;
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
            //sectionContent += "\nconfiguration { \"" +  platformName.Uppercase() + "\" }\n  project { \"" + moduleName + "\" }\n";
            sectionContent += "else()\n  project( \"" + moduleName + "\" )\nendif()\n";
        }
        else
        {
            sectionContent += "project( \"" + moduleName + "\" )\n";
        }

        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "module name = " + moduleName + " for platform " + platformName );
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
GeneratePremake4ModuleDescriptionSection( const TModuleInfoEntry& moduleInfoEntry  ,
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
            sectionContent += "configuration( { \"" + platformName.Uppercase() + "\" } )\n" + GeneratePremake4ModuleDescriptionLine( *moduleInfo, consensusModuleName, platformName );
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
            sectionContent += "configuration( {} )\n  " + GeneratePremake4ModuleDescriptionLine( *moduleInfo, consensusModuleName, AllPlatforms ) + "\n";
        }
        else
        {
            // This module only has a AllPlatforms description and no deviating description for any platform
            // AllPlatforms means we also support MS windows. On that platform we still want to use the WinMain
            // entry point.
            if ( moduleInfo->moduleType == MODULETYPE_EXECUTABLE )
            {
                sectionContent += "configuration( { \"WIN32\" } )\n" +
                                    GeneratePremake4ModuleDescriptionLine( *moduleInfo, consensusModuleName, "win32" ) +
                                  "configuration( { \"NOT WIN32\" } )\n  " +
                                    GeneratePremake4ModuleDescriptionLine( *moduleInfo, consensusModuleName, AllPlatforms );
            }
            else
            {
                sectionContent += GeneratePremake4ModuleDescriptionLine( *moduleInfo, consensusModuleName, AllPlatforms );
            }
        }
    }

    return sectionContent;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4ModuleInfoSection( const TProjectInfo& projectInfo         ,
                                   const TModuleInfoEntry& moduleInfoEntry ,
                                   const CORE::CString& premakeOutputDir   )
{GUCEF_TRACE;

    CORE::CString consensusName = GetConsensusModuleName( moduleInfoEntry );

    // First we define the module name which can differ per platform
    CORE::CString sectionContent = GeneratePremake4ModuleNameSection( moduleInfoEntry );

    // Set the output path for the premake4 generated files
    if ( !premakeOutputDir.IsNULLOrEmpty() )
    {
        // Check to see if an environment variable is desired
        if ( 0 == premakeOutputDir.HasSubstr( "ENVVAR:", true ) )
        {
            // The path specified is actually a directive to use the given environment variable
            CORE::CString premakeOutputDirEnvVar = premakeOutputDir.CutChars( 7, true );
            sectionContent += "location( os.getenv( \"" + premakeOutputDirEnvVar + "\" ) )\n";
        }
        else
        {
            CORE::CString pathToOutputDir = CORE::GetRelativePathToOtherPathRoot( moduleInfoEntry.rootDir, premakeOutputDir );
            sectionContent += "location( \"" + pathToOutputDir + "\" )\n";
        }
    }

    // Add the module description which says what type of module this is
    sectionContent += GeneratePremake4ModuleDescriptionSection( moduleInfoEntry, consensusName );

    // Add module info which is addative meaning AllPlatforms info can be
    // supplemented with it and it is not mutually exclusive
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const CORE::CString& platformName = (*i).first;
        const TModuleInfo& moduleInfo = (*i).second;

        // Generate the different instructions for all platforms (if any exist)
        CORE::CString moduleDependenciesStr = GeneratePremake4ModuleDependenciesLine( projectInfo, moduleInfo, consensusName, platformName );
        CORE::CString moduleLinkingStr = GeneratePremake4ModuleLinkerLine( moduleInfo, consensusName, platformName );
        CORE::CString moduleDefinesStr = GeneratePremake4ModuleDefinesLine( moduleInfo, consensusName, platformName );

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
            CORE::CString moduleDependenciesStr = GeneratePremake4ModuleDependenciesLine( projectInfo, moduleInfo, consensusName, platformName );
            CORE::CString moduleLinkingStr = GeneratePremake4ModuleLinkerLine( moduleInfo, consensusName, platformName );
            CORE::CString moduleDefinesStr = GeneratePremake4ModuleDefinesLine( moduleInfo, consensusName, platformName );

            // Encompass inside an if section if we have any instructions
            if ( !moduleDependenciesStr.IsNULLOrEmpty() ||
                 !moduleLinkingStr.IsNULLOrEmpty()      ||
                 !moduleDefinesStr.IsNULLOrEmpty()       )
            {
                CORE::CString platformSection;

                platformSection = "\nconfiguration( { \"" + platformName.Uppercase() + "\" } )\n";

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
GeneratePremake4ModuleFileContent( const TProjectInfo& projectInfo         ,
                                   const CORE::CString& premakeOutputDir   ,
                                   const TModuleInfoEntry& moduleInfoEntry ,
                                   bool addCompileDate = false             )
{GUCEF_TRACE;

    // Set file header comment section
    CORE::CString fileContent = GetPremake4FileHeader( addCompileDate );

    // Determine the general consensus module name
    CORE::CString consensusModuleName = GetConsensusModuleName( moduleInfoEntry );

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Generating Premake4 content for module " + consensusModuleName );

    // Set project name comment section
    fileContent += "\n-- Configuration for module: " + consensusModuleName + "\n\n";

    // the module description, dependencies, definitions, etc.
    fileContent += GeneratePremake4ModuleInfoSection( projectInfo, moduleInfoEntry, premakeOutputDir );

    // Add all the general include files
    fileContent += GeneratePremake4FileIncludeSection( moduleInfoEntry, consensusModuleName );

    // Add all the source files
    fileContent += GeneratePremake4FileSrcSection( moduleInfoEntry, consensusModuleName );

    // Add all platform files, headers and source
    fileContent += GeneratePremake4FilePlatformFilesSection( moduleInfoEntry );

    CORE::CString additionFileContent ;//= LoadPremake4AdditionFileFromDisk( moduleInfoEntry );
    if ( !additionFileContent.IsNULLOrEmpty() )
    {
        fileContent += GeneratePremake4AutoGenertedSeperator( true );
        fileContent += additionFileContent;
        fileContent += GeneratePremake4AutoGenertedSeperator( false );
    }

    // Add all the include directories for this module
    fileContent += GeneratePremake4ModuleIncludesSection( moduleInfoEntry );

    // Add the templated addition section which is halfway between a completely manually defined
    // addition and completely autogenerated content
    fileContent += GeneratePremake4TemplatedAdditionSection( moduleInfoEntry );

    // We are done generating the content for the CMake file
    return fileContent;
}

/*---------------------------------------------------------------------------*/

void
WritePremake4ModuleFilesToDisk( const TProjectInfo& projectInfo       ,
                                const CORE::CString& premakeOutputDir ,
                                const CORE::CString& logFilename      ,
                                bool addCompileDate = false           )
{GUCEF_TRACE;

    // Write all the premake4 files
    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        const TModuleInfoEntry& moduleInfoEntry = (*i);
        TModuleType allPlatformsType = GetModuleType( moduleInfoEntry, AllPlatforms );
        if ( ( MODULETYPE_HEADER_INCLUDE_LOCATION != allPlatformsType ) &&
             ( MODULETYPE_CODE_INCLUDE_LOCATION != allPlatformsType )    )
        {
            CORE::CString fileContent = GeneratePremake4ModuleFileContent( projectInfo, premakeOutputDir, moduleInfoEntry, addCompileDate );
            if ( logFilename.Length() > 0 )
            {
                fileContent += "\n-- Generator logfile can be found at: " + logFilename;
            }

            CORE::CString pathToPremake4ModuleFile = moduleInfoEntry.rootDir;
            CORE::AppendToPath( pathToPremake4ModuleFile, "premake4.lua" );

            if ( CORE::WriteStringAsTextFile( pathToPremake4ModuleFile, fileContent ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Created premake4.lua file for project dir: " + moduleInfoEntry.rootDir );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to write premake4.lua file content to disk at path " + moduleInfoEntry.rootDir );
            }
        }
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Skipping premake4.lua generation for module of type \"HeaderIncludeLocation\"" );
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

CORE::CString
GeneratePremake4ProjectFileContent( const TProjectInfo& projectInfo       ,
                                    const CORE::CString& outputDir        ,
                                    const CORE::CString& premakeOutputDir )
{
    CORE::CString fileContent = GetPremake4FileHeader( false );

    // Define the start and name of a new project (aka solution)
    fileContent += "\n\nsolution( \"" + projectInfo.projectName + "\" )\n\n";

    // Generate the section which defined all configurations available for this Premake4 project
    TStringSet platformsUsed;
    GetAllPlatformsUsed( projectInfo, platformsUsed );
    platformsUsed.erase( AllPlatforms );

    CORE::CString configurationsSection = "  configurations( {";
    bool first = true;
    TStringSet::iterator i = platformsUsed.begin();
    while ( i != platformsUsed.end() )
    {
        if ( first )
        {
            configurationsSection += " \"" + (*i).Uppercase() + "\"";
            first = false;
        }
        else
        {
            configurationsSection += ", \"" + (*i).Uppercase() + "\"";
        }
        ++i;
    }
    configurationsSection += " } )\n\n";

    fileContent += configurationsSection;

    // Set the output path for the premake4 generated files
    if ( !premakeOutputDir.IsNULLOrEmpty() )
    {
        // Check to see if an environment variable is desired
        if ( 0 == premakeOutputDir.HasSubstr( "ENVVAR:", true ) )
        {
            // The path specified is actually a directive to use the given environment variable
            CORE::CString premakeOutputDirEnvVar = premakeOutputDir.CutChars( 7, true );
            fileContent += "  location( os.getenv( \"" + premakeOutputDirEnvVar + "\" ) )\n\n";
        }
        else
        {
            CORE::CString pathToOutputDir = CORE::GetRelativePathToOtherPathRoot( outputDir, premakeOutputDir );
            fileContent += "  location( \"" + pathToOutputDir + "\" )\n\n";
        }
    }

    // Add the module includes
    CORE::CString moduleIncludeListSection = "  --\n  -- Includes for all modules in the solution:\n  --\n";
    TModuleInfoEntryVector::const_iterator n = projectInfo.modules.begin();
    while ( n != projectInfo.modules.end() )
    {
        const TModuleInfoEntry& moduleInfo = (*n);
        if ( HasIndependentModuleType( moduleInfo.modulesPerPlatform ) )
        {
            CORE::CString pathToModuleDir = CORE::GetRelativePathToOtherPathRoot( outputDir, moduleInfo.rootDir );
            moduleIncludeListSection += "  include( \"" + pathToModuleDir + "\" )\n";
        }
        ++n;
    }
    fileContent += moduleIncludeListSection;

    return fileContent;
}

/*---------------------------------------------------------------------------*/

void
WritePremake4ProjectFileToDisk( const TProjectInfo& projectInfo       ,
                                const CORE::CString& outputDir        ,
                                const CORE::CString& premakeOutputDir ,
                                const CORE::CString& logFilename      ,
                                bool addCompileDate = false           )
{
    CORE::CString fileContent = GeneratePremake4ProjectFileContent( projectInfo, outputDir, premakeOutputDir );
    if ( logFilename.Length() > 0 )
    {
        fileContent += "\n-- Generator logfile can be found at: " + logFilename;
    }

    CORE::CString pathToPremake4ProjectFile = outputDir;
    CORE::AppendToPath( pathToPremake4ProjectFile, "premake4.lua" );

    if ( CORE::WriteStringAsTextFile( pathToPremake4ProjectFile, fileContent ) )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Created ProjectInfo premake4.lua file for project dir: " + outputDir );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to write ProjectInfo premake4.lua file content to disk at path " + outputDir );
    }
}
/*--------------------------------------------------------------------------*/

CPremake4ProjectGenerator::CPremake4ProjectGenerator( void )
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/

CPremake4ProjectGenerator::~CPremake4ProjectGenerator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPremake4ProjectGenerator::GenerateProject( TProjectInfo& projectInfo            ,
                                            const CORE::CString& outputDir       ,
                                            bool addGeneratorCompileTimeToOutput ,
                                            const CORE::CValueList& params       )
{GUCEF_TRACE;

    // Prepare by loading up templates if any where provided
    CORE::CString templateDir = params.GetValueAlways( "premake4gen:TemplateDir" );
    if ( !templateDir.IsNULLOrEmpty() )
    {
        LoadPremake4FileAdditionTemplates( templateDir );
    }

    CORE::CString premakeOutputDir = params.GetValueAlways( "premake4gen:PM4OutputDir" );

    // Write the gathered info to disk in premake4 format
    CORE::CString logfilePath;
    bool addLogfilePathToOutput = CORE::StringToBool( params.GetValueAlways( "writeLogLocationToOutput" ) );
    if ( addLogfilePathToOutput )
    {
        logfilePath = params.GetValueAlways( "logfile" );
    }
    WritePremake4ModuleFilesToDisk( projectInfo, premakeOutputDir, logfilePath, addGeneratorCompileTimeToOutput );

    WritePremake4ProjectFileToDisk( projectInfo, outputDir, premakeOutputDir, logfilePath, addGeneratorCompileTimeToOutput );

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
