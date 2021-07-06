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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefProjectGen_CCIHelperGenerator.h"

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

CORE::CString
GenerateValidGithubActionsWorkflowId( const CORE::CString& targetName     ,
                                      const CORE::CString& targetPlatform )
{GUCEF_TRACE;

    // Github workflow IDs "may only contain alphanumeric characters, '_', and '-'. IDs must start with a letter or '_' and and must be less than 100 characters"
    CORE::CString suggestedworkflowId = targetName + '-' + targetPlatform;

    // Let's avoid non-ASCII stumbling blocks and force the down to ASCII for the IDs
    CORE::CAsciiString asciiWorkflowPermittedId = suggestedworkflowId.ForceToAscii( '_' );
    
    // Replace special chars
    char disallowedChars[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '=', '+', ',', '.', '<', '>', '/', '?', '`', '~', '\\', '|', '{', '}', '[', ']', ';', ':', '\'', '\"' };
    asciiWorkflowPermittedId = asciiWorkflowPermittedId.ReplaceChars( disallowedChars, sizeof(disallowedChars)/sizeof(char), '_' );

    // Check the first char restriction and prefix an underscore if need be
    char firstIdChar = asciiWorkflowPermittedId[ 0 ];
    if ( !( ( firstIdChar >= 65 && firstIdChar <= 90 ) || ( firstIdChar >= 97 && firstIdChar <= 122 ) || ( firstIdChar == '_' ) ) )
        asciiWorkflowPermittedId = '_' + asciiWorkflowPermittedId;

    // Back to the platform wide string convention format
    CORE::CString workflowPermittedId = CORE::ToString( asciiWorkflowPermittedId );
    
    // The string also can't be too long
    if ( workflowPermittedId.Length() > 100 )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGithubActionsWorkflowProjectsYml: Workflow ID \"" + workflowPermittedId + "\" is too long. We will truncate it to the permitted max of 100 chars" );
        workflowPermittedId = workflowPermittedId.CutChars( workflowPermittedId.Length()-100, false, 0 );
    }
    
    return workflowPermittedId;
}

/*-------------------------------------------------------------------------*/

CORE::CString
GenerateGithubActionsWorkflowProjectSection( const CORE::CString& targetName                  ,
                                             const CORE::CString& targetPlatform              ,
                                             const CORE::CString& productName                 ,
                                             const CORE::CString& pathToCMakeTargetsOutputDir )
{GUCEF_TRACE;

    CORE::CString section;
    CORE::CString workingDir;
    
    if ( "linux32" == targetPlatform || "linux64" == targetPlatform || AllPlatforms == targetPlatform )
    {
        // Change cmake relative path dir sep to match linux
        workingDir = pathToCMakeTargetsOutputDir.ReplaceChar( '\\', '/' );
        workingDir += '/' + targetName;
        
        section = "  $workflowId$:\n"
                  "    if: github.event.client_payload.job == '$targetName$-$targetPlatform$'\n"
                  "    runs-on: ubuntu-latest\n"
                  "    steps:\n"
                  "      - uses: actions/checkout@master\n"
                  "      - name: Build $productName$ for platform $targetPlatform$\n"
                  "        working-directory: ./$workingDir$\n"
                  "        run: ./cmake --build\n";
    }
    else
    if ( "win32" == targetPlatform || "win64" == targetPlatform )
    {
        // Change cmake relative path dir sep to match linux
        workingDir = pathToCMakeTargetsOutputDir.ReplaceChar( '/', '\\' );
        workingDir += '\\' + targetName;
        
        section = "  $workflowId$:\n"
                  "    if: github.event.client_payload.job == '$targetName$-$targetPlatform$'\n"
                  "    runs-on: windows-latest\n"
                  "    steps:\n"
                  "      - uses: actions/checkout@master\n"
                  "      - name: Build $productName$ for platform $targetPlatform$\n"
                  "        working-directory: .\\$workingDir$\n"
                  "        run: ./cmake --build\n";
    }
    
    if ( !section.IsNULLOrEmpty() )
    {
        CORE::CString workflowPermittedId = GenerateValidGithubActionsWorkflowId( targetName, targetPlatform );

        section = section.ReplaceSubstr( "$workflowId$", workflowPermittedId );
        section = section.ReplaceSubstr( "$targetName$", targetName ); 
        section = section.ReplaceSubstr( "$targetPlatform$", targetPlatform ); 
        section = section.ReplaceSubstr( "$productName$", productName ); 
        section = section.ReplaceSubstr( "$workingDir$", workingDir ); 
        
        return section;
    }
    return CORE::CString::Empty; 
}

/*-------------------------------------------------------------------------*/

void
GenerateGithubActionsWorkflowProjectsYml( const TProjectInfo& projectInfo                ,
                                          const TProjectTargetInfoMapMap& targets        ,
                                          const CORE::CString::StringSet& platformFilter ,
                                          const CORE::CString& cmakeTargetsOutputDir     )
{GUCEF_TRACE;

    CORE::CString githubActionsWorkflowProjectsContent =
        "name: projects\n"             
        "\n"                           
        "on: repository_dispatch\n"    
        "\n"                           
        "jobs:\n"                      
        "\n";

    // We need the relative path not absolute for the yml
    CORE::CString pathToCMakeTargetsOutputDir = GetShortestRelativePathFromAbsPathToProjectRoot( projectInfo, cmakeTargetsOutputDir );
    
    TProjectTargetInfoMapMap::const_iterator i = targets.begin();
    while ( i != targets.end() )
    {
        const CORE::CString& targetName = (*i).first;
        const TProjectTargetInfoMap& targetsPerPlatform = (*i).second;

        TProjectTargetInfoMap::const_iterator n = targetsPerPlatform.begin();
        while ( n != targetsPerPlatform.end() )
        {
            const CORE::CString& platformName = (*n).first;
            if ( platformFilter.find( platformName ) != platformFilter.end() )
            {
                const TProjectTargetInfo& targetInfo = (*n).second;

                CORE::CString productName;
                if ( GUCEF_NULL == targetInfo.mainModule )
                {
                    productName = targetInfo.projectName;
                }
                else
                {
                    productName = GetConsensusModuleName( *targetInfo.mainModule );
                }
            
                CORE::CString section = GenerateGithubActionsWorkflowProjectSection( targetInfo.projectName, platformName, productName, pathToCMakeTargetsOutputDir ); 
                if ( !section.IsNULLOrEmpty() )
                {
                    githubActionsWorkflowProjectsContent += section;
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGithubActionsWorkflowProjectsYml: Generated Github Actions Workflow job to build target \"" + targetName + "\" with project name \"" + targetInfo.projectName + "\" and platform \"" + platformName + "\"" );
                }
                else
                {
                    // not supported atm
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGithubActionsWorkflowProjectsYml: Could not generate Github Actions Workflow job to build target \"" + targetName + "\" with project name \"" + targetInfo.projectName + "\" and platform \"" + platformName + "\"" );
                }
            }
            ++n;
        }
        ++i;
    }

    TStringVector::const_iterator m = projectInfo.rootDirs.begin();
    while ( m != projectInfo.rootDirs.end() )
    {
        CORE::CString githubActionsWorkflowProjectsYmlPath = CORE::CombinePath( (*m), ".github/workflows/" );
        githubActionsWorkflowProjectsYmlPath = githubActionsWorkflowProjectsYmlPath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR );

        if ( !CORE::CreateDirs( githubActionsWorkflowProjectsYmlPath ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGithubActionsWorkflowProjectsYml: Failed to create directories. Dir path: " + githubActionsWorkflowProjectsYmlPath );
            ++m;
            continue;
        }
        githubActionsWorkflowProjectsYmlPath = CORE::CombinePath( githubActionsWorkflowProjectsYmlPath, "projects.yml" );

        if ( CORE::WriteStringAsTextFile( githubActionsWorkflowProjectsYmlPath, githubActionsWorkflowProjectsContent ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGithubActionsWorkflowProjectsYml: Successfully wrote Github Actions Workflow file with project jobs to: " + githubActionsWorkflowProjectsYmlPath );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGithubActionsWorkflowProjectsYml: Failed to write Github Actions Workflow file with project jobs to: " + githubActionsWorkflowProjectsYmlPath );
        }

        ++m;
    }
}

/*-------------------------------------------------------------------------*/

void
GenerateFilePathListPerTarget( const TProjectInfo& projectInfo                ,
                               const TProjectTargetInfoMapMap& targets        ,
                               const CORE::CString::StringSet& platformFilter ,
                               const CORE::CString& targetsOutputDir          )
{GUCEF_TRACE;

    TProjectTargetInfoMapMap::const_iterator i = targets.begin();
    while ( i != targets.end() )
    {
        const CORE::CString& targetName = (*i).first;
        const TProjectTargetInfoMap& targetsPerPlatform = (*i).second;

        TProjectTargetInfoMap::const_iterator n = targetsPerPlatform.begin();
        while ( n != targetsPerPlatform.end() )
        {
            const CORE::CString& platformName = (*n).first;
            if ( platformFilter.find( platformName ) != platformFilter.end() )
            {
                const TProjectTargetInfo& targetInfo = (*n).second;

                CORE::CString::StringSet allPaths;
                TModuleInfoEntryConstPtrSet::const_iterator m = targetInfo.modules.begin();
                while ( m != targetInfo.modules.end() )
                {
                    const TModuleInfoMap& moduleInfoPerPlatform = (*m)->modulesPerPlatform;

                    // Grab all file paths for this platform's module definition
                    GetAllModuleInfoFilePaths( projectInfo, *(*m), platformName, allPaths, true, true );
                    ++m;
                }
            
                CORE::CString pathListFileContent;
                CORE::CString::StringSet::iterator p = allPaths.begin();
                while ( p != allPaths.end() )
                {
                    pathListFileContent += (*p) + '\n';
                    ++p;
                }

                CORE::CString pathListOutputPath = CORE::CombinePath( targetsOutputDir, platformName );
                pathListOutputPath = CORE::CombinePath( pathListOutputPath, targetName );

                if ( !CORE::CreateDirs( pathListOutputPath ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GenerateFilePathListPerTarget: Failed to create directories while generating list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\". Dir path: " + pathListOutputPath );
                    ++n;
                    continue;
                }

                pathListOutputPath = CORE::CombinePath( pathListOutputPath, "filepaths.txt" );

                if ( CORE::WriteStringAsTextFile( pathListOutputPath, pathListFileContent ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GenerateFilePathListPerTarget: Successfully wrote list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\" to: " + pathListOutputPath );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GenerateFilePathListPerTarget: Failed to write list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\" to: " + pathListOutputPath );
                }
            }
            ++n;
        }
        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
GeneratePathListPerTarget( const TProjectInfo& projectInfo                ,
                           const TProjectTargetInfoMapMap& targets        ,
                           const CORE::CString::StringSet& platformFilter ,
                           const CORE::CString targetsOutputDir           )
{GUCEF_TRACE;

    TProjectTargetInfoMapMap::const_iterator i = targets.begin();
    while ( i != targets.end() )
    {
        const CORE::CString& targetName = (*i).first;
        const TProjectTargetInfoMap& targetsPerPlatform = (*i).second;

        TProjectTargetInfoMap::const_iterator n = targetsPerPlatform.begin();
        while ( n != targetsPerPlatform.end() )
        {
            const CORE::CString& platformName = (*n).first;
            if ( platformFilter.find( platformName ) != platformFilter.end() )
            {
                const TProjectTargetInfo& targetInfo = (*n).second;

                CORE::CString::StringSet allPaths;
                TModuleInfoEntryConstPtrSet::const_iterator m = targetInfo.modules.begin();
                while ( m != targetInfo.modules.end() )
                {
                    const TModuleInfoMap& moduleInfoPerPlatform = (*m)->modulesPerPlatform;

                    // Grab all paths for this platform's module definition
                    GetAllModuleInfoPaths( projectInfo, *(*m), platformName, allPaths, true, true, true );
                    ++m;
                }
            
                CORE::CString pathListFileContent;
                CORE::CString::StringSet::iterator p = allPaths.begin();
                while ( p != allPaths.end() )
                {
                    pathListFileContent += (*p) + '\n';
                    ++p;
                }

                CORE::CString pathListOutputPath = CORE::CombinePath( targetsOutputDir, platformName );
                pathListOutputPath = CORE::CombinePath( pathListOutputPath, targetName );

                if ( !CORE::CreateDirs( pathListOutputPath ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GeneratePathListPerTarget: Failed to create directories while generating list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\". Dir path: " + pathListOutputPath );
                    ++n;
                    continue;
                }

                pathListOutputPath = CORE::CombinePath( pathListOutputPath, "paths.txt" );

                if ( CORE::WriteStringAsTextFile( pathListOutputPath, pathListFileContent ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GeneratePathListPerTarget: Successfully wrote list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\" to: " + pathListOutputPath );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GeneratePathListPerTarget: Failed to write list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\" to: " + pathListOutputPath );
                }
            }
            ++n;
        }
        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
GenerateGlobPatternPathListPerTarget( const TProjectInfo& projectInfo                ,
                                      const TProjectTargetInfoMapMap& targets        ,
                                      const CORE::CString::StringSet& platformFilter ,
                                      const CORE::CString targetsOutputDir           )
{GUCEF_TRACE;

    TProjectTargetInfoMapMap::const_iterator i = targets.begin();
    while ( i != targets.end() )
    {
        const CORE::CString& targetName = (*i).first;
        const TProjectTargetInfoMap& targetsPerPlatform = (*i).second;

        TProjectTargetInfoMap::const_iterator n = targetsPerPlatform.begin();
        while ( n != targetsPerPlatform.end() )
        {
            const CORE::CString& platformName = (*n).first;
            if ( platformFilter.find( platformName ) != platformFilter.end() )
            {
                const TProjectTargetInfo& targetInfo = (*n).second;

                CORE::CString::StringSet allPaths;
                TModuleInfoEntryConstPtrSet::const_iterator m = targetInfo.modules.begin();
                while ( m != targetInfo.modules.end() )
                {
                    const TModuleInfoMap& moduleInfoPerPlatform = (*m)->modulesPerPlatform;

                    // Grab all paths for this platform's module definition
                    GetAllModuleInfoPaths( projectInfo, *(*m), platformName, allPaths, true, true, true );
                    ++m;
                }

                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGlobPatternPathListPerTarget: Found " + CORE::ToString( allPaths.size() ) + " paths. For glob matching we will now eliminate glob patterns that are more specific version of a more generic one" );

                CORE::CString::StringSet allGlobPaths;
                CORE::CString::StringSet::iterator p = allPaths.begin();
                while ( p != allPaths.end() )
                {
                    const CORE::CString& testPath = (*p);

                    bool shorterVersionFound = false;                    
                    CORE::CString::StringSet::iterator p2 = p;
                    while ( p2 != allPaths.begin() )
                    {
                        // Since this is a sorted list shorter entries, thus more global, will come before the item itself
                        --p2;
                        const CORE::CString& otherPath = (*p2);
                        
                        if ( testPath.HasSubstr( otherPath, true ) >= 0 )
                        {
                            shorterVersionFound = true;
                            break;
                        }
                    }

                    if ( !shorterVersionFound )
                    {
                        allGlobPaths.insert( testPath );
                    }

                    ++p;
                }
                
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGlobPatternPathListPerTarget: Found " + CORE::ToString( allGlobPaths.size() ) + " non-overlapping paths. Eliminated " + CORE::ToString( allPaths.size() - allGlobPaths.size() ) + " as redundant for glob pattern generation" );
                
                CORE::CString pathListFileContent;
                p = allGlobPaths.begin();
                while ( p != allGlobPaths.end() )
                {
                    CORE::CString path = (*p).ReplaceChar( '\\', '/' ) + "/*\n";
                    pathListFileContent += path;
                    ++p;
                }

                CORE::CString pathListOutputPath = CORE::CombinePath( targetsOutputDir, platformName );
                pathListOutputPath = CORE::CombinePath( pathListOutputPath, targetName );

                if ( !CORE::CreateDirs( pathListOutputPath ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGlobPatternPathListPerTarget: Failed to create directories while generating list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\". Dir path: " + pathListOutputPath );
                    ++n;
                    continue;
                }

                pathListOutputPath = CORE::CombinePath( pathListOutputPath, "globpaths.txt" );

                if ( CORE::WriteStringAsTextFile( pathListOutputPath, pathListFileContent ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGlobPatternPathListPerTarget: Successfully wrote list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\" to: " + pathListOutputPath );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "GenerateGlobPatternPathListPerTarget: Failed to write list of relevant file paths for target \"" + targetName + "\" and platform \"" + platformName + "\" to: " + pathListOutputPath );
                }
            }
            ++n;
        }
        
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

CCIHelperGenerator::CCIHelperGenerator( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CCIHelperGenerator::~CCIHelperGenerator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CCIHelperGenerator::GenerateProject( TProjectInfo& projectInfo            ,
                                     const CORE::CString& outputDir       ,
                                     bool addGeneratorCompileTimeToOutput ,
                                     const CORE::CValueList& params       )
{GUCEF_TRACE;

    bool treatTagsAsTargets = CORE::StringToBool( params.GetValueAlways( "TreatTagsAsTargets" ), true );  
    CORE::CString targetsOutputDir = params.GetValueAlways( "cihelpergen:TargetsDir" );
    
    CORE::CString::StringSet platformFilter = params.GetValueAlways( "cihelpergen:platformFilter" ).ParseUniqueElements( ',', false );
    if ( platformFilter.empty() )
        GetAllPlatformsUsed( projectInfo, platformFilter );

    if ( targetsOutputDir.IsNULLOrEmpty() )
        targetsOutputDir = outputDir;    
    targetsOutputDir = CORE::RelativePath( targetsOutputDir, true );
    if ( !CORE::CreateDirs( targetsOutputDir ) )
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to create CI Helper project targets output folder: " + targetsOutputDir );

    TProjectTargetInfoMapMap targets;
    SplitProjectPerTarget( projectInfo, targets, treatTagsAsTargets, true ); 

    bool generateFilePathListPerTarget = CORE::StringToBool( params.GetValueAlways( "cihelpergen:GenerateFilePathListPerTarget" ), false );
    bool generatePathListPerTarget = CORE::StringToBool( params.GetValueAlways( "cihelpergen:GeneratePathListPerTarget" ), false );
    bool generateGlobPatternPathListPerTarget = CORE::StringToBool( params.GetValueAlways( "cihelpergen:GenerateGlobPatternPathListPerTarget" ), true );
    bool generateGithubActionsWorkflowProjectsYml = CORE::StringToBool( params.GetValueAlways( "cihelpergen:GenerateGithubActionsWorkflowProjectsYml" ), true );
    
    CORE::CString cmakeTargetsOutputDir = params.GetValueAlways( "cmakegen:TargetsDir" );
    if ( cmakeTargetsOutputDir.IsNULLOrEmpty() )
        cmakeTargetsOutputDir = outputDir;    
    cmakeTargetsOutputDir = CORE::RelativePath( cmakeTargetsOutputDir, true );

    if ( generateFilePathListPerTarget )
        GenerateFilePathListPerTarget( projectInfo, targets, platformFilter, targetsOutputDir );
    if ( generatePathListPerTarget )
        GeneratePathListPerTarget( projectInfo, targets, platformFilter, targetsOutputDir );
    if ( generateGlobPatternPathListPerTarget )
        GenerateGlobPatternPathListPerTarget( projectInfo, targets, platformFilter, targetsOutputDir );
        
    if ( generateGithubActionsWorkflowProjectsYml )
        GenerateGithubActionsWorkflowProjectsYml( projectInfo, targets, platformFilter, cmakeTargetsOutputDir );
    
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
