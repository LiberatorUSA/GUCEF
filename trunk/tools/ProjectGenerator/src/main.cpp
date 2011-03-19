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

#include <stdio.h>

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

#ifndef GUCEF_PROJECTGENERATOR_DATATYPES_H
#include "gucefProjectGenerator_DataTypes.h"
#define GUCEF_PROJECTGENERATOR_DATATYPES_H
#endif /* GUCEF_PROJECTGENERATOR_DATATYPES_H ? */

#ifndef GUCEF_PROJECTGENERATOR_CIPROJECTGENERATOR_H
#include "gucefProjectGenerator_CIProjectGenerator.h"
#define GUCEF_PROJECTGENERATOR_CIPROJECTGENERATOR_H
#endif /* GUCEF_PROJECTGENERATOR_CIPROJECTGENERATOR_H ? */

#ifndef GUCEF_PROJECTGENERATOR_CIPROJECTINFOGATHERER_H
#include "gucefProjectGenerator_CIProjectInfoGatherer.h"
#define GUCEF_PROJECTGENERATOR_CIPROJECTINFOGATHERER_H
#endif /* GUCEF_PROJECTGENERATOR_CIPROJECTINFOGATHERER_H ? */

#ifndef GUCEF_PROJECTGENERATOR_CDIRCRAWLINGPROJECTINFOGATHERER_H
#include "gucefProjectGenerator_CDirCrawlingProjectInfoGatherer.h"
#define GUCEF_PROJECTGENERATOR_CDIRCRAWLINGPROJECTINFOGATHERER_H
#endif /* GUCEF_PROJECTGENERATOR_CDIRCRAWLINGPROJECTINFOGATHERER_H ? */

#ifndef GUCEF_PROJECTGENERATOR_CXMLPROJECTGENERATOR_H
#include "gucefProjectGenerator_CXmlProjectGenerator.h"
#define GUCEF_PROJECTGENERATOR_CXMLPROJECTGENERATOR_H
#endif /* GUCEF_PROJECTGENERATOR_CXMLPROJECTGENERATOR_H ? */

#ifndef GUCEF_PROJECTGENERATOR_CCMAKEPROJECTGENERATOR_H
#include "gucefProjectGenerator_CCMakeProjectGenerator.h"
#define GUCEF_PROJECTGENERATOR_CCMAKEPROJECTGENERATOR_H
#endif /* GUCEF_PROJECTGENERATOR_CCMAKEPROJECTGENERATOR_H ? */

#ifndef GUCEF_PROJECTGENERATOR_CANDROIDMAKEFILEGENERATOR_H
#include "gucefProjectGenerator_CAndroidMakefileGenerator.h"
#define GUCEF_PROJECTGENERATOR_CANDROIDMAKEFILEGENERATOR_H
#endif /* GUCEF_PROJECTGENERATOR_CANDROIDMAKEFILEGENERATOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;
using namespace GUCEF::PROJECTGENERATOR;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

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
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Application parameters: " + argString );
        keyValueList.SetMultiple( argString, '*' );
    }
}

/*---------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "This tool was compiled on: " __DATE__ " @ " __TIME__ );

    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );

    CORE::CString outputDir = keyValueList.GetValueAlways( "outputDir" );
    if ( outputDir.IsNULLOrEmpty() )
    {
        outputDir = CORE::RelativePath( "$CURWORKDIR$" );
    }

    CORE::CString logFilename = outputDir;
    CORE::AppendToPath( logFilename, "ProjectGenerator_Log.txt" );
    CORE::CFileAccess logFileAccess( logFilename, "w" );

    CORE::CStdLogger logger( logFileAccess );
    CORE::CLogManager::Instance()->AddLogger( &logger );

    CORE::CPlatformNativeConsoleLogger console;
    CORE::CLogManager::Instance()->AddLogger( console.GetLogger() );

    TStringVector rootDirs;
    try
    {
        rootDirs = keyValueList.GetValueVector( "rootDir" );
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Number of rootDir arguments passed from command line: " + CORE::UInt32ToString( rootDirs.size() ) );
    }
    catch ( CORE::CValueList::EUnknownKey& )
    {
        rootDirs.push_back( CORE::RelativePath( "$CURWORKDIR$" ) );
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Using current working directory since no rootDir arguments where passed from the command line" );
    }
    
    bool addToolCompileTimeToOutput = CORE::StringToBool( keyValueList.GetValueAlways( "addToolCompileTimeToOutput" ) );
    
    // Get the generators to use
    TStringVector generatorList = keyValueList.GetValueAlways( "generators" ).ParseElements( ';', false );
    if ( generatorList.size() == 0  )
    {
        // No specific generators where specified, defaulting...
        generatorList.push_back( "xml" );
    }

    // Set any global dir excludes that where passed as cmd parameters
    TProjectInfo projectInfo;
    projectInfo.globalDirExcludeList = keyValueList.GetValueAlways( "dirsToIgnore" ).ParseElements( ';', false );
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "There are " + CORE::UInt32ToString( projectInfo.globalDirExcludeList.size() ) + " dirs in the global dir ignore list" );

    projectInfo.projectName = keyValueList.GetValueAlways( "projectName" );

    // Use an info gatherer to get all the project information for us
    CDirCrawlingProjectInfoGatherer infoGatherer;    
    infoGatherer.GatherInfo( rootDirs    ,
                             projectInfo );
                             
    // Now we output the project info using all generators specified
    TStringVector::iterator i = generatorList.begin();
    while ( i != generatorList.end() )
    {
        if ( (*i).Lowercase() == "xml" )
        {
            CXmlProjectGenerator xmlGenerator;
            xmlGenerator.GenerateProject( projectInfo                ,
                                          outputDir                  ,
                                          addToolCompileTimeToOutput );
        }
        else
        if ( (*i).Lowercase() == "androidmake" )
        {
            CAndroidMakefileGenerator androidMakefileGenerator;
            androidMakefileGenerator.GenerateProject( projectInfo                ,
                                                      outputDir                  ,
                                                      addToolCompileTimeToOutput );
        }
        else
        if ( (*i).Lowercase() == "cmake" )
        {
            CCMakeProjectGenerator cmakeGenerator;
            cmakeGenerator.GenerateProject( projectInfo                ,
                                            outputDir                  ,
                                            addToolCompileTimeToOutput );
        }
        ++i;
    }
    
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Wrote log file to: " + logFilename );

    return 0;
}
GUCEF_OSMAIN_END

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
