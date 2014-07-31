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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#include "ProjectGenVSImporter_CDirPreprocessor.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PROJECTGEN {
namespace VSIMPORTER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDirPreprocessor::CDirPreprocessor( void )
    : CIDirPreprocessor() ,
      m_xmlCodec()
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/
    
CDirPreprocessor::CDirPreprocessor( const CDirPreprocessor& src )
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/
    
CDirPreprocessor::~CDirPreprocessor()
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/
    
CDirPreprocessor&
CDirPreprocessor::operator=( const CDirPreprocessor& src )
{GUCEF_TRACE;

    return *this;
}

/*--------------------------------------------------------------------------*/

bool
CDirPreprocessor::InitXmlParser( void )
{GUCEF_TRACE;

    if ( m_xmlCodec.IsNULL() )
    {
        CORE::CDStoreCodecRegistry* registry = &CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry();
        if ( registry->TryLookup( "XML", m_xmlCodec, false ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Requested for data storage codec for xml file, succesfully obtained codec" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Requested for data storage codec for xml file, no such codec is available" );
            CORE::ShowErrorMessage( "Missing codec support",
                                    "Request for data storage codec for xml file but such codec is available!" );
        }
    }
    return !m_xmlCodec.IsNULL();
}

/*--------------------------------------------------------------------------*/

bool
CDirPreprocessor::FindProjectFiles( const CORE::CString& path ,
                                    TStringList& list         )
{GUCEF_TRACE;

    struct CORE::SDI_Data* di = CORE::DI_First_Dir_Entry( path.C_String() );
    if ( NULL != di )
    {
        do
        {
            if ( 0 != DI_Is_It_A_File( di ) )
            {
                CORE::CString name = DI_Name( di );                
                if ( "vcxproj" == CORE::ExtractFileExtention( name ).Lowercase() )
                {
                    list.push_back( name );
                }
            }
        }
        while ( 0 != CORE::DI_Next_Dir_Entry( di ) );
        CORE::DI_Cleanup( di );
    }

    return !list.empty();
}

/*--------------------------------------------------------------------------*/

PROJECTGEN::TModuleType
CDirPreprocessor::StudioConfigurationTypeToModuleType( const CORE::CString& configType )
{GUCEF_TRACE;

    if ( "StaticLibrary" == configType )
    {
        return PROJECTGEN::MODULETYPE_STATIC_LIBRARY;
    }
    else
    if ( "Application" == configType )
    {
        return PROJECTGEN::MODULETYPE_EXECUTABLE; 
    }
    else
    if ( "DynamicLibrary" == configType )
    {
        return PROJECTGEN::MODULETYPE_SHARED_LIBRARY;
    }
    return PROJECTGEN::MODULETYPE_UNKNOWN;
}

/*--------------------------------------------------------------------------*/

void
CDirPreprocessor::ParseGlobalVars( const CORE::CDataNode& projectData ,
                                   TStringMap& globals                )
{GUCEF_TRACE;

    CORE::CDataNode::TConstDataNodeSet nodes = projectData.FindChildrenOfType( "PropertyGroup", true );
    CORE::CDataNode::TConstDataNodeSet::const_iterator n = nodes.begin();
    while ( n != nodes.end() )
    {
        CORE::CString labelType = (*n)->GetAttributeValue( "Label" );
        if ( "Globals" == labelType )
        {
            CORE::CDataNode::const_iterator m = (*n)->Begin();
            while ( m != (*n)->End() )
            {
                globals[ (*m)->GetName() ] = (*m)->GetValue();                
                ++m;
            }
        }
        ++n;
    }
}

/*--------------------------------------------------------------------------*/

void
CDirPreprocessor::ParseVisualStudioVariables( const CORE::CString& testStr ,
                                              TStringSet& vars             )
{GUCEF_TRACE;

    CORE::Int32 i=0;
    CORE::Int32 subStrStart = -1;

    do
    {
        subStrStart = testStr.HasSubstr( "$(", i, true );
        if ( subStrStart >= 0 )
        {
            CORE::Int32 subStrEnd = testStr.HasChar( ')', i+2, true );
            if ( i+2 < subStrEnd )
            {
                vars.insert( testStr.SubstrFromRange( i+2, subStrEnd-1 ) );
                i = subStrEnd + 1;
            }
        }

    }
    while ( subStrStart >= 0 );
}

/*--------------------------------------------------------------------------*/

CORE::CString
CDirPreprocessor::ReplaceVisualStudioVariables( const CORE::CString& testStr ,
                                                const TStringMap& globals    )
{GUCEF_TRACE;

    CORE::CString resultStr = testStr;
    
    TStringMap::const_iterator i = globals.begin();
    while ( i != globals.end() )
    {
        CORE::CString varStr = "$(" + (*i).first + ')';        
        resultStr = resultStr.ReplaceSubstr( varStr, (*i).second );
        
        ++i;
    }

    // remaining variables are likely to be environment variables
    TStringSet remainingVars;
    ParseVisualStudioVariables( resultStr, remainingVars );

    TStringSet::iterator n = remainingVars.begin();
    while ( n != remainingVars.end() )
    {       
        //CORE::CString envVarValue = CORE::GUCEFGetEnv( (*n).C_String() );        
        //if ( !envVarValue.IsNULLOrEmpty() )
        //{
        //    CORE::CString varStr = "$(" + (*n) + ')';
        //    resultStr = resultStr.ReplaceSubstr( varStr, envVarValue );
        //}

        // Let the generator put in lookup code instead of resolving the variable at this time
        resultStr = resultStr.ReplaceSubstr( "$(" + (*n) + ')', "$ENVVAR:" + (*n) + "$" );
        resultStr = resultStr.ReplaceSubstr( "%(" + (*n) + ')', "$ENVVAR:" + (*n) + "$" );
        
        ++n;
    }

    return resultStr;
}

/*--------------------------------------------------------------------------*/

bool
CDirPreprocessor::ProccessProjectFiles( const CORE::CString& path ,
                                        TStringList& list         )
{GUCEF_TRACE;
    
    CORE::UInt32 errorCount = 0;
    
    TStringList::iterator i = list.begin();
    while ( i != list.end() )
    {
        CORE::CString projectFilePath = CORE::CombinePath( path, (*i) );
        CORE::CDataNode rootNode;
        if( !m_xmlCodec->BuildDataTree( &rootNode, projectFilePath ) ) { ++errorCount; continue; }

        PROJECTGEN::TModuleInfoEntry moduleEntry;
        InitializeModuleInfoEntry( moduleEntry );
        moduleEntry.rootDir = path;

        PROJECTGEN::TModuleInfo& moduleInfo = moduleEntry.modulesPerPlatform[ "all" ];
        InitializeModuleInfo( moduleInfo );        
        
        // First parse the globals so we can resolve variables in other sections
        TStringMap globals;
        ParseGlobalVars( rootNode, globals );
        
        CORE::CDataNode::TDataNodeSet nodes = rootNode.FindChildrenOfType( "PropertyGroup", true );
        CORE::CDataNode::TDataNodeSet::iterator n = nodes.begin();
        while ( n != nodes.end() )
        {
            CORE::CString labelType = (*n)->GetAttributeValue( "Label" );
            if ( "Globals" == labelType )
            {
                CORE::CString rootName = (*n)->GetChildValueByName( "RootNamespace" );
                if ( !rootName.IsNULLOrEmpty() )
                {
                    moduleInfo.name = rootName;
                }
            }
            else
            if ( "Configuration" == labelType )
            {
                CORE::CString cTypeStr = (*n)->GetChildValueByName( "ConfigurationType" );
                if ( !cTypeStr.IsNULLOrEmpty() )
                {
                    PROJECTGEN::TModuleType configurationType = StudioConfigurationTypeToModuleType( cTypeStr );
                    if ( PROJECTGEN::MODULETYPE_UNDEFINED == moduleInfo.moduleType )
                    {
                        moduleInfo.moduleType = configurationType;
                    }
                    else
                    if ( PROJECTGEN::MODULETYPE_UNKNOWN != configurationType && PROJECTGEN::MODULETYPE_UNKNOWN == moduleInfo.moduleType )
                    {
                        moduleInfo.moduleType = configurationType;
                    }
                }                
            }
            ++n;
        }

        nodes = rootNode.FindChildrenOfType( "ItemDefinitionGroup", true );
        n = nodes.begin();
        while ( n != nodes.end() )
        {
            CORE::CDataNode* clCompileNode = (*n)->FindChild( "ClCompile" );
            if ( NULL != clCompileNode )
            {                
                PROJECTGEN::TStringSet newDefines = PROJECTGEN::StringVectorToStringSet( clCompileNode->GetChildValueByName( "PreprocessorDefinitions" ).ParseElements( ';', false ) );
                PROJECTGEN::TStringSet::iterator m = newDefines.begin();
                while ( m != newDefines.end() ) 
                {
                    moduleInfo.preprocessorSettings.defines.insert( ReplaceVisualStudioVariables( (*m), globals ) ); 
                    ++m;
                }

                PROJECTGEN::TStringSet additionalIncludeDirs = PROJECTGEN::StringVectorToStringSet( clCompileNode->GetChildValueByName( "AdditionalIncludeDirectories" ).ParseElements( ';', false ) );
                m = additionalIncludeDirs.begin();
                while ( m != additionalIncludeDirs.end() ) 
                {
                    moduleInfo.includeDirs[ ReplaceVisualStudioVariables( (*m), globals ) ];
                    ++m;
                }
            }
            CORE::CDataNode* linkNode = (*n)->FindChild( "Link" );
            if ( NULL != linkNode )
            {
                //PROJECTGEN::TStringSet newLibDirs = PROJECTGEN::StringVectorToStringSet( (*n)->GetChildValueByName( "AdditionalLibraryDirectories" ).ParseElements( ';', false ) );
                //moduleInfo.linkerSettings.linkedLibraries[ ];

                PROJECTGEN::TStringVector additionalDependencies = linkNode->GetChildValueByName( "AdditionalDependencies" ).ParseElements( ';', false );
                if ( !additionalDependencies.empty() )
                {
                    PROJECTGEN::TStringVector::iterator m = additionalDependencies.begin();
                    while ( m != additionalDependencies.end() )
                    {
                        moduleInfo.linkerSettings.linkedLibraries[ ReplaceVisualStudioVariables( (*m), globals ) ] = PROJECTGEN::MODULETYPE_UNKNOWN;
                        ++m;
                    }
                }                
            }
            ++n;
        }

        nodes = rootNode.FindChildrenOfType( "ItemGroup", true );
        n = nodes.begin();
        while ( n != nodes.end() )
        {
            CORE::CDataNode::TDataNodeSet fileNodes = (*n)->FindChildrenOfType( "ClInclude", true );
            CORE::CDataNode::TDataNodeSet::iterator m = fileNodes.begin();
            while ( m != fileNodes.end() )
            {
                CORE::CString relPath = (*m)->GetAttributeValue( "Include" );
                if ( 0 != relPath.Length() )
                {
                    CORE::CString includeFilename = CORE::ExtractFilename( relPath );
                    relPath = relPath.CutChars( includeFilename.Length(), false );
                    relPath = ReplaceVisualStudioVariables( relPath, globals );
                    moduleInfo.includeDirs[ relPath ].push_back( includeFilename );
                }
                ++m;
            }

            fileNodes = (*n)->FindChildrenOfType( "ClCompile", true );
            m = fileNodes.begin();
            while ( m != fileNodes.end() )
            {
                CORE::CString relPath = (*m)->GetAttributeValue( "Include" );
                if ( 0 != relPath.Length() )
                {
                    CORE::CString sourceFilename = CORE::ExtractFilename( relPath );
                    relPath = relPath.CutChars( sourceFilename.Length(), false );
                    relPath = ReplaceVisualStudioVariables( relPath, globals );
                    moduleInfo.sourceDirs[ relPath ].push_back( sourceFilename );
                }
                ++m;
            }

            fileNodes = (*n)->FindChildrenOfType( "ProjectReference", true );
            m = fileNodes.begin();
            while ( m != fileNodes.end() )
            {
                CORE::CString relPath = (*m)->GetAttributeValue( "Include" );
                if ( 0 != relPath.Length() )
                {
                    CORE::CString moduleDependency = CORE::ExtractFilename( relPath );
                    CORE::Int32 dotIndex = moduleDependency.HasChar( '.', false );
                    if ( 0 <= dotIndex ) moduleDependency = moduleDependency.CutChars( moduleDependency.Length() - dotIndex, false );
                    moduleDependency = ReplaceVisualStudioVariables( moduleDependency, globals );
                    moduleInfo.dependencies.push_back( moduleDependency );
                }
                ++m;
            }            

            ++n;
        }

        if ( moduleInfo.name.IsNULLOrEmpty() )
        {
            CORE::Int32 dotIndex = (*i).HasChar( '.', false );
            if ( 0 <= dotIndex )
            {
                moduleInfo.name = (*i).CutChars( (*i).Length() - dotIndex, false );
            }
            else
            {
                moduleInfo.name = (*i);
            }
        }

        CORE::CString moduleInfoFilePath = CORE::CombinePath( path, "ModuleInfo.xml" );

        moduleEntry.lastEditBy = "ProjectGenVSImporter";
        PROJECTGEN::SerializeModuleInfo( moduleEntry, moduleInfoFilePath );

        ++i;
    }

    return 0 == errorCount;
}

/*--------------------------------------------------------------------------*/
    
bool
CDirPreprocessor::ProccessDir( const CORE::CString& path )
{GUCEF_TRACE;

    // Find and and all project files in the dir
    TStringList projectFileList;
    if ( !FindProjectFiles( path, projectFileList ) ) return true;

    // Since we found one or more,.. we will need the xml parser
    if ( !InitXmlParser() ) return false;

    return ProccessProjectFiles( path, projectFileList );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VSIMPORTER */
}; /* namespace PROJECTGEN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
