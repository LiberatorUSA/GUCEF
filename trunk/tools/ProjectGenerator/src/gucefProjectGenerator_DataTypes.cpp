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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefProjectGenerator_DataTypes.h"

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

static void
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

static void
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

static void
MergeStringVectorMap( TStringVectorMap& targetList          ,
                      const TStringVectorMap& listToMergeIn ,
                      bool caseSensitive                    )
{GUCEF_TRACE;

    TStringVectorMap::const_iterator i = listToMergeIn.begin();
    while ( i != listToMergeIn.end() )
    {
        MergeStringVector( targetList[ (*i).first ], (*i).second, caseSensitive ); 
        ++i;
    }
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
    if ( moduleTypeString == "unknown" ) return MODULETYPE_UNKNOWN;
    return MODULETYPE_UNDEFINED;
}

/*-------------------------------------------------------------------------*/

static CORE::CDStoreCodecRegistry::TDStoreCodecPtr
GetXmlDStoreCodec( void )
{GUCEF_TRACE;

    static CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr;
    if ( codecPtr.IsNULL() )
    {
        CORE::CDStoreCodecRegistry* registry = CORE::CDStoreCodecRegistry::Instance();
        if ( !registry->TryLookup( "XML", codecPtr, false ) )
        {
            // No codec is registered to handle XML, try and load a plugin for it
            #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

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
    if ( moduleInfo.moduleType != MODULETYPE_UNDEFINED )
    {
        moduleInfoNode.SetAttribute( "Type", ModuleTypeToString( moduleInfo.moduleType ) );
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
    }
    
    CORE::CDataNode linkerNode;
    linkerNode.SetName( "Linker" );
    bool addedLinkedSettings = false;

    // Now Serialize all linker related info
    // First add all the libraries that are linked but not part of the overall project
    if ( moduleInfo.linkerSettings.linkedLibraries.size() > 0 )
    {        
        addedLinkedSettings = true;
        TStringVector::const_iterator m = moduleInfo.linkerSettings.linkedLibraries.begin();
        while ( m != moduleInfo.linkerSettings.linkedLibraries.end() )
        {
            CORE::CDataNode libraryNode;
            libraryNode.SetName( "Dependency" );
            libraryNode.SetAttribute( "Name", (*m) );
            linkerNode.AddChild( libraryNode );
            ++m;
        }
        
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
    if ( NULL != codec )
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
SerializeProjectInfo( const TProjectInfo& projectInfo     ,
                      const CORE::CString& outputFilepath )
{GUCEF_TRACE;

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec = GetXmlDStoreCodec();
    if ( NULL != codec )
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
DeserializeModuleInfo( TModuleInfo& moduleInfo           ,
                       const CORE::CDataNode& parentNode )
{GUCEF_TRACE;

    const CORE::CDataNode* moduleInfoNode = parentNode.Find( "Module" );
    if ( moduleInfoNode == NULL ) return false;
    
    // Find the overall module properties
    CORE::CString tmpStr = moduleInfoNode->GetAttributeValue( "BuildOrder" );
    if ( !tmpStr.IsNULLOrEmpty() )
    {
        moduleInfo.buildOrder = CORE::StringToInt32( tmpStr );
    }
    else
    {
        moduleInfo.buildOrder = -1;
    }
    moduleInfo.moduleType = StringToModuleType( moduleInfoNode->GetAttributeValue( "Type" ) );
    
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
                moduleInfo.dependencies.push_back( dependencyName );
            }
            ++i;
        }
    }
    
    // Go through all linker related settings, if any exist
    const CORE::CDataNode* linkerNode = moduleInfoNode->Find( "Linker" );
    if ( NULL != linkerNode )
    {
        // Find all the libraries that are linked but not part of the overall project
        CORE::CDataNode::TConstDataNodeSet linkedLibs = linkerNode->FindChildrenOfType( "Dependency" );
        i = linkedLibs.begin();
        while ( i != linkedLibs.end() )
        {
            const CORE::CDataNode* linkedLibNode = (*i);
            CORE::CString linkedLibName = linkedLibNode->GetAttributeValue( "Name" );
            if ( !linkedLibName.IsNULLOrEmpty() )
            {
                moduleInfo.linkerSettings.linkedLibraries.push_back( linkedLibName );
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
                moduleInfo.preprocessorSettings.defines.push_back( defineValue );
            }
            ++i;
        }
    }

   // Go through all preprocessor related settings, if any exist
    const CORE::CDataNode* compilerNode = moduleInfoNode->Find( "Compiler" );
    if ( NULL != compilerNode )
    {
        // Find all the preprocessor definitions
        CORE::CDataNode::TConstDataNodeSet languages = compilerNode->FindChildrenOfType( "Language" );
        i = languages.begin();
        while ( i != languages.end() )
        {
            const CORE::CDataNode* languageNode = (*i);
            CORE::CString name = languageNode->GetAttributeValue( "Name" );
            if ( !name.IsNULLOrEmpty() )
            {
                moduleInfo.compilerSettings.languagesUsed.push_back( name );
            }
            ++i;
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
DeserializeModuleInfo( TModuleInfoEntry& moduleInfoEntry ,
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
        CORE::CString platform = moduleNode->GetAttributeValue( "Platform" ).Lowercase();
        if ( platform.IsNULLOrEmpty() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to locate a Platform value for a module, will default to all platforms but this may not be correct" );
            platform = AllPlatforms;
        }
        
        if ( DeserializeModuleInfo( moduleInfoForPlatform ,
                                    *moduleNode           ) )
        {
            // Special case handling for win32 vs win64 
            // When mswin is specified we add the platform to both win32 and win64
            // This just makes it less effort for people to specify modules for mswin
            if ( "mswin" == platform )
            {
                moduleInfoEntry.modulesPerPlatform[ "win32" ] = moduleInfoForPlatform;
                moduleInfoEntry.modulesPerPlatform[ "win64" ] = moduleInfoForPlatform;
            }
            else
            {
                moduleInfoEntry.modulesPerPlatform[ platform ] = moduleInfoForPlatform;
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
DeserializeModuleInfo( TModuleInfoEntry& moduleInfoEntry  ,
                       const CORE::CString& inputFilepath )
{GUCEF_TRACE;

    CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec = GetXmlDStoreCodec();
    if ( NULL != codec )
    {
        CORE::CDataNode rootNode;        
        if ( codec->BuildDataTree( &rootNode, inputFilepath ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Successfully loaded module information from file \"" + inputFilepath + "\", now we will parse the information" );
            return DeserializeModuleInfo( moduleInfoEntry ,
                                          rootNode        );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "DeserializeModuleInfo: Failed to load the module information from file at " + inputFilepath );
            return false;
        }
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "SerializeProjectInfo: Cannot serialize since no codec is registered that can be used for serialization" );
    return false;
}

/*-------------------------------------------------------------------------*/

void
InitializeModuleInfo( TModuleInfo& moduleInfo )
{GUCEF_TRACE;

    // reset all fields
    moduleInfo.buildOrder = -1;
    moduleInfo.name.Clear();
    moduleInfo.includeDirs.clear();
    moduleInfo.sourceDirs.clear();
    moduleInfo.dependencyIncludeDirs.clear();
    moduleInfo.linkerSettings.linkedLibraries.clear();
    moduleInfo.compilerSettings.languagesUsed.clear();
    moduleInfo.preprocessorSettings.defines.clear();
    moduleInfo.dependencies.clear();
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
    if ( !moduleInfoToMergeIn.name.IsNULLOrEmpty() )
    {
        targetModuleInfo.name = moduleInfoToMergeIn.name;
    }
    if ( moduleInfoToMergeIn.moduleType != MODULETYPE_UNDEFINED )
    {
        targetModuleInfo.moduleType = moduleInfoToMergeIn.moduleType;
    }
    
    // Now combine the other items without overwriting    
    MergeStringVector( targetModuleInfo.compilerSettings.languagesUsed    ,
                       moduleInfoToMergeIn.compilerSettings.languagesUsed ,
                       false                                              );
    MergeStringVector( targetModuleInfo.preprocessorSettings.defines    ,
                       moduleInfoToMergeIn.preprocessorSettings.defines ,
                       true                                             );
    MergeStringVector( targetModuleInfo.linkerSettings.linkedLibraries    ,
                       moduleInfoToMergeIn.linkerSettings.linkedLibraries ,
                       false                                              );
    MergeStringVector( targetModuleInfo.dependencies    ,
                       moduleInfoToMergeIn.dependencies ,
                       false                            );
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
        if ( ( NULL != allPlatformsInfo ) && ( NULL != targetPlatformInfo ) )
        {
            // Use the 'all' plaform as a base to work from
            mergedModuleInfo = *allPlatformsInfo;
            
            // Now merge in the platform specific info
            MergeModuleInfo( mergedModuleInfo, *targetPlatformInfo );
            
            return true;
        }
        else
        if ( ( NULL != allPlatformsInfo ) && ( NULL == targetPlatformInfo ) )
        {
            // We only have the 'all' platform which is fine, we will just use that
            mergedModuleInfo = *allPlatformsInfo;
            return true;
        }
        else
        if ( ( NULL == allPlatformsInfo ) && ( NULL != targetPlatformInfo ) )
        {
            // We only have the target platform which is fine, we will just use that
            // this module aparently is not available for all platforms even in altered form
            mergedModuleInfo = *targetPlatformInfo;
            return true;
        }        
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
MergeAllModuleInfoForPlatform( const TModuleInfoEntryVector& allInfo  ,
                               const CORE::CString& platform          ,
                               TModuleInfoVector& allMergedInfo       ,
                               TModuleInfoEntryPairVector& mergeLinks )
{GUCEF_TRACE;

    allMergedInfo.clear();
    
    TModuleInfoEntryVector::const_iterator i = allInfo.begin();
    while ( i != allInfo.end() )
    {
        // For each module we create a merged platform specific module
        // description which is easy to process if you only care about that platform
        TModuleInfo mergedInfo;
        MergeModuleInfo( (*i), platform, mergedInfo );
        
        // Store the merged info
        allMergedInfo.push_back( mergedInfo );        
        ++i;
    }

    i = allInfo.begin();
    TModuleInfoVector::iterator n = allMergedInfo.begin();
    while ( ( i != allInfo.end() )       && 
            ( n != allMergedInfo.end() )  )
    {
        // Store a link between the merged info and the original info
        TModuleInfoEntryPair mergeLink( &(*i), &(*n) );
        mergeLinks.push_back( mergeLink );
        ++i; ++n;
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
        // A name was specified for this platform
        if ( NULL != moduleInfo )
        {
            *moduleInfo = &(*n).second;
        }
        return &(*n).second.name;
    }
    else
    {
        // If no name is specified for a specific platform then there might still be a 
        // default for all platforms
        if ( targetPlatform != AllPlatforms )
        {
            n = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
            if ( n != moduleInfoEntry.modulesPerPlatform.end() )
            {
                // A default name was specified for this module
                if ( NULL != moduleInfo )
                {
                    *moduleInfo = &(*n).second;
                }
                return &(*n).second.name;
            }
        }            
    }
    return NULL;
}

/*---------------------------------------------------------------------------*/

CORE::CString
GetConsensusModuleName( const TModuleInfoEntry& moduleInfoEntry )
{GUCEF_TRACE;

    TModuleInfoMap::const_iterator n = moduleInfoEntry.modulesPerPlatform.find( AllPlatforms );
    if ( n != moduleInfoEntry.modulesPerPlatform.end() )
    {
        // A name was specified for all platforms which makes our job easy
        // an all platform name always counts as the general consensus name
        return (*n).second.name;
    }
    else
    {
        // If no name is specified for all platforms then we will have to 
        // determine the best name to use. We do this by getting the name 
        // for all platforms and counting how often each is used. The most used
        // name is considered the general consensus name. If the same count applies
        // to multiple we will try to use a popular platform to improve our 'guess'
        
        typedef std::map< CORE::CString, CORE::UInt32 > TStringCountMap;
        
        TStringCountMap countMap;
        TModuleInfoMap::const_iterator n = moduleInfoEntry.modulesPerPlatform.begin();
        while ( n != moduleInfoEntry.modulesPerPlatform.end() )
        {
            const TModuleInfo& moduleInfo = (*n).second;
            TStringCountMap::iterator m = countMap.find( moduleInfo.name );
            if ( m != countMap.end() )
            {
                ++((*m).second);
            }
            else
            {
                countMap[ moduleInfo.name ] = 1;
            }            
            ++n;
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
        
        return (*topNames.begin());
        
    }
    return CORE::CString();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PROJECTGENERATOR */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
