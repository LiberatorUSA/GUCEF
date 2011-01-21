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

#include "gucefProjectGenerator_CXmlProjectGenerator.h"

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

CORE::CDStoreCodecRegistry::TDStoreCodecPtr
GetXmlDStoreCodec( void )
{GUCEF_TRACE;

    static CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr;
    if ( codecPtr.IsNULL() )
    {
        CORE::CDStoreCodecRegistry* registry = CORE::CDStoreCodecRegistry::Instance();
        if ( !registry->TryLookup( "XML", codecPtr, false ) )
        {
            // No codec is registered to handle XML, try and load a plugin for it
            #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32 )

              #ifdef GUCEF_CORE_DEBUG_MODE
              const char* pathToPlugin = "$MODULEDIR$/dstorepluginPARSIFALXML_d";
              #else
              const char* pathToPlugin = "$MODULEDIR$/dstorepluginPARSIFALXML";
              #endif

              CORE::CPluginManager::TPluginPtr codecPlugin =
                  CORE::CDStoreCodecPluginManager::Instance()->LoadPlugin( pathToPlugin );

            #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )

              #ifdef GUCEF_CORE_DEBUG_MODE
              const char* pathToPlugin = "$MODULEDIR$/dstorepluginPARSIFALXML_d";
              #else
              const char* pathToPlugin = "$MODULEDIR$/dstorepluginPARSIFALXML";
              #endif

              CORE::CPluginManager::TPluginPtr codecPlugin =
                  CORE::CDStoreCodecPluginManager::Instance()->LoadPlugin( pathToPlugin );

              if ( NULL == codecPlugin )
              {
                  #ifdef GUCEF_CORE_DEBUG_MODE
                  const char* pathToPlugin = "$MODULEDIR$/../lib/dstorepluginPARSIFALXML_d";
                  #else
                  const char* pathToPlugin = "$MODULEDIR$/../lib/dstorepluginPARSIFALXML";
                  #endif

                  codecPlugin = CORE::CDStoreCodecPluginManager::Instance()->LoadPlugin( pathToPlugin );
              }

            #endif

            if ( NULL != codecPlugin )
            {
                // Now try and get the codec again
                registry->TryLookup( "XML", codecPtr, false );
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Request for data storage codec for xml file, succesfully loaded plugin to handle request" );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Request for data storage codec for xml file but no plugin for it could be loaded!" );
                CORE::ShowErrorMessage( "Missing codec support",
                                        "Request for data storage codec for xml file but no plugin for it could be loaded!" );
            }
        }
    }
    return codecPtr;
}

/*-------------------------------------------------------------------------*/

bool
GenerateProjectInfoDataTree( const TProjectInfo& projectInfo ,
                             CORE::CDataNode& outputInfo     )
{GUCEF_TRACE;

    // start from a clean slate
    outputInfo.Clear();

    // Add project info
    outputInfo.SetName( "Project" );
    outputInfo.SetAttribute( "ModuleCount", CORE::UInt32ToString( projectInfo.modules.size() ) );

    // Add info for each module
    TModuleInfoVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        const TModuleInfo& moduleInfo = (*i);

        // Add basic module info
        CORE::CDataNode moduleInfoNode;
        moduleInfoNode.SetName( "Module" );
        moduleInfoNode.SetAttribute( "Name", moduleInfo.name );
        moduleInfoNode.SetAttribute( "BuildOrder", CORE::Int32ToString( moduleInfo.buildOrder ) );
        moduleInfoNode.SetAttribute( "RootDir", moduleInfo.rootDir );
        moduleInfoNode.SetAttribute( "IsExecutable", CORE::BoolToString( moduleInfo.isExecutable ) );

        // Add headers for all platforms
        CORE::CDataNode headersInfoNode;
        headersInfoNode.SetName( "Files" );
        headersInfoNode.SetAttribute( "Type", "Headers" );
        headersInfoNode.SetAttribute( "Platform", "All" );
        headersInfoNode.SetAttribute( "DirCount", CORE::UInt32ToString( moduleInfo.includeDirs.size() ) );
        TStringVectorMap::const_iterator n = moduleInfo.includeDirs.begin();
        while ( n != moduleInfo.includeDirs.end() )
        {
            CORE::CDataNode pathNode;
            pathNode.SetName( "Dir" );
            pathNode.SetAttribute( "Path", (*n).first );

            CORE::CDataNode fileNode;
            fileNode.SetName( "File" );

            const TStringVector& fileVector = (*n).second;
            pathNode.SetAttribute( "FileCount", CORE::UInt32ToString( fileVector.size() ) );
            TStringVector::const_iterator m = fileVector.begin();
            while ( m != fileVector.end() )
            {
                fileNode.SetAttribute( "Name", (*m) );
                pathNode.AddChild( fileNode );
                ++m;
            }

            headersInfoNode.AddChild( pathNode );
            ++n;
        }
        moduleInfoNode.AddChild( headersInfoNode );
        headersInfoNode.DelSubTree();

        // Add headers for specific platforms
        TStringVectorMapMap::const_iterator x = moduleInfo.platformHeaderFiles.begin();
        while ( x != moduleInfo.platformHeaderFiles.end() )
        {
            const TStringVectorMap& platformHeaders = (*x).second;
            headersInfoNode.SetAttribute( "Platform", (*x).first );
            headersInfoNode.SetAttribute( "DirCount", CORE::UInt32ToString( platformHeaders.size() ) );

            n = platformHeaders.begin();
            while ( n != platformHeaders.end() )
            {
                CORE::CDataNode pathNode;
                pathNode.SetName( "Dir" );
                pathNode.SetAttribute( "Path", (*n).first );

                CORE::CDataNode fileNode;
                fileNode.SetName( "File" );

                const TStringVector& fileVector = (*n).second;
                pathNode.SetAttribute( "FileCount", CORE::UInt32ToString( fileVector.size() ) );
                TStringVector::const_iterator m = fileVector.begin();
                while ( m != fileVector.end() )
                {
                    fileNode.SetAttribute( "Name", (*m) );
                    pathNode.AddChild( fileNode );
                    ++m;
                }

                headersInfoNode.AddChild( pathNode );
                ++n;
            }
            moduleInfoNode.AddChild( headersInfoNode );
            headersInfoNode.DelSubTree();
            ++x;
        }

        // Add sources for all platforms
        CORE::CDataNode sourceInfoNode;
        sourceInfoNode.SetName( "Files" );
        sourceInfoNode.SetAttribute( "Type", "Source" );
        sourceInfoNode.SetAttribute( "Platform", "All" );
        sourceInfoNode.SetAttribute( "DirCount", CORE::UInt32ToString( moduleInfo.sourceDirs.size() ) );
        n = moduleInfo.sourceDirs.begin();
        while ( n != moduleInfo.sourceDirs.end() )
        {
            CORE::CDataNode pathNode;
            pathNode.SetName( "Dir" );
            pathNode.SetAttribute( "Path", (*n).first );

            CORE::CDataNode fileNode;
            fileNode.SetName( "File" );

            const TStringVector& fileVector = (*n).second;
            pathNode.SetAttribute( "FileCount", CORE::UInt32ToString( fileVector.size() ) );
            TStringVector::const_iterator m = fileVector.begin();
            while ( m != fileVector.end() )
            {
                fileNode.SetAttribute( "Name", (*m) );
                pathNode.AddChild( fileNode );
                ++m;
            }

            sourceInfoNode.AddChild( pathNode );
            ++n;
        }
        moduleInfoNode.AddChild( sourceInfoNode );
        sourceInfoNode.DelSubTree();

        // Add sources for specific platforms
        x = moduleInfo.platformSourceFiles.begin();
        while ( x != moduleInfo.platformSourceFiles.end() )
        {
            const TStringVectorMap& platformSources = (*x).second;
            sourceInfoNode.SetAttribute( "Platform", (*x).first );
            sourceInfoNode.SetAttribute( "DirCount", CORE::UInt32ToString( moduleInfo.sourceDirs.size() ) );

            n = platformSources.begin();
            while ( n != platformSources.end() )
            {
                CORE::CDataNode pathNode;
                pathNode.SetName( "Dir" );
                pathNode.SetAttribute( "Path", (*n).first );

                CORE::CDataNode fileNode;
                fileNode.SetName( "File" );

                const TStringVector& fileVector = (*n).second;
                pathNode.SetAttribute( "FileCount", CORE::UInt32ToString( fileVector.size() ) );
                TStringVector::const_iterator m = fileVector.begin();
                while ( m != fileVector.end() )
                {
                    fileNode.SetAttribute( "Name", (*m) );
                    pathNode.AddChild( fileNode );
                    ++m;
                }

                sourceInfoNode.AddChild( pathNode );
                ++n;
            }
            moduleInfoNode.AddChild( sourceInfoNode );
            sourceInfoNode.DelSubTree();
            ++x;
        }

        // Add includes for all platforms
        CORE::CDataNode includesInfoNode;
        includesInfoNode.SetName( "Includes" );
        includesInfoNode.SetAttribute( "Platform", "All" );
        includesInfoNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.dependencyIncludeDirs.size() ) );
        includesInfoNode.SetAttribute( "Source", "Dependency" );
        TStringSet::const_iterator h = moduleInfo.dependencyIncludeDirs.begin();
        while ( h !=  moduleInfo.dependencyIncludeDirs.end() )
        {
            CORE::CDataNode includeNode;
            includeNode.SetName( "Include" );
            includeNode.SetAttribute( "Path", (*h) );

            includesInfoNode.AddChild( includeNode );
            ++h;
        }
        moduleInfoNode.AddChild( includesInfoNode );
        includesInfoNode.DelSubTree();

        // Add includes for specific platforms
        TStringSetMap::const_iterator q = moduleInfo.dependencyPlatformIncludeDirs.begin();
        while ( q != moduleInfo.dependencyPlatformIncludeDirs.end() )
        {
            const TStringSet& platformIncludes = (*q).second;
            includesInfoNode.SetAttribute( "Platform", (*q).first );
            includesInfoNode.SetAttribute( "Count", CORE::UInt32ToString( platformIncludes.size() ) );
            includesInfoNode.SetAttribute( "Source", "Dependency" );

            h = platformIncludes.begin();
            while ( h != platformIncludes.end() )
            {
                CORE::CDataNode includeNode;
                includeNode.SetName( "Include" );
                includeNode.SetAttribute( "Path", (*h) );

                includesInfoNode.AddChild( includeNode );
                ++h;
            }
            moduleInfoNode.AddChild( includesInfoNode );
            includesInfoNode.DelSubTree();
            ++q;
        }

        // Add all the regular include dirs for this module
        // These are already represented in the path attribute of the files section
        // but for ease of processing and clarity they are provided again in the includes section
        includesInfoNode.SetAttribute( "Platform", "All" );
        includesInfoNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.includeDirs.size() ) );
        includesInfoNode.SetAttribute( "Source", "Self" );
        n = moduleInfo.includeDirs.begin();
        while ( n != moduleInfo.includeDirs.end() )
        {
            CORE::CString includeDir = (*n).first.ReplaceChar( '\\', '/' );
            if ( 0 != includeDir.Length() )
            {
                CORE::CDataNode includeNode;
                includeNode.SetName( "Include" );
                includeNode.SetAttribute( "Path", includeDir );
                includesInfoNode.AddChild( includeNode );
            }
            else
            {
                // Check if there is more then one include dir
                // If so we have create an include for an empty include dir
                // to ensure files in subdirs can include the file with the zero length
                // subdir.
                if ( 1 < moduleInfo.includeDirs.size() )
                {
                    CORE::CString includeDir = "../" + CORE::LastSubDir( moduleInfo.rootDir ) + " ";
                    CORE::CDataNode includeNode;
                    includeNode.SetName( "Include" );
                    includeNode.SetAttribute( "Path", includeDir );
                    includesInfoNode.AddChild( includeNode );
                }
            }
            ++n;
        }
        moduleInfoNode.AddChild( includesInfoNode );
        includesInfoNode.DelSubTree();

        //includesInfoNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.platformHeaderFiles.size() ) );
        //n = moduleInfo.includeDirs.begin();
        //while ( n != moduleInfo.includeDirs.end() )
        //{
        //    CORE::CString includeDir = (*n).first.ReplaceChar( '\\', '/' );
        //    if ( 0 != includeDir.Length() )
        //    {
        //        CORE::CDataNode includeNode;
        //        includeNode.SetName( "Include" );
        //        includeNode.SetAttribute( "Path", includeDir );
        //        includesInfoNode.AddChild( includeNode );
        //    }
        //    else
        //    {
        //        // Check if there is more then one include dir
        //        // If so we have create an include for an empty include dir
        //        // to ensure files in subdirs can include the file with the zero length
        //        // subdir.
        //        if ( 1 < moduleInfo.includeDirs.size() )
        //        {
        //            CORE::CString includeDir = "../" + CORE::LastSubDir( moduleInfo.rootDir ) + " ";
        //            CORE::CDataNode includeNode;
        //            includeNode.SetName( "Include" );
        //            includeNode.SetAttribute( "Path", includeDir );
        //            includesInfoNode.AddChild( includeNode );
        //        }
        //    }
        //    ++n;
        //}
        //moduleInfoNode.AddChild( includesInfoNode );
        //includesInfoNode.DelSubTree();

        // Add all the module dependencies
        CORE::CDataNode dependenciesNode;
        dependenciesNode.SetName( "Dependencies" );
        dependenciesNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.dependencies.size() ) );
        TStringVector::const_iterator m = moduleInfo.dependencies.begin();
        while ( m != moduleInfo.dependencies.end() )
        {
            CORE::CDataNode dependencyNode;
            dependencyNode.SetName( "Dependency" );
            dependencyNode.SetAttribute( "Name", (*m) );
            dependenciesNode.AddChild( dependencyNode );
            ++m;
        }
        moduleInfoNode.AddChild( dependenciesNode );

        // Add all the libraries that are linked but not part of the overall project
        CORE::CDataNode linkedLibrariesNode;
        linkedLibrariesNode.SetName( "LinkedLibraries" );
        linkedLibrariesNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.linkedLibraries.size() ) );
        m = moduleInfo.linkedLibraries.begin();
        while ( m != moduleInfo.linkedLibraries.end() )
        {
            CORE::CDataNode libraryNode;
            libraryNode.SetName( "LinkedLibrary" );
            libraryNode.SetAttribute( "Name", (*m) );
            linkedLibrariesNode.AddChild( libraryNode );
            ++m;
        }
        moduleInfoNode.AddChild( linkedLibrariesNode );

        // Add all the info for this module to the overall project
        outputInfo.AddChild( moduleInfoNode );
        ++i;
    }

    return true;
}

/*---------------------------------------------------------------------------*/

bool
WriteProjectInfoDataTreeToDisk( const TProjectInfo& projectInfo         ,
                                const CORE::CString& outputInfoFilename )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Preparing to write all project information to disk file \"" + outputInfoFilename + "\"" );

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec = GetXmlDStoreCodec();
    if ( NULL != codec )
    {
        CORE::CDataNode info;
        if ( GenerateProjectInfoDataTree( projectInfo, info ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully generated a data tree with all project information" );

            if ( codec->StoreDataTree( &info, outputInfoFilename ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully wrote all project information to disk file \"" + outputInfoFilename + "\"" );
                return true;
            }
        }
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Failed to write all project information to disk file \"" + outputInfoFilename + "\"" );
    return false;
}

/*--------------------------------------------------------------------------*/

CXmlProjectGenerator::CXmlProjectGenerator( void )
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/
    
CXmlProjectGenerator::~CXmlProjectGenerator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CXmlProjectGenerator::GenerateProject( TProjectInfo& projectInfo            ,
                                       const CORE::CString& outputDir       ,
                                       bool addGeneratorCompileTimeToOutput )
{GUCEF_TRACE;

    // Write all the project information we gathered to disk 
    CORE::CString projectinfoFilename = outputDir;
    CORE::AppendToPath( projectinfoFilename, "Project.xml" );
    return WriteProjectInfoDataTreeToDisk( projectInfo, projectinfoFilename );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGENERATOR */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
