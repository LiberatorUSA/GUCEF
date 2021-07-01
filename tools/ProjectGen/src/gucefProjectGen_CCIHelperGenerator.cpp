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

void
GeneratePathListPerTarget( const TProjectInfo& projectInfo         ,
                           const TProjectTargetInfoMapMap& targets ,
                           const CORE::CString targetsOutputDir    )
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
            if ( AllPlatforms == platformName )
            {
                ++n;
                continue;
            }

            const TProjectTargetInfo& targetInfo = (*n).second;

            CORE::CString::StringSet allPaths;
            TModuleInfoEntryConstPtrSet::const_iterator m = targetInfo.modules.begin();
            while ( m != targetInfo.modules.end() )
            {
                const TModuleInfoMap& moduleInfoPerPlatform = (*m)->modulesPerPlatform;

                // Grab all file paths for this platform's module definition
                GetAllModuleInfoFilePaths( *(*m), platformName, allPaths );
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
    bool splitTargets = CORE::StringToBool( params.GetValueAlways( "androidmakegen:SplitTargets" ), true );

    CORE::CString targetsOutputDir = params.GetValueAlways( "cihelpergen:TargetsDir" );
    if ( targetsOutputDir.IsNULLOrEmpty() )
        targetsOutputDir = outputDir;    
    targetsOutputDir = CORE::RelativePath( targetsOutputDir, true );
    if ( !CORE::CreateDirs( targetsOutputDir ) )
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to create CI Helper project targets output folder: " + targetsOutputDir );

    TProjectTargetInfoMapMap targets;
    SplitProjectPerTarget( projectInfo, targets, treatTagsAsTargets, true ); 

    GeneratePathListPerTarget( projectInfo, targets, targetsOutputDir );
    
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
