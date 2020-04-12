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

#ifndef GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#include "CDStoreCodecPluginManager.h"
#define GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H
#endif /* GUCEF_CORE_CDSTORECODECPLUGINMANAGER_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#include "gucefProjectGen_DataTypes.h"

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

const CORE::CString AllPlatforms = "all";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static bool
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

/*---------------------------------------------------------------------------*/

static bool
IsStringInList( const TStringSet& list          ,
                bool caseSensitive              ,
                const CORE::CString& testString )
{GUCEF_TRACE;

    TStringSet::const_iterator i = list.begin();
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

/*---------------------------------------------------------------------------*/

static bool
IsStringInKeyList( const TStringVectorMap& list    ,
                   bool caseSensitive              ,
                   const CORE::CString& testString )
{GUCEF_TRACE;

    TStringVectorMap::const_iterator i = list.begin();
    while ( i != list.end() )
    {
        if ( (*i).first.Equals( testString, caseSensitive ) )
        {
            return true;
        }
        ++i;
    }
    return false;
}

/*---------------------------------------------------------------------------*/

void
MergeStringVector( TStringVector& targetList          ,
                   const TStringVector& listToMergeIn ,
                   bool caseSensitive                 )
{GUCEF_TRACE;

    TStringVector::const_iterator i = listToMergeIn.begin();
    while ( i != listToMergeIn.end() )
    {
        if ( !IsStringInList( targetList, caseSensitive, (*i) ) )
        {
            targetList.push_back( (*i) );
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

static bool
IsKeyStringInMap( const TStringMap& testMap       ,
                  bool caseSensitive              ,
                  const CORE::CString& testString )
{
    if ( !caseSensitive )
    {
        TStringMap::const_iterator i = testMap.find( testString );
        return i != testMap.end();
    }
    
    TStringMap::const_iterator i = testMap.begin();
    while ( i != testMap.end() )
    {
        if ( (*i).first.Equals( testString, caseSensitive )  )
        {
            return true;
        }
        ++i;
    }  
    return false;
}

/*---------------------------------------------------------------------------*/

static void
MergeStringMap( TStringMap& targetMap          ,
                const TStringMap& mapToMergeIn ,
                bool caseSensitive             ,
                bool concatExistingEntries     )
{GUCEF_TRACE;

    TStringMap::const_iterator i = mapToMergeIn.begin();
    while ( i != mapToMergeIn.end() )
    {
        if ( !IsKeyStringInMap( targetMap, caseSensitive, (*i).first ) )
        {
            targetMap.insert( (*i) );
        }
        else
        {
            if ( concatExistingEntries )
            {
                targetMap[ (*i).first ] += (*i).second;
            }
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
MergeStringSet( TStringSet& targetList          ,
                const TStringSet& listToMergeIn ,
                bool caseSensitive              )
{GUCEF_TRACE;

    TStringSet::const_iterator i = listToMergeIn.begin();
    while ( i != listToMergeIn.end() )
    {
        if ( caseSensitive )
        {
            TStringSet::iterator n = targetList.find( (*i) );
            if ( n == targetList.end() )
            {
                targetList.insert( (*i) );
            }
        }
        else
        {
            if ( !IsStringInList( targetList, caseSensitive, (*i) ) )
            {
                targetList.insert( (*i) );
            }
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

TStringSet
StringVectorToStringSet( const TStringVector& stringVector )
{GUCEF_TRACE;

    TStringSet stringSet;
    TStringVector::const_iterator i = stringVector.begin();
    while ( i != stringVector.end() )
    {
        stringSet.insert( (*i) );
        ++i;
    }
    return stringSet;
}

/*---------------------------------------------------------------------------*/

CORE::CString
ModuleTypeToString( const TModuleType moduleType )
{GUCEF_TRACE;

    switch ( moduleType )
    {
        case MODULETYPE_UNDEFINED: return "";
        case MODULETYPE_EXECUTABLE: return "Executable";
        case MODULETYPE_SHARED_LIBRARY: return "SharedLibrary";
        case MODULETYPE_STATIC_LIBRARY: return "StaticLibrary";
        case MODULETYPE_HEADER_INCLUDE_LOCATION: return "HeaderIncludeLocation";
        case MODULETYPE_HEADER_INTEGRATE_LOCATION: return "HeaderIntegrateLocation";
        case MODULETYPE_CODE_INTEGRATE_LOCATION: return "CodeIntegrateLocation";
        case MODULETYPE_REFERENCE_LIBRARY: return "ReferenceLibrary";
        case MODULETYPE_BINARY_PACKAGE : return "BinaryPackage";
        case MODULETYPE_UNKNOWN: return "Unknown";
        default: return "";
    }
}

/*-------------------------------------------------------------------------*/

TModuleType
StringToModuleType( const CORE::CString moduleTypeStr )
{
    CORE::CString moduleTypeString = moduleTypeStr.Lowercase();
    if ( moduleTypeString == "" ) return MODULETYPE_UNDEFINED;
    if ( moduleTypeString == "executable" ) return MODULETYPE_EXECUTABLE;
    if ( moduleTypeString == "sharedlibrary" ) return MODULETYPE_SHARED_LIBRARY;
    if ( moduleTypeString == "staticlibrary" ) return MODULETYPE_STATIC_LIBRARY;
    if ( moduleTypeString == "headerincludelocation" ) return MODULETYPE_HEADER_INCLUDE_LOCATION;
    if ( moduleTypeString == "headerintegratelocation" ) return MODULETYPE_HEADER_INTEGRATE_LOCATION;
    if ( moduleTypeString == "codeintegratelocation" ) return MODULETYPE_CODE_INTEGRATE_LOCATION;
    if ( moduleTypeString == "referencelibrary" ) return MODULETYPE_REFERENCE_LIBRARY;
    if ( moduleTypeString == "binarypackage" ) return MODULETYPE_BINARY_PACKAGE;
    if ( moduleTypeString == "unknown" ) return MODULETYPE_UNKNOWN;
    return MODULETYPE_UNDEFINED;
}

/*-------------------------------------------------------------------------*/

CORE::CDStoreCodecRegistry::TDStoreCodecPtr
GetXmlDStoreCodec( void )
{GUCEF_TRACE;

    static CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr;
    if ( codecPtr.IsNULL() )
    {
        CORE::CDStoreCodecRegistry* registry = &CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry();
        if ( !registry->TryLookup( "XML", codecPtr, false ) )
        {
            // No codec is registered to handle XML, try and load a plugin for it
            #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

              #ifdef GUCEF_CORE_DEBUG_MODE
              const char* pathToPlugin = "$MODULEDIR$/dstorepluginPARSIFALXML_d";
              #else
              const char* pathToPlugin = "$MODULEDIR$/dstorepluginPARSIFALXML";
              #endif

            if ( !CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginFromDir( pathToPlugin    ,
                                                                                      CORE::CString() ,
                                                                                      true            ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to load plugin from " + CORE::CString( pathToPlugin ) );
                return NULL;
            }

            #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

              #ifdef GUCEF_CORE_DEBUG_MODE
              const char* pathToPlugin = "$MODULEDIR$/dstorepluginPARSIFALXML_d";
              #else
              const char* pathToPlugin = "$MODULEDIR$/dstorepluginPARSIFALXML";
              #endif

            if ( !CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginFromDir( pathToPlugin    ,
                                                                                      CORE::CString() ,
                                                                                      true            ) )
            {
                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Unable to load plugin from " + CORE::CString( pathToPlugin ) + " attempting alternate location" );

                #ifdef GUCEF_CORE_DEBUG_MODE
                const char* pathToPlugin = "$MODULEDIR$/../lib/dstorepluginPARSIFALXML_d";
                #else
                const char* pathToPlugin = "$MODULEDIR$/../lib/dstorepluginPARSIFALXML";
                #endif

                if ( !CORE::CCoreGlobal::Instance()->GetPluginControl().AddPluginFromDir( pathToPlugin    ,
                                                                                          CORE::CString() ,
                                                                                          true            ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to load plugin from " + CORE::CString( pathToPlugin ) );
                }
            }

            #else

            // Plugin loading not supported
            GUCEF_ERROR_LOG( CORE::LOGVEL_NORMAL, "Plugin loading is not supported for this platform via the ProjectGenerator" );

            #endif

            // Now try and get the codec again
            if ( registry->TryLookup( "XML", codecPtr, false ) )
            {
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

/*---------------------------------------------------------------------------*/

bool
ContainsFileWithFileExtension( const TStringVectorMap& files ,
                               const CORE::CString& fileExt  )
{GUCEF_TRACE;

    TStringVectorMap::const_iterator i = files.begin();
    while ( i != files.end() )
    {
        TStringVector::const_iterator n = (*i).second.begin();
        while ( n != (*i).second.end() )
        {
            if ( fileExt.Equals( CORE::ExtractFileExtention( (*n) ), false ) )
                return true;
            ++n;
        }
        ++i;
    }
    return false;
}

/*---------------------------------------------------------------------------*/

void
ApplyConfigToProject( const CORE::CDataNode& loadedConfig , 
                      TProjectInfo& projectInfo           )
{GUCEF_TRACE;
    
    CORE::CDataNode::TConstDataNodeSet platformDefs = loadedConfig.FindChildrenOfType( "PlatformDefinitions", true );
    CORE::CDataNode::TConstDataNodeSet::const_iterator i = platformDefs.begin();
    while ( i != platformDefs.end() )
    {
        const CORE::CDataNode* definitionGroup = (*i);
        CORE::CDataNode::const_iterator n = definitionGroup->ConstBegin();
        while ( n != definitionGroup->ConstEnd() )
        {
            const CORE::CDataNode* platform = (*n);
            const CORE::CString platformName = platform->GetName();
            CORE::CString aliases = platform->GetAttributeValueOrChildValueByName( "Aliases" );
            CORE::CString platformDirs = platform->GetAttributeValueOrChildValueByName( "PlatformDirs" );

            TPlatformDefinition& platformDef = (projectInfo.platforms)[ platformName ];
            platformDef.aliases = StringVectorToStringSet( aliases.ParseElements( ';', false ) );
            platformDef.platformDirs = StringVectorToStringSet( platformDirs.ParseElements( ';', false ) );

            ++n;
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetLanguageForModule( const TModuleInfo& moduleInfo )
{GUCEF_TRACE;

    const TStringSet& languageSet = moduleInfo.compilerSettings.languagesUsed;
    if ( languageSet.empty() )
    {
        // No language was specified but premake requires one
        // We will determine the languege based on the files in the project
        if ( ContainsFileWithFileExtension( moduleInfo.sourceDirs, "CS" ) )
        {
            return "C#";
        }
        else
        if ( ( ContainsFileWithFileExtension( moduleInfo.sourceDirs, "CPP" ) ) ||
             ( ContainsFileWithFileExtension( moduleInfo.sourceDirs, "CXX" ) ) ||
             ( ContainsFileWithFileExtension( moduleInfo.sourceDirs, "CHH" ) ) )
        {
            return "C++";
        }
        else
        {
            return "C";
        }
    }
    else
    {
        // Premake supports only 1 language per module so we list the first one
        return (*languageSet.begin()).Uppercase();
    }
}

/*-------------------------------------------------------------------------*/

TStringSet
ResolveMultiPlatformName( const CORE::CString& platformName          ,
                          const TPlatformDefinitionMap* platformDefs )
{GUCEF_TRACE;

    TStringSet resultSet;

    // Save some effort for "All Platforms"...
    if ( platformName == AllPlatforms )
    {
        resultSet.insert( platformName );
        return resultSet;
    }
        
    TStringVector elements = platformName.Lowercase().ParseElements( ';', false );
    TStringVector::iterator i = elements.begin();
    while ( i != elements.end() )
    {        
        const CORE::CString& element = (*i);
        
        if ( GUCEF_NULL != platformDefs && !platformDefs->empty() )
        {
            TPlatformDefinitionMap::const_iterator n = platformDefs->begin();
            while ( n != platformDefs->end() )
            {
                // Check to see if this is a regular platform name in its own right
                if ( (*n).first == element )
                {
                    resultSet.insert( element );
                    ++n;
                    continue;
                }
                
                // Check to see if this is an alias for the current platform
                const TPlatformDefinition& platformDef = (*n).second;
                const TStringSet& aliases = platformDef.aliases;
                TStringSet::const_iterator m = aliases.begin();
                while ( m != aliases.end() )
                {
                    if ( (*m) == element )
                    {
                        // Add the regular platform name as a result of matching the alias
                        resultSet.insert( (*n).first );
                        break;
                    }
                    ++m;
                }
                ++n;
            }
        }
        else
        {
            // Use the legacy hardcoded aliases

            if ( (*i).Equals( "mswin", false ) )
            {
                resultSet.insert( "win32" );
                resultSet.insert( "win64" );
            }
            else
            if ( (*i).Equals( "linux", false ) )
            {
                resultSet.insert( "linux32" );
                resultSet.insert( "linux64" );
            }
            else
            {
                resultSet.insert( (*i).Lowercase() );
            }
        }
        ++i;
    }

    if ( resultSet.empty() )
        resultSet.insert( platformName );
    return resultSet;
}

/*-------------------------------------------------------------------------*/

// Serialize a specific moduleInfo which is derived off of
// the given entry. The given moduleInfo can be an independent platform
// moduleInfo or a merged moduleInfo
bool
SerializeModuleInfo( const TModuleInfoEntry& moduleEntry ,
                     const TModuleInfo& moduleInfo       ,
                     const CORE::CString& platform       ,
                     CORE::CDataNode& parentNode         )
{GUCEF_TRACE;

    // Add basic module info
    CORE::CDataNode moduleInfoNode;
    moduleInfoNode.SetName( "Module" );
    
    if ( !moduleInfo.name.IsNULLOrEmpty() )
    {
        moduleInfoNode.SetAttribute( "Name", moduleInfo.name );
    }
    if ( moduleInfo.buildOrder != -1 )
    {
        moduleInfoNode.SetAttribute( "BuildOrder", CORE::Int32ToString( moduleInfo.buildOrder ) );
    }
    if ( moduleInfo.buildChain != -1 )
    {
        moduleInfoNode.SetAttribute( "BuildChain", CORE::Int32ToString( moduleInfo.buildChain ) );
    }
    if ( !moduleInfo.buildChainDependencies.empty() )
    {
        TInt32Set::iterator n = moduleInfo.buildChainDependencies.begin();
        CORE::CString buildChainDepsValue;
        for ( UInt32 i=0; i+1 < moduleInfo.buildChainDependencies.size(); ++i )
        {                     
            buildChainDepsValue += (*n) + ';';
            ++n;
        }
        buildChainDepsValue += (*n);
        moduleInfoNode.SetAttribute( "BuildChainDeps", buildChainDepsValue );
    }
    if ( moduleInfo.moduleType != MODULETYPE_UNDEFINED )
    {
        moduleInfoNode.SetAttribute( "Type", ModuleTypeToString( moduleInfo.moduleType ) );
    }
    if ( !moduleInfo.tags.empty() )
    {
        UInt32 i=0;
        CORE::CString tagValue;
        TStringSet::iterator s = moduleInfo.tags.begin();
        while ( s != moduleInfo.tags.end() )
        {                     
            tagValue += (*s) + ';';
            ++s;
        }
        if ( !tagValue.IsNULLOrEmpty() )
            tagValue = tagValue.CutChars( 1, false, 0 );
        moduleInfoNode.SetAttribute( "Tags", tagValue );
    }

    moduleInfoNode.SetAttribute( "Platform", platform );

    // Add headers
    if ( moduleInfo.includeDirs.size() > 0 )
    {
        CORE::CDataNode headersInfoNode;
        headersInfoNode.SetName( "Files" );
        headersInfoNode.SetAttribute( "Type", "Headers" );
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

            // Don't add dirs that have no files in them
            // These should not be present in our data in the first place. But just in case,...
            if ( fileVector.size() > 0 )
            {
                headersInfoNode.AddChild( pathNode );
            }
            ++n;
        }

        moduleInfoNode.AddChild( headersInfoNode );
        headersInfoNode.DelSubTree();
    }

    // Add sources
    if ( moduleInfo.sourceDirs.size() > 0 )
    {
        CORE::CDataNode sourceInfoNode;
        sourceInfoNode.SetName( "Files" );
        sourceInfoNode.SetAttribute( "Type", "Source" );
        sourceInfoNode.SetAttribute( "DirCount", CORE::UInt32ToString( moduleInfo.sourceDirs.size() ) );
        TStringVectorMap::const_iterator n = moduleInfo.sourceDirs.begin();
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

            // Don't add dirs that have no files in them
            // These should not be present in our data in the first place. But just in case,...
            if ( fileVector.size() > 0 )
            {
                sourceInfoNode.AddChild( pathNode );
            }
            ++n;
        }

        moduleInfoNode.AddChild( sourceInfoNode );
        sourceInfoNode.DelSubTree();
    }

    // Add include paths inherited from dependencies
    if ( moduleInfo.dependencyIncludeDirs.size() > 0 )
    {
        CORE::CDataNode includesInfoNode( "Includes" );
        includesInfoNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.dependencyIncludeDirs.size() ) );
        includesInfoNode.SetAttribute( "Source", "Dependency" );
        TStringSet::const_iterator q = moduleInfo.dependencyIncludeDirs.begin();
        while ( q != moduleInfo.dependencyIncludeDirs.end() )
        {
            CORE::CDataNode includeNode;
            includeNode.SetName( "Include" );
            includeNode.SetAttribute( "Path", (*q) );

            includesInfoNode.AddChild( includeNode );
            ++q;
        }

        moduleInfoNode.AddChild( includesInfoNode );
        includesInfoNode.DelSubTree();
    }

    // Add all the regular include dirs for this module
    // These are already represented in the path attribute of the files section
    // but for ease of processing and clarity they are provided again in the includes section
    if ( moduleInfo.includeDirs.size() > 0 )
    {
       CORE::CDataNode includesInfoNode( "Includes" );
        includesInfoNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.includeDirs.size() ) );
        includesInfoNode.SetAttribute( "Source", "Self" );
        TStringVectorMap::const_iterator n = moduleInfo.includeDirs.begin();
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
                    CORE::CString includeDir = "../" + CORE::LastSubDir( moduleEntry.rootDir ) + " ";
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
    }

    // Add all the module dependencies
    if ( moduleInfo.dependencies.size() > 0 )
    {
        CORE::CDataNode dependenciesNode;
        dependenciesNode.SetName( "Dependencies" );
        dependenciesNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.dependencies.size() ) );
        TStringSet::const_iterator m = moduleInfo.dependencies.begin();
        while ( m != moduleInfo.dependencies.end() )
        {
            CORE::CDataNode dependencyNode;
            dependencyNode.SetName( "Dependency" );
            dependencyNode.SetAttribute( "Name", (*m) );
            dependenciesNode.AddChild( dependencyNode );
            ++m;
        }
        moduleInfoNode.AddChild( dependenciesNode );
    }

    // Add all the runtime module dependencies
    if ( moduleInfo.runtimeDependencies.size() > 0 )
    {
        CORE::CDataNode dependenciesNode;
        dependenciesNode.SetName( "RuntimeDependencies" );
        dependenciesNode.SetAttribute( "Count", CORE::UInt32ToString( moduleInfo.runtimeDependencies.size() ) );
        TStringSet::const_iterator m = moduleInfo.runtimeDependencies.begin();
        while ( m != moduleInfo.runtimeDependencies.end() )
        {
            CORE::CDataNode dependencyNode;
            dependencyNode.SetName( "RuntimeDependency" );
            dependencyNode.SetAttribute( "Name", (*m) );
            dependenciesNode.AddChild( dependencyNode );
            ++m;
        }
        moduleInfoNode.AddChild( dependenciesNode );
    }

    // Add all the module preprocessor instructions
    if ( moduleInfo.preprocessorSettings.defines.size() > 0 )
    {
        CORE::CDataNode preprocessorNode;
        preprocessorNode.SetName( "Preprocessor" );
        TStringSet::const_iterator m = moduleInfo.preprocessorSettings.defines.begin();
        while ( m != moduleInfo.preprocessorSettings.defines.end() )
        {
            CORE::CDataNode defineNode;
            defineNode.SetName( "Define" );
            defineNode.SetAttribute( "String", (*m) );
            preprocessorNode.AddChild( defineNode );
            ++m;
        }
        moduleInfoNode.AddChild( preprocessorNode );
    }

    CORE::CDataNode linkerNode;
    linkerNode.SetName( "Linker" );
    bool addedLinkedSettings = false;

    // Now Serialize all linker related info
    // First add all the libraries that are linked but not part of the overall project
    if ( moduleInfo.linkerSettings.linkedLibraries.size() > 0 )
    {
        addedLinkedSettings = true;
        TLinkedLibrarySettingsMap::const_iterator m = moduleInfo.linkerSettings.linkedLibraries.begin();
        while ( m != moduleInfo.linkerSettings.linkedLibraries.end() )
        {
            CORE::CDataNode libraryNode;
            libraryNode.SetName( "Dependency" );
            libraryNode.SetAttribute( "Name", (*m).first );

            TModuleType linkedLibType = (*m).second.moduleType;
            if ( ( MODULETYPE_UNDEFINED == linkedLibType ) ||
                 ( MODULETYPE_UNKNOWN == linkedLibType )    )
            {
                libraryNode.SetAttribute( "Type", ModuleTypeToString( linkedLibType ) );
            }
            if ( !(*m).second.libPath.IsNULLOrEmpty() )
            {
                libraryNode.SetAttribute( "Path", (*m).second.libPath );
            }
            linkerNode.AddChild( libraryNode );
            ++m;
        }

    }
    if ( !moduleInfo.linkerSettings.targetName.IsNULLOrEmpty() )
    {
        addedLinkedSettings = true;
        linkerNode.SetAttribute( "TargetName", moduleInfo.linkerSettings.targetName ); 
    }

    if ( !moduleInfo.linkerSettings.libPaths.empty() )
    {
        CORE::CString libPaths;
        TStringSet::const_iterator r = moduleInfo.linkerSettings.libPaths.begin();
        while ( r != moduleInfo.linkerSettings.libPaths.end() )
        {
            if ( libPaths.IsNULLOrEmpty() )
            {
                libPaths = (*r);
            }
            else
            {
                libPaths += ';' + (*r);
            }
            ++r;
        }
        linkerNode.SetAttribute( "LibPaths", libPaths );
        addedLinkedSettings = true;
    }

    if ( addedLinkedSettings )
    {
        moduleInfoNode.AddChild( linkerNode );
    }

    // Add all the info for this module to the overall project
    parentNode.AddChild( moduleInfoNode );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
SerializeModuleInfo( const TModuleInfoEntry& moduleEntry ,
                     CORE::CDataNode& parentNode         )
{GUCEF_TRACE;

    CORE::CDataNode moduleEntryNode( "ModuleInfoEntry" );
    moduleEntryNode.SetAttribute( "RootDir", moduleEntry.rootDir );

    if ( !moduleEntry.lastEditBy.IsNULLOrEmpty() )
    {
        moduleEntryNode.SetAttribute( "LastEditBy", moduleEntry.lastEditBy );
    }

    TModuleInfoMap::const_iterator i = moduleEntry.modulesPerPlatform.begin();
    while ( i != moduleEntry.modulesPerPlatform.end() )
    {
        const CORE::CString& platform = (*i).first;
        const TModuleInfo& moduleInfo = (*i).second;

        if ( !SerializeModuleInfo( moduleEntry     ,
                                   moduleInfo      ,
                                   platform        ,
                                   moduleEntryNode ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "SerializeModuleInfo: Failed to serialize moduleInfo of module " + moduleInfo.name + " for platform " + platform );
            return false;
        }

        ++i;
    }
    parentNode.AddChild( moduleEntryNode );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
SerializeModuleInfo( const TModuleInfoEntry& moduleEntry ,
                     const CORE::CString& outputFilepath )
{GUCEF_TRACE;

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec = GetXmlDStoreCodec();
    if ( 0 != codec )
    {
        CORE::CDataNode info;
        if ( SerializeModuleInfo( moduleEntry, info ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SerializeModuleInfo: Successfully generated a data tree with all module information" );

            if ( codec->StoreDataTree( &info, outputFilepath ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SerializeModuleInfo: Successfully wrote all module information to disk file \"" + outputFilepath + "\"" );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "SerializeModuleInfo: Failed to store the serialized module information to disk at " + outputFilepath );
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "SerializeModuleInfo: Failed to serialize the given module information" );
            return false;
        }
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "SerializeModuleInfo: Cannot serialize since no codec is registered that can be used for serialization" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
SerializeProjectInfo( const TProjectInfo& projectInfo ,
                      CORE::CDataNode& rootNodeToBe   )
{GUCEF_TRACE;

    // start from a clean slate
    rootNodeToBe.Clear();

    // Add project info
    rootNodeToBe.SetName( "Project" );
    rootNodeToBe.SetAttribute( "ModuleCount", CORE::UInt32ToString( projectInfo.modules.size() ) );
    rootNodeToBe.SetAttribute( "Name", projectInfo.projectName );

    // Add info for each module
    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        if ( !SerializeModuleInfo( (*i)         ,
                                   rootNodeToBe ) )
        {
            // Failed to serialize this module
            return false;
        }
        ++i;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
DeserializeProjectInfo( TProjectInfo& projectInfo       ,
                        const CORE::CDataNode& rootNode )
{GUCEF_TRACE;

    const CORE::CDataNode* node = rootNode.Search( "Project", '\\', true );
    if ( NULL == node )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeProjectInfo: Failed locate project root data node" );
        return false;
    }

    int errorCount = 0;
    projectInfo.projectName = node->GetAttributeValueOrChildValueByName( "Name" );
    CORE::CDataNode::TConstDataNodeSet nodeSet = node->FindChildrenOfType( "ModuleInfoEntry" );
    CORE::CDataNode::TConstDataNodeSet::const_iterator i = nodeSet.begin();
    while ( i != nodeSet.end() )
    {
        TModuleInfoEntry newModuleInfo;
        InitializeModuleInfoEntry( newModuleInfo );
        projectInfo.modules.push_back( newModuleInfo );
        TModuleInfoEntry& moduleInfoEntry = projectInfo.modules.back();

        if ( !DeserializeModuleInfo( projectInfo, moduleInfoEntry, *(*i) ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeProjectInfo: Failed deserialize module info entry" );
            ++errorCount;
        }
        
        ++i;
    }

    return errorCount == 0;
}

/*-------------------------------------------------------------------------*/

bool
SerializeProjectInfo( const TProjectInfo& projectInfo     ,
                      const CORE::CString& outputFilepath )
{GUCEF_TRACE;

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec = GetXmlDStoreCodec();
    if ( 0 != codec )
    {
        CORE::CDataNode info;
        if ( SerializeProjectInfo( projectInfo, info ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully generated a data tree with all project information" );

            if ( codec->StoreDataTree( &info, outputFilepath ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully wrote all project information to disk file \"" + outputFilepath + "\"" );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "SerializeProjectInfo: Failed to store the serialized project information to disk at " + outputFilepath );
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "SerializeProjectInfo: Failed to serialize the given project information" );
            return false;
        }
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "SerializeProjectInfo: Cannot serialize since no codec is registered that can be used for serialization" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
DeserializeProjectInfo( TProjectInfo& projectInfo            ,
                        const CORE::CString& projectInfoPath )
{GUCEF_TRACE;

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec = GetXmlDStoreCodec();
    if ( 0 != codec )
    {
        CORE::CDataNode info;
        if ( codec->BuildDataTree( &info, projectInfoPath ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully generated a data tree with all project information" );

            if ( DeserializeProjectInfo( projectInfo, info ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded all project information from disk file \"" + projectInfoPath + "\"" );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeProjectInfo: Failed to load project information from deserialized info loaded from " + projectInfoPath );
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeProjectInfo: Failed to deserialize project information from disk at " + projectInfoPath );
            return false;
        }
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeProjectInfo: Cannot deserialize since no codec is registered that can be used for deserialization" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
DeserializeModuleInfo( TModuleInfo& moduleInfo           ,
                       const CORE::CDataNode& parentNode )
{GUCEF_TRACE;

    const CORE::CDataNode* moduleInfoNode = parentNode.Find( "Module" );
    if ( moduleInfoNode == NULL ) return false;

    // Find the overall module properties
    CORE::CString tmpStr = moduleInfoNode->GetAttributeValue( "BuildOrder", "-1" );
    moduleInfo.buildOrder = CORE::StringToInt32( tmpStr );
    tmpStr = moduleInfoNode->GetAttributeValue( "BuildChain", "-1" );
    moduleInfo.buildChain = CORE::StringToInt32( tmpStr );
    tmpStr = moduleInfoNode->GetAttributeValue( "BuildChainDeps" );
    if ( !tmpStr.IsNULLOrEmpty() )
    {
        TStringVector tmpStrVec = tmpStr.ParseElements( ';', false );
        if ( tmpStrVec.empty() )
        {
            for ( UInt32 i=0; i<tmpStrVec.size(); ++i )
            {
                Int32 chainId = CORE::StringToInt32( tmpStrVec[ i ] );
                moduleInfo.buildChainDependencies.insert( chainId );
            }
        }
    }
    moduleInfo.tags = StringVectorToStringSet( moduleInfoNode->GetAttributeValue( "Tags" ).ParseElements( ';', false ) );
    moduleInfo.moduleType = StringToModuleType( moduleInfoNode->GetAttributeValue( "Type" ) );
    moduleInfo.considerSubDirs = CORE::StringToBool( moduleInfoNode->GetAttributeValue( "ConsiderSubDirs", "True" ) );

    // Check to see if a name was defined
    tmpStr = moduleInfoNode->GetAttributeValue( "Name" );
    if ( !tmpStr.IsNULLOrEmpty() )
    {
        moduleInfo.name = tmpStr;
    }

    // Find any/all files for which are part of this module
    CORE::CDataNode::TConstDataNodeSet fileNodes = moduleInfoNode->FindChildrenOfType( "Files" );
    CORE::CDataNode::TConstDataNodeSet::iterator i = fileNodes.begin();
    while ( i != fileNodes.end() )
    {
        const CORE::CDataNode* filesDirsNode = (*i);
        CORE::CString filesType = filesDirsNode->GetAttributeValue( "Type" );

        CORE::CDataNode::TConstDataNodeSet dirs = filesDirsNode->FindChildrenOfType( "Dir" );
        CORE::CDataNode::TConstDataNodeSet::iterator n = dirs.begin();
        while ( n != dirs.end() )
        {
            const CORE::CDataNode* dirsNode = (*n);
            CORE::CString path = dirsNode->GetAttributeValue( "Path" );

            CORE::CDataNode::TConstDataNodeSet files = dirsNode->FindChildrenOfType( "File" );
            CORE::CDataNode::TConstDataNodeSet::iterator m = files.begin();
            while ( m != files.end() )
            {
                CORE::CString filename = (*m)->GetAttributeValue( "Name" );

                if ( filesType == "Headers" )
                {
                    // We have a list of header files
                    moduleInfo.includeDirs[ path ].push_back( filename );
                }
                else
                if ( filesType == "Source" )
                {
                    // We have a list of source files
                    moduleInfo.sourceDirs[ path ].push_back( filename );
                }
                ++m;
            }
            ++n;
        }
        ++i;
    }

    // Find dependency includes
    CORE::CDataNode::TConstDataNodeSet includesNodes = moduleInfoNode->FindChildrenOfType( "Includes" );
    i = includesNodes.begin();
    while ( i != includesNodes.end() )
    {
        const CORE::CDataNode* includesNode = (*i);
        CORE::CString source = includesNode->GetAttributeValue( "Source" ).Lowercase();
        if ( source == "dependency" )
        {
            CORE::CDataNode::TConstDataNodeSet includes = includesNode->FindChildrenOfType( "Include" );
            CORE::CDataNode::TConstDataNodeSet::iterator n = includes.begin();
            while ( n != includes.end() )
            {
                const CORE::CDataNode* includeNode = (*n);
                CORE::CString path = includeNode->GetAttributeValue( "Path" );

                moduleInfo.dependencyIncludeDirs.insert( path );
                ++n;
            }
        }
        else
        if ( source == "self" )
        {
            // If the "self" includes came from actual include files then they would be automatically
            // added when the files are added. However,... some importers might place additional includes
            // under self which would not be auto-detected via include files. As such we have to add them 
            // here as well to make sure. 
            // Importers might not have any way of knowing whether a path was added due to dependency needs

            CORE::CDataNode::TConstDataNodeSet includes = includesNode->FindChildrenOfType( "Include" );
            CORE::CDataNode::TConstDataNodeSet::iterator n = includes.begin();
            while ( n != includes.end() )
            {
                const CORE::CDataNode* includeNode = (*n);
                CORE::CString path = includeNode->GetAttributeValue( "Path" );

                if ( moduleInfo.includeDirs[ path ].empty() )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeModuleInfo: Adding include dir of source \"self\" which does not currently have a reference to include files: " + path );
                }
                ++n;
            }
        }
        ++i;
    }

    // Find all the module dependencies
    const CORE::CDataNode* dependenciesNode = moduleInfoNode->Find( "Dependencies" );
    if ( NULL != dependenciesNode )
    {
        CORE::CDataNode::TConstDataNodeSet dependencies = dependenciesNode->FindChildrenOfType( "Dependency" );
        i = dependencies.begin();
        while ( i != dependencies.end() )
        {
            const CORE::CDataNode* dependencyNode = (*i);
            CORE::CString dependencyName = dependencyNode->GetAttributeValue( "Name" );
            if ( !dependencyName.IsNULLOrEmpty() )
            {
                moduleInfo.dependencies.insert( dependencyName );
            }
            ++i;
        }
    }

    // Get all the runtime module dependencies if any
    const CORE::CDataNode* rdependenciesNode = moduleInfoNode->Find( "RuntimeDependencies" );
    if ( NULL != rdependenciesNode )
    {
        CORE::CDataNode::TConstDataNodeSet dependencies = rdependenciesNode->FindChildrenOfType( "RuntimeDependency" );
        i = dependencies.begin();
        while ( i != dependencies.end() )
        {
            const CORE::CDataNode* dependencyNode = (*i);
            CORE::CString dependencyName = dependencyNode->GetAttributeValue( "Name" );
            if ( !dependencyName.IsNULLOrEmpty() )
            {
                moduleInfo.runtimeDependencies.insert( dependencyName );
            }
            ++i;
        }
    }

    // Go through all linker related settings, if any exist
    const CORE::CDataNode* linkerNode = moduleInfoNode->Find( "Linker" );
    if ( NULL != linkerNode )
    {
        moduleInfo.linkerSettings.targetName = linkerNode->GetAttributeValueOrChildValueByName( "TargetName" );
        
        // If general paths were defined at the linker level add them to each dependency
        // They should be added after the dependency paths this could have been added above because
        // those should take priority in a linker search for paths        
        moduleInfo.linkerSettings.libPaths = StringVectorToStringSet( linkerNode->GetAttributeValueOrChildValueByName( "LibPaths" ).ParseElements( ';' ) );
        
        // Find all the libraries that are linked but not part of the overall project
        CORE::CDataNode::TConstDataNodeSet linkedLibs = linkerNode->FindChildrenOfType( "Dependency" );
        i = linkedLibs.begin();
        while ( i != linkedLibs.end() )
        {
            const CORE::CDataNode* linkedLibNode = (*i);
            CORE::CString linkedLibName = linkedLibNode->GetAttributeValue( "Name" );
            if ( !linkedLibName.IsNULLOrEmpty() )
            {
                TLinkedLibrarySettings& libSettings = moduleInfo.linkerSettings.linkedLibraries[ linkedLibName ]; 
                
                CORE::CString linkedLibType = linkedLibNode->GetAttributeValue( "Type" );
                if ( !linkedLibType.IsNULLOrEmpty() )
                {
                    libSettings.moduleType = StringToModuleType( linkedLibType );
                }
                else
                {
                    libSettings.moduleType = MODULETYPE_UNDEFINED;
                }

                libSettings.libPath = linkedLibNode->GetAttributeValue( "Path" );
            }
            ++i;
        }
    }

   // Go through all preprocessor related settings, if any exist
    const CORE::CDataNode* preprocessorNode = moduleInfoNode->Find( "Preprocessor" );
    if ( NULL != preprocessorNode )
    {
        // Find all the preprocessor definitions
        CORE::CDataNode::TConstDataNodeSet defines = preprocessorNode->FindChildrenOfType( "Define" );
        i = defines.begin();
        while ( i != defines.end() )
        {
            const CORE::CDataNode* defineNode = (*i);
            CORE::CString defineValue = defineNode->GetAttributeValue( "String" );
            if ( !defineValue.IsNULLOrEmpty() )
            {
                moduleInfo.preprocessorSettings.defines.insert( defineValue );
            }
            ++i;
        }
    }

   // Go through all preprocessor related settings, if any exist
    const CORE::CDataNode* compilerNode = moduleInfoNode->Find( "Compiler" );
    if ( NULL != compilerNode )
    {
        // Find all the code languages defined for the compiler
        CORE::CDataNode::TConstDataNodeSet languages = compilerNode->FindChildrenOfType( "Language" );
        i = languages.begin();
        while ( i != languages.end() )
        {
            const CORE::CDataNode* languageNode = (*i);
            CORE::CString name = languageNode->GetAttributeValue( "Name" );
            if ( !name.IsNULLOrEmpty() )
            {
                moduleInfo.compilerSettings.languagesUsed.insert( name );
            }
            ++i;
        }

        // Find all the compiler flags defined per compiler
        CORE::CDataNode::TConstDataNodeSet flagNodes = compilerNode->FindChildrenOfType( "CompilerFlags" );
        i = flagNodes.begin();
        while ( i != flagNodes.end() )
        {
            const CORE::CDataNode* flagsNode = (*i);
            CORE::CString compilerName = flagsNode->GetAttributeValue( "CompilerName" );
            CORE::CString flags = flagsNode->GetAttributeValue( "Flags" );
            if ( !compilerName.IsNULLOrEmpty() && !flags.IsNULLOrEmpty() )
            {
                moduleInfo.compilerSettings.compilerFlags[ compilerName ] = flags;
            }
            ++i;
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
CleanupIncludeDirs( TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    TModuleInfoMap::iterator i = moduleInfoEntry.modulesPerPlatform.begin();
    while ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        TModuleInfo& moduleInfo = (*i).second;
        
        // Check for empty include dirs
        // If the include dir does not have include files as part of this module then 
        // the dir should have been a dependency include dir
        TStringSet dirs;
        TStringVectorMap::iterator n = moduleInfo.includeDirs.begin();
        while ( n != moduleInfo.includeDirs.end() )
        {
            TStringVector& filesInDirList = (*n).second;
            if ( filesInDirList.empty() )
            {
                dirs.insert( (*n).first );
            }
            ++n;
        }

        TStringSet::iterator m = dirs.begin();
        while ( m != dirs.end() )
        {
            moduleInfo.includeDirs.erase( (*m) ); 
            moduleInfo.dependencyIncludeDirs.insert( (*m) );

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Moved dir from module header dir to dependency include dir as it had no files: " + (*m) );

            ++m;
        }

        // Check for duplicates,.. dirs that are listed as 
        // dependency includes but which also have headers part of this module
        // as such its not a dir coming from a dependency
        n = moduleInfo.includeDirs.begin();
        while ( n != moduleInfo.includeDirs.end() )
        {
            TStringSet::iterator p = moduleInfo.dependencyIncludeDirs.find( (*n).first );
            if ( p != moduleInfo.dependencyIncludeDirs.end() )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Deleting dependency include dir since it already exists as a module header dir: " + (*p) );                
                moduleInfo.dependencyIncludeDirs.erase( p );
            }            
            ++n;
        }
                
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
SetModuleInfo( TModuleInfoEntry& moduleInfoEntry ,
               TModuleInfo& moduleInfo           ,
               const CORE::CString& platform     )
{GUCEF_TRACE;

    // First check if we already have a entry for this platform
    TModuleInfoMap::iterator i = moduleInfoEntry.modulesPerPlatform.find( platform );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        // Since we already have an entry for this platform we will merge the two
        MergeModuleInfo( (*i).second, moduleInfo );
    }
    else
    {
        moduleInfoEntry.modulesPerPlatform[ platform ] = moduleInfo;
    }
}

/*-------------------------------------------------------------------------*/

bool
DeserializeModuleInfo( const TProjectInfo& projectInfo   ,
                       TModuleInfoEntry& moduleInfoEntry ,
                       const CORE::CDataNode& parentNode )
{GUCEF_TRACE;

    const CORE::CDataNode* moduleEntryNode = parentNode.Find( "ModuleInfoEntry" );
    if ( NULL == moduleEntryNode ) return false;

    if ( moduleInfoEntry.rootDir.IsNULLOrEmpty() )
    {
        moduleInfoEntry.rootDir = moduleEntryNode->GetAttributeValue( "RootDir" );
    }

    const CORE::CDataNode::TConstDataNodeSet moduleInfoNodes = moduleEntryNode->FindChildrenOfType( "Module" );
    if ( moduleInfoNodes.size() == 0 ) return false;

    CORE::CDataNode::TConstDataNodeSet::const_iterator n = moduleInfoNodes.begin();
    while ( n != moduleInfoNodes.end() )
    {
        TModuleInfo moduleInfoForPlatform;
        InitializeModuleInfo( moduleInfoForPlatform );

        const CORE::CDataNode* moduleNode = (*n);

        // Get all platforms for which this info applies.
        // Keep in mind that multiple platforms can be specified for ease of use.
        // This feature requires platform entries to be seperated by a ';'
        TStringVector platforms = moduleNode->GetAttributeValue( "Platform" ).Lowercase().ParseElements( ';', false);

        if ( platforms.empty() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to locate a Platform value for a module, will default to all platforms but this may not be correct" );
            platforms.push_back( AllPlatforms );
        }

        if ( DeserializeModuleInfo( moduleInfoForPlatform ,
                                    *moduleNode           ) )
        {
            if ( moduleInfoForPlatform.name.IsNULLOrEmpty() )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully deserialized module definition for module with no name specified for the applicable platforms" );
            }
            else
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully deserialized module definition for module with name " + moduleInfoForPlatform.name );
            }

            TStringVector::iterator i = platforms.begin();
            while ( i != platforms.end() )
            {
                TStringSet actualPlatforms = ResolveMultiPlatformName( (*i), &projectInfo.platforms );
                TStringSet::iterator n = actualPlatforms.begin();
                while ( n != actualPlatforms.end() )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Adding module definition for platform " + (*n) );
                    SetModuleInfo( moduleInfoEntry, moduleInfoForPlatform, (*n) );
                    ++n;
                }                

                ++i;
            }
        }
        else
        {
            return false;
        }
        ++n;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
DeserializeModuleInfo( const TProjectInfo& projectInfo           ,
                       TModuleInfoEntryVector& moduleInfoEntries ,
                       const CORE::CString& inputFilepath        )
{GUCEF_TRACE;

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec = GetXmlDStoreCodec();
    if ( 0 != codec )
    {
        CORE::CDataNode rootNode;
        if ( codec->BuildDataTree( &rootNode, inputFilepath ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeModuleInfo: Successfully loaded module information from file \"" + inputFilepath + "\", now we will parse the information" );

            CORE::CDataNode::TDataNodeSet moduleEntryNodes = rootNode.FindNodesOfType( "ModuleInfoEntry", true );
            if ( moduleEntryNodes.empty() )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeModuleInfo: No ModuleInfoEntry nodes were obtained from file \"" + inputFilepath + "\"" );
                return false;
            }

            UInt32 n=0; UInt32 errorCount = 0;
            CORE::CDataNode::TDataNodeSet::iterator i = moduleEntryNodes.begin();
            while ( i != moduleEntryNodes.end() )
            {
                TModuleInfoEntry entry;
                InitializeModuleInfoEntry( entry );
                
                if ( !DeserializeModuleInfo( projectInfo, entry, *(*i) ) )
                    ++errorCount;
                
                moduleInfoEntries.push_back( entry );
                ++i;
            }

            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeModuleInfo: Successfully deserialized " + CORE::UInt32ToString( moduleEntryNodes.size()-errorCount ) + "/" + CORE::UInt32ToString( moduleEntryNodes.size() ) + " entries from file \"" + inputFilepath + "\"" );
            return 0 == errorCount;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeModuleInfo: Failed to load the module information from file at " + inputFilepath );
            return false;
        }
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeModuleInfo: Cannot deserialize since no codec is registered that can be used for deserialization" );
    return false;
}

/*-------------------------------------------------------------------------*/

void
InitializeModuleInfo( TModuleInfo& moduleInfo )
{GUCEF_TRACE;

    // reset all fields
    moduleInfo.buildOrder = -1;
    moduleInfo.buildChain = -1;
    moduleInfo.buildChainDependencies.clear();
    moduleInfo.name.Clear();
    moduleInfo.tags.clear();
    moduleInfo.includeDirs.clear();
    moduleInfo.sourceDirs.clear();
    moduleInfo.dependencyIncludeDirs.clear();
    moduleInfo.linkerSettings.linkedLibraries.clear();
    moduleInfo.compilerSettings.languagesUsed.clear();
    moduleInfo.preprocessorSettings.defines.clear();
    moduleInfo.dependencies.clear();
    moduleInfo.runtimeDependencies.clear();
    moduleInfo.considerSubDirs = true;
    moduleInfo.ignoreModule = false;
    moduleInfo.moduleType = MODULETYPE_UNDEFINED;
}

/*-------------------------------------------------------------------------*/

void
InitializeModuleInfoEntry( TModuleInfoEntry& moduleEntry )
{GUCEF_TRACE;

    moduleEntry.modulesPerPlatform.clear();
    moduleEntry.rootDir.Clear();
}

/*-------------------------------------------------------------------------*/

void
MergeModuleTypeMap( TModuleTypeMap& baseMap           ,
                    const TModuleTypeMap& incomingMap )
{GUCEF_TRACE;

    TModuleTypeMap::const_iterator i = incomingMap.begin();
    while ( i != incomingMap.end() )
    {
        TModuleTypeMap::iterator n = baseMap.find( (*i).first );
        if ( n != baseMap.end() )
        {
            TModuleType moduleType = (*i).second;
            if ( ( MODULETYPE_UNDEFINED != moduleType ) &&
                 ( MODULETYPE_UNKNOWN != moduleType )    )
            {
                baseMap[ (*i).first ] = moduleType;
            }
        }
        else
        {
            baseMap[ (*i).first ] = (*i).second;
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
MergeLinkedLibrarySettingsMap( TLinkedLibrarySettingsMap& baseMap           ,
                               const TLinkedLibrarySettingsMap& incomingMap )
{GUCEF_TRACE;

    TLinkedLibrarySettingsMap::const_iterator i = incomingMap.begin();
    while ( i != incomingMap.end() )
    {
        TLinkedLibrarySettingsMap::iterator n = baseMap.find( (*i).first );
        if ( n != baseMap.end() )
        {
            TModuleType moduleType = (*i).second.moduleType;
            if ( ( MODULETYPE_UNDEFINED != moduleType ) &&
                 ( MODULETYPE_UNKNOWN != moduleType )    )
            {
                baseMap[ (*i).first ].moduleType = moduleType;
            }
        }
        else
        {
            baseMap[ (*i).first ] = (*i).second;
        }
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

void
MergeModuleInfo( TModuleInfo& targetModuleInfo          ,
                 const TModuleInfo& moduleInfoToMergeIn )
{GUCEF_TRACE;

    // First take care of items which we know overwrite
    // We overwrite when the 'moduleInfoToMergeIn' has these attributes set to a
    // value other then the initialization value
    if ( moduleInfoToMergeIn.buildOrder > -1 )
    {
        targetModuleInfo.buildOrder = moduleInfoToMergeIn.buildOrder;
    }
    if ( moduleInfoToMergeIn.buildChain > -1 )
    {
        targetModuleInfo.buildChain = moduleInfoToMergeIn.buildChain;
    }
    TInt32Set::iterator i = moduleInfoToMergeIn.buildChainDependencies.begin();
    while ( i != moduleInfoToMergeIn.buildChainDependencies.end() )
    {
        targetModuleInfo.buildChainDependencies.insert( (*i) );
    }
    if ( !moduleInfoToMergeIn.name.IsNULLOrEmpty() )
    {
        targetModuleInfo.name = moduleInfoToMergeIn.name;
    }
    if ( moduleInfoToMergeIn.moduleType != MODULETYPE_UNDEFINED )
    {
        targetModuleInfo.moduleType = moduleInfoToMergeIn.moduleType;
    }
    if ( !moduleInfoToMergeIn.linkerSettings.targetName.IsNULLOrEmpty() )
    {
        targetModuleInfo.linkerSettings.targetName = moduleInfoToMergeIn.linkerSettings.targetName; 
    }
    if ( moduleInfoToMergeIn.ignoreModule )
    {
        targetModuleInfo.ignoreModule = moduleInfoToMergeIn.ignoreModule;
    }

    // Now combine the other items without overwriting
    MergeStringSet( targetModuleInfo.tags    ,
                    moduleInfoToMergeIn.tags ,
                    true                     );
    MergeStringSet( targetModuleInfo.compilerSettings.languagesUsed    ,
                    moduleInfoToMergeIn.compilerSettings.languagesUsed ,
                    false                                              );
    MergeStringMap( targetModuleInfo.compilerSettings.compilerFlags    ,
                    moduleInfoToMergeIn.compilerSettings.compilerFlags ,
                    false                                              ,
                    true                                               );
    MergeStringSet( targetModuleInfo.preprocessorSettings.defines    ,
                    moduleInfoToMergeIn.preprocessorSettings.defines ,
                    true                                             );
    MergeLinkedLibrarySettingsMap( targetModuleInfo.linkerSettings.linkedLibraries    ,
                                   moduleInfoToMergeIn.linkerSettings.linkedLibraries );
    MergeStringSet( targetModuleInfo.dependencies    ,
                    moduleInfoToMergeIn.dependencies ,
                    false                            );
    MergeStringSet( targetModuleInfo.runtimeDependencies    ,
                    moduleInfoToMergeIn.runtimeDependencies ,
                    true                                    );
    MergeStringSet( targetModuleInfo.dependencyIncludeDirs    ,
                    moduleInfoToMergeIn.dependencyIncludeDirs ,
                    true                                      );
    MergeStringVectorMap( targetModuleInfo.includeDirs    ,
                          moduleInfoToMergeIn.includeDirs ,
                          true                            );
    MergeStringVectorMap( targetModuleInfo.sourceDirs    ,
                          moduleInfoToMergeIn.sourceDirs ,
                          true                           );
}

/*-------------------------------------------------------------------------*/

TModuleInfo*
FindModuleInfoForPlatform( TModuleInfoEntry& moduleInfoEntry ,
                           const CORE::CString& platform     ,
                           bool createNewIfNoneExists        )
{GUCEF_TRACE;

    TModuleInfoMap::iterator i = moduleInfoEntry.modulesPerPlatform.find( platform.Lowercase() );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        return &(*i).second;
    }
    if ( createNewIfNoneExists )
    {
        TModuleInfo moduleInfo;
        InitializeModuleInfo( moduleInfo );

        moduleInfoEntry.modulesPerPlatform.insert( std::pair< CORE::CString, TModuleInfo >( platform, moduleInfo ) );
        return &moduleInfoEntry.modulesPerPlatform[ platform ];
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

const TModuleInfo*
FindModuleInfoForPlatform( const TModuleInfoEntry& moduleInfoEntry ,
                           const CORE::CString& platform           )
{GUCEF_TRACE;

    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.find( platform.Lowercase() );
    if ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        return &(*i).second;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

bool
MergeModuleInfo( const TModuleInfoEntry& moduleInfoEntry ,
                 const CORE::CString& targetPlatform     ,
                 TModuleInfo& mergedModuleInfo           )
{GUCEF_TRACE;

    InitializeModuleInfo( mergedModuleInfo );

    const TModuleInfo* allPlatformsInfo = FindModuleInfoForPlatform( moduleInfoEntry, AllPlatforms );
    const TModuleInfo* targetPlatformInfo = FindModuleInfoForPlatform( moduleInfoEntry, targetPlatform );
    if ( ( NULL != allPlatformsInfo ) || ( NULL != targetPlatformInfo ) )
    {
        // Check if we have both
        if ( NULL != allPlatformsInfo && NULL != targetPlatformInfo )
        {
            // Check if at least one of them has a build order set
            if ( allPlatformsInfo->buildOrder > -1 || targetPlatformInfo->buildOrder > -1 )
            {
                // Check if at least one of them has a module type set
                if ( allPlatformsInfo->moduleType != MODULETYPE_UNDEFINED || targetPlatformInfo->moduleType != MODULETYPE_UNDEFINED )
                {
                    // Use the 'all' plaform as a base to work from
                    mergedModuleInfo = *allPlatformsInfo;

                    // Now merge in the platform specific info
                    MergeModuleInfo( mergedModuleInfo, *targetPlatformInfo );

                    return true;
                }
            }

            // Even though some info is specified for both platforms neither counts as independent
            // definition for which a build order must be set
            return false;
        }
        else
        if ( NULL != allPlatformsInfo && allPlatformsInfo->buildOrder > -1 )
        {
            // We only have the 'all' platform which is fine, we will just use that
            mergedModuleInfo = *allPlatformsInfo;
            return true;
        }
        else
        if ( NULL != targetPlatformInfo && targetPlatformInfo->buildOrder > -1 )
        {
            // We only have the target platform which is fine, we will just use that
            // this module aparently is not available for all platforms even in altered form
            mergedModuleInfo = *targetPlatformInfo;
            return true;
        }
    }

    // This module should not be used since it doesnt have platform specific info
    // nor info which applies to all platforms.
    return false;
}

/*-------------------------------------------------------------------------*/

bool
MergeAllModuleInfoForPlatform( const TModuleInfoEntryVector& allInfo  ,
                               const CORE::CString& platform          ,
                               TModuleInfoVector& allMergedInfo       ,
                               TModuleInfoEntryPairVector& mergeLinks )
{GUCEF_TRACE;

    typedef std::vector< const TModuleInfoEntry* > TModuleInfoEntryPtrVector;

    allMergedInfo.clear();

    TModuleInfoEntryPtrVector indexMap;
    TModuleInfoEntryVector::const_iterator i = allInfo.begin();
    while ( i != allInfo.end() )
    {
        // For each module we create a merged platform specific module
        // description which is easy to process if you only care about that platform
        TModuleInfo mergedInfo;
        if ( MergeModuleInfo( (*i), platform, mergedInfo ) )
        {
            // Store the merged info
            allMergedInfo.push_back( mergedInfo );

            // Store a link between the merged info and the original info
            // at the same index as the merged info
            indexMap.push_back( &(*i) );
        }
        ++i;
    }

    // Now that alterations to the storage are completed we can map the index
    // of each entry to the actual data storage
    for ( CORE::UInt32 m=0; m<allMergedInfo.size(); ++m )
    {
        TModuleInfoEntryPair mergeLink( indexMap[ m ], &(allMergedInfo[ m ]) );
        mergeLinks.push_back( mergeLink );
    }
    return true;
}

/*---------------------------------------------------------------------------*/

const CORE::CString*
GetModuleName( const TModuleInfoEntry& moduleInfoEntry ,
               const CORE::CString& targetPlatform     ,
               const TModuleInfo** moduleInfo          )
{GUCEF_TRACE;

    TModuleInfoMap::const_iterator n = moduleInfoEntry.modulesPerPlatform.find( targetPlatform );
    if ( n != moduleInfoEntry.modulesPerPlatform.end() )
    {
        // A module was specified for this platform
        // Just because we have a module definition doenst mean we have a name
        if ( !(*n).second.name.IsNULLOrEmpty() )
        {
            // We have a name for this specific plaform
            if ( NULL != moduleInfo )
            {
                *moduleInfo = &(*n).second;
            }
            return &( (*n).second.name );
        }
    }

    // If no name is specified for a specific platform then there might still be a
    // default for all platforms
    if ( targetPlatform != AllPlatforms )
    {
        n = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
        if ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            // An 'AllPlatforms' definition is available for this module
            // Just because we have a module definition doenst mean we have a name
            if ( !(*n).second.name.IsNULLOrEmpty() )
            {
                // We have a name for this specific plaform
                if ( NULL != moduleInfo )
                {
                    *moduleInfo = &(*n).second;
                }
                return &( (*n).second.name );
            }
        }
    }
    return NULL;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetConsensusModuleName( const TModuleInfoEntry& moduleInfoEntry ,
                        const TModuleInfo** moduleInfo          )
{GUCEF_TRACE;

    if ( NULL != moduleInfo )
        *moduleInfo = NULL;

    TModuleInfoMap::const_iterator n = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( n != moduleInfoEntry.modulesPerPlatform.end() )
    {
        // A name was specified for all platforms which makes our job easy
        // an all platform name always counts as the general consensus name
        if ( !(*n).second.name.IsNULLOrEmpty() )
        {
            if ( NULL != moduleInfo )
                *moduleInfo = &(*n).second;
            return (*n).second.name;
        }
    }

    // If no name is specified for all platforms then we will have to
    // determine the best name to use. We do this by getting the name
    // for all platforms and counting how often each is used. The most used
    // name is considered the general consensus name. If the same count applies
    // to multiple we will try to use a popular platform to improve our 'guess'

    typedef std::map< CORE::CString, CORE::UInt32 > TStringCountMap;

    TStringCountMap countMap;
    n = moduleInfoEntry.modulesPerPlatform.begin();
    while ( n != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const TModuleInfo& mInfo = (*n).second;
        if ( !mInfo.name.IsNULLOrEmpty() )
        {
            TStringCountMap::iterator m = countMap.find( mInfo.name );
            if ( m != countMap.end() )
            {
                ++((*m).second);
            }
            else
            {
                countMap[ mInfo.name ] = 1;
            }
        }
        ++n;
    }

    if ( countMap.empty() )
    {
        // This should not happen:
        // Given that we have no module definitions for ANY platform and yet have a higher level concept of it...
        // We promised we would always return a module name. The only thing we have at this point is the directory path
        // As such we will assume the dir if named after the module it houses as our last ditch guess
        return CORE::LastSubDir( moduleInfoEntry.rootDir );
    }

    // Now that we have the popularity count of each name get the highest count
    CORE::UInt32 highestCount = 0;
    TStringCountMap::iterator i = countMap.begin();
    while ( i != countMap.end() )
    {
        if ( highestCount < (*i).second )
        {
            highestCount = (*i).second;
        }
        ++i;
    }

    // Make the list of most popular names
    TStringSet topNames;
    i = countMap.begin();
    while ( i != countMap.end() )
    {
        if ( highestCount == (*i).second )
        {
            topNames.insert( (*i).first );
        }
        ++i;
    }

    // If we have multiple use a popular platform if
    // possible, otherwise just grab one

    //@TODO: popular platform check

    CString consensusName = (*topNames.begin());
    if ( NULL != moduleInfo )
    {
        // Now turn the consensus name back into a module pointer
        n = moduleInfoEntry.modulesPerPlatform.begin();
        while ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            if ( (*n).second.name == consensusName )
            {
                *moduleInfo = &(*n).second;
                break;
            }
            ++n;
        }
    }
    return consensusName;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetModuleNameAlways( const TModuleInfoEntry& moduleInfoEntry ,
                     const CORE::CString& targetPlatform     ,
                     const TModuleInfo** moduleInfo          )
{GUCEF_TRACE;

    const CORE::CString* strPtr = GetModuleName( moduleInfoEntry, targetPlatform, moduleInfo );
    if ( GUCEF_NULL == strPtr )
    {
        return GetConsensusModuleName( moduleInfoEntry, moduleInfo );
    }
    return *strPtr;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetModuleTargetName( const TModuleInfoEntry& moduleInfoEntry ,
                     const CORE::CString& targetPlatform     ,
                     bool useModuleNameIfNoTargetName        )
{GUCEF_TRACE;

    const TModuleInfo* moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, targetPlatform );
    if ( NULL != moduleInfo )
    {
        if ( !moduleInfo->linkerSettings.targetName.IsNULLOrEmpty() )
        {
            return moduleInfo->linkerSettings.targetName;
        }
    }
    if ( targetPlatform != AllPlatforms && !targetPlatform.IsNULLOrEmpty() )
    {
        moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, AllPlatforms );
        if ( !moduleInfo->linkerSettings.targetName.IsNULLOrEmpty() )
        {
            return moduleInfo->linkerSettings.targetName;
        }
    }

    // If we got here no target name was defined so the only option left is the 
    // module name. For most projects this is the same thing.
    if ( useModuleNameIfNoTargetName )
    {
        return GetModuleNameAlways( moduleInfoEntry, targetPlatform );
    }

    return CORE::CString(); 
}

/*---------------------------------------------------------------------------*/

void
GetModuleDependencies( const TModuleInfoEntry& moduleInfoEntry ,
                       const CORE::CString& targetPlatform     ,
                       TStringSet& dependencies                ,
                       bool includeRuntimeDependencies         )
{GUCEF_TRACE;

    TModuleInfoEntry& mutableModuleInfoEntry = const_cast< TModuleInfoEntry& >( moduleInfoEntry );
    TModuleInfo* moduleInfo = FindModuleInfoForPlatform( mutableModuleInfoEntry, targetPlatform, false );
    if ( NULL != moduleInfo )
    {
        MergeStringSet( dependencies, moduleInfo->dependencies, false );
        if ( includeRuntimeDependencies )
            MergeStringSet( dependencies, moduleInfo->runtimeDependencies, false );
    }
    if ( targetPlatform != AllPlatforms && !targetPlatform.IsNULLOrEmpty() )
    {
        moduleInfo = FindModuleInfoForPlatform( mutableModuleInfoEntry, AllPlatforms, false );
        if ( NULL != moduleInfo )
        {
            MergeStringSet( dependencies, moduleInfo->dependencies, false );
            if ( includeRuntimeDependencies )
                MergeStringSet( dependencies, moduleInfo->runtimeDependencies, false );
        }
    }
}

/*---------------------------------------------------------------------------*/

bool
GetModuleDependencies( const TProjectInfo& projectInfo           ,
                       const TModuleInfoEntry& moduleInfoEntry   ,
                       const CORE::CString& targetPlatform       ,
                       TModuleInfoEntryConstPtrSet& dependencies ,
                       bool includeDependenciesOfDependencies    ,
                       bool includeRuntimeDependencies           )
{GUCEF_TRACE;

    TStringSet deps;
    GetModuleDependencies( moduleInfoEntry, targetPlatform, deps, includeRuntimeDependencies );  

    bool foundAllDeps = true;
    TStringSet::iterator m = deps.begin();
    while ( m != deps.end() )
    {
        const TModuleInfoEntry* dependency = GetModuleInfoEntry( projectInfo, (*m), targetPlatform );
        if ( GUCEF_NULL != dependency )
            dependencies.insert( dependency );
        else
        {
            foundAllDeps = false; // We cannot satisfy the full dependency chain for the executable for the given platform
        }
        ++m;
    }
    
    if ( includeDependenciesOfDependencies )
    {
        TModuleInfoEntryConstPtrSet depsOfdependencies;
        TModuleInfoEntryConstPtrSet::iterator i = dependencies.begin();
        while ( i != dependencies.end() )
        {
            const TModuleInfoEntry* entry = (*i);
            foundAllDeps = foundAllDeps && GetModuleDependencies( projectInfo, 
                                                                  *entry, 
                                                                  targetPlatform, 
                                                                  depsOfdependencies, 
                                                                  includeDependenciesOfDependencies, 
                                                                  includeRuntimeDependencies ); 
            ++i;
        }
        TModuleInfoEntryConstPtrSet::iterator n = depsOfdependencies.begin();
        while ( n != depsOfdependencies.end() )
        {
            dependencies.insert( (*n) );
            ++n;
        }
    }
    return foundAllDeps;
}

/*---------------------------------------------------------------------------*/

TModuleType
GetModuleType( const TModuleInfoEntry& moduleInfoEntry ,
               const CORE::CString& targetPlatform     )
{GUCEF_TRACE;

    const TModuleInfo* moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, targetPlatform );
    if ( NULL != moduleInfo )
    {
        if ( MODULETYPE_UNDEFINED != moduleInfo->moduleType )
        {
            return moduleInfo->moduleType;
        }
    }
    if ( targetPlatform != AllPlatforms && !targetPlatform.IsNULLOrEmpty() )
    {
        moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, AllPlatforms );
        if ( NULL != moduleInfo )
        {
            return moduleInfo->moduleType;
        }
    }

    // Since there is no specific info for the given platform and no AllPlatform info
    // we will see if we can derive from another if there is consensus
    std::set< TModuleType > typeSet;
    TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.begin();
    while ( i != moduleInfoEntry.modulesPerPlatform.end() )
    {
        const TModuleInfo& platformModuleInfo = (*i).second;
        if ( MODULETYPE_UNDEFINED != platformModuleInfo.moduleType )
            typeSet.insert( platformModuleInfo.moduleType );
        ++i;
    }
    if ( 1 == typeSet.size() )
    {
        return *typeSet.begin();
    }

    return MODULETYPE_UNDEFINED;
}

/*---------------------------------------------------------------------------*/

void
GetModuleInfoWithUniqueModulesTypes( const TModuleInfoEntry& moduleInfoEntry ,
                                     TConstModuleInfoPtrMap& moduleMap       )
{GUCEF_TRACE;

    // First try and get a 'AllPlatforms' definition which makes all the difference
    const TModuleInfo* moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, AllPlatforms );
    if ( NULL != moduleInfo && ( MODULETYPE_UNDEFINED != moduleInfo->moduleType ) )
    {
        // Since a 'AllPlatforms' definition is available we have a baseline to compare against
        // We will only add platform specific entries if they differ from our baseline
        TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.begin();
        while ( i != moduleInfoEntry.modulesPerPlatform.end() )
        {
            const CORE::CString& platformName = (*i).first;
            const TModuleInfo& platformModuleInfo = (*i).second;

            if ( ( MODULETYPE_UNDEFINED != platformModuleInfo.moduleType )  &&
                 ( moduleInfo->moduleType != platformModuleInfo.moduleType ) )
            {
                moduleMap[ platformName ] = &((*i).second);
            }
            ++i;
        }
        moduleMap[ AllPlatforms ] = moduleInfo;
    }
    else
    {
        // If we get here: We cannot filter in this case,.. just add all platforms
        TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.begin();
        while ( i != moduleInfoEntry.modulesPerPlatform.end() )
        {
            const CORE::CString& platformName = (*i).first;
            const TModuleInfo& platformModuleInfo = (*i).second;

            if ( MODULETYPE_UNDEFINED != platformModuleInfo.moduleType )
            {
                moduleMap[ platformName ] = &((*i).second);
            }
            ++i;
        }
    }
}

/*---------------------------------------------------------------------------*/

void
GetModuleInfoWithUniqueModuleNames( const TModuleInfoEntry& moduleInfoEntry ,
                                    TConstModuleInfoPtrMap& moduleMap       )
{GUCEF_TRACE;

    // First try and get a 'AllPlatforms' definition which makes all the difference
    const TModuleInfo* moduleInfo = FindModuleInfoForPlatform( moduleInfoEntry, AllPlatforms );
    if ( NULL != moduleInfo && !moduleInfo->name.IsNULLOrEmpty() )
    {
        // Since a 'AllPlatforms' definition is available we have a baseline to compare against
        // We will only add platform specific entries if they differ from our baseline
        TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.begin();
        while ( i != moduleInfoEntry.modulesPerPlatform.end() )
        {
            const CORE::CString& platformName = (*i).first;
            const TModuleInfo& platformModuleInfo = (*i).second;

            if ( ( !platformModuleInfo.name.IsNULLOrEmpty() )  &&
                 ( moduleInfo->name != platformModuleInfo.name ) )
            {
                moduleMap[ platformName ] = &((*i).second);
            }
            ++i;
        }
        moduleMap[ AllPlatforms ] = moduleInfo;
    }
    else
    {
        // If we get here: We cannot filter in this case,.. just add all platforms
        TModuleInfoMap::const_iterator i = moduleInfoEntry.modulesPerPlatform.begin();
        while ( i != moduleInfoEntry.modulesPerPlatform.end() )
        {
            const CORE::CString& platformName = (*i).first;
            const TModuleInfo& platformModuleInfo = (*i).second;

            if ( MODULETYPE_UNDEFINED != platformModuleInfo.moduleType )
            {
                moduleMap[ platformName ] = &((*i).second);
            }
            ++i;
        }
    }
}

/*---------------------------------------------------------------------------*/

const TModuleInfoEntry*
GetModuleInfoEntry( const TProjectInfo& projectInfo ,
                    const CORE::CString& moduleName ,
                    const CORE::CString& platform   ,
                    const TModuleInfo** moduleInfo  )
{GUCEF_TRACE;

    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        CORE::CString nameOfCurrentModule = GetModuleNameAlways( (*i), platform, moduleInfo );
        if ( nameOfCurrentModule == moduleName )
        {
            return &(*i);
        }
        ++i;
    }
    return NULL;
}

/*---------------------------------------------------------------------------*/

void
GetAllPlatformsUsed( const TProjectInfo& projectInfo ,
                     TStringSet& platformList        )
{GUCEF_TRACE;

    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        const TModuleInfoMap& modulesPerPlatform = (*i).modulesPerPlatform;
        TModuleInfoMap::const_iterator n = modulesPerPlatform.begin();
        while ( n != modulesPerPlatform.end() )
        {
            platformList.insert( (*n).first );
            ++n;
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
GetAllTagsUsed( const TProjectInfo& projectInfo ,
                TStringSet& tagsUsed            )
{GUCEF_TRACE;

    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        const TModuleInfoMap& modulesPerPlatform = (*i).modulesPerPlatform;
        TModuleInfoMap::const_iterator n = modulesPerPlatform.begin();
        while ( n != modulesPerPlatform.end() )
        {
            MergeStringSet( tagsUsed, (*n).second.tags, true );
            ++n;
        }
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

bool
IsModuleTagged( const TModuleInfoEntry& module ,
                const CORE::CString& tag       ,
                const CORE::CString& platform  )
{GUCEF_TRACE;

    TModuleInfoMap::const_iterator i = module.modulesPerPlatform.find( AllPlatforms );
    if ( i != module.modulesPerPlatform.end() )
    {
        if ( (*i).second.tags.find( tag ) != (*i).second.tags.end() )
            return true;
    }
    i = module.modulesPerPlatform.find( platform );
    if ( i != module.modulesPerPlatform.end() )
    {
        if ( (*i).second.tags.find( tag ) != (*i).second.tags.end() )
            return true;
    }
    return false;
}

/*---------------------------------------------------------------------------*/

void
GetTaggedModules( const TProjectInfo& projectInfo            ,
                  const CORE::CString& tag                   ,
                  TModuleInfoEntryConstPtrSet& taggedModules ,
                  const CORE::CString& platform              )
{GUCEF_TRACE;

    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        if ( IsModuleTagged( (*i), tag, platform ) )
            taggedModules.insert( &(*i) );
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

bool
ShouldModuleBeIgnored( const TModuleInfoEntry& moduleInfo ,
                       const CORE::CString& platformName  )
{GUCEF_TRACE;

    const TModuleInfoMap& modulesPerPlatform = moduleInfo.modulesPerPlatform;
    TModuleInfoMap::const_iterator i = modulesPerPlatform.find( platformName );
    if ( i != modulesPerPlatform.end() )
    {
        return (*i).second.ignoreModule;
    }
    i = modulesPerPlatform.find( AllPlatforms );
    if ( i != modulesPerPlatform.end() )
    {
        return (*i).second.ignoreModule;
    }
    return false;
}

/*---------------------------------------------------------------------------*/

bool
IsModuleTaggedWith( const TModuleInfoEntry& moduleInfo ,
                    const CORE::CString& platformName  ,
                    const CORE::CString& tag           )
{GUCEF_TRACE;

    const TModuleInfoMap& modulesPerPlatform = moduleInfo.modulesPerPlatform;
    TModuleInfoMap::const_iterator i = modulesPerPlatform.find( platformName );
    if ( i != modulesPerPlatform.end() )
    {
        if ( IsStringInList( (*i).second.tags, false, tag ) )
            return true;
    }
    i = modulesPerPlatform.find( AllPlatforms );
    if ( i != modulesPerPlatform.end() )
    {
        if ( IsStringInList( (*i).second.tags, false, tag ) )
            return true;
    }
    return false;
}

/*---------------------------------------------------------------------------*/

bool
HasIndependentModuleType( const TModuleInfoMap& moduleDefs )
{GUCEF_TRACE;

    TModuleInfoMap::const_iterator i = moduleDefs.begin();
    while ( i != moduleDefs.end() )
    {
        TModuleType moduleType = (*i).second.moduleType;

        if ( ( moduleType != MODULETYPE_HEADER_INCLUDE_LOCATION )   &&
             ( moduleType != MODULETYPE_HEADER_INTEGRATE_LOCATION ) &&
             ( moduleType != MODULETYPE_CODE_INTEGRATE_LOCATION )   &&
             ( moduleType != MODULETYPE_BINARY_PACKAGE )            &&
             ( moduleType != MODULETYPE_UNDEFINED )                 &&
             ( moduleType != MODULETYPE_UNKNOWN )                    )
        {
            return true;
        }
        ++i;
    }
    return false;
}

/*---------------------------------------------------------------------------*/

void
MergeStringVectorMap( TStringVectorMap& targetMap          ,
                      const TStringVectorMap& mapToMergeIn ,
                      bool caseSensitive                   )
{GUCEF_TRACE;

    TStringVectorMap::const_iterator i = mapToMergeIn.begin();
    while ( i != mapToMergeIn.end() )
    {
        MergeStringVector( targetMap[ (*i).first ], (*i).second, caseSensitive );
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
GetExecutables( const TProjectInfo& projectInfo                ,
                TModuleInfoEntryConstPtrSet& executableTargets ,
                const CORE::CString& platform                  )
{GUCEF_TRACE;

    TModuleInfoEntryVector::const_iterator i = projectInfo.modules.begin();
    while ( i != projectInfo.modules.end() )
    {
        if ( MODULETYPE_EXECUTABLE == GetModuleType( (*i), platform ) )
            executableTargets.insert( &(*i) );
        ++i;
    }
}

/*---------------------------------------------------------------------------*/

void
SplitProjectPerTarget( const TProjectInfo& projectInfo    ,
                       TProjectTargetInfoMapMap& targets  ,
                       bool tagsAsTargets                 ,
                       bool collapseRedundantPlatforms    )
{GUCEF_TRACE;

    TStringSet platformsUsed;
    GetAllPlatformsUsed( projectInfo, platformsUsed );

    TStringSet::iterator p = platformsUsed.begin();
    while ( p != platformsUsed.end() )
    {
        TModuleInfoEntryConstPtrSet executables;
        GetExecutables( projectInfo, executables, (*p) );

        TModuleInfoEntryConstPtrSet::iterator i = executables.begin();
        while ( i != executables.end() )
        {
            const TModuleInfoEntry& executable = *(*i);
            
            // Don't bother if the executable doesnt have a platform definition for the current platform            
            if ( executable.modulesPerPlatform.find( (*p) ) != executable.modulesPerPlatform.end() )
            {            
                TModuleInfoEntryConstPtrSet foundDependencies;
                CORE::CString targetName = GetModuleNameAlways( executable, (*p) );
                if ( GetModuleDependencies( projectInfo, executable, (*p), foundDependencies, true, true ) )
                {
                    // if we made it here we found the executable and were able to satisfy all dependencies
                    // for the current platform

                    CORE::CString projectName = projectInfo.projectName + "_exe_" + targetName; 
                    TProjectTargetInfoMap& targetPerPlatform = targets[ projectName ];
                    TProjectTargetInfo& target = targetPerPlatform[ (*p) ];

                    target.projectName = projectName;
                    target.modules.insert( &executable );
                    TModuleInfoEntryConstPtrSet::iterator j = foundDependencies.begin();
                    while ( j != foundDependencies.end() )
                    {
                        target.modules.insert( (*j) );
                        ++j;
                    }                
                }
                else
                {
                     GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "SplitProjectPerTarget: We cannot satisfy the full dependency chain for executable \"" + targetName + "\" for the given platform \"" + (*p) + "\"" );
                }
            }
            ++i;
        }
        ++p;
    }

    if ( tagsAsTargets )
    {
        TStringSet tagsUsed;
        GetAllTagsUsed( projectInfo, tagsUsed );

        TStringSet::iterator p = platformsUsed.begin();
        while ( p != platformsUsed.end() )
        {
            TStringSet::iterator i = tagsUsed.begin();
            while ( i != tagsUsed.end() )
            {
                TModuleInfoEntryConstPtrSet taggedModules;
                GetTaggedModules( projectInfo, (*i), taggedModules, (*p) );
                CORE::CString projectName = projectInfo.projectName + "_tag_" + (*i);

                TModuleInfoEntryConstPtrSet::iterator m = taggedModules.begin();
                while ( m != taggedModules.end() )
                {
                    const TModuleInfoEntry& taggedModule = *(*m);
                                            
                    // Don't include this module if it doesnt have a definition for the current platform
                    if ( taggedModule.modulesPerPlatform.find( (*p) ) != taggedModule.modulesPerPlatform.end() )
                    {                    
                        // Tagged or not we need to include the dependencies of tagged modules as well
                        // We don't want to make projects that cannot compile
                        TModuleInfoEntryConstPtrSet foundDependencies;
                        if ( GetModuleDependencies( projectInfo, taggedModule, (*p), foundDependencies, true, true ) )
                        {
                            TProjectTargetInfoMap& targetPerPlatform = targets[ projectName ];
                            TProjectTargetInfo& target = targetPerPlatform[ (*p) ];

                            target.projectName = projectName;
                            target.modules.insert( &taggedModule );
                            TModuleInfoEntryConstPtrSet::iterator j = foundDependencies.begin();
                            while ( j != foundDependencies.end() )
                            {
                                target.modules.insert( (*j) );
                                ++j;
                            }
                        }
                    }
                    ++m;
                }
                ++i;
            }
            ++p;
        }
    }

    if ( collapseRedundantPlatforms )
    {
        TProjectTargetInfoMapMap::iterator t = targets.begin();
        while ( t != targets.end() )
        {
            TProjectTargetInfoMap& targetByPlatform = (*t).second;
            TProjectTargetInfoMap::iterator a = targetByPlatform.find( AllPlatforms );
            if ( a != targetByPlatform.end() )
            {
                TProjectTargetInfo& allPlatformsTarget = (*a).second;
                
                // We now check to see if the modules match across platforms which is all that is needed here
                // The modules themselves will deal with platform specifics at an intra-module level
                // The use-case we look for are cases where some platforms have different modules then others
                // in which case we need to keep them as distinct targets
                TStringSet redundantPlatforms; 
                TProjectTargetInfoMap::iterator m = targetByPlatform.begin();
                while (  m != targetByPlatform.end() )
                {
                     const CORE::CString& currentPlatform = (*m).first;
                     if ( currentPlatform != AllPlatforms )
                     {
                         TProjectTargetInfo& somePlatformTarget = (*m).second;
                         if ( somePlatformTarget.modules == allPlatformsTarget.modules )
                            redundantPlatforms.insert( (*m).first );
                    }
                    ++m;
                }
                TStringSet::iterator r = redundantPlatforms.begin();
                while ( r != redundantPlatforms.end() )
                {
                    targetByPlatform.erase( (*r) );
                    ++r;
                }
            }
            // else: targets that don't have a 'AllPlatforms' target cannot be collapsed
            ++t;
        }
    }

    // In order to facilitate uniform processing we also include the complete project as its own target
    // This ensures that backend code doesnt need different code to process the complete project vs some
    // target based subset
    // Note that the full project is by definition "all" platforms because there is no target differntiation
    // It relies soley on module level per-platform differenes to be processed

    TProjectTargetInfoMap& fullProjectTargets = targets[ projectInfo.projectName ];
    TProjectTargetInfo& fullProjectTarget = fullProjectTargets[ AllPlatforms ];
    fullProjectTarget.projectName = projectInfo.projectName;
    TModuleInfoEntryVector::const_iterator w = projectInfo.modules.begin();
    while ( w != projectInfo.modules.end() )
    {
        fullProjectTarget.modules.insert( &(*w) );
        ++w;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGEN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
